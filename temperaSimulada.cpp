#include "./headers/listaItens.hpp"
#include <string>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <numeric>
#include <algorithm>

int CAPACIDADE = 101;

struct Data
{
    int valor;
    int peso;
    int iteracao;
};

// Função para ler o arquivo e armazenar os dados
std::vector<Data> lerArquivo(const std::string &nomeArquivo)
{
    std::ifstream arquivo(nomeArquivo);
    std::vector<Data> dados;
    if (arquivo.is_open())
    {
        std::string linha;
        std::getline(arquivo, linha); // Ignorar a primeira linha
        int valor, peso, iteracao;
        while (arquivo >> valor >> peso >> iteracao)
        {
            dados.push_back({valor, peso, iteracao});
        }
        arquivo.close();
    }
    else
    {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
    }
    return dados;
}
// Função para calcular a média
double calcularMedia(const std::vector<int> &valores)
{
    return std::accumulate(valores.begin(), valores.end(), 0.0) / valores.size();
}

// Função para contar quantas vezes um valor aparece
int contarOcorrencias(const std::vector<int> &valores, int valor)
{
    return std::count(valores.begin(), valores.end(), valor);
}

// Função para realizar as análises
void analisarDados(const std::vector<Data> &dados)
{
    std::vector<int> valores, iteracoes;

    // Extrair os valores e iterações
    for (const auto &item : dados)
    {
        valores.push_back(item.valor);
        iteracoes.push_back(item.iteracao);
    }

    // 1. Calcular a média das iterações
    double mediaIteracoes = calcularMedia(iteracoes);

    // Abrir o arquivo para salvar os resultados
    std::ofstream arquivo("./solucoesParciais.txt", std::ios::app); // Abre em modo append para adicionar os dados

    if (arquivo.is_open())
    {
        arquivo << "Media das Iteracoes: " << mediaIteracoes << std::endl;

        // 2. Encontrar o maior, segundo maior e terceiro maior valor
        std::vector<int> valoresUnicos = valores;                                                          // Criar uma cópia dos valores
        std::sort(valoresUnicos.begin(), valoresUnicos.end(), std::greater<int>());                        // Ordenar em ordem decrescente
        valoresUnicos.erase(std::unique(valoresUnicos.begin(), valoresUnicos.end()), valoresUnicos.end()); // Remover duplicatas

        if (valoresUnicos.size() >= 3)
        {
            int maiorValor = valoresUnicos[0];
            int segundoMaiorValor = valoresUnicos[1];
            int terceiroMaiorValor = valoresUnicos[2];

            int ocorrenciasMaior = contarOcorrencias(valores, maiorValor);
            int ocorrenciasSegundoMaior = contarOcorrencias(valores, segundoMaiorValor);
            int ocorrenciasTerceiroMaior = contarOcorrencias(valores, terceiroMaiorValor);

            arquivo << "Maior valor: " << maiorValor << " - Aparece " << ocorrenciasMaior << " vezes" << std::endl;
            arquivo << "Segundo maior valor: " << segundoMaiorValor << " - Aparece " << ocorrenciasSegundoMaior << " vezes" << std::endl;
            arquivo << "Terceiro maior valor: " << terceiroMaiorValor << " - Aparece " << ocorrenciasTerceiroMaior << " vezes" << std::endl;

            // 3. Calcular a porcentagem do maior valor em relação ao total de valores
            double porcentagemMaior = (static_cast<double>(ocorrenciasMaior) / valores.size()) * 100.0;
            arquivo << "Porcentagem de ocorrências do maior valor em relação a todos os outros: " << porcentagemMaior << "%\n"
                    << std::endl;
        }
        else
        {
            arquivo << "Nao ha valores suficientes para determinar os três maiores.\n"
                    << std::endl;
        }

        arquivo.close(); // Fecha o arquivo após escrever
    }
    else
    {
        std::cerr << "Erro ao abrir o arquivo para salvar os resultados." << std::endl;
    }
}

void copiarArquivo(const std::string &origem, const std::string &destino)
{
    std::ifstream arquivoOrigem(origem);                  // Abre o arquivo de origem para leitura
    std::ofstream arquivoDestino(destino, std::ios::app); // Abre o arquivo de destino para escrita

    if (arquivoOrigem.is_open() && arquivoDestino.is_open())
    {
        std::string linha;
        while (getline(arquivoOrigem, linha))
        {
            arquivoDestino << linha << std::endl; // Copia cada linha do arquivo de origem para o destino
        }
    }
    else
    {
        std::cerr << "Erro ao abrir os arquivos." << std::endl;
    }

    arquivoOrigem.close();
    arquivoDestino.close();
}

void salvarEmArquivo(int valor1, int valor2, int valor3)
{
    // Abre (ou cria) o arquivo em modo de adição (append), para não sobrescrever o conteúdo existente
    std::ofstream arquivo("./solucoesParciais.txt", std::ios::app);

    if (arquivo.is_open())
    {
        arquivo << valor1 << " " << valor2 << " " << valor3 << std::endl; // Escreve os valores separados por um espaço e em uma nova linha
        arquivo.close();                                                  // Fecha o arquivo
    }
    else
    {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
    }
}

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

int temperaSimulada(double temperatura, double resfriamento, int maxIter)
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

    // Armazena a melhor solução
    int melhorValor = valor;
    int melhorPeso = peso;
    int melhorIteracao = 0;

    for (int i = 0; i < maxIter; i++)
    {
        system("cls");

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
            melhorPeso = peso;
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
              << " com valor " << melhorValor << " e peso " << melhorPeso << "\n"
              << std::endl;

    salvarEmArquivo(melhorValor, melhorPeso, melhorIteracao);

    return 0;
}

int execute(double temperatura, double resfriamento, int maxIter)
{
    std::ofstream arquivoParcial("./solucoesParciais.txt");
    if (arquivoParcial.is_open())
    {
        arquivoParcial << "Temperatura: " << temperatura << "\tTaxa de resfriamento: " << resfriamento << "\tMáximo de iterações: " << maxIter << std::endl;
        arquivoParcial.close();
    }
    else
        std::cerr << "Erro ao abrir o arquivo." << std::endl;

    for (int i = 0; i < 20; i++)
    {
        temperaSimulada(temperatura, resfriamento, maxIter);
    }

    std::vector<Data> dados = lerArquivo("./solucoesParciais.txt");

    if (!dados.empty())
    {
        analisarDados(dados);
    }
    else
    {
        std::cerr << "Nenhum dado encontrado no arquivo." << std::endl;
    }

    copiarArquivo("./solucoesParciais.txt", "./solucoes.txt");

    return 0;
}

int main()
{
    execute(10000.0, 0.99, 1000);
    execute(1000.0, 0.99, 1000);
    execute(10000.0, 0.98, 1000);

    return 0;
}