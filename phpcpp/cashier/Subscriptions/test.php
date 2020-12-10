<?php
$data["task"]="3";
//$data["id"]="5";
$data["ticket_id"]="212121212121";
$data["plate_number"]="h 95943";
$data["customer_name"]="jeena";
$data["mobile"]="971553736589";
$data["email"]="";
$data["company_name"]="afme";
$data["description"]="test";
$data["tag"]="123456";
$data["product_id"]="3";

echo json_encode($data);
$response=parcxContractParking($data);
echo json_encode($response);
?>
