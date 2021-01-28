<?php
define("URL","http://".$_SERVER['SERVER_NAME']."/parcx/");

ini_set('display_errors',0);
//DAY CLOSURE
define("DAY_CLOSURE_START","00:00:00");
define("DAY_CLOSURE_END","23:59:59");
define("IMAGEURL","/Parcx/uploads/");
define("TOTALPARKING","200");
define("CURRENCY","AED");
// Database Connection . BEGINS -------------------//
date_default_timezone_set('Asia/Dubai');
///local variables 
define ("DB_HOST", "DBServer");
define ("DB_HOST2", "DBServer2");
define ("DB_USER", "parcxservice");
define ("DB_PASSWORD","1fromParcx!19514");
define ("DB_NAME","parcx_server");
define ("DB_NAME_REPORTING","parcx_reporting");
define ("DB_NAME_DASHBOARD","parcx_dashboard");
define ("COLUMN","Parktron");
define("ImageURL","http://".$_SERVER['SERVER_NAME']."/ANPR/Images");
session_start();

    if (isset($_GET['logout']))
    {
        session_destroy();
        unset($_SESSION['user']);
        header("location: index.php");
    }
        
?>