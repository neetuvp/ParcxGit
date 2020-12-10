<?php 
$page_title="facility Dashboard";

include('../../includes/header.php');
?>

<div class="navbar-has-tablink">

<?php
include('../../includes/navbar-start.php');
?>


</ul>
<div class="header text-dark" id="pdf-report-header">Facility Settings</div>
<div class="row hidden-sm-down">
    <div class="col tab-header d-flex justify-content-center">
        <div class="tab-link active" data-target="overview">Overview</div>
        <div class="tab-link" data-target="form">Add facility</div>
    </div>
</div>

<?php 
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

<div class="content-wrapper">
    <section class="content">
        <div class="container-wide">        
          <!-- add/update facility form --> 
          <form class="block-data card card-body col-md-4" data-status="form" style="display:none;" id="form"> 
          <input type="hidden" id="user_id" value="<?php echo $_SESSION['userId'];?>" />
              <div class="row ">
                  <div class="col form-group">
                    <label for="">Facility name</label>
                    <input type="text" class="form-control" id="facility_name" placeholder=""  required name="facility_name">
                  </div>
              </div>				                                    
              <div class="row">	
					      <div class="col form-group">
                  <label for="">Facility location</label>
                  <input type="text" class="form-control" id="facility_location" >
                  </div> 
              </div>  
              <div class="row">	
					      <div class="col form-group">
                  <label for="">Facility number</label>
                  <input type="number" class="form-control" id="facility_number" value="0">
                  </div> 
              </div>                                                                       
              <input type="submit" class="signUp btn btn-block btn-info mt-2 btn-lg" value="Submit" id="add-edit-button">
            </form>

            <!-- facility table -->
          <div class="block-data" data-status="overview" id="table-list">               
          <div class="table-responsive card card-table-blue">                          
             <?php echo parcxSettings(array("task"=>"2"));?>
              </div>
          </div>             

        </div>
    </section>
</div>
<?php include('../../includes/footer.php');?>
<script>
var status;   
var id;
$(document).ready(function () 
  {  
  $("* [data-target]").on('click', function () 
    {    
    var $target = $(this).data('target');  
    if($target=="form")                        
      {
      $("#form").trigger('reset');
      $("#add-edit-button").val("Submit");  
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
              data["id"]="";
            else
              data["id"]=id;
            data["facility_number"]=$("#facility_number").val();
            data["facility_name"]=$("#facility_name").val();
            data["facility_location"]=$("#facility_location").val(); 
            data["user_id"]=$("#user_id").val(); 
            data["task"]="1";            
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
        }
    });

});

/* === enable disable product === */
$(document).on("click", ".facility-enable-disable-btn", function() 
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
    data["task"]="3";            
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
$(document).on("click", ".facility-edit", function() 
    {
    id = $(this).parent('td').parent('tr').data('id');    
    var data = {};
    data["id"]=id;      
    data["task"]="8";            
    var jsondata=JSON.stringify(data);            
    $.post("../../modules/ajax/settings.php",jsondata,function(result){ 
      console.log(result);
      var response=JSON.parse(result); 
      $("#facility_name").val(response.facility_name);
      $("#facility_number").val(response.facility_number);
      $("#facility_location").val(response.facility_location);
      $("#add-edit-button").val("Edit");  
    })
    .fail(function(jqxhr,status,error){
        alert("Error: "+error);

    }); 
});

</script>
