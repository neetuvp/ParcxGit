//sudo g++ -std=c++11 -Wall -o PX_DaemonMonitor_V01_03_05_20210527 PX_DaemonMonitor_V01_03_05_20210527.cpp PX_GeneralOperations.cpp  -lmysqlcppconn  -I/opt/lampp/include/ -L/opt/parcx/lib/

#include <syslog.h>
#include<sys/stat.h>
#include<unistd.h>
#include<iostream>
#include<stdlib.h>
#include<fstream>
#include <dirent.h>

#include "PX_GeneralOperations.h"
#include <sys/file.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;
sql::ResultSet *res;

GeneralOperations general;

void WriteToLog(string function, string msg) 
    {    
    general.writeLog("Daemons/PXDaemonMonitor/ApplicationLog_PX_DaemonMonitor_" + general.currentDateTime("%Y-%m-%d"), function, msg);
    }

void WriteException(string function, string msg) 
    {
    WriteToLog(function,msg);
    general.writeLog("Daemons/PXDaemonMonitor/ExceptionLog_PX_DaemonMonitor_" + general.currentDateTime("%Y-%m-%d"), function, msg);
    }

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

void UpdateProcessId(int pid) 
    {
    sql::Connection *con;
    sql::Statement *stmt;    
    try 
        {
        con = general.mysqlConnect("pxcloud_server");
        stmt = con->createStatement();
        stmt->executeUpdate("Update daemon_status set pid = " + to_string(pid) + ",start_time=CURRENT_TIMESTAMP() where daemon_name = 'PX_DaemonMonitor_V01_03_05_20210527'");       
        } 
    catch (exception &e) 
        {        
        WriteException("UpdateProcessId", e.what());
        }
    delete stmt;
    delete con;
    }

void getDaemonDetails() 
    {
    sql::Connection *con;
    sql::Statement *stmt;    
    try 
        {
        con = general.mysqlConnect("pxcloud_server");
        stmt = con->createStatement();        
        res=stmt->executeQuery("select daemon_name,path from daemon_status where status=1 and daemon_name!='PX_DaemonMonitor_V01_03_05_20210527'");
        } 
    catch (exception &e) 
        {        
        WriteException("getDaemonDetails", e.what());
        }
    delete stmt;
    delete con;
    }

void daemonMonitor()
    {
    int pid=0; 
    string daemon,command,path;  
     WriteToLog("daemonMonitor","===================DAEMONS===================");  
    while(res->next())
        {
        daemon=res->getString("daemon_name");
        WriteToLog("daemonMonitor",daemon) ;  
        }

    WriteToLog("daemonMonitor","=============================================");
    while(true)   
        {
        try    
            {
            res->beforeFirst();            
            while(res->next())
                {
                daemon=res->getString("daemon_name"); 
                path=res->getString("path");    
                pid = getProcIdByName(daemon);
                if(pid==-1)
                    {                    
                    WriteToLog(daemon,"Not running") ; 
                    command = "cd "+path+" &&  ./"+daemon;
                    WriteToLog("start daemon","Command:"+command);
                    system(command.c_str());
                    }
                } 
                                     
            }
        catch(const std::exception& e)
            {
            WriteException("daemonMonitor", e.what());
            }
        sleep(15);
        }
    }

int main(void) 
    {  
    int pid_file = open("PX_Daemon_Monitor_Daemon.lock", O_CREAT | O_RDWR, 0644);
    int rc = flock(pid_file, LOCK_EX | LOCK_NB);
    if (rc) 
        {
        if (EWOULDBLOCK == errno) 
            {
            WriteToLog("Main: ", "Another Instance is running");
            cout << "Another Instance is running" << endl; // another instance is running
            }
        } 
    else 
        {
        pid_t pid, sid;
        pid = fork();
        if (pid > 0) 
            {
            WriteToLog("Main: ", "Cloud Upload Daemon Version : PX_DaemonMonitor_V01_03_05_20210527    Process Id:" + to_string(pid));
            cout << "Cloud Upload Daemon Version : PX_DaemonMonitor_V01_03_05_20210527" << "Process Id :" << pid << endl;

            UpdateProcessId(pid);
            exit(EXIT_SUCCESS);
            } 
        else if (pid < 0)   
            {
            exit(EXIT_FAILURE);
            }
        umask(0);
        openlog("daemon-named", LOG_NOWAIT | LOG_PID, LOG_USER);
        syslog(LOG_NOTICE, "Successfully started PX_DaemonMonitor");
        WriteToLog("", "Successfully started PX_DaemonMonitor");
        sid = setsid();
        if (sid < 0) 
            {
            exit(EXIT_FAILURE);
            }
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);   
        
        getDaemonDetails();
        daemonMonitor();
        }
    return 0;
}
