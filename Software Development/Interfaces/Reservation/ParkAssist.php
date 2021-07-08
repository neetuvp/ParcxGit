<?php
ini_set("display_errors","1");
header("Content-Type:application/json");
CreateLog("********************************ParkAssist********************************");
/*CreateLog($_GET['json']);
//$data = json_decode(file_get_contents("php://input"),TRUE);

if(!isset($_GET['json']))
    die("FAILURE");
CreateLog("Request:".$_GET['json']); 
$data = json_decode($_GET['json'],TRUE);*/

$ch = curl_init();
$url = "https://insights.parkassist.com/find_your_car/zones.json?device=4C3B51D4-6B43-4615-BBD5-DD1D3DDB7742&lat=40.75518806468766&on=
-73.99224584929085&signature=e6f095c74365b6d556d9544ebc49b8a9&site=SITESLUG&ts=1376927197607";
curl_setopt($ch, CURLOPT_URL, $url);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);
$output = curl_exec($ch);
if (curl_errno($ch)) {
    echo "Curl Error";
}
else
{
   $resultStatus = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    if ($resultStatus == 200)
    {
        echo "Curl Success";
    }
    else
    {
        echo "Curl Failure";
    }
//print_r($output);

}
curl_close($ch);
echo "\n";
echo "Output:".$output;

?>