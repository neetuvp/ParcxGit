#include "validation.h"
#include<iostream>
#include<stdlib.h>
#include <fstream>
#include <time.h>
#include <ctime>
#include <string.h>
#include <regex>
#include<iomanip>
#include <algorithm>
#include<jsoncpp/json/json.h>
#include "PX_GeneralOperations.h"
using namespace std;
GeneralOperations gen;
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
/*void Validation::writelog(string data)
{
	ofstream f;
    f.open("Logs/ReservationLogcpp"+CurrentDateValidation()+".log",fstream::app);
    f << CurrentDateTimeValidation()<<":";
    f << data<< endl;
	
}*/
void Validation::writelog(string function, string txt) {    
    string path = "Services/ExceptionLogs/PX_DaemonService_Validation_" + gen.currentDateTime("%Y-%m-%d");
    gen.writeLog(path, function, txt);
    //WriteToLog("****Validation:"+function+"*****",txt);
}

Json::Value Validation::DataValidation(string data,int len,int datatype,int mandatoryflag) //Datatype: 1 - Integer, 2 - Alphabets, 3 - AlphaNumeric, 4 - SpecialCharacter
{
    Json::Value response;
    response["result"]=true;
    response["data"] = data;
    if(mandatoryflag==1 && data=="")  //Check if data is null
    {
	writelog("DataValidation","Missing Mandatory field");
        response["result"]=false;
        response["reason"] = "Missing Mandatory field";
    }
    if(datatype==3 && data.length()>(unsigned)len)  //Check if length of the data is within the assigned limit
    {
	writelog("DataValidation","Data length exceeds allowed limit");
        response["result"]=false;
        response["reason"] = "Data length exceeded";
    }
    switch(datatype)                 //Check the datatype validity
    {
        case 1:
            if(!checkDigit(data))
            {
		writelog("DataValidation","Invalid Digit:"+data);
                response["result"]=false;
                response["reason"] = "Invalid Number";
            }
            break;
        case 2:
            if(!checkAlpha(data))
            {
		writelog("DataValidation","Invalid Alpha Data:"+data);
                response["result"]=false;
                response["reason"] = "Invalid String";
            }
            break;
        case 3:
            if(!checkAlphaNumeric(data))
            {
                writelog("DataValidation","Invalid AlphaNumeric Data:"+data);
                response["result"]=false;
                response["reason"] = "Invalid String";
            }
            break;
        case 4:
            if(!checkDate(data))
            {
		writelog("DataValidation","Invalid Date:"+data);
                response["result"]=false;
                response["reason"] = "Invalid Date";
            }
            break;
        case 5:
            if(!checkEmail(data))
            {
		writelog("DataValidation","Invalid Email:"+data);
                response["result"]=false;
                response["reason"] = "Invalid Email";
            }
            break;

    }
    return response;

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

/*bool Validation::checkAlphaNumeric(string data)
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
}*/

bool Validation::checkAlphaNumeric(string data)
{
    writelog("checkAlphaNumeric", data);   
    //std::replace_if(data.begin(), data.end(), ::ispunct, '');
    size_t found = data.find("'");
    if (found != string::npos){
        //writelog("checkAlphaNumeric", "String found");        
        return false;

    }
    else{
        return true;
    }
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

Json::Value Validation::checkValidation(Json::Value data,Json::Value rules)
{

    Json::FastWriter fw;
    string type;
    int datatype=0,length=0;
    Json::Value response,validation,row;
    int flag = 0;
    string field;
//$%:;>&*$
try{
    for (auto const& member : data.getMemberNames()) {
        length=0;
        datatype=0;
        field = string(member);
        type = rules[field+"_type"].asString();
        if(type=="int"||type=="bigint"||type=="tinyint")
            datatype=1;
        else if(type=="varchar"||type=="text"|| type=="tinytext")
        {
            datatype=3;
            length = rules[field+"_length"].asInt();
        }
        else if(type=="datetime"||type=="timestamp")
            datatype=4;
        response = DataValidation(data[field].asString(),length,datatype,rules[field+"_mandatory"].asInt());
        response["field"]=field;
        if(response["result"]==false)
        {
            flag = 1;
           validation["result"] = "failed";
           row.append(response);
        }
        
    }
    if(flag==1)
    {
        validation["validation_details"] = row;
    }
    else
    {
        validation["result"] = "success";
    }
}
catch (const std::exception &e) 
{
    writelog("Validation", e.what());        
}
    return validation;


}

string SetDoublePrecision(double amt,int precision)
{
	std::string amt_string="0";
	std::ostringstream streamObj;
	// Set Fixed -Point Notation
	streamObj << std::fixed;
	
	// Set precision to 2 digits
	streamObj << std::setprecision(precision);
	
	//Add double to stream
	streamObj << amt;
	// Get string from output string stream
	amt_string = streamObj.str();
	std::cout << amt_string << std::endl;
	
	return amt_string;
	
}

string removeCharsFromString( string &str, char* charsToRemove ) {
   for ( unsigned int i = 0; i < strlen(charsToRemove); ++i ) {
      str.erase( remove(str.begin(), str.end(), charsToRemove[i]), str.end() );
   }
    return str;
}
string removeAlpha(string &str)
{
    str.erase(remove_if(str.begin(), str.end(), [](char c) { return isalpha(c); } ), str.end());
    return str;
}
string removePunct(string &str)
{
    str.erase(remove_if(str.begin(), str.end(), [](char c) { return ispunct(c); } ), str.end());
    return str;
}
Json::Value Validation::checkSpecialCharacters(Json::Value data,Json::Value rules)
{
    string field,fieldvalue,type,new_string;
    string float_replace = "@~`!@#$%^&*()_=+\\\\';:\"\\/?><,-‘";
    string date_replace = "@~`!@#$%^&*()_=+\\\\';\"\\/?><,‘";

    try{
        for (auto const& member : data.getMemberNames()) {
            field = string(member);
            type = rules[field+"_type"].asString();
            fieldvalue = data[field].asString();

            if(type=="int"||type=="bigint"||type=="tinyint")
            {
               fieldvalue = removeAlpha(fieldvalue);
               fieldvalue = removePunct(fieldvalue);
               data[field] = stoi(fieldvalue);
            }
            else if(type=="double"||type=="float"||type=="decimal")
            {
                fieldvalue = removeAlpha(fieldvalue);
                new_string = removeCharsFromString(fieldvalue, const_cast<char*>(float_replace.c_str()));
                data[field] = new_string;
            }
            else if(type=="varchar"||type=="text" || type=="tinytext")
            {
                fieldvalue = removePunct(fieldvalue);
                data[field] = fieldvalue;
            }   
            else if(type=="datetime"||type=="date" ||type=="timestamp")
            {
                new_string = removeAlpha(fieldvalue);
                new_string = removeCharsFromString(new_string, const_cast<char*>(date_replace.c_str()));
                data[field] = new_string;
            }    
        }
    }
    catch (const std::exception &e) 
    {
        writelog("checkSpecialCharacters", e.what());        
    }
    return data;

}