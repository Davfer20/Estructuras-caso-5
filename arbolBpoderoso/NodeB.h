#include <iostream>
#include "Comparable.h"

class NodeB
{
    Comparable **keys; // Llaves cercanas
    int tam;
    NodeB **listaNodos; // Lista de punteros
    int n;              // Es el orden
    bool leaf;

public:
    NodeB(int t1, bool leaf1);
    void insertNonFull(Comparable *k);
    void splitChild(int i, NodeB *y);
    void display();

    friend class BTree;
};
