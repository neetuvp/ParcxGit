<?php include('includes/header.php');?>
<div class="header text-dark" id="pdf-report-header">Validator Details</div>
<?php include('includes/sidebar.php');?>
<script>document.title = "Parcx Cloud | Details";</script>

<div class="content-wrapper desktop-container">
    <section class="content">
        <div class="container-wide">

            <input type="hidden" id="validator_id" value="<?php echo $_SESSION["validator_id"]?>">

            <!-- info boxes -->
            <div class="row mb-4 jspdf-graph validator-details" id="validator-details">           
            </div>         

            <!-- start graphs -->
            <div class="row jspdf-graph mb-4">
                <div class="col-lg-12">
                    <div class="card barchart-box m-0">
                        <p class="chart-header">Hourly Validations</p>
                        <div class="position-relative">
                            <canvas id="overview-graph" height="300"></canvas>
                        </div>
                    </div>
                </div>
            </div>      

            <!-- table -->
            <h3 class="header p-0 mb-3 mt-4">Today's Validations</h3>
            <div class="table-responsive card card-table-blue" id="today-validation"></div>

        </div>
    </section>
</div>

<!-- page specific scripts -->

<script src="dist/js/chartjs/chart.min.js"></script>
<script src="dist/js/chartjs/custom-graphs.js"></script>

