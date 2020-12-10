

/* Standard C++ includes */
//for compile
#include<phpcpp.h>
#include<iostream>
#include<mysql.h>
#include<sstream>
#include<string>
#include <stdlib.h>
#include<fstream>
/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#define SERVER "DBServer"
#define USER "parcxservice"
#define PASSWORD "1fromParcx!19514"
#define DATABASE "parcx_server"

using namespace std;

// CONNECTION

sql::Connection* mysql_Connect()
	{
    sql::Connection *con=NULL;
	sql::Driver *driver;		
	driver = get_driver_instance();		
	con = driver->connect(SERVER, USER,PASSWORD);
	con->setSchema(DATABASE);
	return con;
	}

// User Login BY User(Only For Active User)
  Php::Value user_login(Php::Parameters &params)
    {
    MYSQL *connect;
    connect=mysql_init(NULL);

    connect=mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0); 

    MYSQL_RES *res_set;
    MYSQL_ROW row;

   std::stringstream sql;	
   std::string username=params[0];
   std::string password=params[1];

   sql << "SELECT operator_name,system_users.user_role_id,user_role_name,account_status,user_id  FROM system_users INNER JOIN system_user_role ON system_users.user_role_id=system_user_role.user_role_id where username= +'" << username << "'and password= +'" << password << "' and validity_from_date<=CURRENT_DATE and validity_to_date>=CURRENT_DATE LIMIT 1";

   mysql_query(connect, sql.str().c_str());

   res_set = mysql_store_result(connect);
   Php::Array array;
   int num_fields = mysql_num_fields(res_set);

   while ((row = mysql_fetch_row(res_set)) != NULL)
   {
	   for (int i = 0; i < num_fields; i++)
	   {

		   array[i] = row[i];
	   }
		    }
    mysql_close(connect);   
    return array;
    }


// END USER LOGIN



// ADD USER BY ADMIN

Php::Value add_user(Php::Parameters &params)
	{
	sql::Connection *con;
	con=mysql_Connect();
	sql::PreparedStatement *prep_stmt;
        
    std::string fullName=params[0];
	std::string userName=params[1];
	std::string userEmail=params[2];
	std::string userType=params[3];
	std::string password=params[4];
	std::string companyname=params[5];
	std::string phone=params[6];
    std::string userFromDate=params[7];
	std::string userToDate=params[8];
        
	prep_stmt=con->prepareStatement("insert into system_users(operator_name,username,email,user_role_id,password,company_name,phone,validity_from_date,validity_to_date)values(?,?,?,?,?,?,?,?,?)");
	prep_stmt->setString(1,fullName);
        prep_stmt->setString(2,userName); 	
	prep_stmt->setString(3,userEmail);
	prep_stmt->setString(4,userType);
	prep_stmt->setString(5,password);
	prep_stmt->setString(6,companyname);
	prep_stmt->setString(7,phone);
        prep_stmt->setString(8,userFromDate);
	prep_stmt->setString(9,userToDate);
        
	if(prep_stmt->execute())
		return "failed";
	else
		return "success";
	}




  Php::Value update_user(Php::Parameters &params)

	    {
	    MYSQL *connect;
		connect = mysql_init(NULL);
		mysql_real_connect(connect, SERVER, USER, PASSWORD, DATABASE, 0, NULL, 0);

		MYSQL_RES *res_set;

	    std::stringstream sql;
            
            int id=params[0];
            std::string fullName=params[1];            
            std::string email=params[2];
            std::string userType=params[3];            
            std::string companyname=params[4];
            std::string phone=params[5];
            std::string from=params[6];
            std::string to=params[7];

	    sql<<"UPDATE system_users SET operator_name='"<<fullName<<"',company_name='"<<companyname<<"',email='"<<email<<"',phone='"<<phone<<"',user_role_id="<<userType<<",validity_from_date='"<<from<<"',validity_to_date='"<<to<<"' WHERE user_id= '"<<id<<"'";
	   
	    mysql_query(connect,sql.str().c_str());
	   
	    res_set=mysql_store_result(connect);

	    mysql_close(connect);  
	 
	    if(res_set)
			return "failed";
		else
			return "success";
	    }

//GET USER View

