#ifndef REGIS
#define REGIS

#include "../graph/INodo.h"
#include "../match/PersonID.h"

#include <string>

using namespace std;

PersonID *PersonID::instance = 0;

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
        PersonID *var = var->getInstance();
        this->setId(var->getID());
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