<script>

    /* === page load / refresh === */

    function loadPage(){
        var data={};        
        data["option-type"]=17;
        data["validator_id"]=$("#validator_id").val();
        var jsondata = JSON.stringify(data);          
           $.post("ajax/system_user.php",jsondata,function(data){
			   
            if(data=="200")
                    window.location.href="index.php";
            else                                                 
                {
                $("#validator-details").html(data);  
               // tableHeight();                    
                }
            })
            .fail(function(jqxhr,status,error){
                alert("Error: "+error);
            }); 

        data["option-type"]=18;
        data["validator_id"]=$("#validator_id").val();
        var jsondata = JSON.stringify(data);    
        $.post("ajax/system_user.php",jsondata,function(data){                         
            $("#today-validation").html(data);   
        })
        .fail(function(jqxhr,status,error){
            alert("Error: "+error);
        }); 
    }

    loadPage();   

    setInterval(function () {   
        loadPage();       
        updateChartData();       
    }, 1000*15);
    
    /* === adjust table height when page size changes === */

    function tableHeight() {        
        var height = $("#account-details").outerHeight();               
        if($("#sub-account-table").height()>=height) {
            $("#sub-account-table").height($("#account-details").height() + 31);               
        }         
    }    

    $(window).resize(function(){
       tableHeight()
    });

    /* === charts === */

    // random test data
    function randomIntFromInterval(min, max) {
        return Math.floor(Math.random() * (max - min + 1) + min);
    }

    // hours_start and hours_end
    var hours_start = ['0:00', '1:00', '2:00', '3:00', '4:00', '5:00', '6:00', '7:00', '8:00', '9:00', '10:00', '11:00', '12:00', '13:00', '14:00', '15:00', '16:00', '17:00', '18:00', '19:00', '20:00', '21:00', '22:00', '23:00'];
    var hours_end = [];

    /* push values to hours_end based on hours_start */

    var i;

    for (i = 0; i < hours_start.length; ++i) {
        // if last value, create string using first value
        if (i != (hours_start.length - 1)) {
            var j;
            j = i + 1;
            var string_value = hours_start[j];
            hours_end.push(string_value);
        } else {
            var string_value = hours_start[0];
            hours_end.push(string_value);
        }
    }

    /* === bar chart === */

    function get_hourly_validations(){
        var maxdata=[];
        var data={};               
        data["validator_id"]=$("#validator_id").val();
        data["option-type"]=19;
        var jsondata = JSON.stringify(data);                              
        $.post("ajax/system_user.php",jsondata,function(data){  			
            var json=JSON.parse(data);       
            for (var i = 0; i < 24; i++) {
                maxdata.push(json[i]);                               
                }              
            })
        .fail(function(jqxhr,status,error){
            alert("Error: "+error);
        });        
        return maxdata;
    }

    function get_average_hourly_validations(){
        var maxdata=[];
        var data={};               
        data["validator_id"]=$("#validator_id").val();
        data["option-type"]=20;
        var jsondata = JSON.stringify(data);                       
        $.post("ajax/system_user.php",jsondata,function(data){  		
            var json=JSON.parse(data);       
            for (var i = 0; i < 24; i++) {
                maxdata.push(json[i]);                               
            }                                                                 
        })
        .fail(function(jqxhr,status,error) {
            alert("Error: "+error);
        });        
        return maxdata;
    }
    
    var overviewGraph, linedata, maxdata;  

    function getChartData() {
        linedata = get_average_hourly_validations(); 
        maxdata = get_hourly_validations();          
    }
    
    function updateChartData() {           
        overviewGraph.data.datasets[0].data = get_hourly_validations();
        overviewGraph.data.datasets[1].data = get_average_hourly_validations(); 
        setTimeout(() => {
            overviewGraph.update();
        }, 100); 
    }
    
    getChartData();    

    function overviewGraph() {
        $(document).ready(function () {
        
            $(function () {
                'use strict'
                var hours_label = [];

                for (var i = 0; i < hours_start.length; i++) {
                    hours_label[i] = hours_start[i] + "\n" + hours_end[i];
                }

                var default_data = {
                    labels: hours_label,
                    datasets: [{
                        data: maxdata,
                        label: 'No. of Validations',
                        backgroundColor: "rgba(0,123,255, 0.5)",
                        borderColor: '#007bff',
                        borderWidth: 1.5,
                    },
                    {
                        type: 'line',
                        label: "Last week Average",
                        backgroundColor: "rgba(40,167,69, 0.5)",
                        borderColor: '#28a745',
                        borderWidth: 2,
                        fill: false,
                        data: linedata,
                    }]
                }

                var ticksStyle = { fontColor: '#000',}
                var mode = 'index';
                var intersect = true;
                var $overviewGraph = $('#overview-graph');
                overviewGraph = new Chart($overviewGraph, {
                    type: 'bar',
                    data: default_data,
                    options: {
                        maintainAspectRatio: false,
                        tooltips: { mode: mode, intersect: intersect },
                        hover: { mode: mode, intersect: intersect },
                        legend: { display: true, text: 'test1' },
                        scales: {
                            yAxes: [{
                                gridLines: {
                                    display: true,
                                    lineWidth: '4px',
                                    color: 'rgba(0, 0, 0, .2)',
                                    zeroLineColor: 'transparent'
                                },
                                ticks: $.extend({ beginAtZero: true, }, ticksStyle)
                            }],
                            xAxes: [{
                                display: true,
                                gridLines: { display: false },
                                ticks: ticksStyle
                            }]
                        },
                    },
                    plugins: [{ beforeInit: function (chart) {
                        chart.data.labels.forEach(function (e, i, a) {
                            // add linebreak where "\n" occurs
                            if (/\n/.test(e)) {
                                a[i] = e.split(/\n/);
                            }
                        });
                    }}]
                })
            })
        });
    }

    setTimeout(() => {
        overviewGraph();
    }, 100); 

    function LoadDeviceData() {

        var overviewVMS

        function updateoverviewVMS() {
            // reset barchart data
            overviewVMS.data.datasets[0].data = Array.from({
                length: 24
            }, () => randomIntFromInterval(0, 100));

            // reset linechart data
            overviewVMS.data.datasets[1].data = Array.from({
                length: 24
            }, () => randomIntFromInterval(0, 100));

            overviewVMS.update();
        }

        function overviewVMS() {
            $(document).ready(function () {

                var tempdata = [];
                tempdata = Array.from({
                    length: 24
                }, () => randomIntFromInterval(0, 100));
                
                $(function () {
                    'use strict'
                    var hours_label = [];
                    for (var i = 0; i < hours_start.length; i++) {
                        hours_label[i] = hours_start[i] + "\n" + hours_end[i];
                    }

                    var vms_data = {
                        labels: hours_label,
                        datasets: [{
                            data: tempdata,
                            label: 'No. of Validations',
                            backgroundColor: "rgba(0,123,255, 0.5)",
                            borderColor: '#007bff',
                            borderWidth: 1.5,
                        }, {
                            type: 'line',
                            label: "Average",
                            borderColor: '#28a745',
                            borderWidth: 2,
                            fill: false,
                            data: Array.from({ length: 24 }, () => randomIntFromInterval(0, 100))
                        }]
                    }

                    var ticksStyle = { fontColor: '#000', }
                    var mode = 'index';
                    var intersect = true;
                    var $overviewVMS = $('#overview-vms');
                    overviewVMS = new Chart($overviewVMS, {
                        type: 'bar',
                        data: vms_data,
                        options: {
                            maintainAspectRatio: false,
                            tooltips: { mode: mode, intersect: intersect },
                            hover: { mode: mode, intersect: intersect },
                            legend: { display: true, text: 'test1' },
                            scales: {
                                yAxes: [{
                                    gridLines: {
                                        display: true,
                                        lineWidth: '4px',
                                        color: 'rgba(0, 0, 0, .2)',
                                        zeroLineColor: 'transparent'
                                    },
                                    ticks: $.extend({ beginAtZero: true, }, ticksStyle)
                                }],
                                xAxes: [{
                                    display: true,
                                    gridLines: { display: false },
                                    ticks: ticksStyle
                                }]
                            },
                        },
                        plugins: [{
                            beforeInit: function (chart) {
                                chart.data.labels.forEach(function (e, i, a) {
                                    // add linebreak where "\n" occurs
                                    if (/\n/.test(e)) { a[i] = e.split(/\n/); }
                                });
                            }
                        }]
                    })
                })
            });
        }

        overviewVMS();
    }

</script>

<?php include('includes/footer.php');?>