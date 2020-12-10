<?php 
$page_title="Device Dashboard";

include('../../includes/header.php');
?>

<div class="navbar-has-tablink navbar-has-sublink">

<?php
include('../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">Overview</div>

<div class="row">
    <div class="col tab-header d-flex justify-content-center">
        <div class="tab-header-link active" data-target="block-view">Block View</div>
        <div class="tab-header-link" data-target="table-view">Table View</div>
    </div>
</div>

<?php 
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

<!-- Modal -->
<div class="modal fade" id="error-log-modal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel"
    aria-hidden="true">
    <div class="modal-dialog modal-xl" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title" id="exampleModalLabel">Notifications</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
            </div>
            <div class="modal-body p-4">

                <!-- info -->
                <div class="row mb-4">
                    <div class="col-4">
                        <div class="border-simple p-4">
                            <img src="" class="mx-auto d-block img-fluid" id="device_img">
                        </div>
                    </div>
                    <div class="col-8">
                        <div class="border-simple h-100 p-3">
                            <h4 class="mb-3" id="device_name"></h4>
                            <p><strong>Category: </strong><span id="device_category"></span></p>
                            <p><strong>IP Address: </strong><span id="device_ip"></span></p>
                            <p><strong>Device number: </strong><span id="device_number"></span></p>
                        </div>
                    </div>
                </div>
                <!-- end / info -->

                <!-- table -->
                <table width="100%" id="alarm-data">
                    
                </table>
                <!-- end / table -->

            </div>
        </div>
    </div>
</div>
<!-- / end modal -->

<!-- Modal -->
<div class="modal fade" id="alarm-modal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel"
    aria-hidden="true">
    <div class="modal-dialog modal-xl" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title" id="exampleModalLabel">Alarms</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close" id="close-alarm-modal">
                    <span aria-hidden="true">&times;</span>
                </button>
            </div>
            <div class="modal-body p-4">             
                <!-- table -->
                <table width="100%" id="latest-alarm-data">

                </table>
                <!-- end / table -->

            </div>
        </div>
    </div>
</div>
<!-- / end modal -->

<div class="content-wrapper">

    <!-- tab-link header -->
    <div class="additional-menu row bg-white border-bottom">

            <div class="col-lg-9">
                <div class="col tab-header d-flex justify-content-left pl-1">
                    <div class="tab-link active" data-target="all">View All</div>
                    <div class="tab-link" data-target="payment_machines">Payment Machines</div>
                    <div class="tab-link" data-target="columns">Entry/Exit Columns</div>
                    <div class="tab-link" data-target="controllers">Controllers</div>
					<div class="tab-link" data-target="camera">Camera</div>
					<div class="tab-link" data-target="vms">VMS</div>
                </div>
            </div>

            <div class="col-lg-3 ml-sm-4 mb-sm-3 mt-sm-3 m-lg-0">
                <div class="nav-details">
                    <span class="text-bold m-0">
                        Current Date and Time:
                    </span>
                    <span id="current_date">
                    </span>
                </div>
            </div>

    </div>
    <!-- end / tab-link header -->

    <div class="additional-menu row bg-white border-bottom">
        <div class="col-md-12 d-flex align-items-center justify--between">
            <span class="menu-info text-danger">
            <strong id="alarm_notification" data-toggle="modal" data-target="#alarm-modal">Alarms Present</strong>
            </span>
<div id="action-buttons" class="col-md-2 btn-group btn-block pt-2 pb-2">
	     <button type="button" class="btn btn-warning">Export</button>
	     <button type="button" class="btn btn-warning dropdown-toggle" data-toggle="dropdown">
		  <span class="caret"></span>
	          <span class="sr-only">Toggle Dropdown</span>
	     </button>
	     <div class="dropdown-menu" role="menu">
	          <a class="dropdown-item" href="#" id="export_excel_report">Export to Excel</a>
	          <a class="dropdown-item" href="#" id="export_pdf_report">Export to PDF</a>
	     </div>
	</div>
        </div>
    </div>

    <section class="content">
        <div class="container-wide">
            <div class="col-md-12 table-view">
              
              <!-- gauges -->
              <div class="jspdf-graph">
                  <!-- <div class="js-gauge js-gauge--1 gauge" id="gauge-0"></div> -->
                  <!-- <div class="js-gauge js-gauge--2 gauge" id="gauge-1"></div> -->
                  <div class="row text-dark">
                            <div class="col-lg-3 col-sm-6 col-xs-12" id="gauge-value1-header">
                                <div class="chart-box text-center">
                                    <p class="text-center chart-header" id="gauge1_name">Carpark 1</p>
                                    <canvas class="text-center" id="carpark1" width="240px"></canvas>
                                    <p class="gauge-val" id="gauge-value1">0</p>
                                </div>
                            </div>
                            <div class="col-lg-3 col-sm-6 col-xs-12" id="gauge-value2-header">
                                <div class="chart-box text-center">
                                    <p class="text-center chart-header" id="gauge2_name">Carpark 2</p>
                                    <canvas class="text-center" id="carpark2" width="240px"></canvas>
                                    <p class="gauge-val" id="gauge-value2"></p>
                                </div>
                            </div>
                            <div class="col-lg-3 col-sm-6 col-xs-12" id="gauge-value3-header">
                                <div class="chart-box text-center">
                                    <p class="text-center chart-header" id="gauge3_name">Carpark 3</p>
                                    <canvas class="text-center" id="carpark3" width="240px"></canvas>
                                    <p class="gauge-val" id="gauge-value3"></p>
                                </div>
                            </div>
                            <div class="col-lg-3 col-sm-6 col-xs-12" id="gauge-value4-header">
                                <div class="chart-box text-center">
                                    <p class="text-center chart-header" id="gauge4_name">Carpark 4</p>
                                    <canvas class="text-center" id="carpark4" width="240px"></canvas>
                                    <p class="gauge-val" id="gauge-value4"></p>
                                </div>
                            </div>
                        </div>
              </div>
             <br>
                <div class="row">                    
                    <div class="col-md-12">
                        <div id="live-device_status-content">
                        </div>

                    </div>
                </div>
            </div>
        </div>
    </section>

</div>
</div>
<script src="../../plugins/gauge/raphael-2.1.4.min.js"></script>
<script src="../../plugins/flot/jquery.flot.min.js"></script>
<script src="../../plugins/flot/jquery.flot.categories.min.js"></script>
<script src="../../dist/js/gauge.js"></script>
<script src="../../dist/js/dashboard.js"></script>

<!-- add moment.js -->
<script src="../../plugins/jquery/moment.min.js"></script>

<script src="../../dist/js/dashboard.js"></script>

<script>
//////////////////////////////
    // create gauge arcs
    //////////////////////////////

    var opts = {
        angle: 0, // The span of the gauge arc
        lineWidth: 0.2, // The line thickness
        radiusScale: 1, // Relative radius
        pointer: {
            length: 0.58, // // Relative to gauge radius
            strokeWidth: 0.035, // The thickness
            color: '#000000' // Fill color
        },
        limitMax: false, // If false, max value increases automatically if value > maxValue
        limitMin: false, // If true, the min value of the gauge will be fixed
        // colorStart: '#fff', // Colors
        // colorStop: '#222',
        strokeColor: '#E0E0E0',
        percentColors: [
            [0.0, "#30B32D"],
            [0.50, "#FFDD00"],
            [1.0, "#F03E3E"]
        ],
        generateGradient: true,
        highDpiSupport: true, // High resolution support
        // renderTicks is Optional
        renderTicks: {
            divisions: 1,
            divWidth: 0.6,
            divLength: 1,
            divColor: '#666666',
            subDivisions: 8,
            subLength: 0.5,
            subWidth: 0.6,
            subColor: '#666666'
        }

    };

    get_gauge_names();
    DashboardUpdateCounters();
	
   
    function get_live_device_Status()
    {
    $.get("../ajax/dashboard.php?task=9", function (data) {
    $('#live-device_status-content').html(data);
     $('[data-target="' + clicked_view_type + '"]').click();
     $('[data-target="' + clicked_device_type + '"]').click();
    });
    $.get("../ajax/dashboard.php?task=14", function (data) {
    if(data==0)      
        $('#alarm_notification').html("No alarms");
    else
        $('#alarm_notification').html("Alarm Present:"+data);
    
    });

    }

    var clicked_view_type = "block-view";
    var clicked_device_type = "all";

    $(document).ready(function () {
 /* Table Show - Hide */
        $('.tab-link').on('click', function () {
            var $target = $(this).data('target');

            if ($target != 'all') {
                $('.card-text, .block-data').addClass("hidden");
                $("#error-log-modal .card-text").removeClass("hidden");
                $('.card-text[data-status="header"]').removeClass("hidden");
                $('.card-text[data-status="' + $target + '"], .block-data[data-status="' + $target +
                    '"]').removeClass("hidden").fadeIn('slow');
            } else {
                // all
                $('.card-text, .block-data').removeClass("hidden").fadeIn('slow');
                $('.card-text[data-status="header"]').removeClass("hidden").fadeIn('slow');
                console.log('else');
            }
            clicked_view_type = $target;
        });

        $('.tab-header-link').on('click', function () {
            var $target = $(this).data('target');

            if ($target === 'block-view') {

                $('[data-status="table-view"]').addClass("hidden");
                $('[data-status="' + $target + '"]').removeClass("hidden").fadeIn('slow');

            } else {

                $('[data-status="block-view"]').addClass("hidden");
                $('[data-status="' + $target + '"]').removeClass("hidden").fadeIn('slow');

            }
             clicked_device_type = $target;
        });

        get_live_device_Status();
        current_ime();;

        setInterval(function () {
            get_live_device_Status();
             get_gauge_names();
             DashboardUpdateCounters();
          
        }, 30000);

        setInterval(function () {
            current_ime();
            $('[data-toggle="tooltip"]').tooltip()
        }, 1000);

    });

    //dismiss
    $(document).on('click', '.btn-dismis-alarm', function () {
    var id = $(this).attr('id');
    var data = {id: id}; 
    var json = JSON.stringify(data);
   // alert(json);
     $.post("../ajax/dashboard.php?task=16", json)
     .done(function (result) {

       $.get("../ajax/dashboard.php?task=15", function (data) {
       $('#latest-alarm-data').html(data);
       });
     }, "json");    
    
    });

    /* Change Active Tab Styling */

    $('.tab-link').on('click', function () {
        $('.tab-link').removeClass('active');
        $(this).addClass('active');
    });

    $('.tab-header-link').on('click', function () {
        $('.tab-header-link').removeClass('active');
        $(this).addClass('active');
    });

    $('#export_excel_report').click(function (event) {
        export_to_excel("#live-device_status-content", "Device status")

    }); // end click event function

    /* Generate Modal Data */

    $('body').on('click', "[data-target='#error-log-modal']", function () {
        
        var device_name = $(this).find(".card-title").text();
        var device_number = $(this).find(".device_number").text();
        var device_ip = $(this).find(".device_ip").text();
        var device_category = $(this).find(".device_category").text();
        var device_img = $(this).attr("data-img")

        $("#device_name").html(device_name);
        $("#device_category").html(device_category);
        $("#device_ip").html(device_ip);
        $("#device_number").html(device_number);
        $("#device_img").attr("src", ("/parcx/dist/img/icon/device_icons/" + device_img + "-lg.png"));
        
        
        var data={};
        data["device_number"]=device_number;
        var temp=JSON.stringify(data);

        //alert(temp);
        $.ajax({
        type: "post",
        url: "../ajax/dashboard.php?task=13",
        data: temp,
        cache:false,
        contentType:"application/json",
        success:function(data){        
        $('#alarm-data').html(data);
        },
        error:function(jqXHR,textStatus,errorThrown){
        alert("error:"+errorThrown+" "+textStatus);
        }
        }); //end of ajax
    });
    //

//latest alarm
$('body').on('click', "[data-target='#alarm-modal']", function () {
    $.get("../ajax/dashboard.php?task=15", function (data) {
    $('#latest-alarm-data').html(data);
    
    });
});

//after close modal reload page
$("#close-alarm-modal").click(function(event){
   //location.reload();
    get_live_device_Status();
    get_gauge_names();
    DashboardUpdateCounters();

});
</script>

<?php include('../../includes/footer.php');?>
