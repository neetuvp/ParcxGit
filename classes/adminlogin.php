
<?php
include('../../includes/common.php');
session_start();

class AdminPanel
{    
       // TO DO

function log($msg)
	{
	$myfile=fopen("/opt/parcx/Logs/WebApplication/ApplicationLogs/parcx".date("d-m-Y").".log","a");
        fwrite($myfile, date("H:i:s")."\t".$msg."\n");
        fclose($myfile);
	}
function error($msg)
	{
	$myfile=fopen("/opt/parcx/Logs/WebApplication/ExceptionLogs/parcx".date("d-m-Y").".log","a");
        fwrite($myfile, date("H:i:s")."\t".$msg."\n");
        fclose($myfile);
	}

        
function db_connect_config(){	
        try
            
            {                        
            $con=mysqli_connect(DB_HOST,DB_USER,DB_PASSWORD,DB_NAME);
        
            if(!$con) 
            {     
	    $this->log("Cant connect to db")  ;                             
            }       

            return $con;
            
            } catch (Exception $ex) 
            {
		$this->error($ex->getMessage());                
                return null;
            }
            
        
        
        } // end 
           


    
 function Userlogin(string $json)
        {        
	
        $data=json_decode($json); 
        header("Content-Type:application/json");	
        
        $results= user_login($data->{'username'},md5($data->{'password'}));
	$this->log("UserLogin\tUsername:".$data->{'username'});
        //empty result  
        if (empty($results)) 
        {
           
            return $this->response(500,"Wrong Username / Password Combination or Validity expired");
        }
        else
       {
        if($results[3]=='1') //Enable Menu
        { 
                  
            $_SESSION['user'] = $results[0];
            $_SESSION['userRollName'] = $results[2]; 
            $_SESSION['userRollId'] = $results[1]; 
            $_SESSION['userId'] = $results[4];             

            $_SESSION['success']  = "You are now logged in";
            $_SESSION['last_login_timestamp'] = time();
            return $this->response(200,"admin"); 

        }
        else{

        return $this->response(500,"Your In Desable Mode. Please Contact Admin");

        }           
        }
      }
   

function addUser(string $json)
        {  
	$this->log("Add user:input".$json);      
        $data=json_decode($json); 
        header("Content-Type:application/json");
        
        $msg= add_user($data->{'fullName'},$data->{'userName'},$data->{'userEmail'},$data->{'userType'},md5($data->{'password'}),$data->{'companyname'},$data->{'phone'},$data->{'userFromDate'},$data->{'userToDate'});
                    
        if($msg="success")				
                {                
                return $this->response(200,"User Inserted Successfully");
                }
            else
                {                                 
                return $this->response(404,"Failed To User Inserted");
                }
                         
        }
        
        
function EditByUser(string $json)
        {        
	$this->log("Edit user:input".$json);      
        $data=json_decode($json); 
        header("Content-Type:application/json");
        
        $msg= edit_data_by_user($data->{'id'});
                               
        return $this->response(200,$msg);
           
                         
        }
        
        
        
        
    function UpdateUser(string $json)
        {  
	$this->log("Update user:input".$json);            
        $data=json_decode($json); 
        header("Content-Type:application/json");
        
        $msg= update_user($data->{'id'},$data->{'name'},$data->{'email'},$data->{'type'},$data->{'company'},$data->{'phone'},$data->{'fromDate'},$data->{'toDate'});
                    
        if($msg="success")				
                {                
                return $this->response(200,"User Details updated Successfully");
                }
            else
                {                                 
                return $this->response(404,"Failed To User Inserted");
                }
                         
        }
        
             
     function DisablUserRole(string $json)
        { 
	$this->log("Disable user:input".$json);             
        $data=json_decode($json); 
        header("Content-Type:application/json");
        
        $msg= disable_user_role($data->{'id'});
                    
        if($msg="success")				
                {                
                return $this->response(200,"Disable Status Successfully Updated");
                }
            else
                {                                 
                return $this->response(404,"Failed Status Update");
                }
                         
        }
        
    //  Updating
        
      function EnableUserRole(string $json)
        { 
	$this->log("Enable user:input".$json);             
        $data=json_decode($json); 
        header("Content-Type:application/json");
        
        $msg= enable_user_role($data->{'id'});
                    
        if($msg="success")				
                {                
                return $this->response(200,"Enable Status Successfully Updated");
                }
            else
                {                                 
                return $this->response(404,"Failed Status Update");
                }             
        }
        
    // Disabling User Function 
        
       function UserStatusDisable(string $json)
        {  
	$this->log("Disable user:input".$json);            
        $data=json_decode($json); 
        header("Content-Type:application/json");
        
        $msg= user_disable_status($data->{'id'});
                    
        if($msg="success")				
                {                
                return $this->response(200,"Status Successfully Updated");
                }
            else
                {                                 
                return $this->response(404,"Failed Status Update");
                }
                         
        }
        
   //  User Enabeling Function
        
      function UserStatusEnable(string $json)
        {  
	$this->log("Enable user:input".$json);            
        $data=json_decode($json); 
        header("Content-Type:application/json");
        
        $msg= user_enable_status($data->{'id'});
                    
        if($msg="success")				
                {                
                return $this->response(200,"Status Successfully Updated");
                }
            else
                {                                 
                return $this->response(404,"Failed Status Update");
                }             
        }
        

    function response($status,$message)
        {        
        header("HTTP/1.1".$response['status']);
        $response['status']=$status;
        $response['message']=$message;
        $json_response=json_encode($response);
        $this->log($json_response);
        return $json_response;
        }//end of .response
        
}//end of class Reservation
