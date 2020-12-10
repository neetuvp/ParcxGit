<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of dashboard
 *
 * @author User
 */
class dashboard
    {   
       
    function db_connect()
        {        
        $con = mysqli_connect(DB_HOST, DB_USER, DB_PASSWORD, DB_NAME_DASHBOARD);
        
        if (!$con)
            $this->error("Connection attempt failed");
        else
            return $con;            
        }    // end 
    
    function db_connect_config()
        {        
        $con = mysqli_connect(DB_HOST, DB_USER, DB_PASSWORD, DB_NAME);
        if (!$con)
            $this->error("Connection attempt failed");
        else
            return $con;            
        } // end 

    function db_connect_reporting()
        {
        $con = mysqli_connect(DB_HOST, DB_USER, DB_PASSWORD, DB_NAME_REPORTING);
        if (!$con)
        $this->error("Connection attempt failed");
        else
        return $con;
        } // end

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
                   
    function live_revenue()
        {
        $con=$this->db_connect();
        $query_string="select sum(gross_amount) as gross_amount,sum(parking_fee-discount_amount) as parking_fee,sum(vat_amount) as vat_amount,sum(lost_ticket_fee+admin_fixed_charges+ticket_replacement_fee) as lost_fee,sum(discount_amount) as discount_amount,sum(product_sale_amount) as product_sale_amount from parking_revenue";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));        
        if ($data = mysqli_fetch_assoc($result)) 
            {
            $response["gross_amount"] =$data["gross_amount"];    
            $response["parking_fee"] =$data["parking_fee"];
            $response["vat_amount"] =$data["vat_amount"];
            $response["lost_fee"] =$data["lost_fee"];
            $response["product_sale_amount"] =$data["product_sale_amount"];
            }           
        mysqli_close($con);        
        echo json_encode($response);
        }
               
    function get_revenue_summary()
        {
        $fdata=$this->getFacilityFeatures();
        $d=$fdata["decimal_places"];
        $currency=$fdata["currency"]; 
        $vat=$fdata["vat_percentage"];

        $con          = $this->db_connect();
        $carpark      = array();
        $carpark_name = array();
        $last_updated_date_time="";
        $i            = 0;
        $html_data    = "";
        $header       = "";
        
        $header .= '<table width="100%" class="jspdf-table" data-status="table-view">';
        $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';

        $header .= '<th class="col-2">Device Name</th>';
        $header .= '<th class="col">Total Revenue</th>';
        $header .= '<th class="col">Parking Fee</th>';
        $header .= '<th class="col">Product Sale Amount</th>';
        $header .= '<th class="col">VAT Amount</th>';
        $header .= '<th class="col">Lost Ticket Fee</th>';
        $header .= '<th class="col">Discount Amount</th>';                
        $header .= '<th class="col">Payable Entries</th>';        
        $header .= '<th class="col">Lost Ticket Count</th>';
        $header .= '<th class="col">Discount Count</th>';
        

        $header .= '</tr>';
        
        $query_string = "select carpark_number,carpark_name,last_updated_date_time from parking_revenue group by carpark_number order by carpark_number ASC";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        while ($data = mysqli_fetch_assoc($result)) 
            {
            $carpark[]      = $data['carpark_number'];
            $carpark_name[] = $data['carpark_name'];
            $last_updated_date_time=$data['last_updated_date_time'];
            }
        $html_data='<input type="hidden" id="last_updated_time" value="'.$last_updated_date_time.'">';
        $html_data .= $header;
        
        while ($i < count($carpark)) 
            {            
            $query_string = "select * from parking_revenue where carpark_number=" . $carpark[$i][0] . " order by carpark_number DESC";           
            $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
            
            $html_data .= '<tbody class="table-view">';

            $html_data .= "<tr class='subhead card-text d-flex justify-content-between align-items-center pl-2' data-status='header'><td>" . $carpark_name[$i] ."</td></tr>";


            while ($data = mysqli_fetch_assoc($result)) 
                {                
                $data_status="all";
				switch($data["device_type"])
					{
					case 3:
						$data_status = "manual-cashier";  
						break;
					case 4:
						$data_status = "payonfoot-machine";						
						break;
					case 5:
						$data_status = "handheld-pos";
						break;					
					}                                              

                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center" data-status="'.$data_status.'">';

                $html_data .= '<td class="col-2 text-left text-dark table-row-detail">';
                $html_data .= '<h6>';
                if ($data["network_status"] == 1) 
                    {
                    $html_data .= '<div class="dot-indicator bg-success-gradient" data-toggle="tooltip" data-placement="top" title="Online" ></div>';
                    } 
                else 
                    {                    
                    $html_data .= '<div class="dot-indicator bg-danger-gradient" data-toggle="tooltip" data-placement="top" title="Currently Off line"></div>';                    
                    }
                $html_data .= $data["device_name"] . '</h6>';                
                $html_data .= '<p class="table-detail">';
                $html_data .= 'Last Transaction update<br>' . $data["last_transaction"].'';
                $html_data .= '</p>';
                $html_data .= '</td>';
                $html_data .= '<td class="col" label="total-revenue">' . number_format($data["gross_amount"],$d) . '</td>';
                $html_data .= '<td class="col" label="parking-fee">' . number_format($data["parking_fee"],$d) . '</td>';
                $html_data .= '<td class="col" label="product-sale-amount">' . number_format($data["product_sale_amount"],$d) . '</td>';
                $html_data .= '<td class="col" label="vat-amount">' . number_format($data["vat_amount"],$d). '</td>';
                $html_data .= '<td class="col" label="lost-ticket-fee">' . number_format($data["lost_ticket_fee"] ,$d). '</td>';                
                $html_data .= '<td class="col" label="total-discount">' . number_format($data["discount_amount"],$d) . '</td>';                
                $html_data .= '<td class="col" label="vip coupons">' . $data["payable_entries_count"] . '</td>';                
                $html_data .= '<td class="col" label="lost-ticket-count">' . $data["lost_ticket_count"] . '</td>';
                $html_data .= '<td class="col" label="discount-count">' . $data["discount_count"] . '</td>';                
                $html_data .= '</tr>';                                               
                }            
            $i++;            
            }

        mysqli_close($con);

        $con=$this->db_connect_reporting();

        $html_data .= '</tbody>';
        $html_data .= '</table>';      
        
        // block view

        $html_data .= '<div data-status="block-view" class="jspdf-graph">';
        $html_data .= '<div class="row">';

        $current_date=date("Y-m-d")." ".DAY_CLOSURE_START;
        $day_closure_start=new DateTime($current_date);
        $start=$day_closure_start->format('Y-m-d H:i:s');
        $day_closure_end=$day_closure_start->add(new DateInterval('PT23H59M59S'));
        $end=$day_closure_end->format('Y-m-d H:i:s');
        
        $query_string = "select * from revenue_shifts where login_time between '$start' and '$end'";   
        //echo $query_string;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));

        while ($data = mysqli_fetch_assoc($result)) 
            {            
            $data_status="all";
                            
            if(stripos($data["device_name"],"Cashier")!==false) 
                {
                $data_status = "manual-cashier";    
                $image_url = "/parcx/dist/img/icon/device_icons/manual-cashier.png";
                }
            else if(stripos($data["device_name"],"Handheld")!==false) 
                {
                $data_status = "handheld-pos";
                $image_url = "/parcx/dist/img/icon/device_icons/handheld-pos.png";
                }
            else if(stripos($data["device_name"],"Auto Payment Machine")!==false) 
                {
                $data_status = "payonfoot-machine";
                $image_url = "/parcx/dist/img/icon/device_icons/payonfoot-machine.png";
                }               

            $html_data .= '<div class="col-lg-3 col-sm-6 col-xs-12 block-data" data-status="'.$data_status.'">';

            if ($data["network_status"] == 1) 
                {
                $html_data .= '<div class="card card-success card-outline" data-network="'.$data[" network_status"].'">';
                } 
            else 
                {
                $html_data .= '<div class="card card-danger card-outline" data-network="'.$data[" network_status"].'">';
                }

            $html_data .= '<div class="card-header">';
            $html_data .= '<h3 class="card-title">'. $data["device_name"] .'</h3>';
            $html_data .= '</div>';
            $html_data .= '<div class="card-body p-0">';
            $html_data .= '<div class="row no-gutters">';
            $html_data .= '<div class="col-4 block-view-img my-auto text-center">';
            $html_data .= '<img class="img-fluid" src="'. $image_url .'">';                
            $html_data .= '</div>';
            $html_data .= '<div class="col-8">';
            $html_data .= '<ul class="nav flex-column">';
            $html_data .= '<li class="nav-item">';
            $html_data .= '<span class="nav-link">';
            $html_data .= 'Current User <span class="float-right">'.$data['operator_name'].'</span>';
            $html_data .= '</span>';
            $html_data .= '</li>';
            $html_data .= '<li class="nav-item">';
            $html_data .= '<span class="nav-link">';
            $html_data .= 'Shift Start Time <span class="float-right">'.substr($data["login_time"],11). '</span>';
            $html_data .= '</span>';
            $html_data .= '</li>';
            $html_data .= '<li class="nav-item">';
            $html_data .= '<span class="nav-link">';
            $html_data .= 'Last Updated <span class="float-right">'. substr($data["last_updated_date_time"],11) .'</span>';
            $html_data .= '</span>';
            $html_data .= '</li>';
            $html_data .= '<li class="nav-item">';
            $html_data .= '<span class="nav-link">';
            $html_data .= 'Earnings <span class="float-right">AED '. $data['shift_earnings'] .'</span>';
            $html_data .= '</span>';
            $html_data .= '</li>';
            $html_data .= '</ul>';
            $html_data .= '</div>';
            $html_data .= '</div>';
            $html_data .= '</div>';
            $html_data .= '</div>';
            $html_data .= '</div>';            
            }            
        mysqli_close($con);            
        $html_data .= '</div>';
        $html_data .= '</div>';
        // end / block view        

        echo $html_data;                
        } // End Function . Live Revenue 

    
    function revenue_lastdays()
        {        
        $con=$this->db_connect_reporting();
        $query_string="select report_date,report_day,sum(gross_amount) as gross_amount from summary_daily_revenue where
        report_date between DATE(NOW())-INTERVAL 7 DAY and DATE(NOW())- INTERVAL 1 DAY group by report_date ";

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
        $con=$this->db_connect();
        $query_string="select sum(gross_amount) as gross_amount from parking_revenue";
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
  

    function getLastUpdatedDateForOccupancy()
        {
        $con = $this->db_connect();
        $query_string = "select max(last_updated_datetime) as last_updated_date_time from counting_carpark";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        if ($data = mysqli_fetch_assoc($result))
            {
            echo  $data["last_updated_date_time"];
            }
        mysqli_close($con);
        }



    function getDeviceAlarmscount($device,$alarmseverity1,$alarmseverity2)
        {
        $con=$this->db_connect_reporting();
        $query_string ="select * from watchdog_device_alarms where device_number='".$device."' and alarm_severity between
        ".$alarmseverity1 ." and ".$alarmseverity2." and alarm_date_time>=DATE_ADD(CURDATE(),INTERVAL -7 DAY)";
        //echo $query_string;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        
        $rows=mysqli_num_rows($result);
        mysqli_close($con);
        return $rows;
        }

    function getLatestAlarmsCount()
        {
        $con=$this->db_connect_reporting();
        $query_string ="select * from watchdog_device_alarms where alarm_severity between 1 and 10 and dismiss=0 and alarm_date_time>=DATE_ADD(CURDATE(),INTERVAL -7 DAY)";
        //echo $query_string;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));

        $rows=mysqli_num_rows($result);
        mysqli_close($con);
        echo $rows;
        }


    function dismissAlarm($id)
        {
        $con=$this->db_connect_reporting();
        $query_string ="update watchdog_device_alarms set dismiss=1 where id=".$id;
        //echo $query_string;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));        
        mysqli_close($con);       
        }

    function getAlarmList($device)
        {
        $con=$this->db_connect_reporting();
        $query_string ="select * from watchdog_device_alarms where device_number='".$device."' and alarm_date_time>=DATE_ADD(CURDATE(),INTERVAL -5 DAY)";
        //echo $query_string;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $htmldata="";
        if (mysqli_num_rows($result) > 0) 
            {
            $htmldata.='<tbody>';
            $htmldata.='<tr class="card-header d-flex justify-content-between align-items-center">';
            $htmldata.='<th class="col">Alarm Severity</th>';
            $htmldata.='<th class="col">Report Date</th>';
            $htmldata.='<th class="col">Alarm code</th>';
            $htmldata.=' <th class="col">Description</th>';
            $htmldata.=' <th class="col">Comment</th>';
            $htmldata.='</tr>';
            $htmldata.='</tbody>';
            $htmldata.='<tbody class="table-view">';
            }    
        while ($data = mysqli_fetch_assoc($result))
            {
            $htmldata.='<tr class="card-text d-flex justify-content-between align-items-center">';
            $htmldata.='<td class="col">';
            $button="";
            if($data["alarm_severity"]<10)
                $button="danger";
            if(($data["alarm_severity"]>=10))
                $button="warning";
            $htmldata.='<div class="dot-indicator bg-'.$button.'-gradient"></div>'.$data["alarm_severity"].'
            </td>';
            $htmldata.='<td class="col">'.$data["alarm_date_time"].'</td>';
            $htmldata.='<td class="col">'.$data["alarm_code"].'</td>';
            $htmldata.='<td class="col">'.$data["alarm_description"].'</td>';
            $dismiss="";
            if($data["dismiss"]==1)
                $dismiss="Dismissed";
            $htmldata.='<td class="col">'.$dismiss.'</td>';
            $htmldata.=' </tr>';
            }
        $htmldata.='</tbody>';
        mysqli_close($con);
        echo $htmldata;
        }

    function getLatestAlarmList()
        {
        $con=$this->db_connect_reporting();
        $query_string ="select * from watchdog_device_alarms where alarm_severity between 1 and 10 and dismiss=0 and alarm_date_time>=DATE_ADD(CURDATE(),INTERVAL -7 DAY) order by alarm_date_time desc";
        //echo $query_string;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $htmldata="";
        if (mysqli_num_rows($result) > 0) 
            {
            $htmldata.='<tbody>';
            $htmldata.='<tr class="card-header d-flex justify-content-between align-items-center">';
            $htmldata.='<th class="col">Alarm Severity</th>';
            $htmldata.='<th class="col">Device name</th>';
            $htmldata.='<th class="col">Report Date</th>';
            $htmldata.='<th class="col">Alarm code</th>';
            $htmldata.=' <th class="col">Comments</th>';
            $htmldata.='<th class="col"></th></tr>';
            $htmldata.='</tbody>';
            $htmldata.='<tbody class="table-view">';
            }    
        while ($data = mysqli_fetch_assoc($result))
            {
            $htmldata.='<tr class="card-text d-flex justify-content-between align-items-center">';
            $htmldata.='<td class="col">';
            $button="";		
            if($data["alarm_severity"]<10)
                $button="danger";
            if(($data["alarm_severity"]>=10))
                $button="warning";
            $htmldata.='<div class="dot-indicator bg-'.$button.'-gradient"></div>'.$data["alarm_severity"].'
            </td>';
            $htmldata.='<td class="col">'.$data["device_name"].'</td>';
            $htmldata.='<td class="col">'.$data["alarm_date_time"].'</td>';
            $htmldata.='<td class="col">'.$data["alarm_code"].'</td>';
            $htmldata.='<td class="col">'.$data["alarm_description"].'</td>';
            $htmldata.=' <td class="col"><button type="button" class="btn btn-danger btn-dismis-alarm" id="'.$data["id"].'">Dismiss</button>
            </td>
            </tr>';
            }
        $htmldata.='</tbody>';
        mysqli_close($con);
        echo $htmldata;
        }


    function get_device_status() 
        {
        $con = $this -> db_connect();
        $carpark = array();
        $carpark_name = array();
        $i = 0;
        $html_data = "";
        $header = "";

        $header .= '<table width="100%" class="jspdf-table hidden" data-status="table-view">';
        $header .= '<tr class="card-header d-flex justify-content-between align-items-center">';
        $header .= '<th class="col">Device Name</th>';
        $header .= '<th class="col"> Device Category</th>';
        $header .= '<th class="col">IP Address</th>';

        $header .= '</tr>';
        $query_string = "select DISTINCT carpark_number,carpark_name from watchdog_device_status order by carpark_number ASC";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));

        while ($data = mysqli_fetch_assoc($result)) 
            {
            $carpark[] = $data['carpark_number'];
            $carpark_name[] = $data['carpark_name'];
            }

        $html_data .= $header;

        while ($i < count($carpark)) 
            {
            $query_string = "select * from watchdog_device_status where carpark_number=".$carpark[$i][0]."  order by device_type,device_number";
            $result = mysqli_query($con, $query_string) or die(mysqli_error($con));

            $html_data .= '<tbody class="table-view">';
            $html_data .= "<tr class='subhead card-text d-flex justify-content-between align-items-center pl-2' data-status='header'><td>" .$carpark_name[$i]. "</td></tr>";

            while ($data = mysqli_fetch_assoc($result)) 
                {
                $data_status = "all";

                if ($data["device_type"] == 1 || $data["device_type"] == 2)                     
                    $data_status = "columns";                    

                if ($data["device_type"] == 3 || $data["device_type"] == 5 || $data["device_type"] == 4) 
                    $data_status = "payment_machines";                

                if ($data["device_type"] == 6||$data["device_type"] == 7) 
                    $data_status = "controllers";                
                
                if ($data["device_type"] == 8) 
                    $data_status = "camera";                
			
                if ($data["device_type"] == 9) 
                    $data_status = "vms";
                

                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center" data-status="'.$data_status .'">';

                $html_data .= '<td class="col text-left text-dark table-row-detail">';
                $html_data .= '<h6>';

                if ($data["device_network_status"] == 1)                 
                    $html_data .= '<div class="dot-indicator bg-success-gradient" data-toggle="tooltip" data-placement="top" title="Online"></div>';
                else 
                    $html_data .= '<div class="dot-indicator bg-danger-gradient" data-toggle="tooltip" data-placement="top" title="Currently Off line"></div>';
                

                $html_data .= $data["device_name"].'</h6></td>';
                $category = "";
                switch ($data["device_type"]) 
                    {
                    case 1:
                        $category = "Entry Column";
                        break;
                    case 2:
                        $category = "Exit Column";
                        break;
                    case 3:
                        $category = "Cashier POS";
                        break;
                    case 4:
                        $category = "Auto Payment Machine";
                        break;
                    case 5:
                        $category = "Handheld POS";
                        break;
                    case 6:
                        $category = "Controller";
                        break;
                    case 7:
                        $category = "Controller";
                        break;
                    case 8:
                        $category = "Camera";
                        break;
                    case 9:
                        $category = "VMS";
                        break;
                    }
                $html_data .= '<td class="col" label="category">'.$category.'</td>';
                $html_data .= '<td class="col" label=device_ip>'.$data["device_ip"].'</td>';
                $html_data .= '</tr>';
                }
            $i++;
            }
        $html_data .= '</tbody>';
        $html_data .= '</table>';

        // start block view

        $html_data .= '<div data-status="block-view">';
        $html_data .= '<div class="row">';

       
        $query_string = "select * from watchdog_device_status order by device_type,device_number";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));

        while ($data = mysqli_fetch_assoc($result)) 
            {
            $data_status = "all";

            if ($data["device_type"] == 1 || $data["device_type"] == 2) 
                {
                $data_status = "columns";
                $image_url = "/parcx/dist/img/icon/device_icons/column.png";
                $data_img = "column";
                } 
            else if ($data["device_type"] == 3) 
                {
                $data_status = "payment_machines";
                $image_url = "/parcx/dist/img/icon/device_icons/cashier-pos.png";
                $data_img = "cashier-pos";
                }
            else if ($data["device_type"] == 5) 
                {
                $data_status = "payment_machines";
                $image_url = "/parcx/dist/img/icon/device_icons/handheld-pos.png";
                $data_img = "handheld-pos";
                } 
            else if ($data["device_type"] == 4) 
                {
                $data_status = "payment_machines";
                $image_url = "/parcx/dist/img/icon/device_icons/payment-machine.png";
                $data_img = "payment-machine";
                } 
            else if ($data["device_type"] == 6||$data["device_type"] == 7) 
                {
                $data_status = "controllers";
                $image_url = "/parcx/dist/img/icon/device_icons/controller.png";
                $data_img = "controller";
                }
            else if ($data["device_type"] == 8) 
                {
                $data_status = "camera";
                $image_url = "/parcx/dist/img/icon/device_icons/cctv.png";
                $data_img = "cctv";
                }
            else if ($data["device_type"] == 9) 
                {
                $data_status = "vms";
                $image_url = "/parcx/dist/img/icon/device_icons/vms.png";
                $data_img = "vms";
                }

            $html_data .= '<div class="col-lg-3 col-md-6 block-data" data-img="'.$data_img.'" data-status="'.$data_status.'" data-toggle="modal" data-target="#error-log-modal">';
            // $html_data .= '<div class="card card-danger card-outline">';

            $html_data .= '<div class="card">';

            $html_data .= '<div class="card-header">';
            $html_data .= '<div class="nav-item d-flex justify-content-between align-items-center">';

            $html_data .= '<h3 class="card-title">'.$data["device_name"].'</h3>';
            $html_data .= '<span>';

            if ($data["device_network_status"] == 1) 
                {
                $html_data .= '<span class="float-right ml-3 header-icon">';
                $html_data .= '<i class="fas fa-server text-success" data-toggle="tooltip" data-placement="top" data-original-title="Server Connectivity"></i>';
                $html_data .= '</span>';
                $html_data .= '</span>';
                } 
            else 
                {
                $html_data .= '<span class="float-right ml-3 header-icon">';
                $html_data .= '<i class="fas fa-server text-danger" data-toggle="tooltip" data-placement="top" data-original-title="Server Connectivity"></i>';
                $html_data .= '</span>';
                $html_data .= '</span>';
                }

            $category = "";
            switch ($data["device_type"]) 
                {
                case 1:
                    $category = "Entry Column";
                    break;
                case 2:
                    $category = "Exit Column";
                    break;
                case 3:
                    $category = "Cashier POS";
                    break;
                case 4:
                    $category = "APM";
                    break;
                case 5:
                    $category = "Handheld POS";
                    break;
                case 6:
                    $category = "Controller";
                    break;
                case 7:
                    $category = "Controller";
                    break;
                case 8:
                    $category = "Camera";
                    break;
                case 9:
                    $category = "VMS";
                    break;
                }

            $html_data .= '</div>';
            $html_data .= '</div>';

            $html_data .= '<div class="card-body p-0">';
            $html_data .= '<div class="row no-gutters">';

            $html_data .= '<div class="col-4 block-view-img my-auto text-center"><img class="img-fluid" src="'. $image_url .'"></div>';

            $html_data .= '<div class="col-8">';
            $html_data .= '<ul class="nav flex-column">';
            $html_data .= '<li class="nav-item">';
            $html_data .= '<span class="nav-link">Category';
            $html_data .= '<span class="float-right device_category">'.$category.'</span>';
            $html_data .= '</span>';
            $html_data .= '</li>';
            $html_data .= '<li class="nav-item">';
            $html_data .= '<span class="nav-link">Device Number';
            $html_data .= '<span class="float-right device_number">'.$data["device_number"].'</span>';
            $html_data .= '</span>';
            $html_data .= '</li>';
            $html_data .= '<li class="nav-item">';
            $html_data .= '<span class="nav-link">IP Address';
            $html_data .= '<span class="float-right device_ip">'.$data["device_ip"].'</span>';
            $html_data .= '</span>';
            $html_data .= '</li>';

            $html_data .= '<li class="nav-item d-flex justify-content-between align-items-center">';
            $html_data .= '<span class="nav-link">';
            $html_data .= 'Notifications';
            $html_data .= '</span>';
            $html_data .= '<span class="nav-link p-1">';
            $html_data .= '<span class="fa-stack fa-1x">';
            $html_data .= '<i class="fa fa-circle fa-stack-2x text-warning"></i>';
            $html_data .= '<strong class="fa-stack-1x text-dark">'.$this->getDeviceAlarmscount($data["device_number"],11,20).'</strong>';
            $html_data .= '</span>';

            $html_data .= '<span class="fa-stack fa-1x">';
            $html_data .= '<i class="fa fa-circle fa-stack-2x text-danger"></i>';
            $html_data .= '<strong class="fa-stack-1x text-white">'.$this->getDeviceAlarmscount($data["device_number"],1,10).'</strong>';
            $html_data .= '</span>';
            $html_data .= '</span>';
            $html_data .= '</li>';

            $html_data .= '</ul>';
            $html_data .= '</div>';

            $html_data .= '</div>'; // close row
            $html_data .= '</div>'; // close card-body

            $html_data .= '</div>';
            $html_data .= '</div>';            
            }       
        $html_data .= '</div>';
        $html_data .= '</div>';
        // end block view
        mysqli_close($con);
        echo $html_data;
        } // End Function . Live Revenue



