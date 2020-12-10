<script>
  $(document).ready(function () {

    // check date is selected
    $(".applyBtn").click(function () {
      $(this).data('clicked', true);
    });

    // view report button click
    $('#view-report-button').click(function (event) {
      if ($('.applyBtn').data('clicked')) {
        // only display loader if button has been clicked
        $("#loader").css("visibility", "visible");
      }
    });

    // initialize multiselect

    // initialize carpark multiselect

    $('#multiselect,#multiselect_carpark').multiselect({
      buttonWidth: '100%',
      includeSelectAllOption: true,
      selectAllText: "All Carparks",
      nonSelectedText: 'Select Carparks',
      selectAllNumber: false,
      allSelectedText: "All Carparks",
    });
    

    $('#multiselect_parking_zone').multiselect({
      buttonWidth: '100%',
      includeSelectAllOption: true,
      selectAllText: "All Parking zone",
      nonSelectedText: 'Select Parking zone',
      selectAllNumber: false,
      allSelectedText: "All Parking zone",
    });

    // initialize payment devices multiselect

    $('#deviceNumber').multiselect({
      buttonWidth: '100%',
      includeSelectAllOption: true,
      selectAllText: "All Devices",
      nonSelectedText: "Select Devices",
      selectAllNumber: false,
      allSelectedText: "All Devices",
    });
    

    // initialize Operator multiselect

    $('#operatormultiple').multiselect({
      buttonWidth: '100%',
      includeSelectAllOption: true,
      selectAllText: "All Operators",
      nonSelectedText: "Select Operators",
      selectAllNumber: false,
      allSelectedText: "All Operators",
    });

    // initialize payment types multiselect

    $('#paymentCategory').multiselect({
      buttonWidth: '100%',
      includeSelectAllOption: true,
      selectAllText: "All  Category",
      nonSelectedText: "Select  Category",
      selectAllNumber: false,
      allSelectedText: "All  Category",
    });

    // initialize payment types multiselect

    $('#paymentType').multiselect({
    buttonWidth: '100%',
    includeSelectAllOption: true,
    selectAllText: "All Payment Types",
    nonSelectedText: "Select Payment Types",
    selectAllNumber: false,
    allSelectedText: "All Payment Types",
    });

    // initialize discounts

    $('#discounts').multiselect({
      buttonWidth: '100%',
      includeSelectAllOption: true,
      selectAllText: "All Discounts",
      nonSelectedText: "Select Discounts",
      selectAllNumber: false,
      allSelectedText: "All Discounts",
    });

    // initialize weekdays

    $('#days').multiselect({
      buttonWidth: '100%',
      includeSelectAllOption: true,
      selectAllText: "All Days of the Week",
      nonSelectedText: 'Select Day of the Week',
      selectAllNumber: false,
      allSelectedText: "All Days of the Week",
    });

    $('#shedule-interval').multiselect({
      buttonWidth: '100%',
      includeSelectAllOption: true,
      selectAllText: "Daily",
      nonSelectedText: 'Select Interval',
      selectAllNumber: false,      
      allSelectedText: "Daily",
    });
    
    $('#shedule-interval').multiselect("selectAll",false);
    $('#shedule-interval').multiselect('updateButtonText');
    
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
          "aaSorting": []
          
          });      
    }

function loadheading(label)
  {
  var data={};
  data["task"]=7;
  data["language"]=$("#language").val();
  data["label"]=label;
  var json = JSON.stringify(data);
  $.post("../ajax/dashboard-ajax.php",json,function(data)
			{		      
      $("#pdf-report-header").html(data);	      
			}); 
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
<script src="<?=URL?>dist/js/plugins/jspdf/canvas2image.js"></script>
<script src="<?=URL?>dist/js/plugins/jspdf/html2canvas.js"></script>
<script src="<?=URL?>dist/js/plugins/jspdf/jspdf.1.5.3.js"></script>
<script src="<?=URL?>dist/js/plugins/jspdf/jspdf.autotable.js"></script>

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

<!-- init daterangepicker // before pdf export -->
<script src="<?=URL?>dist/js/init-daterangepicker.js"></script>

<!-- export scripts -->
<script src="<?=URL?>dist/js/excel-export.js"></script>
<script src="<?=URL?>dist/js/pdf-export.js"></script>

<!-- application scripts --> 
<script src="<?=URL?>dist/js/alertModule.js"></script>
<script src="<?=URL?>dist/js/users.js"></script>
<script src="<?=URL?>dist/js/counter.js"></script>
<script src="<?=URL?>dist/js/validationUsers.js"></script>
<script src="<?=URL?>dist/js/configuration.js"></script>
<!--<script src="ist/js/discountsettings.js"></script>
<script src="dist/js/parkingsettings.js"></script>
<script src="dist/js/whitelistsettings.js"></script>
<script src="dist/js/daemonstatus.js"></script> -->
</body>

</html>