<?php
$json= file_get_contents("php://input");
$case=$_GET["type"];
$data=json_decode($json);
switch($case)
{
        case 1:                     
                $response=updateStatusDynamicParkingRate($data->{"id"},0);
                break; 
        case 2:                     
                $response=updateStatusDynamicParkingRate($data->{"id"},1);
                break; 
        case 3:  
                $response=insertdynamicParkingRate($data->{"name"},$data->{"date"},$data->{"day"},$data->{"type"},$data->{"rate"});
                break;        
        case 5:    
                $response=insertVmsShedule($data->{"id"},$data->{"name"},$data->{"message"},$data->{"colour"},$data->{"action"},$data->{"time"},$data->{"interval"},$data->{"holdTime"},$data->{"startDateTime"});
                break; 
        case 6:
                $response=updateStatusVmsShedule($data->{"id"},1);
                break;
        case 7:
                $response=updateStatusVmsShedule($data->{"id"},0);
                break;
        case 8:
                $response=updateVmsShedule($data->{"id"},$data->{"name"},$data->{"message"},$data->{"colour"},$data->{"action"},$data->{"time"},$data->{"interval"},$data->{"holdTime"},$data->{"startDateTime"},$data->{"editId"});
                break;
        case 9:
                $response=insertVmsDevice($data->{"name"},$data->{"vmsLocation"},$data->{"portName"},$data->{"holdStatus"});
                break;
        case 10:
                $response=updateStatusVmsDevices($data->{"id"},0);
                break;
        case 11:
                $response=updateStatusVmsDevices($data->{"id"},1);
                break;
        case 12:
                $response=updateVmsDevice($data->{"name"},$data->{"vmsLocation"},$data->{"portName"},$data->{"holdStatus"},$data->{"id"});
                break;
}
echo $response;
?>