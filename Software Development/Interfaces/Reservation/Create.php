<?php
ini_set("display_errors","1");
header("Content-Type:application/json");
CreateLogs("********************************Create********************************");

CreateLogs("Request:".file_get_contents("php://input")); 
$createresponse = "";
$data = json_decode(file_get_contents("php://input"),TRUE);
$response = CreateReservation($data);
$createresponse =  json_encode($response);
 
CreateLogs("Response:".$createresponse); 
echo $createresponse;

function CreateLogs($data)
{
    date_default_timezone_set('Asia/Dubai');
    $file = "PhpLog/".date("Y-m-d").".log";	
    $fh = fopen($file, 'a') or die("can't open file");    
    fwrite($fh,date('H:i:s'). "\t");
    fwrite($fh,$data);
    fwrite($fh,"\n");
    fclose($fh);
}

?>
