<?php 
include('../../includes/header.php');
?>

<div class="navbar-has-tablink navbar-has-sublink">

<?php
include('../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">Finance</div>

<?php 
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

    <div class="content-wrapper">
        <section class="content">
            <div class="container-wide">
                <div class="row" id="revenue-live-block" >

                </div> 
            </div>
        </section>
    </div>
</div>

<?php include('../../includes/footer.php');?>

<script>
    
//////////////////////////////
// donut chart - revenue sources
//////////////////////////////

var donutChart=[];
var donutChartValues=[];
var facilityNumber=[];
var facility_number=0;
var carpark_number=0;
var amount;


get_live_revenue();

function get_live_revenue()
    {
    $.get("../ajax/dashboard.php?task=27", function (data) {        
        $('#revenue-live-block').html(data);  
        carpark_number=$("#carpark_number").val();
        facility_number=$("#facility_number").val();
        if (typeof carpark_number !== "undefined")
            window.location="finance_device.php?facility_number="+facility_number+"&carpark_number="+carpark_number;
        else if (typeof facility_number !== "undefined")  
            window.location="finance_carpark.php?facility_number="+facility_number;
        else
            show_donut_chart();
    });
    }
    
$(document).on('click', '.show-facility-details', function () {
    facility_number = $(this).attr("facility_number");  
    window.location="finance_carpark.php?facility_number="+facility_number;
});

function show_donut_chart()
    {        
    $(".donutChart").each(function(index){   
        var donutData        = {
            labels: [
                'Parking Fee',           
                'Lost Fee',                   
                'Product Sale',
                'VAT',

            ],
            datasets: [
              {
                data: [],
                backgroundColor : ['#00a65a','#00c0ef','#f56954', '#f39c12'],
              }
            ]
          }
        var donutOptions     = {
          maintainAspectRatio : false,
          responsive : true,
        }
        var donutChartCanvas = $(this).get(0).getContext('2d');
        facilityNumber[index]=$(this).attr("facility-number");
        donutChart[index] = new Chart(donutChartCanvas, {
        type: 'doughnut',
        data: donutData,
        options: donutOptions      
      });
    
    donutChartValues[index] = donutChart[index].data.datasets[0].data;     
    });
    updateRevenueSources() ;  
    }

function updateRevenueSources() 
    {
    $.get("../ajax/dashboard.php?task=10", function (data) 
        {           
        amount = JSON.parse(data);        
        for(var i=0;i<amount.length;i++)
            {              
            var index = facilityNumber.indexOf(amount[i]["facility_number"]);              
            donutChartValues[index][0] = amount[i]["parking_fee"];            
            donutChartValues[index][1] = amount[i]["lost_fee"];
            donutChartValues[index][2] = amount[i]["product_sale_amount"];             
            donutChartValues[index][3] = amount[i]["vat_amount"];             
            donutChart[index].update();        
            }
        });
    }

</script>