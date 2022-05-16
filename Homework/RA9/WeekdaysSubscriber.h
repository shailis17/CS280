#include <iostream>
#include <string>
#include "Address.h"
#include "NewsPaperSubscriber.h"

#ifndef WEEKDAYSSUBCRIBER_H
#define WEEKDAYSSUBCRIBER_H

using namespace std;

class WeekdaysSubscriber : public NewsPaperSubscriber { //two data members (st_Address, rate)
    
public:
    WeekdaysSubscriber(Address a): NewsPaperSubscriber(a)
    {
        setRate(7.5); 
    }

    void setRate(double r){
        NewsPaperSubscriber::rate  = r;
    }

    void print(){
        cout << getAddress().AddrContents() << endl;
        cout << "Rate: " << getRate() << endl;
        cout << "Service Type: Week Days Subscriber" << endl;
    }

};

#endif