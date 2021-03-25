<?php

/**
 * Description of reporting_valet
 *
 * @author Jude Dcoutho
 */
class reporting_valet {

    //put your code here

    function db_connect() {

        $con = mysqli_connect(DB_HOST, DB_USER, DB_PASSWORD, DB_NAME_REPORTING);

        if (!$con)
            $this->error("Connection attempt failed");
        else
            return $con;
    }

// end 

    function db_connect_dashboard() {

        $con = mysqli_connect(DB_HOST, DB_USER, DB_PASSWORD, DB_NAME_DASHBOARD);

        if (!$con)
            $this->error("Connection attempt failed");
        else
            return $con;
    }

// end 

    function db_connect_config() {

        $con = mysqli_connect(DB_HOST, DB_USER, DB_PASSWORD, DB_NAME);

        if (!$con)
            $this->error("Connection attempt failed");
        else
            return $con;
    }

// end 

    function get_carparks() {
        $con = $this->db_connect_config();
        $html_data = "";
        if ($con) {
            $query_string = "select carpark_name,carpark_number from system_carparks";
            $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
            // $html_data.="<option  value=0>All Carparks</option>"; 

            while ($data = mysqli_fetch_assoc($result)) {
                $html_data .= "<option  value=" . $data['carpark_number'] . ">" . $data['carpark_name'] . "</option>";
            }

            echo $html_data;
        }// end if 
        else {
            echo "<option>Not Available</option>";
        }

        mysqli_close($con);
    }

// End Function 

    function get_device_name($id) {
        $con = $this->db_connect_config();
        if ($con) {
            $query_string = "select device_name from system_devices where device_number=$id";
            $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
            $data = mysqli_fetch_assoc($result);
            return $data["device_name"];
        }
    }

    function get_valet_driver_utilization($FromDateTime, $ToDateTime, $carpark) {
        $con = $this->db_connect();
        $query_string = "select report_date,driver_name,sum(count_pickups) as count_pickups,sum(count_dropoffs) as count_dropoffs,"
                . "max(max_pickup_parking) as max_pickup_parking,min(min_pickup_parking) as min_pickup_parking,"
                . "max(max_parking_dropoff) as max_parking_dropoff,min(min_parking_dropoff) as min_parking_dropoff,"
                . "avg(avg_parking_dropoff) as avg_parking_dropoff,avg(min_pickup_parking) as avg_pickup_parking"
                . " from valet_driver_utilization where report_date between '" . $FromDateTime . "' AND '" . $ToDateTime . "' group by driver_id";


//$query_string="select * from valet_driver_utilization where report_date between '".$FromDateTime."' AND '".$ToDateTime."' group by driver_id";
        if ($carpark != 0) {
            $query_string .= " AND carpark_number=" . $carpark;
        }

//echo $query_string;

        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $html_data = "";

        $header = " <table class='table table-condensed' >";
        $header .= "<tr>";
        $header .= "<th>Driver Name</th>";
        $header .= "<th>Pickups</th>";
        $header .= "<th>Drop Offs</th>";
        $header .= "<th>Max Time Pickup </th>";
        $header .= "<th>Min Time Pickup</th>";
        $header .= "<th>Max Time Drop Off</th>";
        $header .= "<th>Min Time DropOff</th>";
        $header .= "<th>Avg Time Pickup</th>";
        $header .= "<th>Avg Time Dropoff</th>";

        $header .= "</tr>";

        $html_data = $header;

        while ($data = mysqli_fetch_assoc($result)) {
            $html_data .= "<tr>";
            $html_data .= "<td>" . $data['driver_name'] . "</td>";
            $html_data .= "<td> " . $data['count_pickups'] . " </td>";
            $html_data .= "<td>" . $data['count_dropoffs'] . "</td>";
            $html_data .= "<td>" . $data['max_pickup_parking'] . "</td>";
            $html_data .= "<td>" . $data['min_pickup_parking'] . "</td>";
            $html_data .= "<td>" . $data['max_parking_dropoff'] . "</td>";
            $html_data .= "<td>" . $data['min_parking_dropoff'] . "</td>";
            $html_data .= "<td>" . $data['avg_pickup_parking'] . "</td>";
            $html_data .= "<td>" . $data['avg_parking_dropoff'] . "</td>";
            $html_data .= "</tr>";
        }

        $html_data .= "</table>";
        mysqli_close($con);
        echo $html_data;
    }

// End . Driver Utilization

