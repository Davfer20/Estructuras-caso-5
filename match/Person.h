#ifndef PERSON
#define PERSON 1

#include "../arbolBpoderoso/Comparable.h"
#include "../socket/Registered.h"

using namespace std;

class Person : public Comparable
{
private:
    Registered *user;
    string nickname;
    string keyWord;

public:
    Person(Registered *pUser, string pWord)
    {
        this->user = pUser;
        this->nickname = pUser->getNickname();
        this->keyWord = pWord;
    }

    Person(string pWord, Registered *pUser)
    {
        this->user = pUser;
        this->nickname = "!";
        this->keyWord = pWord;
    }

    virtual int compareTo(Comparable *cObject)
    {
        Person *otherUser = dynamic_cast<Person *>(cObject);
        if (this->keyWord == otherUser->getKeyWord())
        {
            if (nickname == otherUser->getNickname())
            {
                return 0;
            }
            else if (nickname < otherUser->getNickname())
            {
                return -1;
            }
            else
            {
                return 1;
            }
        }
        else if (keyWord < otherUser->getKeyWord())
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }

    string getNickname()
    {
        return this->nickname;
    }

    string getKeyWord()
    {
        return this->keyWord;
    }

    Registered *getUser()
    {
        return this->user;
    }

    void incCompatibility()
    {
        this->user->incMatchLevel();
    }

    void printID()
    {
        // do something
    }

    bool isMatch()
    {
        bool resul = false;
        if (this->user->getMatchLevel() > 1)
        {
            resul = true;
        }
        return resul;
    }

    void resetCompatibility()
    {
        this->user->setMathLevel(0);
    }
};

#endif