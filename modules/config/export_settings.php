<?php 
include('../../includes/header.php');
?>

<div class="navbar-has-tablink">

<?php
include('../../includes/navbar-start.php');
?>

</ul>
<div class="header text-dark" id="pdf-report-header">Export Settings</div>


<?php 
ini_set("display_errors",1);
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

<!-- Active Parking-->
<div class="content-wrapper block-data" data-status="active_parking">

 

  <section class="content">
    <div class="container-wide">

      <div class="card card-primary mb-4" id="active-parking" data-status="parking_details">
        <div class="card-header">
          <div class="d-flex justify-content-between align-items-center">
            <div class="col">No</div>
            <div class="col">Name</div>
            <div class="col">Export</div>
            <div class="col">SSH</div>
            <div class="col">SSH Interface</div>
            <div class="col">FTP</div>
            <div class="col">FTP Interface</div>
            <div class="col"></div>
          </div>
        </div>
        <?php
  $no = 1;
  $rate = 'parking_rates1';
   $result = GetExport();
   echo "<input type='hidden' name = 'rateno' id = 'rateno' value='parking_rates1'>";				
   foreach($result as $i=>$row)
   {
  
      echo "<div class='table-view'>";
      echo "<div class='card-text'>";
      echo "<div class='d-flex justify-content-between align-items-center'>";
      echo "<div class='col'>".$no."</div>";
      echo "<div class='col'>".$row['label']."</div>";
      echo "<div class='col' id='export".$row["id"]."'>";
      if($row['export_flag']==1)
      {
          $exportcheck = "checked";
      }
      else{
          $exportcheck = "";
      }
      echo '<input type= "checkbox" name="export" id="export_check'.$row["id"].'"  value = "" '.$exportcheck.' disabled/></td></tr>';
      echo "</div>";
      echo "<div class='col' id='ssh".$row["id"]."'>";
      if($row['ssh']==1)
      {
          $sshcheck = "checked";
      }
      else{
          $sshcheck = "";
      }
      echo '<input type= "checkbox" name="ssh" id="ssh_check'.$row["id"].'" value = "" '.$sshcheck.' disabled /></td></tr>';
      echo "</div>";
      $interface_name = GetInterfaceName($row['ssh_interface_id']);
      echo "<div class='col' id ='ssh_interface".$row["id"]."' >".$interface_name."</div>";
      echo "<div class='col' id='ftp".$row["id"]."'>";
      if($row['ftp']==1)
      {
          $ftpcheck = "checked";
      }
      else{
          $ftpcheck = "";
      }
      echo '<input type= "checkbox" name="ftp" id="ftp_check'.$row["id"].'" value = "" '.$ftpcheck.' disabled/></td></tr>';
      echo "</div>";
      $interface_name = GetInterfaceName($row['ftp_interface_id']);
      echo '<div class="col" id ="ftp_interface'.$row["id"].'" >'.$interface_name.'</div>';
      echo '<div class="col" id = "editbtn'.$row['id'].'">';				
      echo "<input class='btn btn-info btn-block btn-edit-export' id ='editbtn".$row['id']."' type='submit'  value='Edit' onclick = 'EditExport(".$row['id'].",".json_encode(SelectInterfaces()).")' id='".$row['id']."' >";
      echo "</div>";
     
      echo "</div>";
      echo "</div>";
      echo "</div>";
      $no++;
     
   }

   ?>


      </div>

      
    </div>
  </section>
</div><!-- /.row -->
</div>

