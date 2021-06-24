//sudo yum install libssh,sudo yum install libssh-devel
//sudo g++ -Wall  -std=c++11 -fpic -o PX_TerminalMaintenance_V01_03_02_20210622  PX_TerminalMaintenance_V01_03_02_20210622.cpp PX_GeneralOperations.cpp -L/usr/lib -lmysqlcppconn
//sudo g++ -Wall  -std=c++11 -fpic -o PX_TerminalMaintenance_V01_03_02_20210622  PX_TerminalMaintenance_V01_03_02_20210622.cpp PX_GeneralOperations.cpp -L/opt/parcx/lib -lmysqlcppconn
//use sudo to execute this daemon

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include <cstring>
#include <fstream>
#include <ctime>
#include<vector>
#include <time.h>
#include <syslog.h>
#include<sys/stat.h>
#include<sstream>
#include <dirent.h>
#include<string.h>
#include<unistd.h>
//#include<libssh/libssh.h>
//#include<libssh/sftp.h>
#include <fcntl.h>
#include "mysql_connection.h"
#include "mysql_driver.h"
#include <sys/file.h>
#include <errno.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include<cppconn/prepared_statement.h>


#include "PX_GeneralOperations.h"

/*#define DATABASE_REPORTING "parcx_reporting"
#define DATABASE_SERVER "parcx_server"
#define DATABASE_DASHBOARD "parcx_dashboard"*/
#define DATABASE_TERMINAL "ParcxTerminal"
#define MAX_XFER_BUF_SIZE 10240

using namespace std;
GeneralOperations general;
string start_time="";
string startdate="";
string end_time = "";
string enddate="";
sql::Connection *conn;
sql::Statement *stmt;
sql::Driver *driver;
sql::ResultSet *res;
sql::PreparedStatement *prep_stmt;

string dblist;
string primary_backup_path;
string log_path_primary;
string anpr_image_path;
int anpr_retention;

string db_transaction_deletion;
string retention_raw;
string retention_summary;
string raw_tables;
string summary_tables;

string report_endtime;
string report_starttime;

int dbbackup_enabled=0,logbackup_enabled=0,deleteOpenTransactions_enabled=0,deleteOldDBBackup_enabled=0,deleteOldLogBackup_enabled=0,deleteOldTransactions_enabled=0,deleteOldANPRImages_enabled=0,deleteOldLogFile_enabled=0;


void WriteToLog(string function,string msg)
{
    general.writeLog("Daemons/PXMaintenance/ApplicationLog_PX_Maintenance_Daemon_"+general.currentDateTime("%Y-%m-%d"),function,msg);
}

void WriteException(string function,string msg)
{
    general.writeLog("Daemons/PXMaintenance/ExceptionLog_PX_Maintenance_Daemon_"+general.currentDateTime("%Y-%m-%d"),function,msg);
}

string currentDateMysql()
{
    time_t now=time(NULL);	
    std::tm* timeinfo;
    char buffer[80];
    std::time(&now);
    timeinfo=std::localtime(&now);
    std::strftime(buffer,80,"%Y-%m-%d",timeinfo);		
    string date(buffer);
    return date;
}
string dayFromDate(string date)
{
    struct tm tm;
    char out[30];
    strptime(date.c_str(), "%Y-%m-%d %H:%M:%S", &tm);
    mktime(&tm);
    strftime(out, 30, "%A", &tm);
    string day(out);
    return day;
}
void GetDayclosuretime()
{ 
    try{
        conn = general.mysqlConnect(DATABASE_TERMINAL);
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select setting_name,setting_value from device_settings where setting_name in ('dayclosure_start_time','dayclosure_end_time')");
        while(res->next())
        {
            if(res->getString("setting_name")=="dayclosure_start_time")
            {
            start_time = res->getString("setting_value");
            WriteToLog("getDayclosuretime","dayclosure_start_time :"+start_time);
            }
            else
            {
            end_time = res->getString("setting_value");
            WriteToLog("getDayclosuretime","dayclosure_end_time :"+end_time);
            }
        
        }
        delete res;
        delete stmt;
        delete conn;
    }
   catch (sql::SQLException &e) {
        WriteToLog("getDayclosuretime",e.what());
    }

}


