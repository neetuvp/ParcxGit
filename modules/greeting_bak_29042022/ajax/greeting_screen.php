<?php 
$json=file_get_contents("php://input");
WriteLog("Request: ".$json);
$data=json_decode($json,TRUE);

$response=parcxGreetingScreen($data);
if(is_array($response))
    $response=json_encode($response);
WriteLog("Response: ".$json);
echo $response;

function WriteLog($data)
    {
    date_default_timezone_set('Asia/Dubai');
    if (!file_exists('Logs')) {
        mkdir('Logs', 0777, true);
    }
    $file = "Logs/".date('Y-m-d');
    $fh = fopen($file, 'a') or die("can't open file");
    fwrite($fh,"\n");
    fwrite($fh,"Date :".date('Y-m-d H:i:s'). " ");
    fwrite($fh,$data);
    fclose($fh);
    }

?>
