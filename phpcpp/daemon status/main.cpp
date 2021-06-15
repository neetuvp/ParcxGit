//make -B
//sudo make install
//service apache2 restart
//Edit   sudo nano /etc/sudoers to allow sudo access to www-data  Add at the end:-       www-data ALL=(ALL)NOPASSWD:ALL(ubuntu)
//daemon ALL=(ALL)NOPASSWD:ALL for  centos

#include <phpcpp.h>
#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<time.h>
#include<ctime>
#include <dirent.h>

#include <sys/types.h>
#include <signal.h>
#include "mysql_connection.h"
#include "mysql_driver.h"
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include<cppconn/prepared_statement.h>
#include "PX_GeneralOperations.h"

using namespace std;
sql::Connection *conn;
sql::Driver *driver;
sql::Statement *stmt;
sql::ResultSet *res;
sql::PreparedStatement *prep;
sql::ResultSetMetaData *rm;
GeneralOperations general;
#define DBServer "DBServer"
#define DBUsername "parcxservice"
#define DBPassword "1fromParcx!19514"
#define DATABASE "parcx_server"




void writeLog(string function,string msg)
    {
    general.writeLog("WebApplication/ApplicationLogs/PX-DaemonStatus-"+general.currentDateTime("%Y-%m-%d"),function,msg);
    }//end of .writeLog


void writeException(string function,string msg)
    {
    general.writeLog("WebApplication/ExceptionLogs/PX-DaemonStatus-"+general.currentDateTime("%Y-%m-%d"),function,msg);
    }//end of .writeException

int getProcIdByName(string procName)
    {
    int pid = -1;

    // Open the /proc directory
    DIR *dp = opendir("/proc");
    if (dp != NULL)
        {
        // Enumerate all entries in directory until process found
        struct dirent *dirp;
        while (pid < 0 && (dirp = readdir(dp)))
            {
            // Skip non-numeric entries
            int id = atoi(dirp->d_name);
            if (id > 0)
                {
                // Read contents of virtual /proc/{pid}/cmdline file
                string cmdPath = string("/proc/") + dirp->d_name + "/cmdline";
                ifstream cmdFile(cmdPath.c_str());
                string cmdLine;
                getline(cmdFile, cmdLine);
                if (!cmdLine.empty())
                    {
                    // Keep first cmdline item which contains the program path
                    size_t pos = cmdLine.find('\0');
                    if (pos != string::npos)
                        cmdLine = cmdLine.substr(0, pos);
                    // Keep program name only, removing the path
                    pos = cmdLine.rfind('/');
                    if (pos != string::npos)
                        cmdLine = cmdLine.substr(pos + 1);
                    // Compare against requested process name
                    if (procName == cmdLine)
                        pid = id;
                    }
                }
            }
        }

    closedir(dp);
    return pid;
    }

