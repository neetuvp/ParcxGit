<?php 
$page_title="Device Dashboard";

include('../../includes/header.php');
?>

<div class="navbar-has-tablink">

<?php
include('../../includes/navbar-start.php');
?>


</ul>
<div class="header text-dark" id="pdf-report-header">Cloud Settings</div>
<div class="row">
  <div class="col tab-header d-flex justify-content-center">
    <div class="tab-link active" data-target="maintenance-settings">Maintenance Settings</div>
    <div class="tab-link" data-target="upload_settings">Upload Settings</div>
    <div class="tab-link" data-target="download_settings">Download Settings</div>
    
  </div>
</div>

<?php 
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>
<div class="content-wrapper " >
  <section class="content">
    <div class="container-wide">            
      <div class="block-data" data-status="upload_settings" style="display:none;">               
        <!-- <div class="table-responsive card card-table-blue" id="upload_settings">  -->
          <div class="card" >               
            <div class="card-body">     
              <table class="table table-blue table-bordered table-striped" id="upload_settings">                           
                <?php echo parcxSettings(array("task"=>"25"));?>
              </table>
            </div>
          </div>
      </div> 
      <div class="block-data" data-status="download_settings" style="display:none;" >  
      <div class="card" >               
            <div class="card-body">     
              <table class="table table-blue table-bordered table-striped" id="download_settings">                           
                <?php echo parcxSettings(array("task"=>"28"));?>
              </table>
            </div>
          </div>             
        <!-- <div class="table-responsive card card-table-blue" id="download_settings">                          
          <?php //echo parcxSettings(array("task"=>"28"));?>
        </div> -->
      </div> 
      <div class="block-data" data-status="maintenance-settings">               
        <!-- <div class="table-responsive card card-table-blue" id="maintenance_settings">                          
          <?php //echo parcxSettings(array("task"=>"22"));?>
        </div> -->
        <div class="card-body">     
              <table class="table table-blue table-bordered table-striped" id="maintenance_settings">                           
                <?php echo parcxSettings(array("task"=>"22"));?>
              </table>
            </div>
          </div> 
      </div> 
    </div>          
  </section>
