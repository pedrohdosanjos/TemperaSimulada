#ifndef LISTAITENS_HPP
#define LISTAITENS_HPP

#include "item.hpp"
#include <vector>
#include <string>

class ListaItens
{
private:
    std::vector<Item> itens; // Vetor para armazenar os itens

public:
    // Construtor que lê os itens de um arquivo
    ListaItens();

    void exibirItens();

    int getSize() const;

    const std::vector<Item> &getItens() const;
};

#endif