int GetInterval()
{
  int seconds;
  char out[30],out1[30];
  string stime = general.currentDateTime("%Y-%m-%d")+" "+start_time;  //Start Time
  struct tm tm,tm1;
  strptime(stime.c_str(),"%Y-%m-%d %H:%M:%S",&tm);
  time_t e = mktime(&tm);
  time_t n = time(NULL);
 ////current_time in time_t
  seconds = difftime(e,n);

  startdate = stime;
  if(seconds < 0)
  {
    
      tm.tm_mday += 1;
      time_t e = mktime(&tm);
      time_t now = time(NULL);
      seconds = difftime(e,now);
	  strftime(out,30,"%Y-%m-%d %H:%M:%S",&tm);
	  string stime(out);
	  startdate = stime;
    
  }

  strptime(startdate.c_str(),"%Y-%m-%d %H:%M:%S",&tm1);
  tm1.tm_mday += 1;
  tm1.tm_sec  -= 1;
  mktime(&tm1);
  strftime(out1,30,"%Y-%m-%d %H:%M:%S",&tm1);
  string etime(out1);
  enddate = etime;
 
 WriteToLog("Time","start:"+startdate+" end:"+enddate);
  cout<<"Start:"<<startdate<<endl;
  cout<<"End:"<<enddate<<endl;
  cout <<"Seconds:"<<seconds<<endl;
  return seconds;
}



int GetInitialInterval()
{
  int seconds;
  char out[30];
  string stime = currentDateMysql()+" "+start_time;  //Start Time
  struct tm tm;
  strptime(stime.c_str(),"%Y-%m-%d %H:%M:%S",&tm);
  //tm.tm_mday += 1;
  tm.tm_sec  -= 1;
  time_t e = mktime(&tm);
  time_t n = time(NULL);
 //current_time in time_t
  seconds = difftime(e,n);

  strftime(out,30,"%Y-%m-%d %H:%M:%S",&tm);
  string etime(out);      //End Time
  startdate = stime;
  enddate = etime;
  cout<<"Start:"<<stime<<endl;
  cout<<"End:"<<etime<<endl;
 
  //WriteToLog("Seconds1:"+std::to_string(seconds));
  if(seconds < 0)
  {
    
      tm.tm_mday += 1;
      time_t e = mktime(&tm);
      time_t now = time(NULL);
      seconds = difftime(e,now);
    
  }
 // Log("CheckInterval()","Delayflag:"+std::to_string(delayflag));
  cout <<"Seconds:"<<seconds<<endl;
  //WriteToLog("Seconds2:"+std::to_string(seconds));
  return seconds;
}

void GetPreviousDate(string datetime)
{
    char out[30];
    struct tm tm,tm2;
    strptime(datetime.c_str(),"%Y-%m-%d %H:%M:%S",&tm);
    tm.tm_mday -= 1;
    tm.tm_sec += 1;
    mktime(&tm);
    strftime(out,30,"%Y-%m-%d %H:%M:%S",&tm);
    string ptime2(out);      //Previous Time
    report_starttime = ptime2;

    strptime(report_starttime.c_str(),"%Y-%m-%d %H:%M:%S",&tm2);
    tm2.tm_mday += 1;
    tm2.tm_sec -=1;
    mktime(&tm2);
    //tm.tm_sec  -= 1;
    strftime(out,30,"%Y-%m-%d %H:%M:%S",&tm2);
    string ptime1(out); 
    report_endtime = ptime1;
   // return ptime;
}

std::vector<std::string> split(const std::string& s, char delimiter)                                                                                                                          
{                                                                                                                                                                                             
   std::vector<std::string> splits;                                                                                                                                                           
   std::string split;                                                                                                                                                                         
   std::istringstream ss(s);                                                                                                                                                                  
   while (std::getline(ss, split, delimiter))                                                                                                                                                 
   {                                                                                                                                                                                          
      splits.push_back(split);                                                                                                                                                                
   }                                                                                                                                                                                          
   return splits;                                                                                                                                                                             
}


