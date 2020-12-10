<?php
ini_set("display_errors","1");
function CreateLogs($data)
	{
    date_default_timezone_set('Asia/Dubai');
    $file = "/var/log/parcx/Services/ApplicationLogs/Parcx-PlateDetails-".date("Y-m-d").".log";	
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
	$plate_number= $data->{'plate_number'};		
	$zone_rate=$data->{'zone_rate_enabled'};		
	$find_slot=$data->{'locate_parking_slot_enabled'};

	$plate_number= 'AB123';		
	$zone_rate=1;		
	$find_slot=1;

	$response=getCapturedDetailsOfPlate($plate_number);	
	
	$find_car_response=FindCar($plate_number);	
	CreateLogs("find car response:".$find_car_response);
	$json=json_decode($find_car_response,TRUE);
 
if($find_slot==1)
		{
		$response["parking_zone"]=$json[0]["zone"]["name"];		
        $response["parking_slot"]=$json[0]["id"];

		}
	if($zone_rate==1)
		{
		$response["rate_type"]=1;
		$response["rate_plan"]="parking_rates".$json[0]["zone"]["id"];
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