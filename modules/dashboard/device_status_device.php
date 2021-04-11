<?php
$page_title = "Device Dashboard";

include('../../includes/header.php');
?>

<div class="navbar-has-tablink navbar-has-sublink">

    <?php
    include('../../includes/navbar-start.php');
    ?>

</ul>


<div class="header text-dark" id="pdf-report-header">Device Status</div>


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
            <div class="modal-body p-4" >

                <!-- info -->
                <div class="row mb-4">
                    <div class="col-4">
                        <div class="border-simple p-4">
                            <img src="" class="mx-auto d-block img-fluid" id="device_img">
                        </div>
                    </div>
                    <div class="col-8">
                        <div class="border-simple h-100 p-3" id="device_details">

                        </div>
                    </div>
                </div>
                <!-- end / info -->  
                <div id="manual_operation" class="d-flex justify-content-between align-items-center mb-4 hidden">
                    <div class="col"><input type="submit" class="btn btn-outline-info btn-open-barrier btn-block" value="Open Barrier" id=""></div>
                    <div class="col"><input type="submit" class="btn btn-outline-info btn-close-barrier btn-block" value="Close Barrier" id=""></div>
                    <div class="col"><input type="submit" class="btn btn-outline-info btn-close-lane btn-block" value="Lane Closed Mode" id=""></div>
                    <div class="col"><input type="submit" class="btn btn-outline-info btn-free-passage btn-block" value="Free Passage Mode" id=""></div>
                    <div class="col"><input type="submit" class="btn btn-outline-info btn-standard-operation btn-block" value="Standard Operation Mode" id=""></div>
                </div>

                <div class="card-secondary card-outline card-tabs border-simple">
                    <div class="p-0 pt-1 border-bottom-0">
                        <ul class="nav nav-tabs" >

                            <li class="nav-item">
                                <a class="nav-link active"  data-toggle="pill" href="#alarm-data"   aria-selected="true">Alarms</a>
                            </li>

                            <li class="nav-item " id="nav-item-cash-level">
                                <a class="nav-link"  data-toggle="pill" href="#cash-level" aria-selected="false">Cash level</a>
                            </li>                                                        

                        </ul>
                    </div>
                    <div class="card-body">
                        <div class="tab-content">

                            <div class="tab-pane fade" id="cash-level">
                                cashlevels
                            </div>
                            <div class="tab-pane fade active show" id="alarm-data">
                                alarms
                            </div>
                        </div>
                    </div>
                    <!-- /.card -->
                </div>               
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
                <div  id="latest-alarm-data">

                </div>                
            </div>
        </div>
    </div>
</div>
<!-- / end modal -->

<!-- Modal > Manual open Reason -->
<div class="modal fade text-dark" id="detailModal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel" aria-hidden="true">
    <div class="modal-dialog" role="document" id="manual-reason-content">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title">Manual Operation Reason</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
            </div>
            <div class="modal-body pt-4 pl-4 pr-4">
                <p>Reason:</p>
                <textarea name='reason_text' id='reason_text' class="form-control mb-4"></textarea>
                <span id="reasonempty"></span>
            </div>
            <div class="modal-footer">
                <button type='button' class='btn btn-info' name='ok_reason' id='ok_reason' value='OK'>Ok</button>
                <button type='button' class='btn btn-info' name='cancel_reason' id='cancel_reason' value='Cancel'>Cancel</button>
            </div>
        </div>
    </div>
</div>
<!-- end / Modal -->

<div class="content-wrapper">

    <div class="additional-menu row m-0 bg-white border-bottom">
        <div class="col d-flex justify-content-between pl-1 align-items-center ">
            <div class="flex-grow-1 row additional-menu-left">
                <div class="col tab-header d-flex justify-content-left pl-1">
                    <div class="tab-link active" data-target="all">View All</div>
                    <div class="tab-link" data-target="payment_machines">Payment Machines</div>
                    <div class="tab-link" data-target="columns">Entry/Exit Columns</div>
                    <div class="tab-link" data-target="controllers">Controllers</div>
                    <div class="tab-link" data-target="camera">Camera</div>
                    <div class="tab-link" data-target="vms">VMS</div>
                    <div class="tab-link" data-target="valet">Valet</div>
                </div>
            </div>   
            <div class="additional-menu-right row align-items-center">               
                <button class="btn btn-danger" data-toggle="modal" data-target="#alarm-modal"><i class="fas fa-bell mr-1"></i><span id="alarm_notification">Alarms:100</span></button>                
            </div>
        </div>
    </div>    
    <section class="content">
        <div class="container-wide">
            <div class="row" id="device-status-block" >

            </div>                        
        </div>
    </section>