    function get_valet_parking_transactions($FromDateTime, $ToDateTime, $carpark) {
        $con = $this->db_connect();
        $query_string = "select * from valet_parking where valet_in_datetime between '" . $FromDateTime . "' AND '" . $ToDateTime . "'";
        if ($carpark != 0) {
            $query_string .= " AND carpark_number=" . $carpark;
        }

        if (strlen($operation) > 1) {
            $query_string .= " AND action LIKE '%" . $operation . "%'";
        }

        $query_string .= " ORDER BY carpark_number ASC";
//echo $query_string;

        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        if (mysqli_num_rows($result)) {
            $html_data .= $this->valet_parking_summary($FromDateTime, $ToDateTime, $carpark, 0);


            $header = " <table width='100%' class='jspdf-table'>";
            $header .= "<tbody class='table-view'>";
            $header .= "<tr class='card-header d-flex justify-content-between align-items-center'>";
            $header .= "<th class='col'>#</th>";
            $header .= "<th class='col'>Plate Number</th>";
            $header .= "<th class='col-2'>Ticket Id</th>";
            $header .= "<th class='col'></th>";
            $header .= "<th class='col'>Valet IN</th>";
            $header .= "<th class='col'>Valet OUT</th>";
            $header .= "<th class='col'>Country</th>";
            $header .= "<th class='col'>City / Emirate</th>";
            $header .= "<th class='col'>Parking Duration</th>";
            $header .= "<th class='col'>Pickup To Parking(m)</th>";
            $header .= "<th class='col'>Parking To DropOff (m)</th>";
            $header .= "<th class='col'>Dropoff To Delivery (m)</th>";
//Neetu
            if ($_SESSION["userRollName"] == "Admin" || $_SESSION["userRollName"] == "CarParkManager") {
                $header .= "<th class='col'>Edit</th>";
            }
//Neetu
            $header .= "</tr>";

            $html_data = $header;
//Neetu - add id for <td>
            $i = 0;
            while ($data = mysqli_fetch_assoc($result)) {
                $i++;

                $html_data .= "<tr class='card-text d-flex justify-content-between align-items-center' id='view_pictures' valet_id='" . $data['id'] . "'>";
                $html_data .= "<td class='col'>" . $i . "</td>";
                $html_data .= "<td class='col' id='plateno" . $data['id'] . "'>" . $data['plate_number'] . "</td>"; //Neetu-Remove extra space
                $html_data .= "<td class='col-2' id='ticketid" . $data['id'] . "'>" . $data['ticket_number'] . "</td>";
                $html_data .= "<td class='col'><a href='#' class='view-img'  id=" . $data['id'] . ">View Images</a></td>";
                $html_data .= "<td class='col' id='valetin" . $data['id'] . "'>" . $data['valet_in_datetime'] . "</td>";
                $html_data .= "<td class='col'id='valetout" . $data['id'] . "'>" . $data['valet_out_datetime'] . "</td>";
                $html_data .= "<td class='col' id='country" . $data['id'] . "'>" . $data['plate_country'] . "</td>";
                $html_data .= "<td class='col' id='emirate" . $data['id'] . "'>" . $data['plate_emirate'] . "</td>";
                $html_data .= "<td class='col' id='parkingduration" . $data['id'] . "'>" . $data['duration_parking'] . "</td>";
                $html_data .= "<td class='col' id='pickupduration" . $data['id'] . "'>" . $data['duration_pickup_parking'] . "</td>";
                $html_data .= "<td class='col' id='dropoff" . $data['id'] . "'>" . $data['duration_parking_dropoff'] . "</td>";
                $html_data .= "<td class='col' id='delivery" . $data['id'] . "'>" . $data['duration_dropoff_delivery'] . "</td>";
                //Neetu
                if ($_SESSION["userRollName"] == "Admin" || $_SESSION["userRollName"] == "CarParkManager") {
                    $html_data .= "<td class='col'><input class='btn btn-info btn-block btn-edit-trx' id ='editvaletbtn" . $data['id'] . "' type='submit'  value='Edit' onclick = 'EditValetTransaction(" . $data['id'] . ")' id='" . $data['id'] . "' ></td>";
                }
                //Neetu
                $html_data .= "</tr>";
            }

            $html_data .= "</tbody>";
            $html_data .= "</table>";
            mysqli_close($con);
        } else {
            $html_data = "<div class='p-3 card'>No records/transactions available for the current search criteria</div>";
        }

        echo $html_data;
    }

// End . Valet Parking

