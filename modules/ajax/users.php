<?php 
$json=file_get_contents("php://input");
$data=json_decode($json,TRUE);
$response=parcxUserManagement($data);
if(is_array($response))
    {    
    if($data["task"]==12)
        {        
        if($response["message"]=="Success")
            {
            session_start();
            $_SESSION['user'] = $response["operator_name"];
            $_SESSION['userRollName'] = $response["user_role_name"];
            $_SESSION['userRollId'] = $response["user_role_id"]; 
            $_SESSION['userId'] = $response["user_id"];            
            $_SESSION["language"] = $response["language"] ;        
            $_SESSION['last_login_timestamp'] = time();
            echo $response["message"];
            }
        }
    else 
        echo json_encode($response);
    }
else
    echo $response;
?>
