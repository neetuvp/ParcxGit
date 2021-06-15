<?php

include 'common.php';

session_start();

$logginTime = $_SESSION['last_login_timestamp'];

if (time() - $logginTime > 900) 
    {
    //subtract new timestamp from the old one
    echo "<script>alert('Your Session Is Expired. Please Login Your Account!')  </script>";
    unset($_SESSION['username'], $_SESSION['password'], $_SESSION['last_login_timestamp']);
    session_destroy();
    header("location:" . URL . "index.php"); //redirect to index.php
    exit;
    } 
else 
    {
    $_SESSION['last_login_timestamp'] = time(); //set new timestamp
    }

function getLabel($label)
{
    $data["label"]=$label;
    $data["language"] = $_SESSION["language"];
    $data["task"]=13; 
    echo parcxReport($data);
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
  <link rel="icon" href="<?php echo URL; ?>dist/img/icon/favicon.gif" type="image/x-icon">

  <!-- plugin css -->
  <link rel="stylesheet" href="<?=URL?>plugins/daterangepicker/daterangepicker-bs3.css">

  <!-- theme css -->  
  <link rel="stylesheet" href="<?=URL?>dist/css/parcx.min.css">

  <!-- custom css -->
  <link rel="stylesheet" href="<?=URL?>dist/fonts/noto-sans.css">
  <link rel="stylesheet" href="<?=URL?>dist/css/styles.css">
  <link rel="stylesheet" href="<?=URL?>dist/css/jquery-ui.css">

  <!-- ui dependent scripts -->
  <script src="<?=URL?>plugins/jquery/jquery-3.3.1.min.js"></script>
  <script src="<?=URL?>plugins/bootstrap-multiselect.min.js"></script>

  <!-- unchecked -->

  <!-- Font Awesome -->
  <link rel="stylesheet" href="<?=URL?>plugins/font-awesome/css/font-awesome.min.css">
  <link rel="stylesheet" href="<?=URL?>plugins/font-awesome-alt/css/all.css">

</head>

<body class="hold-transition sidebar-mini">
  <div class="wrapper">

    <!-- pdf loader modal -->
    <div class="modal" id="pdf-loader-modal" tabindex="-1" role="dialog" aria-labelledby="" aria-hidden="true">
      <div class="modal-dialog" role="document">
        <div class="modal-content">
          <div class="modal-body p-4 text-center">
            <img src="/parcx/dist/img/loading.gif" class="mb-3">
            <h5 class="mb-2">Generating PDF</h5>
          </div>
        </div>
      </div>
    </div>
