<?php
include('../../classes/counterAlert.php');
$json= file_get_contents("php://input");
$obj=new counterAlert();
$case=$_GET["type"];
switch($case)
{
    case 1:                     
            $response=$obj->counterCondition($json);     
            break; 
    case 2:                     
            $response=$obj->counterTimeInterval($json);     
            break; 
    case 3:                     
            $response=$obj->networkStatus($json);     
            break;                 
}
echo $response;
?>