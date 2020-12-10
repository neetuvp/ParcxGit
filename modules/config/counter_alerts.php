<?php
include('../../includes/header.php');
include('../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">Counter Alerts</div>

<?php
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

<div class="content-wrapper">
    <section class="content">
        <div class="container-wide">
            
            <div class="card card-primary mb-4">
                <section class="content">
                    <div class="container-wide">
                        <!--Alert Module -->
                        <div class="row">
                            <div class="alert-container col-md-4">
                                <span>Type:</span>
                                <input type="radio" name="type" value="condition" id="condition" class="inline-form">
                                <span class="inline-form-text">Condition</span>
                                <input type="radio" name="type" value="TimeInterval" id="TimeInterval" class="inline-form">
                                <span class="inline-form-text">Time interval</span>
                                <input type="radio" name="type" value="NetworkStatus" id="NetworkStatus" class="inline-form">
                                <span class="inline-form-text">Network status</span>
                            </div>
                        </div>
                        <br>

                        <div class="row" id="containerCondition">
                            <div class="col-6 form-box">

                                <div id="divOption">

                                    <div id="divCondition">
                                        <div class="form-group row">
                                            <label for="inputPassword" class="col-4 col-form-label">Counter Name:</label>
                                            <div class="col-8">
                                                <select type="password" class="form-control" id='counterList'>
                                                    <?php
                                                    counterNames_dropdown();
                                                 ?>
                                                </select>
                                            </div>
                                        </div>
                                        <div class="form-group row">
                                            <label for="inputPassword" class="col-4 col-form-label">Condition:</label>
                                            <div class="col-8">
                                                <select type="password" class="form-control" id="conditionList">
                                                    <option value="=">= (Equals)</option>
                                                    <option value="!=">!= (Not equals)</option>
                                                    <option value=">">> (Greater than)</option>
                                                    <option value="<">
                                                        < (Less than)</option> <option value=">=">>= (Greater than or
                                                            equals)
                                                    </option>
                                                    <option value="<=">
                                                        <=( Less than or equals)</option> <option value="between">between
                                                    </option>
                                                </select>
                                            </div>
                                        </div>

                                        <div class="form-group">
                                            <div class="row">
                                                <label for="inputPassword" class="col-4 col-form-label">Value:</label>
                                                <div class="col">
                                                    <input type="text" class="form-control" name="txtvaluefrom" id="value1">
                                                </div>
                                                <div class="col" id="value2">
                                                    <input type="text" class="form-control" name="txtvalueto">
                                                </div>
                                            </div>
                                        </div>

                                    </div>

                                    <div id="timeControl">

                                        <div class="form-group">
                                            <div class="row">
                                                <div class="col-4">
                                                    <input type="radio" name="interval" value="everyHour" id="everyHour"
                                                        checked="checked">
                                                    <label class="col-form-label">Every Hour:</label>
                                                    <!-- <span class="inline-form-text">Every Hour</span> -->
                                                </div>
                                                <div class="col">
                                                    <select id="hourSelect" class="form-control">
                                                        <?php for ($i = 0; $i <= 24; $i++) {echo ("<option value='$i'>$i</option>");}?>
                                                    </select>
                                                </div>
                                            </div>
                                        </div>

                                        <div class="form-group">
                                            <div class="row">
                                                <div class="col-4">
                                                    <input type="radio" name="interval" value="everyMinute" id="everyMinute">
                                                    <label class="col-form-label">Every Minute:</label>
                                                    <!-- <input type="radio" name="interval" value="everyHour" id="everyHour" checked="checked" class="inline-form">
                                                <span class="inline-form-text">Every Minute</span> -->
                                                </div>
                                                <div class="col">
                                                    <select id="minuteSelect" class="form-control">
                                                        <?php for ($i = 0; $i <= 60; $i = $i + 5) {echo ("<option value='$i'>$i</option>");}?>
                                                    </select>
                                                    <select id="minuteSelect2" class="form-control">
                                                        <option value="0">0</option>
                                                        <option value="15">15</option>
                                                        <option value="30">30</option>
                                                    </select>
                                                </div>
                                            </div>
                                        </div>

                                        <div class="form-group">
                                            <div class="row">
                                                <label for="inputPassword" class="col-4 col-form-label">Start date
                                                    time:</label>
                                                <div class="col">
                                                    <input type="date" class="form-control" id="datePicker" value="<?php echo date("
                                                        Y-m-d");?>"/>
                                                </div>
                                                <div class="col">
                                                    <input type="time" class="form-control" value="<?php echo date("
                                                        H:i");?>"
                                                    id="timePicker"/>
                                                </div>
                                            </div>
                                        </div>

                                        <br>
                                        <div>
                                            <h4>Alert choice</h4>
                                            <input type="checkbox" name="email" value="email" id="chkEmail" checked="true"
                                                class="inline-form">
                                            <span class="inline-form-text">Email</span>
                                            <input type="checkbox" name="sms" value="sms" id="chkSms" class="inline-form">
                                            <span class="inline-form-text">SMS</span>
                                        </div>
                                        <br>
                                        <br>

                                        <div id="divEmailSms">
                                            <div id="divEmail">
                                                <h5>Email Addresses</h5>
                                                <div class="row">
                                                    <div class="col">
                                                        <input type="text" id="email1" class="form-control" placeholder="name@example.com" />
                                                    </div>
                                                    <div class="col">
                                                        <input type="text" id="email2" class="form-control" placeholder="name@example.com" />
                                                    </div>
                                                </div>
                                                <br>
                                                <div class="row">
                                                    <div class="col">
                                                        <input type="text" id="email3" class="form-control" placeholder="name@example.com" />
                                                    </div>
                                                    <div class="col">
                                                        <input type="text" id="email4" class="form-control" placeholder="name@example.com" />
                                                    </div>
                                                </div>
                                                <br>
                                                <div class="row">
                                                    <div class="col">
                                                        <input type="text" id="email5" class="form-control" placeholder="name@example.com" />
                                                    </div>
                                                    <div class="col">
                                                        <input type="text" id="email6" class="form-control" placeholder="name@example.com" />
                                                    </div>
                                                </div>
                                                <br>
                                                <br>
                                            </div>
                                            <div id="divMobile">
                                                <h5>Mobile Numbers</h5>
                                                <div class="row">
                                                    <div class="col">
                                                        <input type="text" id="mobile1" class="form-control"
                                                            placeholder="+97150 0000 000" />
                                                    </div>
                                                    <div class="col">
                                                        <input type="text" id="mobile2" class="form-control"
                                                            placeholder="+97150 0000 000" />
                                                    </div>
                                                </div>
                                                <br>
                                                <div class="row">
                                                    <div class="col">
                                                        <input type="text" id="mobile3" class="form-control"
                                                            placeholder="+97150 0000 000" />
                                                    </div>
                                                    <div class="col">
                                                        <input type="text" id="mobile4" class="form-control"
                                                            placeholder="+97150 0000 000" />
                                                    </div>
                                                </div>
                                                <br>
                                                <div class="row">
                                                    <div class="col">
                                                        <input type="text" id="mobile5" class="form-control"
                                                            placeholder="+97150 0000 000" />
                                                    </div>
                                                    <div class="col">
                                                        <input type="text" id="mobile6" class="form-control"
                                                            placeholder="+97150 0000 000" />
                                                    </div>
                                                </div>
                                                <br>
                                            </div>
                                            <input type="submit" value="Save" id="saveButton" class="btn btn-dark" />
                                        </div>
                                    </div>
                                </div>
                            </div>

                            <div class="col-6 form-box">
                                <div id="messgeData">
                                    <h4>Data in Message</h4>
                                    <div id="left">
                                        <?php
                                        counterNames_draggable();
                                        ?>
                                    </div>
                                    <div id="middle">
                                        <p>>></p>
                                    </div>
                                    <div id="right"></div>
                                </div>
                            </div>
                        </div>

                        <!-- /.row (main row) -->
                    </div>
                    <!-- /.container-fluid -->
                </section>
                <!-- /.content -->
            </div>
            <!-- /.content-wrapper -->
            <?php
include '../../includes/footer.php';
?>