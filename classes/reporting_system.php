<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

 
class reporting_system{
    
public $html_data="";
 
function db_connect(){	
    
$con=mysqli_connect(DB_HOST,DB_USER,DB_PASSWORD,DB_NAME_REPORTING);

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

function get_devices()
{
$con       = $this->db_connect_config();
$html_data = "";
if ($con) 
    {
    $query_string = "select device_name,device_number from system_devices";
    $result = mysqli_query($con, $query_string) or die(mysqli_error($con));           
    
    while ($data = mysqli_fetch_assoc($result)) 
        {
        $html_data .= "<option  value=" . $data['device_number'] . ">" . $data['device_name'] . "</option>";			
        }
    
    echo $html_data;
    } // end if 
else 
    {
    echo "<option>Not Available</option>";
    }	
mysqli_close($con);

} // End Function 

function watchdog_network($FromDateTime,$ToDateTime) {

    $con=$this->db_connect(); 
    //$query_string="select * from watchdog_network_logs";
    $query_string="select * from watchdog_network_logs where connection_failure between '".$FromDateTime."' AND '".$ToDateTime."'";
    $query_string.="ORDER BY connection_failure ASC";
    
    $result=mysqli_query($con,$query_string) or die(mysqli_error($con));     

    if(mysqli_num_rows($result) > 0) {

        $html_data = "";

        $header .= '<table width="100%" class="jspdf-table">';
        $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';

        $header .= "<th class='col'>Device Name</th>";
        $header .= "<th class='col'>IP Address</th>";
        $header .= "<th class='col'>Connection Lost</th>";
        $header .= "<th class='col'>Connection Restored</th>";
        $header .= "<th class='col'>Carpark</th>";

        $header .= '</tr>';

        $html_data = $header;
        $html_data .= '<tbody class="table-view">';

        while($data=mysqli_fetch_assoc($result)) {

            $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';

            $html_data .= "<td class='col'>" . $data['device_name'] . " </td>";
            $html_data .= "<td class='col'>" . $data['device_ip'] . " </td>";
            $html_data .= "<td class='col'>" . $data['connection_failure'] . " </td>";
            $html_data .= "<td class='col'>" . $data['connection_restored'] . " </td>";
            $html_data .= "<td class='col'>" . $data['carpark_name'] . " </td>";

            $html_data .= '</tr>';

        }

        $html_data .= '</tbody>';
        $html_data .= '</table>';
        
    } else {
        $html_data="<div class='p-3'>No records/transactions available for the current search criteria</div>";
    }

    mysqli_close($con);    
    echo $html_data;  

}// End . Shift Report 
   
function watchdog_device_alarms($FromDateTime,$ToDateTime,$device,$severity)
{

$con=$this->db_connect(); 
//$query_string="select * from watchdog_network_logs";
$query_string="select * from watchdog_device_alarms where alarm_date_time between '".$FromDateTime."' AND '".$ToDateTime."'";
if($device!="")
	$query_string.=" AND device_number in(".$device.") ";
if($severity!="0")
	$query_string.=" AND alarm_severity ".$severity." ";
$query_string.="ORDER BY alarm_date_time DESC";
 
$result=mysqli_query($con,$query_string) or die(mysqli_error($con));   

if(mysqli_num_rows($result) > 0) {

    $html_data = "";

    $header .= '<table width="100%" class="jspdf-table">';
    $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';

    $header .= "<th class='col'>Device Name</th>";
	$header .= "<th class='col'>Alarm Severity</th>";
    $header .= "<th class='col'>Alarm Date and Time</th>";
    $header .= "<th class='col'>Alarm Description</th>";
    //$header .= "<th class='col'>Carpark</th>";

    $header .= '</tr>';

    $html_data = $header;
    $html_data .= '<tbody class="table-view">';

    while($data=mysqli_fetch_assoc($result)) 
		{
        $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
		$severity="";
		if($data["alarm_severity"]>=1 && $data["alarm_severity"]<=3)
			$severity="<span class='badge badge-danger'>High</span>";
		if($data["alarm_severity"]>=4 && $data["alarm_severity"]<=7)
			$severity="<span class='badge badge-warning'>Medium</span>";
		if($data["alarm_severity"]>=8 && $data["alarm_severity"]<=10)
			$severity="<span class='badge badge-secondary'>Low</span>";
        $html_data .= "<td class='col'>" . $data['device_name'] . " </td>";
		$html_data .= "<td class='col'>" . $severity . " </td>";
        $html_data .= "<td class='col'>" . $data['alarm_date_time'] . " </td>";
        $html_data .= "<td class='col'>" . $data['alarm_description'] . " </td>";
        //$html_data .= "<td class='col'>" . $data['carpark_name'] . " </td>";

        $html_data .= '</tr>';
    }


    $html_data .= '</table>';
     
}
else
{
    $html_data="<div class='p-3'>No records/transactions available for the current search criteria</div>";
}
mysqli_close($con);    
echo $html_data;   

   }// End . alarms
   
function TicketBlacklist($FromDateTime,$ToDateTime)
    {

    $con=$this->db_connect();

    $query_string="select * from parking_blacklist where date_time between '".$FromDateTime."' AND'".$ToDateTime."'";
    $query_string.="ORDER BY date_time ASC";
    //echo $query_string;

    $result=mysqli_query($con,$query_string) or die(mysqli_error($con));
    if(mysqli_num_rows($result) > 0) 
        {
        $html_data="";
        $header .= '<table width="100%" class="jspdf-table">';
        $header.='<tr class="card-header d-flex justify-content-between align-items-center">';
        $header.="<th class='col'>TicketNumber</th>";
        $header.="<th class='col'>Device Number</th>";
        $header.="<th class='col'>Reason</th>";
        $header.="<th class='col'>Date Time</th>";
        $header.="</tr>";

        $html_data=$header;
        $html_data .= '<tbody class="table-view">';
        while($data=mysqli_fetch_assoc($result))
            {
            $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
            $html_data.="<td class='col'> ".$data['ticket_id']." </td>";
            $html_data.="<td class='col'>".$data['device_number']."</td>";
            $html_data.="<td class='col'>".$data['blacklisting_reason']."</td>";
            $html_data.="<td class='col'>".$data['date_time']."</td>";
            $html_data.="</tr>";
            }
        $html_data.="</table>";
        }
    else
    {
    $html_data="<div class='p-3'>No records/transactions available for the current search criteria</div>";
    }
mysqli_close($con);

echo $html_data;


}// End . alarms

    


   

    
} // end class