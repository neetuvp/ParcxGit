<?php
$page_title="Application Home";


//# Import application layout.
include('../../../includes/header.php');
include('../../../includes/navbar-start.php');



$data=array();
$data["task"]=29;     
$data["language"]=$_SESSION["language"];
$data["page"]=3;
$json=parcxReport($data);

?>

</ul>
<div class="header text-dark" id="pdf-report-header"><?=$json["revenue_report"]?></div>

<?php

include('../../../includes/navbar-end.php');
include('../../../includes/sidebar.php');
?>

<div class="content-wrapper">

  <!-- additional menu -->
  <div class="additional-menu row m-0 bg-white border-bottom">
    <div class="col d-flex pl-1 align-items-center">

      <div class="flex-grow-1 row additional-menu-left">

        <!-- carparks -->
        <div class="col-md-2">
          <select class="form-control" id="multiselect" multiple="multiple">
              <?php echo parcxSettings(array("task"=>"12"));?>
          </select>
        </div>

        <!--weekdays -->
        <div class="col-md-2">
          <select id="days" multiple="multiple" class="form-control">
            <option value="'Sunday'" id = "sunday"><?=$json["sunday"]?></option>
            <option value="'Monday'" id = "monday"><?=$json["monday"]?></option>
            <option value="'Tuesday'" id = "tuesday"><?=$json["tuesday"]?></option>
            <option value="'Wednesday'" id = "wednesday"><?=$json["wednesday"]?></option>
            <option value="'Thursday'" id="thursday"><?=$json["thursday"]?></option>
            <option value="'Friday'" id="friday"><?=$json["friday"]?></option>
            <option value="'Saturday'" id="saturday"><?=$json["saturday"]?></option>
          </select>
        </div>                              
<?php include('../../../includes/additional-menu-report.php');?>       
  <section class="content">
    <div class="container-wide">

      <!-- revenue data chart -->
      <div class="d-none" id="chart_container">
        <div class="row jspdf-graph">
          <div class="col-lg-6 pl-0">
            <div class="card barchart-box mt-0">
              <p class="text-center chart-header text-dark justify-content-middle">
                <?=$json["earnings"]?>
              </p>
              <div class="position-relative">
                <div><canvas id="revenue-chart" height="300"></canvas></div>
              </div>
            </div>
          </div>
        </div>
      </div>
      <br>
    <div class="card">
      <div class="card-body" id="report-content">   
	  <?php
	  $current_date=date("Y-m-d");    
	  $data["from"]=$current_date." ".DAY_CLOSURE_START;
	  $data["to"]=$current_date." ".DAY_CLOSURE_END;           
	  $data["carpark"]="";    
	  $data["weekdays"]="";		  
	  $data["language"] = $_SESSION["language"];
	  $data["task"]=22; 
	  echo parcxReport($data); 
	  ?>
      </div>
    </div>
</div>
</section>
</div>

<?php include('../../../includes/footer.php');?>

<script>
//////////////////////////////
// load report data
//////////////////////////////
var id;
var date_range_message="choose date range";
from="<?=$current_date." ".DAY_CLOSURE_START?>";
to="<?=$current_date." ".DAY_CLOSURE_END?>";

var click_count = 0;
var load_report = 0;
var search_label="";
var entries_label = "";
var info_label="";
var next_label="";
var previous_label="";
$(function() 
{
    search_label="<?=$json["search"]?>";
    entries_label = "<?=$json["entries_label"]?>";
    info_label="<?=$json["info_label"]?>";
    next_label="<?=$json["next"]?>"; 
    previous_label = "<?=$json["previous"]?>"; 
    
    $('#multiselect').multiselect(
        {
        buttonWidth: '100%',
        includeSelectAllOption: true,      
        selectAllText: "<?=$json["all_carparks"]?>",
        nonSelectedText: "<?=$json["select_carparks"]?>",
        selectAllNumber: false,
        allSelectedText: "<?=$json["all_carparks"]?>",       
        });    
    $('#days').multiselect(
        {
        buttonWidth: '100%',
        includeSelectAllOption: true,
        selectAllText: "<?=$json["all_days"]?>",               
        nonSelectedText:"<?=$json["select_days"]?>",       
        selectAllNumber: false,
        allSelectedText: "<?=$json["all_days"]?>"      
        }); 
});

