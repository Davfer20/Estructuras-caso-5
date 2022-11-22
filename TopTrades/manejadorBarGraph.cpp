#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string data; // directedGraphDataBase

int main()
{
    ofstream outdata;
    string c_array[] = {"Microsoft,12", "Amazon,22", "Google,23", "Apple,70", "Razer,80"};

    outdata.open("files/lineaBaseDatos.csv"); // opens the file
    if (!outdata)
    {
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }

    outdata << "letter,frequency" << endl;

    for (int i = 0; i < (sizeof c_array / sizeof c_array[0]); ++i)
    {
        outdata << c_array[i] << endl;
    }
    outdata.close();

    cout << "File ready" << endl;
    return 0;
}