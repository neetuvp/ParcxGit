<?php
include('../../includes/header.php');
?>

<div class="navbar-has-tablink">

    <?php
    include('../../includes/navbar-start.php');
    ?>

</ul>

<div class="header text-dark" id="pdf-report-header">Occupancy</div>

<?php
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

<div class="content-wrapper">    

    <section class="content">
        <div class="container-wide container-fix">
            <div class="row text-dark" id="occupancy_content">                
                 
            </div>
        </div>

    </section>
</div>
<script src="../../plugins/knob/jquery.knob.js"></script>
</div>

<script>
var facility_number =<?php echo $_GET["facility_number"] ;?>;
var carpark_number = 0;

function get_live_occupancy()
    {
    $.get("../ajax/dashboard.php?task=2&facility_number=" + facility_number, function (data) {
        $('#occupancy_content').html(data);         
        carpark_number=$("#carpark_number").val();        
        if (typeof carpark_number !== "undefined")
            window.location="occupancy_device.php?facility_number="+facility_number+"&carpark_number="+carpark_number;   
        else
            LoadKnob();
    });
    }
    

$(document).on('click', '.show-carpark-details', function () {
    facility_number = $(this).attr("facility_number");
    carpark_number = $(this).attr("carpark_number");
    window.location="occupancy_device.php?facility_number="+facility_number+"&carpark_number="+carpark_number;    
});

get_live_occupancy();

setInterval(function ()
    {
    get_live_occupancy();      
    }, 30000);
    
    

    
    

          


   
    /* jQueryKnob */
    function LoadKnob()
    {
        $('.knob').knob({
            draw: function () {

                // "tron" case
                if (this.$.data('skin') == 'tron') {

                    var a = this.angle(this.cv)  // Angle
                            ,
                            sa = this.startAngle          // Previous start angle
                            ,
                            sat = this.startAngle         // Start angle
                            ,
                            ea                            // Previous end angle
                            ,
                            eat = sat + a                 // End angle
                            ,
                            r = true

                    this.g.lineWidth = this.lineWidth

                    this.o.cursor
                            && (sat = eat - 0.3)
                            && (eat = eat + 0.3)

                    if (this.o.displayPrevious) {
                        ea = this.startAngle + this.angle(this.value)
                        this.o.cursor
                                && (sa = ea - 0.3)
                                && (ea = ea + 0.3)
                        this.g.beginPath()
                        this.g.strokeStyle = this.previousColor
                        this.g.arc(this.xy, this.xy, this.radius - this.lineWidth, sa, ea, false)
                        this.g.stroke()
                    }

                    this.g.beginPath()
                    this.g.strokeStyle = r ? this.o.fgColor : this.fgColor
                    this.g.arc(this.xy, this.xy, this.radius - this.lineWidth, sat, eat, false)
                    this.g.stroke()

                    this.g.lineWidth = 2
                    this.g.beginPath()
                    this.g.strokeStyle = this.o.fgColor
                    this.g.arc(this.xy, this.xy, this.radius - this.lineWidth + 1 + this.lineWidth * 2 / 3, 0, 2 * Math.PI, false)
                    this.g.stroke()

                    return false
                }
            }
        })
    } // end LoadKnob
    /* END JQUERY KNOB */





</script>

<?php include '../../includes/footer.php'; ?>