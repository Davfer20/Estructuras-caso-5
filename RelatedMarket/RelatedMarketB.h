#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class RelatedMarketB
{
private:
    string data;
    vector<string> queueList;

public:
    RelatedMarketB(vector<string> myvector)
    {
        this->queueList = myvector;
    }

    void getBase()
    {
        ofstream outdata;
        outdata.open("../RelatedMarket/files/donutDataBase.csv"); // opens the file
        if (!outdata)
        {
            cerr << "Error: file could not be opened" << endl;
            exit(1);
        }

        outdata << "name,value" << endl;

        for (int i = 0; i < queueList.size(); ++i)
        {
            outdata << queueList[i] << endl;
        }
        outdata.close();
        cout << "RelatedMarket File ready" << endl;
    }
};