Php::Value get_user_view()
	{ 
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	conn = mysql_init (NULL);	    
	
	mysql_real_connect(conn,SERVER,USER,PASSWORD,DATABASE,0,NULL,0); 

	mysql_query(conn,("SELECT user_id as id,operator_name as user_name,user_role_name,email as user_email,company_name as user_company_name, phone as user_phone,validity_from_date as user_validity_from_date,validity_to_date as user_validity_to_date FROM system_users INNER JOIN system_user_role ON system_users.user_role_id=system_user_role.user_role_id"));
	res = mysql_store_result(conn);

	int num_fields = mysql_num_fields(res);

	Php::Array array;
	Php::Value rows;
	int i = 0;

	while((field = mysql_fetch_field(res))){
	    array[i] = field->name;
	    i++;
	}

	int x = 0;
	while ((row = mysql_fetch_row(res)))
	{

	    for (int z=0;z<num_fields;z++) 
	    {
		std::string fieldname = array[z];
                array[fieldname] = row[z];
                rows[x]=array;

	    }

	    x++;
	}
	if(res != NULL)
	mysql_free_result(res);
	mysql_close (conn);

	return rows;

	}

// END USER View
                                               
 //view enable user 
        
Php::Value  get_active_user()
	{ 
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	conn = mysql_init (NULL);
    
	mysql_real_connect(conn,SERVER,USER,PASSWORD,DATABASE,0,NULL,0); 

	mysql_query(conn,("SELECT user_id as id,username as user_name,operator_name as operator_name,user_role_name as user_role,email as user_email,company_name as user_company, phone as user_phone,validity_from_date as user_from,validity_to_date as user_to FROM system_users INNER JOIN system_user_role ON system_users.user_role_id=system_user_role.user_role_id where account_status is TRUE"));
	res = mysql_store_result(conn);

	int num_fields = mysql_num_fields(res);

	Php::Array array;
	Php::Value rows;
	int i = 0;

	while((field = mysql_fetch_field(res))){
	    array[i] = field->name;
	    i++;
	}

	int x = 0;
	while ((row = mysql_fetch_row(res)))
	{

	    for (int z=0;z<num_fields;z++) 
	    {
		std::string fieldname = array[z];
                array[fieldname] = row[z];
                rows[x]=array;

	    }

	    x++;
	}
	if(res != NULL)
	mysql_free_result(res);
	mysql_close (conn);

	return rows;

	}
      
        //view enable user 
        
    Php::Value  get_de_active_user()
	{ 
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	conn = mysql_init (NULL);

	mysql_real_connect(conn,SERVER,USER,PASSWORD,DATABASE,0,NULL,0); 

	    
	mysql_query(conn,("SELECT user_id as id,operator_name as user_name,user_role_name as user_role,email as user_email,company_name as user_company, phone as user_phone,validity_from_date as user_from,validity_to_date as user_to FROM system_users INNER JOIN system_user_role ON system_users.user_role_id=system_user_role.user_role_id where account_status is false"));
	res = mysql_store_result(conn);

	int num_fields = mysql_num_fields(res);

	Php::Array array;
	Php::Value rows;
	int i = 0;

	while((field = mysql_fetch_field(res))){
	    array[i] = field->name;
	    i++;
	}

	int x = 0;
	while ((row = mysql_fetch_row(res)))
	{

	    for (int z=0;z<num_fields;z++) 
	    {
		std::string fieldname = array[z];
                array[fieldname] = row[z];
                rows[x]=array;

	    }

	    x++;
	}
	if(res != NULL)
	mysql_free_result(res);
	mysql_close (conn);

	return rows;

	}
                
                
// USER DISABLE BUTTON BY ADMIN FOR Disable User account Status


Php::Value user_disable_status(Php::Parameters &params)

	    {
	    MYSQL *connect;
	    connect=mysql_init(NULL);
	    
	    connect=mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0); 
 
	    MYSQL_RES *res_set;

	    std::stringstream sql;	
	    int id=params[0];

	    sql<<"UPDATE system_users SET account_status=False WHERE user_id= '"<<id<<"'";
	   
	    mysql_query(connect,sql.str().c_str());
	   
	    res_set=mysql_store_result(connect);

	    mysql_close(connect);  
	 
	   if(res_set)
			return "failed";
		else
			return "success";
		}

//END USER DISABLE BUTTON  
     
// USER Enable BUTTON BY ADMIN FOR enable User account Status


