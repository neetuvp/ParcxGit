<?php 

//session_start();

$userName=$_SESSION['user'];
$userRollName=$_SESSION['userRollName'];
$userRollId=$_SESSION['userRollId'];
$language="english";

?>


<!-- Main Sidebar Container -->
<aside class="main-sidebar sidebar-dark-primary elevation-4">
  <!-- Brand Logo -->
  <a href="<?=URL?>modules/dashboard/overview.php" class="brand-link">
    <img src="<?=URL?>dist/img/web_logo_dark_bg.png" style="opacity: .8;">
  </a>

  <!-- Sidebar -->
  <div class="sidebar">
    <!-- Sidebar user panel (optional) -->
    <div class="user-panel mt-3 pb-3 mb-3 d-flex">
      <div class="image">
        <img src="<?=URL?>dist/img/user2-160x160.jpg" class="img-circle elevation-2" alt="User Image">
      </div>
      <div class="info">
        <a href="#" class="d-block" style="padding-left:8px;">
          <div id="username2"><?php echo $userName ;?></div></a>
      </div>
    </div>

    <!-- Sidebar Menu -->
    <nav class="mt-2">
      <ul class="nav nav-pills nav-sidebar flex-column" data-widget="treeview" role="menu" data-accordion="false">

        <?php

        $arrRightesR1 = get_menu_access_by_user_role($userRollId);

        $selectedMenu="";
        $selectedMid=0;
        $rol=1;
        $OselectedGroup="x";
        $nselectedGroup="";
        $isFirst=true; 
        $isSubFirst=true; 
        $isGroupPrinted=false;
        $isChildUlStarted=FALSE;
   

        $OselectedGroup="x";
        $nselectedGroup="";
        $i=0;
        $length= sizeof($arrRightesR1);

        foreach($arrRightesR1 as  $r)
          {
          foreach ($r as $key => $value) 
            {
            if($key==='user_role_id')
              {
              if($isFirst)
                {
                if($r['menu_group_order_index']==1)                         
                  echo '<li class="nav-header"> REPORTS </li>';  
                  
                           
                echo '<li class="nav-item has-treeview">';   
                echo '<a href="#" class="nav-link"><i class="'.$r['group_menu_icon'].'"></i><p>';
                echo $r['menu_group_name']; 
                $isChildUlStarted=FALSE;
                echo '<i class="right fa fa-angle-right"></i></p></a>';                       
                }

              if($i-1>=0)
                 {
                 if(($arrRightesR1[$i-1]['menu_group_name'])!=$r['menu_group_name'])
                  {                     
                  if($r['menu_group_order_index']<4)
                    {   
                    echo '<li class="nav-item has-treeview">';  
                    echo '<a href="#" class="nav-link"><i class="'.$r['group_menu_icon'].'"></i><p>';
                    echo $r['menu_group_name']; 
                    $isChildUlStarted=FALSE;
                    echo '<i class="right fa fa-angle-right"></i></p></a>';
                    }                
                  else 
                    {   
                    if($isSubFirst) 
                      {
                      echo '<br><li class="nav-header">OPERATIONS</li><li class="nav-item has-treeview">';  
                      echo '<a href="#" class="nav-link"><i class="'.$r['group_menu_icon'].'"></i><p>';
                      echo $r['menu_group_name']; 
                      $isChildUlStarted=FALSE;
                      echo '<i class="right fa fa-angle-right"></i></p></a>';
                      $isSubFirst=FALSE;
                      }                             
                    else 
                      {                     
                      echo '<li class="nav-item has-treeview">';  
                      echo '<a href="#" class="nav-link"><i class="'.$r['group_menu_icon'].'"></i><p>';
                      echo $r['menu_group_name'];  
                      $isChildUlStarted=FALSE;
                      echo '<i class="right fa fa-angle-right"></i></p></a>';                 
                      }              
                    }
                  }
                }
              }
            
            // End Group Name
            
            // Child Menu  
            if($key==="rr_view")
              {                       
              if($value==1)
                {    
                if(!$isChildUlStarted)
                  {
                  echo '<ul class="nav nav-treeview">';
                  $isChildUlStarted=true;
                  }

                echo '<li class="nav-item"><a href="'.URL.$r['menu_link'].'" class="nav-link"><i class="far fa-circle nav-icon"></i><p>'.$r['menu_name'].'</p></a></li>';

                }
              if($i+1<$length)
                {
                if(($arrRightesR1[$i+1]['menu_group_name'])!=$r['menu_group_name'])
                  {
                  echo '</ul></li>';
                  }
                }
              }   // End Child Menu  
            
             }
            $isFirst=FALSE;  
            $i++;
         }

?>

      </ul>
    </nav>
    <!-- /.sidebar-menu -->
  </div>
  <!-- /.sidebar -->
</aside>