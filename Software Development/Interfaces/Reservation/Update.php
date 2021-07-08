<?php
ini_set("display_errors","0");
header("Content-Type:application/json");
CreateLog("********************************Update********************************");
CreateLog("Request:".file_get_contents("php://input"));
$data = json_decode(file_get_contents("php://input"),TRUE);
$response = UpdateReservation($data);
$updateresponse =  json_encode($response);
 
CreateLog("Response:".$updateresponse); 
echo $updateresponse;


?>
