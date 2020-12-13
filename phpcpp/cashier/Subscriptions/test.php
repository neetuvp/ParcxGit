<?php
$data["task"]="2";
//$data["id"]="5";
$data["ticket_id"]="59457130";
echo json_encode($data)."\n";
$response=parcxContractParking($data);
echo json_encode($response);
?>
