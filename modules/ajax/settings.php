<?php 
$json=file_get_contents("php://input");
$data=json_decode($json,TRUE);
$response=parcxSettings($data);
if(is_array($response))
	echo json_encode($response);
else
	echo $response;
?>
