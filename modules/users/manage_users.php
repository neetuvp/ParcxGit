<?php
include('../../includes/header.php');
?>

<div class="navbar-has-tablink">

<?php
include('../../includes/navbar-start.php');
?>

</ul>
<div class="header text-dark" id="pdf-report-header">Manage Users</div>
<div class="row">
  <div class="col tab-header d-flex justify-content-center">
    <div class="tab-link active" data-target="active-user">Active Users</div>
    <div class="tab-link" data-target="disable-user">Disabled Users</div>
    <div class="tab-link" data-target="add-user">Add User</div>
    
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
          
            
            <div class="modal-body">
            <input type="hidden" id="id"/>
              <div class="control-group">
                <label for="current_password" class="control-label">Current Password</label>
                <div class="controls">
                  <input type="password" id="current_password">
                </div>
              </div>
              <div class="control-group">
                <label for="new_password" class="control-label">New Password</label>
                <div class="controls">
                  <input type="password" id="new_password">
                </div>
              </div>
              <div class="control-group">
                <label for="confirm_password" class="control-label">Confirm Password</label>
                <div class="controls">
                  <input type="password" id="confirm_password">
                </div>
              </div>
            </div>
            </div>
          
        <div class="modal-footer">          
          <button href="#" class="btn btn-primary" id="btnChangePassword">Change Password</button>
        </div>
        </div>
      </div>
    </div>
  </div>
</div>


<!-- add user -->
<div class="content-wrapper block-data" data-status="add-user" style="display:none;">
  <section class="content">
    <div class="container-wide">
      

      <div class="card card-primary mb-4 col-md-6" id="active-user-table">
        <div class="card-body ">
          <div class="row">
            <div class="col-md-6">
              <div class="form-group">
                <label>Full Name</label>
                <input type="text" class="form-control" id="operatorname" placeholder="Enter Full Name">
              </div>
              <!-- /.form-group -->
              <div class="form-group">
                <label>Email</label>
                <input type="email" class="form-control" id="email" placeholder="Enter email">
              </div>
              <!-- /.form-group -->
            </div>

            <div class="col-md-6">
              <div class="form-group">
                <label>User Name</label>
                <input type="text" class="form-control" id="username" placeholder="Enter User Name">
              </div>
              <!-- /.form-group -->
              <div class="form-group">
                <label>Password</label>
                <input type="password" class="form-control" id="password_1" placeholder="Password">
              </div>
              <!-- /.form-group -->
            </div>

            <div class="col-md-6">
              <div class="form-group">
                <label>Confirm password</label>
                <input type="password" class="form-control" id="password_2" placeholder="Confirm Password">
              </div>
              <!-- /.form-group -->
              <div class="form-group">
                <label>Company Name</label>
                <input type="text" class="form-control" id="companyname" placeholder="Company Name">
              </div>
              <!-- /.form-group -->
            </div>

            <!-- /.col -->
            <div class="col-md-6">
              <div class="form-group">
                <label>User Type</label>
                <?php


                            $result = get_user_roles();

                            echo '<select id="usertype" class="form-control select2" style="width: 100%;>';
                            echo '<option value="">Select</option>';

                            foreach ($result as $data) {
                                foreach ($data as $key => $value) {
                                    
                                    if (strpos($key, "user_role_id") !== false) {
                                        $user_role_id = $value;
                                    }
                                    
                                    if (strpos($key, "user_role_name") !== false) {
                                        $user_role_name = $value;
                                    }
                                }
                                echo '<option value="' . $user_role_id . '">' . $user_role_name . '</option>'; //close your tags!!
                            }
                        ?>
                </select>

              </div>
              <!-- /.form-group -->
              <div class="form-group">
                <label>Phone</label>
                <input type="text" class="form-control" id="phone" placeholder="Phone">
              </div>
              <!-- /.form-group -->
            </div>
            <!-- /.col -->
            <div class="col-md-6">

              <div class="form-group">
                <label>From Date</label>

                <div class="input-group">
                  <div class="input-group-prepend">
                    <span class="input-group-text"><i class="fa fa-calendar"></i></span>
                  </div>
                  <input type="date" class="form-control" id="fromDate" data-inputmask="'alias': 'dd/mm/yyyy'"
                    data-mask>
                </div>
                <!-- /.input group -->
              </div>
              <!-- /.form-group -->
            </div>
            <!-- /.col -->
            <div class="col-md-6">
              <div class="form-group">
                <label>To Date</label>
                <div class="input-group">
                  <div class="input-group-prepend">
                    <span class="input-group-text"><i class="fa fa-calendar"></i></span>
                  </div>
                  <input type="date" class="form-control" id="toDate" data-inputmask="'alias': 'dd/mm/yyyy'" data-mask>
                </div>
              </div>
              <!-- /.form-group -->
            </div>
            <div class="col-md-6">
              <!-- /.form-group -->
              <div class="form-group">
                <label for="exampleInputFile">File input</label>
                <div class="input-group">
                  <div class="custom-file">
                    <input type="file" class="custom-file-input" id="exampleInputFile">
                    <label class="custom-file-label" for="exampleInputFile">Choose file</label>
                  </div>
                  <div class="input-group-append">
                    <span class="input-group-text" id="">Upload</span>
                  </div>
                </div>
              </div>
              <!-- /.form-group -->
            </div>
          </div>
          <button type="submit" class="btn btn-primary" id="add_User">Add User</button>
        </div>
      </div>
    </div>
  </section>
