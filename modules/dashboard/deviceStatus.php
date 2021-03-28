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

<div class="content-wrapper">

    <section class="content">
        <div class="container-wide">
            <div class="row" id="device-status-block" >

            </div>
        </div>
    </section>

</div>
</div>

<script src="../../dist/js/dashboard.js"></script>
<script src="../../dist/js/manualOperations.js"></script>

<script>
    var facility_number=0;
    var carpark_number=0;
    var current_level;
    function get_live_device_Status()
    {
        $.get("../ajax/dashboard.php?task=22", function (data) {
            $('#device-status-block').html(data);
        });
    }
    
    function get_device_status_by_carpark()
        {
        $.get("../ajax/dashboard.php?task=23&facility_number=" + facility_number, function (data) {
            $('#device-status-block').html(data);
        });    
        }
        
     function get_device_status_by_device()
        {
        $.get("../ajax/dashboard.php?task=24&facility_number=" + facility_number + "&carpark_number=" + carpark_number, function (data) {
            $('#device-status-block').html(data);
        });    
        }
        
    get_live_device_Status();

    $(document).on('click', '.show-facility-details', function () {
        facility_number = $(this).attr("facility_number");
        get_device_status_by_carpark();
    });

    $(document).on('click', '.show-carpark-details', function () {
        facility_number = $(this).attr("facility_number");
        carpark_number = $(this).attr("carpark_number");
        get_device_status_by_device();
    });
    
function update_status()
    {
    current_level=$("#current_level").val();        
    if(current_level=="facility")
        get_live_device_Status();
    else if(current_level=="carpark")
        {
        facility_number = $("#current_level").attr("facility_number");    
        get_device_status_by_carpark();
        }
    else
        {
        facility_number = $("#current_level").attr("facility_number");  
        carpark_number = $("#current_level").attr("carpark_number");
        get_device_status_by_device();
        }    
    }

 setInterval(function () 
    {
    
    update_status();
    }, 10000);
</script>

<?php include('../../includes/footer.php'); ?>