void GetDBBackupSettings()
{
	try{
		conn = general.mysqlConnect(DATABASE_TERMINAL);
		stmt = conn->createStatement();
		res  = stmt->executeQuery("Select setting_name,setting_value,enabled from device_maintenance_terminal where setting_name in('db_list_for_backup','db_backup_path_primary')");
		while(res->next())
		{
			cout<<res->getString("setting_name")<<endl;
			if(res->getString("setting_name")=="db_list_for_backup")
			{
				dblist = res->getString("setting_value");
			}
			else if(res->getString("setting_name")=="db_backup_path_primary")
			{
				primary_backup_path = res->getString("setting_value");
			}			
		}	
		delete res;
		delete stmt;
		delete conn;
	}
	catch(std::exception &e)
	{
		cout<<e.what()<<endl;
		WriteToLog("GetDBBackupSettings",e.what());
		WriteException("GetDBBackupSettings",e.what());
	}
}

void GetANPRDeleteSettings()
{
	try{
		conn = general.mysqlConnect(DATABASE_TERMINAL);
		stmt = conn->createStatement();
		res  = stmt->executeQuery("Select setting_name,setting_value,enabled from device_maintenance_terminal where setting_name in('anpr_image_path','anpr_retention')");
		while(res->next())
		{
			cout<<res->getString("setting_name")<<endl;
			if(res->getString("setting_name")=="anpr_image_path")
			{
				anpr_image_path = res->getString("setting_value");
			}
			else if(res->getString("setting_name")=="anpr_retention")
			{
				anpr_retention = res->getInt("setting_value");
			}
		}	
		delete res;
		delete stmt;
		delete conn;
	}
	catch(std::exception &e)
	{
		cout<<e.what()<<endl;
		WriteToLog("GetANPRDeleteSettings",e.what());
		WriteException("GetANPRDeleteSettings",e.what());
	}
}

string GetSettingsValue(string setting_name)
{
	string setting_value = "";
	try{
		conn = general.mysqlConnect(DATABASE_TERMINAL);
		stmt = conn->createStatement();
		res  = stmt->executeQuery("Select setting_value from device_maintenance_terminal where setting_name = '"+setting_name+"'");
		if(res->next())
		{
			setting_value = res->getString("setting_value");
		}	
		delete res;
		delete stmt;
		delete conn;
	}
	catch(std::exception &e)
	{
		cout<<e.what()<<endl;
		WriteToLog("GetSettingsValue",e.what());
		WriteException("GetSettingsValue",e.what());
	}
	return setting_value;
}


void DBBackup()
{
	try{
		char db_backup_command[250];
		GetDBBackupSettings();
		char delimiter = ',';
		
		//size_t pos = 0;
		string filename;
		auto list = split(dblist,delimiter);
		for(auto database:list)
		{
			cout<<database<<endl;
			system("umask 777");
			filename = database+"_"+general.currentDateTime("%Y-%m-%d");
			sprintf(db_backup_command," sudo /opt/lampp/bin/mysqldump -u %s -h %s -p%s %s | gzip > %s/%s.sql.gz",general.getUser().c_str(),general.getDBServer().c_str(),general.getPassword().c_str(),database.c_str(),primary_backup_path.c_str(),filename.c_str());
			system(db_backup_command);
			WriteToLog("DBBackup","DBBackup for "+database);
			
		}
		
	}
	catch(std::exception &e)
	{
		cout<<e.what()<<endl;
		WriteToLog("DBBackuo",e.what());
		WriteException("DBBackuo",e.what());
	}


}

