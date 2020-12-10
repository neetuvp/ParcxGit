#ifndef PXHEADER_H
#define PXHEADER_H

#include<iostream>
#include<stdlib.h>
#include <fstream>
#include <time.h>
#include <ctime>
#include <string.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
using namespace std;
class pxheader{
    public:
    bool DataValidation(string data,int len,int datatype,int mandatoryflag);
    string currentDateTime();
    string currentDate();
    void writeException(string path,string function,string text);
    void writeLog(string path,string function,string text);
    sql::Connection* mysqlConnect(string database);
    
};

#endif
