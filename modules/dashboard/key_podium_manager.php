<?php
include('../../includes/header.php');
include('../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header"></div>
<div class="row">
    <div class="col tab-header d-flex justify-content-center">
        <div class="tab-link active" data-target="key-manager">KEY MANAGER</div>
        <div class="tab-link" data-target="podium-manager">PODIUM MANAGER</div>
    </div>
</div>

<?php
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

<div class="content-wrapper">
  <section class="content">
    <div class="container-wide" id="report-table">
        <div class="block-data row" data-status="key-manager">              
            <div class="col-lg-6">
                <h1>KEY IN</h1>
                <diV id="key_in">
                    
                </diV>                                                
            </div> 
            
            <div class="col-lg-6">
                <h1>KEY OUT</h1>
                <div id="key_out">
                    
                </div>
            </div>
        </div> 
        
        <div class="block-data row " style="display:none;" data-status="podium-manager">              
            <div class="col-lg-6">
                <h1>ON THE WAY</h1>
                <div id="podium_in">
                    
                </div>
            </div> 
            
            <div class="col-lg-6">
                <h1>READY FOR DELIVERY</h1>
                <div id="podium_out">
                    
                </div>
            </div>
        </div> 
      
    </div>
  </section>
</div>
<?php include('../../includes/footer.php');?>

<script>

key_in();
key_out();
podium_in();
podium_out();

setInterval(function () {
key_in();
key_out();
podium_in();
podium_out();
        }, 30000);
        
$("* [data-target]").on('click', function ()
   {
       var $target = $(this).data('target');            
       $('.block-data').css('display', 'none');
       $('.block-data[data-status="' + $target + '"]').fadeIn('slow');
       $('.tab-link').removeClass('active');
       $(this).addClass('active');
   });
   
    
function key_in()
    {
    $.get("../ajax/valet.php?task=8", function (data) {       
    $('#key_in').html(data);     
    });    
    }
    
function key_out()
    {
    $.get("../ajax/valet.php?task=9", function (data) {
    $('#key_out').html(data);     
    });    
    }
    
function podium_in()
    {
    $.get("../ajax/valet.php?task=10", function (data) {
    $('#podium_in').html(data);     
    });    
    }
    
function podium_out()
    {
    $.get("../ajax/valet.php?task=11", function (data) {
    $('#podium_out').html(data);     
    });    
    }

</script>