#include "PX_GeneralOperations.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <phpcpp.h>
#include <iostream>
#include <sstream>

#define ServerDB "parcx_server"
#define ReportingDB "parcx_reporting"
#define DashboardDB "parcx_dashboard"
#define dateFormat "%Y-%m-%d"

using namespace std;
GeneralOperations General;
sql::ResultSet *res;
sql::Connection *con;
sql::Statement *stmt;
string query;




void writeLog(string function,string message)
    {
    General.writeLog("WebApplication/ApplicationLogs/PX-UserManagement-"+General.currentDateTime(dateFormat),function,message);    
    }

void writeException(string function,string message)
    {
    General.writeLog("WebApplication/ExceptionLogs/PX-UserManagement-"+General.currentDateTime(dateFormat),function,message); 
    writeLog(function,"Exception: "+message);   
    Php::out<<query<<endl;
    }

void showSideBar(Php::Value data)
    {
    try
        {
        string userrole_id=data["user_role_id"];
        string base_url=data["url"];
        con = General.mysqlConnect(ServerDB);
        stmt = con->createStatement();
        query="SELECT c.*,group_name,group_icon,group_order_index,group_expand,a.* FROM system_navigation_header a,system_navigation_group b,system_navigation c,system_role_rights d where b.group_id=c.group_id and a.header_id=b.header_id and d.menu_id=c.menu_id and d.user_role_id="+userrole_id+" and d.rr_view=1 and menu_status=1 ORDER by header_id,group_order_index,menu_order_index";
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
        delete res;
        delete stmt;
        delete con;
        }
    catch(const std::exception& e)
        {
        writeException("showSideBar",e.what());
        }    
    }

void showNavigationList()
    {
    try
        {                      
        con = General.mysqlConnect(ServerDB);
        stmt = con->createStatement();                
        query="SELECT menu_id,group_name,a.group_id,menu_name FROM system_navigation_group a INNER JOIN system_navigation b ON a.group_id=b.group_id where menu_status=1 order by group_id desc";
        res=stmt->executeQuery(query);
        int group_id=0;
        
        while(res->next())
            {
            if(group_id!=res->getInt("group_id"))
                {
                group_id=res->getInt("group_id");
                Php::out<<"<label>"<<res->getString("group_name")<<"</label>"<<endl;
                }
            Php::out<<"<div class='form-group custom-control custom-checkbox'>"<<endl;
            Php::out<<"<input type='checkbox' class='custom-control-input' id='"<<res->getInt("menu_id")<<"'>"<<endl;
            Php::out<<"<label class='custom-control-label' for='"<<res->getString("menu_id")<<"'>"<<res->getString("menu_name")<<"</label></div>"<<endl;
            }
        delete res;
        delete stmt;
        delete con;        
        }
    catch(const std::exception& e)
        {
        writeException("showNavigationList",e.what());
        } 
    }

void showUserRoleNavigationList()
    {
    try
        {        
        sql::ResultSet *role,*rights;
        con = General.mysqlConnect(ServerDB);
        stmt = con->createStatement();                 
        query="SELECT menu_id,group_name,a.group_id,menu_name FROM system_navigation_group a INNER JOIN system_navigation b ON a.group_id=b.group_id where menu_status=1 order by group_id desc";
        res=stmt->executeQuery(query);
        int group_id=0;
        
        Php::Value menu;
        while(res->next())            
            menu[res->getInt("menu_id")]=0;            
        
        Php::Value start_menu=menu;                        
        
        query="select * from system_user_role";
        role=stmt->executeQuery(query);
        string checked;
        while(role->next())
            {            
            Php::out<<"<div id='parent'>"<<endl;
            Php::out<<"<div id='roleholder' class='accordion'>"<<role->getString("user_role_name")<<endl;
            Php::out<<"<div class='panel' style='display: none;'><form>"<<endl;
            query="select menu_id,rr_view from system_role_rights where rr_view=1 and user_role_id="+role->getString("user_role_id");
            rights=stmt->executeQuery(query);
            menu=start_menu;
            while(rights->next())                
                menu[rights->getInt("menu_id")]=1;                
            res->beforeFirst();
            
            Php::out<< "<div class='row'>"<<endl;
            Php::out<< "<div class='col form-group'>"<<endl;
            Php::out<< "<label for=''>User Role Name</label>"<<endl;
            Php::out<< "<input type='text' disabled class='form-control' id='user_role_name_"<<role->getString("user_role_id")<<"' value='"<<role->getString("user_role_name")<<"' required="">"<<endl;
            Php::out<< "</div></div>"<<endl;
                                
            while(res->next())
                {
                if(group_id!=res->getInt("group_id"))
                    {
                    group_id=res->getInt("group_id");
                    Php::out<<"<label>"<<res->getString("group_name")<<"</label>"<<endl;
                    }
                
                checked="";
                if(menu[res->getInt("menu_id")]==1)
                    checked="checked='checked'";
                
                Php::out<<"<div class='form-group custom-control custom-checkbox'>"<<endl;
                Php::out<<"<input type='checkbox' class='custom-control-input' disabled "<<checked<<" id='"<<role->getString("user_role_id")<<"-"<<res->getInt("menu_id")<<"'>"<<endl;
                Php::out<<"<label class='custom-control-label' for='"<<role->getString("user_role_id")<<"-"<<res->getString("menu_id")<<"'>"<<res->getString("menu_name")<<"</label></div>"<<endl;
                }
            Php::out<<"<input type='button' data-id='"<<role->getString("user_role_id")<<"' class='edit-user-role signUp btn btn-block btn-info mt-2 btn-lg mb-2' value='Edit'>"<<endl;
            Php::out<<"</form></div></div></div>"<<endl;
            }  
        delete role;
        delete res;
        delete stmt;
        delete con;        
        }
    catch(const std::exception& e)
        {
        writeException("showUserRoleNavigationList",e.what());
        } 
    }

