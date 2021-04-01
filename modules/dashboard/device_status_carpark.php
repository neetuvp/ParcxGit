<?php
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

<div class="content-wrapper">    
    <div class="additional-menu row m-0 bg-white border-bottom">
        <div class="col d-flex justify-content-between pl-1 align-items-center">
            <span class="menu-info text-danger">
            <strong id="alarm_notification" data-toggle="modal" data-target="#alarm-modal"></strong>
            </span>
        </div>
    </div>
    <nav aria-label="breadcrumb">
        <ol class="breadcrumb">
            <li class="breadcrumb-item"><a href="device_status_facility.php">Facilities</a></li>
            <li class="breadcrumb-item active" aria-current="page">Carparks</li>
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
var facility_number =<?php echo $_GET["facility_number"] ;?>;
var carpark_number = 0;
var alarm_count


function get_device_status_by_carpark()
    {
    $.get("../ajax/dashboard.php?task=23&facility_number=" + facility_number, function (data) {
        $('#device-status-block').html(data);    
        alarm_count=$("#alarm_count").val();        
        if(alarm_count==0)      
            $('#alarm_notification').html("No alarms");
        else
            $('#alarm_notification').html("Alarm Present:"+alarm_count);
    });
    }
    
$(document).on('click', '.show-carpark-details', function () {
    facility_number = $(this).attr("facility_number");
    carpark_number = $(this).attr("carpark_number");
    window.location="device_status_device.php?facility_number="+facility_number+"&carpark_number="+carpark_number;
    
});

function get_alarmlist()
    {
    var data = {};
    data["device_number"]=0;
    data["facility_number"]=facility_number;
    data["carpark_number"]=carpark_number;    
    var jsondata = JSON.stringify(data);
    $.post("../ajax/dashboard.php?task=13", jsondata, function (result) {         
            $('#latest-alarm-data').html(result);   
            if($("#latest-alarm-data").find('#RecordsTable').length!=0)
                $('#RecordsTable').DataTable(
                    {
                    "paging": true,
                    "lengthChange":true,
                    "searching": true,
                    "ordering": true,
                    "info": true,
                    "autoWidth": false,
                    "lengthMenu": [[5,10, 25, 50, -1], [5,10, 25, 50, "All"]],
                    "aaSorting": []

                    });  
    });   
    }

$('body').on('click', "[data-target='#alarm-modal']", function () {                 
        get_alarmlist();                
});

$(document).on('click', '.btn-dismis-alarm', function () {
    var id = $(this).attr('id'); 
    var data = {};        
    data["id"]=id;        
    var jsondata = JSON.stringify(data);
    $.post("../ajax/dashboard.php?task=16", jsondata, function (result) {                            
        get_alarmlist(); 
        });     
    });


get_device_status_by_carpark();

setInterval(function ()
    {
    get_device_status_by_carpark();    
    }, 30000);
</script>

<?php include('../../includes/footer.php'); ?>