    function valet_parking_summary($FromDateTime, $ToDateTime, $carpark_number, $weekdays) {
        $html_data = $this->keyinout_by_drivers_chart($FromDateTime, $ToDateTime);

        $con = $this->db_connect();
//$query_string="select MAX(duration_parking) as max_parking_duration,MIN(duration_parking) as min_parking_duration from valet_parking where valet_in_datetime between '".$FromDateTime."' AND '".$ToDateTime."'";
        $query_string = "select count(*) as count,min(NULLIF(duration_pickup_parking,0)) as min_pickup_parking,max(duration_pickup_parking) as max_pickup_parking,avg(duration_pickup_parking) as avg_pickup_parking,"
                . "max(duration_parking) as max_parking_duration,avg(duration_parking) as avg_parking_duration,"
                . "min(NULLIF(duration_parking_dropoff,0)) as min_parking_dropoff,max(duration_parking_dropoff) as max_parking_dropoff,avg(duration_parking_dropoff) as avg_parking_dropoff,"
                . "min(NULLIF(duration_dropoff_delivery,0)) as min_dropoff_delivery,max(duration_dropoff_delivery) as max_dropoff_delivery,avg(duration_dropoff_delivery) as avg_dropoff_delivery "
                . "from valet_parking where valet_in_datetime between '" . $FromDateTime . "' AND '" . $ToDateTime . "'";
//echo $query_string;        

        if ($carpark_number != 0) {
            $query_string .= " AND carpark_number=" . $carpark_number;
        }



        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $data_summary = mysqli_fetch_assoc($result);

        $html_data .= '<span id="chart-data" class="d-none" ';
        $html_data .= 'data-max-pickup-parking=' . $data_summary["max_pickup_parking"] . ' data-min-pickup-parking=' . $data_summary["min_pickup_parking"] . ' data-avg-pickup-parking=' . round($data_summary["avg_pickup_parking"], 2) . ' ';
        $html_data .= 'data-max-parking-dropoff=' . $data_summary["max_parking_dropoff"] . ' data-min-parking-dropoff=' . $data_summary["min_parking_dropoff"] . ' data-avg-parking-dropoff=' . round($data_summary["avg_parking_dropoff"], 2) . ' ';
        $html_data .= 'data-max-dropoff-delivery=' . $data_summary["max_dropoff_delivery"] . ' data-min-dropoff-delivery=' . $data_summary["min_dropoff_delivery"] . ' data-avg-dropoff-delivery=' . round($data_summary["avg_dropoff_delivery"], 2) . ' ';
        $html_data .= '></span>';

        $html_data .= '<div class="row mb-4 jspdf-graph">';
        /* $html_data.='<div class="col-lg-3 col-6">';
          $html_data.='<!-- small box -->';
          $html_data.='<div class="small-box bg-success">';
          $html_data.='<div class="inner">';
          $html_data.='<h3>'.$data_summary["max_parking_duration"].'/'.round($data_summary["avg_parking_duration"],2).'</h3>';
          $html_data.='<p>Max / Average Parking Duration </p>';
          $html_data.='</div>';
          $html_data.='<div class="icon">';
          $html_data.='<i class="ion ion-stats-bars"></i>';
          $html_data.='</div>';
          //$html_data.='<a href="#" class="small-box-footer"> <i class="fa fa-arrow-circle-right"></i></a>';
          $html_data.='</div>';
          $html_data.='</div>'; */

        $html_data .= '<div class="col-lg-3 col-6">';
        $html_data .= '<!-- small box -->';
        $html_data .= '<div class="small-box bg-success">';
        $html_data .= '<div class="inner">';
        $html_data .= '<h3>' . $data_summary["max_pickup_parking"] . '/' . round($data_summary["avg_pickup_parking"], 2) . '</h3>';
        $html_data .= '<p>Max / Average time from pickup to parking</p>';
        $html_data .= '</div>';
        $html_data .= '<div class="icon">';
        $html_data .= '<i class="ion ion-stats-bars"></i>';
        $html_data .= '</div>';
        $html_data .= '</div>';
        $html_data .= '</div>';

        $html_data .= '<div class="col-lg-3 col-6">';
        $html_data .= '<!-- small box -->';
        $html_data .= '<div class="small-box bg-success">';
        $html_data .= '<div class="inner">';
        $html_data .= '<h3>' . $data_summary["max_parking_dropoff"] . '/' . round($data_summary["avg_parking_dropoff"], 2) . '</h3>';
        $html_data .= '<p>Max / Average time from carpark to podium</p>';
        $html_data .= '</div>';
        $html_data .= '<div class="icon">';
        $html_data .= '<i class="ion ion-stats-bars"></i>';
        $html_data .= '</div>';
        $html_data .= '</div>';
        $html_data .= '</div>';

        $html_data .= '<div class="col-lg-3 col-6 pr-0">';
        $html_data .= '<!-- small box -->';
        $html_data .= '<div class="small-box bg-success">';
        $html_data .= '<div class="inner">';
        $html_data .= '<h3>' . $data_summary["max_dropoff_delivery"] . '/' . round($data_summary["avg_dropoff_delivery"], 2) . '</h3>';
        $html_data .= '<p>Max / Average time from podium to delivery</p>';
        $html_data .= '</div>';
        $html_data .= '<div class="icon">';
        $html_data .= '<i class="ion ion-stats-bars"></i>';
        $html_data .= '</div>';
        $html_data .= '</div>';
        $html_data .= '</div>';

        $html_data .= '<div class="col-lg-3 col-6 pr-0">';
        $html_data .= '<!-- small box -->';
        $html_data .= '<div class="small-box bg-success">';
        $html_data .= '<div class="inner">';
        $html_data .= '<h3>' . $data_summary["count"] . '</h3>';
        $html_data .= '<p>Total Valet</p>';
        $html_data .= '</div>';
        $html_data .= '<div class="icon">';
        $html_data .= '<i class="ion ion-stats-bars"></i>';
        $html_data .= '</div>';
        $html_data .= '</div>';
        $html_data .= '</div>';


        $html_data .= '</div><!-- Row End -->';
        $html_data .= '</div>';

        mysqli_close($con);

        echo $html_data;
    }

// End Function 

