//sudo g++ -std=c++11 -Wall -o test test.cpp validation.cpp
//      ./test

#include<iostream>
#include<stdlib.h>
#include <time.h>
#include <ctime>
#include "validation.h"

using namespace std;
int main()
{
   /* Validation v;
    v.setx(20);
    cout<<v.getx();
    return 1;*/


    struct tm tm;
    //string data ="2019-03-24T02:02:02-02:00";
    string data ="2018-12-25T14:00:00+05:00";
    if(!strptime(data.c_str(),"%Y-%m-%dT%H:%M:%S+%H:%M",&tm))
    {
        cout<<"here1";
    }
    if(!strptime(data.c_str(),"%Y-%m-%dT%H:%M:%S-%H:%M",&tm))
    {
        cout<<"here2";
    }
    if(!strptime(data.c_str(),"%Y-%m-%dT%H:%M:%SZ",&tm))
    {
        cout<<"here3";
    }
    if(!strptime(data.c_str(),"%Y-%m-%dT%H:%M:%S+%H:%M",&tm) && !strptime(data.c_str(),"%Y-%m-%dT%H:%M:%S-%H:%M",&tm) && !strptime(data.c_str(),"%Y-%m-%dT%H:%M:%SZ",&tm) && data!="")
    {
        cout<<"false"<<endl;   //||strptime(data.c_str(),"%FT%TZ",&tm)
    }
    else
    {
        cout<<"true"<<endl;
    }
    
   /* if(strptime(s.c_str(),"%F",&tm))
    {
        cout<<"Valid Date"<<endl;
    }
    else
    {
        cout<<"Invalid Date"<<endl;
    }*/
    
}