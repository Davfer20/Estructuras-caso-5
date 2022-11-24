#include <iostream>

#ifndef SINGLE

#define SINGLE 1

using namespace std;

// Patron Singleton empleado para variables globales
class PersonID
{
    static PersonID *instance;
    int personID;
    // Private constructor so that no objects can be created.
    PersonID()
    {
        personID = 1; // Contador de ID
    }

public:
    static PersonID *getInstance()
    {
        if (!instance)
            instance = new PersonID;
        return instance;
    }

    // Retorna el ID actual
    int getID()
    {
        int resul = this->personID;
        this->personID += 1;
        return resul;
    }
};
#endif