<?php
  //  header("status", 201);
  // echo http_response_code(); // int(200)
  //CreateLogs("Request:".file_get_contents("php://input")); 
  header("Content-Type: application/json");
    $data="Test";

    header('status:201');

    http_response_code(201);

    echo json_encode($data);



function CreateLogs($data)
{
    date_default_timezone_set('Asia/Dubai');
    $file =date("Y-m-d").".log";	
    $fh = fopen($file, 'a') or die("can't open file");    
    fwrite($fh,date('H:i:s'). "\t");
    fwrite($fh,$data);
    fwrite($fh,"\n");
    fclose($fh);
}

?>