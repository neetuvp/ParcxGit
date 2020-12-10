<?php

$page_title="Application Home";

//# Import application layout.
include('../../../includes/header.php');
include('../../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">Access Movements</div>

<?php
include('../../../includes/navbar-end.php');
include('../../../includes/sidebar.php');
?>
<!-- Modal -->
<div class="modal fade text-dark" id="addToWhiteList" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel" aria-hidden="true">
  <div class="modal-dialog modal-lg" role="document">
    <div class="modal-content">
      <div class="modal-header">
        <h5 class="modal-title">ADD TO WHITELIST</h5>
        <button type="button" class="close" data-dismiss="modal" aria-label="Close">
          <span aria-hidden="true">&times;</span>
        </button>
      </div>
      <div class="modal-body pt-4 pl-4 pr-4">
        <form class="block-data " data-status="add-whitelist"  id="form"> 
        <div class="row">	
					        <div class="col form-group">
                    <label for="">Customer name</label>
                    <input type="text" class="form-control" id="customer_name" placeholder=""  required name="qr">
                  </div>                    
                </div>
                <label>Plate Number</label><h1 id="display_plate"></h1>				
                <div class="row">				 
                  <div class="col form-group">
                      <label for="">Country</label>
                      <!--<input type="text" class="form-control" id="country" style="text-transform:uppercase" placeholder="" required name="country">-->
                      <select id="country">	
                      <option value=''>Select country</option>
                      <option value='UAE'>UAE</option>
                      <option value='BAH'>Bahrain</option>
                      <option value='KUW'>Kuwait</option>
                      <option value='OMN'>Oman</option>
                      <option value='QAT'>Qatar</option>
                      <option value='KSA'>Saudi Arabia</option>                        
                      </select>
                  </div>
                  <div class="col form-group" id="emirates-div">
                      <label for="">Emirates</label>
                      <select id="emirates">									
                      <option value='AUH'>Abu Dhabi</option>
                      <option value='DXB'>Dubai</option>
                      <option value='AJ'>Ajman</option>
                      <option value='SHJ'>Sharjah</option>
                      <option value='RAK'>Ras al Khaimah</option>
                      <option value='FUJ'>Fujairah</option>
                      <option value='UAQ'>Umm al-Quwain</option>
                      </select>
                  </div> 
                  <div class="col form-group">
                    <label for="">Prefix</label>
                    <input type="text" class="form-control" id="prefix" placeholder=""   style="text-transform:uppercase" >
                  </div> 
                  <div class="col form-group">
                    <label for="">Plate number</label>
                    <input type="text" class="form-control" id="plate_number" placeholder=""  >
                  </div> 
				        </div>
       
				        <div class="row d-none">	
				        	<div class="col form-group">
                    <label for="">Qr code number</label>
                    <input type="text" class="form-control" id="qr_code" placeholder="" value='<?php echo date("Ymdhis")?>'  >
                  </div> 

                </div>

                <div class="row">	
					        <div class="col form-group">
                    <label for="">Tag</label>
                    <input type="text" class="form-control" id="tag" placeholder=""  required name="qr">
                  </div> 

                </div>
                                
                <div class="row">
                  <div class="col form-group">
                    <label for="">Activation Date</label>
                    <input type="text" class="form-control" id="whitelist-active-date" autocomplete="off" placeholder="" required name="activation-date">
                  </div>
                  <div class="col form-group">
                    <label for="">Expiry Date</label>
                    <input type="text" class="form-control" id="whitelist-expiry-date" autocomplete="off" placeholder=""required name="expiry-date">
                  </div> 
				        </div>
                              
              
                <div class="form-group">
                  <label for="">Carpark</label>
                  <select name="multiselect" id="multiselect" class="form-control" multiple="multiple">
                  <?php echo parcxSettings(array("task"=>"12"));?>
                  </select>
					      </div>
                <div class="row">	
                  <div class="col form-group">
                    <label for="">Facility Name</label>
                    <select id="facility">                  
                      <?php echo parcxSettings(array("task"=>"5"));?>
                    </select>
                  </div> 
                </div>                           
                <div class="row">	
					        <div class="col form-group">
                    <label for="">Personal Message 1</label>
                    <input type="text" class="form-control" id="message1" placeholder="">
                  </div>                    
                </div>
                <div class="row">	
					        <div class="col form-group">
                    <label for="">Personal Message 2</label>
                    <input type="text" class="form-control" id="message2" placeholder="">
                  </div>                    
                </div>

				        <div class="custom-control custom-checkbox mb-3">
                  <input type="checkbox" class="custom-control-input" id="antipassback">
                  <label class="custom-control-label" for="antipassback">Anti Passback</label>
                </div>
                
                <input type="submit" class="signUp btn btn-block btn-info mt-2 btn-lg" value="Submit" id="add-edit-button">
        </form>
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
      <!-- carparks multiselect -->
      <div class="col-md-2">
          <select class="form-control" id="multiselect_carpark" multiple="multiple">
          <?php echo parcxSettings(array("task"=>"12"));?>
          </select>
        </div>

        <!-- devices multiselect-->
        <div class="col-md-2">
          <select class="form-control" id="deviceNumber" multiple="multiple">
          <?php echo parcxSettings(array("task"=>"14","type"=>"1,2,6,7"));?>
          </select>
        </div>
         <!-- options -->
         <div class="col-md-2">
          <select class="form-control" id="acces_option">
            <option value="">Select Status</option>
            <option value="Access Allowed">Access Allowed</option>
            <option value="Access Denied">Access Denied</option>
            <option value="TAG UNKNOWN">TAG UNKNOWN</option>
          </select>
        </div>

        <!-- plate -->
        <div class="col-md-2">
          <input type="text" id="report_plate_number" class="form-control" placeholder="PLATE/TICKET/TAG">
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
  <input type="hidden" id="user_role" value='<?php echo $_SESSION["userRollName"];?>'>

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
        $data["option"]="";	        
        $data["plate_number"]="";
        $data["wiegand"]="1"; 
        $data["user_role"]=$_SESSION["userRollName"];
        $data["task"]=4;                          
        echo parcxReport($data);
        ?>  
        </div> 
      </div>   
    </div>
  </section>
</div>
<script src="../../../dist/js/whitelistsettings.js"></script>
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
    data["plate_number"]=$("#report_plate_number").val(); 
    data["option"]=$("#acces_option").val();
    data["user_role"]=$("#user_role").val();
    data["wiegand"]="1"; 
    data["task"]=4;
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
    export_to_excel("#report-content", "PMS_Access_Report")

  }); // end click event function


</script>