    function get_valet_counters() {

        $con = $this->db_connect();
        $html_data = "";

        $from = date("Y-m-d") . " 00:00:00";
        $to = date("Y-m-d") . " 23:59:59";

        $query_string = "select count(*) as occupied_spaces from valet_parking where valet_out=0 AND valet_in_datetime between '" . $from . "' AND '" . $to . "'";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $data = mysqli_fetch_assoc($result);

        //

        $html_data .= '<div class="row mb-2">';

        $html_data .= '<div class="col-auto">';
        $html_data .= '<div class="info-box">';
        // $html_data.= '<span class="info-box-icon bg-info-gradient"><i class="fas fa-car-side"></i></span>';
        $html_data .= '<span class="info-box-icon bg-info-gradient"><img src="/parcx/dist/img/icon/icon-occupied.png"></span>';
        $html_data .= '<div class="info-box-content">';
        $html_data .= '<span class="info-box-number">' . $data["occupied_spaces"] . ' Occupied</span>';
        $html_data .= '<span class="info-box-text">Spaces out of ' . TOTALPARKING . '</span>';
        $html_data .= '</div>';
        $html_data .= '</div>';
        $html_data .= '</div>';

        $html_data .= '<div class="col-auto">';
        $html_data .= '<div class="info-box">';
        // $html_data.= '<span class="info-box-icon bg-info-gradient"><i class="fas fa-car-side"></i></span>';
        $html_data .= '<span class="info-box-icon bg-info-gradient"><img src="/parcx/dist/img/icon/icon-available.png" class="img-fluid"></span>';
        $html_data .= '<div class="info-box-content">';
        $html_data .= '<span class="info-box-number">' . (TOTALPARKING - (int) $data["occupied_spaces"]) . ' Available</span>';
        $html_data .= '<span class="info-box-text">Spaces out of ' . TOTALPARKING . '</span>';
        $html_data .= '</div>';
        $html_data .= '</div>';
        $html_data .= '</div>';



        $query_string = "select count(*) as onroute_pickup_parking from valet_parking where valet_out=0 and parking_in=0 AND
    valet_in_datetime between '" . $from . "' AND '" . $to . "'";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $data = mysqli_fetch_assoc($result);

        $html_data .= '<div class="col-auto">';
        $html_data .= '<div class="info-box">';
        $html_data .= '<span class="info-box-icon bg-info-gradient"><i class="fas fa-external-link-alt fa-rotate-180"></i></span>';
        $html_data .= '<div class="info-box-content">';
        $html_data .= '<span class="info-box-number">' . $data["onroute_pickup_parking"] . '</span>';
        $html_data .= '<span class="info-box-text">To parking area</span>';
        $html_data .= '</div>';
        $html_data .= '</div>';
        $html_data .= '</div>';


        $query_string = "select count(*) as onroute_podium_pickup from valet_parking where valet_out=0 and parking_out=1 AND
    valet_in_datetime between '" . $from . "' AND '" . $to . "'";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $data = mysqli_fetch_assoc($result);
        $onroute_podium = $data["onroute_podium_pickup"];

        $html_data .= '<div class="col-auto">';
        $html_data .= '<div class="info-box">';
        $html_data .= '<span class="info-box-icon bg-info-gradient"><i class="fas fa-external-link-alt"></i></span>';
        $html_data .= '<div class="info-box-content">';
        $html_data .= '<span class="info-box-number">' . $onroute_podium . '</span>';
        $html_data .= '<span class="info-box-text">To podium for dropoff</span>';
        $html_data .= '</div>';
        $html_data .= '</div>';
        $html_data .= '</div>';

        $query_string = "select count(*) as valet from valet_parking where valet_out=1 and valet_in_datetime between '" . $from . "'
    AND '" . $to . "'";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $data = mysqli_fetch_assoc($result);

        $html_data .= '<div class="col-auto">';
        $html_data .= '<div class="info-box">';
        $html_data .= '<span class="info-box-icon bg-success-gradient"><i class="fa fa-car"></i></span>';
        $html_data .= '<div class="info-box-content">';
        $html_data .= '<span class="info-box-number">' . $data["valet"] . '</span>';
        $html_data .= '<span class="info-box-text">Cars valeted today</span>';
        $html_data .= '</div>';
        $html_data .= '</div>';
        $html_data .= '</div>';

        $html_data .= '</div>';

        //

        mysqli_close($con);

        echo $html_data;
    }

    function valet_parking_keydashboard() {
        // $html_data="<div class='parking-summary mb-4'>";
// $html_data.="<div class='row'>";
// $html_data.=$this->get_valet_counters();
// $html_data.="</div>";
// $html_data.="</div>";

        $con = $this->db_connect_dashboard();

//$query_string="select MAX(duration_parking) as max_parking_duration,MIN(duration_parking) as min_parking_duration from valet_parking where valet_in_datetime between '".$FromDateTime."' AND '".$ToDateTime."'";
        $query_string = "select plate_prefix,plate_number,car_image,position from valet_keys order by position ASC";

        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));

        $html_data .= '<div class="keymanager d-flex row">';

// while($data_summary=mysqli_fetch_assoc($result)) {
//     $html_data.='<div class="keybox col-md-2">';
//     if($data_summary["plate_number"] == "0") {
//         $html_data.='<div class="position-inactive">';        
//     } else {
//         $html_data.='<div class="position">';    
//         $html_data.='<div id="icon"></div> ';
//     }
//     $html_data.='<div class="text">'.$data_summary["position"].' </div>    ';                 
//     $html_data.='</div>';
//     $imageurl=$this->getImageUrl($data_summary["car_image"]);
//     $html_data.= '<div class="vehicle">';
//     $html_data.='<div class="front-image"> <img src="'.$imageurl.'"></div>';  
//     if($data_summary["plate_number"] == "0") {
//         $html_data.='<br/>##-######';
//     } else {
//         $html_data.='<br/>'.$data_summary["plate_prefix"].'-'.$data_summary["plate_number"];            
//     }
//     $html_data.='</div>';
//     $html_data.='</div>';
// }

        while ($data_summary = mysqli_fetch_assoc($result)) {

            $html_data .= '<div class="keybox col-md-2 mb-3">';
            $html_data .= '<div class="card">';

            // header

            $html_data .= '<div class="card-header text-center"><h4>' . $data_summary["position"] . '</h4></div> ';


            $imageurl = $this->getImageUrl($data_summary["car_image"]);

            $html_data .= '<img height="180" width="100%" src="' . $imageurl . '" >';

            // plate number

            if ($data_summary["plate_number"] == "0") {
                $html_data .= '<div class="card-footer text-muted text-center"><h5>##-######</h5></div>';
            } else {
                $html_data .= '<div class="card-footer text-muted text-center"><h5>' . $data_summary["plate_prefix"] . '-' . $data_summary["plate_number"] . '</h5></div>';
            }

            $html_data .= '</div>';
            $html_data .= '</div>';
        }

        $html_data .= '</div>';