<script>
  /* Table Show - Hide */

  $(document).ready(function () {
    $('.tab-link').on('click', function () {
      var $target = $(this).data('target');
      if ($target != 'all') {
        $('.block-data').css('display', 'none');
        $('.block-data[data-status="' + $target + '"]').fadeIn('slow');
      } else {
        $('.block-data').css('display', 'none').fadeIn('slow');
      }
    });
  });

  /* Change Active Tab Styling */

  $('.tab-link').on('click', function () {
    $('.tab-link').removeClass('active');
    $(this).addClass('active');
  });

  
  function EditExport(id,interfaces)
{
    //var id = $(this).attr('id');
    //alert("here"+id);
    
    var export_check;
    if($('#export_check'+id+':checkbox:checked').length > 0)
    {
        export_check = "checked";
    }
    else{
        export_check = "";
    }
    
    var ssh_check;
    if($('#ssh_check'+id+':checkbox:checked').length > 0)
    {
        ssh_check = "checked";
    }
    else{
        ssh_check = "";
    }

    var ftp_check;
    if($('#ftp_check'+id+':checkbox:checked').length > 0)
    {
        ftp_check = "checked";
    }
    else{
        ftp_check = "";
    }
    var ssh_interface_name = $('#ssh_interface'+id).html();
    var ftp_interface_name = $('#ftp_interface'+id).html();
    var ssh_interfaces_id = 0;
    var ftp_interfaces_id = 0;
    //var export_check = $('#export'+id).html();
   /* var discount_type = $('#discount_type'+id).html();
    var discount_option = $('#discount_option'+id).html();
    var discount_category = $('#discount_category'+id).html();
    var discount_value = $('#discount_value'+id).html();
    var carpark_name = $('#carpark_number'+id).html();
    var carpark_id = "";*/
    $('#export'+id).html("<input type= 'checkbox' name='export' id='edit_export"+id+"' value = '' "+export_check+"/>");
    $('#ssh'+id).html("<input type= 'checkbox' name='ssh' id='edit_ssh"+id+"' value = '' "+ssh_check+"/>");
    $('#ftp'+id).html("<input type= 'checkbox' name='ftp' id='edit_ftp"+id+"' value = '' "+ftp_check+"/>");
    var ssh_interface_select = "<select name = 'ssh_interface_text"+id+"' id = 'ssh_interface_text"+id+"'><option value='0'></option>";    
    for(var key in interfaces)
    {
        if(interfaces[key].interface_type=="ssh")
        {
            if(interfaces.hasOwnProperty(key))
            {
                ssh_interface_select += "<option value = '"+interfaces[key].id+"' >"+interfaces[key].interface_name+"</option>";
            }
            if(interfaces[key].interface_name==ssh_interface_name)
            {
                ssh_interfaces_id = interfaces[key].id;
            }
        }
    }
    $('#ssh_interface'+id).html(ssh_interface_select);        

    var ftp_interface_select = "<select name = 'ftp_interface_text"+id+"' id = 'ftp_interface_text"+id+"'><option value='0'></option>";
    
    for(var key in interfaces)
    {
        if(interfaces[key].interface_type=="ftp")
        {
            if(interfaces.hasOwnProperty(key))
            {
                ftp_interface_select += "<option value = '"+interfaces[key].id+"' >"+interfaces[key].interface_name+"</option>";
            }
            if(interfaces[key].interface_name==ftp_interface_name)
            {
                ftp_interfaces_id = interfaces[key].id;
            }
        }
    }
    $('#ftp_interface'+id).html(ftp_interface_select);  
    $('#editbtn'+id).html("<input class='btn btn-info btn-block btn-edit-discount' id ='editbtn"+id+"' type='submit'  value='Save' onclick='SaveRecord("+id+")' /><input class='btn btn-danger btn-block btn-cancel-export'  type='submit'  value='Cancel' onclick='CancelEditExport()' />");

    $('#ssh_interface_text'+id).val(ssh_interfaces_id);
    $('#ftp_interface_text'+id).val(ftp_interfaces_id);

    /*$('#editbtn'+id).html("<input class='btn btn-info btn-block btn-edit-discount' id ='editbtn"+id+"' type='submit'  value='Save' onclick='EditDiscountRecord("+id+")' />");
    $('.btn-disable-discount').attr('value','Cancel');
   
    $('#discount_option_text'+id).val(discount_option); 
    $('#discount_type_text'+id).val(discount_type); 
    $('#carpark_number_text'+id).val(carpark_id+":"+carpark_name);*/

    
    event.preventDefault();
}
function SaveRecord(id)
{
    var datastring = {};
    if($('#edit_ssh'+id+':checkbox:checked').length > 0)
    {
        if($('#ssh_interface_text'+id).val() == 0)
        {
            alert("Please select SSH Interface");
            return;
        }
    }

    if($('#ssh_interface_text'+id).val() > 0)
    {
        if($('#edit_ssh'+id+':checkbox:checked').length == 0)
        {
            alert("Please enable SSH");
            return;
        }
    }
       
    if($('#edit_ftp'+id+':checkbox:checked').length > 0)
    {
        if($('#ftp_interface_text'+id).val() == 0)
        {
            alert("Please select FTP Interface");
            return;
        }
    }

    if($('#ftp_interface_text'+id).val() > 0)
    {
        if($('#edit_ftp'+id+':checkbox:checked').length == 0)
        {
            alert("Please enable FTP");
            return;
        }
    }
     



    datastring['id'] = id;
    if($('#edit_export'+id+':checkbox:checked').length > 0)
    {
        datastring['export_flag'] = 1;
    }
    else{
        datastring['export_flag'] = 0;
    }
    if($('#edit_ssh'+id+':checkbox:checked').length > 0)
    {
        datastring['ssh'] = 1;
        datastring['ssh_interface_id'] = $('#ssh_interface_text'+id).val();
    }
    else{
        datastring['ssh'] = 0;
        datastring['ssh_interface_id'] = 0;

    }
    if($('#edit_ftp'+id+':checkbox:checked').length > 0)
    {
        datastring['ftp'] = 1;
        datastring['ftp_interface_id'] = $('#ftp_interface_text'+id).val();
    }
    else{
        datastring['ftp'] = 0;
        datastring['ftp_interface_id'] = 0;
    }
    
    
    var jsondata = JSON.stringify(datastring);
    //alert(jsondata);
        $.ajax({
        type:'post',
        url:"../../modules/ajax/exportsettings.php?type=1",
        data:jsondata,
        cache:false,
        contentType:'application/json',
        dataType:'json',
        success:function(data){
            location.reload();
        },
        error:function(jqXHR,textStatus,errorThrown)
        {            
            alert(errorThrown+" "+textStatus);
        }
        });
}
function CancelEditExport()
{
    document.location = "exportsettings.php";
}
</script>
<?php include('../../includes/footer.php');?>