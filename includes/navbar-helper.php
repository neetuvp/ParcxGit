<!-- navbar helper file with all report selection options -->

<!-- top part -->

<?php 
$page_title="Finance Dashboard";

include('../../includes/header.php');
include('../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">Finance</div>

<?php 
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

<!-- within content-wrapper -->

<!-- additional menu -->
<div class="additional-menu row m-0 bg-white border-bottom">
    <div class="col d-flex pl-1 align-items-center">

        <div class="flex-grow-1 row additional-menu-left">

            <!-- date and time -->
            <div class="col-md-3">
                <div class="input-group">
                    <div class="input-group-prepend">
                        <span class="input-group-text"><i class="far fa-clock"></i></span>
                    </div>
                    <input type="text" class="form-control float-right" id="reservationtime" autocomplete="off"
                        placeholder="Choose Date and Time Range">
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

<!-- header elements -->

<!-- operator -->
<div class="col-md-2">
    <select class="form-control" id="operator">
        <?php $reports->get_operators();?>
    </select>
</div>

<!-- payment devices -->
<div class="col-md-2">
    <select class="form-control" id="deviceNumber">
        <?php $reports->get_payment_devices();?>
    </select>
</div>

<!-- payment devices multiselect-->
<div class="col-md-2">
    <select class="form-control" id="deviceNumber" multiple="multiple">
        <?php $reports->get_payment_devices();?>
    </select>
</div>

<!-- carparks single select -->
<div class="col-md-2">
    <select class="form-control" id="carpark">
        <option value="0">All Carparks</option>
        <?php $reports->get_carparks()?>
    </select>
</div>

<!-- tag serial -->
<div class="col-md-2">
    <input type="text" id="serial" class="form-control" placeholder="TAG SERIAL">
</div>

<!-- payment type single select -->
<div class="col-md-2">
    <select class="form-control" id="paymentCategory">
        <option value=0>All Payment Types</option>
        <option value=1>Cash</option>
        <option value=2>Credit Card</option>
        <option value=3>Discount</option>
        <!-- <option value=4>Lost Ticket</option>-->
        <option value=5>Grace Period</option>
    </select>
</div>

<!-- carparks multiselect -->
<div class="col-md-2">
    <select class="form-control" id="multiselect" multiple="multiple">
        <?php $reports->get_carparks();?>
    </select>
</div>

<!-- payment type multiple select -->
<div class="col-md-2">
    <select class="form-control" id="paymentCategory" multiple="multiple">
        <option value=0>All Payment Types</option>
        <option value=1>Cash</option>
        <option value=2>Credit Card</option>
        <option value=3>Discount</option>
        <!-- <option value=4>Lost Ticket</option>-->
        <option value=5>Grace Period</option>
    </select>
</div>

<!-- discounts -->
<div class="col-md-2">
    <select class="form-control" id="discounts">
        <?php $reports->get_discounts();?>
    </select>
</div>

<!-- discounts multiple -->
<div class="col-md-2">
    <select class="form-control" id="discounts" multiple="multiple">
        <?php $reports->get_discounts();?>
    </select>
</div>

<!-- operation methods -->
<div class="col-md-2">
    <select class="form-control" id="operation">
        <option value="0">All Operation Methods</option>
        <option value="cashier">Manual Open from Cashier</option>
        <option value="pushbutton">Remote/Push Button Open</option>
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

<!--weekdays -->
<div class="col-md-2">
    <select id="days" multiple="multiple" class="weekdays">
        <option value="Sunday">Sunday</option>
        <option value="Monday">Monday</option>
        <option value="Tuesday">Tuesday</option>
        <option value="Wednesday">Wednesday</option>
        <option value="Thursday">Thursday</option>
        <option value="Friday">Friday</option>
        <option value="Saturday">Saturday</option>
    </select>
</div>

<!-- tab-link header -->
<div class="additional-menu row m-0 bg-white border-bottom">
    <div class="col d-flex justify-content-between pl-1 align-items-center">

        <div class="flex-grow-1 row additional-menu-left">
            <div class="col tab-header d-flex justify-content-left pl-1">
                <div class="tab-link active" data-target="all">View All</div>
                <div class="tab-link" data-target="payment_machines">Payment Machines</div>
                <div class="tab-link" data-target="columns">Entry/Exit Columns</div>
                <div class="tab-link" data-target="controllers">Controllers</div>
            </div>
        </div>

        <div class="additional-menu-right row align-items-center">

            <div class="nav-details">
                <span class="text-bold m-0">
                    Current Date and Time:
                </span>
                <span id="current_date">
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

<script>

    // multiselects are initialized in footer.php

</script>