// pagination
// $html_data.='<ul id="pagin"></ul>';
        $html_data .= '<ul class="pagination justify-content-center split-pagination"></ul>';

        mysqli_close($con);

        echo $html_data;
    }

// End Function 

    function getImageUrl($image) {

        // image
        if ($image == "default_car.jpg" || $image == "default.jpg")
            $imageurl = IMAGEURL . "default_car.jpg";
        else {
            $con = $this->db_connect();
            $query_string = "select valet_in_datetime from valet_parking where front_plate='" . $image . "'";
            //echo $query_string;
            $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
            $data_summary = mysqli_fetch_assoc($result);

            $folder = $data_summary['valet_in_datetime'];
            $folder = substr($folder, 8, 2) . "-" . substr($folder, 5, 2) . "-" . substr($folder, 0, 4);
            $folder = $folder . '/';
            $imageurl = IMAGEURL . $folder . $image;
            mysqli_close($con);
            //echo $imageurl;
        }
        return $imageurl;
    }

    function get_valet_car_pictures($id) {
        $con = $this->db_connect();

        $query_string = "select front_plate,left_plate,right_plate,back_plate,valet_in_datetime from valet_parking where id=" . $id;        
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));

        $html_data = "";
        $data_summary = mysqli_fetch_assoc($result);

        
        $html_data .= '<div class="row mb-2">';        
        $html_data .= '<div class="col"> <img src=' . $this->getImageUrl($data_summary["front_plate"]) . '></div>';    
        $html_data .= '<div class="col "> <img src=' . $this->getImageUrl($data_summary["right_plate"]) . '></div>';
        $html_data .= '</div>';

        $html_data .= '<div class="row">';
        $html_data .= '<div class="col"> <img src=' . $this->getImageUrl($data_summary["left_plate"]) . '></div>';       
        $html_data .= '<div class="col"> <img src=' . $this->getImageUrl($data_summary["back_plate"]) . '></div>';
        $html_data .= '</div>';


        mysqli_close($con);

        echo $html_data;
    }

// End Function 
//Neetu

    function edit_valet($id, $plateno, $country, $emirate, $carbrand, $carcolor, $carmodel, $frontplate, $rightplate, $leftplate, $backplate) {
        $con = $this->db_connect();
        $query = "update valet_parking set plate_number='" . $plateno . "',plate_country='" . $country . "',plate_emirate = '" . $emirate . "',front_plate = '" . $frontplate . "',right_plate = '" . $rightplate . "',left_plate = '" . $leftplate . "',back_plate = '" . $backplate . "',car_brand = '" . $carbrand . "',car_color='" . $carcolor . "',car_model='" . $carmodel . "' where id=" . $id;
        mysqli_query($con, $query)or die(mysql_error());
        mysqli_close($con);

        //To move the uploaded images from temp folder to uploads folder
        //$carimagename = explode("/",$frontcarimagefolder);
        //rename($_SERVER['DOCUMENT_ROOT'].'/parcx/tempimages/'.end($carimagename),$_SERVER['DOCUMENT_ROOT'].'/Parcx/uploads');
    }

//Neetu 

    function get_valet_car_details($id) {
        $con = $this->db_connect();

        $query_string = "select car_brand,car_color,car_model from valet_parking where id=" . $id;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $data_summary = mysqli_fetch_assoc($result);
        $data->car_brand = $data_summary['car_brand'];
        $data->car_color = $data_summary['car_color'];
        $data->car_model = $data_summary['car_model'];
        $jsondata = json_encode($data);
        echo $jsondata;
        mysqli_close($con);
    }

