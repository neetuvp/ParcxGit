<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of ParkingOperations
 *
 * @author User
 */
class ParkingOperations {
 
    
    function db_connect()
		{
		$con=mysqli_connect(DB_HOST,DB_USER,DB_PASSWORD,DB_NAME_REPORTING);
		if(!$con) 
			$this->error("Connection attempt failed");	
		else
		return $con;
		} // end 
		
	function db_connect2()
		{
		$con=mysqli_connect(DB_HOST2,DB_USER,DB_PASSWORD,DB_NAME_REPORTING);
		if(!$con) 
			$this->error("Connection attempt failed");	
		else
		return $con;
		} // end 

    function db_connect_config(){	
    $con=mysqli_connect(DB_HOST,DB_USER,DB_PASSWORD,DB_NAME);
    if(!$con) 
       $this->error("Connection attempt failed");	
    else
    return $con;

    } // end 
    
    
   
    
    function ListSystemDevices()    
    {
    $html_data="";
    $con=$this->db_connect_config();
    $query_string="select device_number,device_name,device_ip,device_category,carpark_number from system_devices where
    device_category in (6)";
    $result=mysqli_query($con,$query_string) or die(mysqli_error($con));     
    while($data=mysqli_fetch_assoc($result))
		{ 
		$html_data.="<div class='table-view'>";
		$html_data.="<div class='card-text'>";
		$html_data.="<div class='d-flex justify-content-between align-items-center'>";
		$html_data.="<div class='col'>" .$data["device_name"]. "</div>";
		$html_data.="<div class='col'>" .$data["device_ip"]. "</div>";
		$html_data.="<input type='hidden' id='device_details_".$data["device_number"]."' device_ip='".$data["device_ip"]."' device_name='".$data["device_name"]."' carpark_number='".$data["carpark_number"]."'>";											
		$html_data.="<div class='col'></div>";
		$html_data.="<div class='col'></div>";
		$html_data.="<div class='col'></div>";
		$html_data.="<div class='col'></div>";
		$html_data.="<div class='col'></div>";	
		$html_data.="<div class='col'><input class='btn btn-outline-info btn-open-barrier btn-block' type='submit'  value='Open Barrier' id='" .$data["device_number"]."'></div>";
		$html_data.="<div class='col'><input class='btn btn-outline-info btn-close-barrier btn-block' type='submit'  value='Close Barrier' id='" .$data["device_number"]."'></div>";											
			
		//$html_data.="<div class='col'><input class='btn btn-outline-info btn-lock-barrier btn-block' type='submit'  value='Lock Barrier' id='".$data["device_number"]."'></div>";		
		//$html_data.="<div class='col'><input class='btn btn-outline-info btn-unlock-barrier btn-block' type='submit'  value='Unlock Barrier' id='".$data["device_number"]."'></div>";		
		//$html_data.="<div class='col'><input class='btn btn-outline-info btn-enable-lane btn-block' type='submit'  value='Enable Lane' id='".$data["device_number"]."'></div>";		
		//$html_data.="<div class='col'><input class='btn btn-outline-info btn-disable-lane btn-block' type='submit'  value='Disable Lane' id='" .$data["device_number"]."'></div>";
		//$html_data.="<div class='col'><input class='btn btn-outline-info btn-reset-power btn-block' type='submit'  value='Reset Power' id='" .$data["device_number"]."'></div>";						
		$html_data.="</div>";
		$html_data.="</div>";
		$html_data.="</div>";							
		} 
	mysqli_close($con);			
	echo $html_data;
	}// End function list system devices 
	
	function ListAPMDevices()    
    {
    $html_data="";
    $con=$this->db_connect_config();
    $query_string="select device_number,device_name,device_ip,device_category,carpark_number from system_devices where
    device_category in (4)";
    $result=mysqli_query($con,$query_string) or die(mysqli_error($con));     
    while($data=mysqli_fetch_assoc($result))
		{ 
		$html_data.="<div class='table-view'>";
		$html_data.="<div class='card-text'>";
		$html_data.="<div class='d-flex justify-content-between align-items-center'>";
		$html_data.="<div class='col'>" .$data["device_name"]. "</div>";
		$html_data.="<div class='col'>" .$data["device_ip"]. "</div>";
		$html_data.="<input type='hidden' id='device_details_".$data["device_number"]."' device_ip='".$data["device_ip"]."' device_name='".$data["device_name"]."' carpark_number='".$data["carpark_number"]."'>";													
		$html_data.="<div class='col'></div>";
		$html_data.="<div class='col'></div>";
		$html_data.="<div class='col'><input class='btn btn-outline-info btn-issue-lost btn-block' type='submit'  value='Issue Lost' id='" .$data["device_number"]."'></div>";		
		$html_data.="</div>";
		$html_data.="</div>";
		$html_data.="</div>";							
		} 
	mysqli_close($con);			
	echo $html_data;
	}// End function list system devices 


function InsertManualMovements($description,$movement_type,$reason,$device_number,$device_name,$carpark_number,$operator)
{
    $con=$this->db_connect();
    $query_string="INSERT into
    parking_movements_manual(device_number,device_name,carpark_number,operator_name,action,reason,description,date_time,movement_type)
    VALUES('$device_number','$device_name','$carpark_number','$operator','$description','$reason','$description',CURRENT_TIMESTAMP,'$movement_type')";
    $result=mysqli_query($con,$query_string) or die(mysqli_error($con)); 
    mysqli_close($con);
	
	$con=$this->db_connect2();    
    $result=mysqli_query($con,$query_string) or die(mysqli_error($con)); 
    mysqli_close($con);
    echo $result;
} // End 

    
}// End Class 
