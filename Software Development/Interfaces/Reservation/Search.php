<?php
ini_set("display_errors","0");
CreateLog("********************************Search********************************");
header("Content-Type:application/json");
if(!isset($_GET['json']))
    die("FAILURE");
CreateLog("Request:".$_GET['json']); 
$data = json_decode($_GET['json'],TRUE);

$response = SearchReservation($data);
$searchresponse =  json_encode($response);
 
CreateLog("Response:".$searchresponse); 
echo $searchresponse;


?>
