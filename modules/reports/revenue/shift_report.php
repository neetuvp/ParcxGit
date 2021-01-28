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
//include('../../../classes/reporting_revenue.php');
//$reports=new reporting_revenue();

?>

<!-- Modal -->
<div class="modal fade text-dark" id="detailModal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel"
  aria-hidden="true">
  <div class="modal-dialog" role="document">
    <div class="modal-content">
      <div class="modal-body">
        <div class="card">
          <div class="card-header">
            <h3 class="card-title d-inline" id ="modal_title">Detailed Shift Information</h3>
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
          <?php echo parcxSettings(array("task"=>"12"));?>
          </select>
        </div>

        <!-- payment devices multiselect-->
        <div class="col-md-2">
          <select class="form-control" id="deviceNumber" multiple="multiple">
            <?php echo parcxSettings(array("task"=>"37"));?>
          </select>
        </div>

        <!-- operator -->
        <div class="col-md-2">
          <select class="form-control" id="operatormultiple"  multiple="multiple">
            <?php echo parcxSettings(array("task"=>"36"));?>
          </select>
        </div>
        

        <!-- date and time -->
        <div class="col-md-3">
          <div class="input-group">
            <div class="input-group-prepend">
              <span class="input-group-text"><i class="far fa-clock"></i></span>
            </div>
            <input type="text" class="form-control float-right" id="reservationtime" autocomplete="off" placeholder="<?php echo parcxReport(array("task"=>"13","language"=>$_SESSION["language"],"label"=>"choose_datetime_range"));?>">
          </div>
        </div>

        <!-- search -->
        <div class="col-md-1">
        <button type="button" class="btn  btn-secondary" id="view-report-button" onclick ="shift_report()">View Report</button>
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
        <div class="card-body" id="report-content">
          <?php                      
          $current_date=date("Y-m-d");    
          $data["from"]=$current_date." ".DAY_CLOSURE_START;
          $data["to"]=$current_date." ".DAY_CLOSURE_END;           
          $data["carpark"]="";    
          $data["device"]="";	
          $data["operator"]="";	  
		  $data["language"] = $_SESSION["language"];
		  $data["task"]=15; 
          echo parcxReport($data); 
          ?>         
        </div>
      </div>

    </div>

</div>
</section>
</div>

<?php include('../../../includes/footer.php');?>

<script>
var load_report = 0;

$('body').on('click', '#shift_detail', function () 
  {
  var data={};
  data["shift_id"]=$(this).attr('shift_id');
  data["task"]=16;   
  data["language"] = $("#language").val();
  var jsondata = JSON.stringify(data);      
  console.log(jsondata);
  $.post("../../ajax/reports.php",jsondata,function(data)
    {	
    $("#shift-detail-content").html(data); 
		load_report=1;
    })
  .fail(function(jqxhr,status,error)
    {
    alert("Error: "+error);
    }); 

var heading = {};
	heading["task"]=13;
	heading["language"]=$("#language").val(); 
	heading["label"]="detailed_shift";
	jsondata = JSON.stringify(heading);    
	$.post("../../ajax/reports.php",jsondata,function(data)
	  {		
	$("#modal_title").html(data);
  })
.fail(function(jqxhr,status,error)
	  {
  alert("Error: "+error);
  });

	

  }); // end click event function 


//$('#view-report-button').click(function (event) 
 function shift_report()
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
	data["language"] = $("#language").val();
    data["task"]=15;   
    var jsondata = JSON.stringify(data);      
    console.log(jsondata);
    $.post("../../ajax/reports.php",jsondata,function(data)
      {		
          load_report=1;
      $("#report-content").html(data);
    reportSuccess();      
      })
  .fail(function(jqxhr,status,error)
      {
      alert("Error: "+error);
      }); 
    
  } // end if 

  event.preventDefault();
  }
//}); // end traffic report by day 


function loadPage()
  {
  loadheadingreport("shift_report");
  if(load_report==1)
	shift_report(); 
  }
$("#language").change(function(){
  loadPage();
}); 

$( document ).ready(function() {
	loadheadingreport("shift_report");
});

  /* Excel Export */

  $('#export_excel_report').click(function (event) {
    export_to_excel("#report-content", "PMS_Shift_Report")
  });

</script>