void GetLogBackupSettings()
{
	try{
		conn = general.mysqlConnect(DATABASE_TERMINAL);
		stmt = conn->createStatement();
		res  = stmt->executeQuery("Select setting_name,setting_value,enabled from device_maintenance_terminal where setting_name in('log_file_backup_path_primary')");
		while(res->next())
		{
			if(res->getString("setting_name")=="log_file_backup_path_primary")
			{
				log_path_primary = res->getString("setting_value");
			}
		}
		delete res;
		delete stmt;
		delete conn;
	}
	catch(std::exception &e)
	{
		cout<<e.what()<<endl;
		WriteToLog("GetLogSettings",e.what());
		WriteException("GetLogSettings",e.what());
	}
}
void LogBackup()
{
	string filename;
	char log_backup_command[250];
	GetLogBackupSettings();
	std::string delimiter = ",";
	filename = "Log_"+currentDateMysql();
	cout<<"Log path:"<<filename<<endl;
	string logfolder = "Logs";
	string actual_logpath = GetSettingsValue("actual_log_path");
	 
	sprintf(log_backup_command,"find %s/%s -type f  -mtime -1 | xargs  tar -czvf %s/%s.gz",actual_logpath.c_str(),logfolder.c_str(),log_path_primary.c_str(),filename.c_str());
	//WriteToLog("LogBackup",log_backup_command);
	//cout << log_backup_command<<endl;

	system(log_backup_command);
	WriteToLog("LogBackup","Log folder backed up");
	

}

void DeleteOpenTransactions()
{
	try
	{
		string interval = GetSettingsValue("open_transaction_retention");
		conn = general.mysqlConnect(DATABASE_TERMINAL);
		stmt = conn->createStatement();
		int n = stmt->executeUpdate("Delete from open_transactions where date(creation_date_time) <(curdate() - interval "+interval+" day)");
		if(n>0)
		{
			WriteToLog("DeleteOpenTransactions","No of records deleted:"+to_string(n));
			cout << " No of records deleted:"+to_string(n)<<endl;		
		}
	}
	catch(std::exception &e)
	{
		cout<<e.what()<<endl;
		WriteToLog("DeleteOpenTransactions",e.what());
		WriteException("DeleteOpenTransactions",e.what());
	}
	
	
}


void DeleteOldDBBackup()
{
	char file_delete_command[250];
	try{
		int retention_time = stoi(GetSettingsValue("db_backup_retention_period"));
		primary_backup_path = GetSettingsValue("db_backup_path_primary");
		cout<<"Retention:"<<retention_time<<endl;
		sprintf(file_delete_command,"sudo find %s -name '*.sql.gz' -mtime +%d -type f -exec rm {} \\;",primary_backup_path.c_str(),retention_time-1);
		//WriteToLog("DeleteOldDBBackup",file_delete_command);
		system(file_delete_command);
		WriteToLog("DeleteOldDBBackup","DB Backup older than "+to_string(retention_time)+ " days will be deleted");
	}
	catch(exception &e){
		cout<<"in catch: "<<e.what()<<endl;
		WriteException("DeleteOldDBBackup",e.what());
	}
}



void DeleteOldLogBackup()
{
	char file_delete_command[250];
	try{
		int retention_time = stoi(GetSettingsValue("log_backup_retention_period"));
		log_path_primary = GetSettingsValue("log_file_backup_path_primary");
		cout<<"Retention:"<<retention_time<<endl;
		sprintf(file_delete_command,"sudo find %s -name '*.gz' -mtime +%d -type f -exec rm {} \\;",log_path_primary.c_str(),retention_time-1);
		//WriteToLog("DeleteOldLogBackup",file_delete_command);
		system(file_delete_command);
		WriteToLog("DeleteOldLogBackup","Log Backup older than "+to_string(retention_time)+ " days will be deleted");

	}
	catch(exception &e){
		cout<<"in catch: "<<e.what()<<endl;
		WriteException("DeleteOldLogBackup",e.what());
	}
}

void DeleteOldLogFile()
{
	char file_delete_command[250];
	try{
		int retention_time = stoi(GetSettingsValue("log_file_retention_period"));
		string actual_log_path = GetSettingsValue("actual_log_path")+"/Logs";
		cout<<"Retention:"<<retention_time<<endl;
		sprintf(file_delete_command,"sudo find %s -name '*.log' -mtime +%d -type f -exec rm {} \\;",actual_log_path.c_str(),retention_time-1);
		WriteToLog("DeleteOldLogFile",file_delete_command);
		system(file_delete_command);
		WriteToLog("DeleteOldLogFile","Log files older than "+to_string(retention_time)+ " days will be deleted");
	}
	catch(exception &e){
		cout<<"in catch: "<<e.what()<<endl;
		WriteException("DeleteOldLogBackup",e.what());
	}
	
}


