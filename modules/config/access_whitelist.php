<?php 
$page_title="Device Settings";

include('../../includes/header.php');
?>

<div class="navbar-has-tablink">

<?php
include('../../includes/navbar-start.php');
?>


</ul>
<div class="header text-dark" id="pdf-report-header">Access Whitelist</div>
<div class="row hidden-sm-down">
    <div class="col tab-header d-flex justify-content-center">
        <div class="tab-link active" data-target="overview">Overview</div>
        <div class="tab-link" data-target="form">Add Whitelist</div>        
        <div class="tab-link" data-target="synch">Synch Whitelist</div>        
    </div>
</div>

<?php 
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>
<!-- pdf loader modal -->
<div class="modal" id="whitelist-modal" tabindex="-1" role="dialog" aria-labelledby="" aria-hidden="true">
  <div class="modal-dialog" role="document">
    <div class="modal-content">
      <div class="modal-body p-4 text-center">
        <img src="../../dist/img/loading.gif" class="mb-3">
        <h5 class="mb-2" id="modal-header">Synching whitelist</h5>
      </div>
    </div>
  </div>
</div>

<div class="content-wrapper">
    <section class="content">
        <div class="container-wide">  
        <div id="alert-div" class="alert alert-dismissible bg-danger">
          <button type="button" class="close" data-dismiss="alert" aria-hidden="true">×</button>
          <h5><i class="icon fa fa-check"></i> Alert!</h5>
          <div id="alert-message">Error while loading the page.Please refresh the page and try again.</div>
        </div>              
          <!-- add/update form --> 
              <form class="block-data card card-body col-md-6" data-status="form" style="display:none;" id="form"> 		            
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
                    <input type="text" class="form-control" id="prefix" placeholder=""   style="text-transform:uppercase">
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

            <!-- table -->
            <div class="block-data" data-status="overview">
              <div class="card" >               
                  <div class="card-body">     
                    <table id="RecordsTable" class="table table-blue table-bordered table-striped">                    
                    <?php echo parcxWhitelistSettings(array("task"=>"1"));?>
                    </table>
                  </div>                                                  
              </div>             
            </div>  
            <div class="block-data" data-status="synch" style="display:none;">            
            <input class='btn btn-warning mb-3' type='submit' value='Synch Whitelist to all devices' id="btnSynchWhitelist">             
              <div class="card" >               
                  <div class="card-body">     
                    <table id="RecordsTable2" class="table table-blue table-bordered table-striped">                    
                    <?php echo parcxWhitelistSettings(array("task"=>"6"));?>
                    </table>
                  </div>                                                  
              </div>              
            </div>  

        </div>        
    </section>    
</div>
<script src="../../dist/js/whitelistsettings.js"></script>
<?php include('../../includes/footer.php');?>
