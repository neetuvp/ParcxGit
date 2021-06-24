#include<iostream>
#include<stdlib.h>
#include <fstream>
#include <time.h>
#include <ctime>
#include <string.h>

using namespace std;
class Validation{
    int myx;
    public:
    Validation()
    {
        myx= 0;
    }
    void setx(int newx);
    int getx();
    bool DataValidation(string data,int len,int datatype,int mandatoryflag);

    private:
    bool checkDigit(string data);
    bool checkAlpha(string data);
    bool checkAlphaNumeric(string data);
    bool checkDate(string data);
    bool checkEmail(string data);
	void writelog(string data);
	string CurrentDateTimeValidation();
	string CurrentDateValidation();
};