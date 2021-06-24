#include "validation.h"
#include<iostream>
#include<stdlib.h>
#include <fstream>
#include <time.h>
#include <ctime>
#include <string.h>
#include <regex>
#include <algorithm>
using namespace std;
void Validation::setx(int newx)
{
   myx = newx;
}

int Validation::getx()
{
    return myx;
}
string Validation::CurrentDateTimeValidation()
{
    time_t now=time(NULL);	
    std::tm* timeinfo;
    char buffer[80];
    std::time(&now);
    timeinfo=std::localtime(&now);
    std::strftime(buffer,80,"%d-%m-%Y %H:%M:%S",timeinfo);		
    string datetime(buffer);
    return datetime;
}
string Validation::CurrentDateValidation()
{
    time_t now=time(NULL);	
    std::tm* timeinfo;
    char buffer[80];
    std::time(&now);
    timeinfo=std::localtime(&now);
    std::strftime(buffer,80,"%d-%m-%Y",timeinfo);		
    string datetime(buffer);
    return datetime;
}
void Validation::writelog(string data)
{
	ofstream f;
    f.open("Logs/ReservationLogcpp"+CurrentDateValidation()+".log",fstream::app);
    f << CurrentDateTimeValidation()<<":";
    f << data<< endl;
	
}

bool Validation::DataValidation(string data,int len,int datatype,int mandatoryflag) //Datatype: 1 - Integer, 2 - Alphabets, 3 - AlphaNumeric, 4 - SpecialCharacter
{
    if(mandatoryflag==1 && data=="")  //Check if data is null
    {
		writelog("Missing Mandatory field");
        return false;
    }
    if(data.length()>(unsigned)len)  //Check if length of the data is within the assigned limit
    {
		writelog("Data length exceeds allowed limit");
        return false;
    }
    switch(datatype)                 //Check the datatype validity
    {
        case 1:
            if(!checkDigit(data))
            {
				writelog("Invalid Digit:"+data);
                return false;
            }
            break;
        case 2:
            if(!checkAlpha(data))
            {
				writelog("Invalid Alpha Data:"+data);
                return false;
            }
            break;
        case 3:
            if(!checkAlphaNumeric(data))
            {
				writelog("Invalid AlphaNumeric Data:"+data);
                return false;
            }
            break;
        case 4:
            if(!checkDate(data))
            {
				writelog("Invalid Date:"+data);
                return false;
            }
            break;
        case 5:
            if(!checkEmail(data))
            {
				writelog("Invalid Email:"+data);
                return false;
            }
            break;

    }
    return true;

}

bool Validation::checkDigit(string data)
{
    int n = data.length();
    for(int i=0;i<n;i++)
    {
        if(!(isdigit((int)data[i])) && data!="")
        {
            return false;
        }
    }
    return true;
}
bool Validation::checkAlpha(string data)
{
    int n = data.length();
    for(int i=0;i<n;i++)
    {
        if(!(isalpha((int)data[i])) && !(isspace((int)data[i])) && data!="")
        {
            return false;
        }
    }
    return true;
}

bool Validation::checkAlphaNumeric(string data)
{
    int n = data.length();
    char c[n+1];
    strcpy(c,data.c_str());
    
    for(int i=0;i<n;i++)
    {
        if(!(isalnum((int)data[i])) && !(isspace((int)data[i])) && data!="")
        {
            return false;
        }
    }
    return true;
}

bool Validation::checkDate(string data)
{
    struct tm tm;
    
    /*if(!strptime(data.c_str(),"%Y-%m-%dT%H:%M:%S+%H:%M",&tm) && !strptime(data.c_str(),"%Y-%m-%dT%H:%M:%S-%H:%M",&tm) && !strptime(data.c_str(),"%Y-%m-%dT%H:%M:%SZ",&tm) && data!="")
    {
        return false;   //||strptime(data.c_str(),"%FT%TZ",&tm)
    }
    else
    {
        return true;;
    }*/
    if(!strptime(data.c_str(),"%Y-%m-%d %H:%M:%S",&tm) && data!="")
    {
        return false;   //||strptime(data.c_str(),"%FT%TZ",&tm)
    }
    else
    {
        return true;;
    }
}

bool Validation::checkEmail(string data)
{
    /*const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    if(!regex_match(data,pattern) && data!="")
    {
        return false;
    }
    return true;*/
	if(data != "")
	{
		auto b=data.begin(), e=data.end();

		if ((b=std::find(b, e, '@')) != e && std::find(b, e, '.') != e )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else{
		return true;//Not Mandatory
	}
	
}