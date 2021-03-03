<?php
$page_title = "User Roles";

include('../../includes/header.php');
?>

<div class="navbar-has-tablink">

    <?php
    include('../../includes/navbar-start.php');
    ?>


</ul>
<div class="header text-dark" id="pdf-report-header">Manage User Roles</div>
<div class="row hidden-sm-down">
    <div class="col tab-header d-flex justify-content-center">
        <div class="tab-link active" data-target="overview">View User Role</div>
        <div class="tab-link" data-target="form">Add New User Role</div>        
    </div>
</div>

<?php
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>
<link rel="stylesheet" href="<?= URL ?>dist/css/userrole.css">

<div class="content-wrapper">
    <section class="content">
        <div class="container-wide">        
            <!-- add/update carpark form --> 
            <form class="block-data card card-body col-md-3" data-status="form"  id="form" style="display:none;">                 
                <div class="row">
                    <div class="col form-group">
                        <label for="">User Role Name</label>
                        <input type="text" class="form-control" id="user_role_name" placeholder=""  required>
                    </div>
                </div>	                
                <?php
                $data["task"] = 2;
                parcxUserManagement($data);
                ?>

                <input type="submit" class="btn btn-block btn-info mt-2 btn-lg" value="Submit" id="add-edit-button">
            </form>

            <!-- carpark table -->         
            <div class="block-data" data-status="overview" >
                <div class="card card-body col-md-3" >               
                    <?php
                    $data["task"] = 4;
                    parcxUserManagement($data);
                    ?>
                </div>             
            </div>             

        </div>
    </section>
</div>
<?php include('../../includes/footer.php'); ?>
<script>
    var id;
    var menuid;

    $(document).ready(function ()
    {

        $('.accordion').click(function (e)
        {
            e.stopPropagation;
            var t = e.target.tagName.toLowerCase();
            if (t === 'div')
            {
                this.classList.toggle("active");
                $(this).find(".panel").toggle('panelshow');
            }
            return true;
        });


        $("* [data-target]").on('click', function ()
        {
            var $target = $(this).data('target');
            if ($target == "form")
            {
                $("#form").trigger('reset');
            }
            $('.block-data').css('display', 'none');
            $('.block-data[data-status="' + $target + '"]').fadeIn('slow');
            $('.tab-link').removeClass('active');
            $(this).addClass('active');
        });



        //FormSubmit
        var formElement = $("#form");
        var rules_set = {};

        formElement.find('input[type=text]').each(function ()
        {
            var name = $(this).attr('name');
            rules_set[name] = 'required';
        });

        formElement.validate({
            rules: rules_set,
            errorElement: "div",
            errorPlacement: function (error, element) {
                error.addClass("text-danger");
                error.insertAfter(element);
            },
            submitHandler: function ()
            {
                var data = {};
                var i = 0;
                var menu = new Array();
                data["name"] = $("#user_role_name").val();

                formElement.find('input[type=checkbox]').each(function () {
                    if ($(this).is(":checked") == true)
                    {
                        menu[i] = $(this).attr('id')
                        i++;
                    }
                });
                data["menu"] = menu;
                data["task"] = 3;
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



    $(document).on("click", ".edit-user-role", function ()
    {
        id = $(this).data('id');
        console.log(id);
        if ($(this).val() == "Edit")
        {
            $(this).attr('value', 'Update');
            $(this).closest("#parent").find('input[type=checkbox]').prop('disabled', false);
        } else
        {
            $(this).attr('value', 'Edit');
            $(this).closest("#parent").find('input[type=checkbox]').prop('disabled', true);
            var data = {};
            var i = 0;
            var menu = new Array();
            data["user_role_id"] = id;

            $(this).closest("#parent").find('input[type=checkbox]').each(function ()
            {
                if ($(this).is(":checked") == true)
                {
                    menuid = $(this).attr('id');
                    menu[i] = menuid.split('-')[1];
                    i++;
                }
            });
            data["menu"] = menu;
            data["task"] = 5;
            var jsondata = JSON.stringify(data);
            console.log(jsondata);
            $.post("../../modules/ajax/users.php", jsondata, function (result) {
                if (result == "Successfull")
                    location.reload();
                else
                    alert(result);
            });
        }

    });



</script>