Php::Value createUserRole(Php::Value data)
    {
    string msg = "Failed";       
    try
        {
        string name=data["name"];
        Php::Value menu=data["menu"];
           
        con = General.mysqlConnect(ServerDB);
        stmt = con->createStatement();
        query="insert into system_user_role(user_role_name)values('"+name+"')";
        stmt->executeUpdate(query);
        res=stmt->executeQuery("select user_role_id from system_user_role where user_role_name='"+name+"'");
        if(res->next())
            {
            string user_role_id=res->getString("user_role_id");
            for(int i=0;i<menu.size();i++)
                {
                string menu_id=menu[i];
                query="insert into system_role_rights(user_role_id,menu_id,rr_view,rr_edit ,rr_delete)values("+user_role_id+","+menu_id+",1,1,1)";
                stmt->executeUpdate(query);
                }
            }
        delete res;
        delete stmt;
        delete con;
        msg="Successfull";
        }
    catch(const std::exception& e)
        {
        writeException("createUserRole",e.what());
        }
    return msg;
    }

Php::Value updateUserRoleRights(Php::Value data)
    {
    string msg = "Failed";       
    try
        {
        string user_role_id=data["user_role_id"];
        string user_role_name=data["user_role_name"];
        Php::Value menu=data["menu"];
           
        con = General.mysqlConnect(ServerDB);
        stmt = con->createStatement();  
        
        if(user_role_name!="")
            {
            query="update system_user_role set user_role_name='"+user_role_name+"' where user_role_id="+user_role_id;
            stmt->executeUpdate(query);
            }
         
        query="update system_role_rights set rr_view=0,rr_edit=0,rr_delete=0 where user_role_id="+user_role_id;
        stmt->executeUpdate(query);
                
        string id;
        for(int i=0;i<menu.size();i++)
            {
            string menu_id=menu[i];    
            query="select role_rights_id from system_role_rights where menu_id="+menu_id+" and user_role_id="+user_role_id;
            res=stmt->executeQuery(query);
            if(res->next())
                {
                id=res->getString("role_rights_id");
                query="update system_role_rights set rr_view=1,rr_edit=1,rr_delete=1 where role_rights_id="+id;
                delete res;
                }
            else    
                query="insert into system_role_rights(user_role_id,menu_id,rr_view,rr_edit ,rr_delete)values("+user_role_id+","+menu_id+",1,1,1)";
            stmt->executeUpdate(query);
            }
       
        delete stmt;
        delete con;
        msg="Successfull";
        }
    catch(const std::exception& e)
        {
        writeException("updateUserRoleRights",e.what());
        }
    return msg;
    }

void userRoleDropdown()
    {
    try
        {                      
        con = General.mysqlConnect(ServerDB);
        stmt = con->createStatement();                
        query="SELECT * from system_user_role";
        res=stmt->executeQuery(query);                
        while(res->next())           
            Php::out<<"<option value='"<<res->getString("user_role_id")<<"'>"<<res->getString("user_role_name")<<"</option>"<<endl;                        
        delete res;
        delete stmt;
        delete con;        
        }
    catch(const std::exception& e)
        {
        writeException("userRoleDropdown",e.what());
        } 
    }