Php::Value user_enable_status(Php::Parameters &params)

	    {
	    MYSQL *connect;
	    connect=mysql_init(NULL);
      
	    connect=mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0); 
 

	    MYSQL_RES *res_set;

	    std::stringstream sql;	
	    int id=params[0];

	    sql<<"UPDATE system_users SET account_status=True WHERE user_id= '"<<id<<"'";
	   
	    mysql_query(connect,sql.str().c_str());
	   
	    res_set=mysql_store_result(connect);

	    mysql_close(connect);  
	 
	    if(res_set)
			return "failed";
		else
			return "success";
	    }


   
        
Php::Value get_menu_list()
	{ 
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	conn = mysql_init (NULL);	    
	
	mysql_real_connect(conn,SERVER,USER,PASSWORD,DATABASE,0,NULL,0); 

	mysql_query(conn,("SELECT * FROM system_navigation where menu_status is true order by menu_group_name"));
	res = mysql_store_result(conn);

	int num_fields = mysql_num_fields(res);

	Php::Array array;
	Php::Value rows;
	int i = 0;

	while((field = mysql_fetch_field(res))){
	    array[i] = field->name;
	    i++;
	}

	int x = 0;
	while ((row = mysql_fetch_row(res)))
	{

	    for (int z=0;z<num_fields;z++) 
	    {
		std::string fieldname = array[z];
                array[fieldname] = row[z];
                rows[x]=array;

	    }

	    x++;
	}
	if(res != NULL)
	mysql_free_result(res);
	mysql_close (conn);

	return rows;

	}
        
        
        
// Navigation Layout Accessing
          
Php::Value get_menu_access_by_user_role(Php::Parameters &params)
	{ 
            
	MYSQL *connect;
        connect=mysql_init(NULL);

	    connect=mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0); 
 
        MYSQL_RES *res;
        MYSQL_FIELD *field;
        MYSQL_ROW row;
        std::stringstream sql;	
        std::string userRoleId=params[0];

        sql<<"SELECT role_rights_id,menu_group_name,menu_name,system_navigation.menu_id,system_user_role.user_role_id,menu_group_order_index,user_role_name,menu_link,group_menu_icon,rr_view,rr_edit,rr_delete from system_role_rights join system_navigation on system_role_rights.menu_id=system_navigation.menu_id join system_user_role on system_role_rights.user_role_id=system_user_role.user_role_id  WHERE system_role_rights.user_role_id='"<<userRoleId<<"' and rr_view=1 order by system_navigation.menu_group_order_index,system_navigation.menu_order_index,system_user_role.user_role_id";

        mysql_query(connect,sql.str().c_str());

        res=mysql_store_result(connect);


	int num_fields = mysql_num_fields(res);

	Php::Array array;
	Php::Value rows;
	int i = 0;

	while((field = mysql_fetch_field(res))){
	    array[i] = field->name;
	    i++;
	}

	int x = 0;
	while ((row = mysql_fetch_row(res)))
	{

	    for (int z=0;z<num_fields;z++) 
	    {
		std::string fieldname = array[z];
                array[fieldname] = row[z];
                rows[x]=array;

	    }

	    x++;
	}
	if(res != NULL)
	mysql_free_result(res);
	mysql_close (connect);

	return rows;
	}
        
        // End Navigation Layout Accessing
        

        
