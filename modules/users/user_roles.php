<?php
include('../../includes/header.php');
?>

<div class="navbar-has-tablink">

<?php
include('../../includes/navbar-start.php');
?>

</ul>
<div class="header text-dark" id="pdf-report-header">Manage User Roles</div>
<div class="row">
    <div class="col tab-header d-flex justify-content-center">
        <div class="tab-link active" data-target="add-new-role">Add New User Role</div>
        <div class="tab-link" data-target="view-roles">View User Roles</div>
    </div>
</div>

<?php
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

<!-- add user role -->
<div class="content-wrapper block-data" id="AddUserRole" data-status="add-new-role">
    <section class="content">
        <div class="container-wide pb-0">
            <div class="row mb-3">
                <div class="col-sm-6">
                    <h4 class="text-dark">Add New User Role</h4>
                </div><!-- /.col -->
            </div><!-- /.row -->

            <div class="card card-primary mb-4">

                <div class="card-body p-4">
                    <div class="row m-0">

                        <script src="<?=URL?>plugins/jquery/jquery-3.3.1.min.js"></script>

                        <script>
                            $(document).ready(function () {

                                $("#ViewCategory").hide();

                                $("#ViewUserRoleButton").click(function (event) {
                                    event.preventDefault();
                                    $("#ViewCategory").show();
                                    $("#AddUserRole").hide();
                                });

                                $('form#test').on('submit', function (e) {
                                    e.preventDefault();
                                    var data = new FormData($('form#test')[0]);
                                    data.append('sub', 'd');
                                    $.ajax({
                                        processData: false,
                                        contentType: false,
                                        type: 'post',
                                        url: "../ajax/admin.php",
                                        data: data,
                                        success: function (response) {
                                            location.reload(true);
                                            $("#AddUserRole").hide();
                                            $("#ViewCategory").show();
                                        }
                                    });

                                });
                            });
                        </script>

                        <?php

$arrMenus = get_menu_list();   
echo '<form method="post" id="test" enctype="multipart/form-data">
        <div class="">
            <div class="form-group">
                <div class="row m-0">
                    <div class="col-md-2 p-0">
                        <label for="user-role" class="">User Role Name</label>
                    </div>
                    <div class="col-md-3">
                        <input type="text" name="u_rolecode" id="u_rolecode" class="form-control">
                    </div>
                </div>
            </div>';

$oldcat="";
$mname="";

foreach ($arrMenus as $a) {
    // echo '1';
    foreach ($a as $key => $value) {
        // echo '2';
        if ($key==="menu_group_name") {
            // echo '3';
            if($oldcat!==$value)
            {
            $oldcat=$value;

                echo '<div class="form-group mb-0 mt-3">
                        <div class="row m-0">
                            <label class="parent-user-role">'.$value.'</label>
                        </div>
                      </div>';
            }
            // echo '4';
        }
        if($key==="menu_name")
        
        {
        echo '<div class="custom-control custom-checkbox mt-1">
                <input type="checkbox" class="custom-control-input" id="cb_' . $a['menu_id'] . '" name="cb_' . $a['menu_id'] . '" value="'.$value.'">
                <label class="custom-control-label" for="cb_' . $a['menu_id'] . '">' . $value. '</label>
              </div>';
        
        }
    }

}

// echo '</div>'; // close form group

echo '<div class="row mt-3 ml-0">
        <input type="submit" class="btn btn-info" value="Add New User" id="btn" name="sub">
      </div>';

echo '</div>'; // close card body
echo '</form>';

?>
                    </div>
                </div>
            </div>
        </div>
    </section>
</div>
<!-- end / add user role -->