//Neetu 

    function edit_valet_car_pictures($id) {
        $con = $this->db_connect();

        $query_string = "select plate_prefix,plate_number,front_plate,left_plate,right_plate,back_plate,valet_in_datetime from valet_parking where id=" . $id;
        //echo $query_string;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));

        $html_data = "";
        $data_summary = mysqli_fetch_assoc($result);

        $folder = $data_summary['valet_in_datetime'];
        $folder = substr($folder, 8, 2) . "-" . substr($folder, 5, 2) . "-" . substr($folder, 0, 4);
        $folder = $folder . '/';

        $editfolder = date("d-m-Y");
        $frontplatename = trim($data_summary['plate_prefix']) . trim($data_summary['plate_number']) . date("dMYHis") . 'front.jpg';
        $leftplatename = trim($data_summary['plate_prefix']) . trim($data_summary['plate_number']) . date("dMYHis") . 'left.jpg';
        $rightplatename = trim($data_summary['plate_prefix']) . trim($data_summary['plate_number']) . date("dMYHis") . 'right.jpg';
        $backplatename = trim($data_summary['plate_prefix']) . trim($data_summary['plate_number']) . date("dMYHis") . 'back.jpg';
        $html_data .= '<div class="car_image float-left pt-4">';

        //$html_data.='<div class="front-image"> <img src="http://192.168.1.199/ValetParking/uploads/'.$data_summary["front_plate"].'" ></div>';
        $html_data .= '<div class="front-image">Front<img id = "front_plate"  src=' . IMAGEURL . $data_summary["front_plate"] . '><input type ="file" id = "my_file_front" class="frontfile" accept = "image/*" /><input type = "hidden" id = "db_front_image_name" value =' . $data_summary["front_plate"] . '><input type = "hidden" id = "front_image_name" value =' . $frontplatename . '><input type = "hidden" id = "edit_image_folder" value =' . $editfolder . '><input type = "hidden" id = "front_edit" value =0></div>';
        $html_data .= '</div>';

        $html_data .= '<div class="car_image float-left  pt-4">';
        $html_data .= '<div class="front-image">Right<img src=' . IMAGEURL . $data_summary["right_plate"] . '><input type ="file" id = "my_file_right" class="rightfile" accept = "image/*" /><input type = "hidden" id = "db_right_image_name" value =' . $data_summary["right_plate"] . '><input type = "hidden" id = "right_image_name" value =' . $rightplatename . '><input type = "hidden" id = "right_edit" value =0></div>';
        $html_data .= '</div>';

        $html_data .= '<div class="car_image float-left  pt-4">';
        $html_data .= '<div class="front-image">Left<img src=' . IMAGEURL . $data_summary["left_plate"] . '><input type ="file" id = "my_file_left" class="leftfile" accept = "image/*" /><input type = "hidden" id = "db_left_image_name" value =' . $data_summary["left_plate"] . '><input type = "hidden" id = "left_image_name" value =' . $leftplatename . '><input type = "hidden" id = "left_edit" value =0></div>';
        $html_data .= '</div>';

        $html_data .= '<div class="car_image float-left  pt-4">';
        $html_data .= '<div class="front-image">Back<img src=' . IMAGEURL . $data_summary["back_plate"] . '><input type ="file" id = "my_file_back" class="backfile" accept = "image/*" /><input type = "hidden" id = "db_back_image_name" value =' . $data_summary["back_plate"] . '><input type = "hidden" id = "back_image_name" value =' . $backplatename . '><input type = "hidden" id = "back_edit" value =0></div>';
        $html_data .= '</div>';


        mysqli_close($con);

        echo $html_data;
    }

// End Function 

    function upload_valet_car_pictures() {
        if ($_FILES["my_file"]["name"] != '') {
            $date = new DateTime();
            $dateName = $date->format('dmYHis');
            $info = explode('.', strtolower($_FILES["my_file"]["name"]));
            $todir = 'images/';
            $loc = $todir . $dateName . "." . end($info);
            //  $name=$dateName.".".end($info);
            $name = $_POST["image_name"];
            $editfolder = $_POST["edit_image_folder"];
            if (!file_exists($_SERVER['DOCUMENT_ROOT'] . "/Parcx/uploads/" . $editfolder)) {
                mkdir($_SERVER['DOCUMENT_ROOT'] . "/Parcx/uploads/" . $editfolder, 0777, true);
            }
            if (move_uploaded_file($_FILES["my_file"]["tmp_name"], $_SERVER['DOCUMENT_ROOT'] . "/Parcx/uploads/" . $editfolder . "/" . $name)) {
                echo 1;
            } else {
                // echo "Not Uploaded".$_SERVER['DOCUMENT_ROOT']."/parcx/tempimages/".$name;
                // echo "Not Uploaded".$_SERVER['DOCUMENT_ROOT']."/Parcx/uploads/".$editfolder."/".$name;
                echo 0;
            }
            // echo $loc;
        }
    }

