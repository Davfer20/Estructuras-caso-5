#ifndef REGIS
#define REGIS

#include "../graph/INodo.h"
#include <string>

using namespace std;

class Registered : public INodo
{
private:
    string nickname;
    string offer;
    string demand;
    string postdate;
    int matchLevel;

public:
    Registered(string pNickname, string pOffer, string pDemand, string pPostdate)
    {
        this->nickname = pNickname;
        this->offer = pOffer;
        this->demand = pDemand;
        this->postdate = pPostdate;
        this->setId(rand() * 99999999999);
    }

    string getNickname()
    {
        return this->nickname;
    }
    int getMatchLevel()
    {
        return this->matchLevel;
    }

    string getOffer()
    {
        return this->offer;
    }

    string getDemand()
    {
        return this->demand;
    }

    string getPostdate()
    {
        return this->postdate;
    }

    void incMatchLevel()
    {
        this->matchLevel++;
    }

    void setMathLevel(int num)
    {
        this->matchLevel = num;
    }
};

#endif