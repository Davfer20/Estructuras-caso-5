#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class BarTradesB
{
private:
    string data;
    vector<string> dataVector;

public:
    BarTradesB(vector<string> myVector)
    {
        this->dataVector = myVector;
    }

    void getGraphBar()
    {
        ofstream outdata;
        outdata.open("../TopTrades/files/lineaBaseDatos.csv"); // opens the file
        if (!outdata)
        {
            cerr << "Error: file could not be opened" << endl;
            exit(1);
        }
        outdata << "letter,frequency" << endl;

        for (int i = 0; i < dataVector.size(); ++i)
        {
            outdata << dataVector[i] << endl;
        }
        outdata.close();

        cout << "Top Trades File ready" << endl;
    }
};