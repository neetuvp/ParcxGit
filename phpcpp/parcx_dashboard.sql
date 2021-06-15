-- phpMyAdmin SQL Dump
-- version 5.0.1
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: Apr 08, 2021 at 10:44 AM
-- Server version: 10.4.11-MariaDB
-- PHP Version: 7.2.28

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `parcx_dashboard`
--

-- --------------------------------------------------------

--
-- Table structure for table `ake_test_counter`
--

CREATE TABLE `ake_test_counter` (
  `id` int(11) NOT NULL,
  `logicareacode` varchar(20) NOT NULL,
  `logicareaname` varchar(20) NOT NULL,
  `total` int(11) NOT NULL,
  `avai` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `apm_cashbox_levels`
--

CREATE TABLE `apm_cashbox_levels` (
  `id` int(11) NOT NULL,
  `device_number` int(11) NOT NULL,
  `device_name` varchar(50) NOT NULL,
  `denomination` int(11) NOT NULL,
  `float_level` int(11) NOT NULL,
  `max_threshold` int(11) NOT NULL,
  `current_level` int(11) NOT NULL,
  `carpark_number` int(11) NOT NULL,
  `carpark_name` varchar(50) NOT NULL,
  `facility_number` int(11) DEFAULT NULL,
  `last_updated_datetime` datetime NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `apm_cashbox_levels`
--

INSERT INTO `apm_cashbox_levels` (`id`, `device_number`, `device_name`, `denomination`, `float_level`, `max_threshold`, `current_level`, `carpark_number`, `carpark_name`, `facility_number`, `last_updated_datetime`) VALUES
(1, 51, 'APM', 5, 0, 0, 0, 1, '', 200005, '2020-10-20 12:15:46'),
(2, 51, 'APM', 10, 0, 0, 5, 1, '', 200005, '2020-10-20 12:15:46'),
(3, 51, 'APM', 20, 0, 0, 0, 1, '', 200005, '2020-10-20 12:15:46'),
(4, 51, 'APM', 50, 0, 0, 10, 1, '', 200005, '2020-10-20 12:15:46'),
(5, 51, 'APM', 100, 0, 0, 0, 1, '', 200005, '2020-10-20 12:15:46'),
(6, 51, 'APM', 200, 0, 0, 0, 1, '', 200005, '2020-10-20 12:15:46'),
(7, 51, 'APM', 500, 0, 0, 0, 1, '', 200005, '2020-10-20 12:15:46'),
(8, 51, 'APM', 1000, 0, 0, 0, 1, '1', 200005, '2020-10-21 12:39:34');

-- --------------------------------------------------------

--
-- Table structure for table `apm_recycle_levels`
--

CREATE TABLE `apm_recycle_levels` (
  `id` int(11) NOT NULL,
  `device_number` int(11) NOT NULL,
  `device_name` varchar(50) NOT NULL,
  `denomination` int(11) NOT NULL,
  `current_level` int(11) NOT NULL,
  `float_level` int(11) NOT NULL,
  `max_threshold` int(11) NOT NULL,
  `carpark_number` int(11) NOT NULL,
  `carpark_name` varchar(50) NOT NULL,
  `facility_number` int(11) DEFAULT NULL,
  `last_updated_datetime` datetime NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `apm_recycle_levels`
--

INSERT INTO `apm_recycle_levels` (`id`, `device_number`, `device_name`, `denomination`, `current_level`, `float_level`, `max_threshold`, `carpark_number`, `carpark_name`, `facility_number`, `last_updated_datetime`) VALUES
(1, 51, 'APM', 5, 0, 5, 4, 1, '', 200005, '2020-10-20 12:15:48'),
(2, 51, 'APM', 10, 0, 5, 0, 1, '', 200005, '2020-10-20 12:15:48'),
(3, 51, 'APM', 20, 0, 0, 0, 1, '', 200005, '2020-09-17 08:58:14'),
(4, 51, 'APM', 50, 0, 2, 0, 1, '', 200005, '2020-10-20 12:15:48'),
(5, 51, 'APM', 100, 0, 0, 0, 1, '', 200005, '2020-09-17 08:58:14'),
(6, 51, 'APM', 200, 0, 0, 0, 1, '', 200005, '2020-09-17 08:58:14'),
(7, 51, 'APM', 500, 0, 0, 0, 1, '', 200005, '2020-09-17 08:58:14'),
(8, 51, 'APM', 1000, 0, 0, 0, 1, '1', 200005, '2020-10-21 12:39:36');

-- --------------------------------------------------------

--
-- Table structure for table `counters`
--

CREATE TABLE `counters` (
  `parking_counter_id` int(11) NOT NULL,
  `pgs_counter_id` varchar(20) DEFAULT NULL COMMENT 'pgs reference',
  `facility_number` int(11) NOT NULL DEFAULT 0,
  `count_category` tinyint(1) NOT NULL DEFAULT 0 COMMENT '0=parking ,1= pgs',
  `counter_type` tinyint(1) NOT NULL DEFAULT 0 COMMENT '0=facility,1=carpark,2=zone,3=device',
  `counter_name` varchar(100) DEFAULT NULL,
  `carpark_name` varchar(50) DEFAULT NULL,
  `dashboard_order` int(4) NOT NULL DEFAULT 0,
  `carpark_number` varchar(10) NOT NULL DEFAULT '0',
  `device_number` int(6) NOT NULL DEFAULT 0,
  `correction_factor` int(11) DEFAULT 0,
  `total_spaces` int(11) DEFAULT 0,
  `current_level` int(11) DEFAULT 0,
  `occupancy_threshold` int(6) NOT NULL DEFAULT 0,
  `free_spaces` int(11) NOT NULL DEFAULT 0,
  `reservation_current_level` int(6) NOT NULL DEFAULT 0,
  `access_current_level` int(6) NOT NULL DEFAULT 0,
  `shortterm_current_level` int(6) NOT NULL DEFAULT 0,
  `total_shortterm_spaces` int(6) NOT NULL DEFAULT 0,
  `total_reservation_spaces` int(6) NOT NULL DEFAULT 0,
  `total_access_spaces` int(6) NOT NULL DEFAULT 0,
  `total_entries` int(11) NOT NULL DEFAULT 0,
  `total_exits` int(11) NOT NULL DEFAULT 0,
  `total_manual_entry` int(6) NOT NULL DEFAULT 0 COMMENT 'external input or server open',
  `total_manual_exit` int(11) NOT NULL DEFAULT 0 COMMENT 'external input or server open',
  `blacklisted_tickets` int(6) NOT NULL DEFAULT 0,
  `server_manual_opens` int(6) NOT NULL DEFAULT 0,
  `external_manual_open` int(6) NOT NULL DEFAULT 0,
  `shortterm_transactions` int(6) NOT NULL DEFAULT 0,
  `access_transactions` int(6) NOT NULL DEFAULT 0,
  `reservation_transactions` int(6) NOT NULL DEFAULT 0,
  `reservation_entry` int(6) NOT NULL DEFAULT 0,
  `reservation_exit` int(6) NOT NULL DEFAULT 0,
  `shortterm_entry` int(6) NOT NULL DEFAULT 0,
  `shortterm_exit` int(6) NOT NULL DEFAULT 0,
  `access_entry` int(6) NOT NULL DEFAULT 0,
  `access_exit` int(6) NOT NULL DEFAULT 0,
  `occupancy_percentage` decimal(20,2) DEFAULT 0.00,
  `last_updated_datetime` datetime NOT NULL DEFAULT current_timestamp(),
  `cloud_upload_status` int(11) NOT NULL DEFAULT 0,
  `total_sensors` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `hourly_occupancy`
--

CREATE TABLE `hourly_occupancy` (
  `Id` int(11) NOT NULL,
  `facility_number` int(11) NOT NULL,
  `facility_name` varchar(50) NOT NULL,
  `carpark_number` int(11) NOT NULL,
  `carpark_name` varchar(50) NOT NULL,
  `type` varchar(5) NOT NULL,
  `data` varchar(25) NOT NULL,
  `h0to1` int(11) NOT NULL DEFAULT 0,
  `h1to2` int(11) NOT NULL DEFAULT 0,
  `h2to3` int(11) NOT NULL DEFAULT 0,
  `h3to4` int(11) NOT NULL DEFAULT 0,
  `h4to5` int(11) NOT NULL DEFAULT 0,
  `h5to6` int(11) NOT NULL DEFAULT 0,
  `h6to7` int(11) NOT NULL DEFAULT 0,
  `h7to8` int(11) NOT NULL DEFAULT 0,
  `h8to9` int(11) NOT NULL DEFAULT 0,
  `h9to10` int(11) NOT NULL DEFAULT 0,
  `h10to11` int(11) NOT NULL DEFAULT 0,
  `h11to12` int(11) NOT NULL DEFAULT 0,
  `h12to13` int(11) NOT NULL DEFAULT 0,
  `h13to14` int(11) NOT NULL DEFAULT 0,
  `h14to15` int(11) NOT NULL DEFAULT 0,
  `h15to16` int(11) NOT NULL DEFAULT 0,
  `h16to17` int(11) NOT NULL DEFAULT 0,
  `h17to18` int(11) NOT NULL DEFAULT 0,
  `h18to19` int(11) NOT NULL DEFAULT 0,
  `h19to20` int(11) NOT NULL DEFAULT 0,
  `h20to21` int(11) NOT NULL DEFAULT 0,
  `h21to22` int(11) NOT NULL DEFAULT 0,
  `h22to23` int(11) NOT NULL DEFAULT 0,
  `h23to24` int(11) NOT NULL DEFAULT 0,
  `cloud_upload_status` int(1) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `parking_revenue`
--

CREATE TABLE `parking_revenue` (
  `id` int(11) NOT NULL,
  `device_type` int(11) NOT NULL,
  `device_number` int(4) NOT NULL,
  `device_name` varchar(50) NOT NULL,
  `carpark_number` int(3) NOT NULL,
  `carpark_name` varchar(100) NOT NULL,
  `facility_number` int(11) NOT NULL DEFAULT 1,
  `facility_name` varchar(100) NOT NULL,
  `network_status` tinyint(1) NOT NULL DEFAULT 0,
  `gross_amount` double NOT NULL DEFAULT 0,
  `parking_fee` double NOT NULL DEFAULT 0,
  `vat_amount` double NOT NULL DEFAULT 0,
  `lost_ticket_fee` double NOT NULL DEFAULT 0,
  `admin_fixed_charges` double NOT NULL DEFAULT 0,
  `ticket_replacement_fee` double NOT NULL DEFAULT 0,
  `discount_amount` double NOT NULL DEFAULT 0,
  `product_sale_amount` double NOT NULL DEFAULT 0,
  `sms_payment_amount` double NOT NULL DEFAULT 0,
  `creditcard_payment_amount` double NOT NULL DEFAULT 0,
  `payable_entries_count` int(5) NOT NULL DEFAULT 0,
  `lost_ticket_count` int(5) NOT NULL DEFAULT 0,
  `discount_count` int(11) NOT NULL DEFAULT 0,
  `sms_payment_count` int(11) NOT NULL DEFAULT 0,
  `creditcard_payment_count` int(11) NOT NULL DEFAULT 0,
  `product_sale_count` int(11) NOT NULL DEFAULT 0,
  `last_updated_date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `last_transaction` varchar(100) DEFAULT NULL,
  `cloud_upload_status` int(1) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `ticket_check`
--

CREATE TABLE `ticket_check` (
  `id` int(11) NOT NULL,
  `date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `ticket_id` varchar(25) DEFAULT NULL,
  `device_type` tinyint(1) NOT NULL,
  `plate_number` varchar(25) DEFAULT NULL,
  `device_number` int(11) NOT NULL,
  `device_name` varchar(25) DEFAULT NULL,
  `result` text DEFAULT NULL,
  `result_description` text DEFAULT NULL,
  `carpark_number` int(11) NOT NULL,
  `carpark_name` varchar(25) DEFAULT NULL,
  `facility_number` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `valet_keys`
--

CREATE TABLE `valet_keys` (
  `id` int(11) NOT NULL,
  `ticket_number` varchar(25) NOT NULL DEFAULT '0',
  `plate_prefix` varchar(5) NOT NULL DEFAULT '0',
  `plate_number` varchar(10) NOT NULL DEFAULT '0',
  `car_image` varchar(50) NOT NULL DEFAULT 'default_car.jpg',
  `position` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `watchdog_device_status`
--

CREATE TABLE `watchdog_device_status` (
  `id` int(11) NOT NULL,
  `facility_number` varchar(24) NOT NULL DEFAULT '''1''',
  `facility_name` varchar(50) NOT NULL DEFAULT '''Global Village''',
  `carpark_number` int(5) NOT NULL,
  `carpark_name` varchar(50) NOT NULL,
  `device_number` int(5) NOT NULL,
  `device_name` varchar(25) NOT NULL,
  `device_ip` varchar(50) NOT NULL,
  `device_type` int(2) NOT NULL,
  `device_network_status` tinyint(4) NOT NULL DEFAULT 0,
  `cloud_upload_status` int(1) NOT NULL DEFAULT 0,
  `presence_loop_status` int(1) NOT NULL DEFAULT 0,
  `last_presence_loop_active` datetime DEFAULT NULL,
  `last_presence_loop_deactive` datetime DEFAULT NULL,
  `safety_loop_status` int(1) DEFAULT 0,
  `last_safety_loop_active` datetime DEFAULT NULL,
  `last_safety_loop_deactive` datetime DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `ake_test_counter`
--
ALTER TABLE `ake_test_counter`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `apm_cashbox_levels`
--
ALTER TABLE `apm_cashbox_levels`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `apm_recycle_levels`
--
ALTER TABLE `apm_recycle_levels`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `counters`
--
ALTER TABLE `counters`
  ADD PRIMARY KEY (`parking_counter_id`);

--
-- Indexes for table `hourly_occupancy`
--
ALTER TABLE `hourly_occupancy`
  ADD PRIMARY KEY (`Id`);

--
-- Indexes for table `parking_revenue`
--
ALTER TABLE `parking_revenue`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `ticket_check`
--
ALTER TABLE `ticket_check`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `valet_keys`
--
ALTER TABLE `valet_keys`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `watchdog_device_status`
--
ALTER TABLE `watchdog_device_status`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `ake_test_counter`
--
ALTER TABLE `ake_test_counter`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `apm_cashbox_levels`
--
ALTER TABLE `apm_cashbox_levels`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;

--
-- AUTO_INCREMENT for table `apm_recycle_levels`
--
ALTER TABLE `apm_recycle_levels`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;

--
-- AUTO_INCREMENT for table `counters`
--
ALTER TABLE `counters`
  MODIFY `parking_counter_id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `hourly_occupancy`
--
ALTER TABLE `hourly_occupancy`
  MODIFY `Id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `parking_revenue`
--
ALTER TABLE `parking_revenue`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `ticket_check`
--
ALTER TABLE `ticket_check`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `valet_keys`
--
ALTER TABLE `valet_keys`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `watchdog_device_status`
--
ALTER TABLE `watchdog_device_status`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
