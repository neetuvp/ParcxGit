<?php

$page_title="Application Home";

//# Import application layout.
include('../../../includes/header.php');
include('../../../includes/navbar-start.php');

?>

</ul>

<div class="header text-dark" id="pdf-report-header">Shift Report</div>

<?php

include('../../../includes/navbar-end.php');
include('../../../includes/sidebar.php');

//# App Function Classes
include('../../../classes/reporting_revenue.php');
$reports=new reporting_revenue();

?>

<!-- Modal -->
<div class="modal fade text-dark" id="detailModal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel"
  aria-hidden="true">
  <div class="modal-dialog" role="document">
    <div class="modal-content">
      <div class="modal-body">
        <div class="card">
          <div class="card-header">
            <h3 class="card-title d-inline">Detailed Shift Information</h3>
            <button type="button" class="close text-white" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">×</span></button>
          </div>
          <!-- /.card-header -->
          <div class="card-body p-0">
            <table class="table table-condensed" id="shift-detail-content">
            </table>
          </div>
          <!-- /.card-body -->
        </div>
      </div>
    </div>
  </div>
</div>

<div class="content-wrapper">

  <!-- additional menu -->
  <div class="additional-menu row m-0 bg-white border-bottom">
    <div class="col d-flex pl-1 align-items-center">

      <div class="flex-grow-1 row additional-menu-left">

      <!-- carparks multiselect -->
        <div class="col-md-2">
          <select class="form-control" id="multiselect" multiple="multiple">
          <?php $reports->get_carparks();?>
          </select>
        </div>

        <!-- payment devices multiselect-->
        <div class="col-md-2">
          <select class="form-control" id="deviceNumber" multiple="multiple">
            <?php $reports->get_payment_devices();?>
          </select>
        </div>

        <!-- operator -->
        <div class="col-md-2">
          <select class="form-control" id="operatormultiple"  multiple="multiple">
            <?php $reports->get_operators();?>
          </select>
        </div>
        

        <!-- date and time -->
        <div class="col-md-3">
          <div class="input-group">
            <div class="input-group-prepend">
              <span class="input-group-text"><i class="far fa-clock"></i></span>
            </div>
            <input type="text" class="form-control float-right" id="reservationtime" autocomplete="off" placeholder="Choose Date and Time Range">
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

      <div class="additional-menu-right">
        <div id="action-buttons">
          <div class="btn-group">
            <button type="button" class="btn btn-warning">Export</button>
            <button type="button" class="btn btn-warning dropdown-toggle" data-toggle="dropdown">
              <span class="caret"></span>
              <span class="sr-only">Toggle Dropdown</span>
            </button>
            <div class="dropdown-menu" role="menu">
              <a class="dropdown-item" href="#" id="export_excel_report">Export to Excel</a>

              <a class="dropdown-item" href="#" id="export_pdf_report">
                Export to PDF
              </a>
            </div>
          </div>
        </div>
      </div>

    </div>
  </div>
  <!-- end / additional menu -->

  <section class="content">
    <div class="container-wide">
      <div class="card">
        <div class="card-body p-0" id="shift-report-content">
          <?php                      
          $current_date=date("Y-m-d");    
          $data["from"]=$current_date." ".DAY_CLOSURE_START;
          $data["to"]=$current_date." ".DAY_CLOSURE_END;           
          $data["carpark"]="";    
          $data["device"]="";	
          $data["operator"]="";	                
          $reports->shift_report($data); 
          ?>         
        </div>
      </div>

    </div>

</div>
</section>
</div>

<?php include('../../../includes/footer.php');?>

<script>
$('body').on('click', '#shift_detail', function () 
  {
  var data={};
  data["shift_id"]=$(this).attr('shift_id');
  data["option-type"]=7;   
  var jsondata = JSON.stringify(data);      
  console.log(jsondata);
  $.post("../../ajax/reports-ajax.php",jsondata,function(data)
    {	
    $("#shift-detail-content").html(data);  
    })
  .fail(function(jqxhr,status,error)
    {
    alert("Error: "+error);
    });    

  }); // end click event function 


$('#view-report-button').click(function (event) 
  { 
  if ((!daterange)) 
    {
    alert("choose date range");
    } 
  else 
    {
    var data={};
    data["from"]=from;
    data["to"]=to;         
    data["carpark"]=$("#multiselect").val().toString(); 
    data["device"]=$("#deviceNumber").val().toString();  
    data["operator"]=$("#operatormultiple").val().toString();  
    data["option-type"]=6;   
    var jsondata = JSON.stringify(data);      
    console.log(jsondata);
    $.post("../../ajax/reports-ajax.php",jsondata,function(data)
      {		
      $("#shift-report-content").html(data);
    reportSuccess();      
      })
  .fail(function(jqxhr,status,error)
      {
      alert("Error: "+error);
      }); 
    
  } // end if 

  event.preventDefault();

}); // end traffic report by day 

  /* Excel Export */

  $('#export_excel_report').click(function (event) {
    export_to_excel("#shift-report-content", "PMS_Shift_Report")
  });

</script>