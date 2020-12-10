<?php

$page_title="Application";

//# Import application layout.
include('../../../includes/header.php');
include('../../../includes/navbar-start.php');

?>

</ul>

<div class="header text-dark" id="pdf-report-header">Validation Report</div>

<?php

include('../../../includes/navbar-end.php');
include('../../../includes/sidebar.php');
?>



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

        <!-- Validators -->
        <div class="col-md-2">
          <select class="form-control" id="validator" multiple="multiple">          
          <?php echo parcxSettings(array("task"=>"20"));?>
          </select>
        </div>

        <!-- Validators -->
        <div class="col-md-2">
          <select class="form-control" id="validation_product" multiple="multiple">          
          <?php echo parcxSettings(array("task"=>"21"));?>
          </select>
        </div>

        <!-- Ticket number -->
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
        $data["validator"]="";	        
        $data["product"]="";	        
        $data["plate_number"]=""; 
        $data["task"]=3;                          
        echo parcxReport($data);
        ?>  
        </div> 
      </div>   
    </div>
  </section>
</div>

<?php include('../../../includes/footer.php'); ?>

<script>
$(document).ready(function () 
  {
  $('#validator').multiselect({
        buttonWidth: '100%',
        includeSelectAllOption: true,
        selectAllText: "All Validators",
        nonSelectedText: 'Select Validator',
        selectAllNumber: false,
        allSelectedText: "All Validators",
      });

    $('#validation_product').multiselect({
      buttonWidth: '100%',
      includeSelectAllOption: true,
      selectAllText: "All Products",
      nonSelectedText: 'Select Product',
      selectAllNumber: false,
      allSelectedText: "All Products",
    });
  });


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
	  data["validator"]=$("#validator").val().toString();	
    data["product"]=$("#validation_product").val().toString();	
    data["plate_number"]=$("#plate_number").val(); 
    data["task"]=3;
    var temp = JSON.stringify(data);    
    console.log(temp);  
    $.post("../../ajax/reports.php", temp)
      .done(function (result) {        
        loadReport(result);    

      }, "json");
    } // end if 

    event.preventDefault();

  });  

  $('#export_excel_report').click(function (event) {

    export_to_excel("#report-content", "PMS_Validation_Report")

  }); // end click event function
</script>