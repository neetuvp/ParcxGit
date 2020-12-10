
        </ul>

<!-- Right navbar links -->
<ul class="navbar-nav ml-auto">
    <li class="nav-item">
        <a href="#" class="btn btn-info logout-button" id="logout-button">
            <i class="fas fa-sign-out-alt"></i>
            <span class="ml-1 hidden-sm-down">Logout</span>
        </a>
    </li>
</ul>

</nav>

<aside class="main-sidebar sidebar-dark-primary elevation-4">

    <!-- brand logo -->
    <a href="" class="brand-link">
        <img src="dist/img/parcx-cloud-transparent.png">
    </a>

    <!-- sidebar -->
    <div class="sidebar">

        <!-- sidebar user panel -->
        <div class="user-panel mt-3 pb-3 mb-3 d-flex">
            <div class="image">
                <img src="dist/img/customer-logo/<?php echo $_SESSION["logo"];?>" class="img-circle elevation-2" alt="User Image">
            </div>
            <div class="info">               
                <a class="d-block" style="padding-left:8px;"><?php echo $_SESSION["operator_name"];?></a>
                <input type="hidden" id="operator_name" value="<?php echo $_SESSION["operator_name"]?>">
				<input type="hidden" id="operator_id" value="<?php echo $_SESSION["operator_id"]?>">											
            </div>
        </div>

        <!-- sidebar Menu -->
        <nav class="mt-2">
            <ul class="nav nav-pills nav-sidebar flex-column" data-widget="treeview" role="menu" data-accordion="false">
                <li class='nav-item has-treeview w-100'>
                <a href='validate-ticket.php' class='nav-link'>
                <i class='far fa-circle nav-icon'></i>
                <p>Validate Ticket Number</p>
                </a>
                </li>

              
                <?php
                if($_SESSION["report_view"]==1)
                    {                                        
                    echo "<li class='nav-item has-treeview w-100'>";
                    echo "<a href='validation-transactions.php' class='nav-link'>";
                    echo "<i class='far fa-circle nav-icon'></i>";
                    echo "<p>Validation Transactions</p>";
                    echo "</a>";
                    echo "</li>";                   
                    }
                ?>
                
                                                        

            </ul>
        </nav>

    </div>
</aside>

<script>
    

//Logout
$("#logout-button").on("click",function(){    
    window.location.href="index.php";
});

</script>