</div>
<script>
  /* Table Show - Hide */

  function loadSettings(task,location)
    {
      var data = {};    
      data["task"]=task;            
      var jsondata=JSON.stringify(data);            
      $.post("../../modules/ajax/settings.php",jsondata,function(result){ 
       $("#"+location).html(result);
      })
      .fail(function(jqxhr,status,error){
          alert("Error: "+error);

      });  
    }

  $(document).ready(function () {
    $('.tab-link').on('click', function () {
      var $target = $(this).data('target');
      if ($target != 'all') {
        $('.block-data').css('display', 'none');
        $('.block-data[data-status="' + $target + '"]').fadeIn('slow');
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

/* === enable disable === */
var status;   
var id;
$(document).on("click", ".upload-setting-enable-disable-btn", function() 
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
      data["task"]="27";            
      var jsondata=JSON.stringify(data);            
      $.post("../../modules/ajax/settings.php",jsondata,function(result){ 
        if(result=="Successfull")                    
          loadSettings(25,"upload_settings");
        else
          alert(result);
      })
      .fail(function(jqxhr,status,error){
          alert("Error: "+error);

      }); 
    }
  else
    {
      loadSettings(25,"upload_settings");
    }
});

$(document).on("click", ".maintenance-setting-cancel-btn", function() 
    {
      loadSettings(22,"maintenance_settings");
});

$(document).on("click", ".download-setting-enable-disable-btn", function() 
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
      data["task"]="30";            
      var jsondata=JSON.stringify(data);            
      $.post("../../modules/ajax/settings.php",jsondata,function(result){ 
        if(result=="Successfull")
          loadSettings(28,"download_settings");                
        else
          alert(result);
      })
      .fail(function(jqxhr,status,error){
          alert("Error: "+error);

      }); 
    }
  else
    {
      loadSettings(28,"download_settings");
    }
});



/*=====edit======*/
$(document).on("click", ".maintenance-setting-edit", function() 
    {    
    id = $(this).parent('td').parent('tr').data('id'); 
    if($(this).text()==="Edit")
      {             
      setting_value=$(this).parent('td').siblings(":eq( 2 )").text();
      setting_name=$(this).parent('td').parent('tr').data('name');       
      $(this).parent('td').siblings(":eq( 2 )").html("<input type = 'text' id = '"+id+"'  value = '"+setting_value+"'>");
      $(this).text("Save");
	    ($('#cancelbtn'+id).show());      
      }
    else
      {
      setting_value=$("#"+id).val().trim();  
      var data = {}; 
      data["id"]=id;     
      data["setting_value"]=setting_value;
      data["setting_name"]=setting_name;
      data["task"]="23";            
      var jsondata=JSON.stringify(data);         
      $.post("../../modules/ajax/settings.php",jsondata,function(result)
        {         
        if(result=="Successfull")
          loadSettings(22,"maintenance_settings");                
        else
          alert(result);
        })
      .fail(function(jqxhr,status,error)
        {
        alert("Error: "+error);
        });                             
      }
});
  
$(document).on("click", ".upload-setting-edit", function() 
  {    
  id = $(this).parent('td').parent('tr').data('id'); 
  if($(this).text()==="Edit")
    {      						
    var obj = $(this).parent('td').siblings(":eq( 2 )").find('input:checkbox');
    if(obj.attr("checked") =="checked")
      {
      $(this).parent('td').siblings(":eq( 2 )").html("<input type = 'checkbox' id = 'dc"+id+"' checked>");
      } 
    else
      {
      $(this).parent('td').siblings(":eq( 2 )").html("<input type = 'checkbox' id = 'dc"+id+"'>");
      }

    setting_value=$(this).parent('td').siblings(":eq( 3 )").text();
    setting_name=$(this).parent('td').parent('tr').data('name');       
    $(this).parent('td').siblings(":eq( 3 )").html("<input type = 'text' id = 'tasktext"+id+"'  value = '"+setting_value+"'>");
  
    setting_value=$(this).parent('td').siblings(":eq( 4 )").text();
    setting_name=$(this).parent('td').parent('tr').data('name');       
    $(this).parent('td').siblings(":eq( 4 )").html("<input type = 'text' id = 'limittext"+id+"'  value = '"+setting_value+"'>");
  
    setting_value=$(this).parent('td').siblings(":eq( 5 )").text();
    setting_name=$(this).parent('td').parent('tr').data('name');       
    $(this).parent('td').siblings(":eq( 5 )").html("<input type = 'text' id = 'intervaltext"+id+"'  value = '"+setting_value+"'>");
  
  
    $(this).text("Save");
    $(this).parent('td').parent('tr').find(".upload-setting-enable-disable-btn").text("Cancel");
    $(this).parent('td').parent('tr').find(".upload-setting-enable-disable-btn").removeClass("btn-success");
    $(this).parent('td').parent('tr').find(".upload-setting-enable-disable-btn").addClass("btn-danger");
    }
  else
    {		  
     		  
    var data = {}; 
    data["id"]=id;  
    if ($('#dc'+id).is(":checked"))	   
      data["dc"]="1";
    else
      data["dc"]="0";
    data["cloudtask"]=$('#tasktext'+id).val();
    data["limit"]=$('#limittext'+id).val();
    data["interval"]=$('#intervaltext'+id).val();
    data["task"]="26";            
    var jsondata=JSON.stringify(data);          
    $.post("../../modules/ajax/settings.php",jsondata,function(result)
    {         
    if(result=="Successfull")			                 
      loadSettings(25,"upload_settings");
    else
      alert(result);
    })
    .fail(function(jqxhr,status,error)
    {
    alert("Error: "+error);
    });                             
    }
});
  
  $(document).on("click", ".download-setting-edit", function() 
    {    
    id = $(this).parent('td').parent('tr').data('id'); 
    if($(this).text()==="Edit")
      {     						
      var obj = $(this).parent('td').siblings(":eq( 2 )").find('input:checkbox');
      if(obj.attr("checked") =="checked")
      {
        $(this).parent('td').siblings(":eq( 2 )").html("<input type = 'checkbox' id = 'dcdownload"+id+"' checked>");
      }else
      {
        $(this).parent('td').siblings(":eq( 2 )").html("<input type = 'checkbox' id = 'dcdownload"+id+"'>");
      }
  
      setting_value=$(this).parent('td').siblings(":eq( 3 )").text();
      setting_name=$(this).parent('td').parent('tr').data('name');       
      $(this).parent('td').siblings(":eq( 3 )").html("<input type = 'text' id = 'tasktext"+id+"'  value = '"+setting_value+"'>");
	  
	    setting_value=$(this).parent('td').siblings(":eq( 4 )").text();
      setting_name=$(this).parent('td').parent('tr').data('name');       
      $(this).parent('td').siblings(":eq( 4 )").html("<input type = 'text' id = 'limittext"+id+"'  value = '"+setting_value+"'>");
	  
	    setting_value=$(this).parent('td').siblings(":eq( 5 )").text();
      setting_name=$(this).parent('td').parent('tr').data('name');       
      $(this).parent('td').siblings(":eq( 5 )").html("<input type = 'text' id = 'intervaltext"+id+"'  value = '"+setting_value+"'>");
	  
	  
      $(this).text("Save");
      $(this).parent('td').parent('tr').find(".download-setting-enable-disable-btn").text("Cancel");
      $(this).parent('td').parent('tr').find(".download-setting-enable-disable-btn").removeClass("btn-success");
      $(this).parent('td').parent('tr').find(".download-setting-enable-disable-btn").addClass("btn-danger");
      }
    else
      {		  
		  var dcdownload=0;
		  if ($('#dcdownload'+id).is(":checked"))
		  {
			  dcdownload=1;
		  }		  
		  var data = {}; 
		  data["id"]=id;     
		  data["dc"]=dcdownload;
		  data["cloudtask"]=$('#tasktext'+id).val();
		  data["limit"]=$('#limittext'+id).val();
		  data["interval"]=$('#intervaltext'+id).val();
		  data["task"]="29";            
		  var jsondata=JSON.stringify(data);  		
		  $.post("../../modules/ajax/settings.php",jsondata,function(result)
			{         
			if(result=="Successfull")
        loadSettings(28,"download_settings");
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

<?php include('../../includes/footer.php');?>