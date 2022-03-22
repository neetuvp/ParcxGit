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
#define MediaPath "/parcx/Media/Videos"
using namespace std;
GeneralOperations General;
Validation validation;
string validation_failed = "Validation Failed\n";
void writeLog(string function, string message) {
    General.writeLog("WebApplication/ApplicationLogs/PX-ContentManagement-" + General.currentDateTime(dateFormat), function, message);
}

void writeException(string function, string message) {
    General.writeLog("WebApplication/ExceptionLogs/PX-ContentManagement-" + General.currentDateTime(dateFormat), function, message);
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

void logUserActivity(string category, string description, int user_id, string user_name, string status, string ip, string request, string response) {
    try {
        sql::Connection *con;
        sql::PreparedStatement *pstmt;
        con = General.mysqlConnect(ReportingDB);
        pstmt = con->prepareStatement("insert into system_journal(category,description,user_id,user_name,status,ip_address,request,response)values(?,?,?,?,?,?,?,?)");
        pstmt->setString(1, category);
        pstmt->setString(2, description);
        pstmt->setInt(3, user_id);
        pstmt->setString(4, user_name);
        pstmt->setString(5, status);
        pstmt->setString(6, ip);
        pstmt->setString(7, request);
        pstmt->setString(8, response);
        pstmt->executeUpdate();
        delete pstmt;
        delete con;
    }    catch (const std::exception& e) {
        writeException("logUserActivity", e.what());
    }
}


void showPlayListSettings(Php::Value data)
    {
    try
        {
        Php::Value role;
        sql::Connection *con;
        sql::Statement *stmt;            
        sql::ResultSet *res;

        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select * from playlist_settings");
        if(res->rowsCount()>0)
            {
            //Php::out<<"<table class='table table-blue'>"<<endl;
            Php::out << "<thead class='thead-light'>" << std::endl;
            Php::out<<"<tr>"<<endl;
            Php::out<<"<th>Playlist</th>"<<endl;
            Php::out<<"<th>Description</th>"<<endl;        
            
           // if(edit_access==1 ||delete_access==1)
                Php::out<<"<th></th>"<<endl;		           
           
            Php::out<<"</tr>"<<endl;	
            Php::out << "</thead>" << std::endl;		
            }
        while(res->next())
            {
            Php::out<<"<tr data-id='"<<res->getString("id")<<"'>"<<endl;                       
            Php::out<<"<td>"+res->getString("playlist_name")+"</td>"<<endl;
            Php::out<<"<td>"+res->getString("playlist_description")+"</td>"<<endl;            
            Php::out << "<td>"<< std::endl;
            Php::out << "<button type='button' class='btn btn-info playlist-view' title='View Playlist' data-text='View Playlist'><i class='fas fa-eye'></i></button>"<< std::endl;  
            //if(edit_access==1 ||delete_access==1){
               
               // if(delete_access==1)
               // {
                    if(res->getInt("playlist_status")==1)
                        Php::out << "<button type='button' class='btn btn-danger playlist-enable-disable-btn' title='Disable' data-text='Disable'><i class='fas fa-stop-circle'></i></button>"<< std::endl;
                    else
                        Php::out << "<button type='button' class='btn btn-success playlist-enable-disable-btn' title='Enable' data-text='Enable'><i class='fas fa-play-circle'></i></button>"<< std::endl;
                //}
                ////if(edit_access==1)
                {
                    Php::out << "<button type='button' class='btn btn-info playlist-edit' title='Edit'><i class='fas fa-edit'></i></button>"<< std::endl;            
                //}  
            }
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
        writeException("showPlayListSettings",e.what());
        }
    
    }

Php::Value enabledisablePlaylist(Php::Value json) {
    string msg = "Failed";
    try {
        int status = json["status"];
        int id = json["id"];
        sql::Connection *con;
        sql::PreparedStatement *prep_stmt;                    
        con = General.mysqlConnect(ServerDB);
        string query = "update playlist_settings set playlist_status=? where id=?";
        prep_stmt = con->prepareStatement(query);
        prep_stmt->setInt(1,status);
        prep_stmt->setInt(2,id);
        int n = prep_stmt->executeUpdate();
        if(n>0)
        {
            msg = "Successfull";
        }
        
        delete prep_stmt;
        delete con;
    
    }    catch (const std::exception& e) {
        writeException("enabledisablePlaylist", e.what());
    }
    
    return msg;
}

void getPlaylistVideos(Php::Value data)
    {
    try
        {
        Php::Value role;
        sql::Connection *con;
        sql::PreparedStatement *prep_stmt;            
        sql::ResultSet *res;
        int id = data["id"];
        int i=0;
        con= General.mysqlConnect(ServerDB); 
        prep_stmt=con->prepareStatement("SELECT *, (SELECT GROUP_CONCAT(media_label) FROM media_library WHERE FIND_IN_SET(id, playlist_videos)) AS media_label,(SELECT GROUP_CONCAT(media_name) FROM media_library WHERE FIND_IN_SET(id, playlist_videos)) AS media_name FROM playlist_settings where id=?");
        prep_stmt->setInt(1,id);
        res=prep_stmt->executeQuery();
        if(res->rowsCount()==1)
        {
           // Php::out<<"<table width='100%' class='jspdf-table'><tbody class='table-view'>"<<endl;
           res->next();
            vector<string>video_array = split_videolist(res->getString("media_label"),','); //video labels
            vector<string>video_name = split_videolist(res->getString("media_name"),','); //video labels
            while(i<(signed)video_array.size())
            {
                Php::out<<"<div class='btn btn-block btn-lg bg-gray'>"<<endl;
                //Php::out<<video_array[i]<<endl;
                Php::out<<"<div class='d-flex justify-content-between'>";
                Php::out<<"<div><video width='150' controls='controls' preload='metadata'><source src='"+string(MediaPath)+"/"+video_name[i]+"#t=0.5' type='video/mp4'></video></div>"<<endl;
                Php::out<<"<div>"<<video_array[i]<<"</div>"<<endl;
                Php::out<<"</div></div>"<<endl;
                i++;
                	
            }
        }
        else
        {
            Php::out<<"<div class='row'><div class='col'>Playlist is empty</div></div>"<<endl;
        }
        //Php::out<<"</table>"<<endl;	
        delete res;    
        delete prep_stmt;
        delete con;  
        }
    catch(const std::exception& e)
        {
        writeException("getPlaylistVideos",e.what());
        }
    
    }

void getVideoslibrary(Php::Value data)
    {
    try
        {
        Php::Value role;
        sql::Connection *con;
        sql::PreparedStatement *prep_stmt;            
        sql::ResultSet *res;
        
        con= General.mysqlConnect(ServerDB); 
        prep_stmt=con->prepareStatement("Select * from media_library where media_status=1");
        res=prep_stmt->executeQuery();
        if(res->rowsCount()>0)
        {
           // Php::out<<"<table width='100%' class='jspdf-table'><tbody class='table-view'>"<<endl;
            
            while(res->next())
            {
                Php::out<<"<div class='nodrop btn btn-block btn-info btn-lg bg-gray play-video' id = v"+res->getString("id")+" draggable='true' ondragstart='drag(event)' data-path='"+string(MediaPath)+"/"+res->getString("media_name")+"'>"<<endl;
                Php::out<<"<div class='d-flex justify-content-between'>";
                Php::out<<"<div><video width='100' controls='controls' preload='metadata'><source src='"+string(MediaPath)+"/"+res->getString("media_name")+"#t=0.5' type='video/mp4'></video></div>"<<endl;
                Php::out<<"<div>"<<res->getString("media_label")<<"</div>"<<endl;    
                Php::out<<"</div></div>"<<endl;
                	
            }
        }
        else
        {
            Php::out<<"<div class='row'><div class='col'>No Videos found</div></div>"<<endl;
        }
        //Php::out<<"</table>"<<endl;	
        delete res;    
        delete prep_stmt;
        delete con;  
        }
    catch(const std::exception& e)
        {
        writeException("getVideoslibrary",e.what());
        }
    
    }

Php::Value savePlaylist(Php::Value data)
{
    string msg = "Failed";    
    try
    {
        Php::Value role;
        sql::Connection *con;
        sql::PreparedStatement *prep_stmt;            
        string playlist_name = data["playlist_name"];
        string video_list = data["video_list"];
        string description = data["description"];
        
        con= General.mysqlConnect(ServerDB); 
        prep_stmt=con->prepareStatement("Insert into playlist_settings(playlist_name,playlist_description,playlist_videos)values(?,?,?)");
        prep_stmt->setString(1,playlist_name);
        prep_stmt->setString(2,description);
        prep_stmt->setString(3,video_list);
        int n=prep_stmt->executeUpdate();
        if(n>0)
        {
            msg = "Successfull";
        }
         
        delete prep_stmt;
        delete con;  
    }
    catch(const std::exception& e)
    {
        writeException("savePlaylist",e.what());
    }
    return msg;
}

void screenDropdown(Php::Value data)
{
    try
    {
        sql::Connection *con;
        sql::Statement *stmt;            
        sql::ResultSet *res;
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select device_number,device_name from system_devices where device_category=12");
        
        while(res->next())
        {
           Php::out << "<option value='" << res->getString("device_number") << "'>" << res->getString("device_name") << "</option>" << std::endl;
        }	
        delete res;    
        delete stmt;
        delete con;  
    }
    catch(const std::exception& e)
    {
        writeException("screenDropdown",e.what());
    } 
}

void screenListBtns(Php::Value data)
{
    try
    {
        string html_data="";
        sql::Connection *con;
        sql::Statement *stmt;            
        sql::ResultSet *res;
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select device_number,device_name from system_devices where device_category=12 and device_enabled=1");
        
        while(res->next())
        {
            html_data += "<div class='col-lg-9 col-md-6 block-data' data-id=" + res->getString("device_number") + ">";
            html_data += "<div class='card p-2'>";

            html_data += "<div class='card-header'>";
            html_data += "<div class='nav-item d-flex justify-content-between align-items-center'>";

            html_data += "<h3 class='card-title'>" + res->getString("device_name") + "</h3></div>";
            
            html_data += "<div class='card-body p-0'>";
            html_data += "<div class='row no-gutters'>";

            html_data += "<div class='col-4 block-view-img my-auto text-center'><img class='img-fluid' src='/parcx/Media/default.jpg' style='width:100%'></div>";

            html_data += "<div class='col-8'>";
            html_data += "<ul class='nav flex-column'>";
            html_data += "<li class='nav-item'>";
            html_data += "<span class='nav-link'>";
            html_data += "<span class='float-right device_category'>1</span>";
            html_data += "</span>";
            html_data += "</li>";

            html_data += "<li class='nav-item'>";
            html_data += "<span class='nav-link'>";
            html_data += "<span class='float-right device_ip'>2</span>";
            html_data += "</span>";
            html_data += "</li>";





            /*html_data += "<span>";


            html_data += "<span class='p-1'>";
            html_data += "<span class='fa-stack fa-1x' title='warnings'>";
            html_data += "<i class='fa fa-circle fa-stack-2x text-warning'></i>";
            html_data += "<strong class='fa-stack-1x text-dark'>" + to_string(get_alarm_count(facility_number, carpark_number, res->getString("device_number"), 6, 20)) + "</strong>";
            html_data += "</span>";

            html_data += "<span class='fa-stack fa-1x' title='critical alarms'>";
            html_data += "<i class='fa fa-circle fa-stack-2x text-danger' title='critical alarms'></i>";
            html_data += "<strong class='fa-stack-1x text-white'>" + to_string(get_alarm_count(facility_number, carpark_number, res->getString("device_number"), 0, 5)) + "</strong>";
            html_data += "</span>";
            html_data += "</span>";


            if (res->getInt("device_network_status") == 1) {
                html_data += "<span class='float-right ml-3 header-icon'>";
                html_data += "<i class='fas fa-server text-success' title='Server Connectivity'></i>";
                html_data += "</span>";
                html_data += "</span>";
            } else {
                html_data += "<span class='float-right ml-3 header-icon'>";
                html_data += "<i class='fas fa-server text-danger' title='Server Connectivity'></i>";
                html_data += "</span>";
                html_data += "</span>";
            }
            */


            html_data += "</div>";
            html_data += "</div>";
        }
        Php::out<<html_data<<endl;
        delete res;    
        delete stmt;
        delete con;  
    }
    catch(const std::exception& e)
    {
        writeException("screenListBtns",e.what());
    } 
}

void showdevicePlayList(Php::Value data)
    {
    try
        {
        Php::Value role;
        sql::Connection *con;
        sql::Statement *stmt;            
        sql::ResultSet *res;
        int device_number = 0;
        string checked = "";

        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select device_number from system_devices where device_category=12 limit 1");
        if(res->next())
        {
           device_number = res->getInt("device_number");
        }
        res=stmt->executeQuery("select a.*,b.playlist_name from device_playlist a join playlist_settings b on a.playlist_id = b.id  where device_number = "+to_string(device_number));
        if(res->rowsCount()>0)
        {
            //Php::out<<"<table class='table table-blue'>"<<endl;
            Php::out << "<thead class='thead-light'>" << std::endl;
            Php::out<<"<tr>"<<endl;
            Php::out<<"<th>Playlist</th>"<<endl;
            Php::out<<"<th>Start Date</th>"<<endl;        
            Php::out<<"<th>End Date</th>"<<endl;        
            Php::out<<"<th>Repeat Playlist</th>"<<endl; 		           
            //Php::out<<"<th></th>"<<endl;
            Php::out<<"</tr>"<<endl;	
            Php::out << "</thead>" << std::endl;	
            while(res->next())
            {
                Php::out<<"<tr data-id='"<<res->getString("id")<<"'>"<<endl;                       
                Php::out<<"<td>"+res->getString("playlist_name")<<endl;
                Php::out<<"<details class='small'>"<<endl;
                Php::out<<"<summary class='playlist-details-div' data-id='"+res->getString("playlist_id")+"'>View Details</summary>"<<endl;
                Php::out<<"<div id ='playlist-details-"+res->getString("playlist_id")+"' class='playlist-details d-inline'>"<<endl;
                Php::out<<"</div>"<<endl;
                Php::out<<"</details>"<<endl;
                Php::out<<"</td>"<<endl;
                Php::out<<"<td>"+res->getString("start_date_time")+"</td>"<<endl;   
                Php::out<<"<td>"+res->getString("end_date_time")+"</td>"<<endl; 
                if(res->getInt("repeat_videos")==1)
                {
                    checked="checked";
                }
                else
                {
                    checked="";
                }
                Php::out<<"<td><input type='checkbox' id='repeat"+res->getString("id")+"' "+checked+" disabled></td>"<<endl; 
                /*Php::out << "<td>"<< std::endl;
                //Php::out << "<button type='button' class='btn btn-info playlist-view' title='View Playlist' data-text='View Playlist' data-playlist="+res->getString("playlist_id")+"><i class='fas fa-eye'></i></button>"<< std::endl;  
                
                if(res->getInt("status")==1)
                    Php::out << "<button type='button' class='btn btn-danger playlist-enable-disable-btn' title='Disable' data-text='Disable'><i class='fas fa-stop-circle'></i></button>"<< std::endl;
                else
                    Php::out << "<button type='button' class='btn btn-success playlist-enable-disable-btn' title='Enable' data-text='Enable'><i class='fas fa-play-circle'></i></button>"<< std::endl;
                    
                    
                        //Php::out << "<button type='button' class='btn btn-info user-edit' title='Edit'><i class='fas fa-edit'></i></button>"<< std::endl;            
                    
                //}
                Php::out << "</td>"<< std::endl;*/
                Php::out<<"</tr>"<<endl;	
            }
        }
        else
        {
            Php::out<<"No Playlists found for this Screen"<<endl;
        }
        
        //Php::out<<"</table>"<<endl;	
        delete res;    
        delete stmt;
        delete con;  
        }
    catch(const std::exception& e)
        {
        writeException("showdevicePlayList",e.what());
        }
    
    }

Php::Value uploadMedia(Php::Value data)
{
    string filename = toString(data["name"]);
    string from = toString(data["from"]);
    string dest = "../../Media/Videos/"+filename;
    double size = data["size"];
    int error = data["error"];
    string type = toString(data["type"]);
    string duration = data["duration"];
    string result = "";
    writeLog("uploadMedia","size:"+to_string(size/ (1024 * 1024))+" MB");
    writeLog("uploadMedia","duration:"+duration);
    writeLog("uploadMedia","type:"+string(type));
    if(size > 200 * 1024 * 1024)
    {
        result ="File size is >200 MB" ;
    }
    else
    {
        string extension = filename.substr(filename.find_last_of(".") + 1);
        if(extension == "mp4" || extension == "avi" || extension == "MP4" || extension == "AVI" || extension == "jpeg" || extension == "png" || extension == "jpg" || extension == "gif")  {
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
                sql::Connection *con;
                sql::Statement *stmt;            
                try
                { 
                    con= General.mysqlConnect(ServerDB); 
                    stmt=con->createStatement();
                    int n = stmt->executeUpdate("Insert into media_library(media_name,media_label,media_type,media_duration,media_size) values ('"+filename+"','"+filename+"','"+type+"','"+duration+"',"+to_string(size/(1024*1024))+")");
                 
                    delete stmt;
                    delete con;
                    if(n>0)
                    {
                        result = "Success";
                    }
                    else
                    {
                        result = "Failed to create a record";
                    }
                }
            catch(const std::exception& e)
                {
                    result ="DB Error: Failed" ;
                    writeException("uploadMedia",e.what());
                }
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

void showMediaLibraryList(Php::Value data)
    {
    try
        {
        Php::Value role;
        sql::Connection *con;
        sql::Statement *stmt;            
        sql::ResultSet *res;

        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select * from media_library order by id desc");
        if(res->rowsCount()>0)
        {
            //Php::out<<"<table class='table table-blue'>"<<endl;
            Php::out << "<thead class='thead-light'>" << std::endl;
            Php::out<<"<tr>"<<endl;
            Php::out<<"<th><button type='button' class='btn btn-success upload-media-btn' title='Upload Media'><i class='fas fa-plus'></i></button></th>"<<endl;
            Php::out<<"<th>Media</th>"<<endl;
            Php::out<<"<th>Type</th>"<<endl;      
            Php::out<<"<th>Duration</th>"<<endl;      
            Php::out<<"<th>Size</th>"<<endl;      
            
           // if(edit_access==1 ||delete_access==1)
                Php::out<<"<th></th>"<<endl;		           
           
            Php::out<<"</tr>"<<endl;	
            Php::out << "</thead>" << std::endl;		
        }
        else
        {
            Php::out<<"<button type='button' class='btn btn-success upload-media-btn' title='Upload Media'><i class='fas fa-plus'></i></button></br></br>"<<endl;
        }
        while(res->next())
        {
            Php::out<<"<tr data-id='"<<res->getString("id")<<"'>"<<endl;   
            if (res->getString("media_type").find("image") != std::string::npos) {
                Php::out<<"<td><img src='"+string(MediaPath)+"/"+res->getString("media_name")+"'  style='width:100px'></td>"<<endl;
            }
            else if (res->getString("media_type").find("video") != std::string::npos) 
            {
                Php::out<<"<td><video width='100' controls='controls' preload='metadata'><source src='"+string(MediaPath)+"/"+res->getString("media_name")+"#t=0.5' type='video/mp4'></video></td>"<<endl;
            }
            else
            {
                Php::out<<"<td></td>"<<endl;
            }
            Php::out<<"<td>"+res->getString("media_label")+"</td>"<<endl;
            Php::out<<"<td>"+res->getString("media_type")+"</td>"<<endl; 

            Php::out<<"<td>"+res->getString("media_duration")+"</td>"<<endl;  
            Php::out<<"<td>"+res->getString("media_size")+" MB</td>"<<endl;  
            Php::out << "<td>"<< std::endl;
            Php::out << "<button type='button' class='btn btn-info play-video' title='View Video' data-text='View Video' data-path='"+string(MediaPath)+"/"+res->getString("media_name")+"' data-type='"+res->getString("media_type")+"'><i class='fas fa-eye'></i></button>"<< std::endl;  
            
            if(res->getInt("media_status")==1)
                Php::out << "<button type='button' class='btn btn-danger video-enable-disable-btn' title='Disable' data-text='Disable'><i class='fas fa-stop-circle'></i></button>"<< std::endl;
            else
                Php::out << "<button type='button' class='btn btn-success video-enable-disable-btn' title='Enable' data-text='Enable'><i class='fas fa-play-circle'></i></button>"<< std::endl;
               
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
        writeException("showMediaLibraryList",e.what());
        }
    
    }

Php::Value enabledisableVideo(Php::Value json) {
    string msg = "Failed";
    try {
        int status = json["status"];
        int id = json["id"];
        sql::Connection *con;
        sql::PreparedStatement *prep_stmt;                    
        con = General.mysqlConnect(ServerDB);
        string query = "update media_library set media_status=? where id=?";
        prep_stmt = con->prepareStatement(query);
        prep_stmt->setInt(1,status);
        prep_stmt->setInt(2,id);
        int n = prep_stmt->executeUpdate();
        if(n>0)
        {
            msg = "Successfull";
        }
        
        delete prep_stmt;
        delete con;
    
    }    catch (const std::exception& e) {
        writeException("enabledisableVideo", e.what());
    }
    
    return msg;
}

Php::Value getDetailsbyId(string table, string id, Php::Value data) {
    /*Php::Value response;
    try {
        con = General.mysqlConnect(ServerDB);
        prep_stmt = con->prepareStatement("select * from " + table + " where " + id + "=?");
        prep_stmt->setString(1, ToString(data["id"]));
        res = prep_stmt->executeQuery();
        if (res->next()) {
            sql::ResultSetMetaData *res_meta = res -> getMetaData();
            int columns = res_meta -> getColumnCount();
            for (int i = 1; i <= columns; i++)
                response[res_meta -> getColumnLabel(i)] = string(res->getString(i));
        }
        delete res;
        delete stmt;
        delete con;
    } catch (const std::exception& e) {
        writeException("getDetailsbyId", e.what());
    }
    return response;*/
}

Php::Value parcxContentManagement(Php::Parameters &params) {
    Php::Value data = params[0];
    int task = data["task"];
    Php::Value response;
    switch (task) {
        case 1:showPlayListSettings(data);
            break;
        case 2:response=enabledisablePlaylist(data);
            break;
        case 3:getPlaylistVideos(data);
            break;
        case 4:getVideoslibrary(data);
            break;
        case 5:response = savePlaylist(data);
            break;
        case 6:screenListBtns(data);
            break;
        case 7:screenDropdown(data);
            break;
        case 8:showdevicePlayList(data);
            break;
        case 9:response = uploadMedia(data);
            writeLog("Main",toString(response));
            break;
        case 10:showMediaLibraryList(data);
            break;
        case 11:response=enabledisableVideo(data);
            break;
        case 12:response = getDetailsbyId("playlist_settings", "id", data);
            break;
    }
    return response;
}


extern "C" {

    PHPCPP_EXPORT void *get_module() {
        static Php::Extension extension("PX_ContentManagement", "1.0");
        extension.add<parcxContentManagement>("parcxContentManagement");
        return extension;
    }
}
