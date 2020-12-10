<?php
$json= file_get_contents("php://input");
$case=$_GET["type"];
$data=json_decode($json);
$response = "";
switch($case)
{
    case 1:  
    {
        $id = "";
        $data = json_decode($json,true);
        //$type = $data -> {'type'};
        //$response = GetCustomerMessagesText($type);
        $response = GetMessageTextByLanguage($data);
        break;
    }
    case 2:  
    {
        $data = json_decode($json,true);
        $response = UpdateRecord($data);
        break;
    }
    case 3:  
    {
        $data = json_decode($json,true);
        $response = InsertRecord($data);
        break;
    }
    case 4:  
    {
        $id = "";
        $data = json_decode($json);
        $type = $data -> {'type'};
        $response = GetCustomerMessagesMedia($type);
        break;
    }
    case 5:
    {
        $data = json_decode($json,true);
        $response = UpdateImagePath($data);
        break;
    }
    case 6:
    {
        $response = json_encode(GetCustomerMessageSettings());
        break;
    }
    
    

}
echo $response;
?>