<?php
ini_set('display_errors',1);

$lane_details = array(
    array(
        'device_name' => 'exit 1',
        'device_ip' => '192.168.1.11',
        'device_number' => '11',
        'camera_id' => '1'
    ),

    array(
        'device_name' => 'exit 2 ',
        'device_ip' => '192.168.1.12',
        'device_number' => '12',
        'camera_id' => '2'
    ),

    array(
        'device_name' => 'exit 3',
        'device_ip' => '192.168.1.13',
        'device_number' => '13',
        'camera_id' => '3'
    )
);
//print_r ($lane_details);
echo json_encode($lane_details);

?>