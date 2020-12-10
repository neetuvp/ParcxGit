<?php
include('../../includes/header.php');
include('../../includes/navbar-start.php');
?>
</ul>
<div class="header text-dark" id="pdf-report-header">Alerts</div>
<?php
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

<div class="content-wrapper">  
<section class="content">
<div class="container-wide">  		
	<div class="card card-primary mb-4">
		<section class="content">
			<div class="container-wide">
			
				<!--Alert Module -->
				<div class="row">
					<div class="alert-container col-md-6">					
						<span>Type:</span>
						<input type="radio" name="type" value="condition" id="condition" class="inline-form">
						<span class="inline-form-text">Counter Condition</span>
						<input type="radio" name="type" value="TimeInterval" id="TimeInterval" class="inline-form">
						<span class="inline-form-text">Counter Time Interval</span>
						<input type="radio" name="type" value="NetworkStatus" id="NetworkStatus" class="inline-form">
						<span class="inline-form-text">Watchdog Alarm</span>
					</div>
				</div>
				<br>

				<div class="row" id="containerCondition">
					<div class="col-6 form-box">
						<div class="form-group row">
									<label for="" class="col-4 col-form-label">Alert Title</label>
									<div class="col-8">
										<input type="text" id="alert-title"/>
									</div>
								</div>
								<div class="form-group row">
									<label for="" class="col-4 col-form-label">Alert Description</label>
									<div class="col-8">
										<input type="text" id="alert-description"/>
									</div>
								</div>
								<div class="form-group row">
									<label for="" class="col-4 col-form-label">Facility Name:</label>
									<div class="col-8">
										<select  class="form-control" id='facilityNumber'  multiple="multiple">
											<?php echo alertSettings(array("task"=>"1"));?>
										</select>
									</div>
								</div>
								<div class="form-group row">
									<label for="" class="col-4 col-form-label">Carpark Name:</label>
									<div class="col-8">
										<select  class="form-control" id="multiselect"  multiple="multiple">											
										</select>
									</div>
								</div>
						<div id="divOption">
							<div id="divCondition">
								
								
								<div class="form-group row">
									<label for="" class="col-4 col-form-label">Counter Name:</label>
									<div class="col-8">
										<select  class="form-control" id='counter-id'>
										</select>
									</div>
								</div>
																								
								
								<div class="form-group row">
									<label for="inputPassword" class="col-4 col-form-label">Condition:</label>
									<div class="col-8">
										<select type="password" class="form-control" id="conditionList">
											<option value="=">= (Equals)</option>
											<option value="!=">!= (Not equals)</option>
											<option value=">">> (Greater than)</option>
											<option value="<">< (Less than)</option> 
											<option value=">=">>= (Greater than or equals)</option>
											<option value="<="><=( Less than or equals)</option> 
											<option value="between">between</option>
										</select>
									</div>
								</div>

								<div class="form-group row">									
									<label for="" class="col-4 col-form-label">Value:</label>
									<div class="col">
										<input type="text" class="form-control" name="txtvaluefrom" id="value1">
									</div>
									<div class="col" id="value2">
										<input type="text" class="form-control" name="txtvalueto" id="txtvalue2">
									</div>	
									
								</div>
								
																				
							</div>
 							
								
							<div class="form-group row" id="counter-message-div">
									<label for="" class="col-4 col-form-label">Counters in alert:</label>
									<div class="col-8">
										<select  class="form-control" id='counter-message' multiple="multiple">											
										</select>
									</div>
								</div>
								
							<div class="form-group row" id="alarm-div">
								<label for="" class="col-4 col-form-label">Watchdog Alarms</label>
								<div class="col-8">
									<select class="form-control" id='watchdog-alarms' multiple="multiple">
									<option value="111">Chipcoin Reading Error</option>
									<option value="121">Door Open</option>
									<option value="131">Out of chipcoin</option>
									<option value="141">Server unavailable</option>
									<option value="151">LAN Unavailable</option>
									</select>
								</div>
							</div>
								
							<div id="timeControl">															
								<div class="form-group row hour-div">									
									<div class="col-4">
										<input type="radio"    checked="checked">
										<label class="col-form-label">Every Hour:</label>
									</div>
									<div class="col">
										<select class="form-control" id="hour">
											<?php for ($i = 0; $i <= 24; $i++) {echo ("<option value='$i'>$i</option>");}?>
										</select>
									</div>									
								</div>
								
								<div class="form-group row minute-div">									
									<div class="col-4">
										<input type="radio" name="interval">
										<label class="col-form-label">Every Minute:</label>	
									</div>
									<div class="col">
										<select  class="form-control" id="minute">
											<?php 
											for ($i = 0; $i <= 60; $i = $i + 5) 
												{
												echo ("<option value='$i'>$i</option>");
												}
											?>
										</select>										
									</div>									
								</div>
								</div>
										
								<div class="form-group row">									
									<label for="" class="col-4 col-form-label">Start date time:</label>			
									<div class="col">
										<input type="date" class="form-control" id="datePicker" value="<?php echo date('Y-m-d'); ?>"/>
									</div>
									<div class="col">
										<input type="time" class="form-control" value="<?php echo date('H:i');?>"
										id="timePicker"/>
									</div>									
								</div>
								
								

								<br>
								<div>
									<h4>Alert choice</h4>
									<input type="checkbox" name="email" value="email" id="chkEmail" checked="true" class="inline-form">
									<span class="inline-form-text">Email</span>
									<input type="checkbox" name="sms" value="sms" id="chkSms" class="inline-form">
									<span class="inline-form-text">SMS</span>
								</div>
								<br>
								<br>

								<div id="divEmailSms">
									<div id="divEmail">
										<h5>Email Addresses</h5>
										<div class="row">
											<div class="col">
												<input type="text" id="email1" class="form-control" placeholder="name@example.com" />
											</div>
											<div class="col">
												<input type="text" id="email2" class="form-control" placeholder="name@example.com" />
											</div>
										</div>
										<br>
										<div class="row">
											<div class="col">
												<input type="text" id="email3" class="form-control" placeholder="name@example.com" />
											</div>
											<div class="col">
												<input type="text" id="email4" class="form-control" placeholder="name@example.com" />
											</div>
										</div>
										<br>
										<div class="row">
											<div class="col">
												<input type="text" id="email5" class="form-control" placeholder="name@example.com" />
											</div>
											<div class="col">
												<input type="text" id="email6" class="form-control" placeholder="name@example.com" />
											</div>
										</div>
										<br>
										<br>
									</div>
									<div id="divMobile">
										<h5>Mobile Numbers</h5>
										<div class="row">
											<div class="col">
												<input type="text" id="mobile1" class="form-control"
													placeholder="+97150 0000 000" />
											</div>
											<div class="col">
												<input type="text" id="mobile2" class="form-control"
													placeholder="+97150 0000 000" />
											</div>
										</div>
										<br>
										<div class="row">
											<div class="col">
												<input type="text" id="mobile3" class="form-control"
													placeholder="+97150 0000 000" />
											</div>
											<div class="col">
												<input type="text" id="mobile4" class="form-control"
													placeholder="+97150 0000 000" />
											</div>
										</div>
										<br>
										<div class="row">
											<div class="col">
												<input type="text" id="mobile5" class="form-control"
													placeholder="+97150 0000 000" />
											</div>
											<div class="col">
												<input type="text" id="mobile6" class="form-control"
													placeholder="+97150 0000 000" />
											</div>
										</div>
										<br>
									</div>
									<input type="submit" value="Save" id="saveButton" class="btn btn-dark" />
								</div>
							</div>
						</div>
					</div>				
				</div>

				<!-- /.row (main row) -->
			</div>
			<!-- /.container-fluid -->
		</section>
		<!-- /.content -->
	</div>
	<!-- /.content-wrapper -->
	</section>
		<!-- /.content -->
	</div>
	<?php
