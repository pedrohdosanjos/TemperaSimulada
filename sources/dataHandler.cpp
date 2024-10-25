#include "../headers/dataHandler.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <limits>

std::vector<Data> DataHandler::lerArquivo(const std::string &nomeArquivo)
{
    std::ifstream arquivo(nomeArquivo);
    std::vector<Data> dados;

    if (arquivo.is_open())
    {
        std::string linha;
        std::getline(arquivo, linha); // Ignorar a primeira linha
        while (std::getline(arquivo, linha))
        { // Ler o restante das linhas
            std::istringstream ss(linha);
            int valor, peso, iteracao;

            // Ler os três primeiros valores antes do '|'
            ss >> valor >> peso >> iteracao;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '|'); // Ignorar o restante da linha após o '|'

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

double DataHandler::calcularMedia(const std::vector<int> &valores)
{
    return std::accumulate(valores.begin(), valores.end(), 0.0) / valores.size();
}

int DataHandler::contarOcorrencias(const std::vector<int> &valores, int valor)
{
    return std::count(valores.begin(), valores.end(), valor);
}

void DataHandler::exibirDados(const std::vector<Data> &dados)
{
    for (const auto &d : dados)
    {
        std::cout << "Valor: " << d.valor << ", Peso: " << d.peso << ", Iteracao: " << d.iteracao << std::endl;
    }
}

void DataHandler::analisarDados(const std::vector<Data> &dados)
{
    std::vector<int> valores, iteracoes;

    for (const auto &item : dados)
    {
        valores.push_back(item.valor);
        iteracoes.push_back(item.iteracao);
    }

    double mediaIteracoes = calcularMedia(iteracoes);

    std::ofstream arquivo("./solucoesParciais.txt", std::ios::app);
    if (arquivo.is_open())
    {
        arquivo << "Media das Iteracoes: " << mediaIteracoes << std::endl;

        std::vector<int> valoresUnicos = valores;
        std::sort(valoresUnicos.begin(), valoresUnicos.end(), std::greater<int>());
        valoresUnicos.erase(std::unique(valoresUnicos.begin(), valoresUnicos.end()), valoresUnicos.end());

        for (int i = 0; i < valoresUnicos.size(); i++)
        {
            int valor = valoresUnicos[i];
            int ocorrencias = contarOcorrencias(valores, valor);
            arquivo << "Valor: " << valor << " - Aparece " << ocorrencias << " vezes" << std::endl;
        }

        double porcentagemMaior = (static_cast<double>(contarOcorrencias(valores, valoresUnicos[0])) / valores.size()) * 100.0;
        arquivo << "Porcentagem de ocorrências do maior valor em relação a todos os outros: " << porcentagemMaior << "%\n"
                << std::endl;

        arquivo.close();
    }
    else
    {
        std::cerr << "Erro ao abrir o arquivo para salvar os resultados." << std::endl;
    }
}

void DataHandler::copiarArquivo(const std::string &origem, const std::string &destino)
{
    std::ifstream arquivoOrigem(origem);
    std::ofstream arquivoDestino(destino, std::ios::app);

    if (arquivoOrigem.is_open() && arquivoDestino.is_open())
    {
        std::string linha;
        while (getline(arquivoOrigem, linha))
        {
            arquivoDestino << linha << std::endl;
        }
    }
    else
    {
        std::cerr << "Erro ao abrir os arquivos." << std::endl;
    }

    arquivoOrigem.close();
    arquivoDestino.close();
}

void DataHandler::salvarEmArquivo(int valor1, int valor2, int valor3, int valor4, int valor5)
{
    std::ofstream arquivo("./solucoesParciais.txt", std::ios::app);

    if (arquivo.is_open())
    {
        arquivo << valor1 << " " << valor2 << " " << valor3 << " | " << valor4 << " " << valor5 << std::endl;
        arquivo.close();
    }
    else
    {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
    }
}

void DataHandler::imprimeItens(const std::vector<int> &vetor)
{
    for (size_t i = 0; i < vetor.size(); ++i)
    {
        std::cout << vetor[i] << " ";
    }
    std::cout << std::endl;
}