//Neetu

    function amount_collected_by_cashier() {
        $con = $this->db_connect();
        $current_date = date("Y-m-d") . " " . DAY_CLOSURE_START;
        $query_string = "select device_name,sum(gross_amount) as amount from revenue_payments where
payment_date_time>'$current_date' group by device_name";
//echo $query_string;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        if (mysqli_num_rows($result) > 0) {
            while ($data_summary = mysqli_fetch_assoc($result)) {
                echo '<tr class="card-text d-flex justify-content-between align-items-center">';
                echo'<td class="col"> ' . $data_summary["device_name"] . ' </td>';
                echo '<td class="col"> ' . CURRENCY . " " . $data_summary["amount"] . '</td>';
                echo '</tr>';
            }
        }

        mysqli_close($con);
    }

    function valet_by_device() {
        $con = $this->db_connect();
        $current_date = date("Y-m-d") . " " . DAY_CLOSURE_START;
        $query_string = "select device_number,count(valet_in_datetime) as valet_in,count(valet_out_datetime) as valet_out from valet_parking where
    valet_in_datetime>'$current_date' group by device_number";
        //echo $query_string;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        if (mysqli_num_rows($result) > 0) {
            while ($data = mysqli_fetch_assoc($result)) {
                echo '<tr class="card-text d-flex justify-content-between align-items-center">';
                echo'<td class="col">' . $this->get_device_name($data["device_number"]) . '</td>';
                echo'<td class="col"> ' . $data["valet_in"] . ' </td>';
                // echo'<td class="col"> '.$data["valet_out"].' </td>';
                echo'</tr>';
            }
        }

        mysqli_close($con);
    }

    function getDrivers() {
        $con = $this->db_connect_config();
        $query_string = "select valet_driver_name from valet_drivers";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        mysqli_close($con);
        return $result;
    }

    function keyinout_by_drivers() {
        $result = $this->getDrivers();
        $day_start = date("Y-m-d") . " " . DAY_CLOSURE_START;
        $day_end = date("Y-m-d") . " " . DAY_CLOSURE_END;
        while ($data = mysqli_fetch_assoc($result)) {
            echo '<tr class="card-text d-flex justify-content-between align-items-center">';
            echo'<td class="col"> ' . $data["valet_driver_name"] . ' </td>';
            echo'<td class="col"> ' . $this->key_in($data["valet_driver_name"], $day_start, $day_end) . '</td>';
            echo'<td class="col"> ' . $this->key_out($data["valet_driver_name"], $day_start, $day_end) . '</td>';
            echo '</tr>';
        }
    }

    function keyinout_by_drivers_chart($day_start, $day_end) {

        $result = $this->getDrivers();
        $html = "";

        $i = 1;
        while ($data = mysqli_fetch_assoc($result)) {
            $html .= '<span id="driver-chart-data-' . $i . '" class="d-none" ';
            $html .= 'data-driver-name="' . $data["valet_driver_name"] . '" data-key-in=' . $this->key_in($data["valet_driver_name"], $day_start, $day_end) . ' data-key-out=' . $this->key_out($data["valet_driver_name"], $day_start, $day_end) . ' ';
            $html .= '></span>';
            $i++;
        }
        $html .= '<span id="driver-chart-data" class="d-none" data-driver-count=' . $i . '></span> ';

        echo $html;
    }

    function key_in($driver_name, $day_start, $day_end) {
        $con = $this->db_connect();
        $query_string = "select count(valet_in_datetime) as valet_in from valet_parking where 
    valet_in_datetime between '$day_start' and '$day_end' and pickup_driver_name='$driver_name'";
        //echo $query_string;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $valet = 0;
        if ($data = mysqli_fetch_assoc($result))
            $valet = $data["valet_in"];
        mysqli_close($con);
        return $valet;
    }

    function key_out($driver_name, $day_start, $day_end) {
        $con = $this->db_connect();
        $query_string = "select count(valet_out_datetime) as valet_out from valet_parking where 
    valet_in_datetime between '$day_start' and '$day_end' and delivery_driver_name='$driver_name'";
        //echo $query_string;
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        $valet = 0;
        if ($data = mysqli_fetch_assoc($result))
            $valet = $data["valet_out"];
        mysqli_close($con);
        return $valet;
    }
    
  
    
    function get_car_details_to_parking_bay() {
        $con = $this->db_connect();
        $query_string = "select id,plate_number,plate_prefix,valet_in_datetime,front_plate,TIMESTAMPDIFF(MINUTE,valet_in_datetime,now()) as timer,customer_card_number From valet_parking where key_position is NULL and parking_bay is NULL and parking_in=0 and valet_out=0 and valet_in_datetime is not null order by valet_in_datetime desc";                
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con)); 
        $html="";
        while($data = mysqli_fetch_assoc($result))
            {
            $plate_number=$data["plate_prefix"].' '.$data["plate_number"];
            if($plate_number==" ")
                    $plate_number="## ####";
            $timer=$data["timer"];
            $hour=floor($timer/60);
            $minute=$timer%60;
            $timer=$hour." H ".$minute." M";
            $html.='<div class="card mb-2">';
            $html.='<div class="card-header p-2"><h3 class="card-title">'.$plate_number.'</h3></div>';
            $html.='<div class="card-body p-0">';
            $html.='<div class="row no-gutters"> ';                           
            $html.='<div class="col-2 my-auto text-center"><h1>'.$data["customer_card_number"].'</h1></div>';
            $html.='<div class="col-10">';
            $html.='<ul class="nav flex-column">';
            $html.='<li class="nav-item"><span class="nav-link">Entry date time <span class="float-right">'.$data["valet_in_datetime"].'</span></span></li>';
            $html.='<li class="nav-item"><span class="nav-link">Timer <span class="float-right">'.$timer.'</span></span></li>';
            $html.='</ul></div></div></div></div>';
            }
        echo $html;
        mysqli_close($con);        
    }
    
    function get_car_details_paid(){
        $con = $this->db_connect();       
        $query_string="select id,plate_number,plate_prefix,front_plate,key_position,parking_bay,payment_date_time,TIMESTAMPDIFF(MINUTE,payment_date_time,now()) as timer,customer_card_number  From valet_parking where paid_status=1 and parking_out=0 and parking_in=1 and valet_out=0 and payment_date_time is not null order by payment_date_time desc";               
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con)); 
        $html="";
        while($data = mysqli_fetch_assoc($result))
            {
            $plate_number=$data["plate_prefix"].' '.$data["plate_number"];
            if($plate_number==" ")
                    $plate_number="## ####";
            $timer=$data["timer"];
            $hour=floor($timer/60);
            $minute=$timer%60;
            $timer=$hour." H ".$minute." M";
            $html.='<div class="card mb-2">';
            $html.='<div class="card-header p-2"><h3 class="card-title">'.$plate_number.'</h3></div>';
            $html.='<div class="card-body p-0">';
            $html.='<div class="row no-gutters"> ';                           
            $html.='<div class="col-2 my-auto text-center"><h1>'.$data["customer_card_number"].'</h1></div>';
            $html.='<div class="col-10">';
            $html.='<ul class="nav flex-column">';
            $html.='<li class="nav-item"><span class="nav-link">Payment date time <span class="float-right">'.$data["payment_date_time"].'</span></span></li>';
            $html.='<li class="nav-item"><span class="nav-link">Timer <span class="float-right">'.$timer.'</span></span></li>';
            $html.='<li class="nav-item"><span class="nav-link">Key position :  '.$data["key_position"].'<span class="float-right">Parking position : '.$data["parking_bay"].'</span></span></li>';            
            $html.='</ul></div></div></div></div>';
            }
        echo $html;
        mysqli_close($con);        
    }
    
   
    function get_car_details_on_the_way_to_podium(){
        $con = $this->db_connect();       
        $query_string="select id,plate_number,plate_prefix,parking_out_datetime,TIMESTAMPDIFF(MINUTE,parking_out_datetime,now()),customer_card_number From valet_parking where parking_in=1 and parking_out=1 and valet_out=0 and ready_for_delivery=0 and paid_status=1 and parking_out_datetime is not null order by parking_out_datetime desc";                       
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con)); 
        $html="";
        while($data = mysqli_fetch_assoc($result))
            {
            $plate_number=$data["plate_prefix"].' '.$data["plate_number"];
            if($plate_number==" ")
                    $plate_number="## ####";
            $timer=$data["timer"];
            $hour=floor($timer/60);
            $minute=$timer%60;
            $timer=$hour." H ".$minute." M";
            $html.='<div class="card mb-2">';
            $html.='<div class="card-header p-2"><h3 class="card-title">'.$plate_number.'</h3></div>';
            $html.='<div class="card-body p-0">';
            $html.='<div class="row no-gutters"> ';                           
            $html.='<div class="col-2 my-auto text-center"><h1>'.$data["customer_card_number"].'</h1></div>';
            $html.='<div class="col-10">';
            $html.='<ul class="nav flex-column">';
            $html.='<li class="nav-item"><span class="nav-link">Parking out date time <span class="float-right">'.$data["parking_out_datetime"].'</span></span></li>';
            $html.='<li class="nav-item"><span class="nav-link">Timer <span class="float-right">'.$timer.'</span></span></li>';           
            $html.='</ul></div></div></div></div>';
            }
        echo $html;
        mysqli_close($con);        
    }
    
    function get_car_details_ready_to_delivery(){
        $con = $this->db_connect();       
        $query_string="select id,plate_number,plate_prefix,dropoff_datetime,TIMESTAMPDIFF(MINUTE,dropoff_datetime,now()),customer_card_number From valet_parking where parking_in=1 and parking_out=1 and paid_status=1 and valet_out=0 and ready_for_delivery=1 and dropoff_datetime>'".date(Y-m-d)." 00:00:00' order by dropoff_datetime desc";                       
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con)); 
        $html="";
        while($data = mysqli_fetch_assoc($result))
            {
            $plate_number=$data["plate_prefix"].' '.$data["plate_number"];
            if($plate_number==" ")
                    $plate_number="## ####";
            $timer=$data["timer"];
            $hour=floor($timer/60);
            $minute=$timer%60;
            $timer=$hour." H ".$minute." M";
            $html.='<div class="card mb-2">';
            $html.='<div class="card-header p-2"><h3 class="card-title">'.$plate_number.'</h3></div>';
            $html.='<div class="card-body p-0">';
            $html.='<div class="row no-gutters"> ';                           
            $html.='<div class="col-2 my-auto text-center"><h1>'.$data["customer_card_number"].'</h1></div>';
            $html.='<div class="col-10">';
            $html.='<ul class="nav flex-column">';
            $html.='<li class="nav-item"><span class="nav-link">Dop off date time <span class="float-right">'.$data["dropoff_datetime"].'</span></span></li>';
            $html.='<li class="nav-item"><span class="nav-link">Timer <span class="float-right">'.$timer.'</span></span></li>';           
            $html.='</ul></div></div></div></div>';
            }
        echo $html;
        mysqli_close($con);        
    }
    
