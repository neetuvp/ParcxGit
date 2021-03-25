
<script>
var search_label="<?=$json["search"]?>";
var entries_label = "<?=$json["entries_label"]?>";
var info_label="<?=$json["info_label"]?>";
previous_label = '<?=$json["previous"]?>';
next_label = '<?=$json["next"]?>';

$(document).ready(function () 
    {	         
    $("#language").val("<?php echo isset($_SESSION["language"])?$_SESSION["language"]:"English"; ?>");
    // check date is selected
    $(".applyBtn").click(function () 
        {
        $(this).data('clicked', true);
        });

    // view report button click
    $('#view-report-button').click(function (event) 
        {
        if ($('.applyBtn').data('clicked')) 
            {
            // only display loader if button has been clicked
            $("#loader").css("visibility", "visible");
            }
        });        
    
    if($("#report-content").find('#RecordsTable').length!=0) 
       loadDataTable();    
    });
             
                              
  // button ui changes on successful report load
  function loadReport(data)
      {
      
      $("#report-content").html(data);
      $("#loader").css("visibility", "hidden");       
      if($("#report-content").find('table').length!=0)
          {
          $("#action-buttons").css("visibility", "visible");  
          loadDataTable() ;       
          }
      else 
          { 
          $("#action-buttons").css("visibility", "hidden");                
          if($("#export-container").is(":visible")) 
              $("#export-container").addClass("hidden");
          }      
      }

  function reportSuccess() 
    {     
    $("#action-buttons").css("visibility", "visible");
    $("#loader").css("visibility", "hidden");      
    if($("#report-content").find('#RecordsTable').length!=0) 
      loadDataTable();
    }



function loadDataTable()
    {      
    $('#RecordsTable').DataTable(
        {
        "paging": true,
        "lengthChange":true,
        "searching": true,
        "ordering": true,
        "info": true,
        "autoWidth": false,
        "lengthMenu": [[10, 25, 50, -1], [10, 25, 50, "All"]],
        "aaSorting": [],
        "language": {search: search_label,},
        "oLanguage": 
                {
                "sLengthMenu": entries_label,
                "info":info_label,
                "oPaginate": 
                        {
                        "sPrevious": previous_label,
                        "sNext": next_label
                        }
                },					            
        });   	  
    }
	
	
function setLabel(label,element)
    {
    var data={};
    data["task"]=13;
    data["language"]=$("#language").val();
    data["label"]=label;
    var json = JSON.stringify(data);
    $.post("<?=URL?>modules/ajax/reports.php",json,function(data)
        {		      
        $("#"+element).html(data);        
        });     
    }
    
function changeLanguage()
    { 
    var data={};    
    data["language"]=$("#language").val();    
    var json = JSON.stringify(data);
    $.post("<?=URL?>modules/ajax/updatelanguage.php",json,function(data)
        {        
        console.log(data); 
        });        
    }

function loadheading(label)
    {
    setLabel(label,"pdf-report-header");
    }


function loadheadingreport(label)
    {
    setLabel(label,"pdf-report-header");
    setLabel("view_report","view-report-button");
    setLabel("view_details","view-details-button");
    }
</script>

</div>

<!------------------------>
<!-- load plugins -------->
<!------------------------>

<!-- jQuery UI - for draggable divs -->
<script src="<?=URL?>plugins/jquery/jquery-ui.min.js"></script>

<!-- resolve conflict between jQuery UI tooltip with Bootstrap tooltip -->
<script>$.widget.bridge('uibutton', $.ui.button)</script>

<!-- bootstrap 4 -->
<script src="<?=URL?>plugins/bootstrap/js/bootstrap.bundle.min.js"></script>

<!-- daterangepicker -->
<script src="<?=URL?>plugins/jquery/moment.min.js"></script>
<script src="<?=URL?>plugins/daterangepicker/daterangepicker.js"></script>

<!-- theme scripts -->
<script src="<?=URL?>dist/js/parcx.js"></script>

<!-- PDF export -->
<script src="<?=URL?>plugins/jspdf/canvas2image.js"></script>
<script src="<?=URL?>plugins/jspdf/html2canvas.js"></script>
<script src="<?=URL?>plugins/jspdf/jspdf.1.5.3.js"></script>
<script src="<?=URL?>plugins/jspdf/jspdf.autotable.js"></script>

 <!-- DataTables -->
 <script src="<?=URL?>plugins/datatables/jquery.dataTables.js"></script>
<script src="<?=URL?>plugins/datatables-bs4/js/dataTables.bootstrap4.js"></script>


<!-- SlimScroll -->
<script src="<?=URL?>plugins/slimScroll/jquery.slimscroll.min.js"></script>
<!-- FastClick -->
<script src="<?=URL?>plugins/fastclick/fastclick.js"></script>


<!------------------------>
<!-- load custom scripts-->
<!------------------------>

<!-- chart scripts and global vars -->
<?php include('chart-custom.php'); ?>
<!--validation-->
<script src="<?=URL?>dist/js/jquery.validate.js"></script>

<!-- init daterangepicker -->
<script src="<?=URL?>dist/js/init-daterangepicker.js"></script>

<!-- export scripts -->
<script src="<?=URL?>dist/js/excel-export.js"></script>
<script src="<?=URL?>dist/js/pdf-export.js"></script>

<!-- application scripts --> 
<!--<script src="<?=URL?>dist/js/alertModule.js"></script>-->
<!--<script src="<?=URL?>dist/js/counter.js"></script>-->
<script src="<?=URL?>dist/js/validationUsers.js"></script>
<script src="<?=URL?>dist/js/configuration.js"></script>
</body>

</html>
