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

<script>
var facility_number = 0;
var carpark_number=0;

function get_live_device_Status()
    {
    $.get("../ajax/dashboard.php?task=22", function (data) {
        $('#device-status-block').html(data); 
        carpark_number=$("#carpark_number").val();
        facility_number=$("#facility_number").val();
        if (typeof carpark_number !== "undefined")
            window.location="device_status_device.php?facility_number="+facility_number+"&carpark_number="+carpark_number;
        else if (typeof facility_number !== "undefined")  
            window.location="device_status_carpark.php?facility_number="+facility_number;        
    });
    }
    

$(document).on('click', '.show-facility-details', function () {
    facility_number = $(this).attr("facility_number");  
    window.location="device_status_carpark.php?facility_number="+facility_number;
});

get_live_device_Status();

setInterval(function ()
    {
    get_live_device_Status();   
    }, 30000);
    
</script>

<?php include('../../includes/footer.php'); ?>