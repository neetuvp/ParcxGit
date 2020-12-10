<?php
$json["task"]=3;
$json["id"]=1;
$json["check_plate_type"]=1;
$json["check_plate_country"]=1;
$json["check_plate_area"]=2;
$json["check_plate_prefix"]=2;
$json["check_plate_number"]=2;
$json["check_partial_plate_number"]=2;
$json["check_plate_number_begining"]=2;
$json["check_plate_number_end"]=2;
$json["allow_O_Q_mismatch"]=2;
$response=anprSettings($json);
echo json_encode($response);
?>
