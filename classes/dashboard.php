<?php

class dashboard {

    function db_connect() {
        $con = mysqli_connect(DB_HOST, DB_USER, DB_PASSWORD, DB_NAME_DASHBOARD);

        if (!$con)
            $this->error("Connection attempt failed");
        else
            return $con;
    }


    function db_connect_config() {
        $con = mysqli_connect(DB_HOST, DB_USER, DB_PASSWORD, DB_NAME);
        if (!$con)
            $this->error("Connection attempt failed");
        else
            return $con;
    }


    function db_connect_reporting() {
        $con = mysqli_connect(DB_HOST, DB_USER, DB_PASSWORD, DB_NAME_REPORTING);
        if (!$con)
            $this->error("Connection attempt failed");
        else
            return $con;
    }


    function getFacilityFeatures() {       
        $con = $this->db_connect_config();
        if ($con) {
            $query_string = "select setting_name,setting_value from system_settings where setting_name in('currency','decimal_places','vat_percentage','facility_name','address_line1','address_line2','vat_id')";
            $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
            while ($row = mysqli_fetch_assoc($result)) {
                $data[$row["setting_name"]] = $row["setting_value"];
            }
            mysqli_close($con);
        }
        return $data;
    } 
           
    function live_revenue_facility() {
        $con = $this->db_connect();
        $query_string = "select facility_number,sum(gross_amount) as gross_amount,sum(parking_fee-discount_amount) as parking_fee,sum(vat_amount) as vat_amount,sum(lost_ticket_fee+admin_fixed_charges+ticket_replacement_fee) as lost_fee,sum(discount_amount) as discount_amount,sum(product_sale_amount) as product_sale_amount from parking_revenue group by facility_number";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $i=0;
        while ($data = mysqli_fetch_assoc($result)) {
            $row["facility_number"]=$data["facility_number"];
            $row["gross_amount"] = $data["gross_amount"];
            $row["parking_fee"] = $data["parking_fee"];
            $row["vat_amount"] = $data["vat_amount"];
            $row["lost_fee"] = $data["lost_fee"];
            $row["product_sale_amount"] = $data["product_sale_amount"];
            $response[$i]=$row;
            $i++;
        }
        mysqli_close($con);
        echo json_encode($response);
    }
    
     function live_revenue_carpark($facility_number) {
        $con = $this->db_connect();
        $query_string = "select carpark_number,sum(gross_amount) as gross_amount,sum(parking_fee-discount_amount) as parking_fee,sum(vat_amount) as vat_amount,sum(lost_ticket_fee+admin_fixed_charges+ticket_replacement_fee) as lost_fee,sum(discount_amount) as discount_amount,sum(product_sale_amount) as product_sale_amount from parking_revenue where facility_number=".$facility_number." group by carpark_number";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $i=0;
        while ($data = mysqli_fetch_assoc($result)) {
            $row["carpark_number"]=$data["carpark_number"];
            $row["gross_amount"] = $data["gross_amount"];
            $row["parking_fee"] = $data["parking_fee"];
            $row["vat_amount"] = $data["vat_amount"];
            $row["lost_fee"] = $data["lost_fee"];
            $row["product_sale_amount"] = $data["product_sale_amount"];
            $response[$i]=$row;
            $i++;
        }
        mysqli_close($con);
        echo json_encode($response);
    }
    
      function live_revenue_device($facility_number,$carpark_number) {
        $con = $this->db_connect();
        $query_string = "select sum(gross_amount) as gross_amount,sum(parking_fee-discount_amount) as parking_fee,sum(vat_amount) as vat_amount,sum(lost_ticket_fee+admin_fixed_charges+ticket_replacement_fee) as lost_fee,sum(discount_amount) as discount_amount,sum(product_sale_amount) as product_sale_amount from parking_revenue where facility_number=".$facility_number." and carpark_number=".$carpark_number;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $i=0;
        if ($data = mysqli_fetch_assoc($result)) {            
            $row["gross_amount"] = $data["gross_amount"];
            $row["parking_fee"] = $data["parking_fee"];
            $row["vat_amount"] = $data["vat_amount"];
            $row["lost_fee"] = $data["lost_fee"];
            $row["product_sale_amount"] = $data["product_sale_amount"];
            
            $i++;
        }
        mysqli_close($con);
        echo json_encode($row);
    }
    
    function show_live_revenue_facility()
        {        
        $con = $this->db_connect();
        $query_string = "select facility_number,facility_name,count(*) as devices,sum(gross_amount) as gross_amount,sum(parking_fee-discount_amount) as parking_fee,"
                . "sum(vat_amount) as vat_amount,sum(lost_ticket_fee+admin_fixed_charges+ticket_replacement_fee) as lost_fee,"
                . "sum(discount_amount) as discount_amount,sum(product_sale_amount) as product_sale_amount from parking_revenue group by facility_number";
        
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        
        if (mysqli_num_rows($result) == 1) 
            {            
            $data = mysqli_fetch_assoc($result);
            $facility_number=$data["facility_number"];
            mysqli_close($con);            
            $this->show_live_revenue_carpark($facility_number);
            }
        else
            {  
            $facility=$this->getFacilityFeatures();
            $html='<input type="hidden" id="vat" value="'.$facility["vat_percentage"].'">';
            while ($data = mysqli_fetch_assoc($result)) 
                {                
                $html.='<div class="finance-facility col-md-4">';
                $html.='<div class="card ">';
                $html.='<div class="card-body box-profile">';
                $html.='<h3 class="profile-username text-center">'.$data["facility_name"].'</h3>';
                $html.='<p class="text-muted text-center">'.$data["facility_number"].'</p>';                                                                                                
                $html.='<canvas facility-number="'.$data["facility_number"].'" class="donutChart" style="min-height: 250px; height: 250px; max-height: 250px; max-width: 100%;"></canvas>';

                $html.='<ul class="list-group list-group-unbordered mb-3 mt-3">';                                
                $html.='<li class="list-group-item"><b>Total payment devices</b> <a class="float-right">'.$data["devices"].'</a></li>';            
                $html.='<li class="list-group-item"><b>Gross Amount</b> <a class="float-right">'.$data["gross_amount"].' '.$facility["currency"].'</a></li>';
                $html.='<li class="list-group-item"><b>Parking Fee</b> <a class="float-right">'.$data["parking_fee"].' '.$facility["currency"].'</a></li>';
                $html.='<li class="list-group-item"><b>Lost fee</b> <a class="float-right">'.$data["lost_fee"].' '.$facility["currency"].'</a></li>';
                $html.='<li class="list-group-item"><b>Product sale</b> <a class="float-right">'.$data["product_sale_amount"].' '.$facility["currency"].'</a></li>';
                $html.='<li class="list-group-item"><b>Vat amount</b> <a class="float-right">'.$data["vat_amount"].' '.$facility["currency"].'</a></li>';
                $html.='<li class="list-group-item"><b>Discount amount</b> <a class="float-right">'.$data["discount_amount"].' '.$facility["currency"].'</a></li>';
                $html.='</ul>';
                $html.='<button type="button" class="show-facility-details btn btn-block bg-secondary-gradient" facility_number="'.$data["facility_number"].'">More <i class="fa fa-arrow-circle-right"></i></button>';
                $html.='</div>';
                $html.='</div>';
                $html.='</div>';                
                }
            echo $html;
            mysqli_close($con);
            }
        }
        
