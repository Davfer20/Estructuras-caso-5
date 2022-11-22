#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;

int main()
{
    list<string> lalista;
    char linea[128];

    ifstream fs("index.txt");                    // leer de este archivo
    ofstream fstemp("nombretemp.txt", ios::out); // escribir en este archivo
    if (!fs || !fstemp)                          // no se pudo abrir alguno de los 2
    {
        cout << "Error al abrir el archivo!" << endl;
        return 0;
    }

    // modificar linea a linea
    while (!fs.eof())
    {
        // http://www.programacionenc.net/index.php?option=com_content&view=article&id=69:manejo-de-archivos-en-c&catid=37:programacion-cc&Itemid=55

        // https://es.stackoverflow.com/questions/21171/como-modificar-un-archivo-txt-desde-c
    }

    // reemplazar un archivo por otro
    fs.close();
    fstemp.close();
    remove("nombre.txt");                   // borrar el original
    rename("nombretemp.txt", "nombre.txt"); // renombrar el temporal

    cout << "File ready" << endl;
    return 0;
}