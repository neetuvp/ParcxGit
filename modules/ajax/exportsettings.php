<?php
$json= file_get_contents("php://input");
$case=$_GET["type"];
$data=json_decode($json);
switch($case)
{
    case 1:  
    {
        $id = "";
        $data = json_decode($json);
        $id = $data -> {'id'};
        $export_flag = $data -> {'export_flag'};
        $ssh = $data -> {'ssh'};
        $ftp = $data -> {'ftp'};
        $ssh_interface_id = $data -> {'ssh_interface_id'};
        $ftp_interface_id = $data -> {'ftp_interface_id'};
        $response = UpdateExportSettings($id,$export_flag,$ssh,$ftp,$ssh_interface_id,$ftp_interface_id);
        break;
    }
    

}
echo $response;
?>