/* DASHBOARD OCCUPANCY FUNCTIONS */
    function OccupancyFacilityCounters()
        {      
        $con= $this->db_connect();
        $query_string = "select facility_number,carpark_name,total_spaces,current_level,last_updated_datetime from counters";
        $query_string .=" where counter_type=0 ORDER BY dashboard_order ASC";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $html_data = "";
        $i=1;
          
        if(mysqli_num_rows($result)== 1)
            {
            //If only one facility show the car park list directly
            $data = mysqli_fetch_assoc($result);
            $_SESSION['dashboard_level']="carparks"; 
            $_SESSION['facility_number']=$data['facility_number'];
            $this->OccupancyCarparkCounters($data['facility_number']);            
            }        
        else
            {  

            while ($data = mysqli_fetch_assoc($result)) 
                {
                if($i==1)
                    {                     
                    $html_data .= '<input type="hidden" id="last_updated_datetime" value="'.$data['last_updated_datetime'].'">';     
                    }
                $gauge_value=($data['current_level']/$data['total_spaces'])*100;
                $html_data .= ' <div class="col-lg-3 col-sm-6 col-xs-12">';
                $html_data .= ' <div class="chart-box text-center">';
                $html_data .= ' <span id="dashboard_level" level="facility"></span>';
                $html_data .= '<p class="text-center chart-header" id="gauge1_name">'.$data['carpark_name'].'</p>';
                $html_data .= ' <input id="carpark'.$i.'" type="hidden" class="knob" data-thickness="0.3" data-angleArc="250" data-angleOffset="-125"
                                value="'.$gauge_value.'" data-width="250" data-height="150" data-fgColor="'.$this->choose_bg_color($gauge_value,1).'" data-readonly="true">';
                $html_data .= '<p class="gauge-val" id="gauge-value'.$i.'"><span>'.$data['current_level'].'</span>/<span>'.$data['total_spaces'].'</span></p>';         
                $html_data .= '<div class="card-body">';  
                $html_data .= '</div>';
                $html_data .= '<button type="button" class="btn btn-block btn-outline-secondary btn-sm" id="ShowCarparks" facility_number='.$data['facility_number'].'>More <i class="fa fa-arrow-circle-right"></i></button>'; 
                $html_data .= '</div>';   
                $html_data .= '</div>';       
                $i++;          
                }//wend  
            
            }//end if         
        mysqli_close($con);
        echo $html_data;           
        }// End . Facility Occupancy 


    function UpdateOccupancyCarparkCounters($facility_number)
        {        
        $con          = $this->db_connect();        
        $query_string = "select last_updated_datetime,carpark_name,total_spaces,current_level,shortterm_current_level,total_shortterm_spaces,access_current_level,total_access_spaces,reservation_current_level,total_reservation_spaces";
        $query_string .=" from counters where counter_type=1 and facility_number=".$facility_number." ORDER BY dashboard_order ASC";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $counts = array();              
        
        while ($data = mysqli_fetch_assoc($result)) 
            {         
            $gauge_value=($data['current_level']/$data['total_spaces'])*100;
            $color=$this->choose_bg_color($gauge_value,1);      
           
            $counts[] = array
                (
                'carpark_name' => "$data[carpark_name]",
                'gauge' => "$data[current_level]",
                'color' => "$color",
                'gauge_max' => "$data[total_spaces]",
                'shortterm_current_level' => "$data[shortterm_current_level]",
                'total_shortterm_spaces' => "$data[total_shortterm_spaces]",
                'access_current_level' => "$data[access_current_level]",
                'total_access_spaces' => "$data[total_access_spaces]",
                'reservation_current_level' => "$data[reservation_current_level]",
                'total_reservation_spaces' => "$data[total_reservation_spaces]",
                'last_updated_datetime' => "$data[last_updated_datetime]"
                );                    
            }        
        mysqli_close($con);
        echo json_encode($counts);                
        } // End . Function GetOccupancyCarparkCountersDashboard

    function UpdateOccupancyFacilityCounters()
        {       
        $con = $this->db_connect();        
        $query_string = "select total_spaces,current_level,last_updated_datetime from counters where counter_type=0";
        $query_string .=" ORDER by dashboard_order ASC";
        
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $counts = array();              
                
        while ($data = mysqli_fetch_assoc($result)) 
            {         
            $gauge_value=($data['current_level']/$data['total_spaces'])*100;            
            $color=$this->choose_bg_color($gauge_value,1);
            $counts[] = array
                (            
                'current_level' => "$data[current_level]",
                'color' => "$color",
                'total_spaces' => "$data[total_spaces]",
                'gauge_value'=>"$gauge_value",
                'last_updated_datetime' => "$data[last_updated_datetime]"
                );                    
            }
        mysqli_close($con);
        echo json_encode($counts);                
        } // End . Function GetOccupancyCarparkCountersDashboard
    
    

    function OccupancyCarparkCounters($facility_number)
        {       
        $_SESSION['dashboard_level']="carparks";    
        $_SESSION['facility_number']=$facility_number;   
        $con= $this->db_connect();       
        $query_string = "select last_updated_datetime,carpark_name,carpark_number,total_spaces,current_level,shortterm_current_level,total_shortterm_spaces,access_current_level,total_access_spaces,reservation_current_level,total_reservation_spaces";
        $query_string .=" from counters where counter_type=1 and facility_number=".$facility_number." ORDER BY dashboard_order ASC";
        
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $html_data = "";
        $i=1;
        $occupancy_percentage=0;  //temp variable 
        
        
        if(mysqli_num_rows($result)== 1)
            {
            //If only one carpark show the car park list directly 
             $data = mysqli_fetch_assoc($result);
             $this->OccupancyCarparkDetail($facility_number,$data['carpark_number']);
            }        
        else
            {          
            while ($data = mysqli_fetch_assoc($result)) 
                { 
                if($i==1)
                    {                    
                    $html_data .= '<input type="hidden" id="last_updated_datetime" value="'.$data['last_updated_datetime'].'">';             
                    }
                $gauge_value=($data['current_level']/$data['total_spaces'])*100;
                $color=$this->choose_bg_color($gauge_value,1);      
                
                $html_data .= ' <div class="col-lg-3 col-sm-6 col-xs-12">';
                $html_data .= ' <div class="chart-box text-center">';
                $html_data .= ' <span id="dashboard_level" level="carparks" facility_number="'.$facility_number.'"></span>';
                $html_data .= '<p class="text-center chart-header" id="gauge1_name">'.$data['carpark_name'].'</p>';
                $html_data .= ' <input id="carpark'.$i.'" type="hidden" class="knob" data-thickness="0.3" data-angleArc="250" data-angleOffset="-125"
                                value="'.$gauge_value.'" data-width="250" data-height="150" data-fgColor="'.$color.'" data-readonly="true">';
                $html_data .= '<p class="gauge-val" id="gauge-value'.$i.'"><span>'.$data['current_level'].'</span>/<span>'.$data['total_spaces'].'</span></p>';         
                $html_data .= '<div class="card-body">';   
                
                $html_data .= '<div id="row">';
                $html_data .= '<span id="category">ShortTerm</span>';     
                $html_data .='<span class="count" id="shortterm-count-'.$i.'">'.$data['shortterm_current_level'].'/'.$data['total_shortterm_spaces'].'</span><br/>';        
                $occupancy_percentage=($data['shortterm_current_level']/$data['total_shortterm_spaces'])*100;
                $html_data .= '<div class="progress mb-3">';
                $html_data .= '<div class="progress-bar bg-'.$this->choose_bg_color($occupancy_percentage,2).'" id="shortterm-progress-'.$i.'" role="progressbar" aria-valuenow="20" aria-valuemin="0" aria-valuemax="100" style="width:'.$occupancy_percentage.'%">';
                $html_data .= '</div>';//end row
                $html_data .= '</div>';
                $html_data .= '</div>';//end row
                
                $html_data .= '<div id="row">';
                $html_data .= '<span id="category">Access</span>';     
                $html_data .='<span class="count" id="access-count-'.$i.'">'.$data['access_current_level'].'/'.$data['total_access_spaces'].'</span><br/>';   
                $occupancy_percentage=($data['access_current_level']/$data['total_access_spaces'])*100;
                $html_data .= ' <div class="progress mb-3">';
                $html_data .= '<div class="progress-bar bg-'.$this->choose_bg_color($occupancy_percentage,2).'" id="access-progress-'.$i.'" role="progressbar" aria-valuenow="20" aria-valuemin="0" aria-valuemax="100" style="width:'.$occupancy_percentage.'%">';
                $html_data .= '</div>';//end row
                $html_data .= '</div>';
                $html_data .= '</div>';//end row
                
                $html_data .= '<div id="row">';
                $html_data .= '<span id="category">Reservation</span>';     
                $html_data .='<span class="count" id="reservation-count-'.$i.'">'.$data['reservation_current_level'].'/'.$data['total_reservation_spaces'].'</span><br/>';   
                $html_data .= ' <div class="progress mb-3">';
                $occupancy_percentage=($data['reservation_current_level']/$data['total_reservation_spaces'])*100;
                $html_data .= '<div class="progress-bar bg-'.$this->choose_bg_color($occupancy_percentage,2).'" id="reservation-progress-'.$i.'" role="progressbar" aria-valuenow="20" aria-valuemin="0" aria-valuemax="100" style="width:'.$occupancy_percentage.'%">';
                $html_data .= '</div>';//end row
                $html_data .= '</div>';
                $html_data .= '</div>';//end row
                
                $html_data .= '</div>';
                $html_data .= '<button type="button" class="btn btn-block btn-outline-secondary btn-sm" id="ShowCarparkDetail" facility_number="'.$facility_number.'" carpark_number="'.$data['carpark_number'].'">More <i class="fa fa-arrow-circle-right"></i></button>'; 
                $html_data .= '</div>';   

                $html_data .= '</div>';
                $i++;                          
                }//wend
            }//end if         
        mysqli_close($con);
        echo $html_data;         
        } // End Occupancy Overview
 

    function choose_bg_color($current_level,$type)
        {
        //type 1 = gauge , type 2 = progress and badge
        if($current_level <=50)
            {
            $bg="success";
            $color="#28a745"; // Green 
            }
        else if(($current_level >50) && ($current_level <=75))
            {
            $bg="warning";
            $color="#ffcd3c"; //Yellow
            }
        else if($current_level > 75)
            {
            $bg="danger";  
            $color="#dc3545"; //red
            }
        
        if($type==1)
            return $color;
        else if($type==2)
            return $bg;
        } // end choose bhg by occupancy level 

    function OccupancyCarparkDetail($facility_number,$carpark_number)
        {
        $con= $this->db_connect();
        $query_string = "select * from counters where counter_type=1 and facility_number=".$facility_number." AND carpark_number=".$carpark_number;   
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $html_data = "";
        $i=1;
        $data = mysqli_fetch_assoc($result);

        $gauge_percentage=($data['current_level']/$data['total_spaces'])*100;    
        $shortterm_occupancy=($data['shortterm_current_level']/$data['total_shortterm_spaces'])*100;   
        $access_occupancy=($data['access_current_level']/$data['total_access_spaces'])*100;   
        $reservation_occupancy=($data['reservation_current_level']/$data['total_reservation_spaces'])*100;  
        
        $html_data .= '<input type="hidden" id="last_updated_datetime" value="'.$data['last_updated_datetime'].'">';     
        $html_data .= ' <span id="dashboard_level" level="carpark_detail"  carpark_number="'.$carpark_number.'" facility_number="'.$facility_number.'"></span>';
        
        $html_data .= '<div class="container-fluid">';
        $html_data .= '<div class="row mb-2">';
        $html_data .= '<div class="col-sm-6">';
        $html_data .= '<h1>'.$data['carpark_name'].'</h1>';
        $html_data .= '</div>';   
        $html_data .= '</div>';
        $html_data .='</div>';    
   
        //The Occupancy Gauge - Total 
        $html_data .= '<div class="col-md-4">';
        $html_data .= ' <div class="chart-box text-center">';
        $html_data .= '<p class="text-center chart-header" id="gauge1_name">'.$data['carpark_name'].'</p>';
        $html_data .= ' <input id="carpark" type="hidden" class="knob" data-thickness="0.3" data-angleArc="250" data-angleOffset="-125"
        value="'.$gauge_percentage.'" data-width="250" data-height="150" data-fgColor="'.$this->choose_bg_color($gauge_percentage,1).'" data-readonly="true">';
        $html_data .= '<p class="gauge-val" id="gauge-value"><span>'.$data['current_level'].'</span>/<span>'.$data['total_spaces'].'</span></p>';  
        $html_data .= '  </div>';
        $html_data .= '  </div>';//col md 4

        //Occupancy progress bar by type 

        $html_data .= '<div class="col-md-4">';
        $html_data .= '<div class="card">';
        $html_data .= '<div class="card-body p-0">';
        $html_data .= '<table class="table table-striped">';
        $html_data .= '<tr  style="height: 25%">';
        $html_data .= '<th style="width: 10px"></th>';
        $html_data .= '<ths tyle="width: 30%"></th>';
        $html_data .= '<th style="width: 50%">Occupancy</th>';
        $html_data .= '<th style="width: 20%">Current/Total</th>';
        $html_data .= '</tr>';
        
        $html_data .= '<tr>';    
        $html_data .= '<td>ShortTerm</td>';
        $html_data .= '<td>';
        $html_data .= '<div class="progress progress-xs">';  
        $html_data .= ' <div id="progress-shortterm" class="progress-bar bg-'.$this->choose_bg_color($shortterm_occupancy,2).'" style="width: '.$shortterm_occupancy.'%"></div>';
        $html_data .= '</div>';
        $html_data.='</td>';
        $html_data .= '<td><span id="badge-shortterm" class="badge bg-'.$this->choose_bg_color($shortterm_occupancy,2).'">'.$data['shortterm_current_level'].'/'.$data['total_shortterm_spaces'].'</span></td>';
        $html_data .= '</tr>';
        
        $html_data .= '<tr>';    
        $html_data .= '<td>Access</td>';
        $html_data .= '<td>';
        $html_data .= '<div class="progress progress-xs">';
    
        $html_data .= ' <div id="progress-access" class="progress-bar bg-'.$this->choose_bg_color($access_occupancy,2).'" style="width: '.$access_occupancy.'%"></div>';
        $html_data .= '</div>';
        $html_data.='</td>';
        $html_data .= '   <td><span id="badge-access" class="badge bg-'.$this->choose_bg_color($access_occupancy,2).'">'.$data['access_current_level'].'/'.$data['total_access_spaces'].'</span></td>';
        $html_data .= '  </tr>';
        
        $html_data .= '  <tr>';    
        $html_data .= '  <td>Reservation</td>';
        $html_data .= '  <td>';
        $html_data .= '  <div class="progress progress-xs">';
    
        $html_data .= ' <div id="progress-reservation" class="progress-bar bg-'.$this->choose_bg_color($reservation_occupancy,2).'" style="width: '.$reservation_occupancy.'%"></div>';
        $html_data .= '</div>';
        $html_data.='</td>';
        $html_data .= '   <td><span id="badge-reservation" class="badge bg-'.$this->choose_bg_color($reservation_occupancy,2).'">'.$data['reservation_current_level'].'/'.$data['total_reservation_spaces'].'</span></td>';
        $html_data .= '  </tr>';
        
        
        $html_data .= '  </table>';
        $html_data .= ' </div>';

        $html_data .= '  </div>';
        $html_data .= '  </div>'; //col md 4
        
        
        //Entries and Exits By type
        $html_data .= '<div class="col-md-4">';
        $html_data .= '<div class="card">';
        $html_data .= '<div class="card-body p-0">';
        $html_data .= '<table class="table table-striped">';
        $html_data .= '  <tr>';    
        $html_data .= '   <td></td>';
        $html_data .= '   <td>Entries</td>';
        $html_data .= '   <td>Exits</td>';
        $html_data .= '  </tr>';
        $html_data .= '  <tr>';    
        $html_data .= '   <td>ShortTerm</td>';
        $html_data .= '   <td id="shortterm-entry">'.$data['shortterm_entry'].'</td>';
        $html_data .= '   <td id="shortterm-exit">'.$data['shortterm_exit'].'</td>';
        $html_data .= '  </tr>';
        $html_data .= '  <tr>';    
        $html_data .= '   <td>Access</td>';
        $html_data .= '   <td id="access-entry">'.$data['access_entry'].'</td>';
        $html_data .= '   <td id="access-exit">'.$data['access_exit'].'</td>';
        $html_data .= '  </tr>';

        $html_data .= '  <tr>';    
        $html_data .= '   <td>Reservation</td>';
        $html_data .= '   <td id="reservation-entry">'.$data['reservation_entry'].'</td>';
        $html_data .= '   <td id="reservation-exit">'.$data['reservation_exit'].'</td>';
        $html_data .= '  </tr>';
    
        $html_data .= '  <tr>';    
        $html_data .= '   <td>Manual</td>';
        $html_data .= '   <td id="manual-entry">'.$data['total_manual_entry'].'</td>';
        $html_data .= '   <td id="manual-exit">'.$data['total_manual_exit'].'</td>';
        $html_data .= '  </tr>'; 
    

        $html_data .= '  </table>';
        $html_data .= ' </div>';

        $html_data .= '  </div>';
        $html_data .= '  </div>'; //col md 4     
        mysqli_close($con);
        echo $html_data;           
        }// End . Occupancy Carpark Detail 


    function UpdateOccupancyCarparkDetail($facility_number,$carpark_number)
        {
        $con= $this->db_connect();
        $query_string = "select * from counters where counter_type=1 and facility_number=".$facility_number." and carpark_number=".$carpark_number;  
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));    
        $data = mysqli_fetch_assoc($result);
        $gauge_percentage=($data['current_level']/$data['total_spaces'])*100;    
        $json->occupancy_percentage=$data['occupancy_percentage'];
        $json->gauge_color=$this->choose_bg_color($gauge_percentage,1);
        $json->current_level=$data['current_level'];
        $json->total_spaces=$data['total_spaces'];
        $json->shortterm_current_level =$data['shortterm_current_level'];
        $json->shortterm_total_spaces=$data['total_shortterm_spaces'];
        $json->access_current_level =$data['access_current_level'];
        $json->access_total_spaces=$data['total_access_spaces'];
        $json->reservation_current_level =$data['reservation_current_level'];
        $json->reservation_total_spaces=$data['total_reservation_spaces'];

        $json->shortterm_entries=$data['shortterm_entry'];
        $json->shortterm_exits=$data['shortterm_exit'];
        $json->access_entries=$data['access_entry'];
        $json->access_exits=$data['access_exit'];
        $json->reservation_entries=$data['reservation_entry'];
        $json->reservation_exits=$data['reservation_exit'];
        $json->manual_entries=$data['total_manual_entry'];
        $json->manual_exits=$data['total_manual_exit'];
        $json->last_updated_datetime=$data['last_updated_datetime'];

        mysqli_close($con);
        echo json_encode($json);         
        }// end . Function . Dashboard Occupancy CarparkDetail

    function OccupancyHourly($carpark, $type)
        {
        $total_count = array();
        $i           = 0;
        $j           = 0;
        $k           = 1;

	    $con=$this->db_connect_reporting();
    
        if (!$con)        
            $this->error("Connection attempt failed");          
         
        //Start . Generate the SQL Query String . 
        $query_string = "select";
        while ($i < 24) 
            {
            $query_string .= " sum(h" . $i . "to" . ($i + 1) . ") as h" . $i . "to" . ($i + 1);
            if($i<23)
                {
                $query_string .= ","; 
                }
            $i++;
            }
       
        
        $query_string .= " from hourly_occupancy where report_date =curdate() and type='" . $type . "'";
        
        if ($carpark > 0) 
            {
            $query_string.="and carpark_number=".$carpark;           
            }                       
                
        
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        
        if (mysqli_num_rows($result) > 0) 
            {
            $data           = mysqli_fetch_assoc($result);
            $i              = 0;
            $j              = 0;
            //get count for hours
            while ($i < 24) 
                {
                $field           = "h" . $i . "to" . ($i + 1);
                $total_count[$j] = (int) $data[$field];
                $i++;
                $j++;
                }
            mysqli_close($con);
            return $total_count;            
            } 
        else 
            {
            $i = 0;
            while ($i < 24) 
                {
                $total_count[$i] = 0;
                $i++;
                }            
            return $total_count;
            }        
        } // End Hourly Occupancy

    } // End Class
