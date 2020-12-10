<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

class reporting_parking
{
    
    public $html_data = "";
    

    function db_connect()
    {
        $con = mysqli_connect(DB_HOST, DB_USER, DB_PASSWORD, DB_NAME_REPORTING);
        
        if (!$con)
            $this->error("Connection attempt failed");
        else
            return $con;
        
    } // end 
    

    function db_connect_config()
    {
        
        $con = mysqli_connect(DB_HOST, DB_USER, DB_PASSWORD, DB_NAME);
        
        if (!$con)
            $this->error("Connection attempt failed");
        else
            return $con;
        
    } // end 
    

    function get_carparks()
    {
        $con       = $this->db_connect_config();
        $html_data = "";
        if ($con) {
            $query_string = "select carpark_name,carpark_number from system_carparks";
            $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
            // $html_data.="<option  value='0'>All Carparks</option>"; 
            
            while ($data = mysqli_fetch_assoc($result)) {
                $html_data .= "<option  value=" . $data['carpark_number'] . ">" . $data['carpark_name'] . "</option>";
                
            }
            
            echo $html_data;
        } // end if 
        else {
            echo "<option>Not Available</option>";
        }
        
        mysqli_close($con);
        
    } // End Function 

    function get_devices()
    {
        $con       = $this->db_connect_config();
        $html_data = "";
        if ($con) {
            $query_string = "select device_name,device_number from system_devices where device_category in(1,2)";
            $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
            // $html_data.="<option  value='0'>All Carparks</option>"; 
            
            while ($data = mysqli_fetch_assoc($result)) {
                $html_data .= "<option  value=" . $data['device_number'] . ">" . $data['device_name'] . "</option>";
                
            }
            
            echo $html_data;
        } // end if 
        else {
            echo "<option>Not Available</option>";
        }
        
        mysqli_close($con);
        
    } // End Function 

function get_validators()
    {
    $con = $this->db_connect_config();
    $html_data = "";
    if ($con) 
        {
    $query_string = "select validator_name,validator_id from system_validators";
    $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
    // $html_data.="<option value='0'>All Carparks</option>";

    while ($data = mysqli_fetch_assoc($result))
        {
        $html_data .= "<option value=" . $data['validator_id'] . ">" . $data['validator_name'] . "</option>";
        }
    echo $html_data;
        } // end if
    else
    {
    echo "<option>Not Available</option>";
    }
    mysqli_close($con);
    } // End Function

function get_validator_name($id)
    {
    $con = $this->db_connect_config();
    if ($con)
        {
        $query_string = "select validator_name from system_validators where validator_id='$id'";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        if ($data = mysqli_fetch_assoc($result))
            {
            return $data['validator_name'];
            }
        } // end if
    else
    {
    echo "Unknown";
    }
    mysqli_close($con);
    } // End Function
	
function get_device_name($id)
    {
    $con = $this->db_connect_config();
    if ($con)
        {
        $query_string = "select device_name from system_devices where device_number='$id'";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        if ($data = mysqli_fetch_assoc($result))
            {
            return $data['device_name'];
            }
        } // end if
    else
    {
    echo "Unknown";
    }
    mysqli_close($con);
    } // End Function

function get_customer_name($tagserial)
    {
    $con = $this->db_connect_config();
    if ($con)
        {
        $query_string = "select customer_name from access_whitelist where tag_serial='$tagserial'";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        if ($data = mysqli_fetch_assoc($result))
            {
            return $data['customer_name'];
            }
        } // end if
    mysqli_close($con);
    } // End Function

    

    function get_carpark_count()
    {
        $con = $this->db_connect_config();
        if ($con) {
            $query_string = "select carpark_name,carpark_number from system_carparks";
            $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
            $n = mysqli_num_rows($result);
            mysqli_close($con);
            return $n;
        }
        
    }
    
    
    function get_manual_movements_report($FromDateTime, $ToDateTime, $carpark, $operation)
    {
        $con          = $this->db_connect();
        $query_string = "select * from parking_movements_manual where date_time between '" . $FromDateTime . "' AND '" . $ToDateTime . "'";
        
        if (count($carpark) > 0) {
            //$query_string.="and carpark_number=".$carpark;
            $query_string .= " AND carpark_number IN('";
            
            for ($i = 0; $i < count($carpark); $i++) {
                $query_string .= "$carpark[$i]";
                
                if ($i < (count($carpark) - 1)) {
                    $query_string .= "','";
                }
            }
            $query_string .= "')";
        }
        
        if (strlen($operation) > 1) {
            $query_string .= " AND action LIKE '%" . $operation . "%'";
        }
        
        $query_string .= " ORDER BY carpark_number ASC";
        //echo $query_string;
        
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        if (mysqli_num_rows($result)) {
            $html_data = "";
            
            $header .= '<table width="100%" class="jspdf-table">';
            $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';
            
            $header .= "<th class='col'>Device Name</th>";
            $header .= "<th class='col'>Date and Time</th>";
            $header .= "<th class='col'>Operator Name</th>";
            $header .= "<th class='col'>Action</th>";
            $header .= "<th class='col'>Reason</th>";
            $header .= "<th class='col'>Description</th>";
            
            $header .= '</tr>';
            
            $html_data = $header;
            $html_data .= '<tbody class="table-view">';
            
            while ($data = mysqli_fetch_assoc($result)) {
                
                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
                
                $html_data .= "<td class='col'>" . $data['device_name'] . " </td>";
                $html_data .= "<td class='col'>" . $data['date_time'] . "</td>";
                $html_data .= "<td class='col'>" . $data['operator_name'] . "</td>";
                $html_data .= "<td class='col'>" . $data['action'] . "</td>";
                $html_data .= "<td class='col'>" . $data['reason'] . "</td>";
                $html_data .= "<td class='col'>" . $data['description'] . "</td>";
                
                $html_data .= '</tr>';
            }
            
            $html_data .= '</table>';

        } else {
            $html_data = "<div class='p-3'>No records/transactions available for the current search criteria</div>";
        }
        
        mysqli_close($con);
        echo $html_data;
        
        
    } // End . Manual Movements
    

