<?php 
$page_title="Device Settings";

include('../../includes/header.php');
?>

<div class="navbar-has-tablink">

<?php
include('../../includes/navbar-start.php');
?>


</ul>
<div class="header text-dark" id="pdf-report-header">Device Settings</div>
<div class="row hidden-sm-down">
    <div class="col tab-header d-flex justify-content-center">
        <div class="tab-link active" data-target="overview">Overview</div>
        <div class="tab-link" data-target="form">Add device</div>
    </div>
</div>

<?php 
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

<div class="content-wrapper">
    <section class="content">
        <div class="container-wide">        
          <!-- add/update carpark form --> 
          <form class="block-data card card-body col-md-6" data-status="form" style="display:none;" id="form"> 
            <input type="hidden" id="user_id" value="<?php echo $_SESSION['userId'];?>" />
              <div class="row">
                  <div class="col form-group">
                    <label for="">Device Category</label>
                    <select name = "device_category" id ="device_category" class="form-control">
                      <option value = "1" selected>Entry</option>
                      <option value = "2">Exit</option>
                      <option value = "3">Cashier</option>
                      <option value = "4">Auto Payment Machine</option>
                      <option value = "5">Handheld POS</option>
                      <!-- <option value="6">Entry Controller</option>
                      <option value="7">Exit Controller</option> -->
                      <option value="8">Camera</option>
                      <!-- <option value="9">Valet Handheld</option>
                      <option value="10">Key/Podium Manager</option> -->
                    </select>
                  </div>
              </div>
              <div class="row">
                  <div class="col form-group">
                    <label for="">Device name</label>
                    <input type="text" class="form-control" id="device_name" placeholder=""  required name="device_name">
                  </div>
              </div>				                                                  
              <div class="row">	
					      <div class="col form-group">
                  <label for="">Device number</label>
                  <input type="number" class="form-control" id="device_number" value="0" min="0" required name="device_number">
                  </div> 
                  <div class="col form-group">
                  <label>Device IP</label>                  
                  <input type="text" class="form-control" id="device_ip" required name="device_ip">                  
                  </div> 
              </div>                             
              <div class="row">	
					      <div class="col form-group">
                  <label for="">Carpark Name</label>
                  <select id="carpark">                  
                    <?php echo parcxSettings(array("task"=>"12"));?>
                  </select>
                </div> 
                <div class="col form-group">
                  <label for="">Facility Name</label>
                  <select id="facility">                  
                    <?php echo parcxSettings(array("task"=>"5"));?>
                  </select>
                </div> 
              </div> 
              <div class="row div-camera div-device">	
					      <div class="col form-group">
                  <label for="">Camera Index</label>
                  <select id="camera_index">  
                  <option value="0">Select camera index</option>                
                    <?php
                    for($i=1;$i<=10;$i++)
                      echo "<option value='".$i."'>Camera ".$i."</option>";
                    ?>
                  </select>
                </div> 
              </div>               
              <div class="div-rate div-device">
              <div class="row">	
                <div class="col form-group">
                  <label for="">Rate Type</label>
                  <select id="rate_type"> 
                    <option value="1">Variable Rate</option>
                    <option value="2">Fixed Rate</option>     
                  </select>
                </div> 
					      <div class="col form-group">
                  <label for="">Rate Plan</label>
                  <select id="rate_plan">                  
                    <?php echo parcxSettings(array("task"=>"13"));?>
                  </select>
                </div>                 
              </div> 
              <div class="row">						     
                <div class="col form-group">
                  <label for="">Reservation Rate Plan</label>
                  <select id="reservation_rate_plan"> 
                    <option value="">Select Reservation Rate Plan</option>                 
                    <?php echo parcxSettings(array("task"=>"13"));?>
                  </select>
                </div> 
                <div class="col form-group">
                    <label for="">Server handshake interval</label>
                    <input type="number" class="form-control" id="server_handshake_interval" value="90"  required >                    
                  </div>
              </div>
              </div>
              <div class="div-terminal div-device">
                <div class="row ">						     
                  <div class="col form-group">
                    <label for="">Camera</label>
                    <select id="camera_id"> 
                      <option value="0">Select Camera</option>                 
                      <?php echo parcxSettings(array("task"=>"14","type"=>"8"));?>
                    </select>
                  </div> 
                  <div class="col form-group div-device div-entry">
                    <label for="">Plate capturing wait delay</label>
                    <input type="number" class="form-control" id="plate_capturing_wait_delay" value="7"  required >
                    <small class="form-text text-muted">Wait to get plate number in seconds for entry ticket</small>
                  </div>   

                </div> 

                <div class="row ">						     
                  <div class="col form-group">
                    <label >Device function</label>
                    <select id="device_function"> 
                      <option value="1">QR Code</option>
                      <option value="2">Chipcoin</option>
                      <option value="3">Ticketless</option>
                    </select>
                  </div> 

                  <div class="col form-group">
                    <label >Barrier open status type</label>
                    <select id="barrier_open_status_type"> 
                      <option value="0">NO Barrier open</option>
                      <option value="1">NC Barrier Open</option>                      
                    </select>
                  </div> 

                </div> 

                <div class="row">
                  <div class="col form-group">
                    <label for="">Barrier open time limit</label>
                    <input type="number" class="form-control" id="barrier_open_time_limit" value="10"  required >
                    <small class="form-text text-muted">Time limit in seconds for barrier to stay open </small>
                  </div>
                  <div class="col form-group">
                    <label for="">Barrier open hold duration</label>
                    <input type="number" class="form-control" id="duration_hold_barrier_open" value="100"  required >
                    <small class="form-text text-muted">Wait to open barrier for ticketless entry in milliseconds</small>
                  </div>
                </div>
                        
                <div class="row">	
                  <div class="col form-group custom-control custom-checkbox mt-3 mb-3">
                    <input type="checkbox" class="custom-control-input" id="wiegand_enabled">
                    <label class="custom-control-label" for="wiegand_enabled">Wiegand Enabled</label>
                  </div> 
                  <div class="col form-group custom-control custom-checkbox mt-3 mb-3">
                    <input type="checkbox" class="custom-control-input" id="wiegand2_enabled">
                    <label class="custom-control-label" for="wiegand2_enabled">Wiegand2 Enabled</label>
                  </div>                               
                </div> 

                <div class="row">	
                  <div class="col form-group custom-control custom-checkbox mt-3 mb-3">
                    <input type="checkbox" class="custom-control-input" id="barrier_status_monitoring">
                    <label class="custom-control-label" for="barrier_status_monitoring">Barrier status monitoring</label>
                  </div> 
                  <div class="col form-group custom-control custom-checkbox mt-3 mb-3">
                    <input type="checkbox" class="custom-control-input" id="bms_status_enabled">
                    <label class="custom-control-label" for="bms_status_enabled">BMS status Enabled</label>
                  </div>                               
                </div> 

                <div class="row">	
                  <div class="col form-group custom-control custom-checkbox mt-3 mb-3">
                    <input type="checkbox" class="custom-control-input" id="quick_barrier_close">
                    <label class="custom-control-label" for="quick_barrier_close">Quick barrier close</label>
                  </div> 
                  <div class="col form-group custom-control custom-checkbox mt-3 mb-3 div-device div-exit">
                    <input type="checkbox" class="custom-control-input" id="payment_enabled_exit">
                    <label class="custom-control-label" for="payment_enabled_exit">Payment Enabled exit</label>
                  </div>                                                 
                </div> 
                

                
              </div>
              <div class="div-device div-cashier">
                <div class="row">	
                  <div class="col form-group custom-control custom-checkbox mt-3 mb-3">
                    <input type="checkbox" class="custom-control-input" id="customer_receipt_mandatory">
                    <label class="custom-control-label" for="customer_receipt_mandatory">Customer Receipt Mandatory</label>
                  </div>     
                  <div class="col form-group custom-control custom-checkbox mt-3 mb-3" >
                    <input type="checkbox" class="custom-control-input" id="shift_receipt_mandatory">
                    <label class="custom-control-label" for="shift_receipt_mandatory">Shift Receipt Mandatory</label>
                  </div>                            
                </div> 
               
                <div class="row">	
                  <div class="col form-group custom-control custom-checkbox mt-3 mb-3">
                    <input type="checkbox" class="custom-control-input" id="physical_cash_count">
                    <label class="custom-control-label" for="physical_cash_count">Shift Physical Cash Count Required</label>
                  </div> 
                  <div class="col form-group custom-control custom-checkbox mt-3 mb-3">
                    <input type="checkbox" class="custom-control-input" id="issue_lost">
                    <label class="custom-control-label" for="issue_lost">Issue lost</label>
                  </div>                                
                </div>             
              </div>
              <div class="div-rate div-device">
                

                <div class="row">	
                  <div class="col form-group custom-control custom-checkbox mt-3 mb-3">
                    <input type="checkbox" class="custom-control-input" id="anpr_enabled">
                    <label class="custom-control-label" for="anpr_enabled">ANPR Enabled</label>
                  </div> 
                  <div class="col form-group custom-control custom-checkbox mt-3 mb-3">
                    <input type="checkbox" class="custom-control-input" id="display_anpr_image">
                    <label class="custom-control-label" for="display_anpr_image">Display ANPR Image</label>
                  </div>                                    
                </div> 
               

                <div class="row">	
                  <div class="col form-group custom-control custom-checkbox mt-3 mb-3">
                    <input type="checkbox" class="custom-control-input" id="access_enabled">
                    <label class="custom-control-label" for="access_enabled">Access Enabled</label>
                  </div>   
                  <div class="col form-group custom-control custom-checkbox mt-3 mb-3">
                    <input type="checkbox" class="custom-control-input" id="reservation_enabled">
                    <label class="custom-control-label" for="reservation_enabled">Reservation Enabled</label>
                  </div>                              
                </div> 
                               

                <div class="row">	
                  <div class="col form-group custom-control custom-checkbox mt-3 mb-3">
                    <input type="checkbox" class="custom-control-input" id="synch_whitelist">
                    <label class="custom-control-label" for="synch_whitelist">Synch access whitelist</label>
                  </div>   

                  <div class="col form-group custom-control custom-checkbox mt-3 mb-3">
                  <input type="checkbox" class="custom-control-input" id="review_mode">
                  <label class="custom-control-label" for="review_mode">Review mode</label>
                </div> 

                </div>                 

              </div> 
            

              <input type="submit" class="signUp btn btn-block btn-info mt-2 btn-lg" value="Submit" id="add-edit-button">
            </form>

            <!-- carpark table -->
            <div class="block-data" data-status="overview">
              <div class="card" >               
                  <div class="card-body">     
                    <table id="RecordsTable" class="table table-blue table-bordered table-striped">                    
                    <?php echo parcxSettings(array("task"=>"10"));?>
                    </table>
                  </div>                                                  
              </div>             
            </div>           

        </div>
    </section>