include '../../includes/footer.php';
?>

<script>
$("#containerCondition").hide();
$("#divTimeInterval").hide();
$("#value2").hide(); 
var type;

//select time interval 
$("#TimeInterval").click(function(){  
	type="time";
	$("#hour-div").show();
	$("#counter-message-div").show();
	$("#minute-div").show();
    $("#containerCondition").show();
	$("#counter-message-div").show();
    $("#divCondition").hide();     
    $("#timeControl").show();    
    $("#divEmail").show();
    $("#divMobile").hide();   
	$("#alarm-div").hide();
    $("#chkEmail:checked").prop("checked",true);
    $("#chkSms:checked").prop("checked",false);     
    });
    
//select condition
$("#condition").click(function(){
	type="condition";
	$("#alarm-div").hide();
	$("#counter-message-div").show();
	$("#hour-div").show();
	$("#minute-div").show();
	$("#counter-message-div").show();
    $("#containerCondition").show();
    $("#divCondition").show();
    $("#timeControl").show();     
    $("#divEmail").show();   
    $("#divMobile").hide();    
    $("#chkEmail:checked").prop("checked",true);
    $("#chkSms:checked").prop("checked",false);     
    });
    
//select network status
$("#NetworkStatus").click(function(){
	type="watchdog";
	$("#counter-message-div").hide();
	$("#alarm-div").show();
	$("#hour-div").hide();
	$("#minute-div").hide();
    $("#containerCondition").show();
	$("#counter-message-div").hide();
    $("#divCondition").hide();     
    $("#timeControl").hide();    
    $("#divEmail").show();
    $("#divMobile").hide();    
    $("#chkEmail:checked").prop("checked",true);
    $("#chkSms:checked").prop("checked",false);   
    });
	
