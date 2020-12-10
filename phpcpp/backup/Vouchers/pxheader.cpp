#include "pxheader.h"
#include<iostream>
#include<stdlib.h>
#include <fstream>
#include <time.h>
#include <ctime>
#include <string.h>
#include <regex>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
using namespace std;
#define Server "DBServer"
#define Username "parcxservice"
#define Password "1fromParcx!19514"
string pxheader::currentDateTime()
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


string pxheader::currentDate()
{
    time_t now=time(NULL);	
    std::tm* timeinfo;
    char buffer[80];
    std::time(&now);
    timeinfo=std::localtime(&now);
    std::strftime(buffer,80,"%d-%m-%Y",timeinfo);		
    string date(buffer);
    return date;
}

void pxheader::writeException(string path,string function,string txt)
{
    ofstream fp;
    fp.open(path, ios::app);
    fp<<currentDateTime()<<" "<<function<<":: "<<txt<<endl;
    fp.close();
}//end of .writeException

void pxheader::writeLog(string path,string function,string txt)
{
    ofstream fp;
    fp.open(path, ios::app);
    fp<<currentDateTime()<<" "<<function<<":: "<<txt<<endl;
    fp.close();
}//end of .writeLog

sql::Connection* pxheader::mysqlConnect(string database)
{
    sql::Connection *con=NULL;
    try
    {
        sql::Driver *driver;				
        driver = get_driver_instance();				
        con = driver->connect(Server,Username,Password);	
        con->setSchema(database);	
       // cout<<"Connection Successful"<<endl;
    }
    catch(exception &e)
    {				
       // cout<<e.what()<<endl;
        // writeException("mysqlconnect",e.what());									
    }
    return con;
}//end of 














/*bool Validation::DataValidation(string data,int len,int datatype,int mandatoryflag) //Datatype: 1 - Integer, 2 - Alphabets, 3 - AlphaNumeric, 4 - SpecialCharacter
{
    if(mandatoryflag==0)
    {
        return true;
    }
    if(mandatoryflag==1 && data=="")  //Check if data is null
    {
        return false;
    }
    if(data.length()>(unsigned)len)  //Check if length of the data is within the assigned limit
    {
        return false;
    }
    switch(datatype)                 //Check the datatype validity
    {
        case 1:
            if(!checkDigit(data))
            {
                return false;
            }
            break;
        case 2:
            if(!checkAlpha(data))
            {
                return false;
            }
            break;
        case 3:
            if(!checkAlphaNumeric(data))
            {
                return false;
            }
            break;
        case 4:
            if(!checkDate(data))
            {
                return false;
            }
            break;
        case 5:
            if(!checkEmail(data))
            {
                return false;
            }
            break;
        case 6:
            if(!checkIP(data))
            {
                return false;
            }

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
bool Validation::checkIP(string data)
{
    int n = data.length();
    for(int i=0;i<n;i++)
    {
        if(!(isdigit((int)data[i])) && data!="" && data[i]!='.')
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
    struct tm tm;*/
    /*if(!strptime(data.c_str(),"%Y-%m-%dT%H:%M:%S+%H:%M",&tm) && data!="")
    {
        return false;   //||strptime(data.c_str(),"%FT%TZ",&tm)
    }*/
    /*if(!strptime(data.c_str(),"%Y-%m-%dT%H:%M:%S+%H:%M",&tm) && !strptime(data.c_str(),"%Y-%m-%dT%H:%M:%S-%H:%M",&tm) && !strptime(data.c_str(),"%Y-%m-%dT%H:%M:%SZ",&tm) && data!="")
    {
        return false;
    }
    return true;*/
    /*if(!strptime(data.c_str(),"%Y-%m-%dT%H:%M:%S+%H:%M",&tm) && !strptime(data.c_str(),"%Y-%m-%dT%H:%M:%S-%H:%M",&tm) && !strptime(data.c_str(),"%Y-%m-%dT%H:%M:%SZ",&tm) && data!="")
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
    const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    if(!regex_match(data,pattern) && data!="")
    {
        return false;
    }
    return true;
}*/