Php::Value get_user_roles()
	{ 
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	conn = mysql_init (NULL);
   	 
	mysql_real_connect(conn,SERVER,USER,PASSWORD,DATABASE,0,NULL,0); 

	mysql_query(conn,("SELECT * FROM system_user_role order by user_role_id"));
	res = mysql_store_result(conn);

	int num_fields = mysql_num_fields(res);

	Php::Array array;
	Php::Value rows;
	int i = 0;

	while((field = mysql_fetch_field(res))){
	    array[i] = field->name;
	    i++;
	}

	int x = 0;
	while ((row = mysql_fetch_row(res)))
	{

	    for (int z=0;z<num_fields;z++) 
	    {
		std::string fieldname = array[z];
                array[fieldname] = row[z];
                rows[x]=array;

	    }

	    x++;
	}
	if(res != NULL)
	mysql_free_result(res);
	mysql_close (conn);

	return rows;

	}

	void get_user_roles_menu()
	{
		MYSQL *conn;
		MYSQL_RES *res;
		MYSQL_ROW row;		
		conn = mysql_init(NULL);

		mysql_real_connect(conn, SERVER, USER, PASSWORD, DATABASE, 0, NULL, 0);

		mysql_query(conn, ("SELECT user_role_id,user_role_name FROM system_user_role order by user_role_id"));
		res = mysql_store_result(conn);
		
		while ((row = mysql_fetch_row(res)))
		{
			Php::out << "<option value='"<<row[0]<<"'>"<<row[1]<<"</option>" << std::endl;
		}
				
		if (res != NULL)
			mysql_free_result(res);
		mysql_close(conn);
		
	}

	Php::Value get_menu_under_group(Php::Parameters &params)
	{ 
            
	MYSQL *connect;
        connect=mysql_init(NULL);
	   
	    connect=mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0); 
 
        MYSQL_RES *res;
        MYSQL_FIELD *field;
        MYSQL_ROW row;
        std::stringstream sql;	
        std::string menuGroupname=params[0];

        sql<<"SELECT menu_id,menu_group_name,menu_name,menu_link,menu_group_order_index,menu_order_index from system_navigation where menu_group_name='"<<menuGroupname<<"' and menu_status is true";
        mysql_query(connect,sql.str().c_str());

        res=mysql_store_result(connect);


	int num_fields = mysql_num_fields(res);

	Php::Array array;
	Php::Value rows;
	int i = 0;

	while((field = mysql_fetch_field(res))){
	    array[i] = field->name;
	    i++;
	}

	int x = 0;
	while ((row = mysql_fetch_row(res)))
	{

	    for (int z=0;z<num_fields;z++) 
	    {
		std::string fieldname = array[z];
                array[fieldname] = row[z];
                rows[x]=array;

	    }

	    x++;
	}
	if(res != NULL)
	mysql_free_result(res);
	mysql_close (connect);

	return rows;
	}
        
        
       
          
Php::Value get_menu_access_by_role_and_menu_id(Php::Parameters &params)
	{ 
            
	MYSQL *connect;
        connect=mysql_init(NULL);

	    
	    connect=mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0); 
 
        MYSQL_RES *res;
        MYSQL_FIELD *field;
        MYSQL_ROW row;
        std::stringstream sql;	
        std::string userRoleId=params[0];
        std::string menuid=params[1];
        sql<<"SELECT role_rights_id,menu_group_name,menu_name,system_navigation.menu_id,system_user_role.user_role_id,menu_group_order_index,user_role_name,menu_link,group_menu_icon,rr_view,rr_edit,rr_delete from system_role_rights join system_navigation on system_role_rights.menu_id=system_navigation.menu_id join system_user_role on system_role_rights.user_role_id=system_user_role.user_role_id  WHERE system_role_rights.user_role_id='"<<userRoleId<<"' and system_role_rights.menu_id='"<<menuid<<"' order by system_navigation.menu_group_order_index,system_navigation.menu_order_index,system_user_role.user_role_id";

        mysql_query(connect,sql.str().c_str());

        res=mysql_store_result(connect);


	int num_fields = mysql_num_fields(res);

	Php::Array array;
	Php::Value rows;
	int i = 0;

	while((field = mysql_fetch_field(res))){
	    array[i] = field->name;
	    i++;
	}

	int x = 0;
	while ((row = mysql_fetch_row(res)))
	{

	    for (int z=0;z<num_fields;z++) 
	    {
		std::string fieldname = array[z];
                array[fieldname] = row[z];
                rows[x]=array;

	    }

	    x++;
	}
	if(res != NULL)
	mysql_free_result(res);
	mysql_close (connect);

	return rows;
	}
        
        // End Navigation Layout Acce
        
        
        
             
Php::Value get_group_menu_name()
	{ 
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	conn = mysql_init (NULL);

	mysql_real_connect(conn,SERVER,USER,PASSWORD,DATABASE,0,NULL,0); 

	mysql_query(conn,("SELECT menu_group_order_index,menu_group_name FROM system_navigation GROUP BY system_navigation.menu_group_name,menu_group_order_index ORDER BY menu_group_order_index"));
	res = mysql_store_result(conn);

	int num_fields = mysql_num_fields(res);

	Php::Array array;
	Php::Value rows;
	int i = 0;

	while((field = mysql_fetch_field(res))){
	    array[i] = field->name;
	    i++;
	}

	int x = 0;
	while ((row = mysql_fetch_row(res)))
	{

	    for (int z=0;z<num_fields;z++) 
	    {
		std::string fieldname = array[z];
                array[fieldname] = row[z];
                rows[x]=array;

	    }

	    x++;
	}
	if(res != NULL)
	mysql_free_result(res);
	mysql_close (conn);

	return rows;

	}
    
         