//$('#view-report-button').click(function (event) 
function callReport()
{
  
    var data={};
    data["from"]=from;
    data["to"]=to;         
    data["carpark"]=$("#multiselect").val().toString(); 
    data["weekdays"]=$("#days").val().toString();  
    data["option-type"]=1;  
    data["language"] = $("#language").val();	
    data["task"]=22;
	
    var jsondata = JSON.stringify(data);     
	$.post("../../ajax/reports.php",jsondata,function(data)
      {		
	
      $("#report-content").html(data);
      reportSuccess();
      createChart();
	  loadDataTable1();
	  load_report=1;
      })
    .fail(function(jqxhr,status,error)
      {
      alert("Error: "+error);
      });  


  event.preventDefault();
 }
 
//});


function loadDataTable1()
{
	
  $("table[id^='TABLE']").DataTable(
	  {
	  "paging": true,
	  "lengthChange":true,
	  "searching": true,
	  "ordering": true,
	  "info": true,
	  "autoWidth": false,
	  "lengthMenu": [[5,10, 25, 50, -1], [5,10, 25, 50, "All"]],
	  "aaSorting": [],
		"language": {
			search: search_label,
		},
		"oLanguage": {
			"sLengthMenu": entries_label,
			"info":info_label,
			"oPaginate": {
			"sPrevious": previous_label,
			"sNext": next_label
			}
		},
		
	  
	  
	  });   
	
}

$('#view-report-button').click(function (event) 
    { 	
    if (!daterange)		
        alert(date_range_message);        		
    else 
	callReport();	    
    });


/*function loadPage()
{
  loadheadingreport("revenue_report");
  if(load_report==1)
	revenue_report(); 
}
$("#language").change(function(){
  loadPage();
});*/

function loadReportLabels()    
    {
    var data={};
    data["task"]=29;
    data["language"]=$("#language").val();    
    data["page"]=3;
    var json = JSON.stringify(data);
    $.post("../../ajax/reports.php",json,function(data)
        {		
        var json=JSON.parse(data);
        date_range_message=json.choose_datetime_range;
        $("#reservationtime").attr('placeholder',json.choose_datetime_range);        
        $("#pdf-report-header").html(json.revenue_report);   
        $("#view-report-button").html(json.view_report);   
        $("#export").html(json.export);   
        $("#export_excel_report").html(json.export_to_excel);           
        $("#export_pdf_report").html(json.export_to_pdf); 
        $("#logout").html(json.logout); 
        search_label=json.search;   
        entries_label= json.entries_label;
        info_label=json.info_label;
        previous_label=json.previous;
        next_label=json.next;        

        $("#sunday").html(json.sunday);
        $("#monday").html(json.monday);
        $("#tuesday").html(json.tuesday);
        $("#wednesday").html(json.wednesday);
        $("#thursday").html(json.thursday);
        $("#friday").html(json.friday);
        $("#saturday").html(json.saturday);
        $(".chart-header").html(json.earnings);
        
        
        
        $('#multiselect').multiselect('destroy');
        $('#multiselect').multiselect(
            {
            buttonWidth: '100%',
            includeSelectAllOption: true,      
            selectAllText: json.all_carparks,
            nonSelectedText: json.select_carparks,
            selectAllNumber: false,
            allSelectedText: json.all_carparks
            }); 
            
        $('#days').multiselect('destroy');
        $('#days').multiselect(
            {
            buttonWidth: '100%',
            includeSelectAllOption: true,
            selectAllText: json.all_days,
            nonSelectedText:json.select_days,
            selectAllNumber: false,
            allSelectedText: json.all_days
            });      
            
            
        }); 
        
        
    }


$("#language").change(function()
{	  
    loadReportLabels();    
    callReport();		
}); 
  //////////////////////////////
  // excel export
  //////////////////////////////

$('#export_excel_report').click(function (event) 
  {
  export_to_excel("#report-content", "PMS_Revenue_Summary")  
  }); // end click event function

//////////////////////////////
// notification accordion box
//////////////////////////////

// change icon ui based on whether box is open/closed
$(document).on("click", ".notification-box .card-header", function () {
  // $('.notification-box .card-header').click(function (event) {
  $("#notification-icon").toggleClass("fa-angle-down fa-angle-up");
});

//////////////////////////////
// revenue earnings chart
//////////////////////////////

var parking_fee;
var lost_ticket_fee;
var product_sale_amount;
var vat_amount;
var currency;

function createChart() 
  {
  if($("#report-content").find('table').length!=0) 
    {    
    $("#chart_container").removeClass("d-none");
    parking_fee = 0;
    lost_ticket_fee = 0;
    product_sale_amount = 0;
    vat_amount = 0;
    getChartData();
    if (click_count === 0) 
      {            
      revenueEarningsChart();
      click_count += 1;
      } 
    else 
      {      
      updateRevenueEarningsChart()
      }
    } 
  else 
    {
    $("#chart_container").addClass("d-none");
    }
  }

