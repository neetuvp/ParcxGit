//make -B
//sudo make install
//service apache2 restart

#include <phpcpp.h>
#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<time.h>
#include<ctime>
#include "mysql_connection.h"
#include "mysql_driver.h"
#include <mysql.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include<cppconn/prepared_statement.h>

using namespace std;
sql::Connection *conn;
sql::Driver *driver;
sql::Statement *stmt;
sql::ResultSet *res;
sql::PreparedStatement *prep;
sql::ResultSetMetaData *rm;

#define DBServer "DBServer"
#define DBUsername "parcxservice"
#define DBPassword "1fromParcx!19514"
#define DATABASE "parcx_server"


sql::Connection* mySQLConnect()
{
 
  driver = get_driver_instance();
  conn = driver->connect(DBServer,DBUsername,DBPassword);
  conn->setSchema(DATABASE);
  return conn;
}
string currentDateTime()
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

void writeException(string function,string txt)
{
    ofstream fp;
    fp.open("/opt/parcx/Logs/WebApplication/ExceptionLogs/PX-CustomerMessages.log", ios::app);
    fp<<currentDateTime()<<" "<<function<<":: "<<txt<<endl;
    fp.close();
}//end of .writeException


void writeLog(string function,string txt)
{
    ofstream fp;
    fp.open("/opt/parcx/Logs/WebApplication/ApplicationLogs/PX-CustomerMessages.log", ios::app);
    fp<<currentDateTime()<<" "<<function<<":: "<<txt<<endl;
    fp.close();
}//end of .writeException


