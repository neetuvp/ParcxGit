<?php 
$page_title="Device Settings";

include('../../includes/header.php');
?>

<div class="navbar-has-tablink">

<?php
include('../../includes/navbar-start.php');
?>


</ul>
<div class="header text-dark" id="pdf-report-header">System Settings</div>
<div class="row hidden-sm-down">
    <div class="col tab-header d-flex justify-content-center">
        <div class="tab-link active" data-target="overview">Overview</div>
        <div class="tab-link" data-target="form">Add Settings</div>
    </div>
</div>

<?php 
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

<div class="content-wrapper">
    <section class="content">
        <div class="container-wide">        
          <!-- add/update form --> 
          <form class="block-data card card-body col-md-4" data-status="form" style="display:none;" id="form"> 
          <input type="hidden" id="user_id" value="<?php echo $_SESSION['userId'];?>" />
              
            <div class="row">
                  <div class="col form-group">
                    <label for="">Setting Description</label>
                    <input type="text" class="form-control" id="setting_description" placeholder=""  required name="setting_description">
                  </div>
              </div>
              <div class="row">
                  <div class="col form-group">
                    <label for="">Setting name</label>
                    <input type="text" class="form-control" id="setting_name" placeholder=""  required name="setting_name">
                  </div>
              </div>				                                                  
              <div class="row">						     
                  <div class="col form-group">
                  <label>Setting value</label>                  
                  <input type="text" class="form-control" id="setting_value" required name="setting_value">                  
                  </div> 
              </div>              
                                                                                                                           
              <input type="submit" class="signUp btn btn-block btn-info mt-2 btn-lg" value="Submit" id="add-edit-button">
            </form>

            <!--  table -->
            <div class="block-data" data-status="overview">
              <div class="card" >               
                  <div class="card-body">     
                    <table id="RecordsTable" class="table table-blue table-bordered table-striped">                    
                    <?php echo parcxSettings(array("task"=>"17"));?>
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
var setting_value;
var setting_name;

$(document).ready(function () 
  {
    loadDataTable() ;    
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
            data["setting_description"]=$("#setting_description").val();
            data["setting_name"]=$("#setting_name").val();
            data["setting_value"]=$("#setting_value").val();
            data["task"]="18";            
            var jsondata=JSON.stringify(data);   
            console.log(jsondata);         
            $.post("../../modules/ajax/settings.php",jsondata,function(result){ 
              if(result=="Successfull")
                location.reload();                  
              else
                alert(result);
           })
           .fail(function(jqxhr,status,error){
               alert("Error: "+error);});                              
        }
    });

});

/* === enable disable === */
var status;   
var id;
$(document).on("click", ".setting-enable-disable-btn", function() 
    {
    id = $(this).parent('td').parent('tr').data('id');
    var status_text=$(this).text();  
    if(status_text!="Cancel")         
      {
      if(status_text=="Disable")
          status=0;
      else
          status=1;

      var data = {};
      data["id"]=id;
      data["status"]=status;   
      data["task"]="19";            
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
  else
    {
      location.reload(); 
    }
});

/*=====edit======*/
$(document).on("click", ".setting-edit", function() 
    {    
    id = $(this).parent('td').parent('tr').data('id'); 
    if($(this).text()==="Edit")
      {             
      setting_value=$(this).parent('td').siblings(":eq( 2 )").text();
      setting_name=$(this).parent('td').parent('tr').data('name');       
      $(this).parent('td').siblings(":eq( 2 )").html("<input type = 'text' id = '"+id+"'  value = '"+setting_value+"'>");
      $(this).text("Save");
      $(this).parent('td').parent('tr').find(".setting-enable-disable-btn").text("Cancel");
      $(this).parent('td').parent('tr').find(".setting-enable-disable-btn").removeClass("btn-success");
      $(this).parent('td').parent('tr').find(".setting-enable-disable-btn").addClass("btn-danger");
      }
    else
      {
      setting_value=$("#"+id).val().trim();  
      var data = {}; 
      data["id"]=id;     
      data["setting_value"]=setting_value;
      data["setting_name"]=setting_name;
      data["task"]="18";            
      var jsondata=JSON.stringify(data);   
      console.log(jsondata);      
      $.post("../../modules/ajax/settings.php",jsondata,function(result)
        {         
        if(result=="Successfull")
          location.reload();                  
        else
          alert(result);
        })
      .fail(function(jqxhr,status,error)
        {
        alert("Error: "+error);
        });                             
      }
  });

</script>