void getDaemonList()
    {
    try
        {
        int n=0,pid=0;
        string id = "0";
        conn = general.mysqlConnect(DATABASE);
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select * from daemon_status where status =1");
        if(res->rowsCount()>0)			
		    {	            
            Php::out<<"<div class='card card-primary mb-4 col-md-10 p-0' id='active_daemon' data-status='active_daemon'>"<<std::endl;
			Php::out<<"<div class='card-header'>"<< std::endl;	
            Php::out<<"<div class='d-flex justify-content-between align-items-center'>"<<std::endl;
            Php::out<<"<div class='col'>No</div>"<< std::endl;	
            Php::out<<"<div class='col'>Daemon</div>"<< std::endl;
            Php::out<<"<div class='col'>Status</div>"<< std::endl;                        
            Php::out<<"<div class='col'></div>"<< std::endl;
            Php::out<<"<div class='col'></div></div></div>" << std::endl;	
			n=1;
			
			while (res->next())
			    {		
                id = res->getString("id");
				Php::out<<"<div class='table-view'>"<< std::endl;
                Php::out<< "<div class='card-text'>"<< std::endl;
                Php::out<< "<div class='d-flex justify-content-between align-items-center'>"<< std::endl;
                Php::out<< "<div class='col'>"+to_string(n)+"</div>"<< std::endl;
                Php::out<< "<div class='col-2' id = 'daemon"+id+"'>"+res->getString("daemon_label")+"</div>"<< std::endl;
                pid = getProcIdByName(res->getString("daemon_name"));
                if(pid==-1)
                    {
                    Php::out<< "<div class='col-2' id = 'status"+id+"'><color=red>Inactive</color></div>"<< std::endl;
                    Php::out<< "<div class='col'></div>"<< std::endl;
                    Php::out<< "<div class='col'>"<< std::endl;
                    Php::out<< "<input class='btn btn-success btn-block btn-start-daemon' id ='startbtn"+id+"' type='submit'  value='Start' onclick=startDaemon('"+res->getString("daemon_name")+"') />"<< std::endl;
                    Php::out<< "</div>"<< std::endl;
                    }
                else
                    {
                    Php::out<< "<div class='col-2' id = 'status"+id+"'><color=green>Active</color></div>"<< std::endl;
                    Php::out<< "<div class='col'>"<< std::endl;
                    Php::out<< "<input class='btn btn-danger btn-block btn-stop-daemon' id ='stopbtn"+id+"' type='submit'  value='Stop' onclick=stopDaemon('"+res->getString("daemon_name")+"') />"<< std::endl;
                    Php::out<< "</div>"<< std::endl;
                    Php::out<< "<div class='col'>"<< std::endl;
                    Php::out<< "<input class='btn btn-success btn-block btn-restart-daemon' id ='restartbtn"+id+"' type='submit'  value='Restart' onclick=restartDaemon('"+res->getString("daemon_name")+"') />"<< std::endl;
                    Php::out<< "</div>"<< std::endl;
                    }
                Php::out<< "</div>"<< std::endl;
                Php::out<< "</div>"<< std::endl;
                Php::out<< "</div>"<< std::endl;
                n++;		
			    }
			Php::out <<" </div>"<<std::endl;
		    }
		
		delete res;
		delete stmt;					
		delete conn;
        }
    catch(const std::exception& e)
        {
        writeException("getDaemonList",e.what());
        }       
    }

Php::Value stopDaemon(string daemon)
    {       
    Php::Value result;
    result = "failed";
    string command;  
    writeLog("stopDaemon","Daemon:"+daemon);
    try
        {
        int pid = getProcIdByName(daemon);
        if(pid>0)
            {           
            command = "sudo kill "+to_string(pid);
            int n = system(command.c_str());
            result =n;                       
            }
        else
            {            
            result = "Process does not exist";           
            }
        }
    catch(const std::exception& e)
        {
        writeException("stopDaemon",e.what());
        }
    return result;
    }

string GetPath(string daemon)
    {
	string path="";
	sql::Connection *conn;
	sql::Statement *stmt;
	sql::ResultSet *res;
	try
        {
		conn = general.mysqlConnect(DATABASE);
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select path from daemon_status where daemon_name ='"+daemon+"'");
		if(res->next())
		    {
			path = res->getString("path");
		    }
		delete res;
		delete stmt;
		delete conn;		
	    }
	catch(sql::SQLException &e)
	    {
		writeException("GetPath",e.what());
	    }	
	return path;	
    }

Php::Value startDaemon(string daemon)
    {       
    Php::Value result;
    result = "failed";
    string command,path;   
    writeLog("startDaemon","Daemon:"+daemon);
    try
        {
        int pid = getProcIdByName(daemon);
        if(pid==-1)
            {
			path = GetPath(daemon);
            //command = "cd /opt/parcx/Daemons && sudo ./"+daemon;            
			
			command = "cd "+path+" && sudo ./"+daemon;
            writeLog("startDaemon","Command:"+command);
            int n = system(command.c_str());
            result =n;                                   
            }
        else
            {
            result = "Process already exists";
            }
        }
    catch(const std::exception& e)
        {
        writeException("startDaemon",e.what());
        }
    return result;
    }

Php::Value parcxDaemonStatus(Php::Parameters &params)
    {
    Php::Value response;    
    try
        {
        Php::Value data=params[0];      
        int task=data["task"];   
        
        string daemon=data["daemon"];        
        switch (task)
            {
            case 1: 
                    getDaemonList();
                    break; 
            case 2:                    
                    response=stopDaemon(daemon);  
                    break;
            case 3:
                    response=startDaemon(daemon);
                    break;
            case 4: 
                    response=stopDaemon(daemon); 
                    response=startDaemon(daemon);
                    break;
            }
        }
    catch(const std::exception& e)
        {
        writeException("PX_DaemonStatus",e.what());
        }
    return response;
        
    }

extern "C" 
    {        
    PHPCPP_EXPORT void *get_module() 
        {                
        static Php::Extension extension("PX_DaemonStatus", "1.0");
        extension.add<parcxDaemonStatus>("parcxDaemonStatus");        
        return extension;
        }
    }
