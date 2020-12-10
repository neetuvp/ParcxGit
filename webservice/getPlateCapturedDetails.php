<?php
ini_set("display_errors","1");
function CreateLogs($data)
	{
    date_default_timezone_set('Asia/Dubai');
    $file = "/var/log/parcx/Services/ApplicationLogs/Parcx-ANPR-".date("Y-m-d").".log";	
    $fh = fopen($file, 'a') or die("can't open file");    
    fwrite($fh,date('H:i:s'). "\t");
    fwrite($fh,$data);
    fwrite($fh,"\n");
    fclose($fh);
	}
function resizeImage($DESIRED_WIDTH,$DESIRED_HEIGHT,$cameraNo,$imageName)
	{
	CreateLogs("resizeImage: ".$DESIRED_WIDTH." * ".$DESIRED_HEIGHT." Camera:".$cameraNo." ImageName:".$imageName);	
	$location="/opt/lampp/htdocs/ANPR/Images/Scene/".$cameraNo."/Scene_";   
	$image =$location.$imageName;
	$resizedDestination = $location."resized.jpg";       
	$imageSize = getImageSize($image);
	$imageWidth = $imageSize[0];
	$imageHeight = $imageSize[1];		     
	$originalImage = imageCreateFromJPEG($image);
	$resizedImage = imageCreateTrueColor($DESIRED_WIDTH, $DESIRED_HEIGHT);
	imageCopyResampled($resizedImage, $originalImage, 0, 0, 0, 0, $DESIRED_WIDTH+1, $DESIRED_HEIGHT+1, $imageWidth, $imageHeight);
	imageJPEG($resizedImage, $resizedDestination);
	imageDestroy($originalImage);
	imageDestroy($resizedImage); 	
	}	
	
function updateWalet($plate_number,$ticket_id,$amount,$device_number,$device_name,$carpark_number,$carpark_name,$facility_number,$facility_name)
	{
	$data["plate_number"]=$plate_number;
	$data["ticket_id"]=$ticket_id;
	$data["amount"]=$amount;
	$data["device_number"]=$device_number;
	$data["device_name"]=$device_name;
	$data["carpark_number"]=$carpark_number;
	$data["carpark_name"]=$carpark_name;
	$data["facility_number"]=$facility_number;
	$data["facility_name"]=$facility_name;
	$data["task"]=1;	
	$json=json_encode($data);
	CreateLogs("updateWalet: input:".$json);
	$ch= curl_init("http://localhost/parcx/webservice/walet_operations.php");
	curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "POST");
	curl_setopt($ch, CURLOPT_POSTFIELDS, $json);
	curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-Type:application/json','Accept:application/json'));
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
	$response= curl_exec($ch);
	curl_close($ch);
	CreateLogs("updateWalet: output:".$response);
	return $response;	
	}
if($_SERVER['REQUEST_METHOD']=='POST')
	{
	$json= file_get_contents("php://input");	
	CreateLogs("Json input: ".$json);
	$data= json_decode($json); 		
	$device_number= $data->{'device_number'};	
	$reservation_enabled= $data->{'reservation_enabled'};
	$reservation_entry_grace_period=$data->{'reservation_entry_grace_period'};
	$reservation_exit_grace_period=$data->{'reservation_exit_grace_period'};
	$chipcoin_enabled=$data->{'chipcoin_enabled'};	
	$walet_enabled=$data->{'walet_enabled'};
	$resize_image=$data->{'resize_image'};	
	$i=0;
	do
		{
		sleep(1);
		$response=getPlateCapturedDetails($device_number,$reservation_enabled,$reservation_entry_grace_period,$reservation_exit_grace_period,$chipcoin_enabled);		
		$i++;
		}
	while($response["plate_captured_status"]=="false" && $i<3);
	
	CreateLogs("Plates details:: ".json_encode($response));
	
	if($response["plate_captured_status"]=="true")
		{
		if($resize_image==1)						
			resizeImage($data->{'image_width'},$data->{'image_height'},$response["camera_id"],$response["plate_image_name"]);
			
		
		
		if($walet_enabled==1 && $response["device_type"]!=1)
			{
			CreateLogs("Walet enabled");	
			$amount_to_pay=	 $response["amount_to_pay"];	
			if($amount_to_pay>0)
				{
				$walet=updateWalet($response["plate_number"],$response["ticket_id"],$amount_to_pay,$device_number,$response["device_name"],$response["carpark_number"],$response["carpark_name"],$response["facility_number"],$response["facility_name"]);
				CreateLogs("walet update output:".$walet);
				$walet_json=json_decode($walet,true);
				$response["result"]=$walet_json["result"];
				$response["wallet_present"]=$walet_json["wallet_present"];				
				$response["wallet_payment"]=$walet_json["wallet_payment"];
				$response["season_card"]=$walet_json["season_card"];
				if($walet_json["walet_present"]=="true")
					{						
					$response["balance_amount"]=$walet_json["wallet_balance"];
					}
				}
			}
		}
	}
else
	{	
	$response["error"]='true';
	$response["result"]="Not a post request";
	}
CreateLogs("Json output: ".json_encode($response)."\n");
echo json_encode($response);	
?>