//email checkbox    
$("#chkEmail").on("click",function(){
     $("#divEmail").toggle(this.checked);
    });  
    
//message checkbox    
$("#chkSms").on("click",function(){
     $("#divMobile").toggle(this.checked);
    });	
    
//show textbox 2 for "between   condition"
$("#conditionList").on("change",function(){
    if($("#conditionList").val()=="between")
        {
            $("#value2").show();
        }
        else
        {
          $("#value2").hide();  
        }
    }); 
	
 $("#facilityNumber").change(function() 
	{            
    var data = {};	    
	data['task'] = 2;
	data['facility'] = $("#facilityNumber").val().toString();
	var jsondata = JSON.stringify(data);		
	$.post("../ajax/alert_settings.php",jsondata,function(data)
		{		
		$("#multiselect").html(data);		
		$('#multiselect').multiselect('rebuild');		
		});
    });
	
 $("#multiselect").change(function() 
	{            
    var data = {};	    
	data['task'] = 3;
	data['facility'] = $("#facilityNumber").val().toString();
	data['carpark'] = $("#multiselect").val().toString();
	var jsondata = JSON.stringify(data);		
	$.post("../ajax/alert_settings.php",jsondata,function(data)
		{		
		$("#counter-id").html(data);
		$("#counter-message").html(data);
		$('#counter-message').multiselect('rebuild');
		});
    });
	
$("#saveButton").on("click",function()
	{
	var data = {};	    
	data['task'] = 4;
	data['type']=type;
	data['facility_number'] = $("#facilityNumber").val().toString();
	data['carpark_number'] = $("#multiselect").val().toString();
	data['alert_title'] = $("#alert-title").val();
	data['alert_description'] = $("#alert-description").val();
	data['hour'] = $("#hour").val();
	data['minute'] = $("#minute").val();
	data['start_date_time'] = $("#datePicker").val()+" "+$("#timePicker").val()+":00";
	data['counter_id'] = $("#counter-id").val();	
	data['counter_name'] = $("#counter-id option:selected").html(); 
	//$("#counter-id").text();	
	if($("#conditionList").val()=="between")
		data['counter_condition']=$("value1").val()+" between "+$("#txtvalue2").val();
	else
		data['counter_condition']=$("#conditionList").val()+" "+$("#value1").val();
	data['watchdog_alarm']=$("#watchdog-alarms").val().toString();
	data['counters_in_message'] = $("#counter-message").val().toString();
	data['email']="";
	if($("#email1").val()!="")
		data['email']=$("#email1").val();
	if($("#email2").val()!="")
		data['email']=data['email']+","+$("#email2").val();
	if($("#email3").val()!="")
		data['email']=data['email']+","+$("#email3").val();
	if($("#email4").val()!="")
		data['email']=data['email']+","+$("#email4").val();
	if($("#email5").val()!="")
		data['email']=data['email']+","+$("#email5").val();
	
	data['mobile']="";
	if($("#mobile1").val()!="")
		data['mobile']=$("#mobile1").val();
	if($("#mobile2").val()!="")
		data['mobile']=data['mobile']+","+$("#mobile2").val();
	if($("#mobile3").val()!="")
		data['mobile']=data['mobile']+","+$("#mobile3").val();
	if($("#mobile4").val()!="")
		data['mobile']=data['mobile']+","+$("#mobile4").val();
	if($("#mobile5").val()!="")
		data['mobile']=data['mobile']+","+$("#mobile5").val();
	
	
	var jsondata = JSON.stringify(data);		
	console.log(jsondata);
	$.post("../ajax/alert_settings.php",jsondata,function(data)
		{	
		//if(data==="Success")
			//location.reload();
		});
	});
</script>