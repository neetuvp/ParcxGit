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
    <nav aria-label="breadcrumb">
        <ol class="breadcrumb">
            <li class="breadcrumb-item active" aria-current="page">Facilities</li>
        </ol>
    </nav>

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

function get_live_device_Status()
    {
    $.get("../ajax/dashboard.php?task=22", function (data) {
        $('#device-status-block').html(data);        
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