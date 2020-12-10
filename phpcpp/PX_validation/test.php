<?php
$data["option-type"]="13";
$data["validator_id"]="1";
$response=parcxValidation($data);      
if(is_array($response))  
    echo json_encode($response);
else
    echo "Response: ".$response;

?>
