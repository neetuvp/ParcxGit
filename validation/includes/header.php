<?php
session_start();

if(!isset($_SESSION["operator_id"]) )
    {
    header("Location:index.php");
    }

?>
<!DOCTYPE html>

<head>

    <meta charset="utf-8">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta name="robots" content="index, no-follow">
    <meta name="author" content="AL FALAK - PARCX" />
    <title>Parcx | Parking Management</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="dist/img/favicon-32x32.png" type="image/x-icon">

    <!-- css -->
    <?php include('includes/css.php');?>

    <!-- scripts -->
    <script src="dist/js/jquery.min.js"></script>

</head>

<body class="hold-transition sidebar-mini">
    <div class="wrapper">
        <div class="">
            <nav class="main-header navbar navbar-expand navbar-light">
                <ul class="navbar-nav">
                    <li class="nav-item">
                        <a class="nav-link" data-widget="pushmenu" href="#"><i class="fa fa-bars"></i></a>
                    </li>
                </ul>