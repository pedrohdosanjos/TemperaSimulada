#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <string>
#include <vector>

struct Data
{
    int valor;
    int peso;
    int iteracao;
};

class DataHandler
{
public:
    std::vector<Data> lerArquivo(const std::string &nomeArquivo);
    double calcularMedia(const std::vector<int> &valores);
    int contarOcorrencias(const std::vector<int> &valores, int valor);
    void exibirDados(const std::vector<Data> &dados);
    void analisarDados(const std::vector<Data> &dados);
    void copiarArquivo(const std::string &origem, const std::string &destino);
    void salvarEmArquivo(int valor1, int valor2, int valor3, int valor4, int valor5);
    void imprimeItens(const std::vector<int> &vetor);
};

#endif