<!-- view user role -->
<div class="content-wrapper block-data" data-status="view-roles" style="display:none;">
    <section class="content">
        <div class="container-wide">
            <div class="row mb-3">
                <div class="col-sm-6">
                    <h4 class="text-dark">View User Roles</h4>
                </div><!-- /.col -->
            </div><!-- /.row -->

            <div class="card card-primary mb-4">

                <div class="card-body">
                    <div class="row">

                        <link rel="stylesheet" href="<?=URL?>dist/css/userrole.css">

                        <script>
                            $(document).ready(function () {
                                $(document).on('click', '.edit', function (e) {
                                    e.stopPropagation();
                                    if ($(this).val() == "Edit") {
                                        $(this).closest("#parent").find('input[type=checkbox]').prop(
                                            'disabled', true);
                                        $(this).closest("#parent").find('input[type=button]').prop(
                                            'value',
                                            'Edit');
                                        $(this).attr('value', 'Update');
                                        $(this).closest("#parent").find('input[type=checkbox]').prop(
                                            'disabled', false);
                                        // for enabling text box
                                        //$(this).closest('tr').find('input[type=text]').prop('disabled', false);
                                    } else {
                                        $(this).attr('value', 'Edit');
                                        var data = "";
                                        $(this).closest("#parent").find('input[type=checkbox]').prop(
                                            'disabled', true);
                                        // for enabling text box
                                        $(this).closest("#parent").find('input[type=text]').prop(
                                            'disabled',
                                            false);
                                        $(this).closest("#parent").find('input[type=checkbox]').each(
                                            function () {
                                                var value = $(this).is(":checked") ? 1 : 0;
                                                var name = $(this).attr('name');
                                                data = data + name + "=" + value + "&"
                                            });

                                        var value = $(this).attr("roleid");
                                        data = data + "role_id=" + value + "&edit=" + "0"
                                        // alert(data);
                                        $.ajax({
                                            url: "../ajax/admin.php",
                                            type: 'POST',
                                            data: data,
                                            success: function (data) {
                                                alert(data);
                                            }
                                        });
                                    }
                                    return true;
                                })

                                $('.accordion').click(function (e) {
                                    e.stopPropagation
                                    var t = e.target.tagName.toLowerCase();
                                    if (t === 'div') {
                                        this.classList.toggle("active");

                                        $(this).find(".panel").toggle('panelshow');
                                    }
                                    return true;
                                });

                            });
                        </script>


                        <?php

$arrRight = Array();

$arrRole = get_user_roles();

$arrGroup = get_group_menu_name();

echo '<div id="container">'; // opcontainer
$selectedMenu = "";
$selectedMid = 0;
$rol = 1;
$OselectedGroup = "x";
$nselectedGroup = "";
$isGroupPrinted = false;
foreach ($arrRole as $role) {
foreach ($role as $key => $rolev) {
if ($key === "user_role_name") {
echo '<div id="parent">'; // opened parent
echo '<div id="roleholder" class="accordion">'; // open rol
echo $rolev;

echo '<div  class="panel" >';
foreach ($arrGroup as $g) {

    foreach ($g as $key => $group) {
        $isGroupPrinted = false;
        if ($key === "menu_group_name") {
            if ($isGroupPrinted === false) {
                echo '<div>';   //op gpname
                echo $g ["menu_group_name"];
                echo '</div>'; //close gp
                $isGroupPrinted = true;
                // $arrMenuunderGp= qeury menu table where group_id=$g ["menu_group_id"];

               $arrMenuunderGp= get_menu_under_group($g ["menu_group_name"]);
            
            }
    foreach ($arrMenuunderGp as $m4) {
        foreach ($m4 as $key2 => $value2) {
            if($key2 === "menu_name")
            {
            $menuid=$m4['menu_id'];
            $role_id=$role['user_role_id'];   

            // query access riht table with mnuid and role_id one
            $arrRightesR1= get_menu_access_by_role_and_menu_id($role_id,$menuid);   


            if (sizeof($arrRightesR1) > 0 && $key2 === "menu_name") {

                echo '<div>';   //op gpname

                $selectedMenu = $arrRightesR1[0]['menu_name'];
                $selectedMid = $arrRightesR1[0]['menu_id'];

                if ($arrRightesR1[0]['rr_view'] == 1) {
                    echo '<span><input type="checkbox" class="togle" checked="checked" disabled name="cb_' . $selectedMid . '" value="' . $selectedMenu . '"></span>';
                } 
                else {

                    echo '<span><input type="checkbox" class="togle"  disabled name="cb_' . $selectedMid . '" value="' . $selectedMenu . '"></span>';
                }
                echo ' ';
                echo $m4 ['menu_name'];
                //query


                echo '</div>'; //close gp 

                // else part is work when there is no menu in access right table 
            } else if ($key2 === "menu_name") {
                echo '<div>';   //op gpname
                $selectedMenu = $m4 ['menu_name'];
                $selectedMid = $m4 ['menu_id'];
                echo '<span><input type="checkbox" class="togle"  disabled name="cb_' . $selectedMid . '" value="' . $selectedMenu . '"></span>';
                
                echo ' ';
                echo $m4 ['menu_name'];
                echo '</div>'; //close gp
            }
            }
        }
       }

      }
    }
}

echo "<input class='edit' type='button' roleId='" . $role['user_role_id'] . "' value='Edit'> ";
echo '</div>'; // clsoe panel
echo '</div>'; //closed role
echo '</div>'; // close paent
}
}
}
echo '</div>'; // close container
?>

                    </div>
                </div>
            </div>
        </div>
    </section>
</div>
<!-- end / view user role -->
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
</script>

<?php
include('../../includes/footer.php');
?>