// Navigation Layout Accessing                   
Php::Value get_menu_access()
	{ 
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	conn = mysql_init (NULL);
      	
	mysql_real_connect(conn,SERVER,USER,PASSWORD,DATABASE,0,NULL,0); 

	mysql_query(conn,("SELECT role_rights_id,menu_group_name,menu_name,system_navigation.menu_id,system_user_role.user_role_id,user_role_name,menu_link,rr_view,rr_edit,rr_delete from system_role_rights join system_navigation on system_role_rights.menu_id=system_navigation.menu_id join system_user_role on system_role_rights.user_role_id=system_user_role.user_role_id order by system_navigation.menu_group_order_index,system_navigation.menu_order_index,system_user_role.user_role_id"));
	res = mysql_store_result(conn);

	int num_fields = mysql_num_fields(res);

	Php::Array array;
	Php::Value rows;
	int i = 0;

	while((field = mysql_fetch_field(res))){
	    array[i] = field->name;
	    i++;
	}

	int x = 0;
	while ((row = mysql_fetch_row(res)))
	{

	    for (int z=0;z<num_fields;z++) 
	    {
		std::string fieldname = array[z];
                array[fieldname] = row[z];
                rows[x]=array;

	    }

	    x++;
	}
	if(res != NULL)
	mysql_free_result(res);
	mysql_close (conn);

	return rows;

	}
        
        
        
// Navigation Layout Accessing
       
Php::Value get_menus_by_menu_name(Php::Parameters &params)
	{ 
            
	MYSQL *connect;
        connect=mysql_init(NULL);
	
	    connect=mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0); 
 
        MYSQL_RES *res;
        MYSQL_FIELD *field;
        MYSQL_ROW row;
        std::stringstream sql;	
        std::string menuName=params[0];

        sql<<"select * from system_navigation where menu_name='"<<menuName<<"'";

        mysql_query(connect,sql.str().c_str());

        res=mysql_store_result(connect);


	int num_fields = mysql_num_fields(res);

	Php::Array array;
	Php::Value rows;
	int i = 0;

	while((field = mysql_fetch_field(res))){
	    array[i] = field->name;
	    i++;
	}

	int x = 0;
	while ((row = mysql_fetch_row(res)))
	{

	    for (int z=0;z<num_fields;z++) 
	    {
		std::string fieldname = array[z];
                array[fieldname] = row[z];
                rows[x]=array;

	    }

	    x++;
	}
	if(res != NULL)
	mysql_free_result(res);
	mysql_close (connect);

	return rows;
	}
        
        
        
      
        
Php::Value add_user_access_right(Php::Parameters &params)
    {
    MYSQL *connect;
    connect=mysql_init(NULL);
	   
	connect=mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0); 

    MYSQL_RES *res_set;

    std::stringstream sql;	
    std::string qry=params[0];

    sql<<""<<qry<<"";
   
    mysql_query(connect,sql.str().c_str());
   
    res_set=mysql_store_result(connect);

    mysql_close(connect);  
 
   if(res_set)
		return "failed";
	else
		return "success";
    }
        
   
     
 
         
Php::Value add_with_update_access_right(Php::Parameters &params)
    {
    MYSQL *connect;
    connect=mysql_init(NULL);
	    
	connect=mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0); 

    MYSQL_RES *res_set;

    std::stringstream sql;	
    std::string qry=params[0];

    sql<<""<<qry<<"";
   
    mysql_query(connect,sql.str().c_str());
   
    res_set=mysql_store_result(connect);

    mysql_close(connect);  
 
   if(res_set)
		return "failed";
	else
		return "success";
    }

Php::Value change_password(Php::Parameters &params)
	{
		MYSQL *connect;
		connect = mysql_init(NULL);

		connect = mysql_real_connect(connect, SERVER, USER, PASSWORD, DATABASE, 0, NULL, 0);		
		MYSQL_RES *res;				
		std::stringstream sql;
		std::string id = params[0];
		std::string curruntPass=params[1];
		std::string newPass=params[2];

		sql << "select * from system_users where user_id=" << id << " and password=md5('" << curruntPass << "')";
		mysql_query(connect, sql.str().c_str());
		res = mysql_store_result(connect);
		if(res)
		{
			std::stringstream sql;
			sql << "update system_users set password=md5('"<<newPass<<"') where user_id=" << id;
			int n=mysql_query(connect, sql.str().c_str());
			if(n)
				{
					return "Password updation failed";
				}
			else
				{
					return "Password updated succesfully";
				}		
			
		}
		else
		{
			return "Current Password is wrong";
		}	
	mysql_close(connect);		
	}