function getChartData() 
  {
  parking_fee = $("#chart-data").attr('data-parking-fee');
  lost_ticket_fee = $("#chart-data").attr('data-lost-ticket');
  product_sale_amount = $("#chart-data").attr('data-product-sale');
  //vat_amount = $("#chart-data").attr('data-vat');
  currency=$("#chart-data").attr('data-currency');   
  }

function updateRevenueEarningsChart() 
  {  
  revenue_earnings_chart.data.datasets[0].data[0] = parking_fee;
  revenue_earnings_chart.data.datasets[0].data[1] = lost_ticket_fee;
  revenue_earnings_chart.data.datasets[0].data[2] = product_sale_amount;
  //revenue_earnings_chart.data.datasets[0].data[3] = vat_amount;
  revenue_earnings_chart.update();
  }

var revenue_earnings_chart

function revenueEarningsChart() 
  {
  $(document).ready(function () 
    {
    $(function () 
      {
      var default_data = 
        {
        labels: 
          [
          'Parking Fee',
          'Lost Ticket Fee',
          'Product Sales',
          //'VAT'
          ],
        datasets: 
            [
              {
              data: 
                [
                parking_fee,
                lost_ticket_fee,
                product_sale_amount,
                //vat_amount,
                ],
              borderWidth: 1.5,
              backgroundColor: 
                [
                //"rgba(40, 167, 69, 0.5)",
                "#0bc581",
                "#36bea9",
                "#3cb767",
                //"rgba(0, 123, 255, 0.5)",
                //"rgba(69, 40, 167, 0.5)",
                //"rgba(11, 79, 108, 0.5)",
                ],
            borderColor: 
              [
              "rgba(40, 167, 69, 0.8)",
              "rgba(40, 167, 69, 0.8)",
              "rgba(40, 167, 69, 0.8)",
              //"rgba(11, 79, 108, 0.8)",
              ],
            }, 
          ]
        }

      var ticksStyle = 
        {
        fontColor: '#000',
        }

        var mode = 'index'
        var intersect = true

        var $revenue_earnings_chart = $('#revenue-chart')
        revenue_earnings_chart = new Chart($revenue_earnings_chart, {
          type: 'bar',

          data: default_data,
          options: {
            layout: {
              padding: {
                // add padding so text on tallest bar doesn't get cut off
                top: 25
              }
            },
            legend: {
              display: false
            },
            maintainAspectRatio: false,
            tooltips: {
              mode: mode,
              intersect: intersect
            },
            hover: {
              mode: mode,
              intersect: intersect
            },
            scales: {
              yAxes: [{
                gridLines: {
                  display: true,
                  lineWidth: '4px',
                  color: 'rgba(0, 0, 0, .2)',
                  zeroLineColor: 'transparent'
                },
                ticks: $.extend({
                  beginAtZero: true,
                }, ticksStyle)
              }],
              xAxes: [{
                display: true,
                gridLines: {
                  display: false
                },
                ticks: ticksStyle
              }]
            },
          },
          plugins: [{
            beforeInit: function (chart) {
              chart.data.labels.forEach(function (e, i, a) {
                // add linebreak where "\n" occurs
                if (/\n/.test(e)) {
                  a[i] = e.split(/\n/);
                }
              });
            }
          }]
        })

        // Define a plugin to provide data labels
        Chart.plugins.register({
          afterDatasetsDraw: function (chart) {
            var ctx = chart.ctx;

            chart.data.datasets.forEach(function (dataset, i) {
              var meta = chart.getDatasetMeta(i);
              if (!meta.hidden) {
                meta.data.forEach(function (element, index) {
                  // Draw the text in black, with the specified font
                  ctx.fillStyle = 'rgb(0, 0, 0)';

                  var fontSize = 16;
                  var fontStyle = 'normal';
                  var fontFamily = 'Helvetica Neue';
                  ctx.font = Chart.helpers.fontString(
                    fontSize, fontStyle, fontFamily
                  );

                  // Just naively convert to string for now
                  var dataString = currency+" "  + dataset.data[
                    index].toString();

                  // Make sure alignment settings are correct
                  ctx.textAlign = 'center';
                  ctx.textBaseline = 'middle';

                  var padding = 5;
                  var position = element.tooltipPosition();
                  ctx.fillText(dataString, position.x,
                    position.y - (fontSize / 2) -
                    padding);
                });
              }
            });
          }
        });

      }) // end func

    }); // end doc ready

  } // end chart func
</script>