#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class ValueChainB
{
private:
    string data;
    vector<string> dataVector;

public:
    ValueChainB(vector<string> myVector)
    {
        this->dataVector = myVector;
    }

    void getGraphBar()
    {
        ofstream outdata;
        outdata.open("../ValueChain/files/directedGraphDataBase.csv"); // opens the file
        if (!outdata)
        {
            cerr << "Error: file could not be opened" << endl;
            exit(1);
        }
        outdata << "source,target,type" << endl;

        for (int i = 0; i < dataVector.size(); ++i)
        {
            outdata << dataVector[i] << endl;
        }
        outdata.close();

        cout << "Top Trades File ready" << endl;
    }
};