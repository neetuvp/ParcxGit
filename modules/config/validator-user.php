<?php 
$page_title="validator Dashboard";

include('../../includes/header.php');
?>

<div class="navbar-has-tablink">

<?php
include('../../includes/navbar-start.php');
?>

</ul>
<div class="header text-dark" id="pdf-report-header">Validators</div>
<div class="row">
  <div class="col tab-header d-flex justify-content-center">
    <div class="tab-link active" data-target="active_validator">Active validator</div>
    <div class="tab-link" data-target="disabled_validator">Disabled validator</div>
     <div class="tab-link" data-target="add-validator" >Add validator</div>
  </div>
</div>

<?php 
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

   

<!-- add validator -->
<div class="content-wrapper block-data" data-status="add-validator" style="display:none;">
  <section class="content">
    <div class="container-wide">
      <div class="row mb-3">
        <div class="col-sm-6">
          <h4 class="text-dark" id="validatortitle">Add validator</h4>
        </div><!-- /.col -->
      </div><!-- /.row -->

      <div class="card card-primary mb-4 col-md-6" id="add-validator-div">
        <div class="card-body">
          <div class="row">            
              <div class="form-group col">

              <form class="block-data " data-status="add-validator" style="display:none;" id="addvalidator"> 
                <div class="row">
                  <div class="col form-group">
                    <label for="">Username(Email Id)</label>
                    <input type="email" class="form-control" id="username" placeholder="" name="Username" required name="name">
                  </div>
                </div>
                <div class="row">
						      <div class="col form-group">
                    <label for="">Password</label>						
                    <input type="password" class="form-control" id="password" placeholder="" required>
							    </div>
                  <div class="col form-group">
							      <label for="">Confirm Password</label>															
								    <input type="password" class="form-control"  placeholder="" name="confirm-password">
							    </div>
					      </div>
						
                <div class="row">
					        <div class="col form-group">
                    <label for="">Account Name</label>
                    <input type="text" class="form-control" id="account-name" aria-describedby="" placeholder="" name="account-name">
                  </div>				
                  <div class="col form-group">
                        <label for="">Mobile</label>
                        <input type="text" class="form-control" id="mobile" placeholder="">
                  </div>                
                </div>
                
                <div class="row">                      										
                  <div class="col form-group">					
                    <label for="">Carpark</label>
                    <select name="multiselect" id="multiselect" class="form-control" multiple="multiple">
                      <?php  
                      //carparkDropdown();                     
                      parcxValidation(array("option-type"=>"5"));?>
                      
                      </select>												
                  </div>
                </div>

            
                <div class="row">					
                  <div class="col form-group">
                    <label for="">Products Assigned (max 10)</label>
                    <select id="validation-product" multiple="multiple">
                    <?php parcxValidation(array("option-type"=>"8"));?>
                    </select>
                  </div>
                  <div class="col form-group">
                    <label for="">Ticket Age (hours)</label>
                    <input type="number" class="form-control" id="validation-ticket-age" placeholder="" value="0" min="1">
                    <small class="form-text text-muted">Maximum parking duration for which validation can be done</small>
                  </div>
                  <div class="col form-group">
                    <label for="">Ticket Max Validation Hours</label>
                    <input type="number" class="form-control" id="validation-max-hours" placeholder="" value="0">
                    <small class="form-text text-muted">Number of hours of validation which can be given out</small>
                  </div> 
							</div>  
              <h5>Max Validation Hours</h5>
							<div class="row">
								<div class="col form-group">
									<label for="">Daily</label>
									<input type="number" class="form-control"  id="validation-daily-hours" placeholder="" value="0">
								</div>
								<div class="col form-group">
									<label for="">Weekly</label>
									<input type="number" class="form-control"  id="validation-weekly-hours" placeholder="" value="0">
								</div>
								<div class="col form-group">
									<label for="">Monthly</label>
									<input type="number" class="form-control"  id="validation-monthly-hours" placeholder="" value="0">
								</div>
							</div> 	
              <div class="row logo-input">               
                  <div class="col form-group">
                      <label for="">Logo</label>  						
                      <input type="file" class="form-control" id="logo" accept="image/JPEG" >
                      <small class="form-text text-muted">Maximum size 50KB ,550*150,Format: JPG/JPEG</small>                      
                  </div>
                    <div class="col form-group">
                      <img id="logo-preview" src="../../validation/dist/img/customer-logo/user.jpg" alt="your image"/>
                    </div>
                </div>
                <div id="date-div">
                  <div class="custom-control custom-checkbox mb-3">
                    <input type="checkbox" class="custom-control-input" id="date-validity">
                    <label class="custom-control-label" for="date-validity">Enable Date Validity</label>
                  </div>
                  <div class="row date-validity d-none">
                    <div class="col form-group">
                      <label for="">Account Activation Date</label>
                      <input type="text" class="form-control" id="account-active-date" autocomplete="off" placeholder="" name="ActivationDate">
                    </div>
                    <div class="col form-group">
                      <label for="">Account Expiry Date</label>
                      <input type="text" class="form-control" id="account-expiry-date" autocomplete="off" placeholder="" name="ExpiryDate">
                    </div>
                  </div>
                </div>  
              <div class="col form-group custom-control custom-checkbox mt-3 mb-3" id="validation-report">
							<input type="checkbox" class="custom-control-input" id="validation-report-view">
							<label class="custom-control-label" for="validation-report-view">Validation Report</label>
						</div>	 
                <input type="submit" class="signUp btn btn-block btn-info mt-2 btn-lg" value="Submit" id="add_validator">
            </form>
                                                   
              </div>            
          </div>
          <!--row-->

        </div>
        <!--card body-->
      </div>
    </div>
  </section>
