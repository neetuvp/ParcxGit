<?php
ini_set("display_errors","1");
header("Content-Type:application/json");
CreateLog("********************************Cancel********************************");
CreateLog("Request:".file_get_contents("php://input"));
$data = json_decode(file_get_contents("php://input"),TRUE);
$response = CancelReservation($data);
$cancelresponse =  json_encode($response);
 
CreateLog("Response:".$cancelresponse); 
echo $cancelresponse;



?>