void GetDeleteTransactionSettings()
{
	
	try{
		conn = general.mysqlConnect(DATABASE_TERMINAL);
		stmt = conn->createStatement();
		res  = stmt->executeQuery("Select setting_name,setting_value,enabled from device_maintenance_terminal where setting_name in('db_for_transaction_deletion','transaction_retention_raw','transaction_retention_summary','list_tables_for_transaction_deletion_raw','list_tables_for_transaction_deletion_summary')");
		while(res->next())
		{
			if(res->getString("setting_name")=="db_for_transaction_deletion")
			{
				db_transaction_deletion = res->getString("setting_value");
			}
			else if(res->getString("setting_name")=="transaction_retention_raw")
			{
				retention_raw = res->getString("setting_value");
			}
			else if(res->getString("setting_name")=="transaction_retention_summary")
			{
				retention_summary = res->getString("setting_value");
			}
			else if(res->getString("setting_name")=="list_tables_for_transaction_deletion_raw")
			{
				raw_tables = res->getString("setting_value");
			}
			else if(res->getString("setting_name")=="list_tables_for_transaction_deletion_summary")
			{
				summary_tables = res->getString("setting_value");
			}
		}
		delete res;
		delete stmt;
		delete conn;
	}
	catch(std::exception &e)
	{
		cout<<e.what()<<endl;
		WriteToLog("GetDeleteTransactionSettings",e.what());
		WriteException("GetDeleteTransactionSettings",e.what());
	}
}


void DeleteOldTransactions()
{
	GetDeleteTransactionSettings();
	try
	{
		string table,date_field;
		//size_t pos = 0;
		char delimiter = ',';

		conn = general.mysqlConnect(db_transaction_deletion);
		stmt = conn->createStatement();

		//for raw transactions	
		auto list1 = split(raw_tables,delimiter);
		for(auto table:list1)
		{
			cout <<table<<endl;
			//WriteToLog("DeleteOldTransactions","Table:"+table);
			if(table=="revenue_payments")
			{
				date_field = "record_creation_time";
			}
			else{
				date_field = "date_time";
			}
			int n = stmt->executeUpdate("Delete from "+table+" where date("+date_field+") <(curdate() - interval "+retention_raw+" day)");
			if(n>0)
				WriteToLog("DeleteOldTransactions:raw","No of rows deleted for "+table+" :"+to_string(n));
		}


		auto list2 = split(summary_tables,delimiter);
		for(auto table:list2)
		{
			cout <<table<<endl;
			//WriteToLog("DeleteOldTransactions","Table:"+table);
			int n = stmt->executeUpdate("Delete from "+table+" where date(report_date) <(curdate() - interval "+retention_summary+" day)");
			if(n>0)
				WriteToLog("DeleteOldTransactions:summary","No of rows deleted for "+table+" :"+to_string(n));
		}

		/*while ((pos = raw_tables.find(delimiter)) != std::string::npos) {
			table = raw_tables.substr(0, pos);
			WriteToLog("DeleteOldTransactions","Table:"+table);
			int n = stmt->executeUpdate("Delete from "+table+" where date(date_time) <(curdate() - interval "+retention_raw+" day)");
			WriteToLog("DeleteOldTransactions:raw","No of rows deleted for "+table+" :"+to_string(n));
			raw_tables.erase(0, pos + delimiter.length());
		}

		while ((pos = summary_tables.find(delimiter)) != std::string::npos) {
			table = summary_tables.substr(0, pos);
			WriteToLog("DeleteOldTransactions","Table:"+table);
			int n = stmt->executeUpdate("Delete from "+table+" where date(report_date) <(curdate() - interval "+retention_summary+" day)");
			WriteToLog("DeleteOldTransactions:summary","No of rows deleted for "+table+" :"+to_string(n));
			summary_tables.erase(0, pos + delimiter.length());	
		}*/	
	}
	catch(std::exception &e)
	{
		cout<<e.what()<<endl;
		WriteToLog("DeleteOldTransactions",e.what());
		WriteException("DeleteOldTransactions",e.what());
	}
	
}