Php::Value update_user_access_right(Php::Parameters &params)
    {
    MYSQL *connect;
    connect=mysql_init(NULL);
	    
	connect=mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0); 

    MYSQL_RES *res_set;

    std::stringstream sql;	
    std::string qry=params[0];

    sql<<""<<qry<<"";
   
    mysql_query(connect,sql.str().c_str());
   
    res_set=mysql_store_result(connect);

    mysql_close(connect);  
 
   if(res_set)
		return "failed";
	else
		return "success";
    }
        
Php::Value add_user_role(Php::Parameters &params)
 
	{
	sql::Connection *con;
	con=mysql_Connect();
	sql::PreparedStatement *prep_stmt;
	std::string rolename=params[0];

        
	prep_stmt=con->prepareStatement("insert into system_user_role(user_role_name)values(?)");
	prep_stmt->setString(1,rolename);	

	if(prep_stmt->execute())
		return "failed";
	else
		return "success";
	}
 
 
Php::Value last_inserted_user_roll_id()
    {
    MYSQL *connect;
    connect=mysql_init(NULL);

	    
	connect=mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0); 

    MYSQL_RES *res_set;
    MYSQL_ROW row;

   std::stringstream sql;	
   
   sql<<"SELECT MAX(user_role_id) AS user_roll_id FROM system_user_role";
   
    mysql_query(connect,sql.str().c_str());
   
    res_set=mysql_store_result(connect);
    Php::Array array;
    int num_fields= mysql_num_fields(res_set);

	    while((row=mysql_fetch_row(res_set))!=NULL)
		    {           
		          for (int i=0; i<num_fields; i++)  {
		           
		              array[i] =row[i];
		        }
		    }
    mysql_close(connect);   
    return array;
    }
   
   
   
Php::Value edit_data_by_user(Php::Parameters &params)
    {
    MYSQL *connect;
    connect=mysql_init(NULL);

    connect=mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0); 

    MYSQL_RES *res_set;
    MYSQL_ROW row;

   std::stringstream sql;	
   int id=params[0];
   
   sql<<"SELECT user_id,operator_name,username,company_name,email,phone,password,user_role_id,validity_from_date,validity_to_date  FROM system_users  where user_id= +'"<<id<<"' LIMIT 1";
   
    mysql_query(connect,sql.str().c_str());
   
    res_set=mysql_store_result(connect);
    Php::Array array;
    int num_fields= mysql_num_fields(res_set);

	    while((row=mysql_fetch_row(res_set))!=NULL)
		    {           
		          for (int i=0; i<num_fields; i++)  {
		           
		              array[i] =row[i];
		        }
		    }
    mysql_close(connect);   
    return array;
    }



extern "C" {    
    PHPCPP_EXPORT void *get_module() 
    {        
        static Php::Extension extension("PX_AdminLogin", "1.0");        
	extension.add<user_login>("user_login");
        extension.add<add_user>("add_user");
        extension.add<update_user>("update_user");
        extension.add<edit_data_by_user>("edit_data_by_user");
        extension.add<get_de_active_user>("get_de_active_user");
        extension.add<get_user_view>("get_user_view");
        extension.add<user_disable_status>("user_disable_status");
        extension.add<user_enable_status>("user_enable_status");
        extension.add<get_menu_list>("get_menu_list");
        extension.add<get_menu_access_by_user_role>("get_menu_access_by_user_role");
        extension.add<get_user_roles>("get_user_roles");
		extension.add<get_user_roles_menu>("get_user_roles_menu");
		extension.add<get_group_menu_name>("get_group_menu_name");
        extension.add<get_menu_access>("get_menu_access");
        extension.add<add_user_access_right>("add_user_access_right");
        extension.add<add_with_update_access_right>("add_with_update_access_right");        
        extension.add<get_menu_under_group>("get_menu_under_group");
        extension.add<get_menu_access_by_role_and_menu_id>("get_menu_access_by_role_and_menu_id");                
        extension.add<get_menus_by_menu_name>("get_menus_by_menu_name");
        extension.add<add_user_role>("add_user_role");  
        extension.add<last_inserted_user_roll_id>("last_inserted_user_roll_id"); 
        extension.add<update_user_access_right>("update_user_access_right");
        extension.add<get_active_user>("get_active_user");
		extension.add<change_password>("change_password");

		return extension;
    }
}











