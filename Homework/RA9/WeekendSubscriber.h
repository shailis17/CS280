#include <iostream>
#include <string>
#include "Address.h"
#include "NewsPaperSubscriber.h"

#ifndef WEEKENDSUBSCRIBER_H
#define WEEKENDSUBSCRIBER_H

using namespace std;

class WeekendSubscriber : public NewsPaperSubscriber { //two data members (st_Address, rate)
    
public:
    WeekendSubscriber(Address a) : NewsPaperSubscriber(a)
    {
        setRate(4.5); 
    }

    void setRate(double r){
        NewsPaperSubscriber::rate  = r;
    }

    void print(){
        cout << getAddress().AddrContents() << endl;
        cout << "Rate: " << getRate() << endl;
        cout << "Service Type: Weekend Subscriber" << endl;
    }

};

#endif