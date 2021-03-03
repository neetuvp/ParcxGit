<?php
include('includes/common.php');
?>

<html>

<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <title>Parcx Log in</title>
  <!-- Tell the browser to be responsive to screen width -->
  <meta name="viewport" content="width=device-width, initial-scale=1">

  <script src="<?=URL?>plugins/jquery/jquery-3.3.1.min.js"></script>
  <script src="<?=URL?>dist/js/index.js"></script>

  <link rel="stylesheet" href="<?=URL?>plugins/font-awesome/css/font-awesome.min.css">
  <link rel="stylesheet" href="<?=URL?>plugins/font-awesome/css/ionicons.min.css">

  <link rel="stylesheet" href="<?=URL?>dist/css/parcx.min.css">
  <link rel="stylesheet" href="<?=URL?>dist/css/styles.css">
  <link rel="stylesheet" href="<?=URL?>plugins/iCheck/square/blue.css">
  <!-- Google Font: Source Sans Pro -->
  <link href="<?=URL?>dist/fonts/noto-sans.css" rel="stylesheet">
</head>

<body class="hold-transition login-page">
  <div class="login-box">

    <div class="login-logo">
      <a href="#"><b>Parcx</b> Login</a>
    </div>
    <!-- /.login-logo -->
    <div class="card">
      <div class="card-body login-card-body">
        <p class="login-box-msg">Sign in to start your session</p>

        <form class="m-0">

          <!-- <div class="form-group has-feedback">
            <input type="email" id="username" class="form-control" placeholder="User Name">
            <span class="fa fa-user form-control-feedback"></span>
          </div>

          <div class="form-group has-feedback">
            <input type="password" id="password" class="form-control" placeholder="Password">
            <span class="fa fa-lock form-control-feedback"></span>
          </div> -->

          <div class="input-group mb-3">
            <div class="input-group-prepend">
              <div class="input-group-text"><span class="fa fa-user"></span></div>
            </div>
            <input type="email" id="username" class="form-control" placeholder="Username">
          </div>

          <div class="input-group mb-3">
            <div class="input-group-prepend">
              <div class="input-group-text"><span class="fa fa-lock"></span></div>
            </div>
            <input type="password" id="password" class="form-control" placeholder="Password">
          </div>

          <div class="row">
            <div class="col-8">
              <div id="messagebox"></div>
            </div>
            <!-- /.col -->
            <div class="col-4">
              <button id="login" type="submit" class="btn btn-primary btn-block btn-flat">Sign In</button>
            </div>
            <!-- /.col -->
          </div>
        </form>
      </div>
      <!-- /.login-card-body -->
    </div>
  </div>
  <!-- /.login-box -->

  <!-- Bootstrap 4 -->
  <script src="<?=URL?>plugins/bootstrap/js/bootstrap.bundle.min.js"></script>
  <!-- iCheck -->
  <script src="<?=URL?>plugins/iCheck/icheck.min.js"></script>

  <script>
    $(function () {
      $('input').iCheck({
        checkboxClass: 'icheckbox_square-blue',
        radioClass: 'iradio_square-blue',
        increaseArea: '20%' // optional
      })
    })
  </script>
</body>

</html>