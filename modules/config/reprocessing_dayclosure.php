<?php
$page_title="Application Home";


//# Import application layout.
include('../../includes/header.php');
include('../../includes/navbar-start.php');



?>

</ul>
<div class="header text-dark" id="pdf-report-header">Reprocessing DayClosure Report</div>

<?php

include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>
<!-- Modal > Disable Valet Reason -->
<div class="modal fade text-dark" id="reprocess_confirm" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel" aria-hidden="true">
    <div class="modal-dialog" role="document" id="reprocess-confirm-content">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title">Confirm Reprocessing</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
            </div>
            
            <div class="modal-body pt-4 pl-4 pr-4">
                <input type="hidden" id="valet_id" value="">
                <h5><i class="icon fa fa-check"></i> Are you sure you want to reprocess the dayclosure report for the selected date?</h5>
                <br>
                
            </div>
            <div class="modal-footer">
                <button type='button' class='btn btn-info' name='ok_reprocess' id='ok_reprocess' value='OK'>Ok</button>
                <button type='button' class='btn btn-info' name='cancel_reprocess' id='cancel_reprocess'
                        value='Cancel'>Cancel</button>
            </div>
        </div>
    </div>
</div>
<!-- end / Modal -->


<div class="content-wrapper">

  <!-- additional menu -->
  <div class="additional-menu row m-0 bg-white border-bottom">
    <div class="col d-flex pl-1 align-items-center">

      <div class="flex-grow-1 row additional-menu-left">

                                  
          <!-- date and time -->
                <div class="col-md-3">
                    <div class="input-group">
                        <div class="input-group-prepend">
                            <span class="input-group-text"><i class="far fa-clock"></i></span>
                        </div>
                        <input type="text" class="form-control float-right" id="dcdate" autocomplete="off"
                            placeholder="Choose Date">
                    </div>
                </div>

                <!-- search -->
                <div class="col-md-1">
                    <button type="button" class="btn btn-block btn-secondary" id="reprocess-button">Reprocess</button>
                </div>

                <!-- loader -->
                <div class='col-1' id='loader'>
                    <img src='dist/img/loading.gif'>
                </div>
  <!--<section class="content">
    <div class="container-wide">

       
      <div class="card-body" id="report-content">   
	  
      </div>
    
</div>
</section>-->
</div>

<?php include('../../includes/footer.php');?>

<script>
//////////////////////////////
// load report data
//////////////////////////////
var id;

$(function() 
{

    
 $('#dcdate').daterangepicker({
   timePicker: false,
   format: 'YYYY-MM-DD',
   singleDatePicker: true
})
});

$('#reprocess-button').click(function(){
     $('#reprocess_confirm').modal('show');
});

$('#ok_reprocess').click(function(){
    var datastring={};
    datastring["reprocess_date"]=$("#dcdate").val();
    datastring["task"]=33;
	
    var jsondata = JSON.stringify(datastring);  
    console.log(jsondata);
    $.post("../ajax/reports.php",jsondata,function(data)
      {		
      /*$("#report-content").addClass("card")    
      $("#report-content").html(data);
      reportSuccess();
      */
      })
    .fail(function(jqxhr,status,error)
      {
      alert("Error: "+error);
      });  
});


 $(document).on('click', '#cancel_reprocess', function (){
        $('#reprocess_confirm').modal('hide');
    });

</script>
