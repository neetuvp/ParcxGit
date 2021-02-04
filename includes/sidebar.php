<?php 

//session_start();

$userName=$_SESSION['user'];
$userRollName=$_SESSION['userRollName'];
$userRollId=$_SESSION['userRollId'];

?>


<!-- Main Sidebar Container -->
<aside class="main-sidebar sidebar-dark-primary elevation-4">
  <!-- Brand Logo -->
  <div class="brand-link">      
      <!--<img src="<?=URL?>dist/img/web_logo_dark_bg.png" style="opacity: .8;">--> 
      <img src="<?=URL?>dist/img/PARCX.png"> 
  </div>


  <!-- Sidebar -->
  <div class="sidebar">
    <!-- Sidebar user panel (optional) -->
    <div class="user-panel mt-3 pb-3 mb-3 d-flex">
      <div class="image">
        <img src="<?=URL?>dist/img/user.jpg" class="img-circle elevation-2" alt="User Image">
      </div>
      <div class="info" class="d-block info" style="padding-left:8px;">
        <a href="" class="d-block" style="padding-left:8px;">
          <div id="username2"><?php echo $userName ;?></div></a>
      </div>
    </div>

    <!-- Sidebar Menu -->
    <nav class="mt-2">
      <ul class="nav nav-pills nav-sidebar flex-column" data-widget="treeview" role="menu" data-accordion="false">

        <?php
        $data["user_role_id"]=$userRollId; 
        $data["url"]=URL; 
        $data["task"]=1;                          
        parcxUserManagement($data);
        ?>

      </ul>
    </nav>
    <!-- /.sidebar-menu -->
  </div>
  <!-- /.sidebar -->
</aside>