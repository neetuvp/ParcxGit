#include "PX_GeneralOperations.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <phpcpp.h>
#include <iostream>
#include <sstream>
#include <cppconn/prepared_statement.h>
#include "validation.h"
#include<vector>
#define ServerDB "parcx_server"
#define ReportingDB "parcx_reporting"
#define DashboardDB "parcx_dashboard"
#define dateFormat "%Y-%m-%d"

#define MANDATORY "Mandatory"
#define MAXLENGTH "Maximum Length Exceeded"
#define CONSTRAINT "Invalid Character"
#define MediaPath "/Media"
using namespace std;
GeneralOperations General;
Validation validation;
string validation_failed = "Validation Failed\n";
void writeLog(string function, string message) {
    General.writeLog("WebApplication/ApplicationLogs/PX-GreetingScreen-" + General.currentDateTime(dateFormat), function, message);
}

void writeException(string function, string message) {
    General.writeLog("WebApplication/ExceptionLogs/PX-GreetingScreen-" + General.currentDateTime(dateFormat), function, message);
    writeLog(function, "Exception: " + message);
}


string toString(Php::Value param) {
    string value = param;
    return value;
}

std::vector<std::string> split_videolist(const std::string& s, char delimiter) {
    std::vector<std::string> splits;
    std::string split;
    std::istringstream ss(s);
    while (std::getline(ss, split, delimiter)) {
        splits.push_back(split);
    }
    return splits;
}

int generateUniqueId()
{    							    		    
   struct tm tm;
    string startDate = "2020-01-01 00:00:00";

    strptime(startDate.c_str(), "%Y-%m-%d %H:%M:%S", &tm);
    time_t t = mktime(&tm);

    time_t now = time(NULL);
    long seconds = difftime(now, t);
    return seconds;
}

std::string random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

string arrayToString(Php::Value json) {

    stringstream ss;
    ss << "{";
    for (auto &iter : json)
        ss << "\"" << iter.first << "\":\"" << iter.second << "\",";
    string response = ss.str();
    response = response.substr(0, response.size() - 1) + "}";
    return response;
}


Php::Value uploadMedia(Php::Value data)
{
    string filename = toString(data["name"]);
    string from = toString(data["from"]);
    string dest = "../../Media/"+filename+"_"+generateUniqueId();
    double size = data["size"];
    int error = data["error"];
    string type = toString(data["type"]);
    string result = "";
    writeLog("uploadMedia","size:"+to_string(size/ (1024 * 1024 * 1024))+" GB");
    writeLog("uploadMedia","type:"+string(type));
    if(size > 1024 * 1024 * 1024)
    {
        result ="File size is >1 GB" ;
    }
    else
    {
        string extension = filename.substr(filename.find_last_of(".") + 1);
        if(extension == "mp4" ||  extension == "gif" || extension == "lottie" || extension == "json")  {
            if(error>0)
            {
                result ="File Upload Error:"+to_string(error) ;
            }
            else
            {
                system(("sudo cp -p "+from+" "+dest).c_str());
                system(("sudo chmod 777 "+dest).c_str());
                result ="Success" ;
                writeLog("uploadMedia",result);
                
            }
        } 
        else 
        {
            //writeLog("uploadMedia","Invalid Extension");
            result ="Invalid Extension" ;
        }
    }
    writeLog("uploadMedia",result);
    return result;
    
}

void updateStage(Php::Value data)
    {
    try
        {
        string query_string="";
        sql::Connection *con;
        sql::PreparedStatement *prep_stmt=NULL;    
        
        con= General.mysqlConnect(ServerDB); 
        query_string = "Update greeting_screen set message_line1=?,message_line2=?,message_line3=?,description=?,bg_video_file=?,animation_file=?,title=?,last_updated_date_time=NOW() where stage_id=?";
            
            
        prep_stmt = con->prepareStatement(query_string);
        prep_stmt->setString(1,toString(data["message_line1"]));
        prep_stmt->setString(2,toString(data["message_line2"]));
        prep_stmt->setString(3,toString(data["message_line3"]));
        prep_stmt->setString(4,toString(data["description"]));
        prep_stmt->setString(5,toString(data["bg_file"]));
        prep_stmt->setString(6,toString(data["animation_file"]));
        prep_stmt->setString(7,toString(data["title"]));
        prep_stmt->setString(8,toString(data["stage_id"]));
        

        prep_stmt->executeUpdate();
        
    
        delete prep_stmt;
        delete con;  
        }
    catch(const std::exception& e)
        {
        writeException("updateStage",e.what());
        }
    
    }

Php::Value getStageDetails(Php::Value data) {
    Php::Value response;
    try {
        sql::Connection *con;
        sql::PreparedStatement *prep_stmt;
        sql::ResultSet *res;
        con = General.mysqlConnect(ServerDB);
        
        prep_stmt = con->prepareStatement("select * from greeting_screen where stage_id =?");
        prep_stmt->setString(1, toString(data["stage_id"]));
        res = prep_stmt->executeQuery();
        if (res->next()) {
            sql::ResultSetMetaData *res_meta = res -> getMetaData();
            int columns = res_meta -> getColumnCount();
            for (int i = 1; i <= columns; i++)
                response[res_meta -> getColumnLabel(i)] = string(res->getString(i));
        }
        delete res;
        delete prep_stmt;
        delete con;
    } catch (const std::exception& e) {
        writeException("getStageDetails", e.what());
    }
    return response;
}

Php::Value parcxGreetingScreen(Php::Parameters &params) {
    Php::Value data = params[0];
    int task = data["task"];
    Php::Value response;
    switch (task) {
        case 1:response = uploadMedia(data);
            //writeLog("Main",toString(response));
            break;
        case 2:updateStage(data);
            break;
        case 3:response = getStageDetails(data);
            break;
        
        
    }
    return response;
}


extern "C" {

    PHPCPP_EXPORT void *get_module() {
        static Php::Extension extension("PX_GreetingScreen", "1.0");
        extension.add<parcxGreetingScreen>("parcxGreetingScreen");
        return extension;
    }
}
