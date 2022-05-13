<?php
$page_title = "Fuleing Station Pump Settings";

include('../../includes/header.php');
?>

<div class="navbar-has-tablink">

    <?php
    include('../../includes/navbar-start.php');
    ?>


</ul>
<div class="header text-dark" id="pdf-report-header">Manage Users</div>
<div class="row hidden-sm-down">
    <div class="col tab-header d-flex justify-content-center">
        <div class="tab-link active" data-target="overview">Overview</div>
        <div class="tab-link" data-target="form">Add User</div>
    </div>
</div>

<?php
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>
<!-- Modal -->
<div class="modal fade text-dark" id="changePasswordModal" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-sm" role="document">
        <div class="modal-content">
            <div class="modal-body ">
                <div class="card">
                    <div class="card-header">
                        <h3 class="card-title d-inline">Change Password</h3>
                        <button type="button" class="close text-white" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">×</span></button>
                    </div>
                    <!-- /.card-header -->
                    <div class="card-body">
                        <form id="form-change-password">    

                        <div class="modal-body">
                            
                            <input type="hidden" id="id"/>
                            <div class="control-group">
                                <label for="current_password" class="control-label">Current Password</label>
                                <div class="controls">
                                    <input type="password" id="current_password" required name="current_password">
                                </div>
                            </div>
                            <div class="control-group">
                                <label for="new_password" class="control-label">New Password</label>
                                <div class="controls">
                                    <input type="password" id="new_password" required name="new_password">
                                </div>
                            </div>
                            <div class="control-group">
                                <label for="confirm_password" class="control-label">Confirm Password</label>
                                <div class="controls">
                                    <input type="password" id="confirm_password" required name="confirm_password">
                                </div>
                            </div>
                            <div class="control-group">
                            <input type="submit" class="col btn btn-info mt-4" id="btn-change-password" value ="change password"/>
                            </div>
                        </div>
                             </form>
                    </div>

                    
                   
                </div>
            </div>
        </div>
    </div>
</div>

<div class="content-wrapper">
    <section class="content">
        <div class="container-wide">        
            <!-- add/update carpark form --> 
            <form class="block-data card card-body col-md-5" data-status="form" style="display:none;" id="form">                
                <div class="row">

                </div>
                <div class="row">                    
                    <div class="col form-group">
                        <label for="">User Name</label>
                        <input type="text" class="form-control" id="user_name" required name="user_name">
                    </div>
                </div>                
                <div class="row">
                    <div class="col form-group">
                        <label for="">Full Name</label>
                        <input type="text" class="form-control" id="full_name"  required name="full_name">
                    </div>

                    <div class="col form-group">
                        <label for="">Email</label>
                        <input type="email" class="form-control" id="email" >
                    </div>                    
                </div>   
                <div class="row">
                    <div class="col form-group">
                        <label for="">User role</label>
                        <select id="user_role">
                            <?php
                            $data["task"] = 6;
                            parcxUserManagement($data);
                            ?>
                        </select>
                    </div>
                    <div class="col form-group">
                        <label for="">Language</label>
                        <select id="user_language">
                            <option>English</option>
                            <option>Arabic</option>
                            <option>Spanish</option>
                        </select>
                    </div>
                </div>
                <div class="row" id="password-block">	
                    <div class="col form-group">
                        <label for="">Password</label>
                        <input type="password" class="form-control" id="password" required name="password">
                    </div>
                    <div class="col form-group">
                        <label for="">Conform Password</label>
                        <input type="password" class="form-control" id="conform_password" name="ConfirmPassword">
                    </div>
                </div> 
                <div class="row">						     
                    <div class="col form-group">
                        <label for="">Company name</label>
                        <input type="text" class="form-control" id="company_name"  >
                    </div>
                    <div class="col form-group">
                        <label for="">Phone</label>
                        <input type="text" class="form-control" id="phone">
                    </div>  
                </div>

                <div class="row">
                    <div class="col form-group">
                        <label for="">Account Activation Date</label>
                        <input type="text" class="form-control" id="start_date" autocomplete="off" placeholder="" name="ActivationDate">
                    </div>
                    <div class="col form-group">
                        <label for="">Account Expiry Date</label>
                        <input type="text" class="form-control" id="expiry_date" autocomplete="off" placeholder="" name="ExpiryDate">
                    </div>
                </div>

                <input type="submit" class="signUp btn btn-block btn-info mt-2 btn-lg" value="Submit" id="add-edit-button">
            </form>

            <!-- carpark table -->         
            <div class="block-data" data-status="overview">
                <div class="card" >               
                    <div class="card-body">     
                        <table id="RecordsTable" class="table table-blue table-bordered table-striped">                    
                            <?php
                            $data["task"] = 8;
                            parcxUserManagement($data);
                            ?> 
                        </table>
                    </div>                                                  
                </div>             
            </div>             

        </div>
    </section>
