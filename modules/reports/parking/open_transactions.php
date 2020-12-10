<?php

$page_title="Application Home";

//# Import application layout.
include('../../../includes/header.php');
include('../../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">Open Transactions</div>

<?php
include('../../../includes/navbar-end.php');
include('../../../includes/sidebar.php');

//# App Function Classes

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
        <!-- carparks multiselect -->
        <div class="col-md-2">
          <select class="form-control" id="multiselect" multiple="multiple">
          <?php echo parcxSettings(array("task"=>"12"));?>
          </select>
        </div>

        <!-- payment devices multiselect-->
        <div class="col-md-2">
          <select class="form-control" id="deviceNumber" multiple="multiple">
          <?php echo parcxSettings(array("task"=>"14","type"=>"1"));?>
          </select>
        </div>
        <!--parking duration-->
        <div class="col-md-2">
          <select class="form-control" id="parking_duration">          
          <option value="">Select Parking Duration</option>
          <option value="0">Less than 1 Day</option>
          <option value="1">Greater than 1 Day</option>
          <option value="2">Greater than 2 Day</option>
          <option value="3">Greater than 3 Day</option>
          <option value="4">Greater than 4 Day</option>
          <option value="5">Greater than 5 Day</option>
          <option value="6">Greater than 6 Day</option>
          <option value="7">Greater than 7 Day</option>          
          </select>
        </div>


        <!-- plate -->
        <div class="col-md-2">
          <input type="text" id="plate_number" class="form-control" placeholder="PLATE NUMBER/TICKET ID">
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
        <div class="card-body" id="report-content">
        <?php
        $current_date=date("Y-m-d");    
        $data["from"]=$current_date." ".DAY_CLOSURE_START;
        $data["to"]=$current_date." ".DAY_CLOSURE_END;           
        $data["carpark"]="";    
        $data["device"]="";	        
        $data["plate_number"]="";
        $data["parking_duration"]=""; 
        $data["task"]=1;                          
        echo parcxReport($data);
        ?>   
        </div>
      </div>   
</div>
</section>
</div>

<?php include('../../../includes/footer.php');?>

<script>
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
    data["plate_number"]=$("#plate_number").val(); 
    data["parking_duration"]=$("#parking_duration").val(); 
    data["task"]=1;
    var temp = JSON.stringify(data);       
    $.post("../../ajax/reports.php", temp)
      .done(function (result) {        
        loadReport(result);

      }, "json");
    } // end if 

    event.preventDefault();

  }); 

$('#export_excel_report').click(function (event) 
  {
  export_to_excel("#page-content", "PMS_Open_Transaction_Report")
  });


$('body').on('click', "[data-target='#image-modal']", function () 
  {
  var name = $(this).data('value');      
  var image="<img src='http://localhost/ANPR/Images/Scene/"+name+"' width='1000' height='700'>";    
  $("#image-content-modal").html(image);      
  });

</script>