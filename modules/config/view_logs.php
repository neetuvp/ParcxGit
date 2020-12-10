
<?php 
$page_title="Device Dashboard";

include('../../includes/header.php');
?>

<div class="navbar-has-tablink">

<?php
include('../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">View Logs</div>
<?php 
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

<!-- Active Text Messages-->
<div class="content-wrapper">
<!-- additional menu -->
  <div class="additional-menu row m-0 bg-white border-bottom ">
    <div class="col d-flex pl-1 align-items-center">
      <div class="flex-grow-1 row additional-menu-left nav-button-group">               
        <?php echo parcxViewLogs(array("task"=>"1","main_folder"=>""));?>
      </div>      
    </div>
  </div>
  <!-- end / additional menu -->
  <!-- additional menu -->
  <div class="additional-menu row m-0 bg-white border-bottom ">
    <div class="col d-flex pl-1 align-items-center">
      <div class="flex-grow-1 row additional-menu-left nav-button-group" id="subfolder">  
      
      </div>              
      </div>      
    </div>
  <!-- end / additional menu -->

  <section class="content">
    <div class="container-wide">	
      <div class="card" >               
        <div class="card-body" id="report">     
                  
          </table>
        </div>                                                  
      </div>        
    </div>    
  </section>
  </div>
<?php include('../../includes/footer.php');?>
<script>
var main_folder;
var sub_folder; 
var file_name;
var click=1;
showLogs();
$(document).on("click", ".btn-main", function() 
    {
    $('.btn-main').removeClass('btn-success');
    $('.btn-main').addClass('btn-info');   
    $(this).addClass('btn-success');
    $(this).removeClass('btn-info');    
    var data = {};
    data["main_folder"] = $(this).attr("id");;
    data["task"]=1;
    var jsondata = JSON.stringify(data);           
    $.post("../../modules/ajax/view_logs.php",jsondata,function(data){                         
        $("#subfolder").html(data);     
        showLogs();               
    })
    .fail(function(jqxhr,status,error){
        alert("Error: "+error);

    });

    });

function getfolders()
    {
    $(".btn-main").each(function(){      
      if($(this).hasClass("btn-success"))
        {
        main_folder=$(this).attr("id");
        }
    });  
 
    sub_folder="";
    $(".btn-sub").each(function(){      
      if($(this).hasClass("btn-success"))
        {
          sub_folder=$(this).attr("id");
        }
    });
    }

function showLogs()
    {    
    getfolders();
    var data = {};
    data["task"]=2;
    data["main_folder"]=main_folder;
    data["sub_folder"]=sub_folder;
    var jsondata = JSON.stringify(data);           
    $.post("../../modules/ajax/view_logs.php",jsondata,function(data){                         
      $("#report").html(data); 
      $('#RecordsTable').DataTable(
        {
        "paging": true,
        "lengthChange":true,
        "searching": true,
        "ordering": true,
        "info": true,
        "autoWidth": false,
        "lengthMenu": [[10, 25, 50, -1], [10, 25, 50, "All"]],
        "aaSorting": []
        
        });                          
    })
    .fail(function(jqxhr,status,error){
        alert("Error: "+error);

    });  
    }

$(document).on("click", ".btn-sub", function() 
    {
    $('.btn-sub').removeClass('btn-success');
    $('.btn-sub').addClass('btn-info');   
    $(this).addClass('btn-success')
    $(this).removeClass('btn-info')
    showLogs();
    });

$(document).on("click", ".btn-view", function() 
    { 
    getfolders();    
    var newWindow = window.open();    
    var data = {};
    data["task"]=3;
	  data["main_folder"]=main_folder;
	  data["sub_folder"] = sub_folder;
    data["file_name"] = $(this).parent('td').siblings(":eq( 1 )").text();	      

    var jsondata = JSON.stringify(data);  
    console.log(jsondata);	
    $.post("../../modules/ajax/view_logs.php",jsondata,function(data){           
        newWindow.document.write(data);   	
    })
    .fail(function(jqxhr,status,error){
        alert("Error: "+error);
    });
    newWindow.stop();  

    });

function download(text) 
  {   	
	var element = document.createElement('a'); 
	element.setAttribute('href','data:text/plain;charset=utf-8, '+ encodeURIComponent(text)); 
	element.setAttribute('download', file_name); 
	document.body.appendChild(element);   	
	element.click();   
	document.body.removeChild(element); 
  }

$(document).on("click", ".btn-download", function() 
    {
    getfolders();   
    file_name=$(this).parent('td').siblings(":eq( 1 )").text();	 
    
    var data = {};
    data["task"]=3;
	  data["main_folder"]=main_folder;
	  data["sub_folder"] = sub_folder;
    data["file_name"] = file_name;      

    var jsondata = JSON.stringify(data);  
    console.log(jsondata);	
    $.post("../../modules/ajax/view_logs.php",jsondata,function(data){           
          download(data)	
    })
    .fail(function(jqxhr,status,error){
        alert("Error: "+error);
    });   
    });

</script>