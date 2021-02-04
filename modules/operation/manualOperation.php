<?php
header('Access-Control-Allow-Origin: *');
include('../../includes/header.php');
include('../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">Manual Operation</div>

<?php
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

<!-- Modal > Manual open Reason -->
<div class="modal fade text-dark" id="detailModal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel" aria-hidden="true">
    <div class="modal-dialog" role="document" id="manual-reason-content">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title">Manual Operation Reason</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
            </div>
            <div class="modal-body pt-4 pl-4 pr-4">
                <p>Reason:</p>
                <textarea name='reason_text' id='reason_text' class="form-control mb-4"></textarea>
                <span id="reasonempty"></span>
            </div>
            <div class="modal-footer">
                <button type='button' class='btn btn-info' name='ok_reason' id='ok_reason' value='OK'>Ok</button>
                <button type='button' class='btn btn-info' name='cancel_reason' id='cancel_reason'
                        value='Cancel'>Cancel</button>
            </div>
        </div>
    </div>
</div>
<!-- end / Modal -->

<div class="content-wrapper">
    <section class="content">
        <div class="container-wide">

            <div class="col-md-12 padding-reset">
                <div class="card" id="page-content">   
                    <div class="card card-primary" id="active-rates-table">
                        <div class="card-header">
                            <div class="d-flex justify-content-between align-items-center">
                                <div class="col" >Device Name</div>
                                <div class="col" >Device IP</div>
                                <div class="col"></div>
                                <div class="col"></div>
                                <div class="col"></div>
                                <div class="col"></div>
                                <div class="col"></div>
                                <div class="col"></div>
                            </div>      
                        </div>  
                        <?php
                        listManualOperation();
                        ?>


                    </div>      
                </div>
            </div>

        </div>
</div>
</section>
</div>
<script src="../../dist/js/manualOperations.js"></script>
<?php
include('../../includes/footer.php');
?>

