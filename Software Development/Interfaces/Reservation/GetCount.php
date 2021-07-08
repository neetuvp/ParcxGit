<?php
ini_set("display_errors","1");
header("Content-Type:application/json");
CreateLog("********************************GetCount********************************");
CreateLog($_GET['json']);
//$data = json_decode(file_get_contents("php://input"),TRUE);

if(!isset($_GET['json']))
    die("FAILURE");
CreateLog("Request:".$_GET['json']); 
$data = json_decode($_GET['json'],TRUE);


$response = CountReservation($data);
$countresponse =  json_encode($response);
 
CreateLog("Response:".$countresponse); 
echo $countresponse;


?>
