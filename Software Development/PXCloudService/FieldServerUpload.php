<?php

ini_set("display_errors", 1);
header("Content-Type: application/json; charset=UTF-8");
if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $json = file_get_contents("php://input"); 
    //$json = str_replace("'","",$json);
    $json_request = json_decode($json, true);
    
    //Add request ip address for posting parking_movements_iot
    $json_request["ipaddress"] = $_SERVER['REMOTE_ADDR'];
    $json = json_encode($json_request);
    
    $data = json_decode(PostDataToServer($json), true);
    $response = $data;
    $response['status_code'] = 200;
    $response['status_message'] = "OK";
    $response['transaction_id'] = $json_request['transaction_id'];
    http_response_code(200);
} else {
    http_response_code(405); //405 = Method Not Allowed ie if POST is used instead of GET 
    $response['status_code'] = 405;
    $response['status_message'] = "Method not allowed";
    $response['result_description'] = "The method is incorrect";
    $response['result'] = "failed";
}
echo json_encode($response);

?>