</div>
<?php include('../../includes/footer.php');?>
<script>
var status;   
var id;
var previous_carpark;
var previous_facility;
var previous_device;
$('#div-cashier').hide();
$('.div-camera').hide();
$("#device_category").change(function()
  {
  var cat=$("#device_category").val();
    $(".div-device").hide();
    if(cat==1||cat==2)
      {                   
      $('.div-terminal').show(); 
      $('.div-rate').show();   
      if(cat==1)
        {
        $('.div-exit').hide(); 
        $('.div-entry').show();  
        }
      else
        {
        $('.div-entry').hide();  
        $('.div-exit').show();  
        }
      }
    else if(cat==3||cat==4||cat==5)
      {        
        $('.div-cashier').show(); 
      $('.div-rate').show();      
      }
    else if(cat==8)
      {      
      $('.div-camera').show();
      }   	
  });

$(document).ready(function () 
  {
    loadDataTable() ;   
  $("#device_category").change();     
  $("* [data-target]").on('click', function () 
    {    
    var $target = $(this).data('target'); 
    if($target=="form")                        
      {
      $("#form").trigger('reset');
      $("#add-edit-button").val("Submit");    
      $("#device_category").change();   
      } 
    $('.block-data').css('display', 'none');
    $('.block-data[data-status="' + $target + '"]').fadeIn('slow');        
   $('.tab-link').removeClass('active');
    $(this).addClass('active');  
  });

 

  //FormSubmit
  var formElement = $("#form");
    var rules_set = {};

    formElement.find('input[type=text]').each(function()
        {
        var name = $(this).attr('name');
        rules_set[name] = 'required';
        });

    formElement.validate({
        rules: rules_set,
        errorElement: "div",
        errorPlacement: function (error, element) {
            error.addClass("text-danger");
            error.insertAfter(element);
        },
        submitHandler: function () 
            {	                                                             
            var data = {};
            if($("#add-edit-button").val()=="Submit")
              {
              data["id"]=""; 
              data["previous_carpark"]=$("#carpark").val();
              data["previous_facility"]=$("#facility").val();
              data["previous_device"]=$("#device_number").val();                     
              }
            else
              {
              data["id"]=id;
              data["previous_carpark"]=previous_carpark;
              data["previous_facility"]=previous_facility;
              data["previous_device"]=previous_device;
              }
            data["device_category"]=$("#device_category").val();
            data["device_category_name"]=$("#device_category option:selected").text();
            data["carpark_number"]=$("#carpark").val();
            data["carpark_name"]=$("#carpark option:selected").text(); 
            data["facility_number"]=$("#facility").val(); 
            data["facility_name"]=$("#facility option:selected").text();
            data["device_number"]=$("#device_number").val(); 
            data["device_name"]=$("#device_name").val(); 
            data["device_ip"]=$("#device_ip").val(); 
            data["rate_type"]=$("#rate_type").val(); 
            data["rate_plan"]=$("#rate_plan").val(); 
            data["camera_id"]=$("#camera_id").val(); 
            data["camera_index"]=$("#camera_index").val(); 
            data["reservation_rate_plan"]=$("#reservation_rate_plan").val(); 

            data["server_handshake_interval"]=$("#server_handshake_interval").val(); 
            data["device_function"]=$("#device_function").val(); 
            data["barrier_open_time_limit"]=$("#barrier_open_time_limit").val(); 
            data["duration_hold_barrier_open"]=$("#duration_hold_barrier_open").val(); 
            data["plate_capturing_wait_delay"]=$("#plate_capturing_wait_delay").val(); 
            data["barrier_open_status_type"]=$("#barrier_open_status_type").val(); 

            if ($('#quick_barrier_close').is(":checked")) 
              data["quick_barrier_close"]="1";
            else
              data["quick_barrier_close"]="0";

            if ($('#payment_enabled_exit').is(":checked")) 
              data["payment_enabled_exit"]="1";
            else
              data["payment_enabled_exit"]="0";

            if ($('#anpr_enabled').is(":checked")) 
              data["anpr_enabled"]="1";
            else
              data["anpr_enabled"]="0";
            
            if ($('#wiegand_enabled').is(":checked")) 
              data["wiegand_enabled"]="1";
            else
              data["wiegand_enabled"]="0";

            if ($('#wiegand2_enabled').is(":checked")) 
              data["wiegand2_enabled"]="1";
            else
              data["wiegand2_enabled"]="0";

            if ($('#access_enabled').is(":checked")) 
              data["access_enabled"]="1";
            else
              data["access_enabled"]="0";

            if ($('#reservation_enabled').is(":checked")) 
              data["reservation_enabled"]="1";
            else
              data["reservation_enabled"]="0";

            if ($('#review_mode').is(":checked")) 
              data["review_mode"]="1";
            else
              data["review_mode"]="0";

            if ($('#display_anpr_image').is(":checked")) 
              data["display_anpr_image"]="1";
            else
              data["display_anpr_image"]="0";

              if ($('#bms_status_enabled').is(":checked")) 
              data["bms_status_enabled"]="1";
            else
              data["bms_status_enabled"]="0";

              if ($('#barrier_status_monitoring').is(":checked")) 
              data["barrier_status_monitoring"]="1";
            else
              data["barrier_status_monitoring"]="0";



            if ($('#synch_whitelist').is(":checked")) 
              data["synch_whitelist"]="1";
            else
              data["synch_whitelist"]="0";

            if ($('#customer_receipt_mandatory').is(":checked")) 
              data["customer_receipt_mandatory"]="1";
            else
              data["customer_receipt_mandatory"]="0";

            if ($('#shift_receipt_mandatory').is(":checked")) 
              data["shift_receipt_mandatory"]="1";
            else
              data["shift_receipt_mandatory"]="0";

            if ($('#physical_cash_count').is(":checked")) 
              data["physical_cash_count"]="1";
            else
              data["physical_cash_count"]="0";

            if ($('#issue_lost').is(":checked")) 
              data["issue_lost"]="1";
            else
              data["issue_lost"]="0";

            data["user_id"]=$("#user_id").val(); 
            data["task"]="15";            
            var jsondata=JSON.stringify(data);  
            console.log(jsondata);                   
            $.post("../../modules/ajax/settings.php",jsondata,function(result){ 
              if(result=="Successfull")
                location.reload();                  
              else
                alert(result);
           })
           .fail(function(jqxhr,status,error){
               alert("Error: "+error);

           });                              
        }
    });

});

