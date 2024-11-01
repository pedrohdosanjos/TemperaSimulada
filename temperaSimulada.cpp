#include "./headers/listaItens.hpp"
#include "./headers/dataHandler.hpp"
#include <string>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <sstream>

int CAPACIDADE = 101;

double funcaoTemperaturaLinear(int iteracao, double temperaturaInicial)
{
    int maxIter = 3000;
    return (temperaturaInicial / maxIter) * (maxIter - iteracao);
}

double funcaoTemperaturaExponencial(int iteracao, double temperaturaInicial)
{
    double alpha = 0.996; // fator de decaimento
    return temperaturaInicial * pow(alpha, iteracao);
}

void calcularPesoValor(const std::vector<int> &vetor, const ListaItens &lista, int &peso, int &valor)
{
    peso = 0;
    valor = 0;

    const auto &itens = lista.getItens();

    for (size_t i = 0; i < vetor.size(); ++i)
    {
        if (vetor[i] == 1)
        {
            peso += itens[i].peso;   // Soma o peso
            valor += itens[i].valor; // Soma o valor
        }
    }

    if (peso > CAPACIDADE)
    {
        valor = 0;
    }
}

int temperaSimulada(double temperatura, bool lin_exp, int init_rand, DataHandler handler)
{
    std::srand(std::time(0));

    ListaItens lista;    // Cria a lista de itens a partir do arquivo
    lista.exibirItens(); // Exibe os itens lidos

    int size = lista.getSize(); // Retorna o tamanho da lista

    std::vector<int> itensSelecionados(size);

    if (init_rand == 0)
    { // Inicia com 0's
        for (int i = 0; i < size; i++)
        {
            itensSelecionados[i] = 0;
        }
    }

    else if (init_rand == 1)
    { // Inicia com 1's
        for (int i = 0; i < size; i++)
        {
            itensSelecionados[i] = 1;
        }
    }

    else if (init_rand == 2)
    { // Gera uma solução inicial aleatória
        for (int i = 0; i < size; i++)
        {
            itensSelecionados[i] = rand() % 2; // Gera 0 ou 1
        }
    }
    std::cout << "\nSolucao inicial: ";

    handler.imprimeItens(itensSelecionados);

    // Calcula o peso e valor inicial
    int peso = 0, valor = 0;
    calcularPesoValor(itensSelecionados, lista, peso, valor);

    std::cout << "\nValor inicial: " << valor << "\nPeso inicial: " << peso << std::endl;

    // Armazena a melhor solução
    int melhorValor = valor;
    int melhorPeso = peso;
    int melhorIteracao = 0;

    double temperaturaVariavel = temperatura;
    double temperaturaInicial = temperatura;

    for (int i = 0; temperaturaVariavel > 0.01; i++)
    {
        if (lin_exp == 0)
            // Resfriamento da temperatura
            temperaturaVariavel = funcaoTemperaturaLinear(i, temperaturaInicial);

        else if (lin_exp == 1)
            temperaturaVariavel = funcaoTemperaturaExponencial(i, temperaturaInicial);

        else
        {
            std::cerr << "Erro ao escolher a função de resfriamento." << std::endl;
            return 1;
        }

        system("clear");

        // Gera uma solução vizinha
        std::vector<int> vizinho = itensSelecionados;
        int pos = rand() % size;         // Escolhe uma posição aleatória
        vizinho[pos] = 1 - vizinho[pos]; // Troca 0 por 1 ou 1 por 0

        // Calcula o peso e valor do vizinho
        int pesoVizinho = 0, valorVizinho = 0;
        calcularPesoValor(vizinho, lista, pesoVizinho, valorVizinho);

        std::cout << "Candidato: \n";
        handler.imprimeItens(vizinho);

        std::cout << "Valor: " << valorVizinho << "\nPeso: " << pesoVizinho << "\n"
                  << std::endl;

        int delta = valorVizinho - valor;

        // Verifica se aceita a nova solução
        if (delta > 0)
        {
            itensSelecionados = vizinho;
            valor = valorVizinho;
            peso = pesoVizinho;
        }

        else
        {
            // Aceita a nova solução pior com uma probabilidade
            double randomProb = rand() / double(RAND_MAX);         // Número aleatório entre 0 e 1
            double probAceitar = exp(delta / temperaturaVariavel); // Fórmula da Simulated Annealing

            if (randomProb < probAceitar)
            {
                // Aceita a nova solução mesmo sendo pior
                itensSelecionados = vizinho;
                valor = valorVizinho;
                peso = pesoVizinho;
            }
        }

        // Atualiza a melhor solução
        if (valor > melhorValor)
        {
            melhorValor = valor;
            melhorPeso = peso;
            melhorIteracao = i;
        }

        handler.imprimeItens(itensSelecionados);
        // Exibe a solução atual
        std::cout << "Iteracao " << i << ":\nValor: " << valor << "\nPeso: " << peso << "\n"
                  << std::endl;

        std::cout << "Temperatura: " << temperaturaVariavel << std::endl;
    }

    std::cout << "\nA melhor solucao foi encontrada na iteracao " << melhorIteracao
              << " com valor " << melhorValor << " e peso " << melhorPeso << "\n"
              << std::endl;

    handler.salvarEmArquivo(melhorValor, melhorPeso, melhorIteracao, valor, peso);

    return 0;
}

int execute(double temperatura, bool lin_exp, int init_rand, int execucoes, DataHandler handler)

{

    std::ofstream arquivoParcial("./solucoesParciais.txt");
    if (arquivoParcial.is_open())
    {
        arquivoParcial << "Temperatura inicial: " << temperatura << " | População inicial " << init_rand << "\nFunção de resfriamento " << lin_exp << " | " << execucoes << " execuções" << "\n";
        arquivoParcial.close();
    }
    else
        std::cerr << "Erro ao abrir o arquivo." << std::endl;

    for (int i = 0; i < execucoes; i++)
    {
        temperaSimulada(temperatura, lin_exp, init_rand, handler);
    }

    std::vector<Data> dados = handler.lerArquivo("./solucoesParciais.txt");

    if (!dados.empty())
    {
        handler.analisarDados(dados);
    }
    else
    {
        std::cerr << "Nenhum dado encontrado no arquivo." << std::endl;
    }

    handler.copiarArquivo("./solucoesParciais.txt", "./solucoes.txt");

    return 0;
}

int main()
{
    DataHandler handler;
    // temperatura inicial, linear(0) ou exponencial(1), iniciar com 0's(0) com 1's(1) ou população aleatória(2)
    execute(10000.0, 1, 1, 1000, handler);

    return 0;
}