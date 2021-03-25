<?php
ini_set("display_errors","1");
header("Content-Type: application/json; charset=UTF-8");
include("../classes/general.php"); 
$general = new General_Oprations();
$filename="PXTicketCheck-webservice";
$request_ip_address=$_SERVER['REMOTE_ADDR']; 
//$request_proxy_ip_address=$_SERVER['HTTP_X_FORWARDED_FOR'];// To be logged 	

$json= file_get_contents("php://input");		
$general->write_logs($filename,"TicketCheck","Json input: ".$json);
$general->write_logs($filename,"TicketCheck","Request IP:".$request_ip_address);
$data= json_decode($json,TRUE); 		
$response=parcxTicketCheck($data);
$response['status_code']=200;
$response['status_message']="OK";	
$response['transaction_id']=$data['transaction_id'];
http_response_code(200);

$general->write_logs($filename,"TicketCheck","Json output: ".json_encode($response)."\n");
echo json_encode($response);
?>