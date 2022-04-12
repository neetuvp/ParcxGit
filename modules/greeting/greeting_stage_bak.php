<?php
include('../../includes/header.php');
?>

<div class="navbar-has-tablink">

    <?php
    include('../../includes/navbar-start.php');
    ?>


</ul>
<div class="header text-dark" id="pdf-report-header">Demo</div>

<?php
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>
<html lang="en">
    <head>
        <meta charset="utf-8">
        <!--  This file has been downloaded from bootdey.com @bootdey on twitter -->
        <!--  All snippets are MIT license http://bootdey.com/license -->
        <title>tickets timeline - Bootdey.com</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">

        <link href="css/bootstrap.min.css" rel="stylesheet">
        <link href="css/parcx.css" rel="stylesheet">
        <script src="js/bootstrap.min.js"></script>
        <script src="js/lottie-player.js"></script>

    </head>
    <body>
        <link rel="stylesheet" href="css/font-awesome.min.css">
        <!--<link href="https://maxcdn.bootstrapcdn.com/font-awesome/4.3.0/css/font-awesome.min.css" rel="stylesheet">-->
        <div class="content-wrapper">
            <!-- Content Header (Page header) -->
            <div class="content-header">
                <div class="container-fluid">
                    <div class="row mb-2">
                        <div class="col-sm-6">
                            <!--<h1 class="m-0">Dashboard</h1>-->
                        </div><!-- /.col -->          
                    </div><!-- /.row -->
                </div><!-- /.container-fluid -->
            </div>
            <!-- /.content-header -->
            <section class="content">
                <div class="container-fluid">
                    <div class="container bootstrap snippets bootdeys">
                        <div class="col-md-9">
                            <div class="timeline-centered timeline-sm">
                                <article class="timeline-entry">
                                    <div class="timeline-entry-inner">
                                        
                                        <div class="dropdown">
                                            <div class="timeline-icon bg-violet stage"><i class="fa fa-exclamation"></i>
                                                <div class="dropdown-content">
                                                    <div class="btn btn-block btn-default edit-btn" data-id="1">Edit</div>
                                                    <div class="btn btn-block btn-default preview-btn" data-id="1">Preview</div>
                                                        <!--<input type="hidden" id="stage-id" value="1">-->
                                                </div>
                                            </div>
                                        </div>
                                        <div class="timeline-label"><h4 class="timeline-title" id="title-1">Standby</h4>

                                            <p></p></div>
                                    </div>
                                </article>
                                <article class="timeline-entry left-aligned">
                                    <div class="timeline-entry-inner">
                                        
                                        <div class="dropdown">
                                            <div class="timeline-icon bg-green stage"><i class="fa fa-car"></i>
                                                <div class="dropdown-content">
                                                    <div class="btn btn-block btn-default edit-btn" data-id="2">Edit</div>
                                                    <div class="btn btn-block btn-default preview-btn" data-id="2">Preview</div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="timeline-label bg-green"><h4 class="timeline-title" id="title-2">Vehicle Arrives</h4>

                                            <p></p></div>
                                    </div>
                                </article>
                                <article class="timeline-entry">
                                    <div class="timeline-entry-inner">
                                        
                                        <div class="dropdown">
                                            <div class="timeline-icon bg-orange stage"><i class="fa fa-paper-plane"></i>
                                                <div class="dropdown-content">
                                                    <div class="btn btn-block btn-default edit-btn" data-id="3">Edit</div>
                                                    <div class="btn btn-block btn-default preview-btn" data-id="3">Preview</div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="timeline-label bg-orange"><h4 class="timeline-title" id="title-3">App Promotion</h4>

                                            <p></p></div>
                                    </div>
                                    
                                </article>
                                <article class="timeline-entry left-aligned">
                                    <div class="timeline-entry-inner">
                                        
                                        <div class="dropdown">
                                            <div class="timeline-icon bg-red stage"><i class="fa fa-arrow-up"></i>
                                                <div class="dropdown-content">
                                                    <div class="btn btn-block btn-default edit-btn" data-id="4">Edit</div>
                                                    <div class="btn btn-block btn-default preview-btn" data-id="4">Preview</div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="timeline-label bg-red"><h4 class="timeline-title" id="title-4">Fueling Started</h4>

                                            <p></p></div>
                                    </div>
                                </article>
                                <article class="timeline-entry">
                                    <div class="timeline-entry-inner">
                                        
                                        <div class="dropdown">
                                            <div class="timeline-icon bg-pink stage"><i class="fa fa-paper-plane"></i>
                                                <div class="dropdown-content">
                                                    <div class="btn btn-block btn-default edit-btn" data-id="5">Edit</div>
                                                    <div class="btn btn-block btn-default preview-btn" data-id="5">Preview</div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="timeline-label bg-pink"><h4 class="timeline-title" id="title-5">Advertising</h4>

                                            <p></p></div>
                                    </div>
                                    
                                </article>
                                <article class="timeline-entry left-aligned">
                                    <div class="timeline-entry-inner">
                                        
                                        <div class="dropdown">
                                            <div class="timeline-icon bg-blue stage"><i class="fa fa-thumbs-up"></i>
                                                <div class="dropdown-content">
                                                    <div class="btn btn-block btn-default edit-btn" data-id="6">Edit</div>
                                                    <div class="btn btn-block btn-default preview-btn" data-id="6">Preview</div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="timeline-label bg-blue"><h4 class="timeline-title" id="title-6">Fueling Completed</h4>
                                            <p></p></div>
                                    </div>
                                </article>
                                <article class="timeline-entry">
                                    <div class="timeline-entry-inner">
                                        
                                        <div class="dropdown">
                                            <div class="timeline-icon bg-dark stage"><i class="fa fa-calculator"></i>
                                                <div class="dropdown-content">
                                                    <div class="btn btn-block btn-default edit-btn" data-id="7">Edit</div>
                                                    <div class="btn btn-block btn-default preview-btn" data-id="7">Preview</div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="timeline-label bg-dark"><h4 class="timeline-title" id="title-7">Fueling AmountToPay</h4>
                                            <p></p></div>
                                    </div>
                                                                 
                                </article>
                                <article class="timeline-entry left-aligned">
                                    <div class="timeline-entry-inner">
                                        
                                        <div class="dropdown">
                                            <div class="timeline-icon bg-green stage"><i class="fa fa-money-bill"></i>
                                                <div class="dropdown-content">
                                                    <div class="btn btn-block btn-default edit-btn" data-id="8">Edit</div>
                                                    <div class="btn btn-block btn-default preview-btn" data-id="8">Preview</div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="timeline-label bg-green"><h4 class="timeline-title" id="title-8">Payment Collection</h4>
                                            <p></p></div>
                                    </div> 
                                </article>
                                
                                <article class="timeline-entry">
                                    <div class="timeline-entry-inner">
                                        
                                        <div class="dropdown">
                                            <div class="timeline-icon bg-yellow stage"><i class="fa fa-circle"></i>
                                                <div class="dropdown-content">
                                                    <div class="btn btn-block btn-default edit-btn" data-id="9">Edit</div>
                                                    <div class="btn btn-block btn-default preview-btn" data-id="9">Preview</div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="timeline-label bg-yellow"><h4 class="timeline-title" id="title-9">Thank You</h4>
                                            <p></p></div>
                                    </div>
                                    <div class="timeline-entry-inner">
                                        <div style="-webkit-transform: rotate(-90deg); -moz-transform: rotate(-90deg);" class="timeline-icon"><i class="fa fa-plus"></i></div>
                                    </div>                             
                                </article>
                            </div>
                        </div>
                    </div>
                </div>
            </section>
        </div>
        <style type="text/css">
            body{
                margin-top:20px;
                background:#eee;
            }

            .timeline-centered {
                position: relative;
                margin-bottom: 30px;
            }
            .timeline-centered.timeline-sm .timeline-entry {
                margin-bottom: 20px !important;
            }
            .timeline-centered.timeline-sm .timeline-entry .timeline-entry-inner .timeline-label {
                padding: 1em;
            }
            .timeline-centered:before,
            .timeline-centered:after {
                content: " ";
                display: table;
            }
            .timeline-centered:after {
                clear: both;
            }
            .timeline-centered:before {
                content: '';
                position: absolute;
                display: block;
                width: 7px;
                background: #ffffff;
                left: 50%;
                top: 20px;
                bottom: 20px;
                margin-left: -4px;
            }
            .timeline-centered .timeline-entry {
                position: relative;
                width: 50%;
                float: right;
                margin-bottom: 70px;
                clear: both;
            }
            .timeline-centered .timeline-entry:before,
            .timeline-centered .timeline-entry:after {
                content: " ";
                display: table;
            }
            .timeline-centered .timeline-entry:after {
                clear: both;
            }
            .timeline-centered .timeline-entry.begin {
                margin-bottom: 0;
            }
            .timeline-centered .timeline-entry.left-aligned {
                float: left;
            }
            .timeline-centered .timeline-entry.left-aligned .timeline-entry-inner {
                margin-left: 0;
                margin-right: -28px;
            }
            .timeline-centered .timeline-entry.left-aligned .timeline-entry-inner .timeline-time {
                left: auto;
                right: -115px;
                text-align: left;
            }
            .timeline-centered .timeline-entry.left-aligned .timeline-entry-inner .timeline-icon {
                float: right;
            }
            .timeline-centered .timeline-entry.left-aligned .timeline-entry-inner .timeline-label {
                margin-left: 0;
                margin-right: 85px;
            }
            .timeline-centered .timeline-entry.left-aligned .timeline-entry-inner .timeline-label:after {
                left: auto;
                right: 0;
                margin-left: 0;
                margin-right: -9px;
                -moz-transform: rotate(180deg);
                -o-transform: rotate(180deg);
                -webkit-transform: rotate(180deg);
                -ms-transform: rotate(180deg);
                transform: rotate(180deg);
            }
            .timeline-centered .timeline-entry .timeline-entry-inner {
                position: relative;
                margin-left: -31px;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner:before,
            .timeline-centered .timeline-entry .timeline-entry-inner:after {
                content: " ";
                display: table;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner:after {
                clear: both;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-time {
                position: absolute;
                left: -115px;
                text-align: right;
                padding: 10px;
                -webkit-box-sizing: border-box;
                -moz-box-sizing: border-box;
                box-sizing: border-box;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-time > span {
                display: block;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-time > span:first-child {
                font-size: 18px;
                font-weight: bold;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-time > span:last-child {
                font-size: 12px;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-icon {
                background: #fff;
                color: #999999;
                display: block;
                width: 60px;
                height: 60px;
                -webkit-background-clip: padding-box;
                -moz-background-clip: padding-box;
                background-clip: padding-box;
                border-radius: 50%;
                text-align: center;
                border: 7px solid #ffffff;
                line-height: 45px;
                font-size: 15px;
                float: left;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-icon.bg-primary {
                background-color: #dc6767;
                color: #fff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-icon.bg-success {
                background-color: #5cb85c;
                color: #fff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-icon.bg-info {
                background-color: #5bc0de;
                color: #fff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-icon.bg-warning {
                background-color: #f0ad4e;
                color: #fff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-icon.bg-danger {
                background-color: #d9534f;
                color: #fff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-icon.bg-red {
                background-color: #bf4346;
                color: #fff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-icon.bg-green {
                background-color: #488c6c;
                color: #fff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-icon.bg-blue {
                background-color: #0a819c;
                color: #fff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-icon.bg-yellow {
                background-color: #f2994b;
                color: #fff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-icon.bg-orange {
                background-color: #e9662c;
                color: #fff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-icon.bg-pink {
                background-color: #bf3773;
                color: #fff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-icon.bg-violet {
                background-color: #9351ad;
                color: #fff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-icon.bg-grey {
                background-color: #4b5d67;
                color: #fff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-icon.bg-dark {
                background-color: #594857;
                color: #fff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label {
                position: relative;
                background: #ffffff;
                padding: 1.7em;
                margin-left: 85px;
                -webkit-background-clip: padding-box;
                -moz-background-clip: padding;
                background-clip: padding-box;
                -webkit-border-radius: 3px;
                -moz-border-radius: 3px;
                border-radius: 3px;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-red {
                background: #bf4346;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-red:after {
                border-color: transparent #bf4346 transparent transparent;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-red .timeline-title,
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-red p {
                color: #ffffff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-green {
                background: #488c6c;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-green:after {
                border-color: transparent #488c6c transparent transparent;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-green .timeline-title,
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-green p {
                color: #ffffff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-orange {
                background: #e9662c;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-orange:after {
                border-color: transparent #e9662c transparent transparent;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-orange .timeline-title,
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-orange p {
                color: #ffffff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-yellow {
                background: #f2994b;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-yellow:after {
                border-color: transparent #f2994b transparent transparent;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-yellow .timeline-title,
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-yellow p {
                color: #ffffff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-blue {
                background: #0a819c;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-blue:after {
                border-color: transparent #0a819c transparent transparent;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-blue .timeline-title,
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-blue p {
                color: #ffffff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-pink {
                background: #bf3773;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-pink:after {
                border-color: transparent #bf3773 transparent transparent;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-pink .timeline-title,
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-pink p {
                color: #ffffff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-violet {
                background: #9351ad;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-violet:after {
                border-color: transparent #9351ad transparent transparent;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-violet .timeline-title,
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-violet p {
                color: #ffffff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-grey {
                background: #4b5d67;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-grey:after {
                border-color: transparent #4b5d67 transparent transparent;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-grey .timeline-title,
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-grey p {
                color: #ffffff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-dark {
                background: #594857;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-dark:after {
                border-color: transparent #594857 transparent transparent;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-dark .timeline-title,
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label.bg-dark p {
                color: #ffffff;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label:after {
                content: '';
                display: block;
                position: absolute;
                width: 0;
                height: 0;
                border-style: solid;
                border-width: 9px 9px 9px 0;
                border-color: transparent #ffffff transparent transparent;
                left: 0;
                top: 20px;
                margin-left: -9px;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label .timeline-title,
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label p {
                color: #999999;
                margin: 0;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label p + p {
                margin-top: 15px;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label .timeline-title {
                margin-bottom: 10px;
                font-family: 'Oswald';
                font-weight: bold;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label .timeline-title span {
                -webkit-opacity: .6;
                -moz-opacity: .6;
                opacity: .6;
                -ms-filter: alpha(opacity=60);
                filter: alpha(opacity=60);
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label p .timeline-img {
                margin: 5px 10px 0 0;
            }
            .timeline-centered .timeline-entry .timeline-entry-inner .timeline-label p .timeline-img.pull-right {
                margin: 5px 0 0 10px;
            }
        </style>
        <div class="modal fade" id="info-modal" tabindex="-1" role="dialog" aria-labelledby="edit-preview" aria-hidden="true">
            <div class="modal-dialog">
                <div class="modal-content">
                    <div class="modal-header">
                        <h5 class="modal-title" id="exampleModalLabel">Info</h5>
                        <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                            <span aria-hidden="true">&times;</span>
                        </button>
                    </div>
                    <div class="modal-body" id='info-message'> 
                   </div>
                </div>
            </div>
        </div>

        <div class="modal fade" id="preview-modal" tabindex="-1" role="dialog" aria-labelledby="preview" aria-hidden="true" style="z-index:1999;">
            <div class="modal-dialog modal-xl" style="width:100%;height:100%;overflow:hidden;">
                <div class="modal-content" style="width:100%;height:100%;overflow:hidden;">
                    <div class="modal-header">
                        <h5 class="modal-title" id="exampleModalLabel">Preview</h5>
                        <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                            <span aria-hidden="true">&times;</span>
                        </button>
                    </div>
                    <div class="modal-body" style="width:100%;height:100%;overflow:hidden;">        
                        <embed type="text/html" id="preview-webpage" src="http://192.168.1.72/parcx" style="width:100%;height:100%;">            
                    </div>
                </div>
            </div>
        </div>

        <div class="modal fade" id="edit-modal"  tabindex="-1" aria-labelledby="exampleModalLabel"
             aria-hidden="true" style="overflow-y:auto;z-index:1999;">
            <div class="modal-dialog modal-xl" style="width:100%;">
                <div class="modal-content">
                    <div class="modal-header">
                        <h5 class="modal-title" id="exampleModalLabel">Edit</h5>
                        <button type="button" class="close" data-dismiss="modal" aria-label="Close" id="close-playlist-modal">
                            <span aria-hidden="true">&times;</span>
                        </button>
                    </div>
                    <div class="modal-body" style="margin:15px;"> 
                        <div class="container">
                            <div class="row justify-content-start">
                                <div class="col-xs-3">               
                                    <button class='btn btn-block btn-success btn-schedule' data-schedule='1'>Morning</button>
                                </div>
                                <div class="col-xs-3"> 
                                    <button class='btn btn-block btn-info btn-schedule' data-schedule='2'>Afternoon</button>
                                </div>
                                <div class="col-xs-3"> 
                                    <button class='btn btn-block btn-info btn-schedule' data-schedule='3'>Evening</button>
                                </div>
                                <div class="col-xs-3">
                                    <button class='btn btn-block btn-info btn-schedule' data-schedule='4'>Night</button>
                                </div>
                            </div>
                        </div>
                        <br>
                        <div class="alert-danger" id="error-upload" style="display:none;"></div>
                        <!--<form name="form1" class="block-data card card-body" data-status="form"  id="form">   -->             
                            <!--<div class="alert alert-light mb-2" role="alert" id="messagebox">
                            </div>-->
			<form name="form1" class="block-data" data-status="form"  id="form">
                            <div class="row"> 
                                <div class="col form-group">
                                    <label for="">Title</label>
                                    <input type="text" class="form-control" id="title" name="title" readonly>
                                </div>
                            </div>
                            <div class="row"> 
                                <div class="col form-group">
                                    <label for="">Description</label>
                                    <input type="text" class="form-control" id="description" required name="description">
                                </div>
                            </div>

                            <div class="row"> 
                                <div class="col form-group">
                                    <label for="">Message Line 1</label>
                                    <textarea class="form-control" id="message_line1"></textarea>
                                </div>
                            </div>

                            <div class="row"> 
                                <div class="col form-group">
                                    <label for="">Message Line 2</label>
                                    <textarea class="form-control" id="message_line2"></textarea>
                                </div> 
                            </div>

                            <div class="row"> 
                                <div class="col form-group">
                                    <label for="">Message Line 3</label>
                                    <textarea class="form-control" id="message_line3"></textarea>
                                </div> 
                            </div>


                            <div class="row">                         
                                <div class="col form-group">
                                    <label for="">Timeout</label>
                                    <input type="number" class="form-control" id="timeout" required name="timeout">
                                </div> 
                            </div>


                            <div class="row"> 
                                <div class="col form-group">
                                    <label for="">Backgroud Video</label>
                                    <!--<input type="file" id="bg_file" name="bgfile" accept ="video/mp4" />-->
                                    <input type="hidden" id="bgfile_hidden" value="">
                                    <br>
                                    <!--<video width='100%' height="100px" controls='controls' preload='metadata' id="bgvideo" ><source id= "video-source" src='/Media/Video1.mp4#t=0.5' type='video/mp4' ></video>-->
                                    <input type='file'  id='bg_file' accept ="video/mp4"  />
                                    <video width="25%" height="200" controls>
                                        Your browser does not support the video tag.
                                    </video>
                                </div> 
                            </div>

                            <div class="row"> 
                                <div class="col form-group">
                                    <label for="">Status Animation</label></br>
                                    <label class="radio-inline">
                                        <input type="radio" class="radiobtn" name ="animation-radio" data-name="gif" id="file_gif" value=".gif" checked />&nbsp;Gif<br />
                                    </label>

                                    <label class="radio-inline">
                                        <input type="radio" class="radiobtn" name ="animation-radio" data-name="lottie" id="file_lottie" value=".lottie,.json" />Lottie<br />
                                    </label>
                                  
                                       
                                      
                                    <br>
                                    <input type="file"  id="animation_file" name="animationfile" accept=".gif"/>
                                    <input type="hidden" id="animationfile_hidden" value="">
                                    <input type="hidden" id="animationtype_hidden" value="">

                                    <br>
                                    <img src ="" width='25%' height="200" id="image_preview"/>
                                    <div id="lottie_preview" style="width:25%;height:200px;"></div>

                                    <!--<lottie-player src="https://localhost/parcx/Media/test.json"></lottie-player>-->
                                </div>                       
                            </div>  


                            <input type="submit" class="save-stage-details btn btn-block btn-info mt-2 btn-lg" value="Save" id="save-details">

                        </form>             
                    </div>
                </div>
            </div>
        </div>
        <!-- / end modal -->


        <script type="text/javascript">
            var stage_id;
            var schedule = 1;
            /*$(document).on("click", ".stage", function ()
             {
             $("#edit-preview-modal").modal("show");
             stage_id = $(this).attr("data-id");
             $("#stage-id").val(stage_id);
             
             });*/

	    
            $(document).on("click", ".preview-btn", function ()
            {
                stage_id = $(this).attr("data-id");
                $("#preview-modal").modal("show");
                $("#preview-webpage").attr("src", "http://192.168.1.72/PGEGreeting");
            });

            function showform()
            {
                $("#form").trigger("reset");
                $("#edit-preview-modal").modal("hide");
                $("#edit-modal").modal("show");
                $("#error-upload").hide();

                $("#image_preview").attr("src", "");
                $("#image_preview").hide();
                $("#lottie_preview").html("");
                $("#lottie_preview").hide();
                document.querySelector("video").src = "";


                var data = {};
                data["stage_id"] = stage_id;//$("#stage-id").val();
                data["schedule"] = schedule;
                data["task"] = 3;
                var jsondata = JSON.stringify(data);
                console.log(jsondata);
                $.post("ajax/greeting_screen.php?task=3", jsondata, function (result) {
                    console.log(result);
                    var json = JSON.parse(result);
                    $("#title").val(json.title);
                    $("#description").val(json.description);
                    $("#message_line1").val(json.message_line1);
                    $("#message_line2").val(json.message_line2);
                    $("#message_line3").val(json.message_line3);
                    $("#timeout").val(json.timeout_period);
                    $("#bgfile_hidden").val(json.bg_video_file);
                    $("#animationfile_hidden").val(json.animation_file);
                    $("#animationtype_hidden").val(json.animation_type);
                    document.querySelector("video").src = "Media/" + json.bg_video_file + "#t=0.5";

                    //$('#bgvideo').find("source").attr("src", '/parcx/Media/'+json.bg_video_file+'#t=0.5');
                    if (json.animation_type == "image/gif")
                    {
                        $("#file_gif").prop("checked", true);
                        $("#file_lottie").prop("checked", false);
                        $("#image_preview").show();
                        $("#lottie_preview").hide();
                        $("#image_preview").attr("src", "Media/" + json.animation_file);
                    } else if (json.animation_type == "lottie" || json.animation_type == "application/json")
                    {
                        $("#file_lottie").prop("checked", true);
                        $("#file_gif").prop("checked", false);
                        $('#animation_file').attr("accept", ".json")
                        $("#image_preview").hide();
                        $("#lottie_preview").show();
                        $("#lottie_preview").html("<lottie-player autoplay='true' loop='' src='Media/" + json.animation_file + "' speed='1'  width='200' height='150' background='transparent'></lottie-player>");
                    }

                });
            }


            $(document).on("click", ".edit-btn", function ()
            {
                stage_id = $(this).attr("data-id");//$("#stage-id").val();
                showform(stage_id, 1);
            });

            $(document).on("click", ".btn-schedule", function ()
            {
                schedule = $(this).attr("data-schedule");
                $('.btn-schedule').removeClass('btn-success');
                $('.btn-schedule').addClass('btn-info');
                $(this).removeClass('btn-info');
                $(this).addClass('btn-success');
                showform(stage_id, schedule);
            });


            $('.radiobtn').change(function () {
                $('#animation_file').val(""); // before setting
                $('#animation_file').attr("accept", $(this).val())
            })

            function uploadfile(type)
            {
                event.preventDefault();
                var formData = new FormData();
                if (type == 1)
                    var file_data = $("#bg_file").prop("files")[0];
                else if (type == 2)
                    var file_data = $("#animation_file").prop("files")[0];
                file_data.name = new Date().valueOf() + "_" + file_data.name;
                formData.append('file', file_data);
                // formData.append('task',1);
                //formData.append('name',new Date().valueOf()+"_"+file_data.name)
                $.ajax({

                    url: "ajax/greeting_screen.php?task=1",
                    method: 'POST',
                    data: formData,
                    async: false,
                    contentType: false,
                    processData: false,
                    success: function (response) {
                        try {
                            var json = JSON.parse(response);
                            if (json.status == 200) {
                                var new_filname = json.file_name;
                                if (type == 1)
                                    //$("#bgfile_hidden").val(file_data.name);
                                    $("#bgfile_hidden").val(new_filname);
                                else if (type == 2) {
                                    //$("#animationfile_hidden").val(file_data.name);
                                    $("#animationfile_hidden").val(new_filname);
                                    $("#animationtype_hidden").val(file_data.type);
                                }
                                result = "Success"
                                UpdateStageDetails();
                            } else
                            {
                                result = "Failed"
                                $("#error-upload").show();
                                $("#error-upload").html(json.message);
                                //alert(json.message);

                            }
                        } catch (e) {

                            $("#error-upload").show();
                            $("#error-upload").html(response);
                        }
                    },
                    error: function (jqXHR, textStatus, errorThrown) {
                        $("#error-upload").show();
                        $("#error-upload").html(textStatus + errorThrown);
                        //alert(textStatus);
                        //alert(errorThrown);
                    } //end of error
                });

            }
            $(document).on("click", ".save-stage-details", function ()
            {
                var result = "Success";


                if ($('#bg_file').val() || $('#animation_file').val())
                {
                    if ($('#bg_file').val()) {
                        uploadfile(1);
                    }
                    if ($('#animation_file').val()) {
                        uploadfile(2);
                    }


                } else
                {
                    UpdateStageDetails();
                }

            });

            function UpdateStageDetails()
            {
                event.preventDefault();
                var data = {};
                data["stage_id"] = stage_id;
                data["schedule"] = schedule;
                data["title"] = $("#title").val();
                data["description"] = $("#description").val();
                data["message_line1"] = $("#message_line1").val();
                data["message_line2"] = $("#message_line2").val();
                data["message_line3"] = $("#message_line3").val();
                data["timeout"] = $("#timeout").val();
                data["bg_file"] = $("#bgfile_hidden").val();
                data["animation_file"] = $("#animationfile_hidden").val();
                data["animation_type"] = $("#animationtype_hidden").val();

                data["task"] = 2;
                var jsondata = JSON.stringify(data);
                console.log(jsondata);
                $.post("ajax/greeting_screen.php?task=2", jsondata, function (result) {
                    console.log(result);
                    if (result === "Success")
                    {
                        $("#edit-modal").modal('hide');
                        $("#info-message").html("Update Successful");
                        $("#info-modal").modal('show');
                        
                    }
                    else
                        alert(result);
                });
            }
            
            $('#info-modal').on('hidden.bs.modal', function () {
                location.reload();
               })

            document.getElementById("bg_file")
                    .onchange = function (event) {
                        let file = event.target.files[0];
                        let blobURL = URL.createObjectURL(file);
                        document.querySelector("video").src = blobURL;
                    }

            document.getElementById("animation_file")
                    .onchange = function (event) {
                        event.preventDefault();
                        let file = event.target.files[0];
                        let blobURL = URL.createObjectURL(file);
                        if (file.type == "image/gif")
                        {
                            $("#image_preview").attr("src", blobURL);
                            $("#image_preview").show();
                            $("#lottie_preview").hide();
                        } else if (file.type == "application/json")
                        {
                            alert("<lottie-player autoplay='true' loop='' src='" + blobURL + "' speed='1'   background='transparent'></lottie-player>");
                            $("#lottie_preview").show();
                            $("#lottie_preview").html("<lottie-player autoplay='true' loop='' src='" + blobURL + "' speed='1'   background='transparent'></lottie-player>");
                            $("#image_preview").hide();

                        }
                    }
        </script>
    </body>
</html>