</div>
<?php include('../../includes/footer.php'); ?>
<script>
    var status;
    var id;
    $(document).ready(function ()
    {
        $('#start_date').daterangepicker({timePicker: false, format: 'YYYY-MM-DD', singleDatePicker: true});
        $('#expiry_date').daterangepicker({timePicker: false, format: 'YYYY-MM-DD', singleDatePicker: true});
        $('#RecordsTable').DataTable(
                {
                    "paging": true,
                    "lengthChange": true,
                    "searching": true,
                    "ordering": true,
                    "info": true,
                    "autoWidth": false,
                    "lengthMenu": [[10, 25, 50, -1], [10, 25, 50, "All"]],
                    "aaSorting": []

                });
        $("* [data-target]").on('click', function ()
        {
            var $target = $(this).data('target');
            if ($target == "form")
            {
                $("#form").trigger('reset');
                $("#password-block").show();
                $("#user_name").attr("disabled",false);
                $("#add-edit-button").val("Submit");
            }
            $('.block-data').css('display', 'none');
            $('.block-data[data-status="' + $target + '"]').fadeIn('slow');
            $('.tab-link').removeClass('active');
            $(this).addClass('active');
        });


        //FormSubmit
        var formElement = $("#form");
        var rules_set = {
            ConfirmPassword:
                    {
                        equalTo: "#password",
                        required: true
                    }
        };

        formElement.find('input[type=text]').each(function ()
        {
            var name = $(this).attr('name');
            rules_set[name] = 'required';
        });

        formElement.validate({
            rules: rules_set,
            messages: {
                ConfirmPassword: {
                    required: "Retype your new password",
                    equalTo: "Passwords don't match, please check"
                }
            },
            errorElement: "div",
            errorPlacement: function (error, element) {
                error.addClass("text-danger");
                error.insertAfter(element);
            },
            submitHandler: function ()
            {
                var data = {};
                if ($("#add-edit-button").val() == "Submit")
                    data["id"] = "";
                else
                    data["id"] = id;

                data["full_name"] = $("#full_name").val();
                data["user_name"] = $("#user_name").val();
                data["email"] = $("#email").val();
                data["password"] = $("#password").val();
                data["company_name"] = $("#company_name").val();
                data["phone"] = $("#phone").val();
                data["start_date"] = $("#start_date").val();
                data["expiry_date"] = $("#expiry_date").val();
                data["user_role"] = $("#user_role").val();
                data["language"] = $("#user_language").val();
                data["task"] = "7";

                var jsondata = JSON.stringify(data);                
                $.post("../../modules/ajax/users.php", jsondata, function (result) {
                    if (result == "Successfull")
                        location.reload();
                    else
                        alert(result);
                });
            }
        });
        
     //FormSubmit change password
        var formElement2 = $("#form-change-password");
        var rules_set2 = {
            confirm_password:
                    {
                        equalTo: "#new_password",
                        required: true
                    }
        };

       

        formElement2.validate({
            rules: rules_set2,
            messages: {
                confirm_password: {
                    required: "Retype your new password",
                    equalTo: "Passwords don't match, please check"
                }
            },
            errorElement: "div",
            errorPlacement: function (error, element) {
                error.addClass("text-danger");
                error.insertAfter(element);
            },
            submitHandler: function ()
            {
                var data = {};
                id = $("#id").val();       
                var currentPass=$("#current_password").val();
                var newPass = $("#new_password").val();
                var confirmPass = $("#confirm_password").val();
                
                var data = {};
                data["id"] = id;
                data["current_password"] = currentPass;
                data["new_password"] = newPass;
                data["task"] = "10";

                var jsondata = JSON.stringify(data);               
                $.post("../../modules/ajax/users.php", jsondata, function (result) {
                    if (result == "Successfull")
                        location.reload();
                    else
                        alert(result);
                });
            }
        });

    });
    
   
    /* === enable disable product === */
    var status;
    var id;
    $(document).on("click", ".user-enable-disable-btn", function ()
    {
        id = $(this).parent('td').parent('tr').data('id');
        var status_text = $(this).text();
        if (status_text == "Disable")
            status = 0;
        else
            status = 1;

        var data = {};
        data["id"] = id;
        data["status"] = status;
        data["task"] = "9";
        var jsondata = JSON.stringify(data);
        $.post("../../modules/ajax/users.php", jsondata, function (result) {
            if (result == "Successfull")
                location.reload();
            else
                alert(result);
        });
    });
    /*change password*/
    $(document).on("click", ".user-change-password", function (){
        id = $(this).parent('td').parent('tr').data('id');
        $("#id").val(id); 
        $('#changePasswordModal').modal('show');
    });
                

    /*=====edit======*/
    $(document).on("click", ".user-edit", function ()
    {
        id = $(this).parent('td').parent('tr').data('id');
        var data = {};
        data["id"] = id;
        data["task"] = "11";
        var jsondata = JSON.stringify(data);
        $.post("../../modules/ajax/users.php", jsondata, function (result) {
            $('.block-data[data-status="overview"]').hide();
            $('.block-data[data-status="form"]').show();
            $('.tab-link').removeClass('active');
                        
            data["full_name"] = $("#full_name").val();
                data["user_name"] = $("#user_name").val();
                data["email"] = $("#email").val();
                data["password"] = $("#password").val();
                data["company_name"] = $("#company_name").val();
                data["phone"] = $("#phone").val();
                data["start_date"] = $("#start_date").val();
                data["expiry_date"] = $("#expiry_date").val();
                data["user_role"] = $("#user_role").val();
                data["language"] = $("#user_language").val();
                
            var response = JSON.parse(result);
            $("#full_name").val(response.operator_name);
            $("#user_name").val(response.username);
            $("#email").val(response.email);
            $("#company_name").val(response.company_name);
            $("#phone").val(response.phone);
            $("#start_date").val(response.validity_from_date);
            $("#expiry_date").val(response.validity_to_date);            
            $("#user_language").val(response.language);
            $("#user_role").val(response.user_role_id);            
            
            $("#password").val(response.password);
            $("#conform_password").val(response.password);
            $("#password-block").hide();
            $("#user_name").attr("disabled",true);
            
            $("#add-edit-button").val("Edit");                        
        });

    });

</script>
