#include "./headers/listaItens.hpp"
#include <string>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

int CAPACIDADE = 101;

void imprimeItens(const std::vector<int> &vetor)
{
    for (size_t i = 0; i < vetor.size(); ++i)
    {
        std::cout << vetor[i] << " ";
    }
    std::cout << std::endl;
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

int main()
{
    std::srand(std::time(0));

    ListaItens lista;    // Cria a lista de itens a partir do arquivo
    lista.exibirItens(); // Exibe os itens lidos

    int size = lista.getSize(); // Retorna o tamanho da lista

    std::cout << "\nTamanho da lista: " << lista.getSize() << std::endl;

    // Gera uma solução inicial aleatória
    std::vector<int> itensSelecionados(size);
    for (int i = 0; i < size; i++)
    {
        itensSelecionados[i] = rand() % 2; // Gera 0 ou 1
    }

    std::cout << "\nSolucao inicial: ";

    imprimeItens(itensSelecionados);

    // Calcula o peso e valor inicial
    int peso = 0, valor = 0;
    calcularPesoValor(itensSelecionados, lista, peso, valor);

    std::cout << "\nValor inicial: " << valor << "\nPeso inicial: " << peso << std::endl;

    // Parâmetros da Simulated Annealing
    double temperatura = 10000.0;
    double resfriamento = 0.99;
    int iteracoes = 1000;

    // Armazena a melhor solução
    int melhorValor = valor;
    int melhorIteracao = 0;

    for (int i = 0; i < iteracoes; i++)
    {
        // Gera uma solução vizinha
        std::vector<int> vizinho = itensSelecionados;
        int pos = rand() % size;         // Escolhe uma posição aleatória
        vizinho[pos] = 1 - vizinho[pos]; // Troca 0 por 1 ou 1 por 0

        // Calcula o peso e valor do vizinho
        int pesoVizinho = 0, valorVizinho = 0;
        calcularPesoValor(vizinho, lista, pesoVizinho, valorVizinho);

        std::cout << "Candidato: \n";
        imprimeItens(vizinho);

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
            double randomProb = rand() / double(RAND_MAX); // Número aleatório entre 0 e 1
            double probAceitar = exp(delta / temperatura); // Fórmula da Simulated Annealing

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
            melhorIteracao = i;
        }

        // Resfriamento da temperatura
        temperatura *= resfriamento;

        imprimeItens(itensSelecionados);
        // Exibe a solução atual
        std::cout << "Iteracao " << i << ":\nValor: " << valor << "\nPeso: " << peso << "\n"
                  << std::endl;

        if (temperatura < 1e-5)
            break; // Parar se a temperatura for muito baixa
    }

    std::cout << "\nA melhor solucao foi encontrada na iteracao " << melhorIteracao
              << " com valor " << melhorValor << " e peso " << peso << std::endl;

    return 0;
}