void DeleteOldANPRImages()
{
	char file_delete_command[250];
	DIR *dir,*sub_dir;
	struct dirent *ent,*ent1;
	string del_anpr_path,del_anpr_path_final;
	try{
		GetANPRDeleteSettings();
			if ((dir = opendir (anpr_image_path.c_str())) != NULL) {
				while ((ent = readdir (dir)) != NULL) {
					if(ent->d_type==DT_DIR && (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0))
					{
						del_anpr_path = anpr_image_path+"/"+ent->d_name;
						if ((sub_dir = opendir (del_anpr_path.c_str())) != NULL) {
							while ((ent1 = readdir (sub_dir)) != NULL) {
								if(ent1->d_type==DT_DIR && (strcmp(ent1->d_name, ".")!=0 && strcmp(ent1->d_name, "..")!=0))
								{
									del_anpr_path_final = del_anpr_path+"/"+ent1->d_name;
									sprintf(file_delete_command,"sudo find %s -type d -mtime +%d -exec rm -rf {} \\;",del_anpr_path_final.c_str(),anpr_retention-1);
									//WriteToLog("DeleteOldANPRImages",file_delete_command);
									system(file_delete_command);
									WriteToLog("DeleteOldANPRImages","ANPR Images older than "+to_string(anpr_retention)+ " days will be deleted");

								}
							}
						}
					}
				}
			}
	}
	catch(exception &e){
		cout<<"in catch: "<<e.what()<<endl;
		WriteException("DeleteOldLogBackup",e.what());
	}
}
void UpdateProcessId(int pid)
{
	try{
		conn = general.mysqlConnect(DATABASE_TERMINAL);
		stmt = conn->createStatement();
		stmt->executeUpdate("Update daemon_status set pid = "+to_string(pid)+",start_time=CURRENT_TIMESTAMP() where daemon_label = 'Maintenance Daemon'");
		
	}
	catch(exception &e){
		cout<<"UpdateProcessId: "<<e.what()<<endl;
		WriteException("UpdateProcessId",e.what());
	}
	delete stmt;
	delete conn;
	
	
}

