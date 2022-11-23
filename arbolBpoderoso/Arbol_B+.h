#include <iostream>
#include <vector>
#include "NodeB.h"
#include "Comparable.h"
using namespace std;

#ifndef ARBOL_B

#define ARBOL_B 1

class BTree
{
private:
    NodeB *root;
    int tam;

public:
    BTree(int pTam)
    {
        root = NULL;
        this->tam = pTam;
    }

    void display()
    {
        if (root != NULL)
            root->display();
    }

    vector<Comparable *> getConjuntoS()
    {
        vector<Comparable *> conjuntoS;

        if (root != NULL)
            root->getConjuntoS(&conjuntoS);
        return conjuntoS;
    }

    void insert(Comparable *k);
};

NodeB::NodeB(int t1, bool leaf1)
{
    tam = t1;
    leaf = leaf1;

    keys = new Comparable *[2 * tam - 1]; // Calcula el tamaño de la lista de nodos
    listaNodos = new NodeB *[2 * tam];
    n = 0;
}

void NodeB::display()
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (leaf == false)
            listaNodos[i]->display();
        keys[i]->printID();
    }

    if (leaf == false)
    {
        listaNodos[i]->display();
    }
}

void NodeB::getConjuntoS(vector<Comparable *> *conjuntoS)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (leaf == false)
            listaNodos[i]->getConjuntoS(conjuntoS);
        conjuntoS->push_back(keys[i]);
        keys[i]->printID();
    }

    if (leaf == false)
    {
        listaNodos[i]->getConjuntoS(conjuntoS);
    }
}

void BTree::insert(Comparable *k)
{
    if (root == NULL)
    {
        root = new NodeB(tam, true);
        root->keys[0] = k;
        root->n = 1; // Se le asigna
    }
    else
    {
        if (root->n == 2 * tam - 1) // Las holas intermedias tienen como mínimo (m-1)/2(Parte entera), entonces pasa si rompe el tamaño
        {
            NodeB *newNode = new NodeB(tam, false);

            newNode->listaNodos[0] = root;

            newNode->splitChild(0, root);

            int i = 0;
            if (newNode->keys[0] < k)
                i++;
            newNode->listaNodos[i]->insertNonFull(k);

            root = newNode;
        }
        else
            root->insertNonFull(k);
    }
}

void NodeB::insertNonFull(Comparable *k)
{
    int i = n - 1; // Para el tamaño maximo

    if (leaf == true)
    {
        while (i >= 0 && keys[i]->compareTo(k) == 1)
        {
            keys[i + 1] = keys[i];
            i--;
        }

        keys[i + 1] = k;
        n = n + 1; // Sube el nivel si sobrepasa el tamaño
    }
    else
    {
        while (i >= 0 && keys[i]->compareTo(k) == 1) // Busca un lugar donde se debe insertar
            i--;

        if (listaNodos[i + 1]->n == 2 * tam - 1)
        {
            splitChild(i + 1, listaNodos[i + 1]);

            if (keys[i + 1] < k)
                i++;
        }
        listaNodos[i + 1]->insertNonFull(k);
    }
}

void NodeB::splitChild(int i, NodeB *y) // y es el root
{
    NodeB *nuevoNodoTemp = new NodeB(y->tam, y->leaf); // Se crea con el tamaño
    nuevoNodoTemp->n = tam - 1;

    for (int j = 0; j < tam - 1; j++)
        nuevoNodoTemp->keys[j] = y->keys[j + tam];

    if (y->leaf == false)
    {
        for (int j = 0; j < tam; j++)
            nuevoNodoTemp->listaNodos[j] = y->listaNodos[j + tam];
    }

    y->n = tam - 1;
    for (int j = n; j >= i + 1; j--)
        listaNodos[j + 1] = listaNodos[j];

    listaNodos[i + 1] = nuevoNodoTemp;

    for (int j = n - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    keys[i] = y->keys[tam - 1];
    n = n + 1;
};

#endif