    //Validation report
function validation_report($FromDateTime, $ToDateTime, $validationId, $ticketId)
    {
    $con = $this->db_connect();
    $query_string = "select * from parking_validation where datetime between '" . $FromDateTime . "' AND '" .
    $ToDateTime . "'";

    if ($validationId > 0) 
        $query_string.=" and validation_provider=".$validationId;
    if($ticketId!="")
        $query_string.=" and ticket_id=".$ticketId;


    $query_string .= " ORDER BY datetime ASC";
   // echo $query_string;

    $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
    if (mysqli_num_rows($result)) {
    $html_data = "";

    $header .= '<table width="100%" class="jspdf-table">';
        $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';

            $header .= "<th class='col'>Ticket Id</th>";
            $header .= "<th class='col'>Validation Value</th>";
            $header .= "<th class='col'>Validation type</th>";
            $header .= "<th class='col'>Validation Provider</th>";
            $header .= "<th class='col'>Date time</th>";            

            $header .= '</tr>';

        $html_data = $header;
        $html_data .= '<tbody class="table-view">';

            while ($data = mysqli_fetch_assoc($result)) {

            $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
            $validationType="UnKnown";
            if($data['validation_type']==1)
                $validationType="Hour";    
            if($data['validation_type']==2)
                $validationType="Percentage";
            if($data['validation_type']==3)
                $validationType="Money";

                $html_data .= "<td class='col'>" . $data['ticket_id'] . " </td>";
                $html_data .= "<td class='col'>" . $data['validation_value'] . "</td>";
                $html_data .= "<td class='col'>" . $validationType . "</td>";
                $html_data .= "<td class='col'>" . $this->get_validator_name($data['validation_provider']) . "</td>";
                $html_data .= "<td class='col'>" . $data['datetime'] . "</td>";                

                $html_data .= '</tr>';
            }

            $html_data .= '</table>';

    } else {
    $html_data = "<div class='p-3'>No records/transactions available for the current search criteria</div>";
    }

    mysqli_close($con);
    echo $html_data;


    } // End . Validation
    
//Blacklist report
function blacklist_report($FromDateTime, $ToDateTime, $carpark)
    {
    $con = $this->db_connect();
    $query_string = "select * from parking_blacklist where date_time between '" . $FromDateTime . "' AND '" .
    $ToDateTime . "'";

   if (count($carpark) > 0) {
            //$query_string.="and carpark_number=".$carpark;
            $query_string .= " AND carpark_number IN('";
            
            for ($i = 0; $i < count($carpark); $i++) {
                $query_string .= "$carpark[$i]";
                
                if ($i < (count($carpark) - 1)) {
                    $query_string .= "','";
                }
            }
            $query_string .= "')";
        }


    $query_string .= " ORDER BY date_time DESC";
   //echo $query_string;

    $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
    if (mysqli_num_rows($result)) {
    $html_data = "";

    $header .= '<table width="100%" class="jspdf-table">';
    $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';
    
    $header .= "<th class='col'>Ticket Id</th>";
    $header .= "<th class='col'>Device name</th>";  
    $header .= "<th class='col'>Date time</th>";  
    $header .= "<th class='col'>Blacklisting Reason</th>";
    $header .= "<th class='col'>Desription</th>";                        

    $header .= '</tr>';

    $html_data = $header;
        $html_data .= '<tbody class="table-view">';

            while ($data = mysqli_fetch_assoc($result)) {

            $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';          
                $html_data .= "<td class='col'>" . $data['ticket_id'] . " </td>";
                $html_data .= "<td class='col'>" . $data['device_name'] . "</td>";
                $html_data .= "<td class='col'>" . $data['date_time'] . "</td>";
                $html_data .= "<td class='col'>" . $data['blacklisting_reason'] . "</td>";
                $html_data .= "<td class='col'>" . $data['blacklisting_description'] . "</td>";                

                $html_data .= '</tr>';
            }

            $html_data .= '</table>';

    } else {
    $html_data = "<div class='p-3'>No records/transactions available for the current search criteria</div>";
    }

    mysqli_close($con);
    echo $html_data;


    } // End . Manual Movements

function get_access_movements($FromDateTime, $ToDateTime, $carpark, $serial,$acces_option)
    {
        $con          = $this->db_connect();
        $query_string = "select * from parking_movements_access where date_time between '" . $FromDateTime . "' AND '" . $ToDateTime . "' ";
        
        if (count($carpark) > 0) {
            //$query_string.="and carpark_number=".$carpark;
            $query_string .= " AND carpark_number IN('";
            
            for ($i = 0; $i < count($carpark); $i++) {
                $query_string .= "$carpark[$i]";
                
                if ($i < (count($carpark) - 1)) {
                    $query_string .= "','";
                }
            }
            $query_string .= "')";
        }
        if ($acces_option !="0") {
            $query_string .= " AND status_message LIKE '".$acces_option."%'";
        }

        if ($serial > 0) {
            $query_string .= " AND tag_serial LIKE '%" . $serial . "'";
        }
        $query_string .=" order by date_time desc";
        //echo  $query_string;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        
        if (mysqli_num_rows($result) > 0) {

            $header .= '<div class="card-body p-0">';

            $header .= '<table width="100%" class="jspdf-table">';
            $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';

            $header .= "<th class='col'>Date and Time</th>";	   
            $header .= "<th class='col'>Serial No.</th>";
            $header .= "<th class='col'>EPC</th>";
            $header .= "<th class='col'>Customer name</th>";
            $header .= "<th class='col'>Device Name</th>";
            $header .= "<th class='col'>Carpark Name</th>";
            $header .= "<th class='col-2'>Action</th>";
	    $header .= "<th class='col'></th>";

            $header .= '</tr>';

            //
            
            $html_data .= $header;
            
            $html_data .= '<tbody class="table-view">';
            
            while ($data = mysqli_fetch_assoc($result)) {
                
                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
                
                $html_data .= "<td class='col'> " . $data['date_time'] . " </td>";		
                $html_data .= "<td class='col'> " . $data['tag_serial'] . " </td>";	
                if(strlen($data['epc'])!=0)	
                    $html_data .= "<td class='col'> " . substr($data['epc'],-11) . " </td>";
                else
                    $html_data .= "<td class='col'> " . rtrim(substr($data['epc_list'],-12),",") . " </td>";

		        $html_data .= "<td class='col'>" . $this->get_customer_name($data['tag_serial']) . " </td>";				                
                $html_data .= "<td class='col'> " . $data['device_name'] . " </td>";
                $html_data .= "<td class='col'> " . $data['carpark_name'] . " </td>";
                $html_data .= "<td class='col-2'> " . $data['status_message'] . " </td>";
		if(strlen($data['tag_serial'])==0 &&strlen($data['tag'])>2 && strlen($data['epc'])>2 &&
		$data['status_message']=="TAG UNKNOWN" && $data['whitelist_added']==0 && $_SESSION["userRollName"]=="Admin")
                {
$html_data .= "<td class='col'><input data-toggle='modal' data-target='#addToWhiteList' id='btnAddToWhiteList".$data['id']."' type='botton' class='btn-link btnWhitelistAdd' value='Add to whitelist' data-id=".$data['id']."></td>";
$html_data.="<input type='hidden' id='tag-".$data['id']."' value='".$data['tag']."' >";
$html_data.="<input type='hidden' id='tid-".$data['id']."' value='".$data['tid']."' >";
$html_data.="<input type='hidden' id='carpark-".$data['id']."' value='".$data['carpark_number']."'>";
}
		else
                	$html_data .= "<td class='col'></td>";		
                $html_data .= '</tr>';
            }
            
            $html_data .= '</table>';

            $html_data .= '</div>';
            
            mysqli_close($con);
            
            echo $html_data;
        } //
        else {
            echo "<div class='p-3'>No records/transactions available for the current search criteria</div>";
        }
        
    } // End . Access Movements
    
    
    function get_access_movements2($FromDateTime, $ToDateTime, $carpark, $serial)
    {
        $con          = $this->db_connect();
        $query_string = "select * from parking_movements_access where status_message!='TAG UNKNOWN' and date_time between '" . $FromDateTime . "' AND '" . $ToDateTime . "'";
        
        if (count($carpark) > 0) {
            //$query_string.="and carpark_number=".$carpark;
            $query_string .= " AND carpark_number IN('";
            
            for ($i = 0; $i < count($carpark); $i++) {
                $query_string .= "$carpark[$i]";
                
                if ($i < (count($carpark) - 1)) {
                    $query_string .= "','";
                }
            }
            $query_string .= "')";
        }
        if ($serial > 0) {
            $query_string .= " AND tag_serial LIKE '%" . $serial . "'";
        }
        
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        
        if (mysqli_num_rows($result) > 0) {

            $header .= '<div class="card-body p-0">';

            $header .= '<table width="100%" class="jspdf-table">';
            $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';

            $header .= "<th class='col'>Date and Time</th>";
            $header .= "<th class='col'>Tag</th>";
            $header .= "<th class='col'>Device Name</th>";
            $header .= "<th class='col'>Carpark Name</th>";
            $header .= "<th class='col'>Action</th>";
            $header .= "<th class='col'></th>";
            $header .= '</tr>';

            //
            
            $html_data .= $header;
            
            $html_data .= '<tbody class="table-view">';
            
            while ($data = mysqli_fetch_assoc($result)) {
                
                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
                
                $html_data .= "<td class='col'> " . $data['date_time'] . " </td>";
                $html_data .= "<td class='col'> " . $data['tag'] . " </td>";
                $html_data .= "<td class='col'> " . $data['device_name'] . " </td>";
                $html_data .= "<td class='col'> " . $data['carpark_name'] . " </td>";
                $html_data .= "<td class='col'> " . $data['status_message'] . " </td>";
                $html_data .= "<td class='col'>  </td>";
                $html_data .= '</tr>';
            }
            
            $html_data .= '</table>';

            $html_data .= '</div>';
            
            mysqli_close($con);
            
            echo $html_data;
        } //
        else {
            echo "<div class='p-3'>No records/transactions available for the current search criteria</div>";
        }
        
    } // End . Access Movements
    
    
    function visitor_frequency_report_old($FromDateTime, $ToDateTime, $carpark, $serial, $weekdays)
    {
        $con = $this->db_connect();
        
        $query_string = "select max(date_time) as datetime,carpark_name,tag_serial,count(*) as total from parking_movements_access where date_time between '" . $FromDateTime . "' AND '" . $ToDateTime . "'";
        
        if (count($carpark) > 0) {
            //$query_string.="and carpark_number=".$carpark;
            $query_string .= " AND carpark_number IN('";
            
            for ($i = 0; $i < count($carpark); $i++) {
                $query_string .= "$carpark[$i]";
                
                if ($i < (count($carpark) - 1)) {
                    $query_string .= "','";
                }
            }
            $query_string .= "')";
        }
        if ($serial > 0) {
            $query_string .= " AND tag_serial LIKE '%" . $serial . "'";
        }
        
        if (count($weekdays) > 0) {
            $query_string .= " AND day IN('";
            
            for ($i = 0; $i < count($weekdays); $i++) {
                $query_string .= "$weekdays[$i]";
                
                if ($i < (count($weekdays) - 1)) {
                    $query_string .= "','";
                }
            }
            $query_string .= "')";
        }
        
        $query_string .= " group by tag_serial";
        
        
        // echo $query_string;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        
        if (mysqli_num_rows($result) > 0) {
            
            
            $header .= '<div class="card-body p-0">';
            
            $header .= '<table width="100%" class="jspdf-table">';
            $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';
            
            $header .= "<th class='col'>Serial No.</th>";
            $header .= "<th class='col'>Carpark Name</th>";
            $header .= "<th class='col'>Total Visits</th>";
            $header .= "<th class='col'>Last Visit</th>";
            
            $header .= '</tr>';
            
            $html_data .= $header;
            
            $html_data .= '<tbody class="table-view">';
            
            while ($data = mysqli_fetch_assoc($result)) {
                
                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
                
                $html_data .= "<td class='col'>" . $data['tag_serial'] . "</td>";
                if (count($carpark) > 0 && count($carpark) != $this->get_carpark_count())
                    $html_data .= "<td class='col'>" . $data['carpark_name'] . "</td>";
                else
                    $html_data .= "<td class='col'>Multiple Carparks</td>";

                $html_data .= "<td class='col'>" . $data['total'] . "</td>";
                $html_data .= "<td class='col'>" . $data['datetime'] . "</td>";
                
                $html_data .= '</tr>';
            }
            
            $html_data .= '</table>';
            
            $html_data .= '</div>';

            mysqli_close($con);
            
            echo $html_data;
        } //
        else {
            echo "<div class='p-3'>No records/transactions available for the current search criteria</div>";
        }
        
    } // End . visitor frequency
    
function visitor_frequency_report($FromDateTime, $ToDateTime, $carpark, $serial, $weekdays)
    {
    $con = $this->db_connect_config();

    $query_string = "select tag_serial,frequency from access_whitelist";
    if ($serial > 0) 
        {
        $query_string .= " where tag_serial LIKE '" . $serial . "%'";
        }
    $result=mysqli_query($con, $query_string) or die(mysqli_error($con));     
    if(mysqli_num_rows($result)> 0) 
        {
        $header .= '<div class="card-body p-0">';
                $header .= '<table width="100%" class="jspdf-table">';
                    $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';

                        $header .= "<th class='col'>Serial No.</th>";                        
                        $header .= "<th class='col'>Total Visits</th>";                        
                        $header .= '</tr>';

                    $html_data .= $header;

                    $html_data .= '<tbody class="table-view">';

                        while ($data = mysqli_fetch_assoc($result)) {

                        $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';

                            $html_data .= "<td class='col'>" . $data['tag_serial'] . "</td>";
                            
                            $html_data .= "<td class='col'>" . $data['frequency'] . "</td>";
                           
                            $html_data .= '</tr>';
                        }

                        $html_data .= '</table>';

                $html_data .= '</div>';

            mysqli_close($con);

            echo $html_data;
            } //
            else {
            echo "<div class='p-3'>No records/transactions available for the current search criteria</div>";
            }

        } // End . visitor frequency

