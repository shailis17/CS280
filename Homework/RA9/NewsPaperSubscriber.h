#include <iostream>
#include <string>
#include "Address.h"

#ifndef NEWSPAPERSUBSCRIBER_H
#define NEWSPAPERSUBSCRIBER_H

using namespace std;

class NewsPaperSubscriber { //two data members (st_Address, rate)
    Address st_Address;
    public: double rate;

public: 
    NewsPaperSubscriber() // initializes the class object without passed parameters
    {
        st_Address = Address();
        rate = 0;
    }

    NewsPaperSubscriber(Address a) // receives one parameter to initialize the street address
    {
        st_Address = a;
        rate = 0;
    }

    Address getAddress(){
        return st_Address;
    }

    double getRate(){
        return rate;
    }

    void setAddress(Address a){
        st_Address = a;
    }

    virtual void setRate(double r) = 0; // setRate is abstract

    virtual void print() = 0; //abstract, no return, no parameters

    bool equals(NewsPaperSubscriber* other) //receives a NewsPaperSubscriber pointer parameter
    {
        if(getAddress() == other->getAddress()) //indicates two subscribers are equal if they have the same street address
            return true;
        
        return false;
    }

};

#endif