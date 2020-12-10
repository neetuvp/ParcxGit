<?php
ini_set("display_errors","1");
header("Content-Type: application/json; charset=UTF-8");
include("../classes/general.php"); 
$general = new General_Oprations();
$filename="PX_PlateSuggestions-Webservice";

 $general = new General_Oprations();

//$general->write_logs("Test","********************************Create********************************","Testing write logs function");
//$general->write_exception("Test","********************************Create********************************","Testing write exception in function");


//echo file_get_contents("php://input");
if($_SERVER['REQUEST_METHOD']=='POST')
	{
		$general->write_logs($filename,"POST Method","got a post request");  
	$json= file_get_contents("php://input");	
	$general->write_logs($filename,"Json input: ",$json);    
	$response=PlateSuggestions($json);
	$general->write_logs($filename,"Json output: ",$response);    
	echo $response;
    }
    else{
		$general->write_logs($filename,"POST Method","not a post request");    
		}

	
?>