    function get_parking_duration($fromdate, $todate, $carpark, $category, $weekdays)
        {
        $total_count = array();
        $i           = 0;
        $j           = 0;
        $k           = 1;
        
        $con = $this->db_connect();
        
        //Start . Generate the SQL Query String . 
        $query_string = "select sum(m0to30) as m0to30,sum(m30to60) as m30to60";
        $i            = 1;
        while ($i < 24) 
            {
            $query_string .= ",sum(h" . $i . "to" . ($i + 1) . ") as h" . $i . "to" . ($i + 1);
            $i++;
            }
        $k = 1;
        while ($k < 3) 
            {
            $query_string .= ",sum(d" . $k . "to" . ($k + 1) . ") as d" . $k . "to" . ($k + 1);
            $k++;
            }

        $query_string .= ",max(maximum_parking_duration) as max_parking_duration,min(nullif(minimum_parking_duration,0)) as min_parking_duration,avg(average_parking_duration) as avg_parking_duration" . " from parking_duration where report_date between '" . $fromdate . "' AND '" . $todate . "' and category='" . $category . "'";
        
        if (count($carpark) > 0) 
            {
            //$query_string.="and carpark_number=".$carpark;
            $query_string .= " AND carpark_number IN('";
            
            for ($i = 0; $i < count($carpark); $i++) {
                $query_string .= "$carpark[$i]";
                
                if ($i < (count($carpark) - 1)) {
                    $query_string .= "','";
                }
            }
            $query_string .= "')";
            }
        
        if (count($weekdays) > 0) {
            $query_string .= " AND dayname(report_date) IN('";
            
            for ($i = 0; $i < count($weekdays); $i++) {
                $query_string .= "$weekdays[$i]";
                
                if ($i < (count($weekdays) - 1)) {
                    $query_string .= "','";
                }
            }
            $query_string .= "')";
        }
        
       // echo $query_string;    
        
        //END . Generate the SQL Query String . 
        
        
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        
        if (mysqli_num_rows($result) > 0) {
            $data           = mysqli_fetch_assoc($result);
            $total_count[0] = (int) $data["m0to30"];
            $total_count[1] = (int) $data["m30to60"];
            $i              = 1;
            $j              = 2;
            //get count for hours
            while ($i < 24) {
                $field           = "h" . $i . "to" . ($i + 1);
                $total_count[$j] = (int) $data[$field];
                $i++;
                $j++;
            }
            
            //get count for days        
            while ($k < 5) {
                $field           = "d" . $k . "to" . ($k + 1);
                $total_count[$j] = (int) $data[$field];
                $k++;
                $j++;
            }
            $total_count[$j++] = (int) $data["max_parking_duration"];
            $total_count[$j++] = (int) $data["min_parking_duration"];
            $total_count[$j++] = (int) $data["avg_parking_duration"];
            
            mysqli_close($con);
            
            return $total_count;
            
        } else {
            $i = 0;
            while ($i < 15) {
                $total_count[$i] = 0;
                $i++;
            }
            
            return $total_count;
        }
        
        
    } // End get parking duration
    
    

