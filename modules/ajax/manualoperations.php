<?php  
require_once('../../includes/common.php');
include('../../classes/ParkingOperations.php');
$operations=new ParkingOperations();
$task=$_GET['task'];
$data_obj=json_decode(file_get_contents("php://input"));
switch($task)
	{    
    case 1: 
			$description=$data_obj->{'description'};
			$movement_type=$data_obj->{'movement_type'};   
			$reason=$data_obj->{'reason'}; 
			$device_number=$data_obj->{'device_number'};    
			$device_name=$data_obj->{'device_name'};
			$carpark_number=$data_obj->{'carpark_number'};
			$operator=$data_obj->{'operator'};					
			$result=$operations->InsertManualMovements($description,$movement_type,$reason,$device_number,$device_name,$carpark_number,$operator);		
			echo $result;
			break;
		
	case 2:
			$device_ip=$data_obj->{'device_ip'};
			$result=sendtcpmessage($device_ip,"8091","A01");
			echo $result;
			break;
	} // End Switch 
?>

