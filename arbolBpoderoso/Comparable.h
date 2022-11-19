#ifndef COMPARABLE

#define COMPARABLE 1

class Comparable
{
private:
public:
    virtual int compareTo(Comparable *cObject) = 0;
    virtual void printID() = 0;
};

#endif