    function traffic_summary_report($from,$to,$carpark,$weekdays)
		{  
		// $from=$data["from"];
		// $to=$data["to"];
		// $carpark=$data["carpark"];
		// $weekdays=$data["weekdays"];
        $entry_count           = array();
        $exit_count            = array();
        $access_entry          = array();
        $access_exit           = array();
        $manualopen_count      = array();
        $manualclose_count     = array();
        $pushbutton_open_count = array();       
        $count                 = array();
        $con                   = $this->db_connect(2);       
         
        $i=0;
        $show_date_by_report=false;
        $day_report           = array();
        $to_date=new DateTime($to);
        $period = new DatePeriod(new DateTime($from),new DateInterval('P1D'),$to_date->modify("+1 day"));
        foreach($period as $date) 
            { 
            $key= $date->format("Y-m-d");                
            $day_report[$key]["short_term_entry_count"]=0;
            $day_report[$key]["short_term_exit_count"]=0; 
            $day_report[$key]["access_entry_count"]=0;
            $day_report[$key]["access_exit_count"]=0; 
            $day_report[$key]["manual_entry_count"]=0;
            $day_report[$key]["manual_exit_count"]=0; 
            $i++;            
            }
        if($i>1)
            $show_date_by_report=true;
                            
        //get entry counts
        
        $query = "select * from summary_parking_movements where  report_date between '" . $from . "' AND '" .$to. "'";
        
        if (strlen($carpark) > 0) 		           
            $query .= " AND carpark_id IN(".$carpark.")";                       
		
		if (strlen($weekdays) > 0) 		           
            $query .= " AND report_day IN(".$weekdays.")";                       
        
        
        //echo $query;
        $result = mysqli_query($con, $query) or die(mysqli_error($con));
        $current_date          = date("Y-m-d H:i:s");
        $total_entry           = 0;
        $total_exit            = 0;
        $total_mopen           = 0;
        $total_mclose          = 0;
        $access_entry_total    = 0;
        $access_exit_total     = 0;
        $total_pushbutton_open = 0;
        
        for ($i = 0; $i < 24; $i++) 
            {
            $entry_count[$i]           = 0;
            $exit_count[$i]            = 0;
            $manualopen_count[$i]      = 0;
            $manualclose_count[$i]     = 0;
            $pushbutton_open_count[$i] = 0;
            $access_entry[$i]          = 0;
            $access_exit[$i]           = 0;
            }
        
        if (mysqli_num_rows($result) > 0) 
            {
            while ($data = mysqli_fetch_assoc($result)) 
                {
                $category = (int) $data['category'];
                $movement_type=(int) $data['movement_type']; 
                $day=$data["report_date"]; 
                
                switch ($category) 
                    {
                    case 1:
                        if($movement_type==1)
                            $day_report[$day]["short_term_entry_count"]=(int) $data["total_count"]+$day_report[$day]["short_term_entry_count"];
                        if($movement_type==2)
                            $day_report[$day]["short_term_exit_count"]=(int) $data["total_count"]+$day_report[$day]["short_term_exit_count"];
                        break;
                    case 2:
                        if($movement_type==1)
                            $day_report[$day]["access_entry_count"]=(int) $data["total_count"]+$day_report[$day]["access_entry_count"];
                        if($movement_type==2)
                            $day_report[$day]["access_exit_count"]=(int) $data["total_count"]+$day_report[$day]["access_exit_count"];
                        break;
                    case 3:
                        if($movement_type==1 ||$movement_type==3)
                            $day_report[$day]["manual_entry_count"]=(int) $data["total_count"]+$day_report[$day]["manual_entry_count"];
                            if($movement_type==2 ||$movement_type==4)
                            $day_report[$day]["manual_exit_count"]=(int) $data["total_count"]+$day_report[$day]["manual_exit_count"];
                    break;
                        }



                $i = 0;
                while ($i < 24) 
                    {
                    $field    = "h" . $i . "to" . ($i + 1);                                                  
                    switch ($category) 
                        {
                        case 1:
                            if($movement_type==1)
                                {
                                $entry_count[$i] = (int) $data[$field] + $entry_count[$i];
                                $total_entry = (int) $data[$field] + $total_entry;                                
                                }
                            if($movement_type==2)
                                {
                                $exit_count[$i] = (int) $data[$field] + $exit_count[$i];
                                $total_exit = $total_exit + (int) $data[$field];                                
                                }
                            break;
                        
                        case 2:
                            if($movement_type==1)
                                {
                                $access_entry[$i] = (int) $data[$field]+$access_entry[$i] ;
                                $access_entry_total = $access_entry_total + (int) $data[$field];                                 
                                }
                            if($movement_type==2)
                                {
                                $access_exit[$i] = (int) $data[$field]+$access_exit[$i];
                                $access_exit_total = $access_exit_total + (int) $data[$field];                                   
                                }
                            break;
                        
                        case 3:
                            if($movement_type==1 ||$movement_type==3)
                                {
                                $manualopen_count[$i] = (int) $data[$field] + $manualopen_count[$i];
                                $total_mopen = $total_mopen + (int) $data[$field];                                
                                }
                            if($movement_type==2 ||$movement_type==4)
                                {
                                $manualclose_count[$i] = (int) $data[$field]+$manualclose_count[$i];
                                $total_mclose = (int) $data[$field] + $total_mclose;                                
                                }
                            break;
                        
                       
                        
                             /* 
                            case 5:
                            $pushbutton_open_count[$i] = (int) $data[$field];
                            $total_pushbutton_open = $total_pushbutton_open + (int) $data[$field];
                            break;                                                                                                   
                            
                            case 10:
                            $count[$i]=(int)$data[$field];
                            $total=(int)$data['total_count'];;
                            break;
                            
                            
                            case 50: // exit counts during grace period 
                            $count[$i]=(int)$data[$field];
                            $total_exit_during_grace=(int)$data['total_count'];;
                            break;
                            */
                            
                        }
                        $i++;                    
                    }
                
                } //wend
            
            //  Begin . Traffic Summary 
            
            $summary = '<div class="row mb-4 jspdf-graph">';
            $summary .= '<div class="col-lg-3 col">';        
            $summary .= '<div class="small-box bg-success">';
            $summary .= ' <div class="inner">';
            $summary .= '     <h3>' . $total_entry . '/' . $access_entry_total . '</h3>';
            $summary .= '     <p>Entries<br>[Short-term/Contract]</p>';
            $summary .= '    </div>';
            $summary .= '    <div class="icon">';
            $summary .= '    <i class="ion ion-pie-graph"></i>';
            $summary .= '   </div>';        
            $summary .= ' </div>';
            $summary .= ' </div>';
            $summary .= ' <div class="col-lg-3 col">';
            $summary .= '  <div class="small-box bg-success">';
            $summary .= '    <div class="inner">';
            $summary .= '     <h3>' . $total_exit . '/' . $access_exit_total . '</h3>';
            $summary .= '    <p>Exits<br>[Short-term/Contract]</p>';
            $summary .= '  </div>';
            $summary .= '  <div class="icon">';
            $summary .= '  <i class="ion ion-pie-graph"></i>';
            $summary .= '  </div>';   
            $summary .= '</div>';
            $summary .= ' </div>';
            
            $summary .= '   <div class="col-lg-3 col">';
            $summary .= '<div class="small-box bg-success">';
            $summary .= '  <div class="inner">';
            $summary .= '     <h3>' . $total_mopen . '/' . $total_mclose . '</h3>';
            $summary .= '   <p>Manual Operation<br>[Entry/Exit]</p>';
            $summary .= ' </div>';
            $summary .= ' <div class="icon">';
            $summary .= ' <i class="ion ion-pie-graph"></i>';
            $summary .= ' </div>';        
            $summary .= '</div>';
            $summary .= '</div>';
            
            $summary .= '</div> <!--End. Row -->';

            echo $summary;
            
            // End . Traffic Summary 

            /*Date by report*/
            if($show_date_by_report==true)
                {
                $header = '<table width="100%" class="jspdf-table">';
                $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';
                
                $header .= "<th class='col'>Date</th>";            
                $header .= "<th class='col'>Entries Short-term</th>";
                $header .= "<th class='col'>Exits Short-term</th>";
                $header .= "<th class='col'>Entries Contract</th>";
                $header .= "<th class='col'>Exits Contract</th>";
                $header .= "<th class='col'>Manual Entry </th>";
                $header .= "<th class='col'>Manual Exit</th>";            
                $header .= '</tr>';
                $html_data=$header;
                $html_data .= '<tbody class="table-view">';



                $day_report=array_reverse($day_report,true);

                foreach($day_report as $key=>$value)
                    {                
                    $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';                
                    $html_data .= "<td class='col'>" . $key . " </td>";                
                    $html_data .= "<td class='col'>" . $day_report[$key]["short_term_entry_count"] . " </td>";
                    $html_data .= "<td class='col'>" . $day_report[$key]["short_term_exit_count"] . " </td>";
                    $html_data .= "<td class='col'>" . $day_report[$key]["access_entry_count"] . " </td>";
                    $html_data .= "<td class='col'>" . $day_report[$key]["access_exit_count"] . " </td>";
                    $html_data .= "<td class='col'>" . $day_report[$key]["manual_entry_count"] . " </td>";
                    $html_data .= "<td class='col'>" .$day_report[$key]["manual_exit_count"] . " </td>";
                    
                    $html_data .= '</tr>';
                    }
                $html_data .= '<tr class="card-text subhead d-flex justify-content-between align-items-center">';            
                $html_data .= "<td class='col'>Total</td>";                   
                $html_data .= "<td class='col'>" . $total_entry . "</td>";
                $html_data .= "<td class='col'>" . $total_exit . "</td>";
                $html_data .= "<td class='col'>" . $access_entry_total . "</td>";
                $html_data .= "<td class='col'>" . $access_exit_total . "</td>";
                $html_data .= "<td class='col'>" . $total_mopen . "</td>";
                $html_data .= "<td class='col'>" . $total_mclose . "</td>";                    
                $html_data .= '</tr>';

                $html_data .= '</table><br>'; 
                echo $html_data;    
                }       

            
            // Begin . Hourly Traffic Statistics a
            
            //$html_data = "";
            
            $header = '<table width="100%" class="jspdf-table">';
            $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';
            
            $header .= "<th class='col'>From</th>";
            $header .= "<th class='col'>To</th>";
            $header .= "<th class='col'>Entries Short-term</th>";
            $header .= "<th class='col'>Exits Short-term</th>";
            $header .= "<th class='col'>Entries Contract</th>";
            $header .= "<th class='col'>Exits Contract</th>";
            $header .= "<th class='col'>Manual Entry </th>";
            $header .= "<th class='col'>Manual Exit</th>";
            
            $header .= '</tr>';
            
            $html_data = $header;
            $html_data .= '<tbody class="table-view">';
            
            $i       = 0;
            $date    = DAY_CLOSURE_START;

            $dateEnd = DAY_CLOSURE_END;
            $j       = (int) substr($date, 0, 2);
            $i       = 0;

            while ($i < 24) 
                {
                // data for chart                

                $entryData[$i] = $entry_count[$i] + $access_entry[$i];
                $exitData[$i] = $exit_count[$i] + $access_exit[$i];

                $html_data .= '<span display="none" id="chartData_' . $i . '" data-entry="' . $entryData[$i] . '" data-exit="' . $exitData[$i] . '"></span>';

                // end / data for chart

                $j       = (int) substr($date, 0, 2);
                $date_to = strtotime($date) + 3600;
                $date_to = date("H:i:s", $date_to);


                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
                
                $html_data .= "<td class='col'>" . $date . " </td>";
                $html_data .= "<td class='col'>" . $date_to . " </td>";
                $html_data .= "<td class='col'>" . $entry_count[$i] . " </td>";
                $html_data .= "<td class='col'>" . $exit_count[$i] . " </td>";
                $html_data .= "<td class='col'>" . $access_entry[$i] . " </td>";
                $html_data .= "<td class='col'>" . $access_exit[$i] . " </td>";
                $html_data .= "<td class='col'>" . $manualopen_count[$i] . " </td>";
                $html_data .= "<td class='col'>" . $manualclose_count[$i] . " </td>";
                
                $html_data .= '</tr>';
                
                $date = $date_to;
                $i++;
                }
            
            $html_data .= '<tr class="card-text subhead d-flex justify-content-between align-items-center">';
            
            $html_data .= "<td class='col'>Total</td>";
            $html_data .= "<td class='col'></td>";
            $html_data .= "<td class='col'>" . $total_entry . "</td>";
            $html_data .= "<td class='col'>" . $total_exit . "</td>";
            $html_data .= "<td class='col'>" . $access_entry_total . "</td>";
            $html_data .= "<td class='col'>" . $access_exit_total . "</td>";
            $html_data .= "<td class='col'>" . $total_mopen . "</td>";
            $html_data .= "<td class='col'>" . $total_mclose . "</td>";
            
            $html_data .= '</tr>';
            
            $html_data .= '</table>';
            
            //End . Hourly Traffic Statistics 
            

            } //endif         
        else 
            {
            $html_data = "<div class='card p-3'>No records/transactions available for the current search criteria</div>";
            }        
        echo $html_data;                
        }    