</div>
<!-- End Add validator -->

<!-- Active validators-->
<div class="content-wrapper block-data" data-status="active_validator">
  <section class="content" id="Activediv">
    <div class="container-wide">
      <div class="row mb-3">
        <div class="col-md-12 d-flex justify-content-between">
          <h4 class="text-dark">Active validators</h4>
          
        </div><!-- /.col -->
      </div><!-- /.row -->

      <div class="card card-primary mb-4" id="active-validators-table">
      <?php echo parcxValidation(array("option-type"=>"2","status"=>"1"));?>
      </div>
    </div>
  </section>  
</div><!-- /.row -->


<!-- Disabled validator-->
<div class="content-wrapper block-data" data-status="disabled_validator" style="display:none;">
  <section class="content">
    <div class="container-wide">
      <div class="row mb-3">
        <div class="col-sm-6">
          <h4 class="text-dark">Disabled validator</h4>
        </div><!-- /.col -->
      </div><!-- /.row -->

      <div class="card card-primary mb-4" id="active-rates-table">        
        <?php echo parcxValidation(array("option-type"=>"2","status"=>"0"));?>
      </div>
    </div>
  </section>
</div><!-- /.row -->
</div>
<?php include('../../includes/footer.php');?>
<!-- password strength -->
<script src="../../validation/dist/js/password-strength/password.js"></script>
<script src="../../validation/dist/js/custom/password.js"></script>
<!-- jquery validate -->
<script src="../../validation/dist/js/custom/licenses-form-validation.js"></script>
<script>
var id;
  /* Table Show - Hide */
  $(document).ready(function () {
   // $('#multiselect').multiselect('refresh');

   $('#validation-product').multiselect({
				buttonWidth: '100%',
				includeSelectAllOption: true,
				selectAllText: "All Products",
				nonSelectedText: 'Select Product',
				selectAllNumber: false,
				allSelectedText: "All Products",
      });	
      
      $('#multiselect').multiselect({
      buttonWidth: '100%',
      includeSelectAllOption: true,
      selectAllText: "All Carparks",
      nonSelectedText: 'Select Carparks',
      selectAllNumber: false,
      allSelectedText: "All Carparks",
    });

    $("#date-validity").change(function() {            
            if ($(this).is(':checked')) {
                $(".date-validity").removeClass("d-none");
            } else {
                $(".date-validity").addClass("d-none");
            }
        });

        $('#account-active-date').daterangepicker({
            timePicker: false,            
            format: 'YYYY-MM-DD',
            singleDatePicker: true          
        });
			
        $('#account-expiry-date').daterangepicker({
            timePicker: false,           
            format: 'YYYY-MM-DD',
            singleDatePicker: true			
        });

    
    $('.tab-link').on('click', function () {
      $("#emirates-div").hide();
      var $target = $(this).data('target');
      if ($target != 'all') {
        $('.block-data').css('display', 'none');
        $('.block-data[data-status="' + $target + '"]').fadeIn('slow');
        if ($target == "add-validator")  
          {  
            $("#addvalidator").trigger("reset");                   
            $('#validatortitle').text("Add Validator");	           
            $('#add_validator').val("Submit");                                 
        }
      
      } else {
        $('.block-data').css('display', 'none').fadeIn('slow');
      }
    });

  
  });

  /* Change Active Tab Styling */

  $('.tab-link').on('click', function () {
    $('.tab-link').removeClass('active');
    $(this).addClass('active');
  });

