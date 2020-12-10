<?php
$data["device_number"]="11";
$data["currency"]="AED";
$data["device_type"]=1;
$data["carpark_number"]=1;
$data["facility_number"]=200005;
$data["task"]=16;

$response=downloadDeviceSettings($data);
if(is_array($response))
    $response=json_encode($response);
echo $response;

?>