        function show_live_revenue_carpark($facility_number)
        {
        
        $con = $this->db_connect();
        $query_string = "select facility_number,carpark_number,carpark_name,count(*) as devices,sum(gross_amount) as gross_amount,sum(parking_fee-discount_amount) as parking_fee,"
                . "sum(vat_amount) as vat_amount,sum(lost_ticket_fee+admin_fixed_charges+ticket_replacement_fee) as lost_fee,"
                . "sum(discount_amount) as discount_amount,sum(product_sale_amount) as product_sale_amount from parking_revenue where facility_number=".$facility_number." group by carpark_number";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));              
        if (mysqli_num_rows($result) == 1) 
            {            
            $data = mysqli_fetch_assoc($result);
            $carpark_number=$data["carpark_number"];
            mysqli_close($con);
            $html="<input type='hidden' id='facility_number' value='".$facility_number."'>";
            $html.="<input type='hidden' id='carpark_number' value='".$carpark_number."'>";
            echo $html;
            //$this->get_revenue_summary($facility_number,$carpark_number);
            }
        else
            {              
            $facility=$this->getFacilityFeatures();  
            $html="<input type='hidden' id='facility_number' value='".$facility_number."'>";
            while ($data = mysqli_fetch_assoc($result)) 
                {                
                $html.='<div class="finance-carpark col-md-4">';
                $html.='<div class="card ">';
                $html.='<div class="card-body box-profile">';
                $html.='<h3 class="profile-username text-center">'.$data["carpark_name"].'</h3>';
                $html.='<p class="text-muted text-center">'.$data["carpark_number"].'</p>';                                                                                                
                $html.='<canvas facility-number="'.$data["facility_number"].'" carpark-number="'.$data["carpark_number"].'" class="donutChart" style="min-height: 250px; height: 250px; max-height: 250px; max-width: 100%;"></canvas>';

                $html.='<ul class="list-group list-group-unbordered mb-3 mt-3">';                                
                $html.='<li class="list-group-item"><b>Total payment devices</b> <a class="float-right">'.$data["devices"].'</a></li>';            
                $html.='<li class="list-group-item"><b>Gross Amount</b> <a class="float-right">'.$data["gross_amount"].' '.$facility["currency"].'</a></li>';
                $html.='<li class="list-group-item"><b>Parking Fee</b> <a class="float-right">'.$data["parking_fee"].' '.$facility["currency"].'</a></li>';
                $html.='<li class="list-group-item"><b>Lost fee</b> <a class="float-right">'.$data["lost_fee"].' '.$facility["currency"].'</a></li>';
                $html.='<li class="list-group-item"><b>Product sale</b> <a class="float-right">'.$data["product_sale_amount"].' '.$facility["currency"].'</a></li>';
                $html.='<li class="list-group-item"><b>Vat amount</b> <a class="float-right">'.$data["vat_amount"].' '.$facility["currency"].'</a></li>';
                $html.='<li class="list-group-item"><b>Discount amount</b> <a class="float-right">'.$data["discount_amount"].' '.$facility["currency"].'</a></li>';
                $html.='</ul>';
                $html.='<button type="button" class="show-carpark-details btn btn-block bg-secondary-gradient" facility_number="'.$data["facility_number"].'" carpark_number="'.$data["carpark_number"].'">More <i class="fa fa-arrow-circle-right"></i></button>';
                $html.='</div>';
                $html.='</div>';
                $html.='</div>';                
                }
            echo $html;
             mysqli_close($con);
            }
        }


    function get_revenue_summary($facility_number,$carpark_number) {
        $fdata = $this->getFacilityFeatures();
        $d = $fdata["decimal_places"];
        $currency = $fdata["currency"];
        $vat = $fdata["vat_percentage"];

        $con = $this->db_connect();    
        
        $query_string="select sum(parking_fee) as parking_fee,sum(lost_ticket_fee) as lost_fee,sum(gross_amount) as earnings,sum(vat_amount) as vat,sum(product_sale_amount) as product_sale,sum(discount_amount) as discount_amount from parking_revenue where carpark_number=" . $carpark_number . " and facility_number=".$facility_number;

        


        $result=mysqli_query($con,$query_string) or die(mysqli_error($con));    
        
        $data_summary=mysqli_fetch_assoc($result);    
               
        $html_data='<div class="row mb-4 jspdf-graph">';
        $html_data.='<div class="col">';    
        $html_data.='<div class="small-box bg-success">';
        $html_data.='<div class="inner">';
        $html_data.='<h3>'.$currency." ".number_format($data_summary["earnings"],$d).'</h3>';
        $html_data.='<p>Total Revenue</p>';
        $html_data.='</div>';     		
        $html_data.='</div>';
        $html_data.='</div>';


        $html_data.='<div class="col">';		
        $html_data.='<div class="small-box box-color-parking-fee">';
        $html_data.='<div class="inner">';
        $html_data.='<h3>'.$currency." ".number_format($data_summary["parking_fee"],$d).'</h3>';
        $html_data.='<p>Parking Fee</p>';
        $html_data.='</div>';     		
        $html_data.='</div>';
        $html_data.='</div>';

        $html_data.='<div class="col">';    
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

        $html_data.='<div class="col">';		
        $html_data.='<div class="small-box box-color-product-fee">';
        $html_data.='<div class="inner">';
        $html_data.='<h3>'.$currency." ".number_format($data_summary["product_sale"],$d).'</h3>';
        $html_data.='<p>Product Sale Amount</p>';
        $html_data.='</div>';	
        $html_data.='</div>';
        $html_data.='</div>';

         if($vat!=0)
             {
             $html_data.='<div class="col">';		
             $html_data.='<div class="small-box bg-success">';
             $html_data.='<div class="inner">';
             $html_data.='<h3>'.$currency." ".number_format($data_summary["vat"],$d).'</h3>';
             $html_data.='<p>VAT</p>';
             $html_data.='</div>';
             $html_data.='</div>';
             $html_data.='</div>';
             }

        $html_data.='<div class="col">';
        $html_data.='<!-- small box -->';
        $html_data.='<div class="small-box box-color-discount-fee">';
        $html_data.='<div class="inner">';
        $html_data.='<h3>'.$currency." ".number_format($data_summary["discount_amount"],$d).'</h3>';
        $html_data.='<p>Discount Amount</p>';
        $html_data.='</div>';
        $html_data.='</div>';
        $html_data.='</div>';

        $html_data.='</div><!-- Row End -->';
        $html_data.='</div>';

        

        echo $html_data;
       
        $html_data = "";
        $header = "<div class='card'>";

        $header .= '<table class="jspdf-table table table-blue table-bordered" data-status="table-view">';
        $header .= '<thead><tr>';

        $header .= '<th>Device Name</th>';
        $header .= '<th>Total Revenue</th>';
        $header .= '<th>Parking Fee</th>';
        $header .= '<th>Product Sale Amount</th>';
        $header .= '<th>VAT Amount</th>';
        $header .= '<th>Lost Ticket Fee</th>';
        $header .= '<th>Discount Amount</th>';
        $header .= '<th>Payable Entries</th>';
        $header .= '<th>Lost Ticket Count</th>';
        $header .= '<th>Discount Count</th>';


        $header .= '</tr></thead>';
        $html_data=$header;

        
        $query_string = "select * from parking_revenue where carpark_number=" . $carpark_number . " and facility_number=".$facility_number;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));

        $html_data .= '<tbody>';

        while ($data = mysqli_fetch_assoc($result)) 
            {
            $data_status = "all";
            switch ($data["device_type"]) 
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

            $html_data .= '<tr data-status="' . $data_status . '">';

            $html_data .= '<td class="text-left text-dark table-row-detail">';
            $html_data .= '<h6>';
                if ($data["network_status"] == 1) {
                    $html_data .= '<div class="dot-indicator bg-success-gradient" data-toggle="tooltip" data-placement="top" title="Online" ></div>';
                } else {
                    $html_data .= '<div class="dot-indicator bg-danger-gradient" data-toggle="tooltip" data-placement="top" title="Currently Off line"></div>';
                }
                $html_data .= $data["device_name"] . '</h6>';
                $html_data .= '<small class="form-text text-muted">Last transaction:'.$data["last_transaction"].' </small>';
                $html_data .= '</td>';
                $html_data .= '<td>' . number_format($data["gross_amount"], $d) . '</td>';
                $html_data .= '<td>' . number_format($data["parking_fee"], $d) . '</td>';
                $html_data .= '<td>' . number_format($data["product_sale_amount"], $d) . '</td>';
                $html_data .= '<td>' . number_format($data["vat_amount"], $d) . '</td>';
                $html_data .= '<td>' . number_format($data["lost_ticket_fee"], $d) . '</td>';
                $html_data .= '<td>' . number_format($data["discount_amount"], $d) . '</td>';
                $html_data .= '<td>' . $data["payable_entries_count"] . '</td>';
                $html_data .= '<td>' . $data["lost_ticket_count"] . '</td>';
                $html_data .= '<td>' . $data["discount_count"] . '</td>';
                $html_data .= '</tr>';
            }
            
        $html_data .= '</tbody>';
        $html_data .= '</table></div>';

        mysqli_close($con);
               
        echo $html_data;
    }

// End Function . Live Revenue 

    
    function revenue_lastdays() {
        $con = $this->db_connect_reporting();
        $query_string = "select report_date,report_day,sum(gross_amount) as gross_amount from summary_daily_revenue where
        report_date between DATE(NOW())-INTERVAL 7 DAY and DATE(NOW())- INTERVAL 1 DAY group by report_date ";

        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $response = array();
        while ($data = mysqli_fetch_assoc($result)) {
            $temp = [
                "day" => $data["report_day"],
                "amount" => $data["gross_amount"],
                "date" => $data["report_date"]
            ];
            array_push($response, $temp);
        }
        mysqli_close($con);
        $con = $this->db_connect();
        $query_string = "select sum(gross_amount) as gross_amount from parking_revenue";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        if ($data = mysqli_fetch_assoc($result)) {
            $temp = [
                "day" => date('l'),
                "amount" => $data["gross_amount"],
                "date" => date("Y-m-d")
            ];
            array_push($response, $temp);
        }
        mysqli_close($con);
        echo json_encode($response);
    }

    function getLastUpdatedDateForOccupancy() {
        $con = $this->db_connect();
        $query_string = "select max(last_updated_datetime) as last_updated_date_time from counting_carpark";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        if ($data = mysqli_fetch_assoc($result)) {
            echo $data["last_updated_date_time"];
        }
        mysqli_close($con);
    }

    function watchdog_device_alarms($FromDateTime, $ToDateTime, $device, $severity) {

        $con = $this->db_connect_reporting();
        //$query_string="select * from watchdog_network_logs";
        $query_string = "select * from watchdog_device_alarms where alarm_date_time between '" . $FromDateTime . "' AND '" . $ToDateTime . "'";
        if ($device != "")
            $query_string .= " AND device_number in(" . $device . ") ";
        if ($severity != "0")
            $query_string .= " AND alarm_severity " . $severity . " ";
        $query_string .= "ORDER BY alarm_date_time DESC";

        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));

        if (mysqli_num_rows($result) > 0) {

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

            while ($data = mysqli_fetch_assoc($result)) {
                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center">';
                $severity = "";
                if ($data["alarm_severity"] >= 1 && $data["alarm_severity"] <= 3)
                    $severity = "<span class='badge badge-danger'>High</span>";
                if ($data["alarm_severity"] >= 4 && $data["alarm_severity"] <= 7)
                    $severity = "<span class='badge badge-warning'>Medium</span>";
                if ($data["alarm_severity"] >= 8 && $data["alarm_severity"] <= 10)
                    $severity = "<span class='badge badge-secondary'>Low</span>";
                $html_data .= "<td class='col'>" . $data['device_name'] . " </td>";
                $html_data .= "<td class='col'>" . $severity . " </td>";
                $html_data .= "<td class='col'>" . $data['alarm_date_time'] . " </td>";
                $html_data .= "<td class='col'>" . $data['alarm_description'] . " </td>";
                //$html_data .= "<td class='col'>" . $data['carpark_name'] . " </td>";

                $html_data .= '</tr>';
            }


            $html_data .= '</table>';
        } else {
            $html_data = "<div class='p-3'>No records/transactions available for the current search criteria</div>";
        }
        mysqli_close($con);
        echo $html_data;
    }

