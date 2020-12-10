<?php
date_default_timezone_set("Asia/dubai");
if($_SERVER["REQUEST_METHOD"]=='POST')
        {
        $json= file_get_contents("php://input");
        $data=json_decode($json,TRUE);
        $response=parcxValidation($data);        
        if(is_array($response))
	        {
                if($data["option-type"]==1 && $response["status"]=="200")
                        {
                        session_start();        
                        $_SESSION["user_name"]=$response["user_name"];
                        $_SESSION["operator_name"]=$response["display_name"];
                        $_SESSION["operator_id"]=$response["user_id"];
                        $_SESSION["logo"]=$response["logo"];
                        $_SESSION["login_time"]=date("Y-m-d H:i:s");
                        $_SESSION["report_view"]=$response["report_view"];
                        }        
                echo json_encode($response);
                }
        else
	        echo $response;             
        }
?>
