<?php
require_once('../../../includes/common.php');

$page_title="Application Home";

//# Import application layout.
include('../../../includes/header.php');
include('../../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">Valet Parking</div>

<?php
include('../../../includes/navbar-end.php');
include('../../../includes/sidebar.php');

//# App Function Classes
include('../../../classes/reporting_valet.php');
$reports=new reporting_valet();
?>

<!-- Modal -->
<div class="modal fade text-dark" id="detailModal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel"
  aria-hidden="true">
  <div class="modal-dialog modal-lg">
    <div class="modal-content">
      <div class="modal-body">
        <div class="card">
          <div class="card-header">
            <h3 class="card-title">Vehicle Pictures </h3>
          </div>
          <!-- /.card-header -->
          <div class="card-body p-0">
            <div id="valet_pictures">



            </div>
          </div>
          <!-- /.card-body -->
        </div>
      </div>
      <div class="modal-footer">
        <!-- <button type="button" class="btn btn-secondary" data-dismiss="modal">Export PDF</button> -->
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
      </div>
    </div>
  </div>
</div>

<div class="content-wrapper">

  <!-- additional menu -->
  <div class="additional-menu row m-0 bg-white border-bottom">
    <div class="col d-flex pl-1 align-items-center">

      <div class="flex-grow-1 row additional-menu-left">

        <!-- carparks single select -->
        <div class="col-md-2">
          <select class="form-control" id="carpark">
            <option value="0">All Carparks</option>
            <?php $reports->get_carparks()?>
          </select>
        </div>

        <!-- date and time -->
        <div class="col-md-3">
          <div class="input-group">
            <div class="input-group-prepend">
              <span class="input-group-text"><i class="far fa-clock"></i></span>
            </div>
            <input type="text" class="form-control float-right" id="reservationtime" autocomplete="off" placeholder="Choose Date and Time Range">
          </div>
        </div>

        <!-- search -->
        <div class="col-md-1">
          <button type="button" class="btn btn-block btn-secondary" id="view-report-button">Search</button>
        </div>

        <!-- loader -->
        <div class='col-1' id='loader'>
          <img src='../../../dist/img/loading.gif'>
        </div>

      </div>

      <div class="additional-menu-right">
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
  <!-- end / additional menu -->

  <section class="content">
    <div class="container-wide">

 <!-- valet summary chart -->
      <div class="d-none" id="chart_container">
        <div class="row jspdf-graph">

          <div class="col-lg-12">
            <div class="card barchart-box report-chart">
            <p class="text-center chart-header text-dark">Valet Summary</p>
              <div class="position-relative">
                <div><canvas id="valet_summary_chart" height="300"></canvas></div>
              </div>
            </div>
          </div>

          
<!--driver utilization chart-->
          <div class="col-lg-12">
            <div class="card barchart-box report-chart">
            <p class="text-center chart-header text-dark">Driver Utilization</p>
              <div class="position-relative">
                <div><canvas id="driver_utilization_chart" height="300"></canvas></div>
              </div>
            </div>
          </div>
<!--driver utilization chart-->
        </div>
      </div>
<!-- valet summary chart -->

      <div class="">
        <div class="card-body p-0" id="report-content">        
        </div>
      </div>

    </div>
    

</div>
</section>
</div>

<?php include('../../../includes/footer.php');?>

<script>
  
  $('#view-report-button').click(function (event) { // close the home page banner : control button

    var carpark = $("#carpark").val();

    if ((!daterange)) {
      alert("choose date range");
    } else {
      var data = {
        toDate: to,
        fromDate: from,
        carpark: carpark
      };
      var temp = JSON.stringify(data);
      //alert(temp);
      $.post("../../ajax/valet.php?task=1", temp)
        .done(function (result) {

          $("#report-content").html(result);
          createChart(result);

          reportSuccess();

        }, "json");
    } // end if 

    event.preventDefault();

  }); // end traffic report by day 

  $('#export_excel_report').click(function (event) {

    export_to_excel("#report-content", "Valet_Transactions")

  }); // end click event function 




  $(document).on('click', '.view-img', function () {

    var valet_id = $(this).attr('id');
    // console.log(valet_id)
    var report_data = {
      id: valet_id
    };
    var temp = JSON.stringify(report_data);

    //alert(temp);

    $.post("../../ajax/valet.php?task=3", temp)
      .done(function (result) {
        // alert(result);
        $("#valet_pictures").html(result);
        reportSuccess();
      }, "json");

    // e.preventDefault();

  }); // end click event function

  function EditValetTransaction(id) {
    var car_brand;
    var car_color;
    var car_model;
    $.ajax({
      type: "GET",
      async: false,
      url: "../../ajax/valet.php?task=5&id=" + id,
      success: function (result) {
        var obj = JSON.parse(result);
        car_brand = obj.car_brand;
        car_color = obj.car_color;
        car_model = obj.car_model;
      },
    });
    var carimage;
    var report_data = {
      id: id
    };
    var temp = JSON.stringify(report_data);
    $.ajax({
      type: "POST",
      async: false,
      url: "../../ajax/valet.php?task=6",
      data: temp,
      success: function (result) {
        carimage = result;
      },
    });

    var ticketid = $('#ticketid' + id).html();
    var valetin = $('#valetin' + id).html();
    var valetout = $('#valetout' + id).html();
    var parkingduration = $('#parkingduration' + id).html();
    var pickup = $('#pickupduration' + id).html();
    var dropoff = $('#dropoff' + id).html();
    var delivery = $('#delivery' + id).html();
    var plateno = $('#plateno' + id).html();
    var country = $('#country' + id).html();
    var emirate = $('#emirate' + id).html();
    $('#filter-content').hide();
    var htmldata = "<div class='card card-primary mb-4' id='cardimage'><div class='card-body'><div class='row'>";
    htmldata += "<table border = 0 width = 50%><tr><td>Ticket Id</td><td>" + ticketid + "</td></tr>";
    htmldata += "<tr><td>Valet In</td><td>" + valetin + "</td></tr>";
    htmldata += "<tr><td>Valet Out</td><td>" + valetout + "</td></tr>";
    htmldata += "<tr><td>Parking Duration</td><td>" + parkingduration + "</td></tr>";
    htmldata += "<tr><td>Pickup To Parking(m)</td><td>" + pickup + "</td></tr>";
    htmldata += "<tr><td>Parking To Dropoff(m)</td><td>" + dropoff + "</td></tr>";
    htmldata += "<tr><td>Dropoff To Delivery(m)</td><td>" + delivery + "</td></tr>";
    htmldata += "<tr><td>Car Images</td><td>" + carimage + "</td></tr>";
    htmldata += "<tr><td>Plate Number</td><td><input type = 'text' id = 'plateno_text" + id +
      "' class='form-control' name = 'plateno_text" + id + "'  value = '" + plateno + "' ></td></tr>";
    htmldata += "<tr><td>Country</td><td><input type = 'text' id = 'country_text" + id +
      "' class='form-control' name = 'country_text" + id + "'  value = '" + country + "' ></td></tr>";
    htmldata += "<tr><td>Emirate</td><td><input type = 'text' id = 'emirate_text" + id +
      "' class='form-control' name = 'emirate_text" + id + "'  value = '" + emirate + "' ></td></tr>";
    htmldata += "<tr><td>Car Brand</td><td><input type = 'text' id = 'carbrand_text" + id +
      "' class='form-control' name = 'carbrand_text" + id + "'  value = '" + car_brand + "' ></td></tr>";
    htmldata += "<tr><td>Car Color</td><td><input type = 'text' id = 'carcolor_text" + id +
      "' class='form-control' name = 'carcolor_text" + id + "'  value = '" + car_color + "' ></td></tr>";
    htmldata += "<tr><td>Car Model</td><td><input type = 'text' id = 'carmodel_text" + id +
      "' class='form-control' name = 'carmodel_text" + id + "'  value = '" + car_model + "' ></td></tr>";
    htmldata += "<td class='col'><input class='btn btn-danger btn-block btn-cancel-trx' id ='cancelvaletbtn" + id +
      "' type='submit'  value='Cancel' onclick = 'CancelValetTransaction()' style = 'width:30%'></td><td class='col'><input class='btn btn-info btn-block btn-save-trx' id ='savevaletbtn" +
      id + "' type='submit'  value='Save' onclick = 'SaveValetTrx(" + id + ")' id='" + id + "' ></td></tr>";
    htmldata += "</div></div></div>"
    $('#report-content').html(htmldata);


    /*  var plateno = $('#plateno'+id).html();
      var country = $('#country'+id).html();
      var emirate = $('#emirate'+id).html();
      $('#plateno'+id).html("<input type = 'text' id = 'plateno_text"+id+"' name = 'plateno_text"+id+"'  value = '"+plateno+"' >");
      $('#country'+id).html("<input type = 'text' id = 'country_text"+id+"' name = 'country_text"+id+"'  value = '"+country+"' >");
      $('#emirate'+id).html("<input type = 'text' id = 'emirate_text"+id+"' name = 'emirate_text"+id+"'  value = '"+emirate+"' >");
      $('#editvaletbtn'+id).attr('value','Save');
      $('#editvaletbtn'+id).attr('onclick','SaveValetTrx('+id+')');*/
  }

  function SaveValetTrx(id) {
    var datastring = {};
    datastring['plateno'] = $("#plateno_text" + id).val();
    datastring['country'] = $("#country_text" + id).val();
    datastring['emirate'] = $('#emirate_text' + id).val();
    datastring['carbrand'] = $("#carbrand_text" + id).val();
    datastring['carcolor'] = $("#carcolor_text" + id).val();
    datastring['carmodel'] = $('#carmodel_text' + id).val();
    if ($('#front_edit').val() == 1) {
      datastring['frontplate'] = $('#edit_image_folder').val() + "/" + $('#front_image_name').val();
    } else {
      datastring['frontplate'] = $('#db_front_image_name').val()
    }
    if ($('#right_edit').val() == 1) {
      datastring['rightplate'] = $('#edit_image_folder').val() + "/" + $('#right_image_name').val();
    } else {
      datastring['rightplate'] = $('#db_right_image_name').val()
    }
    if ($('#left_edit').val() == 1) {
      datastring['leftplate'] = $('#edit_image_folder').val() + "/" + $('#left_image_name').val();
    } else {
      datastring['leftplate'] = $('#db_left_image_name').val()
    }
    if ($('#back_edit').val() == 1) {
      datastring['backplate'] = $('#edit_image_folder').val() + "/" + $('#back_image_name').val();
    } else {
      datastring['backplate'] = $('#db_back_image_name').val()
    }
    //datastring['front_image_path']=$('#front_image_folder').val()+$('#front_image_name').val();

    var jsondata = JSON.stringify(datastring);
    $.ajax({
      type: "post",
      url: "../../ajax/valet.php?task=4&id=" + id,
      data: jsondata,
      cache: false,
      contentType: "application/json",
      success: function (data) {
        //alert(data);
        //location.reload(true);
        var carpark = $("#carpark").val();

        if ((!daterange)) {
          alert("choose date range");
        } else {
          var data = {
            toDate: to,
            fromDate: from,
            carpark: carpark
          };
          var temp = JSON.stringify(data);
          //alert(temp);
          $.post("../../ajax/valet.php?task=1", temp)
            .done(function (result) {

              $("#report-content").html(result);
              reportSuccess();
            }, "json");
        } // end if

        event.preventDefault();

      },
      error: function (jqXHR, textStatus, errorThrown) {

        alert(textStatus);
        alert(errorThrown);
      } //end of error
    }); //end of ajax*/
    event.preventDefault();
  }

  function CancelValetTransaction() {
    $('#filter-content').show();
    var carpark = $("#carpark").val();

    if ((!daterange)) {
      alert("choose date range");
    } else {
      var data = {
        toDate: to,
        fromDate: from,
        carpark: carpark
      };
      var temp = JSON.stringify(data);
      //alert(temp);

      $.post("../../ajax/valet.php?task=1", temp)
        .done(function (result) {

          $("#report-content").html(result);

          reportSuccess();

        }, "json");

    } // end if

  }


  /*function EditImage(imageposition)
  {
     $("#my_file").click(function(){alert("here")});
     //var file = this.files[0];
     //  alert("here");
       event.preventDefault();
  }*/
  $(document).ready(function () {
    $(document).on('change', '#my_file_front', function () {
      var name = document.getElementById("my_file_front").files[0].name;
      var ext = name.split('.').pop().toLowerCase();
      if (CheckImageValid("my_file_front")) {
        UploadImage("my_file_front", "front_image_name", "front_edit")
      } else {
        alert("Invalid Image");
      }
    });
    $(document).on('change', '#my_file_right', function () {
      var name = document.getElementById("my_file_right").files[0].name;
      var ext = name.split('.').pop().toLowerCase();
      if (CheckImageValid("my_file_right")) {
        UploadImage("my_file_right", "right_image_name", "right_edit")
      } else {
        alert("Invalid Image");
      }
    });
    $(document).on('change', '#my_file_left', function () {
      var name = document.getElementById("my_file_left").files[0].name;
      var ext = name.split('.').pop().toLowerCase();
      if (CheckImageValid("my_file_left")) {
        UploadImage("my_file_left", "left_image_name", "left_edit")
      } else {
        alert("Invalid Image");
      }
    });
    $(document).on('change', '#my_file_back', function () {
      var name = document.getElementById("my_file_back").files[0].name;
      var ext = name.split('.').pop().toLowerCase();
      if (CheckImageValid("my_file_back")) {
        UploadImage("my_file_back", "back_image_name", "back_edit")
      } else {
        alert("Invalid Image");
      }
    });

  });

  function CheckImageValid(fileelementid) {
    var name = document.getElementById(fileelementid).files[0].name;
    var ext = name.split('.').pop().toLowerCase();
    if (jQuery.inArray(ext, ['gif', 'png', 'jpg', 'jpeg']) == -1) {
      alert("Invalid Image File");
    }
    var oFReader = new FileReader();
    oFReader.readAsDataURL(document.getElementById(fileelementid).files[0]);
    var f = document.getElementById(fileelementid).files[0];
    var fsize = f.size || f.fileSize;
    if (fsize > 2000000) {
      alert("Image File Size is very big");
      return false;
    } else {
      return true;
    }
  }

  function UploadImage(fileelementid, imagename, editflag) {
    var form_data = new FormData();
    form_data.append("my_file", document.getElementById(fileelementid).files[0]);
    form_data.append("edit_image_folder", document.getElementById('edit_image_folder').value);
    form_data.append("image_name", document.getElementById(imagename).value);
    $.ajax({
      url: "../../ajax/valet.php?task=7",
      method: "POST",
      data: form_data,
      contentType: false,
      cache: false,
      processData: false,
      beforeSend: function () {},
      success: function (data) {
        if (data == 1) {
          $('#' + editflag).val("1");
          alert("Image uploaded successfully!!");
        } else {
          alert("Image upload failed!!");
        }
        //$("#container").css("background-image","url("+data+")");
        //$("#background").val(data);
      },
      error: function (jqXHR, textStatus, errorThrown) {

        alert(textStatus);
        alert(errorThrown);
      } //end of error
    });
  }
  //Neetu

  //////////////////////////////
  // bar chart -
  //////////////////////////////
var click_count=0;
  var minimum = [];
  var average = [];
  var maximum = [];
  var keyIn=[];
  var keyOut=[];
  var driverLabels=[];

  function createChart(result) {
    if (result.indexOf("No records/transactions available for the current search criteria") === -1) {
      // show chart when report first loads
      $("#chart_container").removeClass("d-none");
               
      minimum=[];
      maximum=[];
      average=[];
      keyIn=[];
      keyOut=[];
      driverLabels=[];
      getChartData();
      // for first click
      if (click_count === 0) {
        // load chart
        valetChart();
        driverUtilizationChart();
        click_count += 1;
      } else {
        // for all other clicks
        updateChart();
      }
    } else {
      $("#chart_container").addClass("d-none");
    }
  }


  var valetLabels = ["Pickup to Parking","Parking To Podium","Podium To Delivery"];

  function getChartData() {    

    minimum[0]= $("#chart-data").attr('data-min-pickup-parking');
    minimum[1]= $("#chart-data").attr('data-min-parking-dropoff');
    minimum[2]= $("#chart-data").attr('data-min-dropoff-delivery');

    average[0]= $("#chart-data").attr('data-avg-pickup-parking');
    average[1]= $("#chart-data").attr('data-avg-parking-dropoff');
    average[2]= $("#chart-data").attr('data-avg-dropoff-delivery');

    maximum[0]= $("#chart-data").attr('data-max-pickup-parking');
    maximum[1]= $("#chart-data").attr('data-max-parking-dropoff');
    maximum[2]= $("#chart-data").attr('data-max-dropoff-delivery');  

  //driver chart
  var n=$("#driver-chart-data").attr('data-driver-count');
  
    for (i = 1; i < n; i++) {
      var temp_var = "driver-chart-data-" + i
      var temp1 = $("#" + temp_var).attr('data-driver-name');
      var temp2 = $("#" + temp_var).attr('data-key-in');
      var temp3 = $("#" + temp_var).attr('data-key-out');
      
      driverLabels[i-1]=temp1;        
      keyIn[i-1]=temp2;
      keyOut[i-1]=temp3;     
      console.log(temp1+" "+temp2+" "+temp3);      
    }  

    for (var j in driverLabels) {
      driverLabels[j] = driverLabels[j].replace(" ", "\n");
      console.log(driverLabels[j])
    }

  }

  function updateChart() {
    // set barchart data
    valet_summary_chart.data.datasets[0].data = minimum;
    valet_summary_chart.data.datasets[1].data = average;
    valet_summary_chart.data.datasets[2].data = maximum;
    valet_summary_chart.update();

    driver_utilization_chart.data.datasets[0].data = keyIn;
    driver_utilization_chart.data.datasets[1].data = keyOut;
    driver_utilization_chart.update();
  }

  var valet_summary_chart  ;

  function valetChart() {

    $(document).ready(function () {

      $(function () {
        var default_data = {
          labels: valetLabels,
          datasets: [{
              data: minimum,
              label: 'Minimum',             
              backgroundColor: "rgba(40,167,69, 0.5)",
              borderColor: '#28a745',
              borderWidth: 1.5,
            },
            {
              data: average,
              label: 'Average',              
              backgroundColor: "rgba(0,123,255, 0.5)",
              borderColor: '#007bff',
              borderWidth: 1.5,
            },
            {
            data: maximum,
            label: 'Maximum',           
            backgroundColor: "rgba(124,124, 28, 0.5)",
            borderColor: '#28a745',
            borderWidth: 1.5,
            },
          ]
        }

        var ticksStyle = {
          fontColor: '#000',
        }

        var mode = 'index'
        var intersect = true

        var $valet_summary_chart = $('#valet_summary_chart')
        valet_summary_chart = new Chart($valet_summary_chart, {
          type: 'bar',

          data: default_data,
          options: {
            maintainAspectRatio: false,
            tooltips: {
              mode: mode,
              intersect: intersect
            },
            hover: {
              mode: mode,
              intersect: intersect
            },
            legend: {
              display: true,
              text: 'test1'
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

      })

    });

  }

  var driver_utilization_chart;

  function driverUtilizationChart() {

    $(document).ready(function () {

      $(function () {
        var default_data = {
          labels: driverLabels,
          datasets: [{
              data: keyIn,
              label: 'KeyIn',

              // transparent bar with normal border
              backgroundColor: "rgba(40,167,69, 0.5)",
              borderColor: '#28a745',
              borderWidth: 1.5,
            },
            {
              data: keyOut,
              label: 'KeyOut',

              // transparent bar with normal border
              backgroundColor: "rgba(0,123,255, 0.5)",
              borderColor: '#007bff',
              borderWidth: 1.5,
            },
            
          ]
        }

        var ticksStyle = {
          fontColor: '#000',
        }

        var mode = 'index'
        var intersect = true

        var $driver_utilization_chart = $('#driver_utilization_chart')
        driver_utilization_chart = new Chart($driver_utilization_chart, {
          type: 'bar',

          data: default_data,
          options: {
            maintainAspectRatio: false,
            tooltips: {
              mode: mode,
              intersect: intersect
            },
            hover: {
              mode: mode,
              intersect: intersect
            },
            legend: {
              display: true,
              text: 'test1'
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

      })

    });

  }
</script>