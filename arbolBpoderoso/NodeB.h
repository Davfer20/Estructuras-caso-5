#include <iostream>
#include "Comparable.h"

using namespace std;
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
    void getConjuntoS(vector<Comparable *> *conjuntoS);

    friend class BTree;
};
