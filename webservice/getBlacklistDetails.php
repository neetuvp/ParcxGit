<?php
ini_set("display_errors","1");
function CreateLogs($data)
	{
    date_default_timezone_set('Asia/Dubai');
    $file = "/var/log/parcx/Services/ApplicationLogs/Parcx-Blacklist-".date("Y-m-d").".log";	
    $fh = fopen($file, 'a') or die("can't open file");    
    fwrite($fh,date('H:i:s'). "\t");
    fwrite($fh,$data);
    fwrite($fh,"\n");
    fclose($fh);
	}
if($_SERVER['REQUEST_METHOD']=='POST')
	{
	$json= file_get_contents("php://input");	
	CreateLogs("Json input: ".$json);
	$data= json_decode($json); 
	
	$ticketNo= $data->{'ticket_no'};		
	$blacklistedStatus= isBlacklisted($ticketNo);
	CreateLogs("Blacklist status: ".$blacklistedStatus);
	
	if(strcasecmp($blacklistedStatus, "blacklisted")==0)
		{
		$response["error"]='false';
		$response["blacklisted"]='true';
		$response["result"]='Blacklisted';
		}
	else
		{
		$response["error"]='false';
		$response["blacklisted"]='false';
		$response["result"]='Not Blacklisted';
		}	
	
	}
else
	{	
	$response["error"]='true';
	$response["result"]="Not a post request";
	}
CreateLogs("Json output: ".json_encode($response)."\n");
echo json_encode($response);	
?>