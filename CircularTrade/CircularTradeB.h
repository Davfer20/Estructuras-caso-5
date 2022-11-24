#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class CircularTradeB
{
private:
    string data;
    vector<string> queueList;

public:
    CircularTradeB(vector<string> myvector)
    {
        this->queueList = myvector;
    }

    void getBase()
    {
        ofstream outdata;
        outdata.open("../CircularTrade/files/directedGraphDataBase.csv"); // opens the file
        if (!outdata)
        {
            cerr << "Error: file could not be opened" << endl;
            exit(1);
        }

        outdata << "source,target,type" << endl;

        for (int i = 0; i < queueList.size(); ++i)
        {
            outdata << queueList[i] << endl;
        }
        outdata.close();
        cout << "File ready" << endl;
    }
};