</div>
</div>


<script src="../../dist/js/manualOperations.js"></script>

<script>
    var facility_number = <?php echo $_GET["facility_number"]; ?>;
    var carpark_number =<?php echo $_GET["carpark_number"]; ?>;
    var device_number = 0;


    var $target = "all";

    function display_target_devices()
    {
        if ($target != 'all')
        {
            $('.block-data').addClass("hidden");
            $('.block-data[data-status="' + $target + '"]').removeClass("hidden").fadeIn('slow');
        } else
        {
            $('.block-data').removeClass("hidden").fadeIn('slow');
        }
    }

    $(document).ready(function ()
    {
        $('.tab-link').on('click', function ()
        {
            $target = $(this).data('target');
            $('.tab-link').removeClass('active');
            $(this).addClass('active');
            display_target_devices();
        });
    });

    var alarm_count = 0;
    function get_device_status_by_device()
    {
        $.get("../ajax/dashboard.php?task=24&facility_number=" + facility_number + "&carpark_number=" + carpark_number, function (data) {
            $('#device-status-block').html(data);
            alarm_count = $("#alarm_count").val();           
            $('#alarm_notification').html("Alarms :" + alarm_count);
            display_target_devices();
        });
    }
    var device_name;
    var device_ip;
    var device_img;
    var device_type;
    var device_category;
    var jsondata;

    $('body').on('click', "[data-target='#error-log-modal']", function ()
    {
        device_name = $(this).find(".card-title").text();
        //device_number = $(this).find(".device_number").text();
        device_number = $(this).attr("data-number")
        device_ip = $(this).find(".device_ip").text();
        device_category = $(this).find(".device_category").text();
        device_img = $(this).attr("data-img")
        device_type = $(this).attr("data-type");


        $("#device_img").attr("src", ("/parcx/dist/img/icon/device_icons/" + device_img + "-lg.png"));

        if (device_type == 1 || device_type == 2)
        {
            $("#manual_operation").removeClass("hidden");
            $("#manual_operation input").attr("id", device_number);
        } else
        {
            $("#manual_operation").addClass("hidden");
        }

        set_jsondata_input();
        get_device_details();
        get_alarmlist();

        $('#cash-level').html("");
        if (device_type == 4)
        {
            $("#nav-item-cash-level").removeClass("d-none");
            get_cashbox_level();
        } else
            $("#nav-item-cash-level").addClass("d-none");


    });

    function set_jsondata_input()
    {
        var data = {};
        data["device_number"] = device_number;
        data["facility_number"] = facility_number;
        data["carpark_number"] = carpark_number;
        data["device_category"] = device_category;
        jsondata = JSON.stringify(data);
    }

    function get_device_details()
    {
        $.post("../ajax/dashboard.php?task=25", jsondata, function (result) {
            $('#device_details').html(result);
        });
    }

    function get_cashbox_level()
    {
        $.post("../ajax/dashboard.php?task=26", jsondata, function (result) {
            $('#cash-level').html(result);
        });
    }

    function get_alarmlist()
    {
        $.post("../ajax/dashboard.php?task=13", jsondata, function (result) {

            if (device_number == 0)
            {
                $('#latest-alarm-data').html(result);
                if ($("#latest-alarm-data").find('#RecordsTable').length != 0)
                    $('#latest-alarm-data #RecordsTable').DataTable(
                            {
                                "lengthMenu": [[5,10, 25, 50, -1], [5,10, 25, 50, "All"]],
                            });
            } else
            {
                var details = '<input type="hidden" device_type="' + device_type + '" id="device_details_' + device_number + '" device_ip="' + device_ip + '" device_name="' + device_name + '" carpark_number="' + carpark_number + '">';
                $('#alarm-data').html(details + result);
                if ($("#alarm-data").find('#RecordsTable').length != 0)
                    $('#alarm-data #RecordsTable').DataTable(
                            {
                                "lengthMenu": [[5, 10, 25, 50, -1], [5, 10, 25, 50, "All"]],
                            });
            }

        });
    }

    $('body').on('click', "[data-target='#alarm-modal']", function () {
        device_number = 0;
        set_jsondata_input();
        get_alarmlist();
    });

    $(document).on('click', '.btn-dismis-alarm', function () {
        var id = $(this).attr('id');
        var data = {};
        data["id"] = id;
        var jsondata = JSON.stringify(data);
        $.post("../ajax/dashboard.php?task=16", jsondata, function (result) {
            get_alarmlist();
        });
    });


    get_device_status_by_device();

    setInterval(function ()
    {
        get_device_status_by_device();
    }, 30000);
</script>

<?php include('../../includes/footer.php'); ?>