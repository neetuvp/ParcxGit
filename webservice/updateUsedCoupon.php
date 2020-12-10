<?php
require_once './dbfunctions_server.php';
//date_default_timezone_set('Asia/dubai');
$db=new DB_Functions();  
if($_SERVER['REQUEST_METHOD']=='POST'){
    
    //$json = $_POST['json']; 
    $json= file_get_contents("php://input");
    $db->writeLog("update used coupon input:".$json);
    $data= json_decode($json);  
    $flag=$db->updateUsedCoupon($data->{'couponId'},$data->{'ticket'});    
    if($flag>0)
        {
        $response['error'] = "FALSE"; 
        $response['message'] = "Data updated";            
        }
    else
        {
        $response['error'] = "TRUE"; 
        $response['message'] = "updation unsuccessfull"; 
        }
}
else
        {
        $response['error'] = "TRUE"; 
        $response['message'] = "no data"; 
        }
$db->writeLog("update used coupon output:".json_encode($response));
echo json_encode($response);