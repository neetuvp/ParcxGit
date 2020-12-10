<?php
ini_set("display_errors","1");
header("Content-Type: application/json; charset=UTF-8");
include("../classes/general.php"); 
$general = new General_Oprations();
$filename="PostToServer-Webservice";
if($_SERVER['REQUEST_METHOD']=='POST')
	{
	$request_ip_address=$_SERVER['REMOTE_ADDR']; 		
	$json= file_get_contents("php://input");		
	$general->write_logs($filename,"PostToServer","Json input: ".$json);
	$general->write_logs($filename,"PostToServer","Request IP:".$request_ip_address);
	$data= json_decode($json,TRUE); 		
	$response=uploadToServer($data);	
	}
$general->write_logs($filename,"PostToServer","Json output: ".$response."\n");
echo $response;
?>