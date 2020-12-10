<?php

$page_title="Application";

//# Import application layout.
include('../../../includes/header.php');
include('../../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">Plates Captured</div>

<?php
include('../../../includes/navbar-end.php');
include('../../../includes/sidebar.php');
include('../../../classes/reporting_parking.php');
$obj=new reporting_parking();
?>
<!-- Modal -->
<div class="modal fade" id="image-modal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel"
    aria-hidden="true">
    <div class="modal-dialog modal-xl" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title" id="exampleModalLabel">ANPR Image</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
            </div>
            <div class="modal-body p-4" id="image-content-modal">
            </div>
        </div>
    </div>
</div>
<!-- / end modal -->


<div class="content-wrapper">

    <!-- additional menu -->
    <div class="additional-menu row m-0 bg-white border-bottom">
        <div class="col d-flex pl-1 align-items-center">

            <div class="flex-grow-1 row additional-menu-left">

                <div class="col-md-2">
                    <select class="form-control" id="device">
                        <?php $obj->get_camera_devices() ?>
                    </select>
                </div>

                <div class="col-md-2">                    
                    <input type = "text" class="form-control" id="plate" placeholder="Plate Number" >
                </div>

                <div class="col-md-1">                    
                    <select class="form-control" id="plate_type" placeholder="Plate Type" >
                       <option value=''>Type</option>  
					   <option value='Police'>Police</option>
                       <option value='Transport'>Transport</option>  
                       <option value='Commercial'>Commercial</option>
                       <option value='Private'>Private</option>
                       <option value='Learning'>Learning</option>
                       <option value='CD_Diplomat'>CD_Diplomat</option>					   
					   <option value='CC_Diplomat'>CC_Diplomat</option>  
					   <option value='Protocol'>Protocol</option>
                       <option value='Government'>Government</option>  
                       <option value='Export'>Export</option>
                       <option value='Taxi'>Taxi</option>
                       <option value='Testing'>Testing</option>
                       <option value='Transfer'>Transfer</option>					   
					   <option value='truck'>Truck</option>
                       <option value='Bus'>Bus</option>
                       <option value='PublicTransport'>Public Transport</option>
                       <option value='PrivateTransport'>Private Transport</option>                                    
                    </select>
                </div>
                <div class="col-md-1">                    
                    <select class="form-control" id="plate_area" placeholder="Plate Area" >
                       <option value=''>Area</option>
					   <option value='AUH'>AUH</option>
                       <option value='DUB'>DUB</option>
                       <option value='SHJ'>SHJ</option>  
                       <option value='AJ'>AJ</option>
                       <option value='RAK'>RAK</option>
                       <option value='UAQ'>UAQ</option>
                       <option value='FUJ'>FUJ</option>                                    
                    </select>
                </div>

                <div class="col-md-1">                    
                    <select class="form-control" id="plate_country" placeholder="Plate Country" >
                       <option value=''>Country</option>  
                       <option value='UnitedArabEmirates'>United Arab Emirates</option> 
                       <option value='Kuwait'>Kuwait</option> 
                       <option value='Saudi Arabia'>Saudi Arabia</option> 
                       <option value='Oman'>Oman</option> 
                       <option value='Qatar'>Qatar</option> 
                       <option value='Bahrain'>Bahrain</option> 
                                    
                    </select>
                </div>

                <!-- date and time -->
                <div class="col-md-3">
                    <div class="input-group">
                        <div class="input-group-prepend">
                            <span class="input-group-text"><i class="far fa-clock"></i></span>
                        </div>
                        <input type="text" class="form-control float-right" id="reservationtime" autocomplete="off"
                            placeholder="Choose Date and Time Range">
                    </div>
                </div>

                <!-- search -->
                <div class="col-md-1">
                <button type="button" class="btn  btn-secondary" id="view-report-button">View Report</button>
                </div>

                <!-- loader -->
                <div class='col-1' id='loader'>
                    <img src='../../../dist/img/loading.gif'>
                </div>

            </div>

        </div>
    </div>
    <!-- end / additional menu -->

    <section class="content">
        <div class="container-wide">

            <!-- begin report -->
            <div class="card">
                <div class="card-body p-0" id="plates-captured-report-content">

                </div>
            </div>
            <!-- / end report -->

        </div>

</div>
</section>
</div>

<?php include('../../../includes/footer.php');?>

<script>
    $('#view-report-button').click(function (event) {
        var device = $("#device").val();
        var daterange = $("#reservationtime").val();
        var from = daterange.substring(0, 19);
        var to = daterange.substring(22, 41);
        var plate = $('#plate').val();
        var type = $('#plate_type').val();
        var area = $('#plate_area').val();
        var country = $('#plate_country').val();

        if ((!daterange)) {
            alert("choose date range");
        } else {
            var data = {
                device: device,
                toDate: to,
                fromDate: from,
                plate: plate,
                type: type,
                area: area,
                country: country
            };
            var temp = JSON.stringify(data);
            //alert(temp);
            $.post("../../ajax/parking.php?task=10", temp)
                .done(function (result) {                    
                    $("#plates-captured-report-content").html(result);
                    $("#loader").css("visibility","hidden");
                }, "json");
        } // end if 

        event.preventDefault();

    }); // 

    $('body').on('click', "[data-target='#image-modal']", function () {
    var id = $(this).data('value');
    var camera_no = $('#camera_no'+id).val();
    var plate_image = $('#plate_image'+id).val();	
    var date = $('#date'+id).val();	
    var data = {
        camera_no:camera_no,
        plate_image:plate_image,
        date:date
    };
    var json = JSON.stringify(data)    
    $.post("../../ajax/parking.php?task=9", json)
    .done(function (result) {        
    $("#image-content-modal").html(result);
    }, "json");
});


</script>