Php::Value insertUpdateUsers(Php::Value json)
    {
    string msg = "Failed";    
    try
        {
        string id=json["id"];
        string full_name=json["full_name"];
        string user_name=json["user_name"];
        string email=json["email"];
        string password=json["password"];
        string company_name=json["company_name"];
        string phone=json["phone"];
        string start_date=json["start_date"];
        string expiry_date=json["expiry_date"];
        string user_role=json["user_role"];
        string language=json["language"];
        
        con = General.mysqlConnect(ServerDB);
        stmt = con->createStatement();                
        if(id=="")
            {
            query="select user_id from system_users where username='"+user_name+"'";
            res=stmt->executeQuery(query);
            if(res->next())
                {
                msg="Username exist try with another username";
                delete res;
                delete stmt;
                delete con;
                return msg;
                }
            }
        
        if(id=="")
            query="insert into system_users(operator_name,username,company_name,email,phone,password,user_role_id,language,validity_from_date,validity_to_date,account_status)values('"+full_name+"','"+user_name+"','"+company_name+"','"+email+"','"+phone+"',md5('"+password+"'),"+user_role+",'"+language+"','"+start_date+"','"+expiry_date+"',1)";
        else
            query="update system_users set operator_name='"+full_name+"',username='"+user_name+"',company_name='"+company_name+"',email='"+email+"',phone='"+phone+"',user_role_id="+user_role+",language='"+language+"',validity_from_date='"+start_date+"',validity_to_date='"+expiry_date+"' where user_id="+id;
        stmt->executeUpdate(query);
        
        delete stmt;
        delete con;
        msg = "Successfull";
        }
    catch(const std::exception& e)
        {
        writeException("insertUpdateUsers",e.what());
        Php::out<<query<<endl;
        } 
    return msg;
    }

void showUsersList()
    {
    try
        {
        Php::Value role;
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select * from system_user_role");
        while(res->next())
            {
            role[res->getInt("user_role_id")]=string(res->getString("user_role_name"));
            }
        delete res;
        res=stmt->executeQuery("select * from system_users");
        if(res->rowsCount()>0)
            {
            //Php::out<<"<table class='table table-blue'>"<<endl;
            Php::out << "<thead>" << std::endl;
            Php::out<<"<tr>"<<endl;
            Php::out<<"<th>User name</th>"<<endl;
            Php::out<<"<th>Name</th>"<<endl; 
            Php::out<<"<th>User Role</th>"<<endl;
            Php::out<<"<th>Language</th>"<<endl;       
            Php::out<<"<th>From</th>"<<endl;           
            Php::out<<"<th>To</th>"<<endl;           
            Php::out<<"<th></th>"<<endl;		           
            Php::out<<"<th></th>"<<endl;
            Php::out<<"<th></th>"<<endl;
            Php::out<<"</tr>"<<endl;	
            Php::out << "</thead>" << std::endl;		
            }
        while(res->next())
            {
            Php::out<<"<tr data-id='"<<res->getString("user_id")<<"'>"<<endl;                       
            Php::out<<"<td>"+res->getString("username")+"</td>"<<endl;
            Php::out<<"<td>"+res->getString("operator_name")+"</td>"<<endl;            
            Php::out<<"<td>"<<role[res->getInt("user_role_id")]<<"</td>"<<endl;
            Php::out<<"<td>"+res->getString("language")+"</td>"<<endl;
            Php::out<<"<td>"+res->getString("validity_from_date")+"</td>"<<endl;
            Php::out<<"<td>"+res->getString("validity_to_date")+"</td>"<<endl;            
            
            Php::out << "<td>"<< std::endl;
            if(res->getInt("account_status")==1)
                Php::out << "<button type='button' class='col btn btn-danger user-enable-disable-btn'>Disable</button>"<< std::endl;
            else
                Php::out << "<button type='button' class='col btn btn-success user-enable-disable-btn'>Enable</button>"<< std::endl;
            Php::out << "</td>"<< std::endl;

            Php::out << "<td>"<< std::endl;
            Php::out << "<button type='button' class='col btn btn-info user-edit' ><i class='fas fa-edit'></i>Edit</button>"<< std::endl;            
            Php::out << "</td>"<< std::endl;
            Php::out << "<td>"<< std::endl;
            Php::out << "<button type='button' class='col btn btn-info user-change-password' ><i class='fas fa-edit'></i>Change Password</button>"<< std::endl;            
            Php::out << "</td>"<< std::endl;
            Php::out<<"</tr>"<<endl;	
            }
        //Php::out<<"</table>"<<endl;	
        delete res;    
        delete stmt;
		delete con;  
        }
    catch(const std::exception& e)
        {
        writeException("showUsersList",e.what());
        }
    
    }

