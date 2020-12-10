<?php
ini_set("display_errors","1");
function CreateLogs($data)
	{
    date_default_timezone_set('Asia/Dubai');
    $file = "/var/log/parcx/Services/ApplicationLogs/Parcx-Reservation-".date("Y-m-d").".log";	
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
	$ticket_no= $data->{'ticket_no'};
	$reservation_entry_grace_period= $data->{'reservation_entry_grace_period'};
	$reservation_exit_grace_period= $data->{'reservation_exit_grace_period'};	
	$carpark= $data->{'carpark_number'};
	$facility= $data->{'facility_number'};
	$device_type= $data->{'device_type'};
		
	$reservation_response=getReservationDetails($ticket_no,$reservation_entry_grace_period,$reservation_exit_grace_period,$carpark,$facility,$device_type);		
	$response=$reservation_response;	
	}
else
	{	
	$response["error"]='true';
	$response["result"]="Not a post request";
	}
CreateLogs("Json output: ".json_encode($response)."\n");
echo json_encode($response);	
?>