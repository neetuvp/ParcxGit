<?php 
$json=file_get_contents("php://input");
$data=json_decode($json,TRUE);
echo parkingReport($data);
?>