function revenue_lastdays() {
    $con = $this->db_connect();
    $query_string = "SELECT DATE(payment_date_time) as report_date,DAYNAME(payment_date_time) as report_day, SUM(gross_amount) gross_amount FROM revenue_payments where payment_date_time >= DATE(NOW()) - INTERVAL 7 DAY GROUP BY  DATE(payment_date_time) ";

    $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
    $response = array();
    while ($data = mysqli_fetch_assoc($result)) 
    {
        $temp = [
            "day" => $data["report_day"],
            "amount" => $data["gross_amount"],
            "date" => $data["report_date"]
        ];
        array_push($response, $temp);
    }
    mysqli_close($con);        
    echo json_encode($response);
}

 function live_revenue(){
        $con = $this->db_connect();
        $query_string="select COALESCE(SUM(gross_amount),0) as gross_amount,COALESCE(SUM(parking_fee),0) as parking_fee,COALESCE(SUM(vat_amount),0) as vat_amount,COALESCE(SUM(lost_fee+admin_fixed_charges+ticket_replacement_fee),0) as lost_fee,COALESCE(SUM(discount_amount),0) as discount_amount from revenue_payments where DATE(payment_date_time)=CURDATE()";                        
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        if ($data = mysqli_fetch_assoc($result)) {
            $response["gross_amount"] = $data["gross_amount"];
            $response["parking_fee"] = $data["parking_fee"]-$data["discount_amount"];
            $response["vat_amount"] = $data["vat_amount"];
            $response["lost_fee"] = $data["lost_fee"];            
        }
        $query_string="select COALESCE(SUM(gross_amount),0) as product_sale_amount from revenue_payments where  payment_category='ProductSale' and DATE(payment_date_time)=CURDATE()";
        $result = mysqli_query($con, $query_string) or die(mysqli_error($con));
        if ($data = mysqli_fetch_assoc($result)) {
            $response["product_sale_amount"] = $data["product_sale_amount"];
        }
        mysqli_close($con);
        echo json_encode($response);
    }
}

// End Class 
