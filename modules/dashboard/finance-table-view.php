<?php 
$page_title="Finance Dashboard";

include('../../includes/header.php');
?>

<div class="navbar-has-tablink navbar-has-sublink">

<?php
include('../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">Finance</div>

<div class="row">
    <div class="col tab-header d-flex justify-content-center">
        
        <div class="tab-header-link active" data-target="table-view">Table View</div>
        <div class="tab-header-link" data-target="block-view">Block View</div>
    </div>
</div>

<?php 
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

    <div class="content-wrapper">

        <!-- tab-link header -->
        <div class="additional-menu row m-0 bg-white border-bottom">
            <div class="col d-flex justify-content-between pl-1 align-items-center">

                <div class="flex-grow-1 row additional-menu-left">
                    <div class="col tab-header d-flex justify-content-left pl-1">
                        <div class="tab-link active" data-target="all">View All</div>
                        <div class="tab-link" data-target="manual-cashier">Manual Cashiers</div>
                        <div class="tab-link" data-target="payonfoot-machine">Pay on Foot Machines</div>
                        <div class="tab-link" data-target="handheld-pos">Handheld POS</div>
                    </div>
                </div>

                <div class="additional-menu-right row align-items-center">

                    <div class="nav-details">
                        <span class="text-bold m-0">
                            Service Updated Date and Time:
                        </span>
                        <span id="last-updated">
                        </span>
                    </div>

                    <div id="action-buttons">
                        <div class="btn-group">
                            <button type="button" class="btn btn-warning">Export</button>
                            <button type="button" class="btn btn-warning dropdown-toggle" data-toggle="dropdown">
                                <span class="caret"></span>
                                <span class="sr-only">Toggle Dropdown</span>
                            </button>
                            <div class="dropdown-menu" role="menu">
                                <a class="dropdown-item" href="#" id="export_excel_report">Export to Excel</a>

                                <a class="dropdown-item" href="#" id="export_pdf_report">
                                    Export to PDF
                                </a>
                            </div>
                        </div>
                    </div>

                </div>

            </div>
        </div>
        <!-- end / tab-link header -->

        <section class="content">
            <div class="container-wide">

                <div class="col-md-12 table-view">

                    <div class="row">

                        <div class="col-md-12">

                            <!-- revenue -->
                            <div class="row jspdf-graph mb-4">

                                <!-- rev barchart -->
                                <div class="col-lg-8 pl-0 pr-3">
                                    <div class="card barchart-box mt-0 h-100">
                                        <p class="text-center chart-header text-dark">Revenue - Last 7 Days</p>
                                        <div class="">
                                            <div class="position-relative">
                                                <canvas id="revenue-7days" height="269"></canvas>
                                            </div>
                                        </div>

                                    </div>
                                </div>
                                <!-- end / rev chart -->

                                <!-- donut chart - revenue sources -->
                                <div class="col-lg-4 pr-0">
                                    <div class="card barchart-box mt-0" id="parking-duration">
                                        <p class="text-center chart-header text-dark">Revenue Sources</p>
                                        <div class="">
                                            <div class="position-relative">
                                                <canvas id="revenue_sources_chart" height="175"></canvas>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                                <!-- end / donut chart -->

                            </div>

                            <div id="live-revenue-summary-content">
                            </div> <!-- End . Live Revenue Summary Content --->

                        </div>

                    </div>


                </div>

            </div>

        </section>
    </div>
</div>

<script src="../../plugins/gauge/raphael-2.1.4.min.js"></script>
<script src="../../plugins/flot/jquery.flot.min.js"></script>
<script src="../../plugins/flot/jquery.flot.categories.min.js"></script>
<script src="../../dist/js/gauge.js"></script>


<!-- add moment.js -->
<script src="../../plugins/jquery/moment.min.js"></script>

<!-- save day_closure_start as js variable for use in graphs -->
<div id="dom-target" style="display: none;">
    <?php 
        $date = DAY_CLOSURE_START;
        echo htmlspecialchars($date); /* You have to escape because the result will not be valid HTML otherwise */
    ?>
</div>

<?php include('../../includes/footer.php');?>

<script>
function get_live_revenue_summary()
    {	
    $.get( "../ajax/dashboard.php?task=5", function( data ) 
        {  
        $('#live-revenue-summary-content').html(data);     
        $('[data-target="' + clicked_view_type + '"]').click();
        $('[data-target="' + clicked_device_type + '"]').click();
        $('#last-updated').html($("#last_updated_time").val());
        });            
    } 

/* Table Show - Hide */
var clicked_view_type = "table-view";
var clicked_device_type = "all";

$(document).ready(function () 
    {
    $('.tab-link').on('click', function () 
        {
        var $target = $(this).data('target');            
        if ($target != 'all') 
            {
            $('.card-text, .block-data').addClass("hidden");
            $('.card-text[data-status="header"]').removeClass("hidden");
            $('.card-text[data-status="' + $target + '"], .block-data[data-status="' + $target +'"]').removeClass("hidden").fadeIn('slow');
            } 
        else 
            {
            // all
            $('.card-text, .block-data').removeClass("hidden").fadeIn('slow');
            $('.card-text[data-status="header"]').removeClass("hidden").fadeIn('slow');
            }
            clicked_device_type = $target;
        });

    $('.tab-header-link').on('click', function () 
        {
        var $target = $(this).data('target');

        if ($target === 'block-view') 
            {                
            $('[data-status="table-view"]').addClass("hidden");
            $('[data-status="' + $target + '"]').removeClass("hidden").fadeIn('slow');
            } 
        else 
            {
            $('[data-status="block-view"]').addClass("hidden");
            $('[data-status="' + $target + '"]').removeClass("hidden").fadeIn('slow');
            }
            //clicked_view_type = $target;            
        });

    get_live_revenue_summary();    
    setInterval(function () 
        {             
        get_live_revenue_summary();  
        updateRevenueSources();         
        }, 1000*30);

    });

    /* Change Active Tab Styling */

    $('.tab-link').on('click', function () 
        {
        $('.tab-link').removeClass('active');
        $(this).addClass('active');
        });

    $('.tab-header-link').on('click', function () 
        {
        $('.tab-header-link').removeClass('active');
        $(this).addClass('active');
        });

    $('#export_excel_report').click(function (event) 
        {
        export_to_excel("#live-revenue-summary-content", "Live_Revenue_Report")
        }); // end click event function

/////////////////////////////
// bar chart - revenue last 7 days
//////////////////////////////

var rev_7_days;
var $result;
var revenue_data = [];

function rev7days() 
    {
    var $rev_7_days
    var revenue_amounts = {};
    var weekdays = [];

    $.get("../ajax/dashboard.php?task=4", function (data) {
        
        // get dynamic data

        /* chart variables */

        var weekdates = [];
        var date_string = [];
        var test = 0

        $result = JSON.parse(data);
        // console.log($result);

        function addDates() 
            {
            if (test <= 7) 
                {
                var temp1 = moment().subtract(test, 'days').format("MM-DD");
                weekdates.unshift(temp1);

                var temp2 = moment().subtract(test, 'days').format("ddd");
                weekdays.unshift(temp2);

                // used for comparing to $result
                var temp3 = moment().subtract(test, 'days').format("YYYY-MM-DD");
                date_string.unshift(temp3);

                test += 1;

                addDates();
                }
            }

        addDates()
        

        // create label

        var daylabels = [];        
        for (var i in weekdays) 
            {
            if (i < 7) 
                {
                daylabels[i] = (weekdays[i] + "\n" + weekdates[i]);
                } 
            else 
                {
                daylabels[i] = ("Today" + "\n" + weekdays[i] + " " + weekdates[i]);
                }
            }

        // console.log(weekdates);

        // put revenue data in array
        for (var i in date_string) {
            for (var j in $result) {
                if (date_string[i] === $result[j].date) {
                    revenue_data[i] = $result[j].amount;                   
                }
            }
        }

        // console.log("revenue_data = " + revenue_data);

        $(function () {
            'use strict'

            var ticksStyle = {
                fontColor: '#000',
                // fontStyle: 'bold'
            }

            var mode = 'index'
            var intersect = true

            // console.log("revd = " + revenue_data);

            $rev_7_days = $('#revenue-7days')
            rev_7_days = new Chart($rev_7_days, {
                type: 'bar',
                // type: 'horizontalBar',
                data: {
                    labels: daylabels,
                    datasets: [{
                        data: revenue_data,
                        label: 'Revenue',

                        // transparent bar with normal border
                        // backgroundColor: "rgba(40,167,69, 0.5)",
                        // borderColor: '#28a745',

                        backgroundColor: [
                            "rgba(40,167,69, 0.5)",
                            "rgba(40,167,69, 0.5)",
                            "rgba(40,167,69, 0.5)",
                            "rgba(40,167,69, 0.5)",
                            "rgba(40,167,69, 0.5)",
                            "rgba(40,167,69, 0.5)",
                            "rgba(40,167,69, 0.5)",
                            "rgba(69,40,167, 0.5)"
                        ],

                        borderColor: [
                            '#28a745',
                            '#28a745',
                            '#28a745',
                            '#28a745',
                            '#28a745',
                            '#28a745',
                            '#28a745',
                            '#4528a7',
                        ],

                        borderWidth: 2,
                    }]
                },
                options: {
                    maintainAspectRatio: false,
                    responsive: true,
                    tooltips: {
                        mode: mode,
                        intersect: intersect
                    },
                    hover: {
                        mode: mode,
                        intersect: intersect
                    },
                    legend: {
                        display: false
                    },
                    scales: {
                        yAxes: [{
                            // display: false,
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
                            ticks: $.extend({
                                beginAtZero: true,
                            }, ticksStyle)
                        }]
                    },
                    animation: {
                        duration: 1,                       
                    }
                },
                // plugin to add line break for long label
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

        })

    });
    }

//////////////////////////////
// donut chart - revenue sources
//////////////////////////////

var config
var ctx
var pieChart
var pieValues

function revenueSources() 
    {        
    var pieChartCanvas = $("#revenue_sources_chart").get(0).getContext("2d");
    pieChart = new Chart(pieChartCanvas, {
        type: 'doughnut',
        data: {
            datasets: [{
                data: [],
                backgroundColor: [
                    "rgba(40, 167, 69, 0.5)",
                    "rgba(0, 123, 255, 0.5)",
                    "rgba(113, 128, 172, 0.5)"
                ],
                borderColor: [
                    "#28a745",
                    "#007bff",
                    "#7180ac"
                ],
                label: 'Dataset 1'
            }],
            labels: [
                'Parking Fee',
                'Lost Fee',
                'Product Sale'
            ]
        },
        options: {
            responsive: true,
            legend: {
                position: 'top',
            },
            animation: {
                animateScale: true,
                animateRotate: true
            }
        }
    })

    // get dynamic data

    var parking_fee, vat_amount, lost_fee, amount;
    pieValues = pieChart.data.datasets[0].data

    updateRevenueSources();

    // edits to add margin between legend and chart

    function getBoxWidth(labelOpts, fontSize) 
        {
        return labelOpts.usePointStyle ?
            fontSize * Math.SQRT2 :
            labelOpts.boxWidth;
        };

    Chart.NewLegend = Chart.Legend.extend(
        {
        afterFit: function () {
            this.height = this.height + 10;
        },
    });

function createNewLegendAndAttach(chartInstance, legendOpts) 
    {
    var legend = new Chart.NewLegend(
        {
        ctx: chartInstance.chart.ctx,
        options: legendOpts,
        chart: chartInstance
        });

    if (chartInstance.legend) 
        {
        Chart.layoutService.removeBox(chartInstance, chartInstance.legend);
        delete chartInstance.newLegend;
        }

    chartInstance.newLegend = legend;
    Chart.layoutService.addBox(chartInstance, legend);
    }

    // Register the legend plugin
    Chart.plugins.register({
        beforeInit: function (chartInstance) {
            var legendOpts = chartInstance.options.legend;

            if (legendOpts) {
                createNewLegendAndAttach(chartInstance, legendOpts);
            }
        },
        beforeUpdate: function (chartInstance) {
            var legendOpts = chartInstance.options.legend;

            if (legendOpts) {
                legendOpts = Chart.helpers.configMerge(Chart.defaults.global.legend, legendOpts);

                if (chartInstance.newLegend) {
                    chartInstance.newLegend.options = legendOpts;
                } else {
                    createNewLegendAndAttach(chartInstance, legendOpts);
                }
            } else {
                Chart.layoutService.removeBox(chartInstance, chartInstance.newLegend);
                delete chartInstance.newLegend;
            }
        },
        afterEvent: function (chartInstance, e) {
            var legend = chartInstance.newLegend;
            if (legend) {
                legend.handleEvent(e);
            }
        }
    });

    }

function updateRevenueSources() 
    {
    $.get("../ajax/dashboard.php?task=10", function (data) 
        {
        amount = JSON.parse(data);
        pieValues[0] = amount["parking_fee"];
        pieValues[1] = amount["lost_fee"];
        pieValues[2] = amount["product_sale_amount"];        
        pieChart.update();
        //update barchart
        revenue_data[7]=amount["gross_amount"];
        rev_7_days.update();
        });
    }

rev7days();
revenueSources();
</script>