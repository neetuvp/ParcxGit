<?php 
$page_title="Device Dashboard";

include('../../includes/header.php');
?>

<div class="navbar-has-tablink">

<?php
include('../../includes/navbar-start.php');
?>

</ul>
<div class="header text-dark" id="pdf-report-header">ANPR Settings</div>

<?php 
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

<div class="content-wrapper block-data d-none" id="edit-anpr-form" >
  <section class="content">
    <div class="container-wide">      
		<div class="card card-primary mb-4">
			<div class="card-body">	
				<div class="row mb-3">
					<div class="col-sm-6">
					<h4 class="text-dark" id="caprak_name">Carpark name:  P1</h4>
					</div>
				</div>
				<div class="custom-control custom-checkbox mb-3">
					<input type="checkbox" class="custom-control-input" id="check_plate_type">
					<label class="custom-control-label" for="check_plate_type">Check plate type</label>
				</div>
				<div class="custom-control custom-checkbox mb-3">
					<input type="checkbox" class="custom-control-input" id="check_plate_country">
					<label class="custom-control-label" for="check_plate_country">Check plate country</label>
				</div>
				<div class="custom-control custom-checkbox mb-3">
					<input type="checkbox" class="custom-control-input" id="check_plate_area">
					<label class="custom-control-label" for="check_plate_area">Check plate area</label>
				</div>
				<div class="custom-control custom-checkbox mb-3">
					<input type="checkbox" class="custom-control-input" id="check_plate_prefix">
					<label class="custom-control-label" for="check_plate_prefix">Check plate prefix</label>
				</div>
				<div class="custom-control custom-checkbox mb-3">
					<input type="checkbox" class="custom-control-input" id="check_plate_number">
					<label class="custom-control-label" for="check_plate_number">Check plate number</label>
				</div>
				
				<div class="custom-control custom-checkbox mb-3">
					<input type="checkbox" class="custom-control-input" id="allow_O_Q_mismatch">
					<label class="custom-control-label" for="allow_O_Q_mismatch">Allow O Q mismaich</label>
				</div>	
				<div class="custom-control custom-checkbox mb-3">
				<input type="checkbox" class="custom-control-input" id="check_partial_plate_number">
					<label class="custom-control-label" for="check_partial_plate_number">Check parial plate number</label>
				</div>
				<div class="d-none" id="div_partial_check">									
					<div class="row">
						<div class="col-md-3 form-group">
							<label for="">Number of letters from begining</label>
							<input type="number" class="form-control" id="check_plate_number_begining" placeholder="" min="1" max="10">
						</div>
						<div class="col-md-3 form-group">
							<label for="">Number of letters from end</label>
							<input type="number" class="form-control" id="check_plate_number_end" placeholder=""  min="1" max="10">
						</div>						                       
					</div>
				</div>
				
			
			<div class="col-md-1 form-group">
                   <input type="button" class="btn btn-block btn-info mt-2 btn-lg" value="Save" id="btnSubmit">				
                </div> 
			
       
        </div><!--card body-->
      </div>
    </div>
  </section>
</div>
<!-- End edit form -->

<!-- Table anpr settings -->
<div class="content-wrapper block-data" id="anpr-settings-table">
	<section class="content">
		<div class="container-wide">     
			<div class="card card-primary mb-4" >
				<?php echo anprSettings(array("task"=>"1"));?>
			</div>
		</div>
	</section>
</div>
<!-- End table anpr settings-->
<?php include('../../includes/footer.php');?>

<script>
var id;
 $("#check_partial_plate_number").change(function() 
	{            
    if ($(this).is(':checked')) 
		$("#div_partial_check").removeClass("d-none");
	else 		
        $("#div_partial_check").addClass("d-none");           
    });
	
//edit anpr settings
$(".btn-edit-anpr-settings").click(function (event) {	
	$("#edit-anpr-form").removeClass("d-none");	
	$("#anpr-settings-table").css('display', 'none');
	id=$(this).attr('id');	
	var data = {};	
    data['id'] = id;
	data['task'] = 3;
	var jsondata = JSON.stringify(data);	
	$.post("../ajax/anpr_settings.php",jsondata,function(data)
		{			
		//console.log(data);
		var response=JSON.parse(data);
		$("#caprak_name").html("Carpark :"+response.carpark_name);
		if(response.check_plate_type == 1) 			
            $("#check_plate_type").attr("checked",true);
		
		if(response.check_plate_country == 1) 			
            $("#check_plate_country").attr("checked",true);
		
		if(response.check_plate_area == 1) 			
            $("#check_plate_area").attr("checked",true);
		
		if(response.check_plate_prefix == 1) 			
            $("#check_plate_prefix").attr("checked",true);
		
		if(response.check_plate_number == 1) 			
            $(check_plate_number).attr("checked",true);
		
		if(response.check_partial_plate_number == 1) 			
			{
			$("#check_partial_plate_number").attr("checked",true);
			$("#check_partial_plate_number").trigger("change"); 
			$("#check_plate_number_begining").val(response.check_plate_number_begining);
			$("#check_plate_number_end").val(response.check_plate_number_end);
			}
		
		if(response.allow_O_Q_mismatch == 1) 			
            $("#allow_O_Q_mismatch").attr("checked",true);                                	
		});	
    }); //edit anpr settings
	
$("#btnSubmit").click(function (event) 
	{	
	var data = {};	
    data['id'] = id;
	data['task'] = 2;
	
	if ($("#check_plate_type").is(":checked"))
		data['check_plate_type'] = 1;
	else
		data['check_plate_type'] = 0;
	
	if ($("#check_plate_country").is(":checked"))
		data['check_plate_country'] = 1;
	else
		data['check_plate_country'] = 0;
	
	if ($("#check_plate_area").is(":checked"))
		data['check_plate_area'] = 1;
	else
		data['check_plate_area'] = 0;
	
	if ($("#check_plate_prefix").is(":checked"))
		data['check_plate_prefix'] = 1;
	else
		data['check_plate_prefix'] = 0;
	
	if ($("#check_plate_number").is(":checked"))
		data['check_plate_number'] = 1;
	else
		data['check_plate_number'] = 0;
	
	if ($("#check_partial_plate_number").is(":checked"))
		{
		data['check_partial_plate_number'] = 1;
		data['check_plate_number_begining'] = $("#check_plate_number_begining").val();
		data['check_plate_number_end'] = $("#check_plate_number_end").val();
		}
	else
		{
		data['check_partial_plate_number'] = 0;
		data['check_plate_number_begining'] =0;
		data['check_plate_number_end'] = 0;
		}
	
	if ($("#allow_O_Q_mismatch").is(":checked"))
		data['allow_O_Q_mismatch'] = 1;
	else
		data['allow_O_Q_mismatch'] = 0;
	
	var jsondata = JSON.stringify(data);
	//console.log(jsondata);	
	$.post("../ajax/anpr_settings.php",jsondata,function(data)
		{			
		if(data==="Updation Successfull")
			location.reload();
		else
			alert(data);
		
		});	
	
    }); //edit anpr settings


</script>