void GetDeviceMaintenanceSettings()
{
	try{
		conn = general.mysqlConnect(DATABASE_TERMINAL);
		stmt = conn->createStatement();
		res  = stmt->executeQuery("Select setting_name,setting_value,enabled from device_maintenance_terminal where setting_name in('dbbackup_enabled','logbackup_enabled','deleteOpenTransactions_enabled','deleteOldDBBackup_enabled','deleteOldLogBackup_enabled','deleteOldTransactions_enabled','deleteOldANPRImages_enabled','deleteOldLogFile_enabled')");
		while(res->next())
		{
			if(res->getString("setting_name")=="dbbackup_enabled")
			{
				dbbackup_enabled = res->getInt("setting_value");
			}
			else if(res->getString("setting_name")=="logbackup_enabled")
			{
				logbackup_enabled = res->getInt("setting_value");
			}
			else if(res->getString("setting_name")=="deleteOpenTransactions_enabled")
			{
				deleteOpenTransactions_enabled = res->getInt("setting_value");
			}
			else if(res->getString("setting_name")=="deleteOldDBBackup_enabled")
			{
				deleteOldDBBackup_enabled = res->getInt("setting_value");
			}
			else if(res->getString("setting_name")=="deleteOldLogBackup_enabled")
			{
				deleteOldLogBackup_enabled = res->getInt("setting_value");
			}
			else if(res->getString("setting_name")=="deleteOldTransactions_enabled")
			{
				deleteOldTransactions_enabled = res->getInt("setting_value");
			}
			else if(res->getString("setting_name")=="deleteOldANPRImages_enabled")
			{
				deleteOldANPRImages_enabled = res->getInt("setting_value");
			}
			else if(res->getString("setting_name")=="deleteOldLogFile_enabled")
			{
				deleteOldLogFile_enabled = res->getInt("setting_value");
			}
			
		}
		delete res;
		delete stmt;
		delete conn;
	}
	catch(std::exception &e)
	{
		cout<<e.what()<<endl;
		WriteToLog("GetLogSettings",e.what());
		WriteException("GetLogSettings",e.what());
	}
}
//*****************************************Main*****************************************************
int main(void)
{
    int pid_file = open("px_maintenance.lock", O_CREAT | O_RDWR, 0644);
	int rc = flock(pid_file, LOCK_EX | LOCK_NB);
	if(rc) {
		if(EWOULDBLOCK == errno)
		{
			WriteToLog("Main: " , "Another Instance is running");
			cout<<"Another Instance is running"<<endl; // another instance is running
		}
	}
	else {
		pid_t pid,sid;
		pid = fork();
		if(pid > 0)
		{
			WriteToLog("Main: " , "Maintenance Daemon Version : PX_TerminalMaintenance_V01_03_02_20210622    Process Id:"+to_string(pid));
			cout<<"Maintenance Daemon Version : PX_TerminalMaintenance_V01_03_02_20210622"<<"Process Id :"<<pid<<endl;	
			UpdateProcessId(pid);
			exit(EXIT_SUCCESS);
		}
		else if(pid < 0)
		{
			exit(EXIT_FAILURE);
		}
		umask(0);
		openlog("daemon-named",LOG_NOWAIT|LOG_PID,LOG_USER);
		syslog(LOG_NOTICE,"Successfully started Maintenance Service");
		sid = setsid();
		if(sid<0)
		{
			exit(EXIT_FAILURE);
		}
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);

		int SLEEP_INTERVAL;
		GetDayclosuretime();
		SLEEP_INTERVAL = GetInitialInterval();
		WriteToLog("Main","Daemon Sleeps for "+std::to_string(SLEEP_INTERVAL)+" seconds");
		WriteToLog("Main","StartDate:"+startdate+" EndDate:"+enddate);
		GetDeviceMaintenanceSettings();
		while(1)
		{
			WriteToLog("","******************************************Log******************************************");
			if(dbbackup_enabled==1)
			{
				//WriteToLog("","##########DBBackup##########");
				DBBackup();
				sleep(5);				
			}
			if(logbackup_enabled==1)
			{
				//WriteToLog("","##########LogBackup##########");
				LogBackup();
				sleep(5);			
			}
			if(deleteOpenTransactions_enabled==1)
			{
				//WriteToLog("","##########DeleteOpenTransactions##########");
				DeleteOpenTransactions();
				sleep(5);			
			}
			if(deleteOldDBBackup_enabled==1)
			{
				//WriteToLog("","##########DeleteOldDBBackup##########");
				DeleteOldDBBackup();
				sleep(5);				
			}
			if(deleteOldLogBackup_enabled==1)
			{
				//WriteToLog("","##########DeleteOldLogBackup##########");
				DeleteOldLogBackup();
				sleep(5);
			}
			if(deleteOldLogFile_enabled==1)
			{
				//WriteToLog("","##########DeleteOldLogFile##########");
				cout<<"##########DeleteOldLogFile##########"<<endl;
				DeleteOldLogFile();
				sleep(5);
			}
			if(deleteOldTransactions_enabled==1)
			{
				//WriteToLog("","##########DeleteOldTransactions##########");
				DeleteOldTransactions();
				sleep(5);			
			}
			if(deleteOldANPRImages_enabled==1)
			{
				//WriteToLog("","##########DeleteOldANPRImages##########");
				DeleteOldANPRImages();			
			}
			
			SLEEP_INTERVAL = GetInterval();//Add 30 min to dayclosure time;
			WriteToLog("Main","Daemon Sleeps for "+std::to_string(SLEEP_INTERVAL)+" seconds");
			WriteToLog("Main","StartDate:"+startdate+" EndDate:"+enddate);
			sleep(SLEEP_INTERVAL);
			//SLEEP_INTERVAL = CheckInterval();
			//WriteToLog("Main","Daemon Sleeps for "+std::to_string(SLEEP_INTERVAL)+" seconds");
			//WriteToLog("Main","StartDate:"+startdate+" EndDate:"+enddate);
			cout<<"Start:"+startdate+" End:"+enddate<<endl;
		}
	}
    return 0;
}