$(document).on("click", ".btn-enable-disable-validator", function() 
  {		
  var id = $(this).attr('id');
  var status_text=$(this).val();  
  if(status_text==="Disable")
    status=0;
  else
    status=1;
  var data={};
  data["user_id"]=id;
  data["status"]=status;	
  data["option-type"]=4;
  var jsondata = JSON.stringify(data);   
  $.post("../../validation/ajax/validation.php",jsondata,function(data){                           
    location.reload();               
    })
    .fail(function(jqxhr,status,error){
        alert("Error: "+error);

    }); 
});

$(document).on("click", ".btn-edit-validator", function() 
  {		
  id = $(this).attr('id'); 
  id = id.substring(4);     
  var data={};
  data["user_id"]=id;  
  data["option-type"]=6;
  var jsondata = JSON.stringify(data); 
  //console.log(jsondata);    
  $.post("../../validation/ajax/validation.php",jsondata,function(data){ 
    //console.log(data);
    var response=JSON.parse(data);     
    $("#username").val(response.email);
    $("#username").prop("disabled",true);
    $("#account-name").val(response.display_name);
    $("#mobile").val(response.mobile);
    $('#multiselect').val(response.carparks.split(','));
    $('#multiselect').multiselect('refresh');    
    $("#validation-product").val(response.products.split(','));
    $('#validation-product').multiselect('refresh');    
    $("#validation-ticket-age").val(response.ticket_age);
    $("#validation-max-hours").val(response.total_maximum_validation_hours);
    $("#validation-daily-hours").val(response.daily_maximum_validation_hours);
    $("#validation-weekly-hours").val(response.weekly_maximum_validation_hours);
    $("#validation-monthly-hours").val(response.monthly_maximum_validation_hours);
    if(response.date_validity == 1) 
      {
      $("#date-validity").attr("checked",true);      
      $("#account-active-date").val(response.start_date);
      $("#account-expiry-date").val(response.expiry_date);
      }
    else
      $("#date-validity").attr("checked",false);
    $("#date-validity").trigger("change"); 
        
  if(response.validation_report_view == 1) 
      $("#validation-report-view").attr("checked",true);
    else
    $("#validation-report-view").attr("checked",false);
  


    $('#validatortitle').text("Edit Validator");	
    $('#add_validator').val("Edit"); 
    $('.tab-link').removeClass('active');		
		$('.block-data').css('display', 'none');
    $('.block-data[data-status="add-validator"]').fadeIn('slow'); 
        
    
    })
    .fail(function(jqxhr,status,error){
        alert("Error: "+error);

    }); 
});

</script>