</div>
<!-- end / add user -->

<!-- view active user -->
<div class="content-wrapper block-data" data-status="active-user">
  <section class="content">
    <div class="container-wide">
      

      <div class="card card-primary mb-4" id="active-user-table">

        <?php

$result = get_active_user();
foreach ($result as $data) {
    echo '<div class="card-header">';
    echo '<div class="d-flex justify-content-between align-items-center">';
    $i = 0;
    
  
    echo '<div class="col">USER NAME</div>';
    echo '<div class="col">NAME</div>';
    echo '<div class="col">ROLE</div>';
    echo '<div class="col">EMAIL</div>';
    echo '<div class="col">COMPANY</div>';
    echo '<div class="col">PHONE</div>';
    echo '<div class="col">FROM</div>';
    echo '<div class="col">TO</div>';
    echo '<div class="col"></div>';
    echo '<div class="col"></div>';
    echo '<div class="col"></div>';
    echo '</div>';
    echo '</div>';
    break;
}


foreach ($result as $data) {
    $id = $data['id'];    
    echo '<div class="table-view">';
    echo '<div class="card-text">';
    echo '<div class="d-flex justify-content-between align-items-center">';
    foreach ($data as $key => $value) {        
    if (strpos($key, "user_name") !== false) {
            echo '<div class="col" id="username-' .$id . '">' . $value . '</div>';
        }
        if (strpos($key, "operator_name") !== false) {
            echo '<div class="col" id="name-' .$id . '">' . $value . '</div>';
        }
        if (strpos($key, "role") == true) {
            echo '<div class="col" id="role-' . $id . '">' . $value . '</div>';
        }
        if (strpos($key, "email") == true) {
            echo '<div class="col" id="email-' . $id. '">' . $value . '</div>';
        }
        if (strpos($key, "company") == true) {
            echo '<div class="col" id="company-' . $id . '">' . $value . '</div>';
        }
        if (strpos($key, "phone") == true) {
            echo '<div class="col" id="phone-' . $id . '">' . $value . '</div>';
        }
        if (strpos($key, "user_from") !== false) {
            echo '<div class="col" id="from-' . $id . '">' . $value . '</div>';
        }
        if (strpos($key, "user_to") !== false) {
            echo '<div class="col" id="to-' . $id . '">' . $value . '</div>';
        }
        
    }
    
    print "<div class='col'>";    
    echo '<input type="submit" id="' . $id . '" class="btn btn-danger btn-block btn-disable-user" name="Add" value ="Disable">';
    print "</div> ";
    print "<div class='col'>";  
    // echo '<input type="submit" id="' . $id . '" class="btn btn-primary" name="Add" value ="Edit">';
    echo '<input type="submit" id="edit' . $id . '" class="btn btn-info btn-block edit-user-button" name="Add" value ="Edit">';
   
    print "</div> ";
    print "<div class='col'>";   
    // echo '<input type="submit" id="' . $id . '" class="btn btn-primary" name="Add" value="Edit">';
  echo '<input type="submit" id="password' . $id . '" class="btn btn-info btn-block change_password_button" data-toggle="modal" name="Add" data-target="#changePasswordModal" value="Change Password">';

    print "</div> ";
    
    echo '</div>';
    echo '</div>';
    echo '</div>';
}

?>

      </div>

    </div>
  </section>
  <!-- /.content -->
</div>
<!-- end / view active user -->


<!-- view disabled users -->
<div class="content-wrapper block-data" data-status="disable-user" style="display:none;">
  <section class="content">
    <div class="container-wide">
    

      <div class="card card-primary mb-4" id="active-user-table">

        <?php

$result = get_de_active_user();
if (sizeof($result) > 0) {
    foreach ($result as $data) {
        echo '<div class="card-header">';
        echo '<div class="d-flex justify-content-between align-items-center">';
        $i = 0;
        
        foreach ($data as $key => $value) {
            $i = $i + 1;
            
            if (strpos($value, "user") !== false) {
                //echo'  <td>' . $value . '</td>';
                
                $user_field = substr($value, 5);
                $UserField  = strtoupper($user_field); // string upper function  
                $users      = str_replace("_", " ", $UserField); // string replace function    
                
                echo '<div class="col" id="column-' . $i . '">' . $users . '</div>';
                
            }
        }
        echo '<div class="col"></div>';
        echo '</div>';
        echo '</div>';
        break;
    }
    
    foreach ($result as $data) {
        $k = 0;
        echo '<div class="table-view">';
        echo '<div class="card-text">';
        echo '<div class="d-flex justify-content-between align-items-center">';
        foreach ($data as $key => $value) {
            $k = $k + 1;
            if (strpos($key, "user") !== false) {
                echo '<div class="col" id="column-' . $k . '">' . $value . '</div>';
            }
            
        }
        
        print "<div class='col'>";
        $id = $data['id'];
        echo '<input type="submit" id="' . $id . '" class="btn btn-success btn-block btn-enable-user" name="Add" value ="Enable">';
        print "</div> ";
        print "</div> ";
        print "</div> ";
    }
}

?>

      </div>

    </div>
  </section>
  <!-- /.content -->
</div>
<!-- end / view disabled users -->
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

  /* Adjust Table Spacing */

  $('#column-4, #column-12').removeClass('col').addClass('col-2');
</script>

<?php
include '../../includes/footer.php';
?>