// End . alarms

    function getDeviceAlarmscount($facility_number,$carpark_number,$device, $alarmseverity1, $alarmseverity2) {
        $con = $this->db_connect_reporting();
        $query_string = "select * from watchdog_device_alarms where device_number='" . $device . "' and facility_number=".$facility_number." and carpark_number=".$carpark_number." and alarm_severity between
        " . $alarmseverity1 . " and " . $alarmseverity2 . " and alarm_date_time>=DATE_ADD(CURDATE(),INTERVAL -7 DAY)";
        //echo $query_string;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
       
                
        $rows = mysqli_num_rows($result);
        mysqli_close($con);
        return $rows;
    }
    
    function get_alarm_count($facility,$carpark,$device, $alarmseverity1, $alarmseverity2) {
        $con = $this->db_connect_reporting();
        $query_string = "select count(*) as count from watchdog_device_alarms where alarm_code%2=1 and dismiss=0 and alarm_severity between " . $alarmseverity1 . " and " . $alarmseverity2 . " and alarm_date_time>=DATE_ADD(CURDATE(),INTERVAL -7 DAY)";
        
        if($facility>0)
            $query_string.=" and facility_number='" . $facility . "'";
        if($carpark>0)
            $query_string.=" and carpark_number=".$carpark;
        if($device>0)
            $query_string.=" and device_number in (".$device.")";
        
        $query_string.=" GROUP by device_number,alarm_code";
        
        //echo $query_string."<br>";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $rows=0;
        if ($data = mysqli_fetch_assoc($result))                  
            $rows = $data["count"];
        
        //echo $rows."<br>";
        mysqli_close($con);
        return $rows;
    }

    function get_latest_alarms_count() {
        $con = $this->db_connect_reporting();
        $query_string = "select * from watchdog_device_alarms where alarm_severity between 1 and 10 and dismiss=0 and alarm_date_time>=DATE_ADD(CURDATE(),INTERVAL -7 DAY)";
        //echo $query_string;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));

        $rows = mysqli_num_rows($result);
        mysqli_close($con);
        echo $rows;
    }

    function dismiss_alarm($id) {
        $con = $this->db_connect_reporting();
        $query_string ="select alarm_code,device_number from watchdog_device_alarms where id=" . $id;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        if($data = mysqli_fetch_assoc($result)) 
            {
            $device_number=$data["device_number"];
            $code=$data["alarm_code"];
            $query_string = "update watchdog_device_alarms set dismiss=1 where device_number=".$device_number." and alarm_code=".$code;        
            $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
            }
        mysqli_close($con);
    }

    function get_alarm_list($data) {
        $device=$data["device_number"];
        $carpark=$data["carpark_number"];
        $facility=$data["facility_number"];
        
        $con = $this->db_connect_reporting();
        $query_string = "select * from watchdog_device_alarms where alarm_date_time>=DATE_ADD(CURDATE(),INTERVAL - 7 DAY)";       
        if($facility>0)
            $query_string.=" and facility_number='" . $facility . "'";
        if($carpark>0)
            $query_string.=" and carpark_number=".$carpark;
        if($device>0)
            $query_string.=" and device_number in (".$device.")";
        
        $query_string.=" GROUP by device_number,alarm_code";
            
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $htmldata = "";
        if (mysqli_num_rows($result) > 0) {
            
            $htmldata .= '<table id="RecordsTable" class="table table-blue table-bordered jspdf-table">';
            $htmldata .= '<thead><tr>';            
            if($device==0)                
                $htmldata .= ' <th>Device name</th>';
            $htmldata .= '<th>Alarm Severity</th>';            
            $htmldata .= '<th>Report Date</th>';
            $htmldata .= '<th>Alarm code</th>';
            $htmldata .= ' <th>Description</th>';
            $htmldata .= ' <th>Comment</th>';
            
            $htmldata .= ' <th></th>';
                
            $htmldata .= '</tr>';
            $htmldata .= '</thead>';
            $htmldata .= '<tbody>';
        
        
        while ($data = mysqli_fetch_assoc($result)) {   
            if($device==0)                
                $htmldata .= '<td>' . $data["device_name"] . '</td>';
            $htmldata .= '<td>';
            $button = "";
            if ($data["alarm_severity"] < 6)
                $button = "danger";
            if (($data["alarm_severity"] >= 6))
                $button = "warning";
            $htmldata .= '<div class="dot-indicator bg-' . $button . '-gradient"></div>' . $data["alarm_severity"] . '
            </td>';
            $htmldata .= '<td>' . $data["alarm_date_time"] . '</td>';
            $htmldata .= '<td>' . $data["alarm_code"] . '</td>';
            $htmldata .= '<td>' . $data["alarm_description"] . '</td>';
            $dismiss = "";
            if ($data["dismiss"] == 1)
                $dismiss = "Dismissed";
            $htmldata .= '<td>' . $dismiss . '</td>';   
            if ($data["dismiss"] == 1)
                $htmldata .= ' <td></td>';
            else
                $htmldata .= ' <td><button type="button" class="btn btn-danger btn-dismis-alarm" id="' . $data["id"] . '">Dismiss</button></td>';
                
            $htmldata .= ' </tr>';
        }
        $htmldata .= '</tbody></table>';
        }
       else
            $htmldata="<strong>No alarms</strong>";
        mysqli_close($con);
        echo $htmldata;
    }

    function get_latest_alarm_list($data) {
        $con = $this->db_connect_reporting();
        $query_string = "select * from watchdog_device_alarms where alarm_severity between 1 and 10 and dismiss=0 and alarm_date_time>=DATE_ADD(CURDATE(),INTERVAL -7 DAY) order by alarm_date_time desc";
        //echo $query_string;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $htmldata = "";
        if (mysqli_num_rows($result) > 0) {
            $htmldata .= '<tbody>';
            $htmldata .= '<tr class="card-header d-flex justify-content-between align-items-center">';
            $htmldata .= '<th class="col">Alarm Severity</th>';
            $htmldata .= '<th class="col">Device name</th>';
            $htmldata .= '<th class="col">Report Date</th>';
            $htmldata .= '<th class="col">Alarm code</th>';
            $htmldata .= ' <th class="col">Comments</th>';
            $htmldata .= '<th class="col"></th></tr>';
            $htmldata .= '</tbody>';
            $htmldata .= '<tbody class="table-view">';
        }
        while ($data = mysqli_fetch_assoc($result)) {
            $htmldata .= '<tr class="card-text d-flex justify-content-between align-items-center">';
            $htmldata .= '<td class="col">';
            $button = "";
            if ($data["alarm_severity"] < 5)
                $button = "danger";
            if (($data["alarm_severity"] >= 5))
                $button = "warning";
            $htmldata .= '<div class="dot-indicator bg-' . $button . '-gradient"></div>' . $data["alarm_severity"] . '
            </td>';
            $htmldata .= '<td class="col">' . $data["device_name"] . '</td>';
            $htmldata .= '<td class="col">' . $data["alarm_date_time"] . '</td>';
            $htmldata .= '<td class="col">' . $data["alarm_code"] . '</td>';
            $htmldata .= '<td class="col">' . $data["alarm_description"] . '</td>';
            $htmldata .= ' <td class="col"><button type="button" class="btn btn-danger btn-dismis-alarm" id="' . $data["id"] . '">Dismiss</button>
            </td>
            </tr>';
        }
        $htmldata .= '</tbody>';
        mysqli_close($con);
        echo $htmldata;
    }
    
    function get_device_status_by_facility()
        {        
        $html="";
        $facility_number=0;
        $con = $this->db_connect();
        $query_string = "select facility_number,facility_name,count(DISTINCT carpark_number) as carparks from watchdog_device_status group by facility_number";        
        $facility_result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $rows=mysqli_num_rows($facility_result);        
        if($rows==1)
            {
            $data = mysqli_fetch_assoc($facility_result);
            $facility_number=$data["facility_number"];
            $carparks=$data["carparks"];
            mysqli_close($con);              
            if($carparks>1)
                {
                $html="<input type='hidden' id='facility_number' value='".$facility_number."'>";            
                echo $html;
                }
            else
                $this->get_device_status_by_carpark($facility_number);           
            }
        else
            {
            while ($facility = mysqli_fetch_assoc($facility_result)) 
                {
                $facility_number=$facility["facility_number"];
                $facility_name=$facility["facility_name"];
                //if($rows>1)
                    {
                    $query_string ="select count(*) as devices,count(distinct carpark_number) as carparks from watchdog_device_status where facility_number=".$facility_number;
                    $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
                    if ($data = mysqli_fetch_assoc($result)) 
                        {
                        $devices=$data["devices"];
                        $carparks=$data["carparks"];
                        }
                    $query_string ="select device_network_status,count(*) as status from watchdog_device_status where facility_number=".$facility_number." GROUP by device_network_status";
                    $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
                    $online=0;
                    $offline=0;
                    while ($data = mysqli_fetch_assoc($result)) 
                        {
                        if($data["device_network_status"]==0)
                            $offline=$data["status"];
                       else
                           $online=$data["status"];                                            
                        }                                                        

                    $html='<div class="col-md-3">';
                    $html.='<div class="card ">';
                    $html.='<div class="card-body box-profile">';
                    $html.='<h3 class="profile-username text-center">'.$facility_name.'</h3>';
                    $html.='<p class="text-muted text-center">'.$facility_number.'</p>';
                    $html.='<ul class="list-group list-group-unbordered mb-3">';
                    $html.='<li class="list-group-item"><b>Total carparks</b> <a class="float-right">'.$carparks.'</a></li>';
                    $html.='<li class="list-group-item"><b>Total devices</b> <a class="float-right">'.$devices.'</a></li>';

                    $html.='<li class="list-group-item"><b>Device status</b>';
                    $html.='<a class="float-right">';
                    $html.='<span class="nav-link p-1">';
                    $html.='<span class="fa-stack fa-1x" title="Online">';
                    $html.='<i class="fa fa-circle fa-stack-2x text-success"></i>';
                    $html.='<strong class="fa-stack-1x text-white">'.$online.'</strong>';
                    $html.='</span>';
                    $html.='<span class="fa-stack fa-1x" title="Offline">';
                    $html.='<i class="fa fa-circle fa-stack-2x text-danger"></i>';
                    $html.='<strong class="fa-stack-1x text-white">'.$offline.'</strong>';
                    $html.='</span></span></a></li>';

                    $html.='<li class="list-group-item">';                                    
                    $html.='<b>Alarms</b>'; 
                    $html.='<a class="float-right">';
                    $html.='<span class="nav-link p-1">';
                    $html.='<span class="fa-stack fa-1x" title="warnings">';
                    $html.='<i class="fa fa-circle fa-stack-2x text-warning"></i>';
                    $html.='<strong class="fa-stack-1x text-dark">'.$this->get_alarm_count($facility_number,0,0,6,20).'</strong>';
                    $html.='</span>';
                    $html.='<span class="fa-stack fa-1x" title="critical alarms">';
                    $html.='<i class="fa fa-circle fa-stack-2x text-danger"></i>';
                    $html.='<strong class="fa-stack-1x text-white">'.$this->get_alarm_count($facility_number,0,0,0,5).'</strong>';
                    $html.='</span></span></a></li></ul>';

                    $html.='<button type="button" class="show-facility-details btn btn-block bg-secondary-gradient" facility_number="'.$facility_number.'">More <i class="fa fa-arrow-circle-right"></i></button>';
                    $html.='</div></div></div>';
                    echo $html;
                    }
                }
            mysqli_close($con);  
            }         
        
        }
        
    function get_device_status_by_carpark($facility_number)
        {
        $carpark_number=0;
        $html="";
        $con = $this->db_connect();
        $query_string = "select carpark_number,carpark_name from watchdog_device_status where facility_number=".$facility_number." group by carpark_number";
        $carpark_result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $rows=mysqli_num_rows($carpark_result);        
        if ($rows == 1) 
            { 
            $data = mysqli_fetch_assoc($carpark_result);
            $carpark_number=$data["carpark_number"];
            mysqli_close($con);
            $html="<input type='hidden' id='facility_number' value='".$facility_number."'>";
            $html.="<input type='hidden' id='carpark_number' value='".$carpark_number."'>";
            echo $html;
            }
        else
            {            
            while ($carpark = mysqli_fetch_assoc($carpark_result)) 
                {
                $carpark_number=$carpark["carpark_number"];
                $carpark_name=$carpark["carpark_name"];    
                //if($rows>1)
                    {
                    $query_string ="select count(*) as devices from watchdog_device_status where facility_number=".$facility_number." and carpark_number=".$carpark_number;

                    $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
                    if ($data = mysqli_fetch_assoc($result)) 
                        {
                        $devices=$data["devices"];                                       
                        }
                    $query_string ="select device_network_status,count(*) as status from watchdog_device_status where facility_number=".$facility_number." and carpark_number=".$carpark_number." GROUP by device_network_status";
                    $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
                    $online=0;
                    $offline=0;
                    while ($data = mysqli_fetch_assoc($result)) 
                        {
                        if($data["device_network_status"]==0)
                            $offline=$data["status"];
                       else
                           $online=$data["status"];                                            
                        }                                                        

                    $html='<div class="col-md-3">';
                    $html.='<div class="card ">';
                    $html.='<div class="card-body box-profile">';
                    $html.='<h3 class="profile-username text-center">'.$carpark_name.'</h3>';
                    $html.='<p class="text-muted text-center">'.$carpark_number.'</p>';
                    $html.='<ul class="list-group list-group-unbordered mb-3">';               
                    $html.='<li class="list-group-item"><b>Total devices</b> <a class="float-right">'.$devices.'</a></li>';

                    $html.='<li class="list-group-item"><b>Device status</b>';
                    $html.='<a class="float-right">';
                    $html.='<span class="nav-link p-1">';
                    $html.='<span class="fa-stack fa-1x" title="Online">';
                    $html.='<i class="fa fa-circle fa-stack-2x text-success"></i>';
                    $html.='<strong class="fa-stack-1x text-white">'.$online.'</strong>';
                    $html.='</span>';
                    $html.='<span class="fa-stack fa-1x" title="Offline">';
                    $html.='<i class="fa fa-circle fa-stack-2x text-danger"></i>';
                    $html.='<strong class="fa-stack-1x text-white">'.$offline.'</strong>';
                    $html.='</span></span></a></li>';                  

                    $online='<div class="dot-indicator bg-success-gradient"  title="Online"></div>';
                    $offline='<div class="dot-indicator bg-danger-gradient"  title="Offline"></div>';

                    $query_string ="SELECT group_concat(device_number)device_number,group_concat(device_network_status)device_network_status,device_type FROM watchdog_device_status where facility_number=".$facility_number." and carpark_number=".$carpark_number." group by device_type";                    
                    $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
                    
                    while($data = mysqli_fetch_assoc($result)) 
                        {                                  
                        $category =$this->get_device_category_name($data["device_type"]);
                        $n_status = explode(',', $data["device_network_status"]);                       
                        if (in_array("0", $n_status))                        
                            $status=$offline;
                        else
                            $status=$online; 
                            
                                           
                        $html.='<li class="list-group-item"><b>'.$status.''.$category.'</b><a class="float-right">';
                        $html.='<span class="nav-link p-1">';
                        $html.='<span class="fa-stack fa-1x" title="warnings">';
                        $html.='<i class="fa fa-circle fa-stack-2x text-warning"></i>';
                        $html.='<strong class="fa-stack-1x text-dark">'.$this->get_alarm_count($facility_number,$carpark_number,$data["device_number"],6,20).'</strong>';
                        $html.='</span>';
                        $html.='<span class="fa-stack fa-1x" title="critical alarms">';
                        $html.='<i class="fa fa-circle fa-stack-2x text-danger"></i>';
                        $html.='<strong class="fa-stack-1x text-white">'.$this->get_alarm_count($facility_number,$carpark_number,$data["device_number"],0,5).'</strong>';
                        $html.='</span></span>';
                        $html.='</a></li>';
                        }

                    $html.='</ul>';

                    $html.='<button type="button" class="show-carpark-details btn btn-block bg-secondary-gradient" facility_number="'.$facility_number.'" carpark_number="'.$carpark_number.'">More <i class="fa fa-arrow-circle-right"></i></button>';
                    $html.='</div></div></div>';
                    echo $html;
                    }
                }
            echo "<input type='hidden' value='".$this->get_alarm_count($facility_number,0,0, 0,20)."' id='alarm_count'>";
            mysqli_close($con);          
            }       
        }
        
    function get_device_category_name($device_type)
        {
        $category="";
        switch ($device_type) {
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
        return $category;    
        }
        
    function device_details($data)
    {          
        $device_number=$data["device_number"];
        $carpark_number=$data["carpark_number"];
        $facility_number=$data["facility_number"];
        $category=$data["device_category"];
        $con=$this->db_connect();
        if($con!=NULL)
            {            
            $sql="select * from watchdog_device_status where device_number=".$device_number." and facility_number=".$facility_number." and carpark_number=".$carpark_number;
            $result = $con->query($sql);


            $html_data='';

            if($data = mysqli_fetch_array($result)) 
                {                                   
                $html_data.='<h4 class="mb-3">'.$data["device_name"].'</h4>';
                $html_data.='<p><strong>Category: </strong>'.$category.'</p>';
                $html_data.='<p><strong>Device number: </strong>'.$data["device_number"].'</p>'; 
                $html_data.='<p><strong>IP Address: </strong>'.$data["device_ip"].'</p>'; 
                if($data["device_type"]==1 || $data["device_type"]==2){
                    if($data["presence_loop_status"]==1)
                        $html_data.='<p><strong>Presence loop status: </strong><span class="dot-indicator bg-success-gradient" data-toggle="tooltip" data-placement="top" title="Active"></span></p>';
                    else
                    $html_data.='<p><strong>Presence loop status: </strong><span class="dot-indicator bg-danger-gradient" data-toggle="tooltip" data-placement="top" title="Deactive"></span></p>';

                    $html_data.='<p><strong>Last presence loop active: </strong>'.$data["last_presence_loop_active"].'</p>';
                    $html_data.='<p><strong>Last presence loop deactive: </strong>'.$data["last_presence_loop_deactive"].'</p>';
                    if($data["safety_loop_status"]==1)
                        $html_data.='<p><strong>Safety loop status: </strong><span class="dot-indicator bg-success-gradient" data-toggle="tooltip" data-placement="top" title="Active"></span></p>';
                    else
                        $html_data.='<p><strong>Safety loop status: </strong><span class="dot-indicator bg-danger-gradient" data-toggle="tooltip" data-placement="top" title="Deactive"></span></p>';

                    $html_data.='<p><strong>Last safety loop active: </strong>'.$data["last_safety_loop_active"].'</p>';
                    $html_data.='<p><strong>Last safety loop deactive: </strong>'.$data["last_safety_loop_deactive"].'</p>';
                }
                if($data["device_type"]==3 || $data["device_type"]==5)
                {
                $sql="select * from parcx_reporting.revenue_shifts where device_number=".$device_number." and facility_number=".$facility_number." and carpark_number=".$carpark_number." order by last_updated_date_time desc limit 1";
                $result = $con->query($sql);   
                if($data = mysqli_fetch_array($result))
                    {
                    $html_data.='<p><strong>Last hift id: </strong>'.$data["shift_id"].'</p>'; 
                    $html_data.='<p><strong>Operator name: </strong>'.$data["operator_name"].'</p>'; 
                    if($data["shift_status"]==1)
                        $html_data.='<p><strong>Shift status: </strong><span class="dot-indicator bg-danger-gradient" data-toggle="tooltip" data-placement="top" title="Closed"></span></p>';
                    else
                        $html_data.='<p><strong>Shift status: </strong><span class="dot-indicator bg-success-gradient" data-toggle="tooltip" data-placement="top" title="Open"></span></p>';
                    $html_data.='<p><strong>Shift earnings: </strong>'.$data["shift_earnings"].'</p>'; 
                    }
                }
            }
            mysqli_close($con);
            echo $html_data;
        }
    else
        echo "No server connection";
    }
        
        
    function get_device_status_by_device($facility_number,$carpark_number)
        {
        
        $con = $this->db_connect();
        $query_string = "select * from parcx_server.system_device_category";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $device= array();
        while ($data = mysqli_fetch_assoc($result)) 
            {
            $id=$data["id"];
            $device[$id]["name"]=$data["category_name"];
            $device[$id]["image"]=$data["image_url"];
            $device[$id]["group"]=$data["group"];              
            }            
        $query_string = "select * from watchdog_device_status where facility_number=".$facility_number." and carpark_number=".$carpark_number." order by device_type,device_number";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));

        while ($data = mysqli_fetch_assoc($result)) {
            $html_data="";
            $i=$data["device_type"];                    
            $data_status =$device[$i]["group"] ;
            $category =$device[$i]["name"];
            $image_url=$device[$i]["image"];
            
            

            $html_data .= '<div class="col-lg-3 col-md-6 block-data" data-number="'.$data["device_number"].'" data-type="'.$i.'" data-img="' . $image_url . '" data-status="' . $data_status . '" data-toggle="modal" data-target="#error-log-modal">';            
            $html_data .= '<div class="card">';

            $html_data .= '<div class="card-header">';
            $html_data .= '<div class="nav-item d-flex justify-content-between align-items-center">';

            $html_data .= '<h3 class="card-title">' . $data["device_name"] . '</h3>';
            $html_data .= '<span>';

            if ($data["device_network_status"] == 1) {
                $html_data .= '<span class="float-right ml-3 header-icon">';
                $html_data .= '<i class="fas fa-server text-success" data-toggle="tooltip" data-placement="top" data-original-title="Server Connectivity"></i>';
                $html_data .= '</span>';
                $html_data .= '</span>';
            } else {
                $html_data .= '<span class="float-right ml-3 header-icon">';
                $html_data .= '<i class="fas fa-server text-danger" data-toggle="tooltip" data-placement="top" data-original-title="Server Connectivity"></i>';
                $html_data .= '</span>';
                $html_data .= '</span>';
            }

            
        
            $html_data .= '</div>';
            $html_data .= '</div>';

            $html_data .= '<div class="card-body p-0">';
            $html_data .= '<div class="row no-gutters">';

            $html_data .= '<div class="col-4 block-view-img my-auto text-center"><img class="img-fluid" src="/parcx/dist/img/icon/device_icons/' . $image_url . '.png"></div>';

            $html_data .= '<div class="col-8">';
            $html_data .= '<ul class="nav flex-column">';
            $html_data .= '<li class="nav-item">';
            $html_data .= '<span class="nav-link">Category';
            $html_data .= '<span class="float-right device_category">' . $category . '</span>';
            $html_data .= '</span>';
            $html_data .= '</li>';
            $html_data .= '<li class="nav-item">';
            $html_data .= '<span class="nav-link">Device Number';
            $html_data .= '<span class="float-right device_number">' . $data["device_number"] . '</span>';
            $html_data .= '</span>';
            $html_data .= '</li>';
            $html_data .= '<li class="nav-item">';
            $html_data .= '<span class="nav-link">IP Address';
            $html_data .= '<span class="float-right device_ip">' . $data["device_ip"] . '</span>';
            $html_data .= '</span>';
            $html_data .= '</li>';

            $html_data .= '<li class="nav-item d-flex justify-content-between align-items-center">';
            $html_data .= '<span class="nav-link">';
            $html_data .= 'Notifications';
            $html_data .= '</span>';
            $html_data .= '<span class="nav-link p-1">';
            $html_data .= '<span class="fa-stack fa-1x" title="warnings">';
            $html_data .= '<i class="fa fa-circle fa-stack-2x text-warning"></i>';
            $html_data .= '<strong class="fa-stack-1x text-dark">' . $this->get_alarm_count($facility_number,$carpark_number,$data["device_number"], 6, 20) . '</strong>';
            $html_data .= '</span>';

            $html_data .= '<span class="fa-stack fa-1x" title="critical alarms">';
            $html_data .= '<i class="fa fa-circle fa-stack-2x text-danger" title="critical alarms"></i>';
            $html_data .= '<strong class="fa-stack-1x text-white">' . $this->get_alarm_count($facility_number,$carpark_number,$data["device_number"], 0,5) . '</strong>';
            $html_data .= '</span>';
            $html_data .= '</span>';
            $html_data .= '</li>';

            $html_data .= '</ul>';
            $html_data .= '</div>';

            $html_data .= '</div>'; // close row
            $html_data .= '</div>'; // close card-body

            $html_data .= '</div>';
            $html_data .= '</div>';
            echo $html_data;
        }
        echo "<input type='hidden' value='".$this->get_alarm_count($facility_number,$carpark_number,0, 0,20)."' id='alarm_count'>";
        mysqli_close($con);           
        //echo "<input type='hidden' id='current_level' value='device' facility_number='".$facility_number."' carpark_number='".$carpark_number."'>";
        }
               
    function get_device_status() {
        $con = $this->db_connect();
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

        while ($data = mysqli_fetch_assoc($result)) {
            $carpark[] = $data['carpark_number'];
            $carpark_name[] = $data['carpark_name'];
        }

        $html_data .= $header;

        while ($i < count($carpark)) {
            $query_string = "select * from watchdog_device_status where carpark_number=" . $carpark[$i][0] . "  order by device_type,device_number";
            $result = mysqli_query($con, $query_string) or die(mysqli_error($con));

            $html_data .= '<tbody class="table-view">';
            $html_data .= "<tr class='subhead card-text d-flex justify-content-between align-items-center pl-2' data-status='header'><td>" . $carpark_name[$i] . "</td></tr>";

            while ($data = mysqli_fetch_assoc($result)) {
                $data_status = "all";

                if ($data["device_type"] == 1 || $data["device_type"] == 2)
                    $data_status = "columns";

                if ($data["device_type"] == 3 || $data["device_type"] == 5 || $data["device_type"] == 4)
                    $data_status = "payment_machines";

                if ($data["device_type"] == 6 || $data["device_type"] == 7)
                    $data_status = "controllers";

                if ($data["device_type"] == 8)
                    $data_status = "camera";

                if ($data["device_type"] == 9)
                    $data_status = "vms";


                $html_data .= '<tr class="card-text d-flex justify-content-between align-items-center" data-status="' . $data_status . '">';

                $html_data .= '<td class="col text-left text-dark table-row-detail">';
                $html_data .= '<h6>';

                if ($data["device_network_status"] == 1)
                    $html_data .= '<div class="dot-indicator bg-success-gradient" data-toggle="tooltip" data-placement="top" title="Online"></div>';
                else
                    $html_data .= '<div class="dot-indicator bg-danger-gradient" data-toggle="tooltip" data-placement="top" title="Currently Off line"></div>';


                $html_data .= $data["device_name"] . '</h6></td>';
                $category = "";
                switch ($data["device_type"]) {
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
                $html_data .= '<td class="col" label="category">' . $category . '</td>';
                $html_data .= '<td class="col" label=device_ip>' . $data["device_ip"] . '</td>';
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

        while ($data = mysqli_fetch_assoc($result)) {
            $data_status = "all";

            if ($data["device_type"] == 1 || $data["device_type"] == 2) {
                $data_status = "columns";
                $image_url = "/parcx/dist/img/icon/device_icons/column.png";
                $data_img = "column";
            } else if ($data["device_type"] == 3) {
                $data_status = "payment_machines";
                $image_url = "/parcx/dist/img/icon/device_icons/cashier-pos.png";
                $data_img = "cashier-pos";
            } else if ($data["device_type"] == 5) {
                $data_status = "payment_machines";
                $image_url = "/parcx/dist/img/icon/device_icons/handheld-pos.png";
                $data_img = "handheld-pos";
            } else if ($data["device_type"] == 4) {
                $data_status = "payment_machines";
                $image_url = "/parcx/dist/img/icon/device_icons/payment-machine.png";
                $data_img = "payment-machine";
            } else if ($data["device_type"] == 6 || $data["device_type"] == 7) {
                $data_status = "controllers";
                $image_url = "/parcx/dist/img/icon/device_icons/controller.png";
                $data_img = "controller";
            } else if ($data["device_type"] == 8) {
                $data_status = "camera";
                $image_url = "/parcx/dist/img/icon/device_icons/cctv.png";
                $data_img = "cctv";
            } else if ($data["device_type"] == 9) {
                $data_status = "vms";
                $image_url = "/parcx/dist/img/icon/device_icons/vms.png";
                $data_img = "vms";
            }

            $html_data .= '<div class="col-lg-3 col-md-6 block-data" data-img="' . $data_img . '" data-status="' . $data_status . '" data-toggle="modal" data-target="#error-log-modal">';
            // $html_data .= '<div class="card card-danger card-outline">';

            $html_data .= '<div class="card">';

            $html_data .= '<div class="card-header">';
            $html_data .= '<div class="nav-item d-flex justify-content-between align-items-center">';

            $html_data .= '<h3 class="card-title">' . $data["device_name"] . '</h3>';
            $html_data .= '<span>';

            if ($data["device_network_status"] == 1) {
                $html_data .= '<span class="float-right ml-3 header-icon">';
                $html_data .= '<i class="fas fa-server text-success" data-toggle="tooltip" data-placement="top" data-original-title="Server Connectivity"></i>';
                $html_data .= '</span>';
                $html_data .= '</span>';
            } else {
                $html_data .= '<span class="float-right ml-3 header-icon">';
                $html_data .= '<i class="fas fa-server text-danger" data-toggle="tooltip" data-placement="top" data-original-title="Server Connectivity"></i>';
                $html_data .= '</span>';
                $html_data .= '</span>';
            }

            $category = "";
            switch ($data["device_type"]) {
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

            $html_data .= '<div class="col-4 block-view-img my-auto text-center"><img class="img-fluid" src="' . $image_url . '"></div>';

            $html_data .= '<div class="col-8">';
            $html_data .= '<ul class="nav flex-column">';
            $html_data .= '<li class="nav-item">';
            $html_data .= '<span class="nav-link">Category';
            $html_data .= '<span class="float-right device_category">' . $category . '</span>';
            $html_data .= '</span>';
            $html_data .= '</li>';
            $html_data .= '<li class="nav-item">';
            $html_data .= '<span class="nav-link">Device Number';
            $html_data .= '<span class="float-right device_number">' . $data["device_number"] . '</span>';
            $html_data .= '</span>';
            $html_data .= '</li>';
            $html_data .= '<li class="nav-item">';
            $html_data .= '<span class="nav-link">IP Address';
            $html_data .= '<span class="float-right device_ip">' . $data["device_ip"] . '</span>';
            $html_data .= '</span>';
            $html_data .= '</li>';

            $html_data .= '<li class="nav-item d-flex justify-content-between align-items-center">';
            $html_data .= '<span class="nav-link">';
            $html_data .= 'Notifications';
            $html_data .= '</span>';
            $html_data .= '<span class="nav-link p-1">';
            $html_data .= '<span class="fa-stack fa-1x">';
            $html_data .= '<i class="fa fa-circle fa-stack-2x text-warning"></i>';
            $html_data .= '<strong class="fa-stack-1x text-dark">' . $this->getDeviceAlarmscount(0,0,$data["device_number"], 11, 20) . '</strong>';
            $html_data .= '</span>';

            $html_data .= '<span class="fa-stack fa-1x">';
            $html_data .= '<i class="fa fa-circle fa-stack-2x text-danger"></i>';
            $html_data .= '<strong class="fa-stack-1x text-white">' . $this->getDeviceAlarmscount(0,0,$data["device_number"], 1, 10) . '</strong>';
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
    }

// End Function . Live Revenue



    /* DASHBOARD OCCUPANCY FUNCTIONS */
    
   
    
     function get_occupancy_facility() {
        $con = $this->db_connect();
        $query_string = "select facility_number,carpark_name,total_spaces,current_level,last_updated_datetime from counters where counter_type=0 ORDER BY dashboard_order ASC";
       
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));        
                
        if (mysqli_num_rows($result) == 1) 
            {
            $data = mysqli_fetch_assoc($result);
            $facility_number=$data["facility_number"];           
            mysqli_close($con);            
            $this->get_occupancy_carpark($facility_number);
            } 
        else 
            {
            $html_data = "";
            while ($data = mysqli_fetch_assoc($result)) 
                {
               
                $gauge_value = ($data['current_level'] / $data['total_spaces']) * 100;
                $html_data .= ' <div class="col-lg-3 col-sm-6 col-xs-12">';
                $html_data .= ' <div class="chart-box text-center">';                
                $html_data .= '<p class="text-center chart-header" id="gauge1_name">' . $data['carpark_name'] . '</p>';                
                $html_data .= ' <input  type="hidden" class="knob" data-thickness="0.3" data-angleArc="250" data-angleOffset="-125"
                                value="' . $gauge_value . '" data-width="250" data-height="150" data-fgColor="' . $this->choose_bg_color($gauge_value, 1) . '" data-readonly="true">';
                $html_data .= '<p class="gauge-val" ><span>' . $data['current_level'] . '</span>/<span>' . $data['total_spaces'] . '</span></p>';
                $html_data .= '<div class="card-body">';
                $html_data .= '</div>';
                $html_data .= '<button type="button" class="btn btn-block btn-outline-secondary btn-sm show-facility-details" facility_number=' . $data['facility_number'] . '>More <i class="fa fa-arrow-circle-right"></i></button>';
                $html_data .= '</div>';
                $html_data .= '</div>';                
            }//wend  
            echo $html_data;
            mysqli_close($con);
        }//end if                         
    }
    

    function get_occupancy_carpark($facility_number) {
       
        $con = $this->db_connect();
        $query_string = "select last_updated_datetime,carpark_name,carpark_number,total_spaces,current_level,shortterm_current_level,total_shortterm_spaces,access_current_level,total_access_spaces,reservation_current_level,total_reservation_spaces";
        $query_string .= " from counters where counter_type=1 and facility_number=" . $facility_number . " ORDER BY carpark_number ASC";                       
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        
        if (mysqli_num_rows($result) == 1) 
            {
            //If only one carpark show the car park list directly 
            $data = mysqli_fetch_assoc($result);                        
            $carpark_number=$data["carpark_number"];
            mysqli_close($con);
            $html="<input type='hidden' id='facility_number' value='".$facility_number."'>";
            $html.="<input type='hidden' id='carpark_number' value='".$carpark_number."'>";
            echo $html;            
            } 
        else 
            {
            $html_data = "";        
            $occupancy_percentage = 0;             
            while ($data = mysqli_fetch_assoc($result)) 
                {               
                $gauge_value = ($data['current_level'] / $data['total_spaces']) * 100;
                $color = $this->choose_bg_color($gauge_value, 1);

                $html_data .= ' <div class="col-lg-3 col-sm-6 col-xs-12">';
                $html_data .= ' <div class="chart-box text-center">';                
                $html_data .= '<h1 class="text-left chart-header" id="gauge1_name">' . $data['carpark_name'] . '</h1>';                
                $html_data .= ' <input  type="hidden" class="knob" data-thickness="0.3" data-angleArc="250" data-angleOffset="-125"
                                value="' . $gauge_value . '" data-width="250" data-height="150" data-fgColor="' . $color . '" data-readonly="true">';
                $html_data .= '<p class="gauge-val" ><span>' . $data['current_level'] . '</span>/<span>' . $data['total_spaces'] . '</span></p>';
                $html_data .= '<div class="card-body">';

                // $html_data .= '<div id="row">';
                $html_data .= '<div id="row">';
                $html_data .= '<div class="col-5 text-left" style="float:left;" id="category">ShortTerm</div>';
                $html_data .= ' <div class="col text-right" >' . $data['shortterm_current_level'] . '/' . $data['total_shortterm_spaces'] . '</div>';
                $occupancy_percentage = ($data['shortterm_current_level'] / $data['total_shortterm_spaces']) * 100;
                $html_data .= '</div>'; //end row
                $html_data .= '<div class="progress mb-3">';
                $html_data .= '<div class="progress-bar bg-' . $this->choose_bg_color($occupancy_percentage, 2) . '" id="shortterm-progress-' . $i . '" role="progressbar" aria-valuenow="20" aria-valuemin="0" aria-valuemax="100" style="width:' . $occupancy_percentage . '%">';
                $html_data .= '</div>'; //end progress-bar
                $html_data .= '</div>'; // end progress mb-3
                //$html_data .= '</div>';//end row
                //$html_data .= '<div id="row">';
                $html_data .= '<div id="row">';
                $html_data .= '<div class="col-5 text-left" style="float:left;" id="category">Access</div>';
                $html_data .= '<div class="col text-right" >' . $data['access_current_level'] . '/' . $data['total_access_spaces'] . '</div>';
                $occupancy_percentage = ($data['access_current_level'] / $data['total_access_spaces']) * 100;
                $html_data .= '</div>'; //end row count
                $html_data .= ' <div class="progress mb-3">';
                $html_data .= '<div class="progress-bar bg-' . $this->choose_bg_color($occupancy_percentage, 2) . '" id="access-progress-' . $i . '" role="progressbar" aria-valuenow="20" aria-valuemin="0" aria-valuemax="100" style="width:' . $occupancy_percentage . '%">';
                $html_data .= '</div>'; //end row
                $html_data .= '</div>';
                //$html_data .= '</div>';//end row
                // $html_data .= '<div id="row">';
                $html_data .= '<div id="row">';
                $html_data .= '<div class="col-5 text-left" style="float:left;" id="category">Reservation </div>';
                echo $data['reservation_current_level'];
                $html_data .= '<div class="col text-right" >' . $data['reservation_current_level'] . '/' . $data['total_reservation_spaces'] . '</div>';
                $html_data .= '</div>'; //end row count
                $html_data .= ' <div class="progress mb-3">';
                 
                $occupancy_percentage = ($data['reservation_current_level'] / $data['total_reservation_spaces']) * 100;
               
                $html_data .= '<div class="progress-bar bg-' . $this->choose_bg_color($occupancy_percentage, 2) . '" id="reservation-progress-' . $i . '" role="progressbar" aria-valuenow="20" aria-valuemin="0" aria-valuemax="100" style="width:' . $occupancy_percentage . '%">';
                $html_data .= '</div>'; //end row
                $html_data .= '</div>';
                // $html_data .= '</div>';//end row

                $html_data .= '</div>';
                $html_data .= '<button type="button" class="btn btn-block bg-secondary-gradient show-carpark-details" facility_number="' . $facility_number . '" carpark_number="' . $data['carpark_number'] . '">More <i class="fa fa-arrow-circle-right"></i></button>';
                $html_data .= '</div>';

                $html_data .= '</div>';               
            }//wend
            mysqli_close($con);
        echo $html_data;
        }//end if         
        
    }
    
    
    function get_occupancy_device($facility_number, $carpark_number) {
        $con = $this->db_connect();
        $query_string = "select * from counters where counter_type=1 and facility_number=" . $facility_number . " AND carpark_number=" . $carpark_number;        
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $html_data = "";
        $i = 1;
        $shortterm_occupancy=0;
        $access_occupancy=0;
        $reservation_occupancy=0;
        $gauge_percentage=0;
        $data = mysqli_fetch_assoc($result);
        
        if($data['total_spaces']>0)
            $gauge_percentage = ($data['current_level'] / $data['total_spaces']) * 100;
        if($data['total_shortterm_spaces']>0)
            $shortterm_occupancy = ($data['shortterm_current_level'] / $data['total_shortterm_spaces']) * 100;
        if($data['total_access_spaces']>0)
            $access_occupancy = ($data['access_current_level'] / $data['total_access_spaces']) * 100;
        if($data['total_reservation_spaces']>0)
            $reservation_occupancy = ($data['reservation_current_level'] / $data['total_reservation_spaces']) * 100;  
                

        $html_data .= '<div class="container-fluid">';
        $html_data .= '<div class="row mb-2">';
        $html_data .= '<div class="col-sm-6">';
        $html_data .= '<h1 id="gauge1_name">' . $data['carpark_name'] . '</h1>';
        //$html_data .= '<h5>' . $_SESSION['facility_name'] . '</h5>';
        $html_data .= '</div>';
        $html_data .= '</div>';
        $html_data .= '</div>';

        //The Occupancy Gauge - Total 
        $html_data .= '<div class="col-md-4">';
        $html_data .= ' <div class="chart-box text-center">';
        $html_data .= '<p class="text-center chart-header" id="gauge1_name"></p>';
        $html_data .= ' <input id="carpark" type="hidden" class="knob" data-thickness="0.3" data-angleArc="250" data-angleOffset="-125"
        value="' . $gauge_percentage . '" data-width="250" data-height="150" data-fgColor="' . $this->choose_bg_color($gauge_percentage, 1) . '" data-readonly="true">';
        $html_data .= '<p class="gauge-val" id="gauge-value"><span>' . $data['current_level'] . '</span>/<span>' . $data['total_spaces'] . '</span></p>';
        $html_data .= '  </div>';
        $html_data .= '  </div>'; //col md 4
        //Occupancy progress bar by type 

        $html_data .= '<div class="col-md-4">';
        $html_data .= '<div class="card">';
        $html_data .= '<div class="card-body p-0">';
        $html_data .= '<table class="table table-striped">';
        $html_data .= '<tr  style="height: 25%">';
        $html_data .= '<td style="width: 10px"></td>';        
        $html_data .= '<td style="width: 50%">Occupancy</td>';
        $html_data .= '<td style="width: 20%">Current/Total</td>';
        $html_data .= '</tr>';

        $html_data .= '<tr>';
        $html_data .= '<td>ShortTerm</td>';
        $html_data .= '<td>';
        $html_data .= '<div class="progress progress-xs">';
        $html_data .= ' <div id="progress-shortterm" class="progress-bar bg-' . $this->choose_bg_color($shortterm_occupancy, 2) . '" style="width: ' . $shortterm_occupancy . '%"></div>';
        $html_data .= '</div>';
        $html_data .= '</td>';
        $html_data .= '<td><span id="badge-shortterm" class="badge bg-' . $this->choose_bg_color($shortterm_occupancy, 2) . '">' . $data['shortterm_current_level'] . '/' . $data['total_shortterm_spaces'] . '</span></td>';
        $html_data .= '</tr>';

        $html_data .= '<tr>';
        $html_data .= '<td>Access</td>';
        $html_data .= '<td>';
        $html_data .= '<div class="progress progress-xs">';

        $html_data .= ' <div id="progress-access" class="progress-bar bg-' . $this->choose_bg_color($access_occupancy, 2) . '" style="width: ' . $access_occupancy . '%"></div>';
        $html_data .= '</div>';
        $html_data .= '</td>';
        $html_data .= '   <td><span id="badge-access" class="badge bg-' . $this->choose_bg_color($access_occupancy, 2) . '">' . $data['access_current_level'] . '/' . $data['total_access_spaces'] . '</span></td>';
        $html_data .= '  </tr>';

        $html_data .= '  <tr>';
        $html_data .= '  <td>Reservation</td>';
        $html_data .= '  <td>';
        $html_data .= '  <div class="progress progress-xs">';

        $html_data .= ' <div id="progress-reservation" class="progress-bar bg-' . $this->choose_bg_color($reservation_occupancy, 2) . '" style="width: ' . $reservation_occupancy . '%"></div>';
        $html_data .= '</div>';
        $html_data .= '</td>';
        $html_data .= '   <td><span id="badge-reservation" class="badge bg-' . $this->choose_bg_color($reservation_occupancy, 2) . '">' . $data['reservation_current_level'] . '/' . $data['total_reservation_spaces'] . '</span></td>';
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
        $html_data .= '   <th></th>';
        $html_data .= '   <th>Entries</th>';
        $html_data .= '   <th>Exits</th>';
        $html_data .= '  </tr>';
        $html_data .= '  <tr>';
        $html_data .= '   <td>ShortTerm</td>';
        $html_data .= '   <td id="shortterm-entry">' . $data['shortterm_entry'] . '</td>';
        $html_data .= '   <td id="shortterm-exit">' . $data['shortterm_exit'] . '</td>';
        $html_data .= '  </tr>';
        $html_data .= '  <tr>';
        $html_data .= '   <td>Access</td>';
        $html_data .= '   <td id="access-entry">' . $data['access_entry'] . '</td>';
        $html_data .= '   <td id="access-exit">' . $data['access_exit'] . '</td>';
        $html_data .= '  </tr>';

        $html_data .= '  <tr>';
        $html_data .= '   <td>Reservation</td>';
        $html_data .= '   <td id="reservation-entry">' . $data['reservation_entry'] . '</td>';
        $html_data .= '   <td id="reservation-exit">' . $data['reservation_exit'] . '</td>';
        $html_data .= '  </tr>';

        $html_data .= '  <tr>';
        $html_data .= '   <td>Manual</td>';
        $html_data .= '   <td id="manual-entry">' . $data['total_manual_entry'] . '</td>';
        $html_data .= '   <td id="manual-exit">' . $data['total_manual_exit'] . '</td>';
        $html_data .= '  </tr>';


        $html_data .= '  </table>';
        $html_data .= ' </div>';

        $html_data .= '  </div>';
        $html_data .= '  </div>'; //col md 4     
        mysqli_close($con);
        echo $html_data;
    }

    function OccupancyFacilityCounters() {
        $con = $this->db_connect();
        $query_string = "select facility_number,carpark_name,total_spaces,current_level,last_updated_datetime from counters";
        $query_string .= " where counter_type=0 ORDER BY dashboard_order ASC";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $html_data = "";
        $i = 1;

        if (mysqli_num_rows($result) == 1) {
            //If only one facility show the car park list directly
            $data = mysqli_fetch_assoc($result);
            $_SESSION['dashboard_level'] = "carparks";
            $_SESSION['facility_number'] = $data['facility_number'];
            $_SESSION['facility_name'] = $data['carpark_name'];
            $this->OccupancyCarparkCounters($data['facility_number']);
        } else {

            while ($data = mysqli_fetch_assoc($result)) {
                if ($i == 1) {
                    $html_data .= '<input type="hidden" id="last_updated_datetime" value="' . $data['last_updated_datetime'] . '">';
                }
                $gauge_value = ($data['current_level'] / $data['total_spaces']) * 100;
                $html_data .= ' <div class="col-lg-3 col-sm-6 col-xs-12">';
                $html_data .= ' <div class="chart-box text-center">';
                $html_data .= ' <span id="dashboard_level" level="facility"></span>';
                $html_data .= '<p class="text-center chart-header" id="gauge1_name">' . $data['carpark_name'] . '</p>';
                $html_data .= ' <input id="carpark' . $i . '" type="hidden" class="knob" data-thickness="0.3" data-angleArc="250" data-angleOffset="-125"
                                value="' . $gauge_value . '" data-width="250" data-height="150" data-fgColor="' . $this->choose_bg_color($gauge_value, 1) . '" data-readonly="true">';
                $html_data .= '<p class="gauge-val" id="gauge-value' . $i . '"><span>' . $data['current_level'] . '</span>/<span>' . $data['total_spaces'] . '</span></p>';
                $html_data .= '<div class="card-body">';
                $html_data .= '</div>';
                $html_data .= '<button type="button" class="btn btn-block btn-outline-secondary btn-sm" id="ShowCarparks" facility_number=' . $data['facility_number'] . '>More <i class="fa fa-arrow-circle-right"></i></button>';
                $html_data .= '</div>';
                $html_data .= '</div>';
                $i++;
            }//wend  
        }//end if         
        mysqli_close($con);
        echo $html_data;
    }

// End . Facility Occupancy 

    function UpdateOccupancyCarparkCounters($facility_number) {
        $con = $this->db_connect();
        $query_string = "select last_updated_datetime,carpark_name,total_spaces,current_level,shortterm_current_level,total_shortterm_spaces,access_current_level,total_access_spaces,reservation_current_level,total_reservation_spaces";
        $query_string .= " from counters where counter_type=1 and facility_number=" . $facility_number . " ORDER BY dashboard_order ASC";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $counts = array();

        while ($data = mysqli_fetch_assoc($result)) {
            $gauge_value = ($data['current_level'] / $data['total_spaces']) * 100;
            $color = $this->choose_bg_color($gauge_value, 1);

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
    }

// End . Function GetOccupancyCarparkCountersDashboard

    function UpdateOccupancyFacilityCounters() {
        $con = $this->db_connect();
        $query_string = "select total_spaces,current_level,last_updated_datetime from counters where counter_type=0";
        $query_string .= " ORDER by dashboard_order ASC";

        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $counts = array();

        while ($data = mysqli_fetch_assoc($result)) {
            $gauge_value = ($data['current_level'] / $data['total_spaces']) * 100;
            $color = $this->choose_bg_color($gauge_value, 1);
            $counts[] = array
                (
                'current_level' => "$data[current_level]",
                'color' => "$color",
                'total_spaces' => "$data[total_spaces]",
                'gauge_value' => "$gauge_value",
                'last_updated_datetime' => "$data[last_updated_datetime]"
            );
        }
        mysqli_close($con);
        echo json_encode($counts);
    }

// End . Function GetOccupancyCarparkCountersDashboard

    function OccupancyCarparkCounters($facility_number) {
        $_SESSION['dashboard_level'] = "carparks";
        $_SESSION['facility_number'] = $facility_number;
        $con = $this->db_connect();
        $query_string = "select last_updated_datetime,carpark_name,carpark_number,total_spaces,current_level,shortterm_current_level,total_shortterm_spaces,access_current_level,total_access_spaces,reservation_current_level,total_reservation_spaces";
        $query_string .= " from counters where counter_type=1 and facility_number=" . $facility_number . " ORDER BY dashboard_order ASC";

        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $html_data = "";
        $i = 1;
        $occupancy_percentage = 0;  //temp variable 


        if (mysqli_num_rows($result) == 1) {
            //If only one carpark show the car park list directly 
            $data = mysqli_fetch_assoc($result);
            $this->OccupancyCarparkDetail($facility_number, $data['carpark_number']);
        } else {
            while ($data = mysqli_fetch_assoc($result)) {
                if ($i == 1) {
                    $html_data .= '<input type="hidden" id="last_updated_datetime" value="' . $data['last_updated_datetime'] . '">';
                }
                $gauge_value = ($data['current_level'] / $data['total_spaces']) * 100;
                $color = $this->choose_bg_color($gauge_value, 1);

                $html_data .= ' <div class="col-lg-3 col-sm-6 col-xs-12">';
                $html_data .= ' <div class="chart-box text-center">';
                $html_data .= ' <span id="dashboard_level" level="carparks" facility_number="' . $facility_number . '"></span>';

                $html_data .= '<h1 class="text-left chart-header" id="gauge1_name">' . $data['carpark_name'] . '</h1>';
                $html_data .= '<h5 class="text-left facility-name">' . $_SESSION['facility_name'] . ' </h5>';
                $html_data .= ' <input id="carpark' . $i . '" type="hidden" class="knob" data-thickness="0.3" data-angleArc="250" data-angleOffset="-125"
                                value="' . $gauge_value . '" data-width="250" data-height="150" data-fgColor="' . $color . '" data-readonly="true">';
                $html_data .= '<p class="gauge-val" id="gauge-value' . $i . '"><span>' . $data['current_level'] . '</span>/<span>' . $data['total_spaces'] . '</span></p>';
                $html_data .= '<div class="card-body">';

                // $html_data .= '<div id="row">';
                $html_data .= '<div id="row">';
                $html_data .= '<div class="col-5 text-left" style="float:left;" id="category">ShortTerm</div>';
                $html_data .= ' <div class="col text-right" id="shortterm-count-' . $i . '">' . $data['shortterm_current_level'] . '/' . $data['total_shortterm_spaces'] . '</div>';
                $occupancy_percentage = ($data['shortterm_current_level'] / $data['total_shortterm_spaces']) * 100;
                $html_data .= '</div>'; //end row
                $html_data .= '<div class="progress mb-3">';
                $html_data .= '<div class="progress-bar bg-' . $this->choose_bg_color($occupancy_percentage, 2) . '" id="shortterm-progress-' . $i . '" role="progressbar" aria-valuenow="20" aria-valuemin="0" aria-valuemax="100" style="width:' . $occupancy_percentage . '%">';
                $html_data .= '</div>'; //end progress-bar
                $html_data .= '</div>'; // end progress mb-3
                //$html_data .= '</div>';//end row
                //$html_data .= '<div id="row">';
                $html_data .= '<div id="row">';
                $html_data .= '<div class="col-5 text-left" style="float:left;" id="category">Access</div>';
                $html_data .= '<div class="col text-right" id="access-count-' . $i . '">' . $data['access_current_level'] . '/' . $data['total_access_spaces'] . '</div>';
                $occupancy_percentage = ($data['access_current_level'] / $data['total_access_spaces']) * 100;
                $html_data .= '</div>'; //end row count
                $html_data .= ' <div class="progress mb-3">';
                $html_data .= '<div class="progress-bar bg-' . $this->choose_bg_color($occupancy_percentage, 2) . '" id="access-progress-' . $i . '" role="progressbar" aria-valuenow="20" aria-valuemin="0" aria-valuemax="100" style="width:' . $occupancy_percentage . '%">';
                $html_data .= '</div>'; //end row
                $html_data .= '</div>';
                //$html_data .= '</div>';//end row
                // $html_data .= '<div id="row">';
                $html_data .= '<div id="row">';
                $html_data .= '<div class="col-5 text-left" style="float:left;" id="category">Reservation</div>';
                $html_data .= '<div class="col text-right" id="reservation-count-' . $i . '">' . $data['reservation_current_level'] . '/' . $data['total_reservation_spaces'] . '</div>';
                $html_data .= '</div>'; //end row count
                $html_data .= ' <div class="progress mb-3">';
                $occupancy_percentage = ($data['reservation_current_level'] / $data['total_reservation_spaces']) * 100;
                $html_data .= '<div class="progress-bar bg-' . $this->choose_bg_color($occupancy_percentage, 2) . '" id="reservation-progress-' . $i . '" role="progressbar" aria-valuenow="20" aria-valuemin="0" aria-valuemax="100" style="width:' . $occupancy_percentage . '%">';
                $html_data .= '</div>'; //end row
                $html_data .= '</div>';
                // $html_data .= '</div>';//end row

                $html_data .= '</div>';
                $html_data .= '<button type="button" class="btn btn-block bg-secondary-gradient" id="ShowCarparkDetail" facility_number="' . $facility_number . '" carpark_number="' . $data['carpark_number'] . '">More <i class="fa fa-arrow-circle-right"></i></button>';
                $html_data .= '</div>';

                $html_data .= '</div>';
                $i++;
            }//wend
        }//end if         
        mysqli_close($con);
        echo $html_data;
    }

// End Occupancy Overview

    function choose_bg_color($current_level, $type) {
        //type 1 = gauge , type 2 = progress and badge
        if ($current_level <= 50) {
            $bg = "success";
            $color = "#28a745"; // Green 
        } else if (($current_level > 50) && ($current_level <= 75)) {
            $bg = "warning";
            $color = "#ffcd3c"; //Yellow
        } else if ($current_level > 75) {
            $bg = "danger";
            $color = "#dc3545"; //red
        }

        if ($type == 1)
            return $color;
        else if ($type == 2)
            return $bg;
    }

// end choose bhg by occupancy level 

    function cash_levels($data) 
        {
        $con = $this->db_connect();
        $query_string = "SELECT a.device_number,a.device_name,a.denomination,a.current_level as recycler_current_level,b.current_level as cashbox_current_level FROM apm_recycle_levels as a join apm_cashbox_levels as b on a.denomination=b.denomination and a.device_number=b.device_number and a.carpark_number=b.carpark_number and a.facility_number=b.facility_number and a.device_number= ".$data["device_number"]." and a.carpark_number =".$data["carpark_number"]." and a.facility_number=".$data["facility_number"]." order by denomination asc";

        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        if(mysqli_num_rows($result)>0)
            {
            $html_data = "<table id='cash_level_table' class='table table-blue table-bordered jspdf-table'>";
            $html_data .= "<thead><tr>";        
            $html_data .= "<th>Banknote</th>";
            $html_data .= "<th>Recycler level</th>";
            $html_data .= "<th>Cashbox level</th>";
            $html_data .= "<th>Total</th>";
            $html_data .= "</tr></thead>";
            $html_data .= "<tbody>";


            $apm_total = 0;            
            while ($data = mysqli_fetch_assoc($result)) 
                {                          
                $html_data .= "<tr>";            
                $html_data .= "<td>" . $data['denomination'] . "</td>";
                $html_data .= "<td>" . $data['recycler_current_level'] . "</td>";
                $html_data .= "<td>" . $data['cashbox_current_level'] . "</td>";
                $total = $data['denomination'] * $data['cashbox_current_level'] + $data['denomination'] * $data['recycler_current_level'];
                $html_data .= "<td>" . $total . "</td>";
                $html_data .= "</tr>";
                $apm_total = $apm_total + $total;
                }
            if($html_data!="")
            $html_data.="<tr><td></td><td></td><td><h3>Total</h3></td><td><h3>".$apm_total."</h3></td></tr>";
            $html_data.="</tbody></table>";
            
            echo $html_data;
            }
        mysqli_close($con);
        }

// End

    function OccupancyCarparkDetail($facility_number, $carpark_number) {
        $con = $this->db_connect();
        $query_string = "select * from counters where counter_type=1 and facility_number=" . $facility_number . " AND carpark_number=" . $carpark_number;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $html_data = "";
        $i = 1;
        $data = mysqli_fetch_assoc($result);

        $gauge_percentage = ($data['current_level'] / $data['total_spaces']) * 100;
        $shortterm_occupancy = ($data['shortterm_current_level'] / $data['total_shortterm_spaces']) * 100;
        $access_occupancy = ($data['access_current_level'] / $data['total_access_spaces']) * 100;
        $reservation_occupancy = ($data['reservation_current_level'] / $data['total_reservation_spaces']) * 100;

        $html_data .= '<input type="hidden" id="last_updated_datetime" value="' . $data['last_updated_datetime'] . '">';
        $html_data .= ' <span id="dashboard_level" level="carpark_detail"  carpark_number="' . $carpark_number . '" facility_number="' . $facility_number . '"></span>';

        $html_data .= '<div class="container-fluid">';
        $html_data .= '<div class="row mb-2">';
        $html_data .= '<div class="col-sm-6">';
        $html_data .= '<h1 id="gauge1_name">' . $data['carpark_name'] . '</h1>';
        $html_data .= '<h5>' . $_SESSION['facility_name'] . '</h5>';
        $html_data .= '</div>';
        $html_data .= '</div>';
        $html_data .= '</div>';

        //The Occupancy Gauge - Total 
        $html_data .= '<div class="col-md-4">';
        $html_data .= ' <div class="chart-box text-center">';
        $html_data .= '<p class="text-center chart-header" id="gauge1_name"></p>';
        $html_data .= ' <input id="carpark" type="hidden" class="knob" data-thickness="0.3" data-angleArc="250" data-angleOffset="-125"
        value="' . $gauge_percentage . '" data-width="250" data-height="150" data-fgColor="' . $this->choose_bg_color($gauge_percentage, 1) . '" data-readonly="true">';
        $html_data .= '<p class="gauge-val" id="gauge-value"><span>' . $data['current_level'] . '</span>/<span>' . $data['total_spaces'] . '</span></p>';
        $html_data .= '  </div>';
        $html_data .= '  </div>'; //col md 4
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
        $html_data .= ' <div id="progress-shortterm" class="progress-bar bg-' . $this->choose_bg_color($shortterm_occupancy, 2) . '" style="width: ' . $shortterm_occupancy . '%"></div>';
        $html_data .= '</div>';
        $html_data .= '</td>';
        $html_data .= '<td><span id="badge-shortterm" class="badge bg-' . $this->choose_bg_color($shortterm_occupancy, 2) . '">' . $data['shortterm_current_level'] . '/' . $data['total_shortterm_spaces'] . '</span></td>';
        $html_data .= '</tr>';

        $html_data .= '<tr>';
        $html_data .= '<td>Access</td>';
        $html_data .= '<td>';
        $html_data .= '<div class="progress progress-xs">';

        $html_data .= ' <div id="progress-access" class="progress-bar bg-' . $this->choose_bg_color($access_occupancy, 2) . '" style="width: ' . $access_occupancy . '%"></div>';
        $html_data .= '</div>';
        $html_data .= '</td>';
        $html_data .= '   <td><span id="badge-access" class="badge bg-' . $this->choose_bg_color($access_occupancy, 2) . '">' . $data['access_current_level'] . '/' . $data['total_access_spaces'] . '</span></td>';
        $html_data .= '  </tr>';

        $html_data .= '  <tr>';
        $html_data .= '  <td>Reservation</td>';
        $html_data .= '  <td>';
        $html_data .= '  <div class="progress progress-xs">';

        $html_data .= ' <div id="progress-reservation" class="progress-bar bg-' . $this->choose_bg_color($reservation_occupancy, 2) . '" style="width: ' . $reservation_occupancy . '%"></div>';
        $html_data .= '</div>';
        $html_data .= '</td>';
        $html_data .= '   <td><span id="badge-reservation" class="badge bg-' . $this->choose_bg_color($reservation_occupancy, 2) . '">' . $data['reservation_current_level'] . '/' . $data['total_reservation_spaces'] . '</span></td>';
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
        $html_data .= '   <td id="shortterm-entry">' . $data['shortterm_entry'] . '</td>';
        $html_data .= '   <td id="shortterm-exit">' . $data['shortterm_exit'] . '</td>';
        $html_data .= '  </tr>';
        $html_data .= '  <tr>';
        $html_data .= '   <td>Access</td>';
        $html_data .= '   <td id="access-entry">' . $data['access_entry'] . '</td>';
        $html_data .= '   <td id="access-exit">' . $data['access_exit'] . '</td>';
        $html_data .= '  </tr>';

        $html_data .= '  <tr>';
        $html_data .= '   <td>Reservation</td>';
        $html_data .= '   <td id="reservation-entry">' . $data['reservation_entry'] . '</td>';
        $html_data .= '   <td id="reservation-exit">' . $data['reservation_exit'] . '</td>';
        $html_data .= '  </tr>';

        $html_data .= '  <tr>';
        $html_data .= '   <td>Manual</td>';
        $html_data .= '   <td id="manual-entry">' . $data['total_manual_entry'] . '</td>';
        $html_data .= '   <td id="manual-exit">' . $data['total_manual_exit'] . '</td>';
        $html_data .= '  </tr>';


        $html_data .= '  </table>';
        $html_data .= ' </div>';

        $html_data .= '  </div>';
        $html_data .= '  </div>'; //col md 4     
        mysqli_close($con);
        echo $html_data;
    }

// End . Occupancy Carpark Detail 

    function UpdateOccupancyCarparkDetail($facility_number, $carpark_number) {
        $con = $this->db_connect();
        $query_string = "select * from counters where counter_type=1 and facility_number=" . $facility_number . " and carpark_number=" . $carpark_number;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $data = mysqli_fetch_assoc($result);
        $gauge_percentage = ($data['current_level'] / $data['total_spaces']) * 100;
        $json->occupancy_percentage = $data['occupancy_percentage'];
        $json->gauge_color = $this->choose_bg_color($gauge_percentage, 1);
        $json->current_level = $data['current_level'];
        $json->total_spaces = $data['total_spaces'];
        $json->shortterm_current_level = $data['shortterm_current_level'];
        $json->shortterm_total_spaces = $data['total_shortterm_spaces'];
        $json->access_current_level = $data['access_current_level'];
        $json->access_total_spaces = $data['total_access_spaces'];
        $json->reservation_current_level = $data['reservation_current_level'];
        $json->reservation_total_spaces = $data['total_reservation_spaces'];

        $json->shortterm_entries = $data['shortterm_entry'];
        $json->shortterm_exits = $data['shortterm_exit'];
        $json->access_entries = $data['access_entry'];
        $json->access_exits = $data['access_exit'];
        $json->reservation_entries = $data['reservation_entry'];
        $json->reservation_exits = $data['reservation_exit'];
        $json->manual_entries = $data['total_manual_entry'];
        $json->manual_exits = $data['total_manual_exit'];
        $json->last_updated_datetime = $data['last_updated_datetime'];

        mysqli_close($con);
        echo json_encode($json);
    }

// end . Function . Dashboard Occupancy CarparkDetail

    function OccupancyHourly($carpark, $type) {
        $total_count = array();
        $i = 0;
        $j = 0;
        $k = 1;

        $con = $this->db_connect_reporting();

        if (!$con)
            $this->error("Connection attempt failed");

        //Start . Generate the SQL Query String . 
        $query_string = "select";
        while ($i < 24) {
            $query_string .= " sum(h" . $i . "to" . ($i + 1) . ") as h" . $i . "to" . ($i + 1);
            if ($i < 23) {
                $query_string .= ",";
            }
            $i++;
        }


        $query_string .= " from hourly_occupancy where report_date =curdate() and type='" . $type . "'";

        if ($carpark > 0) {
            $query_string .= "and carpark_number=" . $carpark;
        }


        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));

        if (mysqli_num_rows($result) > 0) {
            $data = mysqli_fetch_assoc($result);
            $i = 0;
            $j = 0;
            //get count for hours
            while ($i < 24) {
                $field = "h" . $i . "to" . ($i + 1);
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
    }

// End Hourly Occupancy
}

// End Class