#include "PX_GeneralOperations.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <phpcpp.h>
#include <iostream>
#include <sstream>

#define ServerDB "parcx_server"
#define ReportingDB "parcx_reporting"
#define DashboardDB "parcx_dashboard"
#define dateTimeFormat "%Y-%m-%d %H:%M:%S"

using namespace std;
GeneralOperations General;
sql::ResultSet *res;
sql::Connection *con;
sql::Statement *stmt;
string query;




void writeLog(string function,string message)
    {
    General.writeLog("WebApplication/ApplicationLogs/PX-UserManagement-"+General.currentDateTime("%Y-%m-%d"),function,message);    
    }

void writeException(string function,string message)
    {
    General.writeLog("WebApplication/ExceptionLogs/PX-UserManagement-"+General.currentDateTime("%Y-%m-%d"),function,message); 
    writeLog(function,"Exception: "+message);   
    }

void showSideBar(Php::Value data)
    {
    try
        {
        string userrole_id=data["user_role_id"];
        string base_url=data["url"];
        con = General.mysqlConnect(ServerDB);
        stmt = con->createStatement();
        query="SELECT c.*,group_name,group_icon,group_order_index,group_expand,a.* FROM system_navigation_header a,system_navigation_group b,system_navigation c,system_role_rights d where b.group_id=c.group_id and a.header_id=b.header_id and d.menu_id=c.menu_id and d.user_role_id="+userrole_id+" and d.rr_view=1 ORDER by header_id,group_order_index,menu_order_index";
        res=stmt->executeQuery(query);
        int group_id=0,header_id=0,group_expand=0;
        while(res->next())
            {
            if(header_id!=res->getInt("header_id"))
                {
                header_id=res->getInt("header_id");
                if(group_id!=0 && group_expand==1)
                    {
                    Php::out<<"</ul>"<<endl;
                    Php::out<<"</li>"<<endl;
                    }
                Php::out<<"<li class='nav-header'>"<<res->getString("header_name")<<"</li>"<<endl;                     
                group_id=0;
                }
            if(group_id!=res->getInt("group_order_index"))	
                {                  
                if(group_id!=0 && group_expand==1)
                    {
                    Php::out<<"</ul>"<<endl;
                    Php::out<<"</li>"<<endl;
                    }
                group_id=res->getInt("group_order_index");
                group_expand=res->getInt("group_expand");
                if(group_expand==1)
                    {
                    Php::out<<"<li class='nav-item has-treeview'>"<<endl;
                    Php::out<<"<a href='#' class='nav-link'>"<<endl;
                    Php::out<<"<i class='nav-icon "<<res->getString("group_icon")<<"'></i>"<<endl;
                    Php::out<<"<p>"<<res->getString("group_name")<<"<i class='right fa fa-angle-right'></i></p></a>"<<endl;
                    Php::out<<"<ul class='nav nav-treeview'>"<<endl;
                    }
                }          
            Php::out <<"<li class='nav-item has-treeview w-100'>"<<endl;
            Php::out <<"<a href='"<<base_url<<res->getString("menu_link")<<"' class='nav-link'>"<<endl;                
            Php::out<<"<i class='nav-icon "<<res->getString("menu_icon")<<"'></i>"<<endl;                
            Php::out <<"<p>"<<res->getString("menu_name")<<"</p>"<<endl;
            Php::out <<"</a>"<<endl;
            Php::out <<"</li>"<<endl;                            
            }
        }
    catch(const std::exception& e)
        {
        writeException("showSideBar",e.what());
        }    
    }

Php::Value parcxUserManagement(Php::Parameters &params)
    {
    Php::Value data=params[0];      
    int task=data["task"];   
    Php::Value response;
    switch (task)
        {
        case 1:showSideBar(data);
            break;
        }
    return response;
    }


extern "C" 
	{    
    PHPCPP_EXPORT void *get_module()
	{        
	static Php::Extension extension("PX_UserManagement", "1.0");
        extension.add<parcxUserManagement>("parcxUserManagement");               
        return extension;
		}
	}
