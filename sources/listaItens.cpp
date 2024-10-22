#include "../headers/listaItens.hpp"
#include <iostream>
#include <fstream> // Para manipulação de arquivos

// Construtor que lê os itens de um arquivo
ListaItens::ListaItens()
{
    std::ifstream arquivo("itens.txt"); // Abre o arquivo
    if (arquivo.is_open())
    {
        int valor, peso;
        while (arquivo >> valor >> peso)
        {                                       // Lê dois inteiros (valor e peso)
            itens.push_back(Item(valor, peso)); // Adiciona o item na lista
        }
        arquivo.close(); // Fecha o arquivo
    }
    else
    {
        std::cerr << "Não foi possível abrir o arquivo itens.txt" << std::endl;
    }
}

// Método para exibir os itens
void ListaItens::exibirItens()
{
    for (const auto &item : itens)
    {
        std::cout << "Valor: " << item.valor << ", Peso: " << item.peso << std::endl;
    }
}

int ListaItens::getSize() const
{
    return itens.size();
}

const std::vector<Item> &ListaItens::getItens() const
{
    return itens;
}
