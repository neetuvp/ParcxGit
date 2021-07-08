<?php
ini_set("display_errors","1");
header("Content-Type:application/json");
CreateLogs("################################Request################################");

CreateLogs("Request:".file_get_contents("php://input")); 

$json=file_get_contents("php://input");
$data=json_decode($json,TRUE);
$response=parcxReservation($data);

if(is_array($response)){
	echo json_encode($response);
        CreateLogs("Response:".json_encode($response)); 
}
else{
	echo $response;
        CreateLogs("Response:".$response); 
}



function CreateLogs($data)
{
    date_default_timezone_set('Asia/Dubai');
    $file = "/opt/parcx/Logs/Services/ApplicationLogs/PX_Reservation_".date("d-m-Y").".log";	
    $fh = fopen($file, 'a') or die("can't open file");    
    fwrite($fh,date('H:i:s'). "\t");
    fwrite($fh,$data);
    fwrite($fh,"\n");
    fclose($fh);
}

?>
