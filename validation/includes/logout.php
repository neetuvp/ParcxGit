<?php
session_start();
$inactive=time()-$_SESSION["last_updated_time"];  
if(isset($_SESSION["operator_id"]) && $inactive>=60*5)    
    {            
    $data["operator_id"]=$_SESSION["operator_id"];
	$data["operator"]=$_SESSION["operator"];
    writeCloudLog("User session timeout logout operator_id:".$_SESSION["operator_id"]);
    $response=logoutCloudUser($data);                         
    writeCloudLog("User session timeout logout:".json_encode($response));
    echo $response["status"]; 
    session_destroy();         
    }
          
?>