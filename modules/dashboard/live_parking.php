<?php
include('../../includes/header.php');
include('../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">Live Review</div>

<?php
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

<div class="content-wrapper">
  <section class="content">
    <div class="container-wide" id="report-table">	      
    </div>
  </section>
</div>
<?php include('../../includes/footer.php');?>

<script>
loadPage();
function live_parking()
  {
  var data={};
  data["task"]=6;
  data["language"]=$("#language").val();
  var json = JSON.stringify(data);
  $.post("../ajax/dashboard-ajax.php",json,function(data)
			{	
      var div= '<div class="card"><div class="card-body"><table id="RecordsTable" class="table table-blue table-bordered table-striped">'+data+' </table></div></div>'
      $("#report-table").html(div);	      
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
			}); 
  }
function loadPage()
  {
  loadheading("live_parking");
  live_parking(); 
  }
$("#language").change(function(){
  loadPage();
}); 

</script>