#include <iostream>
#include <string>
#include "Address.h"
#include "NewsPaperSubscriber.h"

#ifndef WEEKLYSUBSCRIBER_H
#define WEEKLYSUBSCRIBER_H

using namespace std;

class WeeklySubscriber : public NewsPaperSubscriber { //two data members (st_Address, rate)
    
public:
    WeeklySubscriber(Address a): NewsPaperSubscriber(a)
    {
        setRate(10.5); 
    }

    void setRate(double r){
        NewsPaperSubscriber::rate  = r;
    }

    void print(){
        cout << getAddress().AddrContents() << endl;
        cout << "Rate: " << getRate() << endl;
        cout << "Service Type: Weekly Subscriber" << endl;
    }

};

#endif