Php::Value enableDisableUser(Php::Value json)
    {
    string msg = "Failed";    
    try
        {
        string status=json["status"];
        string id=json["id"];
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        query="update system_users set account_status="+status+" where user_id="+id;
        stmt->executeUpdate(query);        
        delete stmt;
        delete con;       
        msg = "Successfull";	
        }
    catch(const std::exception& e)
        {
        writeException("enableDisableUser",e.what());
        }
    return msg;
    }

Php::Value changePassword(Php::Value json)
    {
    string msg = "Failed";    
    try
        {        
        string id=json["id"];
        string current_password=json["current_password"];
        string new_password=json["new_password"];
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        query="select user_id from system_users where user_id="+id+" and password=md5('"+current_password+"')";
        res=stmt->executeQuery(query);
        if(res->next())
            {
            query="update system_users set password=md5('"+new_password+"') where user_id="+id;
            stmt->executeUpdate(query);        
            }
        else
            msg="current password is wrong";
        delete stmt;
        delete con;       
        msg = "Successfull";	
        }
    catch(const std::exception& e)
        {
        writeException("changePassword",e.what());
        }
    return msg;
    }

Php::Value getUserDetails(Php::Value json)
    {
    Php::Value response;    
    try
        {
        string id=json["id"];        
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        query="select * from system_users where user_id="+id;
        res=stmt->executeQuery(query);
        if(res->next())
            {
            sql::ResultSetMetaData *res_meta = res -> getMetaData();
            int columns = res_meta -> getColumnCount();   
            for (int i = 1; i <= columns; i++) 							
                    response[res_meta -> getColumnLabel(i)]=string(res->getString(i));				
            }
        delete res;    
        delete stmt;
        delete con;
        }
    catch(const std::exception& e)
        {
        writeException("getUserDetails",e.what());
        } 
    return response;       
    }

Php::Value loginUser(Php::Value json)
    {
    Php::Value response;
    string message= "Failed";    
    try
        {                
        string user_name=json["user_name"];
        string password=json["password"];
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        query="SELECT md5('"+password+ "') as user_password,password,validity_from_date,validity_to_date,operator_name,system_users.user_role_id,user_role_name,account_status,user_id,language  FROM system_users INNER JOIN system_user_role ON system_users.user_role_id=system_user_role.user_role_id where username= '"+user_name+"'  LIMIT 1";
        res=stmt->executeQuery(query);
        if(res->next())
            {            
            string current_date = General.currentDateTime(dateFormat);
            string expiry_date = res->getString("validity_to_date");
            string start_date = res->getString("validity_from_date");
            string user_password=res->getString("user_password");
            password=res->getString("password");
            
            if(res->getInt("account_status")==0)
                message="Account disabled";
            else if(password!=user_password)
                message="Wrong password";
            else if (expiry_date.compare(current_date) < 0) 
                message="User validity expired";  
            else if (start_date.compare(current_date) > 0) 
               message = "User validity not started";
            else
                {
                message="Success";
                response["operator_name"]=string(res->getString("operator_name"));
                response["user_role_id"]=string(res->getString("user_role_id"));
                response["user_role_name"]=string(res->getString("user_role_name"));
                response["user_id"]=string(res->getString("user_id"));
                response["language"]=string(res->getString("language"));
                }                                                                                                               
            delete res;
            }
        else
            message="Wrong Username";
        delete stmt;
        delete con; 
        response["message"]=message;
        }
    catch(const std::exception& e)
        {
        writeException("loginUser",e.what());
        }
    return response;
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
        case 2:showNavigationList();
            break;
        case 3:response=createUserRole(data);
            break;
        case 4:showUserRoleNavigationList();
            break;
        case 5:response=updateUserRoleRights(data);
            break;
        case 6:userRoleDropdown();
            break;
        case 7:response=insertUpdateUsers(data);
            break;
        case 8:showUsersList();
            break;
        case 9:response=enableDisableUser(data);
            break;
        case 10:response=changePassword(data);
            break;
        case 11:response=getUserDetails(data);
            break; 
        case 12:response=loginUser(data);
            break; 
        }
    return response;
    }


extern "C" 
	{    
    PHPCPP_EXPORT void *get_module()
	{        
	static Php::Extension extension("PX_AdminLogin", "1.0");
        extension.add<parcxUserManagement>("parcxUserManagement");               
        return extension;
		}
	}
