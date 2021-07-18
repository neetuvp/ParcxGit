<?php

ini_set("display_errors","1");
header("Content-Type: application/json; charset=UTF-8");

//$general = new General_Oprations();
$filename="PXTicketCheck/ApplicationLog-PX-TicketCheck-";
$request_ip_address=$_SERVER['REMOTE_ADDR']; 
//$request_proxy_ip_address=$_SERVER['HTTP_X_FORWARDED_FOR'];// To be logged 	
write_log("START","====================================================");
$json= file_get_contents("php://input");		
write_log("TicketCheck","Json input: ".$json);
write_log("TicketCheck","Request IP:".$request_ip_address);
$data= json_decode($json,TRUE); 		
$response=parcxTicketCheck($json);
$response['status_code']=200;
$response['status_message']="OK";	
$response['transaction_id']=$data['transaction_id'];
http_response_code(200);

write_log("TicketCheck","Json output: ".json_encode($response)."\n");
write_log("END","====================================================");
echo json_encode($response);

function write_log($function_name, $message) 
{
    date_default_timezone_set('Asia/Dubai');
    $log_file_path = "/opt/parcx/Logs/Services/PXCloudTicketCheck/ApplicationLog-PX-TicketCheck-". date("Y-m-d") . ".log";
    if (file_exists($log_file_path)) 
        {
        $myfile = fopen($log_file_path, 'a');
        shell_exec('chmod -R 777 '.$log_file_path);            
        } 
    else 
        {
        $myfile = fopen($log_file_path, 'w');
        shell_exec('chmod -R 777 '.$log_file_path);
        }        
    fwrite($myfile, date("Y-m-d H:i:s") . "\t" . $function_name . ":" . $message . "\r\n\n");
    fclose($myfile);
}
?>