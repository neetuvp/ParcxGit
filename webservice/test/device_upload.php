<?php
ini_set("display_errors","1");
header("Content-Type: application/json; charset=UTF-8");
include("../classes/general.php"); 
$general = new General_Oprations();
$filename="PXDeviceUpload-webservice";
if($_SERVER['REQUEST_METHOD']=='POST')
	{
	$request_ip_address=$_SERVER['REMOTE_ADDR']; 		
	$json= file_get_contents("php://input");		
	$general->write_logs($filename,"PostToServer","Json input: ".$json);
	$general->write_logs($filename,"PostToServer","Request IP:".$request_ip_address);		
	$data= json_decode($json,TRUE); 	
    $request=$data["data"];  
    $task= $data["task"];    
    if($task==6) 
        {
        $con=mysqli_connect("localhost","parcxservice","1fromParcx!19514","parcx_reporting");  
        $k=0;  
        for($i=0;$i<sizeof($request);$i++)
            {
                $device_number = $request[$i]["device_number"];
                $device_name = $request[$i]["device_name"];
                $carpark_number = $request[$i]["carpark_number"];
                $carpark_name = $request[$i]["carpark_name"];
                $facility_number = $request[$i]["facility_number"];
                $id = $request[$i]["id"];
                $operator_id = $request[$i]["operator_id"];
                $operator_name = $request[$i]["operator_name"];
                $shift_id = $request[$i]["shift_id"];
                $parking_rate_label = $request[$i]["parking_rate_label"];
                $parking_rate_name = $request[$i]["parking_rate_name"];
                $entry_grace_period = $request[$i]["entry_grace_period"];
                $exit_grace_period = $request[$i]["exit_grace_period"];
                $vat_type = $request[$i]["vat_type"];
                $vat_percentage = $request[$i]["vat_percentage"];
                $ticket_id = $request[$i]["ticket_id"];

                $entry_date_time = $request[$i]["entry_date_time"];
                if ($entry_date_time == "")
                    $entry_date_time = "NULL";
                else
                    $entry_date_time = "'" . $entry_date_time . "'";

                $payment_date_time = $request[$i]["payment_date_time"];

                $max_exit_date_time = $request[$i]["max_exit_date_time"];
                if ($max_exit_date_time == "")
                    $max_exit_date_time = "NULL";
                else
                    $max_exit_date_time = "'" . $max_exit_date_time . "'";

                $parking_duration = $request[$i]["parking_duration"];
                $payment_category = $request[$i]["payment_category"];
                $parking_fee = $request[$i]["parking_fee"];
                $vat_amount = $request[$i]["vat_amount"];
                $lost_fee = $request[$i]["lost_fee"];
                $admin_fixed_charges = $request[$i]["admin_fixed_charges"];
                $ticket_replacement_fee = $request[$i]["ticket_replacement_fee"];
                $discount_amount = $request[$i]["discount_amount"];
                $gross_amount = $request[$i]["gross_amount"];
                $amount_received = $request[$i]["amount_received"];
                $discount_id = $request[$i]["discount_id"];
                if ($discount_id == "")
                    $discount_id = "0";
                $discount_category = $request[$i]["discount_category"];
                $discount_name = $request[$i]["discount_name"];
                $coupon_id = $request[$i]["coupon_id"];
                $coupon_category = $request[$i]["coupon_category"];
                $coupon_source = $request[$i]["coupon_source"];
                $payment_type = $request[$i]["payment_type"];
                
                $validation_value = $request[$i]["validation_value"];
                $validation_id = $request[$i]["validation_id"];
                $validator_id=$request[$i]["validator_id"];
                $validation_type=$request[$i]["validation_type"];
                
                
                
                        
                $balance_returned = $request[$i]["balance_returned"];
                $credit_note = $request[$i]["credit_note"];
                $bank_notes = $request[$i]["bank_notes"];
                $receipt_number = $request[$i]["receipt_number"];
                $authentication_code = $request[$i]["authentication_code"];
                $entry_plate_number = $request[$i]["entry_plate_number"];
                
                $exit_plate_number = "";            
                $wallet_points = $request[$i]["wallet_points"];  
                $operation_type= $request[$i]["operation_type"]; 
                                                               
                $query="select id from valet_parking where ticket_number='".$ticket_id."'";  
                $result = mysqli_query($con, $query) or die(mysqli_error($con));               
                if ($data = mysqli_fetch_assoc($result)) 
                    {
                    $query="update valet_parking set paid_status=1,payment_date_time='".$payment_date_time."' where ticket_number='".$ticket_id."'";			          
                    $result = mysqli_query($con, $query) or die(mysqli_error($con));                     
                    if(mysqli_affected_rows($con)>0)    
                        {
                            if($validator_id!="")
                            {                    
                            $offline_validation_id="";
                            $ss_id=explode(',',$validator_id);
                            $ss_type=explode(',',$validation_type);
                            $ss_value=explode(',',$validation_value);
                            
                            
                            $hour=0;
                            $percentage=0;
                            for ($j = 0; $j < count($ss_id); $j++) 
                                {
                                $offline_validator_id=$ss_id[$j];
                                $offline_validation_type=$ss_type[$j];
                                $offline_validation_value=$ss_value[$j];                        
        
                                if ($offline_validation_type == 1)
                                    {
                                    $offline_validation_type = "Time Value";
                                    $hour=$hour+$offline_validation_value;
                                    }
                                if ($offline_validation_type == 2)
                                    {
                                    $offline_validation_type = "Percentage Value";                                        
                                    $percentage=$percentage+$offline_validation_value;
                                    }                                
                                $query = "insert into parking_validation(product_name,carpark_number,carpark_name,facility_number,ticket_id,plate_number,validation_value,validation_type,validator_id,date_time)VALUES('Offline'," .$carpark_number .",'" .$carpark_name ."'," .$facility_number .",'" .$ticket_id ."','".$entry_plate_number."'," .$offline_validation_value .",'" .$offline_validation_type ."','" .$offline_validator_id ."',CURRENT_TIMESTAMP)";                        
                                $result = mysqli_query($con, $query) or die(mysqli_error($con));  
                                $query="select id from parking_validation order by id desc";
                                $result = mysqli_query($con, $query) or die(mysqli_error($con));               
                                if ($data = mysqli_fetch_assoc($result)) 
                                    {
                                    $v_id=$data["id"];                                         
                                    $offline_validation_id=$v_id.",".$offline_validation_id;
                                    }                                
                                }
                            
                            //online exist
                            if(count($ss_value)>count($ss_id))
                                {
                                $offline_validation_value=$ss_value[$j];
                                $hour=$hour+$offline_validation_value;
        
                                if(count($ss_value)>$j+1)
                                    {
                                    $offline_validation_value=$ss_value[$j+1];    
                                    $percentage=$percentage+$offline_validation_value;
                                    }
                                }
        
                                                                        
                            
                            if($validation_id!="")
                                $validation_id=$offline_validation_id.$validation_id;
                            else
                                $validation_id=substr($offline_validation_id,0, -1);     
                            
                            $validation_value=$hour.",".$percentage;
                            }

                        $query ="insert into revenue_payments(device_number,device_name,carpark_number,carpark_name,facility_number,operator_id,operator_name,shift_id,parking_rate_label,parking_rate_name,entry_grace_period,exit_grace_period,vat_type,vat_percentage,ticket_id,entry_date_time,payment_date_time,max_exit_date_time,parking_duration,payment_category,parking_fee,vat_amount,lost_fee,admin_fixed_charges,ticket_replacement_fee,discount_amount,gross_amount,amount_received,discount_id,discount_category,discount_name,coupon_id,coupon_category,coupon_source,payment_type,validation_value,validation_id,receipt_number,bank_notes,balance_returned,credit_note,authentication_code,entry_plate_number,exit_plate_number,wallet_points)values('".$device_number."','".$device_name."'," .$carpark_number. ",'" . $carpark_name . "','" . $facility_number . "'," . $operator_id . ",'" . $operator_name . "','" . $shift_id . "','" . $parking_rate_label . "','" . $parking_rate_name . "','" . $entry_grace_period . "','" . $exit_grace_period . "','" . $vat_type . "','" . $vat_percentage . "','" . $ticket_id . "'," .$entry_date_time . ",'" .$payment_date_time . "'," .$max_exit_date_time . ",'" .$parking_duration . "','" .$payment_category . "','" .$parking_fee . "','" .$vat_amount . "','" .$lost_fee . "','" . $admin_fixed_charges. "','" . $ticket_replacement_fee . "','" . $discount_amount . "','" .$gross_amount . "','" . $amount_received . "','" .$discount_id . "','" .$discount_category ."','" .$discount_name . "','" .$coupon_id ."','" .$coupon_category. "','" .$coupon_source. "','" .$payment_type ."','" .$validation_value. "','" .$validation_id. "','" .$receipt_number. "','" .$bank_notes . "','" .$balance_returned . "','" .$credit_note . "','" .$authentication_code . "','" .$entry_plate_number . "','" .$exit_plate_number . "'," .$wallet_points .")";
                        $result = mysqli_query($con, $query) or die(mysqli_error($con)); 
                        }
                                        
                    $jsonresponse[$k++] = strval($id);                      

                    }
                
            }    
        mysqli_close($con); 
        $response=json_encode($jsonresponse);
        $general->write_logs($filename,"Paymenr","Json output: ".$response."\n");
        echo $response;	
        }
    else if($task==13)
        {
        $con=mysqli_connect("localhost","parcxservice","1fromParcx!19514","parcx_reporting");            
        for($i=0;$i<sizeof($request);$i++)
            { 
            $device_number = $request[$i]["device_number"];
            $device_name = $request[$i]["device_name"];
            $carpark_number = $request[$i]["carpark_number"];
            $carpark_name = $request[$i]["carpark_name"];
            $facility_number = $request[$i]["facility_number"];
    
            $id = $request[$i]["id"];
            $operator_id = $request[$i]["operator_id"];
            $operator_name = $request[$i]["operator_name"];
            $shift_id = $request[$i]["shift_id"];
            $login_time = $request[$i]["login_time"];
            $logout_time = $request[$i]["logout_time"];
            $shift_duration = $request[$i]["shift_duration"];
            $parking_fees = $request[$i]["parking_fees"];
            $vat_amount = $request[$i]["vat_amount"];
            $lost_fees = $request[$i]["lost_fees"];
            $admin_fixed_charges = $request[$i]["admin_fixed_charges"];
            $ticket_replacement_fee = $request[$i]["ticket_replacement_fee"];
            $discount_amount = $request[$i]["discount_amount"];
            $shift_earnings = $request[$i]["shift_earnings"];
            $payin_amount = $request[$i]["payin_amount"];
            $payout_amount = $request[$i]["payout_amount"];
            $shift_opening_amount = $request[$i]["shift_opening_amount"];
            $shift_closing_amount = $request[$i]["shift_closing_amount"];
            $shift_status = $request[$i]["shift_status"];
            $physical_cash_collected = $request[$i]["physical_cash_collected"];
            $wallet_points = $request[$i]["wallet_points"];
    
            $query = "select * from revenue_shifts where shift_id='" . $shift_id . "'";            
            $result = mysqli_query($con, $query) or die(mysqli_error($con));                       
            if(mysqli_num_rows($result)==0)
                {               
                //No shift	,create shift
                $query = "INSERT INTO revenue_shifts(device_number,device_name,carpark_number,carpark_name,facility_number,shift_id,operator_id,operator_name,login_time,payin_amount,shift_opening_amount,last_updated_date_time)VALUES(" .$device_number . ",'" . $device_name . "'," . $carpark_number . ",'" .$carpark_name . "'," .$facility_number. ",'" .$shift_id . "'," .$operator_id .",'" .$operator_name . "','" .$login_time ."'," .$payin_amount ."," .$shift_opening_amount . ",CURRENT_TIMESTAMP)";
                $result = mysqli_query($con, $query) or die(mysqli_error($con));       
                }

            if ((int)$parking_fees > 0 || (int)$shift_earnings > 0) {
                $query = "UPDATE revenue_shifts set wallet_points=" .$wallet_points . ",parking_fees=".$parking_fees .",vat_amount=".$vat_amount .",lost_fee=".$lost_fees .",admin_fixed_charges=".$admin_fixed_charges .",ticket_replacement_fee=".$ticket_replacement_fee .",discount_amount=".$discount_amount .",shift_earnings=".$shift_earnings .",last_updated_date_time=CURRENT_TIMESTAMP where shift_id='".$shift_id ."'";
                $result = mysqli_query($con, $query) or die(mysqli_error($con));       

            }
            if ((int)$shift_status == 1) {
                $query = "Update revenue_shifts set shift_status=1,payout_amount=".$payout_amount .",shift_closing_amount=".$shift_closing_amount .",logout_time='".$logout_time ."',shift_duration='".$shift_duration ."',physical_cash_collected=".$physical_cash_collected .",last_updated_date_time=CURRENT_TIMESTAMP where shift_id='".$shift_id ."'";
                $result = mysqli_query($con, $query) or die(mysqli_error($con));

            }

            
            $jsonresponse[$i] = strval($id);    
            }  
        mysqli_close($con); 
        $response=json_encode($jsonresponse);
        $general->write_logs($filename,"shift","Json output: ".$response."\n");
        echo $response;	
        }
    }
?>