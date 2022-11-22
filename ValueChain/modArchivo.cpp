#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string data; // directedGraphDataBase

int main()
{
    ofstream outdata;
    string c_array[] = {"Microsoft,Amazon,meneito", "Microsoft,HTC,licensing", "JOSE,Apple,suit", "Motorola,Apple,suit", "Nokia,Apple,resolved", "HTC,Apple,suit", "Kodak,Apple,suit", "DAVID,DAVIDLOCO,DAVIDLOCOLOCO", "Microsoft,Foxconn,suit", "Nokia,Qualcomm,suit"};

    outdata.open("files/directedGraphDataBase.csv"); // opens the file
    if (!outdata)
    {
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }

    outdata << "source,target,type" << endl;

    for (int i = 0; i < (sizeof c_array / sizeof c_array[0]); ++i)
    {
        outdata << c_array[i] << endl;
    }
    outdata.close();

    cout << "File ready" << endl;
    return 0;
}