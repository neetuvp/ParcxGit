<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

 
class reporting_revenue
    {    
    public $html_data="";
 
    function db_connect()
        {	    
        $con=mysqli_connect(DB_HOST,DB_USER,DB_PASSWORD,DB_NAME_REPORTING);
        if(!$con) 
        $this->error("Connection attempt failed");	
        else
            return $con;
        } // end 

    function db_connect_config()
        {	        
        $con=mysqli_connect(DB_HOST,DB_USER,DB_PASSWORD,DB_NAME);
        if(!$con) 
            $this->error("Connection attempt failed");	
        else
            return $con;
        } // end 

    function db_connect_dashboard()
        {
        $con=mysqli_connect(DB_HOST,DB_USER,DB_PASSWORD,DB_NAME_DASHBOARD);
        if(!$con)
            $this->error("Connection attempt failed");
        else
            return $con;
        } // end


    function get_payment_devices()
        {        
        $con=$this->db_connect_config();   
        $html_data="";
        if($con)
            {  
            $query_string="select device_name,device_number from system_devices where device_category=3 or device_category=5 or device_category=4 ORDER BY device_category ASC";
            $result=mysqli_query($con,$query_string) or die(mysqli_error($con));         
            while($data=mysqli_fetch_assoc($result))        
                $html_data.="<option value=".$data['device_number'].">".$data['device_name']."</option>";                   
                
            echo $html_data;
            }
        else
            echo "<option>Not Available</option>";                
        mysqli_close($con);        
        }// End FUnction 

    function get_apm_devices()
        {            
        $con=$this->db_connect_config();   
        $html_data="";
        if($con)
            {  
            $query_string="select device_name,device_number from system_devices where device_category=4 ORDER BY device_category ASC";
            $result=mysqli_query($con,$query_string) or die(mysqli_error($con));             
            while($data=mysqli_fetch_assoc($result))            
                $html_data.="<option value=".$data['device_number'].">".$data['device_name']."</option>";                           
                
            echo $html_data;
            }
        else
            echo "<option>Not Available</option>";                        
        mysqli_close($con);            
        }// End FUnction 


    function get_operators()
        {
        $con=$this->db_connect_config();   
        $html_data="";
        if($con)
            {  
            $query_string="select operator_name,user_id from system_users";
            $result=mysqli_query($con,$query_string) or die(mysqli_error($con));                 
            while($data=mysqli_fetch_assoc($result))        
                $html_data.="<option  value=".$data['user_id'].">".$data['operator_name']."</option>";                   
            
            echo $html_data;
            }
        else        
            echo "<option>Not Available</option>";                
        mysqli_close($con);        
        }
   

    function get_carparks()
        {
        $con=$this->db_connect_config();   
        $html_data="";
        if($con)
            {  
            $query_string="select carpark_name,carpark_number from system_carparks";
            $result=mysqli_query($con,$query_string) or die(mysqli_error($con));                        
            while($data=mysqli_fetch_assoc($result))            
                $html_data.="<option  value=".$data['carpark_number'].">".$data['carpark_name']."</option>";   
                
            echo $html_data;
            }
        else
            echo "<option>Not Available</option>";                    
        mysqli_close($con);        
        }

    function get_discounts()
        {
        $con=$this->db_connect_config();    
        if($con)
            {
            $query_string="select discount_id,discount_name from revenue_discounts";
            $result=mysqli_query($con,$query_string) or die(mysqli_error($con));            
            while($data=mysqli_fetch_assoc($result))            
                $html_data.="<option value=".$data['discount_id'].">".$data['discount_name']."</option>";            

            echo $html_data;
            }
        else
            echo "<option>Not Available</option>";        
        mysqli_close($con);
        }


    function shift_report($data)
        { 
        $FromDateTime=$data["from"];
        $ToDateTime=$data["to"];
        $operator=$data["operator"];
        $device =$data["device"];
        $carpark =$data["carpark"];
        $con=$this->db_connect(); 
        $query_string="select * from revenue_shifts where login_time between '".$FromDateTime."' AND '".$ToDateTime."'";

        if (strlen($carpark) > 0) 		           
            $query_string .= " AND carpark_number IN(".$carpark.")";                       
		
	    if (strlen($operator) > 0) 		           
            $query_string .= " AND operator_id IN(".$operator.")"; 

        if (strlen($device) > 0) 		           
            $query_string .= " AND device_number IN(".$device.")"; 

        $query_string.=" ORDER BY device_number ASC";       
        $result=mysqli_query($con,$query_string) or die(mysqli_error($con)); 
        if(mysqli_num_rows($result)) 
            {
            $physical_cash_collected=0;
            $difference=0;
                        
            $html_data = "";

            $header .= '<table width="100%" class="jspdf-table">';
            $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';

            $header .= "<th class='col'>#</th>";
            $header .= "<th class='col-2'>Shift Number</th>";
            $header .= "<th class='col'>Device Name</th>";
            $header .= "<th class='col'>Operator Name</th>";
            $header .= "<th class='col'>Shift Start</th>";
            $header .= "<th class='col'>Shift End</th>";
            $header .= "<th class='col'>Shift Earnings</th>";
            $header .= "<th class='col'>Over / Short</th>";
            $header .= "<th class='col'>Last Updated</th>";
            $header .= "<th class='col'>Status</th>";
    
            $header .= '</tr>';

            $html_data = $header;
            $html_data .= '<tbody class="table-view">';

            while($data=mysqli_fetch_assoc($result))
                {                         
                if($data['physical_cash_collected']>0)                    
                    $difference=$data['physical_cash_collected']-$data['shift_earnings'];                
                else                    
                    $difference="-";                    
            
                        
                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center pointer" data-toggle="modal" data-target="#detailModal" id="shift_detail" shift_id="'.$data['shift_id'].'">';

                $html_data .= "<td class='col'>" . $data['id'] . "</td>";
                $html_data .= "<td class='col-2'>" . $data['shift_id'] . "</td>";
                $html_data .= "<td class='col'>" . $data['device_name'] . "</td>";
                $html_data .= "<td class='col'>" . $data['operator_name'] . "</td>";
                $html_data .= "<td class='col'>" . $data['login_time'] . "</td>";
                $html_data .= "<td class='col'>" . $data['logout_time'] . "</td>";
                $html_data .= "<td class='col'>" . $data['shift_earnings'] . "</td>";
                $html_data .= "<td class='col'>" . $difference . "</td>";
                $html_data .= "<td class='col'>" . $data['last_updated_date_time'] . "</td>";
                
                if($data['shift_status'] == 1)                 
                    $html_data.="<td class='col'><span class='badge bg-danger shift_status_closed'>Closed</span></td>";                               
                else 
                    $html_data.="<td class='col'><span class='badge bg-success'>Open</span></td>";
             
        
                $html_data .= '</tr>';

                }          

            $html_data .= '</table>';
         
            }
        else             
            $html_data="<div class='p-3'>No records/transactions available for the current search criteria</div>";
        mysqli_close($con);
        echo $html_data;
        }
   
  
   
   
    function get_shift_physical_cash_collected($shift_id)
        {
        $con=$this->db_connect(); 
        $query_string="select total from revenue_physical_cash where shift_id='".$shift_id."'";         
        $result=mysqli_query($con,$query_string) or die(mysqli_error($con)); 
        $data=mysqli_fetch_assoc($result);        
        $physical_cash_collected=(float)$data["total"];
        mysqli_close($con);
        return  $physical_cash_collected;
        }
   
    function get_shift_details($shift_id) 
        {    
        $html_data="";
        $con=$this->db_connect(); 
        $query_string="select * from revenue_shifts where shift_id='".$shift_id."'";     
        $result=mysqli_query($con,$query_string) or die(mysqli_error($con));               
        $data=mysqli_fetch_assoc($result);
             
        $html_data.="<tr><td>Device Name</td><td> ".$data['device_name']." </td></tr>";  
        $html_data.="<tr><td>Operator Name</td><td>".$data['operator_name']."</td></tr>";    
        $html_data.="<tr><td>Start Time</td><td>".$data['login_time']."</td></tr>";
        $html_data.="<tr><td>End Time</td><td>".$data['logout_time']."</td></tr>";
        $html_data.="<tr><td>Shift Open Till Amount</td><td>".$data['shift_opening_amount']."</td></tr>";        
        $html_data.="<tr><td>Shift Close Till Amount</td><td>".$data['shift_closing_amount']."</td></tr>";
        $html_data.="<tr><td>Pay In Amount</td><td>".$data['payin_amount']."</td></tr>";        
        $html_data.="<tr><td>Pay Out Amount</td><td>".$data['payout_amount']."</td></tr>";
        $html_data.="<tr><td>Duration</td><td>".$data['shift_duration']."</td></tr>";                   
        $html_data.="<tr><td>Parking Fee Collected</td><td>".$data['parking_fee']."</td></tr>";  
        $html_data.="<tr><td>Lost Ticket Fee </td><td>".$data['lost_fee']."</td></tr>";  
        $html_data.="<tr><td>VAT</td><td>".$data['vat_amount']."</td></tr>";               
        $html_data.="<tr><td>Shift Earnings</td><td class='font-weight-bold'>".$data['shift_earnings']."</td></tr>";          
        $html_data.="<tr><td>Physical Cash Collected</td><td class='font-weight-bold'>".$data['physical_cash_collected']."</td></tr>";       
        if($data['physical_cash_collected']>0)                      
            $difference=$data['physical_cash_collected']-$data['shift_earnings'];                
        else            
            $difference="-";               
        $html_data.="<tr><td>Over / Short</td><td class='font-weight-bold'>".$difference."</td></tr>";
        $html_data.="</div>";         
        mysqli_close($con);
        echo $html_data;     
        } 
	
    function get_payment_details($data) 
        {  
        $id=$data["payment_id"];
        $fdata=$this->getFacilityFeatures();
        $d=$fdata["decimal_places"];
        $currency=$fdata["currency"]; 
        $vat= $fdata["vat_percentage"]; 
        $html_data="";
        $con=$this->db_connect(); 
        $query_string="select * from revenue_payments where id='".$id."'";     
        $result=mysqli_query($con,$query_string) or die(mysqli_error($con));               
        $data=mysqli_fetch_assoc($result);        	   
        $html_data.="<tr><td>Ticket Id</td><td> ".$data['ticket_id']." </td></tr>";  
        $html_data.="<tr><td>Device Name</td><td> ".$data['device_name']." </td></tr>";  
        $html_data.="<tr><td>Parking Rate</td><td>".$data['parking_rate_label']."(".$data['parking_rate_name'].")</td></tr>";    
        $html_data.="<tr><td>Entry Grace Period</td><td>".$data['entry_grace_period']."</td></tr>";
        $html_data.="<tr><td>Exit Grace Period</td><td>".$data['exit_grace_period']."</td></tr>";
        if($vat!=0)
            $html_data.="<tr><td>VAT</td><td>".$data['vat_percentage']."% ".$data['vat_type']."</td></tr>";
        $html_data.="<tr><td>Entry Date Time</td><td>".$data['entry_date_time']."</td></tr>";        
        $html_data.="<tr><td>Payment Date Time</td><td>".$data['payment_date_time']."</td></tr>";
        $html_data.="<tr><td>Max Exit Date Time</td><td>".$data['max_exit_date_time']."</td></tr>";        
        $html_data.="<tr><td>Parking Duration</td><td>".$data['parking_duration']."</td></tr>";
        $html_data.="<tr><td>Category</td><td>".$data['payment_category']."</td></tr>"; 
        if($data['validation_id']!="")
            {
            $v =  explode (",", $data['validation_value']);
            $html_data.="<tr><td>Validations</td><td>".$v[0]." Hour ".$v[1]." %</td></tr>";                   
            }
        $html_data.="<tr><td>Parking Fee</td><td>".number_format($data['parking_fee'],$d)." ".$currency."</td></tr>";  
        $html_data.="<tr><td>Lost Ticket Fee </td><td>".number_format($data['lost_fee'],$d)." ".$currency."</td></tr>"; 
        if($vat!=0) 
            $html_data.="<tr><td>VAT</td><td>".number_format($data['vat_amount'],$d)." ".$currency."</td></tr>"; 

        $html_data.="<tr><td>Discount</td><td>".number_format($data['discount_amount'],$d)." ".$currency."</td></tr>"; 		
        if($data['discount_amount']>0)
            $html_data.="<tr><td>Discount name</td><td>".$data['discount_name']."</td></tr>";
        $html_data.="<tr><td>Gross Amount</td><td class='font-weight-bold'>".number_format($data['gross_amount'],$d)." ".$currency."</td></tr>";          
        if($data['validation_hours']>0)
            $html_data.="<tr><td>Validation hours</td><td>".$data['validation_hours']."</td></tr>";                                
                                
        $notes=$data['bank_notes'];
        $total=0;
        if(strlen($notes)>0)
            {
            $notes=substr($notes,0,-1);				
            $notes=str_replace(";",":",$notes);
            $array=explode(":",$notes);
            $notes="";
            
            for($i=0;$i<count($array);$i+=2)
                {
                $a=$array[$i];
                $b=$array[$i+1];
                $sum=$a." ".$currency." x ".$b." = ".($a*$b)." ".$currency;
                if(count($array)>0)
                    $notes=$sum."<br>".$notes;
                $total+=($a*$b);
                }	
            }
        if($notes!="")    
            $html_data.="<tr><td>Amount received</td><td>".number_format($total,$d)."  ".$currency."</td></tr>";		
        $html_data.="<tr><td>Balance returned</td><td>".number_format($data['balance_returned'],$d)." ".$currency." </td></tr>";
        $html_data.="<tr><td>Credit note</td><td>".number_format($data['credit_note'],$d)." ".$currency." </td></tr>";		
        if($notes!="")
            $html_data.="<tr><td>Bank notes</td><td>".$notes."</td></tr>";
        $html_data .= "<tr><td><input class='btn btn-info btn-show-pdf-reciept' value='PDF Receipt' type='button' data-id='".$data['id']."'></td></tr>";
        //$html_data.="</div>";   
        //$html_data .= "<input class='btn btn-info btn-show-pdf-reciept' value='PDF Receipt' type='button' data-id='".$data['id']."'>";      
        mysqli_close($con);
        echo $html_data;    
        } //END 


    function get_pdf_reciept($data) 
        {  
        $id=$data["payment_id"];
        $fdata=$this->getFacilityFeatures(); 
        $vat= $fdata["vat_percentage"]; 
        $currency=$fdata["currency"];
        $d=$fdata["decimal_places"];
        $html_data="";
        $con=$this->db_connect(); 
        $query_string="select * from revenue_payments where id='".$id."'";     
        $result=mysqli_query($con,$query_string) or die(mysqli_error($con));               
        $data=mysqli_fetch_assoc($result);        	   
        $html_data.="<div class='row mb-4'><div class='col text-center'>";                                          
        $html_data.="<div>".$fdata["facility_name"]."</div>"; 
        $html_data.="<div>".$fdata["address_line1"]."</div>"; 
        $html_data.="<div>".$fdata["address_line2"]."</div>"; 
        if($vat>0)
            $html_data.="<div>TRN No ".$fdata["vat_id"]."</div>";                    
        
        $html_data.="<hr><div>Device Name: ".$data["device_name"]."</div>";
        $html_data.="<div>Operator Name: ".$data["operator_name"]."</span></div>";
        $html_data.="<div>Payment Type: ".$data["payment_type"]."</div></div></div>";
        $html_data.="<div class='table-responsive-sm'>";
        $html_data.="<table class='table table-striped table-receipt'>";
        $html_data.="<thead><tr><th></th><th></th><th></th></tr></thead>";
        $html_data.="<tbody><tr><td></td><td>";    
        $html_data.="<div>".$data["payment_category"]."</div>";
        $html_data.="<span>Ticket ID: ".$data["ticket_id"]."</span>";
        $html_data.="<span>Entry Date Time: ".$data["entry_date_time"]."</span>";
        $html_data.="<span>Payment Date Time: ".$data["payment_date_time"]."</span> ";   
        $html_data.="</td>";
        $html_data.="<td></td></tr>";
        $html_data.="<tr><td></td>";
        $html_data.="<td><strong>Parking Duration</strong></td>";
        $html_data.="<td><span>".$data["parking_duration"]."</span></td></tr>";
        
        if($data['validation_id']!="")
            {
            $v =  explode (",", $data['validation_value']); 
            $html_data.="<tr><td></td>";           
            $html_data.="<td><strong>Validations</strong></td>";
            $html_data.="<td><span>".$v[0]." H ".$v[1]." %</span></td></tr>";              
            }
        $html_data.="<tr><td></td>";
        $html_data.="<td><strong>Parking Fees</strong></td>";
        $html_data.="<td><span>".$currency." ".number_format($data["parking_fee"],$d)."</span></td></tr>";
        if($data["discount_amount"]>0)
            {
            $html_data.="<tr><td></td>";
            $html_data.="<td><strong>Discount amount</strong><span>".$data["discount_name"]."</span></td>";
            $html_data.="<td><span>".$currency." ".number_format($data["discount_amount"],$d)."</span></td></tr>";
            }
        $html_data.="<tr><td></td>";
        $html_data.="<td><strong>Net Amount</strong></td>";
        $net=$data["gross_amount"]-$data["vat_amount"];
        $html_data.="<td><span>".$currency." ".number_format($net,$d)."</span></td></tr>";
        if($vat>0)
            {
            $html_data.="<tr><td></td>";
            $html_data.="<td><strong>VAT (".$vat."%)</strong></td>";
            $html_data.="<td><span>".$currency." ".number_format($data["vat_amount"],$d)."</span></td></tr>";
            }
        
        $html_data.="<tr><td></td>";
        $html_data.="<td><strong>Gross amount</strong></td>";
        $html_data.="<td><span>".$currency." ".number_format($data["gross_amount"],$d)."</span></td></tr>";
        $html_data.="</tbody></table> ";
        mysqli_close($con);
        echo $html_data;    
            } //END
    
    function get_banknote_denominations($country,$con) 
        {
        $query_string="select * from banknote_denomination_cashier where country='".$country."'"; 
        $result=mysqli_query($con,$query_string) or die(mysqli_error($con));
        $data=mysqli_fetch_assoc($result);
        $denomination=array();
        $i=1;
        while($i < 11)
            {
            $field="denomination".$i;
        
            $denomination[$i]=$data[$field];
            
            $i++;
            }  
        return $denomination;
        } // END . Get list of banknotes by  country 
    
        // Get the list of physcial bank notes by denomination collected during the shift
        function get_banknotes_collected($shift_id,$con)
        {
        $query_string="select * from revenue_physical_cash where shift_id='".$shift_id."'"; 
        $result=mysqli_query($con,$query_string) or die(mysqli_error($con));
        $data=mysqli_fetch_assoc($result);
        $denomination=$this->get_banknote_denominations("UAE", $con);
        
        $html_data = "<div id='sub_header'><h4>Banknotes Collected</h4></div>";
        $html_data .= "<table cellpadding='0' cellspacing='0' border='0'>";       
        $i=1;
        while($i < 11)
        {
        $field="denomination".$i;     
        //$html_data .= "<tr><td>".$denomination[$i]." </td><td>".$data[$field]."</td></tr>";
        $html_data .= "<tr><td>".$denomination[$i]." </td><td>".$data[$field]."</td></tr>";
        $i++;
        }
        
        $html_data.="</table>";
        
        return $html_data;
        
        } // END . Get list of physical bank note counts

    function revenue_discount_usage_total($discountId,$FromDateTime,$ToDateTime,$carpark)
        {
        $con=$this->db_connect(); 
        //discount with available count
        $query_string="select sum(discount_usage) as discount_usage FROM revenue_discount_usage where discount_id='$discountId' and report_date between '$FromDateTime' and '$ToDateTime'";
        if(count($carpark) > 0)
            {
            //$query_string.="and carpark_number=".$carpark;
            $query_string.=" AND carpark_number IN('";
    
            for($i=0; $i < count($carpark);$i++)
            {
            $query_string.="$carpark[$i]";
            
            if($i < (count($carpark) -1)){
            $query_string.="','";
                }
            }
            $query_string.="')";
            } 
        
            // echo "<br>".$query_string;
        
        $result=mysqli_query($con,$query_string) or die(mysqli_error($con));   
        
        if($data=mysqli_fetch_assoc($result))
            return $data['discount_usage'];
        else
            return 0;   
        mysqli_close($con);     

        }


    function revenue_discount_usage_report($FromDateTime,$ToDateTime,$carpark)
        {
            $con=$this->db_connect(); 
            //discount with available count
            //$query_string="select discount_id,discount_name,discount_method,discounts_available FROM revenue_discount_usage where id in(select max(id) from revenue_discount_usage where report_date between '$FromDateTime' and '$ToDateTime'";
            $query_string="select discount_id,discount_name,discount_method,discounts_available FROM revenue_discount_usage where report_date in(select max(report_date) from revenue_discount_usage where report_date between '$FromDateTime' and '$ToDateTime'";
            if(count($carpark) > 0)
            {
                //$query_string.="and carpark_number=".$carpark;
                $query_string.=" AND carpark_number IN('";
        
                for($i=0; $i < count($carpark);$i++)
                {
                $query_string.="$carpark[$i]";
                
                if($i < (count($carpark) -1)){
                $query_string.="','";
                    }
                }
                $query_string.="')";
            } 
            
            $query_string.=" GROUP BY discount_id) group by discount_id";
        // echo $query_string;
            
            $result=mysqli_query($con,$query_string) or die(mysqli_error($con));   
            
            if(mysqli_num_rows($result)>0)
            {
                $header .= '<div class="card-body p-0">';

                $header .= '<table width="100%" class="jspdf-table">';
                $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';

                $header .= "<th class='col'>Discount Name</th>";
                $header .= "<th class='col'>Discount Method</th>";
                $header .= "<th class='col'>Discount Used</th>";
                $header .= "<th class='col'>Discount Unused</th>";

                $header .= '</tr>';

                $html_data .= $header;

                $html_data .= '<tbody class="table-view">';

                while ($data = mysqli_fetch_assoc($result)) {

                    $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';

                    $html_data .= "<td class='col'> " . $data['discount_name'] . " </td>";
                    $html_data .= "<td class='col'> " . $data['discount_method'] . " </td>";
                    $html_data .= "<td class='col'> " . $this->revenue_discount_usage_total($data['discount_id'], $FromDateTime, $ToDateTime, $carpark) . " </td>";
                    $html_data .= "<td class='col'> " . $data['discounts_available'] . " </td>";

                    $html_data .= '</tr>';

                }

                $html_data .= '</tbody>';
                $html_data .= '</table>';
                $html_data .= '</div>';

            }
            else
            {
                $html_data="<div class='p-3'>No records/transactions available for the current search criteria</div>";
            }
            mysqli_close($con);    
            echo $html_data;           
        }   
    
       
    function revenue_lastdays()
        {
        $con=$this->db_connect();
        $query_string="select report_date,report_day,sum(gross_amount) as gross_amount from summary_daily_revenue where report_date between DATE(NOW())-INTERVAL 7 DAY and DATE(NOW())- INTERVAL 1 DAY group by report_date ";
        
        $result=mysqli_query($con,$query_string) or die(mysqli_error($con));
        $response=array();
        while($data=mysqli_fetch_assoc($result)) 
            {
            $temp=[
            "day"=>$data["report_day"],       
            "amount"=>$data["gross_amount"],
            "date"=>$data["report_date"]
            ];
            array_push($response,$temp);
            }
        mysqli_close($con);
        $con=$this->db_connect_dashboard();
        $query_string="select sum(gross_amount) as gross_amount from summary_daily_revenue_live";
        $result=mysqli_query($con,$query_string) or die(mysqli_error($con));
        if($data=mysqli_fetch_assoc($result))
        {
        $temp=[
        "day"=>date('l'),
        "amount"=>$data["gross_amount"],
        "date"=>date("Y-m-d")
        ];
        array_push($response,$temp);
        }
        mysqli_close($con);
        echo json_encode($response);
    
        }
        
    function getFacilityFeatures()
        {
        $data;
        $con=$this->db_connect_config();      
        if($con)
            {  
            $query_string="select setting_name,setting_value from system_settings where setting_name in('currency','decimal_places','vat_percentage','facility_name','address_line1','address_line2','vat_id')";
            $result=mysqli_query($con,$query_string) or die(mysqli_error($con));            
            while($row=mysqli_fetch_assoc($result))
                {
                $data[$row["setting_name"]] =$row["setting_value"] ;  
                }
            mysqli_close($con);
            }
        return $data;    
        }   
    
    function revenue_report($data)
        {    
        $FromDateTime=$data["from"];
        $ToDateTime=$data["to"];
        $carpark=$data["carpark"];
        $weekdays=$data["weekdays"];
        $carpark_displayed_flag=0;

        $fdata=$this->getFacilityFeatures();
        $d=$fdata["decimal_places"];
        $currency=$fdata["currency"]; 
        $vat=$fdata["vat_percentage"];
        $data["currency"]=$currency;
        $data["decimal_places"]=$d;
        $data["vat"]=$vat;
        $con=$this->db_connect();     
        
        $query_string="select device_name,carpark_number,carpark_name,sum(gross_amount) as gross_amount,sum(parking_fee) as parking_fee,sum(lost_ticket_fee) as lost_ticket_fee,"
        . "sum(vat_amount) as vat_amount,sum(product_sale_amount) as product_sale_amount,sum(discount_amount) as
        discount_amount,sum(payable_entries_count) as
        payable_entries_count,sum(lost_ticket_count) as lost_ticket_count,sum(discount_count) as discount_count,sum(creditcard_payment_count) as creditcard_count,".
        "sum(sms_payment_count) as sms_count,sum(creditcard_payment_amount) as creditcard_amount,"
        . "sum(gross_amount-creditcard_payment_amount) as cash_amount,sum(sms_payment_amount) as
        sms_amount,sum(product_sale_count) as product_sale_count from summary_daily_revenue where report_date between
        '".$FromDateTime."'
        AND '".$ToDateTime."'";
        
        if (strlen($carpark) > 0) 		           
            $query_string .= " AND carpark_number IN(".$carpark.")";                       
            
        if (strlen($weekdays) > 0) 		           
            $query_string .= " AND dayname(report_date) IN(".$weekdays.")"; 

        $query_string.=" AND device_number > 0 GROUP BY device_number ORDER BY carpark_number ASC";   
        //echo $query_string."<br>";
        $result=mysqli_query($con,$query_string) or die(mysqli_error($con));               
        if(mysqli_num_rows($result)>0)
            {        
            $this->revenue_report_summary($data);
            $this->revenue_day_closure_comments($data);
            $this->revenue_report_summary_by_date($data);    

            $header.="<div class='header pl-0 mt-4 mb-3'>Earnings</div>";
            $header .= '<table width="100%" class="jspdf-table">';
            $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';

            $header .= "<th class='col'>Device Name</th>";
            $header .= "<th class='col'>Parking Fee</th>";
            $header .= "<th class='col'>Lost Fee</th>";
            $header .= "<th class='col'>Product Sales</th>";
            // if($vat!=0)       
            //     $header .= "<th class='col'>VAT</th>";
            $header .= "<th class='col'>Total Earnings</th>";
            $header .= '</tr>';

            $html_data = $header;
            $html_data .= '<tbody class="table-view">';

            while($data=mysqli_fetch_assoc($result)) 
                {           
                if($carpark != $data['carpark_number']) 
                    {
                    $carpark=$data['carpark_number'];
                    $carpark_displayed_flag=0;
                    }
        
                if(($carpark != 0) && $carpark_displayed_flag ==0)
                    {
                    $html_data .= '<tr class="card-text subhead d-flex justify-content-between align-items-center">';
                    $html_data .="<td class='col'>".$data['carpark_name']."</td>";
                    $html_data .="</tr>";
                    $carpark_displayed_flag=1;
                    }  

                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
                $html_data .= "<td class='col'>" . $data['device_name'] . " </td>";
                $html_data .= "<td class='col'>" . number_format($data['parking_fee'],$d) . " </td>";
                $html_data .= "<td class='col'>" . number_format($data['lost_ticket_fee'],$d) . " </td>";
                $html_data .= "<td class='col'>" . number_format($data['product_sale_amount'],$d) . " </td>";   
                // if($vat!=0)               
                //     $html_data .= "<td class='col'>" . number_format($data['vat_amount'],$d) . " </td>";    
                $html_data .= "<td class='col'>" . number_format($data['gross_amount'],$d) . " </td>";
                $html_data .= '</tr>';
                }

            $html_data .= '</table>';
            echo $html_data;
            /////////////////////////
            //Earning by payment type
            ////////////////////////
            $html_data = "";
            $header ="";
            $carpark_displayed_flag=0;
            $header.="<div class='header pl-0 mt-4 mb-3'>Earnings By Payment Type</div>";
            $header .= '<table width="100%" class="jspdf-table">';
            $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';

            $header .= "<th class='col'>Device Name</th>";
            $header .= "<th class='col'>Cash</th>";
            $header .= "<th class='col'>Credit Card</th>";
            $header .= "<th class='col'>SMS</th>";        
            $header .= '</tr>';

            $html_data = $header;
            $html_data .= '<tbody class="table-view">';
            mysqli_data_seek($result,0);
            while($data=mysqli_fetch_assoc($result)) 
                {
                // carpark header
                if($carpark != $data['carpark_number']) 
                    {
                    $carpark=$data['carpark_number'];
                    $carpark_displayed_flag=0;
                    }

                if(($carpark != 0) && $carpark_displayed_flag ==0)
                    {
                    $html_data .= '<tr class="card-text subhead d-flex justify-content-between align-items-center">';
                    $html_data .="<td class='col'>".$data['carpark_name']."</td>";
                    $html_data .="</tr>";
                    $carpark_displayed_flag=1;
                    }
                // end carpark header

                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
                $html_data .= "<td class='col'>" . $data['device_name'] . " </td>";
                $html_data .= "<td class='col'>" . number_format($data['cash_amount'],$d) . " </td>";
                $html_data .= "<td class='col'>" . number_format($data['creditcard_amount'],$d) . " </td>";
                $html_data .= "<td class='col'>" . number_format($data['sms_amount'],$d) . " </td>";       
                $html_data .= '</tr>';
                }
            $html_data .= '</table>';
            echo $html_data;


            ////////////////////////
            //Prodct sale report
            ////////////////////////
            $this->product_sale_report($FromDateTime, $ToDateTime, $carpark,$weekdays,$d);



            //////////////////////
            //Discounts report
            ///////////////////
            $this->discount_usage_summary($FromDateTime, $ToDateTime, $carpark,$weekdays,$d);
            ////////////////////////
            //Transaction Count
            ////////////////////////
            $html_data="";
            $header ="";
            $carpark_displayed_flag=0;
            $header.="<div class='header pl-0 mt-4 mb-3'>Transaction Count</div>";
            $header .= '<table width="100%" class="jspdf-table">';
            $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';

            $header .= "<th class='col'>Device Name</th>";
            $header .= "<th class='col'>Payable Entries</th>";        
            $header .= "<th class='col'>Lost Ticket</th>";
            $header .= "<th class='col'>Discounts </th>";
            $header .= "<th class='col'>ProductSales</th>";
            $header .= "<th class='col'>Cash</th>";
            $header .= "<th class='col'>Credit card</th>";
            $header .= '</tr>';

            $html_data = $header;
            $html_data .= '<tbody class="table-view">';
            mysqli_data_seek($result,0);
            while($data=mysqli_fetch_assoc($result))
                {            
                if($carpark != $data['carpark_number']) 
                    {
                    $carpark=$data['carpark_number'];
                    $carpark_displayed_flag=0;
                    }
                if(($carpark != 0) && $carpark_displayed_flag ==0)
                    {
                    $html_data .= '<tr class="card-text subhead d-flex justify-content-between align-items-center">';
                    $html_data .="<td class='col'>".$data['carpark_name']."</td>";
                    $html_data .="</tr>";
                    $carpark_displayed_flag=1;
                    }

                
                $cashcount=$data['payable_entries_count']-$data['creditcard_count'];
                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
                $html_data .= "<td class='col'>" . $data['device_name'] . " </td>";
                $html_data .= "<td class='col'>" . $data['payable_entries_count'] . " </td>";           
                $html_data .= "<td class='col'>" . $data['lost_ticket_count'] . " </td>";
                $html_data .= "<td class='col'>" . $data['discount_count'] . " </td>";
                $html_data .= "<td class='col'>" . $data['product_sale_count'] . " </td>";
                $html_data .= "<td class='col'>" . $cashcount . " </td>";
                $html_data .= "<td class='col'>" . $data['creditcard_count'] . " </td>";
                $html_data .= '</tr>';
                }
            $html_data .= '</table>';
            echo $html_data;
            mysqli_close($con);    
            }
        else 
            {
            $html_data="<div class='card'><div class='p-3'>No records/transactions available for the current search criteria</div></div>";
            echo $html_data;
            }
        }// End . Revenue Summary  

    function product_sale_report($FromDateTime, $ToDateTime, $carpark,$weekdays,$d)
        {
        $con=$this->db_connect();     
        $query_string="select device_name,product_name,MIN(carpark_number) as carpark_number,carpark_name,sum(product_quantity) as product_quantity,unit_price,sum(product_sale_amount) as product_sale from revenue_product_usage where report_date between '".$FromDateTime."'AND '".$ToDateTime."'";
        
        if (strlen($carpark) > 0) 		           
            $query_string .= " AND carpark_number IN(".$carpark.")";                       
            
        if (strlen($weekdays) > 0) 		           
            $query_string .= " AND dayname(report_date) IN(".$weekdays.")";  
            
        $query_string.=" GROUP BY device_name,product_id ORDER BY carpark_number ASC";
    
        $result=mysqli_query($con,$query_string) or die(mysqli_error($con));
        $html_data="<div class='header pl-0 mt-4 mb-3'>Product Sales</div>";
        if(mysqli_num_rows($result)>0)
            {
            $header ="";
            $header.=$html_data;
            $carpark_displayed_flag=0;        
            $header .= '<table width="100%" class="jspdf-table">';
            $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';

            $header .= "<th class='col'>Device Name</th>";
            $header .= "<th class='col'>Product Name</th>";
            $header .= "<th class='col'>Quantity</th>";
            $header .= "<th class='col'>Unit Price</th>";        
            $header .= "<th class='col'>Total Amount</th>";
            $header .= '</tr>';

            $html_data = $header;
            $html_data .= '<tbody class="table-view">';
            mysqli_data_seek($result,0);
            while($data=mysqli_fetch_assoc($result))
                {
                // carpark header
                if($carpark != $data['carpark_number']) 
                    {
                    $carpark=$data['carpark_number'];
                    $carpark_displayed_flag=0;
                    }

                if(($carpark != 0) && $carpark_displayed_flag ==0)
                    {
                    $html_data .= '<tr class="card-text subhead d-flex justify-content-between align-items-center">';
                    $html_data .="<td class='col'>".$data['carpark_name']."</td>";
                    $html_data .="</tr>";
                    $carpark_displayed_flag=1;
                    }

            
                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
                $html_data .= "<td class='col'>" . $data['device_name'] . " </td>";
                $html_data .= "<td class='col'>" . $data['product_name'] . " </td>";
                $html_data .= "<td class='col'>" . $data['product_quantity'] . " </td>";
                $html_data .= "<td class='col'>" . number_format($data['unit_price'],$d) . " </td>";           
                $html_data .= "<td class='col'>" . number_format($data['product_sale'],$d) . " </td>";                        
                $html_data .= '</tr>';
                }
            $html_data .= '</table>';
            }
        else
            {
            $html_data.="<div class='card table-view'><div class='p-3'>No Product Sales In This Time Period</div></div>";
            }
        echo $html_data;
        }
    

        //Discount_usage

    function discount_usage_summary($FromDateTime,$ToDateTime,$carpark,$weekdays)
        {
        if($ToDateTime>=date("Y-m-d"))
            $ToDateTime=date("Y-m-d",strtotime("-1 days"))." ".DAY_CLOSURE_END;
        else
            $ToDateTime=$ToDateTime." ".DAY_CLOSURE_END;

        $FromDateTime=$FromDateTime." ".DAY_CLOSURE_START;

        $con=$this->db_connect();    
        $query_string="select device_name,discount_name,MIN(carpark_number) as carpark_number,carpark_name,count(id) as discount_usage,sum(discount_amount) as discount_amount from revenue_payments where discount_amount > 0  and payment_date_time between '".$FromDateTime."'AND '".$ToDateTime."'";
        
        if (strlen($carpark) > 0) 		           
            $query_string .= " AND carpark_number IN(".$carpark.")";                       
            
        if (strlen($weekdays) > 0) 		           
            $query_string .= " AND dayname(payment_date_time) IN(".$weekdays.")";
        

        $query_string.=" AND device_number > 0 GROUP BY device_name,discount_id ORDER BY carpark_number ASC";
        
        $result=mysqli_query($con,$query_string) or die(mysqli_error($con)); 
        $html_data="<div class='header pl-0 mt-4 mb-3'>Discounts</div>";
        if(mysqli_num_rows($result)>0)
            {
            $header ="";
            $header.=$html_data;
            $carpark_displayed_flag=0;
            $header .= '<table width="100%" class="jspdf-table">';
            $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';
            $header .= "<th class='col'>Device Name</th>";
            $header .= "<th class='col'>Discount Name</th>";
            $header .= "<th class='col'>Discount Count</th>";            
            $header .= "<th class='col'>Discount Amount</th>";
            $header .= '</tr>';

            $html_data = $header;
            $html_data .= '<tbody class="table-view">';
            mysqli_data_seek($result,0);
            while($data=mysqli_fetch_assoc($result))
                {
                // carpark header
                if($carpark != $data['carpark_number']) 
                    {
                    $carpark=$data['carpark_number'];
                    $carpark_displayed_flag=0;
                    }

                if(($carpark != 0) && $carpark_displayed_flag ==0)
                    {
                    $html_data .= '<tr class="card-text subhead d-flex justify-content-between align-items-center">';
                    $html_data .="<td class='col'>".$data['carpark_name']."</td>";
                    $html_data .="</tr>";
                    $carpark_displayed_flag=1;
                    }
            
                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
                $html_data .= "<td class='col'>" . $data['device_name'] . " </td>";
                $html_data .= "<td class='col'>" . $data['discount_name'] . " </td>";
                $html_data .= "<td class='col'>" . $data['discount_usage'] . " </td>";
                $html_data .= "<td class='col'>" . number_format($data['discount_amount'],$d) . " </td>";
                $html_data .= '</tr>';
                }
            $html_data .= '</table>';
            }
        else
            {
            $html_data.="<div class='card table-view'><div class='p-3'>No Discounts In This Time Period</div></div>";
            }
        echo $html_data;
        }

    function revenue_report_summary($data)
        {
        $con=$this->db_connect(); 
        $FromDateTime=$data["from"];
        $ToDateTime=$data["to"];
        $carpark=$data["carpark"];
        $weekdays=$data["weekdays"];
        $d=$data["decimal_places"];
        $currency=$data["currency"]; 
        $vat=$data["vat"];

        $query_string="select sum(parking_fee) as parking_fee,sum(lost_ticket_fee) as lost_fee,sum(gross_amount) as earnings,sum(vat_amount) as vat,sum(product_sale_amount) as product_sale,sum(discount_amount) as discount_amount from summary_daily_revenue where report_date between '".$FromDateTime."' AND '".$ToDateTime."'";

        if (strlen($carpark) > 0) 		           
                $query_string .= " AND carpark_number IN(".$carpark.")";                       
            
        if (strlen($weekdays) > 0) 		           
            $query_string .= " AND dayname(report_date) IN(".$weekdays.")"; 


        $query_string.=" AND device_number > 0 ORDER BY device_number ASC";


        $result=mysqli_query($con,$query_string) or die(mysqli_error($con));    

        $html_data="<br>";
        $data_summary=mysqli_fetch_assoc($result);    
        
        

        // data for chart

        $html_data.='<span id="chart-data" class="d-none" data-currency='.$currency.' data-parking-fee="'.number_format($data_summary["parking_fee"],$d,'.','').'" data-lost-ticket="'.number_format($data_summary["lost_fee"],$d,'.','').'" data-product-sale="'.number_format($data_summary["product_sale"],$d,'.','').'" data-vat="'.number_format($data_summary["vat"],$d,'.','').'"></span>';

        $html_data.='<div class="row mb-4 jspdf-graph">';
        $html_data.='<div class="col-auto">';    
        $html_data.='<div class="small-box bg-success">';
        $html_data.='<div class="inner">';
        $html_data.='<h3>'.$currency." ".number_format($data_summary["earnings"],$d).'</h3>';
        $html_data.='<p>Total Revenue</p>';
        $html_data.='</div>';
        $html_data.='<div class="icon">';
        $html_data.='<i class="ion ion-stats-bars"></i>';
        $html_data.='</div>';		
        $html_data.='</div>';
        $html_data.='</div>';


        $html_data.='<div class="col-auto">';		
        $html_data.='<div class="small-box box-color-parking-fee">';
        $html_data.='<div class="inner">';
        $html_data.='<h3>'.$currency." ".number_format($data_summary["parking_fee"],$d).'</h3>';
        $html_data.='<p>Parking Fee</p>';
        $html_data.='</div>';
        $html_data.='<div class="icon">';
        $html_data.='<i class="ion ion-stats-bars"></i>';
        $html_data.='</div>';		
        $html_data.='</div>';
        $html_data.='</div>';

        $html_data.='<div class="col-auto">';    
        $html_data.='<div class="small-box box-color-lost-fee">';
        $html_data.='<div class="inner">';
        $html_data.='<h3>'.$currency." ".number_format($data_summary["lost_fee"],$d).'</h3>';
        $html_data.='<p>Lost Ticket Fee</p>';
        $html_data.='</div>';
        $html_data.='<div class="icon">';
        $html_data.='<i class="ion ion-stats-bars"></i>';
        $html_data.='</div>';		
        $html_data.='</div>';
        $html_data.='</div>';

        $html_data.='<div class="col-auto">';		
        $html_data.='<div class="small-box box-color-product-fee">';
        $html_data.='<div class="inner">';
        $html_data.='<h3>'.$currency." ".number_format($data_summary["product_sale"],$d).'</h3>';
        $html_data.='<p>Product Sale Amount</p>';
        $html_data.='</div>';
        $html_data.='<div class="icon">';
        $html_data.='<i class="ion ion-stats-bars"></i>';
        $html_data.='</div>';		
        $html_data.='</div>';
        $html_data.='</div>';

        // if($vat!=0)
        //     {
        //     $html_data.='<div class="col-auto">';		
        //     $html_data.='<div class="small-box bg-success">';
        //     $html_data.='<div class="inner">';
        //     $html_data.='<h3>'.$currency." ".number_format($data_summary["vat"],$d).'</h3>';
        //     $html_data.='<p>VAT</p>';
        //     $html_data.='</div>';
        //     $html_data.='<div class="icon">';
        //     $html_data.='<i class="ion ion-stats-bars"></i>';
        //     $html_data.='</div>';
        //     $html_data.='</div>';
        //     $html_data.='</div>';
        //     }

        $html_data.='<div class="col-auto">';
        $html_data.='<!-- small box -->';
        $html_data.='<div class="small-box box-color-discount-fee">';
        $html_data.='<div class="inner">';
        $html_data.='<h3>'.$currency." ".number_format($data_summary["discount_amount"],$d).'</h3>';
        $html_data.='<p>Discount Amount</p>';
        $html_data.='</div>';
        $html_data.='<div class="icon">';
        $html_data.='<i class="ion ion-stats-bars"></i>';
        $html_data.='</div>';
        $html_data.='</div>';
        $html_data.='</div>';

        $html_data.='</div><!-- Row End -->';
        $html_data.='</div>';


        mysqli_close($con);

        echo $html_data;

        } // End Function 
    
    function revenue_day_closure_comments($data)
        {
        $FromDateTime=$data["from"];
        $ToDateTime=$data["to"];
        $carpark=$data["carpark"];
        $weekdays=$data["weekdays"];
        $con=$this->db_connect(); 

        $query_string="select dayname(report_date) as day,report_date,comments,carpark_name,device_name from summary_daily_revenue where report_date between '".$FromDateTime."' AND '".$ToDateTime."'";
        if (strlen($carpark) > 0) 		           
                $query_string .= " AND carpark_number IN(".$carpark.")";                       
            
        if (strlen($weekdays) > 0) 		           
            $query_string .= " AND dayname(report_date) IN(".$weekdays.")"; 

        $query_string.=" AND reproccessing_flag=1 ORDER BY device_number ASC";


        $result=mysqli_query($con,$query_string) or die(mysqli_error($con));   

        

        if(mysqli_num_rows($result) > 0)
            {        
            $header.="<div class='header pl-0 mt-4 mb-3'>Notifications</div>";
            $header .= '<table width="100%" class="jspdf-table ">';
            $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';            
            $header.="<th class='col'>Report Date</th>";
            $header.="<th class='col'>Device Name</th>";
            $header.="<th class='col'>Carpark</th>";
            $header.="<th class='col'>Comments</th>";
            $header.="</tr>";
            $header.="<tbody class='table-view'>";

            $html_data=$header;

            while($data=mysqli_fetch_assoc($result))
                {    
                $html_data.="<tr class='card-text d-flex justify-content-between align-items-center'>";
                $html_data.="<td class='col'> ".$data['report_date']."<small class='form-text text-muted'>".$data['day']."</small></td>";
                $html_data.="<td class='col'>".$data['device_name']."</td>";
                $html_data.="<td class='col'>".$data['carpark_name']."</td>";
                $html_data.="<td class='col'>".$data['comments']."</td>";
                $html_data.="</tr>";  

                }//wend
            $html_data.="</tbody>";
            $html_data.="</table>";  
            $html_data.="</div>";
            mysqli_close($con);
            echo $html_data;
            }//end if 
        } //end function 

    function revenue_report_summary_by_date($data)
        {
        $con=$this->db_connect(); 
        $FromDateTime=$data["from"];
        $ToDateTime=$data["to"];
        $carpark=$data["carpark"];
        $weekdays=$data["weekdays"];
        $d=$data["decimal_places"];
        $currency=$data["currency"]; 
        $vat=$data["vat"];
        $query_string="select report_date,dayname(report_date) as day,sum(gross_amount) as gross_amount,sum(parking_fee) as parking_fee,sum(lost_ticket_fee) as lost_ticket_fee,"
        . "sum(vat_amount) as vat_amount,sum(product_sale_amount) as product_sale_amount,sum(discount_amount) as
        discount_amount,sum(payable_entries_count) as
        payable_entries_count,sum(lost_ticket_count) as lost_ticket_count,sum(discount_count) as discount_count,sum(creditcard_payment_count) as creditcard_count,".
        "sum(sms_payment_count) as sms_count,sum(creditcard_payment_amount) as creditcard_amount,"
        . "sum(gross_amount-creditcard_payment_amount) as cash_amount,sum(sms_payment_amount) as
        sms_amount,sum(product_sale_count) as product_sale_count from summary_daily_revenue where report_date between
        '".$FromDateTime."'
        AND '".$ToDateTime."'";
        
        if (strlen($carpark) > 0) 		           
            $query_string .= " AND carpark_number IN(".$carpark.")";                       
            
        if (strlen($weekdays) > 0) 		           
            $query_string .= " AND dayname(report_date) IN(".$weekdays.")"; 

        $query_string.=" GROUP BY report_date ORDER BY report_date DESC";   
        //echo $query_string."<br>";
        $result=mysqli_query($con,$query_string) or die(mysqli_error($con));               
        if(mysqli_num_rows($result)>0)
            {      

            $header.="<div class='header pl-0 mt-4 mb-3'>Report By Date</div>";
            $header .= '<table width="100%" class="jspdf-table">';
            $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';
            $header .= "<th class='col'>Date</th>";
            $header .= "<th class='col'>Total Earnings</th>";
            $header .= "<th class='col'>Parking Fee</th>";
            $header .= "<th class='col'>Lost Fee</th>";
            $header .= "<th class='col'>Product Sales Amount</th>";        
            // if($vat!=0)       
            //     $header .= "<th class='col'>VAT</th>";        

            $header .= "<th class='col'>Cash Amount</th>";
            $header .= "<th class='col'>Credit Card Amount</th>";
            $header .= "<th class='col'>Wallet Amount</th>";                               

            $header .= "<th class='col'>Payable Entries Count</th>";        
            $header .= "<th class='col'>Lost Ticket Count</th>";
            $header .= "<th class='col'>Discounts Count</th>";
            $header .= "<th class='col'>ProductSales Count</th>";
            $header .= "<th class='col'>Cash Transaction Count</th>";
            $header .= "<th class='col'>Credit card Transaction Count</th>";  

            $header .= '</tr>';

            $html_data = $header;
            $html_data .= '<tbody class="table-view">';

            while($data=mysqli_fetch_assoc($result)) 
                {                        
                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
                $html_data .= "<td class='col'>" . $data['report_date'] . "<small class='form-text text-muted'>".$data['day']."</small> </td>";
                $html_data .= "<td class='col'>" . number_format($data['gross_amount'],$d) . " </td>";           
                $html_data .= "<td class='col'>" . number_format($data['parking_fee'],$d) . " </td>";
                $html_data .= "<td class='col'>" . number_format($data['lost_ticket_fee'],$d) . " </td>";
                $html_data .= "<td class='col'>" . number_format($data['product_sale_amount'],$d) . " </td>";                                         
                $html_data .= "<td class='col'>" . number_format($data['cash_amount'],$d) . " </td>";
                $html_data .= "<td class='col'>" . number_format($data['creditcard_amount'],$d) . " </td>";
                $html_data .= "<td class='col'>" . number_format($data['sms_amount'],$d) . " </td>";                   
                
                $cashcount=$data['payable_entries_count']-$data['creditcard_count'];
                $html_data .= "<td class='col'>" . $data['payable_entries_count'] . " </td>";           
                $html_data .= "<td class='col'>" . $data['lost_ticket_count'] . " </td>";
                $html_data .= "<td class='col'>" . $data['discount_count'] . " </td>";
                $html_data .= "<td class='col'>" . $data['product_sale_count'] . " </td>";
                $html_data .= "<td class='col'>" . $cashcount . " </td>";
                $html_data .= "<td class='col'>" . $data['creditcard_count'] . " </td>";
                $html_data .= '</tr>';
                }

            $html_data .= '</table>';
            echo $html_data;        
            }
            mysqli_close($con);
        }    


    function apm_payout_report($data)
        {
        $startdate=$data["from"];
        $enddate=$data["to"];
        $device=$data["device"];
        $carpark=$data["carpark"];  
            
        $date_from= new DateTime($startdate);
        $date_to=new DateTime($enddate);
        $current_date=date("Y-m-d H:i:s");
    
        $query_string="select * from revenue_apm_cash_payout where datetime between '".$startdate."' AND '".$enddate."'";       
        if (strlen($carpark) > 0) 		           
            $query_string .= " AND carpark_number IN(".$carpark.")";   
  
        if (strlen($device) > 0) 		           
            $query_string .= " AND device_number IN(".$device.")";          
        
        $con=$this->db_connect();
        
        $result=mysqli_query($con,$query_string) or die(mysqli_error($con));
        $total_count= mysqli_num_rows($result);
        
        if(mysqli_num_rows($result) > 0)
            {
            $fdata=$this->getFacilityFeatures();
            $d=$fdata["decimal_places"];
            $currency=$fdata["currency"]; 
            
            $html_data="";
        

            $header .= '<table width="100%" class="jspdf-table">';
            $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';       
            $header.="<th class='col'>Device Name</th>";
            $header.="<th class='col'>Operation</th>";		
            $header.="<th class='col'>DateTime</th>";
            $header.="<th class='col'>Operator</th>";
            $header.="<th class='col'>Banknote*Quantity</th>";        
            $header.="<th class='col'>Amount</th>";
            $header.="</tr>";
            $header.="<tbody class='table-view'>";
            $html_data=$header;
            $total=0;
            while($data=mysqli_fetch_assoc($result))
                {			
                $notes=$data['notes_payout'];
                $notes=substr($notes,0,-1);				
                $notes=str_replace(";",",",$notes);
                $array=explode(",",$notes);
                $notes="";
                for($i=0;$i<count($array);$i+=2)
                    {
                    $a=$array[$i];
                    $b=$array[$i+1];
                    $sum=$a." ".$currency." x ".$b." = ".($a*$b)." ".$currency."";
                    if(count($array)>0)
                        $notes=$sum."<br>".$notes;
                    }
                
                $html_data.="<tr class='card-text d-flex justify-content-between align-items-center'>";                       
                $html_data.="<td class='col'> ".$data['device_name']." </td>";
                $html_data.="<td class='col'>".$data['operation']."</td>";
                $html_data.="<td class='col'>".$data['datetime']."</td>";
                $html_data.="<td class='col'>".$data['user']."</td>";
                $html_data.="<td class='col'>".$notes."</td>";
                $html_data.="<td class='col'> ".$data['total_payout_amount']." ".$currency."</td>";           
                $html_data.="</tr>";
                $total=$total+$data['total_payout_amount'];
            
                }          
            $html_data.="</table>"; 
            $summary="";
            $summary .= '<div class="row mb-4 jspdf-graph">';

            $summary .= '<div class="col-lg-3 col-6">';
            $summary .= '<!-- small box -->';
            $summary .= '<div class="small-box bg-success">';
            $summary .= '<div class="inner">';           
            $summary.='<h3>'.$currency." ".number_format($total,$d).'</h3>';
            $summary .= '<h6>Total Amount Payout</h6>';
            $summary .= '</div>';
            $summary .= '<div class="icon">';
            $summary .= '<i class="ion ion-stats-bars"></i>';
            $summary .= '</div>';       
            $summary .= '</div>';
            
            $summary .= '</div></div>';

            }
        else
            {
            $html_data="<div class='p-3'>No records/transactions available for the current search criteria</div>";
            }
        
        mysqli_close($con);      
        echo $summary.$html_data;                
        } 


    function apm_refill_report($data)
        {        
        $startdate=$data["from"];
        $enddate=$data["to"];
        $device=$data["device"];
        $carpark=$data["carpark"];  

        $query_string="select * from revenue_apm_cash_refill where datetime between '".$startdate."' AND '".$enddate."'";       
        if (strlen($carpark) > 0) 		           
            $query_string .= " AND carpark_number IN(".$carpark.")";   
  
        if (strlen($device) > 0) 		           
            $query_string .= " AND device_number IN(".$device.")";  
            
        //echo $query_string;
        
        $con=$this->db_connect();
        
        $result=mysqli_query($con,$query_string) or die(mysqli_error($con));
        $total_count= mysqli_num_rows($result);
        
        if(mysqli_num_rows($result) > 0)
            { 
            $fdata=$this->getFacilityFeatures();
            $d=$fdata["decimal_places"];
            $currency=$fdata["currency"]; 

            $html_data="";        

            $header .= '<table width="100%" class="jspdf-table">';
            $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';       
            $header.="<th class='col'>Device Name</th>";		
            $header.="<th class='col'>DateTime</th>";
            $header.="<th class='col'>Operator</th>";
            $header.="<th class='col'>Banknote*Quantity</th>";        
            $header.="<th class='col'>Amount</th>";
            $header.="</tr>";
            $header.="<tbody class='table-view'>";
            $html_data=$header;
            $total=0;
            while($data=mysqli_fetch_assoc($result))
                {
                $notes=$data['notes_refilled'];
                $notes=substr($notes,0,-1);				
                $notes=str_replace(";",",",$notes);
                $array=explode(",",$notes);
                $notes="";
                for($i=0;$i<count($array);$i+=2)
                    {
                    $a=$array[$i];
                    $b=$array[$i+1];
                    $sum=$a." ".$currency." x ".$b." = ".($a*$b)." ".$currency;
                    if(count($array)>0)
                        $notes=$sum."<br>".$notes;
                    }
                                    
                $html_data.="<tr class='card-text d-flex justify-content-between align-items-center'>";                       
                $html_data.="<td class='col'> ".$data['device_name']." </td>";
                $html_data.="<td class='col'>".$data['datetime']."</td>";
                $html_data.="<td class='col'>".$data['user']."</td>";
                $html_data.="<td class='col'>".$notes."</td>";
                $html_data.="<td class='col'> ".$data['total_amount_refilled']." ".$currency."</td>";           
                $html_data.="</tr>";  
                $total=$total+$data['total_amount_refilled'];
            
                }          
            $html_data.="</table>";
            
            
            
            $summary="";
            $summary .= '<div class="row mb-4 jspdf-graph">';

            $summary .= '<div class="col-lg-3 col-6">';
            $summary .= '<!-- small box -->';
            $summary .= '<div class="small-box bg-success">';
            $summary .= '<div class="inner">';
            $summary .= '<h3>'.$currency." ".number_format($total,$d). '</h3>';           
            $summary .= '<h6>Total Amount refilled</h6>';
            $summary .= '</div>';
            $summary .= '<div class="icon">';
            $summary .= '<i class="ion ion-stats-bars"></i>';
            $summary .= '</div>';       
            $summary .= '</div>';
            
            $summary .= '</div></div>';

            }
        else        
            $html_data="<div class='p-3'>No records/transactions available for the current search criteria</div>";                    
        
        mysqli_close($con);          
        echo $summary.$html_data;
                
        } 

    
    function get_device_name($device_id)
        { 
            $con=$this->db_connect_config();       
            $query="select device_name from system_devices where device_number=".$device_id;       
            $result=mysqli_query($con,$query) or die(mysqli_error($con));
            
            if(mysqli_num_rows($result) > 0)
            {     
            $data=mysqli_fetch_assoc($result);
            $name=$data['device_name'];
            }//endif 
            else
            {            
                $name= "unknown";
            }
        
            mysqli_close($con);
            return $name;
        } // end get deivce name
            

    function get_product_name($product_id)
        { 
            $con=$this->db_connect_config();       
            $query="select product_name from revenue_products where id=".$product_id;
            $result=mysqli_query($con,$query) or die(mysqli_error($con));
            
            if(mysqli_num_rows($result) > 0)
                {     
                $data=mysqli_fetch_assoc($result);
                $name=$data['product_name'];
                }//endif 
            else
                {             
                $name= "unknown";
                }       
            mysqli_close($con);
            return $name;
        } // end get_product_name

        function get_product_unit_price($product_id)
        { 
            $con=$this->db_connect_config();       
            $query="select unit_price from revenue_products where id=".$product_id;
            $result=mysqli_query($con,$query) or die(mysqli_error($con));
            
            if(mysqli_num_rows($result) > 0)
                {     
                $data=mysqli_fetch_assoc($result);
                $name=$data['unit_price'];
                }//endif 
            else
                {             
                $name= "unknown";
                }       
            mysqli_close($con);
            return $name;
        } // end get_product_name

        
    function revenue_payment_transactions($data)
        {
        $startdate=$data["from"];
        $enddate=$data["to"];
        $device=$data["device"];
        $carpark=$data["carpark"];
        $type=$data["payment-type"];
        $category=$data["payment-category"];
        $discount=$data["discount"];   
        $showvoid=$data["showvoid"];   
            
        $query_string="select * from revenue_payments where payment_date_time between '".$startdate."' AND '".$enddate."'";     
        
        if($showvoid ==0)    
            $query_string.=" and (void is null or void='')";      
        else      
            $query_string.=" and void is not null";     
        
        if (strlen($carpark) > 0) 		           
            $query_string .= " AND carpark_number IN(".$carpark.")";   
        
        if (strlen($device) > 0) 		           
            $query_string .= " AND device_number IN(".$device.")";  
            
        if (strlen($type) > 0) 		           
            $query_string .= " AND payment_type IN(".$type.")";

        //category
        //0->all 1->parking Fee 2->Lost 3->discount 4->grace period 5->product sale 
        if(count($category) > 0) 
            {
            for($i=0; $i < count($category);$i++)
                {
                if($i==0)            
                    $query_string.=" and (";        
                else
                    $query_string.=" Or ";
                
                switch($category[$i])
                    {
                    case 1:$query_string.=" (parking_fee>0 and lost_fee=0 and discount_amount=0)";
                        break;
                    case 2: $query_string.=" lost_fee>0";
                        break;
                    case 3: $query_string.=" discount_amount>0";
                        break;
                    case 4: $query_string.=" (discount_amount=0 AND gross_amount=0)";
                        break;
                    case 5: $query_string.=" payment_category='ProductSale'";
                        break;
                    }
                }
            $query_string.=" )";
            }    
        
        if (strlen($discount) > 0) 		           
            $query_string .= " AND discount_id IN(".$discount.")";

        //echo $query_string;
        
        $con=$this->db_connect();     
        $result=mysqli_query($con,$query_string) or die(mysqli_error($con));
        $total_count= mysqli_num_rows($result);
        $count=0;
        $total_revenue=0;
        $total_vat=0;
        
        if(mysqli_num_rows($result) > 0)
            { 
            $fdata=$this->getFacilityFeatures();
            $d=$fdata["decimal_places"];
            $currency=$fdata["currency"]; 
            $vat=$fdata["vat_percentage"];   
            

            //$header .= '<div class="card-body p-0">';
            //$header .= '<div class="card card-table-blue">';
            //$header .= '<table width="100%" class="table table-blue table-hover jspdf-table">';
            $header.='<table id="RecordsTable" class="table table-blue table-bordered table-striped jspdf-table"> ';
            $header .= '<thead>';
            $header .= '<tr>';
            $header .= '<th>#</th>';
            $header .= '<th>Ticket Number</th>';
            $header .= '<th>Device Name</th>';       
            $header .= '<th>Payment Time</th>'; 
            $header .= '<th>Parking Duration</th>'; 
            $header .= '<th>Category</th>';   
            $header .= '<th>Payment Type</th>'; 
            $header .= '<th>Discount Name</th>';                                              
            $header .= '<th>Discount Amount</th>';
            $header .= '<th>Gross Amount</th>';                                                       
            
            $header .= '</tr>';
            $header .= '</thead>';
            
            $html_data .= $header;
            $html_data .= '<tbody>';

            $category="";
            while($data=mysqli_fetch_assoc($result)) 
                {
                $count++;
                if($data['discount_amount']>0)
                    $category="Discount";
                else if($data['lost_fee']>0)
                    $category="Lost";
                else if($data['discount_amount']==0 &&$data['gross_amount']==0)
                    $category="Grace Period";
                else if($data['payment_category']=="ProductSale")
                    $category="Product Sale";
                else if($data['payment_category']=="WALLET")                    
                    $category="Wallet";  
                else if($data['payment_category']=="Reservation")                    
                    $category="Reservation";                   
                else                     
                    $category="Parking Fee";

                // if($data['discount_amount']>0)
                //     $category="Discount";
                // else if($data['lost_fee']>0)
                //     $category="Lost";
                // else if($data['discount_amount']==0 &&$data['gross_amount']==0)
                //     $category="Grace Period";
                // else if($data['payment_category']=="ProductSale")
                //     $category="Product Sale";
                // else    
                //     $category="Parking Fee";
                $html_data .= '<tr class="pointer"  id="payment_detail" payment_id="'.$data['id'].'">';            
                $html_data.="<td>$count</td>";

                if($showvoid == 0) 
                    {
                    $html_data .= "<td>" . $data['ticket_id'] . " </td>";
                    $html_data .= "<td>" . $data['device_name'] . " </td>";
                    } 
                else 
                    {
                    $html_data .= "<td>" . $data['ticket_id'] . "<br>Void Reason:".$data['void_reason']."</td>";
                    $html_data .= "<td>" . $data['device_name'] . " <br>User:".$data['void_user']."</td>";
                    }

            
                $html_data .= "<td >" . $data['payment_date_time'] . " </td>";           
                $html_data .= "<td >" . $data['parking_duration'] . " </td>"; 
                $html_data .= "<td>" . $category. " </td>";                         
                $html_data .= "<td>" . $data['payment_type'] . " </td>";                      
                $html_data .= "<td>" . $data['discount_name'] . " </td>";
                $html_data .= "<td>" . number_format($data['discount_amount'],$d) . " </td>";
                $html_data .= "<td>" . number_format($data['gross_amount'],$d) . " </td>";                                                                      
                        
                if(($_SESSION["userRollName"]=="Admin"||$_SESSION["userRollName"]=="CarParkManager")&& $showvoid ==0) 
                    {
                    //$html_data .= "<td><input class='btn btn-info btn-void-transaction'  type='button'  value='Void' data-id='".$data['id']."' ></td>";
                    }            
            
                //$html_data .= "<td><input class='btn btn-info btn-show-pdf-reciept' value='PDF Receipt' type='button' data-id='".$data['id']."'></td>";
            
                $html_data .= '</tr>';

                $total_vat = $total_vat + $data['vat_amount'];
                $total_revenue = $total_revenue + $data['gross_amount'];            
                } 

            $html_data .= '</tbody>';
            $html_data .= '</table>';
            //$html_data .= '</div>';
            //$html_data .= '</div>';

            $summary .= '<div class="row mb-4 jspdf-graph">';
            $summary .= '<div class="col-lg-3 col-6">';       
            $summary .= '<div class="small-box bg-success">';
            $summary .= '<div class="inner">';
            $summary .= '<h3>'.$currency." ".number_format($total_revenue,$d). '</h3>';
            $summary .= '<h6>Total Revenue</h6>';
            $summary .= '</div>';             
            $summary .= '</div>';        
            $summary .= '</div>';            
            
            $summary .= '<div class="col-lg-3 col-6">';       
            $summary .= '<div class="small-box bg-success">';
            $summary .= '<div class="inner">';
            $summary .= '<h3> ' .$count. '</h3>';
            $summary .= '<h6>Total Transactions</h6>';
            $summary .= '</div>';                    
            $summary .= '</div>';
            $summary .= '</div>';        
            $summary .= '</div><!-- Row End -->';
            }
        else      
            {
            $html_data="<div class='p-0'>No records/transactions available for the current search criteria</div>";
            }
                
        mysqli_close($con);  
        echo $summary;        
        echo $html_data;	   
        } // end . payment transactions

    function revenue_vat_report($data)
        {
        $startdate=$data["from"];
        $enddate=$data["to"];
        $device=$data["device"];
        $carpark=$data["carpark"];
        $type=$data["payment-type"];
        $category=$data["payment-category"];
        $discount=$data["discount"];   
        $showvoid=$data["showvoid"];
        
        $fdata=$this->getFacilityFeatures();
        $d=$fdata["decimal_places"];
        $currency=$fdata["currency"]; 
        $vat=$fdata["vat_percentage"]; 

        if($vat!=0)            
            {
            $query_string="select * from revenue_payments where payment_date_time between '".$startdate."' AND '".$enddate."'";     
            
            if($showvoid ==0)    
                $query_string.=" and (void is null or void='')";      
            else      
                $query_string.=" and void is not null";     
            
            if (strlen($carpark) > 0) 		           
                $query_string .= " AND carpark_number IN(".$carpark.")";   
            
            if (strlen($device) > 0) 		           
                $query_string .= " AND device_number IN(".$device.")";  
                
            if (strlen($type) > 0) 		           
                $query_string .= " AND payment_type IN(".$type.")";

            //category
            //0->all 1->parking Fee 2->Lost 3->discount 4->grace period 5->product sale 
            if(count($category) > 0) 
                {
                for($i=0; $i < count($category);$i++)
                    {
                    if($i==0)            
                        $query_string.=" and (";        
                    else
                        $query_string.=" Or ";
                    
                    switch($category[$i])
                        {
                        case 1:$query_string.=" (parking_fee>0 and lost_fee=0 and discount_amount=0)";
                            break;
                        case 2: $query_string.=" lost_fee>0";
                            break;
                        case 3: $query_string.=" discount_amount>0";
                            break;
                        case 4: $query_string.=" (discount_amount=0 AND gross_amount=0)";
                            break;
                        case 5: $query_string.=" payment_category='ProductSale'";
                            break;
                        }
                    }
                $query_string.=" )";
                }    
            
            if (strlen($discount) > 0) 		           
                $query_string .= " AND discount_id IN(".$discount.")";

            //echo $query_string;
            
            $con=$this->db_connect();     
            $result=mysqli_query($con,$query_string) or die(mysqli_error($con));
            $total_count= mysqli_num_rows($result);
            $count=0;
            $total_revenue=0;
            $total_vat=0;
            
            if(mysqli_num_rows($result) > 0)
                {                           

                $header .= '<div class="card-body p-0">';
                $header .= '<div class="card card-table-blue">';
                $header .= '<table width="100%" class="table table-blue table-hover jspdf-table">';

                $header .= '<thead>';
                $header .= '<tr>';
                $header .= '<th>#</th>';
                $header .= '<th>Ticket Number</th>';
                $header .= '<th>Device Name</th>';       
                $header .= '<th>Payment Time</th>'; 
                $header .= '<th>Parking Duration</th>';                                         
                $header .= '<th>VAT Amount</th>';                
                $header .= '<th>Discount Amount</th>';
                $header .= '<th>Gross Amount</th>';
                $header .= '<th>Category</th>';                
                $header .= '<th>Payment Type</th>';                                
                
                $header .= '</tr>';
                $header .= '</thead>';
                
                $html_data .= $header;
                $html_data .= '<tbody>';

                $category="";
                while($data=mysqli_fetch_assoc($result)) 
                    {
                    $count++;
                    if($data['discount_amount']>0)
                        $category="Discount";
                    else if($data['lost_fee']>0)
                        $category="Lost";
                    else if($data['discount_amount']==0 &&$data['gross_amount']==0)
                        $category="Grace Period";
                    else if($data['payment_category']=="ProductSale")
                        $category="Product Sale";
                    else    
                        $category="Parking Fee";
                    $html_data .= '<tr class="pointer"  id="payment_detail" payment_id="'.$data['id'].'">';            
                    $html_data.="<td>$count</td>";

                    if($showvoid == 0) 
                        {
                        $html_data .= "<td>" . $data['ticket_id'] . " </td>";
                        $html_data .= "<td>" . $data['device_name'] . " </td>";
                        } 
                    else 
                        {
                        $html_data .= "<td>" . $data['ticket_id'] . "<br>Void Reason:".$data['void_reason']."</td>";
                        $html_data .= "<td>" . $data['device_name'] . " <br>User:".$data['void_user']."</td>";
                        }

                
                    $html_data .= "<td >" . $data['payment_date_time'] . " </td>";           
                    $html_data .= "<td >" . $data['parking_duration'] . " </td>";                               
                    $html_data .= "<td>" . number_format($data['vat_amount'],$d) . " </td>";
                    $html_data .= "<td>" . number_format($data['discount_amount'],$d) . " </td>";
                    $html_data .= "<td>" . number_format($data['gross_amount'],$d) . " </td>";                    
                    $html_data .= "<td>" . $category. " </td>";                    
                    $html_data .= "<td>" . $data['payment_type'] . " </td>";           
                            
                    if(($_SESSION["userRollName"]=="Admin"||$_SESSION["userRollName"]=="CarParkManager")&& $showvoid ==0) 
                        {
                        //$html_data .= "<td><input class='btn btn-info btn-void-transaction'  type='button'  value='Void' data-id='".$data['id']."' ></td>";
                        }            
                
                    //$html_data .= "<td><input class='btn btn-info btn-show-pdf-reciept' value='PDF Receipt' type='button' data-id='".$data['id']."'></td>";
                
                    $html_data .= '</tr>';

                    $total_vat = $total_vat + $data['vat_amount'];
                    $total_revenue = $total_revenue + $data['gross_amount'];            
                    } 

                $html_data .= '</tbody>';
                $html_data .= '</table>';
                $html_data .= '</div>';
                $html_data .= '</div>';

                $summary .= '<div class="row mb-4 jspdf-graph">';
                $summary .= '<div class="col-lg-3 col-6">';       
                $summary .= '<div class="small-box bg-success">';
                $summary .= '<div class="inner">';
                $summary .= '<h3>'.$currency." ".number_format($total_revenue,$d). '</h3>';
                $summary .= '<h6>Total Revenue</h6>';
                $summary .= '</div>';
                $summary .= '<div class="icon">';
                $summary .= '<i class="ion ion-stats-bars"></i>';
                $summary .= '</div>';      
                $summary .= '</div>';        
                $summary .= '</div>';
                if($vat!=0)
                    {
                    $summary .= '<div class="col-lg-3 col-6">';        
                    $summary .= '<div class="small-box bg-success">';
                    $summary .= '<div class="inner">';
                    $summary .= '<h3>'.$currency." ".number_format($total_vat,$d). '</h3>';
                    $summary .= '<h6>Total VAT</h6>';
                    $summary .= '</div>';
                    $summary .= '<div class="icon">';
                    $summary .= '<i class="ion ion-stats-bars"></i>';
                    $summary .= '</div>';        
                    $summary .= '</div>';
                    $summary .= '</div>';
                    }
                
                $summary .= '<div class="col-lg-3 col-6">';       
                $summary .= '<div class="small-box bg-success">';
                $summary .= '<div class="inner">';
                $summary .= '<h3> ' .$count. '</h3>';
                $summary .= '<h6>Total Transactions</h6>';
                $summary .= '</div>';
                $summary .= '<div class="icon">';
                $summary .= '<i class="ion ion-stats-bars"></i>';
                $summary .= '</div>';            
                $summary .= '</div>';
                $summary .= '</div>';        
                $summary .= '</div><!-- Row End -->';
                }
            else      
                {
                $html_data="<div class='card'><div class='p-3'>No records/transactions available for the current search criteria</div></div>";
                }
                    
            mysqli_close($con);  
            echo $summary;
            }
        else
            $html_data="<div class='card'><div class='p-3'>No VAT for this facility</div></div>";
                
        echo $html_data;	   
        } // end . payment transactions
    
        
    function revenue_validation_transactions($startdate,$enddate)
        {      
        $html_data="";
        
        $query_string="select * from revenue_parking_validation where validation_date_time between '".$startdate."' and '".$enddate."'"; 
        
        $con=$this->db_connect();
        
        $result=mysqli_query($con,$query_string) or die(mysqli_error($con));
                
        if(mysqli_num_rows($result) > 0)
            {            
            $header .= '<div class="card-body p-0">';

            $header .= '<table width="100%" class="jspdf-table">';
            $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';
            
            $header .= "<th class='col'>Ticket Number</th>";
            $header .= "<th class='col'>Validator Name</th>";
            $header .= "<th class='col'>Validation Date and Time</th>";
            
            $header .= '</tr>';
            
            $html_data .= $header;
            
            $html_data .= '<tbody class="table-view">';
            
            while ($data = mysqli_fetch_assoc($result)) {
            
                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
            
                $html_data .= "<td class='col'> " . $data['ticket_id'] . " </td>";
                $html_data .= "<td class='col'> " . $data['validator_name'] . " </td>";
                $html_data .= "<td class='col'> " . $data['validation_date_time'] . " </td>";
            
                $html_data .= '</tr>';
            }
            
            $html_data .= '</table>';
            
            $html_data .= '</div>';

            }
        else
            {
            $html_data.="<div class='p-3'>No records/transactions available for the current search criteria</div>";
            }        
        mysqli_close($con);  
        echo $html_data;                
        } // end . validation transactions


    function void_revenue_payment($data) 
        {
        $id=$data["id"];  
        $reason=$data["reason"];  
        $con=$this->db_connect();
        $query="update revenue_payments set
        void='1',void_user='".$_SESSION["user"]."',void_reason='".$reason."',void_datetime=NOW() where id=".$id;
        mysqli_query($con,$query)or die(mysql_error());
        mysqli_close($con);
        }

    function revenue_creditcard_payments_report($data)
        {
        $FromDateTime=$data["from"];
        $ToDateTime=$data["to"];
        $carpark=$data["carpark"];
        $device=$data["device"];
        $con = $this->db_connect();
        $query_string = "select * from revenue_creditcard_payments where  date_time between '". $FromDateTime . "' AND '" . $ToDateTime . "'";
        if (strlen($carpark) > 0) 		           
            $query_string .= " AND carpark_number IN(".$carpark.")";   
        
        if (strlen($device) > 0) 		           
            $query_string .= " AND device_number IN(".$device.")";  

        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));

        if (mysqli_num_rows($result) > 0) 
            {
            $fdata=$this->getFacilityFeatures();
            $d=$fdata["decimal_places"];
            $currency=$fdata["currency"]; 
                
            $header .= '<div class="card-body p-0">';

            $header .= '<table width="100%" class="jspdf-table">';
            $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';
            $header .= "<th class='col'>Device name</th>";
            $header .= "<th class='col'>Merchant id</th>";
            $header .= "<th class='col'>Terminal Id</th>";
            $header .= "<th class='col'>Ticket number</th>";
            $header .= "<th class='col-3'>Comment</th>";
            $header .= "<th class='col'>Card Type</th>";
            $header .= "<th class='col'>Amount</th>";
            $header .= "<th class='col'>Authentication Code</th>";
            $header .= "<th class='col'>Reference Number</th>";
            $header .= "<th class='col'>Date Time</th>";
            $header .= "<th class='col'>Status</th>";
            $header .= '</tr>';

            $html_data .= $header;

            $html_data .= '<tbody class="table-view">';
            $total=0;
            $count=0;
            while ($data = mysqli_fetch_assoc($result)) 
                {
                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
                $html_data .= "<td class='col'> " . $this->get_device_name($data['device_number']) . " </td>";
                $html_data .= "<td class='col'> " . $data['merchant_id'] . " </td>";
                $html_data .= "<td class='col'> " . $data['terminal_id'] . " </td>";
                $html_data .= "<td class='col'> " . $data['ticket_number'] . " </td>";
                $html_data .= "<td class='col-3'> " . $data['comment'] . " </td>";
                $html_data .= "<td class='col'> " . $data['card_type'] . " </td>";
                $html_data .= "<td class='col'> " . $data['amount'] . " </td>";
                $html_data .= "<td class='col'> " . $data['authentication_code'] . " </td>";
                $html_data .= "<td class='col'> " . $data['reference_number'] . " </td>";
                $html_data .= "<td class='col'> " . $data['date_time'] . " </td>";
                $html_data .= "<td class='col'> " . $data['status'] . " </td>";

                $html_data .= '</tr>';                         
                    $total = $total + (float) $data['amount'];
                    $count++;
                }

            $html_data .= '</table>';

            $html_data .= '</div>';

            $summary .= '<div class="row mb-4 jspdf-graph">';

            $summary .= '<div class="col-lg-3 col-6">';
            $summary .= '<!-- small box -->';
            $summary .= '<div class="small-box bg-success">';
            $summary .= '<div class="inner">';
                    
            $summary.='<h3>'.$currency." ".number_format($total,$d).'</h3>';
            $summary .= '<h6>Total Revenue</h6>';
            $summary .= '</div>';
            $summary .= '<div class="icon">';
            $summary .= '<i class="ion ion-stats-bars"></i>';
            $summary .= '</div>';
        // $summary .= '<a href="#" class="small-box-footer"> <i class="fa fa-arrow-circle-right"></i></a>';
            $summary .= '</div>';
            
            $summary .= '</div>';
            
            $summary .= '<div class="col-lg-3 col-6">';
            $summary .= '<!-- small box -->';
            $summary .= '<div class="small-box bg-info">';
            $summary .= '<div class="inner">';
            $summary .= '<h3> ' .$count. '</h3>';
            $summary .= '<h6>Total Transactions</h6>';
            $summary .= '</div>';
            $summary .= '<div class="icon">';
            $summary .= '<i class="ion ion-stats-bars"></i>';
            $summary .= '</div>';
            //$summary .= '<a href="#" class="small-box-footer"> <i class="fa fa-arrow-circle-right"></i></a>';
            $summary .= '</div>';
            $summary .= '</div>';
            
            $summary .= '<div class="col-lg-3 col-6">';
            $summary .= '</div>';

            $summary .= '</div><!-- Row End -->';
            $summary .= '</div>';

            mysqli_close($con);

            echo $summary;
            echo $html_data;
            } //
        else 
            echo "<div class='p-3'>No records/transactions available for the current search criteria</div>";        
            }

    function cash_levels($device)
       {              
       $con=$this->db_connect_dashboard();
       $query_string="SELECT a.device_number,a.device_name,a.denomination,a.current_level as recycler_current_level,b.current_level as cashbox_current_level FROM apm_recycle_levels as a join apm_cashbox_levels as b on a.denomination=b.denomination and a.device_number=b.device_number order by denomination asc";
	   if (strlen($carpark) > 0)              
            $query_string .= " AND device_number IN(".$device.")"; 
	   
       $result=mysqli_query($con,$query_string) or die(mysqli_error($con));
       
        $header .= '<table width="100%" >';
        $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';
        //$header.="<th class='col'>DeviceName</th>";
        $header.="<th class='col'>Banknote</th>";        
        $header.="<th class='col'>Recycler level</th>";
		$header.="<th class='col'>Cashbox level</th>";
		$header.="<th class='col'>Total</th>";		
        $header.="</tr>";
        $header.="<tbody class='table-view'>";

        
		$apm_total=0;	
        $html_data="";
        $device="";
        while($data=mysqli_fetch_assoc($result))
            {
			if($html_data=="")
				$html_data.=$header;
			if($device!=$data['device_number'])
				{
                if($device!="")
                    {
                    $html_data.="<tr class='card-text d-flex justify-content-between align-items-center'><td class='col'></td><td class='col'></td><td class='col'><h3>Total</h3></td><td class='col'><h3>".$apm_total."</h3></td></tr>";
                    $apm_total=0;   
                    }    
                $html_data.="<tr class='subhead card-text d-flex justify-content-between align-items-center pl-2'><td>".$data['device_name']."</td></tr>";
                $device=$data["device_number"];
                }
            $html_data.="<tr class='card-text d-flex justify-content-between align-items-center'>";
            //$html_data.="<td class='col'>".$data['device_name']."</td>";    
            $html_data.="<td class='col'>".$data['denomination']."</td>";            
            $html_data.="<td class='col'>".$data['recycler_current_level']."</td>";            
			$html_data.="<td class='col'>".$data['cashbox_current_level']."</td>";   
			$total=$data['denomination']*$data['cashbox_current_level']+$data['denomination']*$data['recycler_current_level'];			
            $html_data.="<td class='col'>".$total."</td>";  		
            $html_data.="</tr>";
			$apm_total=$apm_total+$total;									
           }
        if($html_data!="")
            $html_data.="<tr class='card-text d-flex justify-content-between align-items-center'><td class='col'></td><td class='col'></td><td class='col'><h3>Total</h3></td><td class='col'><h3>".$apm_total."</h3></td></tr>";
           
       echo $html_data;
       } // End

	function revenue_payment_exceptions($data)
        {  
        $startdate=$data["from"];
        $enddate=$data["to"];
        $device=$data["device"];
        $carpark=$data["carpark"];
        $option=$data["exception_option"];
        $html_data="";      
        $query_string="select * from revenue_payment_exceptions where date_time between '".$startdate."' and '".$enddate."'"; 
	 
        if (strlen($carpark) > 0) 		           
            $query_string .= " AND carpark_number IN(".$carpark.")";   
      
        if (strlen($device) > 0) 		           
            $query_string .= " AND device_number IN(".$device.")";  
	
	    if ($option !="0") 
            $query_string .= " AND exception LIKE '".$option."%'";
        
        
        $con=$this->db_connect();
     
        $result=mysqli_query($con,$query_string) or die(mysqli_error($con));
            
        if(mysqli_num_rows($result) > 0)
            {
            $fdata=$this->getFacilityFeatures();
            $d=$fdata["decimal_places"];
            $currency=$fdata["currency"]; 
                            
            $header .= '<div class="card-body p-0">';

            $header .= '<table width="100%" class="jspdf-table">';
            $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';
            
            $header .= "<th class='col'>Device name</th>";
            $header .= "<th class='col'>Ticket number</th>";
            //$header .= "<th class='col'>Paid status</th>";
            $header .= "<th class='col'>Exception</th>";
            $header .= "<th class='col'>Amount received</th>";
            $header .= "<th class='col'>Balance returned</th>";
            $header .= "<th class='col'>Credit note</th>";
            $header .= "<th class='col'>Bank notes</th>";
            
            $header .= '</tr>';
            
            $html_data .= $header;
        
            $html_data .= '<tbody class="table-view">';
        
            while ($data = mysqli_fetch_assoc($result)) 
                {
                if($data['paid_status']==1)
                    $paid="Not paid";
                else if($data['paid_status']==2)
                    $paid="Paid";
                else if($data['paid_status']==3)
                    $paid="Exited";
			
                $notes=$data['bank_notes'];
                $notes=substr($notes,0,-1);
                $notes=str_replace(","," ".$currency." x ",$notes);
                $notes=str_replace(";","<br>",$notes);
                
                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
            
                $html_data .= "<td class='col'> " . $data['device_name'] . " </td>";
                $html_data .= "<td class='col'> " . $data['ticket_id'] . " </td>";
                //$html_data .= "<td class='col'> " . $paid. " </td>";
                $html_data .= "<td class='col'> " . $data['exception'] . "</td>";
                $html_data .= "<td class='col'> " . $data['amount_received'] . " ".$currency."</td>";
                $html_data .= "<td class='col'> " . $data['balance_returned'] . " ".$currency." </td>";
                $html_data .= "<td class='col'> " . $data['credit_note'] . " ".$currency."</td>";
                $html_data .= "<td class='col'> " . $notes . " </td>";
            
                $html_data .= '</tr>';
                }        
            $html_data .= '</table>';
            
            $html_data .= '</div>';
            }
        else
            $html_data.="<div class='p-3'>No records/transactions available for the current search criteria</div>";
        
        mysqli_close($con);  
        echo $html_data;	   
        }

} // end class