void GetCustomerMessagesText(Php::Parameters &params)
{
    int device_type = params[0];
    try
    {
        int n=0;
        string id = "0";
        conn = mySQLConnect();
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select * from customer_messages_text where status =1 and device_type = "+to_string(device_type));
        if(res->rowsCount()>0)			
		{	
            Php::out<< "<input type='hidden' name = 'device_type' id = 'device_type' value='"+to_string(device_type)+"'>"<<std::endl;
            Php::out<<"<div class='card card-primary mb-4 col-md-12 p-0' id='active_messages' data-status='active_messages'>"<<std::endl;
			Php::out<<"<div class='card-header'>"<< std::endl;	
            Php::out<<"<div class='d-flex justify-content-between align-items-center'>"<<std::endl;
            Php::out<<"<div class='col'>No</div>"<< std::endl;	
            Php::out<<"<div class='col-2'>Message</div>"<< std::endl;
            Php::out<<"<div class='col-2'>English Line 1</div>"<< std::endl;
            Php::out<<"<div class='col-2'>English Line 2</div>"<< std::endl;
            Php::out<<"<div class='col-2'>English Line 3</div>"<< std::endl;
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
                Php::out<< "<div class='col-2' id = 'message"+id+"'>"+res->getString("message")+"</div>"<< std::endl;
                Php::out<< "<div class='col-2' id = 'english1"+id+"'>"+res->getString("english_line1")+"</div>"<< std::endl;
                Php::out<< "<div class='col-2' id = 'english2"+id+"'>"+res->getString("english_line2")+"</div>"<< std::endl;
                Php::out<< "<div class='col-2' id = 'english3"+id+"'>"+res->getString("english_line3")+"</div>"<< std::endl;
                Php::out<< "<div class='col'>"<< std::endl;
                Php::out<< "<input class='btn btn-danger btn-block btn-cancel-parking' id ='cancelbtn"+id+"' type='submit'  value='Cancel' onclick='CancelEdit("+id+")' style='visibility:hidden'/>"<< std::endl;
                Php::out<<"</div>"<< std::endl;
                Php::out<< "<div class='col' id = 'editdivfixed"+id+"'>"<< std::endl;
                Php::out<< "<input class='btn btn-info btn-block btn-edit-parking' id ='editmessage"+id+"' type='submit'  value='Edit' onclick = 'EditMessage("+id+")' id='"+id+"' >"<< std::endl;
                Php::out<< "</div>"<< std::endl;             
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
        writeException("GetCustomerMessagesText",e.what());
    }
    
   
}


void GetCustomerMessagesMedia(Php::Parameters &params)
{
    int device_type = params[0];
    try
    {
        int n=0;
        string id = "0";
        conn = mySQLConnect();
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select * from customer_messages_media where status =1 and device_type = "+to_string(device_type));
        if(res->rowsCount()>0)			
		{	
            Php::out<< "<input type='hidden' name = 'device_type' id = 'device_type' value='"+to_string(device_type)+"'>"<<std::endl;
            Php::out<<"<div class='card card-primary mb-4 col-md-12 p-0' id='active_messages' data-status='active_messages'>"<<std::endl;
			Php::out<<"<div class='card-header'>"<< std::endl;	
            Php::out<<"<div class='d-flex justify-content-between align-items-center'>"<<std::endl;
            Php::out<<"<div class='col'>No</div>"<< std::endl;	
            Php::out<<"<div class='col-2'>Message</div>"<< std::endl;
            Php::out<<"<div class='col-2'>Image Type</div>"<< std::endl;
            Php::out<<"<div class='col-4'>Image Path</div>"<< std::endl;
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
                Php::out<< "<div class='col-2' id = 'message"+id+"'>"+res->getString("message")+"</div>"<< std::endl;
                Php::out<< "<div class='col-2' id = 'type"+id+"'>"+res->getString("media_type")+"</div>"<< std::endl;
                Php::out<< "<div class='col-4' id = 'path"+id+"'>"+res->getString("media_path")+"</div>"<< std::endl;
                Php::out<< "<div class='col'>"<< std::endl;
                Php::out<< "<input class='btn btn-danger btn-block btn-cancel-parking' id ='cancelbtn"+id+"' type='submit'  value='Cancel' onclick='CancelEdit("+id+")' style='visibility:hidden'/>"<< std::endl;
                Php::out<<"</div>"<< std::endl;
                Php::out<< "<div class='col' id = 'editdivfixed"+id+"'>"<< std::endl;
                Php::out << "<input class='btn btn-info btn-block product-enable-disable-btn' type = 'submit' value= 'Enable'>"<< std::endl;
                Php::out<< "<input class='btn btn-info btn-block btn-edit-parking' id ='editmessage"+id+"' type='submit'  value='Edit' onclick = 'EditMessage("+id+")' id='"+id+"' >"<< std::endl;
                Php::out<< "</div>"<< std::endl;             
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
        writeException("GetCustomerMessagesMedia",e.what());
    }
    
   
}

Php::Value GetDeviceTypesCustomerMessages(Php::Parameters &params)
{
    int type = params[0];
    Php::Value result;
    try
    {
        conn = mySQLConnect();
        stmt = conn->createStatement();
        if(type==1)
            res = stmt->executeQuery("Select distinct(device_type) from customer_messages_text where status =1");
        else if(type==2)
            res = stmt->executeQuery("Select distinct(device_type) from customer_messages_media where status =1");
        int i=0;
        if(res->rowsCount()>0)			
        {	
            while(res->next())
            {
                if(res->getInt("device_type") == 1)
                {
                    result[i]="Entry";
                    i++;
                }
                if(res->getInt("device_type") == 2)
                {
                    result[i]="Exit";
                    i++;
                }
                if(res->getInt("device_type") == 3)
                {
                    result[i]="Cashier";
                    i++;
                }
                if(res->getInt("device_type") == 4)
                {
                    result[i]="APM";
                    i++;
                }
                if(res->getInt("device_type") == 5)
                {
                    result[i]="Handheld";
                    i++;
                }
            }
        }
        delete res;
        delete stmt;
        delete conn;
    }
    catch(const std::exception& e)
    {
        writeException("GetDeviceTypesCustomerMessages",e.what());
    }
    return result;

}


Php::Value UpdateRecord(Php::Parameters &params)
{
    Php::Value result=0;
    try
    {
        conn = mySQLConnect();
        stmt = conn->createStatement();
        Php::Value data = params[0];
        int id = data["id"];
        string language = data["language"];
        string line1 =  data["line1"];
        string line2 =  data["line2"];
        string line3 =  data["line3"];
        prep = conn->prepareStatement("Update customer_messages_text set "+language+"_line1=?,"+language+"_line2=?,"+language+"_line3=? where id = ?");

        prep->setString(1,line1);
        prep->setString(2,line2);
        prep->setString(3,line3);
        prep->setInt(4,id);

        if(prep->executeUpdate())
        {
            result["success"]=1;
        }
        
        delete prep;
        delete stmt;
        delete conn;
    }
    catch(const std::exception& e)
    {
        writeException("UpdateRecord",e.what());
    }
    return result;

}

Php::Value InsertRecord(Php::Parameters &params)
{
    Php::Value result=0;
    string sql=""; 
    try
    {
        conn = mySQLConnect();
        stmt = conn->createStatement();
        Php::Value data = params[0];
		int device_type =  data["device_type"];
		string message  =  data["message"];
        string media_path = data["media_path"];
        //string english1 =  data["english_line1"];
        //string english2 =  data["english_line2"];
        //string english3 =  data["english_line3"];
        prep = conn->prepareStatement("Insert into customer_messages_text (device_type,message)values(?,?)");

        prep->setInt(1,device_type);
        prep->setString(2,message);
        prep->executeUpdate();
        
        delete prep;
        sql = "Update customer_messages_text set ";
        res = stmt->executeQuery("Show columns from customer_messages_text where FIELD like '%_line%'");
        int n = res->rowsCount();
        int i = 0;
        while(res->next())
        {

            string field = res->getString("Field");

            string val = data[field];
            writeException("val:"+val,"col:"+field);
            sql   = sql + field +" = '"+val+"'";
            
            if(i<(n-1))
            {
                sql = sql+", ";
            }
            i++;
        }
        delete res;

        sql = sql+" where device_type = "+to_string(device_type)+" and message = '"+message+"'";
        writeException("","sqqql:"+sql);
        if(stmt->executeUpdate(sql))
        {
            result["text"]="success";
        }

        sql = "Insert into customer_messages_media(device_type,message,media_path)values("+to_string(device_type)+",'"+message+"','"+media_path+"')";
        if(stmt->executeUpdate(sql))
        {
            result["media"]="success";
        }

        delete stmt;
        delete conn;
    }
    catch(const std::exception& e)
    {
        writeException("InsertRecord",e.what());
    }
    return result;

}




Php::Value GetMessageText(Php::Parameters &params)
{
    int device_type = params[0];
    MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	conn = mysql_init (NULL);	  
    if(!conn)
    {
        Php::out<<"Mysql Init Failed"<<endl;
        writeException("GetMessageText","Mysql Init Failed");
    }  
	
	conn = mysql_real_connect(conn,DBServer,DBUsername,DBPassword,DATABASE,0,NULL,0); 
    if(!conn)
    {
        writeException("GetMessageText","Connection Failed");
    }
    string sql = "SELECT id,message,english_line1,english_line2,english_line3 from customer_messages_text where device_type = "+to_string(device_type);
    
    mysql_query(conn,(sql.c_str()));
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

Php::Value GetMessageLanguages(Php::Parameters &params)
{
    Php::Value result;
    string temp;
    int i =0;
    try
    {
        conn = mySQLConnect();
        stmt = conn->createStatement();
        res = stmt->executeQuery("Show columns from`customer_messages_text` where FIELD like '%_line1'");
        while(res->next())
        {
            temp = res->getString("Field");
            temp = temp.substr(0, temp.size()-6);
            result[i] = temp;
            i++;
        }
        delete stmt;
        delete conn;
    }
    catch(const std::exception& e)
    {
        writeException("UpdateRecord",e.what());
    }
    return result;
}


void GetMessageTextByLanguage(Php::Parameters &params)
{
    Php::Value data = params[0];
    int device_type = data["type"];
    string language = data["language"];
    try
    {
        int n=0;
        string id = "0";
        conn = mySQLConnect();
        stmt = conn->createStatement();
        writeException("","Select id,message,"+language+"_line1,"+language+"_line2,"+language+"_line3 from customer_messages_text where status =1 and device_type = "+to_string(device_type));
        res = stmt->executeQuery("Select a.id,a.message,"+language+"_line1,"+language+"_line2,"+language+"_line3,media_path from customer_messages_text as a inner join customer_messages_media as b on a.message=b.message where a.status =1 and a.device_type = "+to_string(device_type));
        if(res->rowsCount()>0)			
		{	
            Php::out<< "<input type='hidden' name = 'device_type' id = 'device_type' value='"+to_string(device_type)+"'>"<<std::endl;
            Php::out<< "<input type='hidden' name = 'language' id = 'language' value='"+language+"'>"<<std::endl;
            Php::out<<"<div class='card card-primary mb-4 col-md-12 p-0' id='active_messages' data-status='active_messages'>"<<std::endl;
			Php::out<<"<div class='card-header'>"<< std::endl;	
            Php::out<<"<div class='d-flex justify-content-between align-items-center'>"<<std::endl;
            Php::out<<"<div class='col'>No</div>"<< std::endl;	
            Php::out<<"<div class='col-2'>Message</div>"<< std::endl;
            Php::out<<"<div class='col-2'>"+language+" Line 1</div>"<< std::endl;
            Php::out<<"<div class='col-2'>"+language+" Line 2</div>"<< std::endl;
            Php::out<<"<div class='col-2'>"+language+" Line 3</div>"<< std::endl;
            Php::out<<"<div class='col'>Image</div>"<< std::endl;
            //Php::out<<"<div class='col'>Upload</div>"<< std::endl;
            //Php::out<<"<div class='col'></div>"<< std::endl;
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
                Php::out<< "<div class='col-2' id = 'message"+id+"'>"+res->getString("message")+"</div>"<< std::endl;
                Php::out<< "<div class='col-2' id = '"+language+"1"+id+"'>"+res->getString(language+"_line1")+"</div>"<< std::endl;
                Php::out<< "<div class='col-2' id = '"+language+"2"+id+"'>"+res->getString(language+"_line2")+"</div>"<< std::endl;
                Php::out<< "<div class='col-2' id = '"+language+"3"+id+"'>"+res->getString(language+"_line3")+"</div>"<< std::endl;
                //Php::out<< "<div class='col' id = 'media_path"+id+"'><a target='_blank' href='"+res->getString("media_path")+"'><img src='"+res->getString("media_path")+"' alt='Img'></a></div>"<< std::endl;
                Php::out<< "<div class='col' id = 'media_path"+id+"'><img class='openModal openImageDialog"+res->getString("message")+"' src='"+res->getString("media_path")+"' alt='Img' data-toggle='modal' data-target='#uploadModal' data-backdrop='false' data-id="+res->getString("message")+"></div>"<< std::endl;

                //Php::out << "<div class='col'><input data-toggle='modal' data-target='#uploadModal' data-backdrop='false' class='btn btn-info btn-edit-image' id ='editimage"+id+"'   type='button'  value='Upload'  data-id='"+res->getString("message")+"'  style='visibility:hidden' ></div>"<< std::endl;
                Php::out<< "<div class='col'>"<< std::endl;
                //Php::out<< "<input type='file' id='myfile' name='myfile'>"<< std::endl;
                Php::out<< "<input class='btn btn-danger btn-block btn-cancel-parking' id ='cancelbtn"+id+"'  type='submit'  value='Cancel' onclick='CancelEdit("+id+")' style='visibility:hidden'/>"<< std::endl;
                Php::out<<"</div>"<< std::endl;
                Php::out<< "<div class='col' id = 'editdivfixed"+id+"'>"<< std::endl;
                Php::out<< "<input class='btn btn-info btn-block btn-edit-parking' id ='editmessage"+id+"' type='submit'  value='Edit' onclick = 'EditMessage("+id+")' id='"+id+"' >"<< std::endl;
                Php::out<< "</div>"<< std::endl;             
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
        writeException("GetMessageTextByLanguage",e.what());
    }
    
   
}


Php::Value UpdateImagePath(Php::Parameters &params)
{
    Php::Value result=0;
    try
    {
        conn = mySQLConnect();
        stmt = conn->createStatement();
        Php::Value data = params[0];
        string message = data["message"];
        string media_path = data["media_path"];
        prep = conn->prepareStatement("Update customer_messages_media set media_path = ? where message=?");

        prep->setString(1,media_path);
        prep->setString(2,message);

        if(prep->executeUpdate())
        {
            result["success"]=1;
        }
        
        delete prep;
        delete stmt;
        delete conn;
    }
    catch(const std::exception& e)
    {
        writeException("UpdateImagePath",e.what());
    }
    return result;
}

Php::Value GetCustomerMessageSettings(Php::Parameters &params)
{
    Php::Value result;

    try
    {
        conn = mySQLConnect();
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select setting_name,setting_value from system_settings where setting_name in('customer_message_limit','customer_media_upload_limit','customer_media_path')");
        while(res->next())
        {
            if(res->getString("setting_name")=="customer_message_limit")
            {
                result["message_limit"] = res->getString("setting_value").asStdString();
            }
            else if(res->getString("setting_name")=="customer_media_upload_limit")
            {
                result["upload_limit"] = res->getString("setting_value").asStdString();
            }
            else if(res->getString("setting_name")=="customer_media_path")
            {
                result["media_path"] = res->getString("setting_value").asStdString();
            }
        }
        delete stmt;
        delete conn;
    }
    catch(const std::exception& e)
    {
        writeException("GetCustomerMessageSettings",e.what());
    }
    return result;
}


extern "C" {
    
    /**
     *  Function that is called by PHP right after the PHP process
     *  has started, and that returns an address of an internal PHP
     *  strucure with all the details and features of your extension
     *
     *  @return void*   a pointer to an address that is understood by PHP
     */
    PHPCPP_EXPORT void *get_module() 
    {
        // static(!) Php::Extension object that should stay in memory
        // for the entire duration of the process (that's why it's static)
        static Php::Extension extension("PX_CustomerMessages", "1.0");
        extension.add<GetCustomerMessagesText>("GetCustomerMessagesText");
        extension.add<GetDeviceTypesCustomerMessages>("GetDeviceTypesCustomerMessages");
        extension.add<GetMessageText>("GetMessageText");
        extension.add<UpdateRecord>("UpdateRecord");
		extension.add<InsertRecord>("InsertRecord");
        extension.add<GetCustomerMessagesMedia>("GetCustomerMessagesMedia");
        extension.add<GetMessageLanguages>("GetMessageLanguages");
        extension.add<GetMessageTextByLanguage>("GetMessageTextByLanguage");
        extension.add<UpdateImagePath>("UpdateImagePath");
        extension.add<GetCustomerMessageSettings>("GetCustomerMessageSettings");
               // @todo    add your own functions, classes, namespaces to the extension
        
        // return the extension
        return extension;
    }
}