    function traffic_summary_report_old($from, $to, $carpark, $weekdays)
    {
        
        $entry_count           = array();
        $exit_count            = array();
        $access_entry          = array();
        $access_exit           = array();
        $manualopen_count      = array();
        $manualclose_count     = array();
        $pushbutton_open_count = array();
        //$terminal_manualclose_count=array();       
        $count                 = array();
        $con                   = $this->db_connect();
        $i                     = 0;
        $date                  = new DateTime($checkdate);
        $date->add(new DateInterval('PT0H'));
        $date_to     = new DateTime($checkdate);
        $total_entry = 0;
        $total_exit  = 0;
        
        
        $date_to->add(new DateInterval('PT1S'));
        $date_to->add(new DateInterval('PT59M59S'));
        $current_date = date("Y-m-d H:i:s");
        
        //get entry counts
        
        $query = "select * from summary_parking_movements where  report_date between '" . substr($from, 0, 11) . "' AND '" . substr($to, 0, 11) . "'";
        
        if (count($carpark) > 0) {
            //$query_string.="and carpark_number=".$carpark;
            $query .= " AND car_park_number IN('";
            
            for ($i = 0; $i < count($carpark); $i++) {
                $query.= "$carpark[$i]";
                
                if ($i < (count($carpark) - 1)) {
                    $query.= "','";
                }
            }
            $query.= "')";
        }
        
        if (count($weekdays) > 0) {
            $query .= " AND report_day IN('";
            
            for ($i = 0; $i < count($weekdays); $i++) {
                $query .= "$weekdays[$i]";
                
                if ($i < (count($weekdays) - 1)) {
                    $query .= "','";
                }
            }
            $query .= "')";
        }
        
        
        //echo $query;
        $result = mysqli_query($con, $query) or die(mysqli_error($con));
        $current_date          = date("Y-m-d H:i:s");
        $total_entry           = 0;
        $total_exit            = 0;
        $total_mopen           = 0;
        $total_mclose          = 0;
        $access_entry_total    = 0;
        $access_exit_total     = 0;
        $total_pushbutton_open = 0;
        
        for ($i = 0; $i < 24; $i++) {
            $entry_count[$i]           = 0;
            $exit_count[$i]            = 0;
            $manualopen_count[$i]      = 0;
            $manualclose_count[$i]     = 0;
            $pushbutton_open_count[$i] = 0;
            $access_entry[$i]          = 0;
            $access_exit[$i]           = 0;
        }
        
        if (mysqli_num_rows($result) > 0) {
            while ($data = mysqli_fetch_assoc($result)) {
                
                $i = 0;
                while ($i < 24) {
                    $field    = "h" . $i . "to" . ($i + 1);
                    $category = (int) $data['category'];
                    $movement_type=(int) $data['movement_type'];                                
                    switch ($category) {
                        case 1:
                            if($movement_type==1)
                                {
                                $entry_count[$i] = (int) $data[$field] + $entry_count[$i];
                                $total_entry = (int) $data[$field] + $total_entry;
                                }
                            if($movement_type==2)
                                {
                                $exit_count[$i] = (int) $data[$field] + $exit_count[$i];
                                $total_exit = $total_exit + (int) $data[$field];
                                }
                            break;
                        
                        case 2:
                            if($movement_type==1)
                                {
                                $access_entry[$i] = (int) $data[$field]+$access_entry[$i] ;
                                $access_entry_total = $access_entry_total + (int) $data[$field];                                  
                                }
                            if($movement_type==2)
                                {
                                $access_exit[$i] = (int) $data[$field]+$access_exit[$i];
                                $access_exit_total = $access_exit_total + (int) $data[$field];                               
                                }
                            break;
                        
                        case 3:
                            if($movement_type==3)
                                {
                                $manualopen_count[$i] = (int) $data[$field] + $manualopen_count[$i];
                                $total_mopen = $total_mopen + (int) $data[$field];
                                }
                            if($movement_type==4)
                                {
                                $manualclose_count[$i] = (int) $data[$field]+$manualclose_count[$i];
                                $total_mclose = (int) $data[$field] + $total_mclose;
                                }
                            break;
                        
                       
                        
                             /* 
                            case 5:
                            $pushbutton_open_count[$i] = (int) $data[$field];
                            $total_pushbutton_open = $total_pushbutton_open + (int) $data[$field];
                            break;                                                                                                   
                            
                            case 10:
                            $count[$i]=(int)$data[$field];
                            $total=(int)$data['total_count'];;
                            break;
                            
                            
                            case 50: // exit counts during grace period 
                            $count[$i]=(int)$data[$field];
                            $total_exit_during_grace=(int)$data['total_count'];;
                            break;
                            */
                            
                    }
                    $i++;
                    
                }
            } //wend
            
            //  Begin . Traffic Summary 
            
            $summary .= '<div class="row mb-4 jspdf-graph">';
            $summary .= '<div class="col-lg-3 col-6">';
            $summary .= ' <!-- small card -->';
            $summary .= '<div class="small-box bg-info">';
            $summary .= ' <div class="inner">';
            $summary .= '     <h3>' . $total_entry . '/' . $access_entry_total . '</h3>';
            $summary .= '     <p>Entries<br>[Short-term/Contract]</p>';
            $summary .= '    </div>';
            $summary .= '    <div class="icon">';
            $summary .= '    <i class="ion ion-pie-graph"></i>';
            $summary .= '   </div>';
            $summary .= '   <a href="#" class="small-box-footer">';
            $summary .= '      <i class="fa fa-arrow-circle-right"></i>';
            $summary .= '   </a>';
            $summary .= ' </div>';
            $summary .= ' </div>';
            $summary .= ' <div class="col-lg-3 col-6">';
            $summary .= '  <!-- small card -->';
            $summary .= '  <div class="small-box bg-info">';
            $summary .= '    <div class="inner">';
            $summary .= '     <h3>' . $total_exit . '/' . $access_exit_total . '</h3>';
            $summary .= '    <p>Exits<br>[Short-term/Contract]</p>';
            $summary .= '  </div>';
            $summary .= '  <div class="icon">';
            $summary .= '  <i class="ion ion-pie-graph"></i>';
            $summary .= '  </div>';
            $summary .= '  <a href="#" class="small-box-footer">';
            $summary .= '     <i class="fa fa-arrow-circle-right"></i>';
            $summary .= ' </a>';
            $summary .= '</div>';
            $summary .= ' </div>';
            
            $summary .= '   <div class="col-lg-3 col-6">';
            $summary .= ' <!-- small card -->';
            $summary .= '<div class="small-box bg-info">';
            $summary .= '  <div class="inner">';
            $summary .= '     <h3>' . $total_mopen . '/' . $total_pushbutton_open . '</h3>';
            $summary .= '   <p>Manual Open<br>[Software/Pushbutton]</p>';
            $summary .= ' </div>';
            $summary .= ' <div class="icon">';
            $summary .= ' <i class="ion ion-pie-graph"></i>';
            $summary .= ' </div>';
            $summary .= ' <a href="#" class="small-box-footer">';
            $summary .= '  <i class="fa fa-arrow-circle-right"></i>';
            $summary .= ' </a>';
            $summary .= '</div>';
            $summary .= '</div>';
            
            $summary .= '</div> <!--End. Row -->';
            
            // End . Traffic Summary 
            
            
            $html_data = $summary;
            
            // Begin . Hourly Traffic Statistics a
            
            $html_data = "";
            
            $header .= '<table width="100%" class="jspdf-table">';
            $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';
            
            $header .= "<th class='col'>From</th>";
            $header .= "<th class='col'>To</th>";
            $header .= "<th class='col'>Entries Short-term</th>";
            $header .= "<th class='col'>Exits Short-term</th>";
            $header .= "<th class='col'>Entries Contract</th>";
            $header .= "<th class='col'>Exits Contract</th>";
            $header .= "<th class='col'>Manual Open </th>";
            $header .= "<th class='col'>Manual close</th>";
            
            $header .= '</tr>';
            
            $html_data = $header;
            $html_data .= '<tbody class="table-view">';
            
            $i       = 0;
            $date    = DAY_CLOSURE_START;

            $dateEnd = DAY_CLOSURE_END;
            $j       = (int) substr($date, 0, 2);
            $i       = 0;

            while ($i < 24) {

                // data for chart                

                $entryData[$i] = $entry_count[$i] + $access_entry[$i];
                $exitData[$i] = $exit_count[$i] + $access_exit[$i];

                $html_data .= '<span display="none" id="chartData_' . $i . '" data-entry="' . $entryData[$i] . '" data-exit="' . $exitData[$i] . '"></span>';

                // end / data for chart

                $j       = (int) substr($date, 0, 2);
                $date_to = strtotime($date) + 3600;
                $date_to = date("H:i:s", $date_to);


                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
                
                $html_data .= "<td class='col'>" . $date . " </td>";
                $html_data .= "<td class='col'>" . $date_to . " </td>";
                $html_data .= "<td class='col'>" . $entry_count[$i] . " </td>";
                $html_data .= "<td class='col'>" . $exit_count[$i] . " </td>";
                $html_data .= "<td class='col'>" . $access_entry[$i] . " </td>";
                $html_data .= "<td class='col'>" . $access_exit[$i] . " </td>";
                $html_data .= "<td class='col'>" . $manualopen_count[$i] . " </td>";
                $html_data .= "<td class='col'>" . $manualclose_count[$i] . " </td>";
                
                $html_data .= '</tr>';
                
                $date = $date_to;
                $i++;
            }
            
            $html_data .= '<tr class="card-text subhead d-flex justify-content-between align-items-center">';
            
            $html_data .= "<td class='col'>Total</td>";
            $html_data .= "<td class='col'></td>";
            $html_data .= "<td class='col'>" . $total_entry . "</td>";
            $html_data .= "<td class='col'>" . $total_exit . "</td>";
            $html_data .= "<td class='col'>" . $access_entry_total . "</td>";
            $html_data .= "<td class='col'>" . $access_exit_total . "</td>";
            $html_data .= "<td class='col'>" . $total_mopen . "</td>";
            $html_data .= "<td class='col'>" . $total_mclose . "</td>";
            
            $html_data .= '</tr>';
            
            $html_data .= '</table>';
            
            //End . Hourly Traffic Statistics 
            

        } //endif 
        
        else {
            $html_data = "<div class='card p-3'>No records/transactions available for the current search criteria</div>";
        }
        
        echo $html_data;
        
        
    } // end daily traffic report
	