/* === enable disable product === */
var status;   
var id;
$(document).on("click", ".device-enable-disable-btn", function() 
    {
    id = $(this).parent('td').parent('tr').data('id');
    var status_text=$(this).text();           
    if(status_text=="Disable")
        status=0;
    else
        status=1;

    var data = {};
    data["id"]=id;
    data["status"]=status;   
    data["task"]="11";            
    var jsondata=JSON.stringify(data);            
    $.post("../../modules/ajax/settings.php",jsondata,function(result){ 
      if(result=="Successfull")
        location.reload();                  
      else
        alert(result);
    })
    .fail(function(jqxhr,status,error){
        alert("Error: "+error);

    }); 
});

/*=====edit======*/
$(document).on("click", ".device-edit", function() 
    {
    id = $(this).parent('td').parent('tr').data('id');    
    var data = {};
    data["id"]=id;      
    data["task"]="16";            
    var jsondata=JSON.stringify(data);            
    $.post("../../modules/ajax/settings.php",jsondata,function(result){       
      $("#form").trigger('reset');
      $(":checkbox").attr("checked", false);
      var response=JSON.parse(result);            
      $("#device_category").val(response.device_category);
      $("#device_category").change();
      $("#device_name").val(response.device_name);
      $("#device_number").val(response.device_number);
      $("#device_ip").val(response.device_ip);
      $("#facility").val(response.facility_number);
      $("#multiselect").val(response.carpark_number);      
      $("#rate_type").val(response.rate_type);
      $("#rate_plan").val(response.rate_plan);
      $("#reservation_rate_plan").val(response.reservation_rate_plan);  
      $("#camera_id").val(response.camera_id);   
      $("#camera_index").val(response.camera_index);
      $("#server_handshake_interval").val(response.server_handshake_interval);
      $("#plate_capturing_wait_delay").val(response.plate_capturing_wait_delay);
      $("#device_function").val(response.device_function);
      $("#barrier_open_status_type").val(response.barrier_open_status_type);   
      $("#barrier_open_time_limit").val(response.barrier_open_time_limit);   
      $("#duration_hold_barrier_open").val(response.duration_hold_barrier_open);   

      if(response.customer_receipt_mandatory==1)
        $('#customer_receipt_mandatory').attr("checked","checked");      
      if(response.shift_receipt_mandatory ==1)
        $('#shift_receipt_mandatory').attr("checked","checked");      
      if(response.shift_physical_cash_count_required==1)
        $('#physical_cash_count').attr("checked","checked");      
      if(response.synch_whitelist_flag ==1)
        $('#synch_whitelist').attr("checked","checked");      
      if(response.issue_lost==1)
        $('#issue_lost').attr("checked","checked");   

      if(response.wiegand_enabled==1)
        $('#wiegand_enabled').attr("checked","checked");   
      if(response.wiegand2_enabled==1)
        $('#wiegand2_enabled').attr("checked","checked");   
      if(response.barrier_status_monitoring==1)
        $('#barrier_status_monitoring').attr("checked","checked");   
      if(response.quick_barrier_close==1)
        $('#quick_barrier_close').attr("checked","checked");   
      if(response.payment_enabled_exit==1)
        $('#payment_enabled_exit').attr("checked","checked");   
      if(response.bms_status_enabled==1)
        $('#bms_status_enabled').attr("checked","checked");   
      if(response.anpr_enabled==1)
        $('#anpr_enabled').attr("checked","checked");      
      if(response.display_anpr_image==1)
        $('#display_anpr_image').attr("checked","checked");   
      if(response.access_enabled==1)
        $('#access_enabled').attr("checked","checked");   
      if(response.reservation_enabled==1)
        $('#reservation_enabled').attr("checked","checked");   
      if(response.synch_whitelist==1)
        $('#synch_whitelist').attr("checked","checked");   
      if(response.review_mode==1)
        $('#review_mode').attr("checked","checked");   
        
      $("#add-edit-button").val("Edit");  
      previous_carpark=response.carpark_number;
      previous_facility=response.facility_number;
      previous_device=response.device_number;
    })
    .fail(function(jqxhr,status,error){
        alert("Error: "+error);

    }); 
});

</script>