	function open_transaction_report($from,$to)
		{
		$query_string="select * from open_transactions where entry_date_time between '".$from."' AND '".$to."'";       		 
		  
		  $con=$this->db_connect();
		 
		  $result=mysqli_query($con,$query_string) or die(mysqli_error($con));
		  $total_count= mysqli_num_rows($result);
		  
		  if(mysqli_num_rows($result) > 0)
		  {
			  
			$html_data="";
		   

			$header .= '<table width="100%" class="jspdf-table">';
			$header .= '<tr class="card-header d-flex justify-content-between align-items-center">';       
            $header.="<th class='col'>Entry DateTime</th>";
            $header.="<th class='col'>Ticket Id</th>";			
            $header.="<th class='col'>Entry type</th>";			
            $header.="<th class='col'>Device Name</th>";								
            $header.="<th class='col'>Entry Grace Period</th>";
            $header.="<th class='col'>Total Amount Paid</th>";
            $header.="<th class='col'>Payment Date Time</th>";
            $header.="<th class='col'>Exit Grace Period</th>";
			$header.="</tr>";
			$header.="<tbody class='table-view'>";
			$html_data=$header;
			
			while($data=mysqli_fetch_assoc($result))
			{			
                if($data["entry_type"]==1)
                    $entry_type="Short Term";
                if($data["entry_type"]==2)
                    $entry_type="Access";
                if($data["entry_type"]==3)
                    $entry_type="Reservation";	
				
				$html_data.="<tr class='card-text d-flex justify-content-between align-items-center'>";                       
                $html_data.="<td class='col'>".$data['entry_date_time']."</td>";
                $html_data.="<td class='col'>".$data['ticket_id']."</td>";
                $html_data.="<td class='col'>".$entry_type."</td>";
                $html_data.="<td class='col'> ".$data['device_name']." </td>";								
                $html_data.="<td class='col'>".$data['entry_grace_period']."</td>";				
                $html_data.="<td class='col'>".$data['total_amount_paid']."</td>";				
                $html_data.="<td class='col'>".$data['last_payment_date_time']."</td>";				
                $html_data.="<td class='col'>".$data['max_exit_date_time']."</td>";				
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
		}
    
        function parking_movements($from,$to,$device)
		{
        $query_string="select * from parking_movements where date_time between '".$from."' AND '".$to."'";
        if($device!="")
            $query_string.=" and device_number in(".$device.") ";

        $query_string.=" order by id desc";      
	  
		  $con=$this->db_connect();
		 
		  $result=mysqli_query($con,$query_string) or die(mysqli_error($con));
		  $total_count= mysqli_num_rows($result);
		  
		  if(mysqli_num_rows($result) > 0)
		  {
			  
			$html_data="";
		   

			$header .= '<table width="100%" class="jspdf-table">';
			$header .= '<tr class="card-header d-flex justify-content-between align-items-center">'; 
                       $header.="<th class='col'>#</th>";
            $header.="<th class='col'>Entry DateTime</th>";
            $header.="<th class='col'>Ticket Id</th>";			            
            $header.="<th class='col'>Plate Number</th>";
            $header.="<th class='col'>Device Name</th>";
			$header.="</tr>";
			$header.="<tbody class='table-view'>";
			$html_data=$header;
			$i=0;
			while($data=mysqli_fetch_assoc($result))
			{			
             
				
				$html_data.="<tr class='card-text d-flex justify-content-between align-items-center'>";   
                                 $html_data.="<td class='col'>".$i."</td>";
                $html_data.="<td class='col'>".$data['date_time']."</td>";
                $html_data.="<td class='col'>".$data['ticket_id']."</td>";               
                $html_data.="<td class='col'>".$data['plate_number']."</td>";               
                $html_data.="<td class='col'> ".$data['device_name']." </td>";								               
				$html_data.="</tr>";  				
			 $i++;
			}          
			$html_data.="</table>"; 
                       $summary .= '<div class="row mb-4 jspdf-graph">';
                        $summary .= '<div class="col-lg-3 col-6">';                       
                        $summary .= '<div class="small-box bg-success">';
                        $summary .= '<div class="inner">';
                        $summary .= '<h3>' .$total_count. '</h3>';
                        $summary .= '<h6>Transactions</h6>';
                        $summary .= '</div>';
                        $summary .= '<div class="icon">';
                        $summary .= '<i class="ion ion-stats-bars"></i>';
                        $summary .= '</div>';
                        $summary .= '</div>';
                        $summary .= '</div>';
                        $summary .= '</div><!-- Row End -->';
                        $html_data=$summary.$html_data;
			

		  }
		  else
		  {
			  $html_data="<div class='p-3'>No records/transactions available for the current search criteria</div>";
		  }
			
                  
    
mysqli_close($con);  
echo $html_data;
}
    

    //shows the parking duration report for the whole facility 
    function show_open_transactions_report($fromdate, $todate, $carpark)
    {
        $count_shortterm = array();
        $count_contract  = array();
        $duration_label  = array();
        
        $i = 0;
        $j = 0;
        
        $count_shortterm = $this->get_open_transactions($fromdate, $todate, $carpark, "shortterm");
        $count_contract=$this->get_open_transactions($fromdate, $todate, $carpark, "longterm");
        
        
        //CREATE THE DURATION LABELS 
        $duration_label[0] = "0 to 30 Min";
        $duration_label[1] = "30 to 60 Min";
        $i                 = 2;
        while ($i < 24) {
            $duration_label[$i] = $i . " - " . ($i + 1) . " Hours";
            $i++;
        }
        
        $j = 1;
        while ($j < 4) {
            $duration_label[$i] = $j . " - " . ($j + 1) . " Days";
            $i++;
            $j++;
        }
        //-------------------// 
        
        $html_data = "";
        
        $header .= '<table width="100%" class="jspdf-table">';
        $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';
        $header .= "<th class='col'>Duration </th>";
        $header .= "<th class='col'>Count-Short term</th>";
        $header .= "<th class='col'>Count-Contract</th>";
        $header .= "</tr>";
        
        $html_data = $header;
        $html_data .= '<tbody class="table-view">';
        $i         = 0;
        while ($i < 27) {

            $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
            $html_data .= "<td class='col'> " . $duration_label[$i] . " </td>";
            $html_data .= "<td class='col'>" . $count_shortterm[$i] . "</td>";
            $html_data .= "<td class='col'>" . $count_contract[$i] . "</td>";
            $html_data .= "</tr>";
            
            $i++;
        }
        $html_data .= "</table>";
        
        echo $html_data;
        
    } //END. Show parking duration report 
    
    
    //shows the parking duration report for the whole facility 
    function show_parking_duration_report($fromdate, $todate, $carpark, $weekdays)
    {
        $count_shortterm = array();
        $count_contract  = array();
        $duration_label  = array();
        
        $i = 0;
        $j = 0;
        
        $count_shortterm = $this->get_parking_duration($fromdate, $todate, $carpark, "shortterm", $weekdays);
        $count_contract  = $this->get_parking_duration($fromdate, $todate, $carpark, "longterm", $weekdays);
        $flag=false;

        $count1 = array_sum($count_shortterm);
        $count2 = array_sum($count_contract);
        
        //CREATE THE DURATION LABELS 
        $duration_label[0] = "0 - 30 Min";
        $duration_label[1] = "30 - 60 Min";

        $duration_chart_label[0] = "0 - 30\nMin";
        $duration_chart_label[1] = "30 - 60\nMin";

        $i= 2;
        
        while ($i <= 24) 
            {            
            $duration_label[$i] = ($i-1) . " - " .$i . " Hours";
            $duration_chart_label[$i] = ($i-1) . " - " .$i . "\n" . "Hours";
            $i++;
            }
        
        $j = 1;
        while ($j < 3) 
            {
            $duration_label[$i] = $j . " - " . ($j + 1) . " Days";
            $duration_chart_label[$i] = $j . " - " . ($j + 1) . "\n" . "Days";
            $i++;
            $j++;
            }

        if (($count1 > 0) || ($count2 > 0)) {
        
            $html_data = "";

            // create display minutes/hours text for info box

            $max_dur_short_term = $count_shortterm[27];
            $max_dur_contract = $count_contract[27];
            $min_dur_short_term = $count_shortterm[28];
            $min_dur_contract = $count_contract[28];
            $avg_dur_short_term = $count_shortterm[29];
            $avg_dur_contract = $count_contract[29];

            $display_values = array($max_dur_short_term, $max_dur_contract, $min_dur_short_term, $min_dur_contract,$avg_dur_short_term, $avg_dur_contract);

            // print_r($display_values);

            for ($i = 0; $i < count($display_values); $i++) {

                if ($display_values[$i] < 60) {
                    $display_values[$i] .= "m";
                } else {
                    $temp = $display_values[$i]/60; // 737/60 = 12.28
                    $temp1 = floor($temp);          // 12 hours
                    $temp2 = $temp - $temp1;        // 12.28 - 12 = 0.28
                    $temp2 = $temp2 * 60;           // 0.28 * 60 = 17 minutes

                    $display_values[$i] = $temp1 . "h " . $temp2 . "m";
                }
            }

            // print_r($display_values);
            
            $header .= '<div class="row mb-4 jspdf-graph text-center">';
            $header .= '<div class="col-md-4 pl-0">';
            $header .= '<!-- small card -->';
            $header .= '<div class="small-box bg-info">';
            $header .= '<h5 class="small-box-footer text-white">Maximum Parking Duration</h5>';
            $header .= '<div class="inner row">';
            $header .= '<div class="col">';
            $header .= '<h3>'.$display_values[0].'</h3>';
            $header .= '<h5>Short-Term</h5>';
            $header .= '</div>';
            $header .= '<div class="col">';
            $header .= '<h3>'.$display_values[1].'</h3>';
            $header .= '<h5>Contract</h5>';
            $header .= '</div>';
            $header .= '</div>';
            $header .= '</div>';
            $header .= '</div>';
            $header .= '<div class="col-md-4">';
            $header .= '<!-- small card -->';
            $header .= '<div class="small-box bg-info">';
            $header .= '<h5 class="small-box-footer text-white">Minimum Parking Duration</h5>';
            $header .= '<div class="inner row">';
            $header .= '<div class="col">';
            $header .= '<h3>'.$display_values[2].'</h3>';
            $header .= '<h5>Short-Term</h5>';
            $header .= '</div>';
            $header .= '<div class="col">';
            $header .= '<h3>'.$display_values[3].'</h3>';
            $header .= '<h5>Contract</h5>';
            $header .= '</div>';
            $header .= '</div>';
            $header .= '</div>';
            $header .= '</div>';
            $header .= '<div class="col-md-4 pr-0">';
            $header .= '<!-- small card -->';
            $header .= '<div class="small-box bg-info">';
            $header .= '<h5 href="#" class="small-box-footer text-white">Average Parking Duration</h5>';
            $header .= '<div class="inner row">';
            $header .= '<div class="col">';
            $header .= '<h3>'.$display_values[4].'</h3>';
            $header .= '<h5>Short-Term</h5>';
            $header .= '</div>';
            $header .= '<div class="col">';
            $header .= '<h3>'.$display_values[5].'</h3>';
            $header .= '<h5>Contract</h5>';
            $header .= '</div>';
            $header .= '</div>';
            $header .= '</div>';
            $header .= '</div>';
            $header .= '</div>';
            
            $header .= '<div class="col-md-6 p-0">';

            // begin header
            
            $header .= '<table width="100%" class="jspdf-table">';
            $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';
            
            $header .= "<th class='col'>Duration</th>";
            $header .= "<th class='col'>Short-term Parkers</th>";
            $header .= "<th class='col'>Contract Parkers</th>";
            
            $header .= '</tr>';
            
            $html_data .= $header;
            
            $html_data .= '<tbody class="table-view">';
            
            $i = 0;
            
            while ($i < (count($count_shortterm) - 3)) {

                // data for chart

                $html_data .= '<span display="none" id="chartData_'.$i.'" data-duration="'.$duration_chart_label[$i].'" data-short-term="'.$count_shortterm[$i].'" data-contract="'.$count_contract[$i].'"></span>';

                // end / data for chart

                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
                
                $html_data .= "<td class='col'>" . $duration_label[$i] . "</td>";
                $html_data .= "<td class='col'>" . $count_shortterm[$i] . "</td>";
                $html_data .= "<td class='col'>" . $count_contract[$i] . "</td>";
                
                $html_data .= '</tr>';
                
                $i++;
            }
            
            $html_data .= '</table>';
            
            $html_data .= '</div>';

        } else {
            $html_data="<div class='card'><div class='p-3'>No records/transactions available for the current search criteria</div></div>";
        }
        echo $html_data; 
    } //END. Show parking duration report 

    /* occupancy */

    function hourly_occupancy_report($carpark, $type)
    {
        $total_count = array();
        $i           = 0;
        $j           = 0;
        $k           = 1;

	$con=$this->db_connect();
        // $con = mysqli_connect("192.168.1.227", "parcx", "Parcx123!", "parcx_reporting");
        
         if (!$con)
         {
             $this->error("Connection attempt failed");
         }                
        //Start . Generate the SQL Query String . 
        $query_string = "select";
        while ($i < 24) {
            $query_string .= " sum(h" . $i . "to" . ($i + 1) . ") as h" . $i . "to" . ($i + 1);
            if($i<23)
            {
                $query_string .= ","; 
            }
            $i++;
        }
       
        
        $query_string .= " from hourly_occupancy where report_date =curdate() and type='" . $type . "'";
        
        if ($carpark > 0) {
            $query_string.="and carpark_number=".$carpark;
            /*$query_string .= " AND carpark_number IN('";
            
            for ($i = 0; $i < count($carpark); $i++) {
                $query_string .= "$carpark[$i]";
                
                if ($i < (count($carpark) - 1)) {
                    $query_string .= "','";
                }
            }
            $query_string .= "')";*/
        }
        
        
       // echo $query_string;    
                
        
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        
        if (mysqli_num_rows($result) > 0) {
            $data           = mysqli_fetch_assoc($result);
            $i              = 0;
            $j              = 0;
            //get count for hours
            while ($i < 24) {
                $field           = "h" . $i . "to" . ($i + 1);
                $total_count[$j] = (int) $data[$field];
                $i++;
                $j++;
            }
            mysqli_close($con);
            return $total_count;
            
        } else {
            $i = 0;
            while ($i < 24) {
                $total_count[$i] = 0;
                $i++;
            }
            
            return $total_count;
        }
        
    } // End Hourly Occupancy

function update_whitelist_flag($tag)
    {
    $con = $this->db_connect();
    if ($con)
        {
        $query_string = "update parking_movements_access set whitelist_added=1 where tag ='$tag'";        
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));               
        mysqli_close($con);        
	    }
    } // End Function
    
	
function get_camera_devices()
  {
      
    
    $con= $this->db_connect_config();
    $html_data="<option value=0>Select Device</option>";
    if($con)
        {       
        $query_string = "Select device_name,device_number from system_devices where device_category = 8";
        $result = $con->query($query_string);
        while($data = mysqli_fetch_array($result))
        {                
            $html_data .= "<option value = '".$data['device_number']."'>".$data['device_name']."</option>";  
        }
        mysqli_close($con);
        echo $html_data;
        }
    }

    function report_plates_captured($json)
    {    	
        $con=$this->db_connect();
            $json=json_decode($json);
            $sql="select * from plates_captured where  capture_date_time >='".$json->{'fromDate'}."' and capture_date_time <= '".$json->{'toDate'}."'";			
            if($json->{'device'}>0)
                $sql.=" and camera_device_number='".$json->{'device'}."'";

            if($json->{'plate'}!="")
                $sql.=" and plate_number like '%".$json->{'plate'}."%'";
            
            if($json->{'type'}!="")
               $sql.=" and plate_type = '".$json->{'type'}."'";

            if($json->{'area'}!="")
                $sql.=" and plate_area = '".$json->{'area'}."'";

            if($json->{'country'}!="")
               $sql.=" and plate_country = '".$json->{'country'}."'";

            $sql.=" order by capture_date_time desc";
            

            
            $i=1;
            $result = $con->query($sql);
            if(mysqli_num_rows($result) > 0)
                { 
                $html_data="";
                $html_data.='<div class="card-header">';
                $html_data.='<div class="d-flex justify-content-between align-items-center">';
                $html_data.='<div class="col">CAMERA NAME</div>';
                $html_data.='<div class="col">DATE TIME</div>';
                $html_data.='<div class="col">PLATE NUMBER</div>';            
                $html_data.='<div class="col">PLATE AREA</div>';
                $html_data.='<div class="col">PLATE COUNTRY</div>';            
                $html_data.='<div class="col">CROPPED IMAGE</div>'; 
                $html_data.='<div class="col">FULL IMAGE</div>';          
                $html_data.='</div>';
                $html_data.='</div>';    
                //echo $sql;
                while($data = mysqli_fetch_array($result))
                    {  
                    $plate_image=$data['plate_image_name'];
                    $plate_image=str_replace(' ','%20',$plate_image);
                    $plate_image=str_replace('#','%23',$plate_image);	
                    
                    $plate_date=$data['capture_date_time'];
                    $plate_date=substr($plate_date,0,10);
                    
                    $html_data.= '<div class="table-view">';
                    $html_data.= '<div class="card-text">';
                    $html_data.= '<div class="d-flex justify-content-between align-items-center"><input type="hidden" id = "date'.$i.'" value = "'.$plate_date.'"><input type="hidden" id = "camera_no'.$i.'" value = "'.$data['camera_device_number'].'"><input type="hidden" id = "plate_image'.$i.'" value = "'.$data['plate_image_name'].'">';               
                    $html_data.= '<div class="col">' . $this->get_device_name($data['camera_device_number']). '</div>';
                    $html_data.= '<div class="col">' . $data['capture_date_time']. '</div>';
                    $html_data.= '<div class="col">' . $data['plate_number']. '</div>';               
                    $html_data.= '<div class="col">' . $data['plate_area']. '</div>';
                    $html_data.= '<div class="col">' . $data['plate_country']. '</div>';                
                    $html_data.= '<div class="col"><img
                            src="'.ImageURL.'\\'.$data['camera_device_number'].'\\'.$plate_date.'\\Crop_'.$plate_image.'"
                            width="100" ; height="50" ;></center>
                    </div>';                
                    $html_data.= '<div class="col" >
                    <input type="button" data-value = "'.$i.'" data-toggle="modal" data-target="#image-modal"
                        value="View" class="btn btn-link">
                    </div>';
                    //$html_data.= '<div class="col"><a target="_blank" href = "'.ImageURL.'/Scene/'.$data['CAMERA_DEVICE_NUMBER'].'/Scene_'.$data['PLATE_IMAGE_NAME'].'">View</a></center></div>';
                    $html_data.= '</div>';
                    $html_data.= '</div>';
                    $i = $i + 1;
                    }
                }
            else
                {
                    $html_data="<div class='card'><div class='p-3'>No records/transactions available for the current search criteria</div></div>";
                }
            mysqli_close($con);
            echo $html_data;
            
    }


    function get_anpr_image_platescaptured($json)
        {
               
        $json=json_decode($json);
        $html_data="";
        $plateImage = $json->{'plate_image'};
		$plateImage=str_replace(' ','%20',$plateImage);
		$plateImage=str_replace('#','%23',$plateImage);
        $camera_id = $json->{'camera_no'};
        $plate_date=$json->{'date'};
        $html_data.='<div>';
        //$html_data.='<img src ="'.ImageURL.'/default.jpg" width="100%"; height="500";>';
        $html_data.='<img src ="'.ImageURL.'\\'.$camera_id.'\\'.$plate_date.'\\Scene_'.$plateImage.'" width="1000"; height="700";>';
        $html_data.='</div>';
        echo $html_data;
        }

    	
} // end class
