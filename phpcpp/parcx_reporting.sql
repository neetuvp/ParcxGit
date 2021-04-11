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
-- Database: `parcx_reporting`
--

-- --------------------------------------------------------

--
-- Table structure for table `alerts_logs`
--

CREATE TABLE `alerts_logs` (
  `id` int(11) NOT NULL,
  `log_datetime` datetime NOT NULL DEFAULT current_timestamp(),
  `emailId` text NOT NULL,
  `counter_condition` text NOT NULL,
  `message_send` text NOT NULL,
  `response` text NOT NULL,
  `send_datetime` datetime NOT NULL,
  `alert_update_status` int(11) NOT NULL DEFAULT 0,
  `comment` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `contract_parking_transactions`
--

CREATE TABLE `contract_parking_transactions` (
  `id` int(11) NOT NULL,
  `product_id` int(11) NOT NULL DEFAULT 0,
  `product_name` varchar(50) NOT NULL,
  `validity_start_date` date NOT NULL,
  `validity_end_date` date NOT NULL,
  `product_price` double NOT NULL,
  `date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `customer_name` varchar(100) DEFAULT NULL,
  `company_name` varchar(100) DEFAULT NULL,
  `customer_email` varchar(100) DEFAULT NULL,
  `customer_mobile` varchar(100) DEFAULT NULL,
  `description` varchar(100) DEFAULT NULL,
  `validity_days` int(11) NOT NULL DEFAULT 0,
  `carpark_number` varchar(100) DEFAULT NULL,
  `facility_number` int(11) NOT NULL,
  `plate_number` varchar(20) DEFAULT NULL,
  `tag` varchar(20) DEFAULT NULL,
  `ticket_id` varchar(25) DEFAULT NULL,
  `device_number` int(11) NOT NULL,
  `device_name` varchar(25) DEFAULT NULL,
  `cloud_upload_status` int(1) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `hourly_occupancy`
--

CREATE TABLE `hourly_occupancy` (
  `Id` int(11) NOT NULL,
  `Report_Date` date NOT NULL,
  `Report_Day` varchar(11) NOT NULL,
  `Carpark_Number` int(11) NOT NULL,
  `Carpark_name` varchar(25) NOT NULL,
  `Type` varchar(10) NOT NULL,
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
  `facility_number` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `hourly_validation_summary`
--

CREATE TABLE `hourly_validation_summary` (
  `id` int(11) NOT NULL,
  `report_date` date NOT NULL,
  `validator_id` int(11) NOT NULL,
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
  `total` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `open_transactions`
--

CREATE TABLE `open_transactions` (
  `id` int(11) NOT NULL,
  `facility_number` int(11) DEFAULT 1001,
  `carpark_number` int(11) DEFAULT NULL,
  `device_number` int(11) DEFAULT NULL,
  `device_name` varchar(25) DEFAULT NULL,
  `ticket_id` varchar(50) NOT NULL,
  `chip_utid` varchar(50) DEFAULT NULL,
  `plate_number` varchar(30) DEFAULT NULL,
  `entry_type` int(2) NOT NULL COMMENT '1 Shorterm/2 Access/ 3Reservation',
  `entry_date_time` datetime NOT NULL,
  `entry_grace_period` datetime NOT NULL DEFAULT current_timestamp(),
  `pay_upto` datetime DEFAULT NULL,
  `last_payment_date_time` datetime DEFAULT NULL,
  `max_exit_date_time` datetime DEFAULT NULL,
  `total_amount_paid` decimal(10,2) NOT NULL DEFAULT 0.00,
  `parking_rate` varchar(50) DEFAULT NULL,
  `total_validation_hours` int(6) NOT NULL DEFAULT 0,
  `operation_mode` varchar(30) DEFAULT NULL,
  `creation_date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `cloud_upload_status` int(11) NOT NULL DEFAULT 0,
  `plate_captured_id` int(11) DEFAULT NULL,
  `movement_type` int(1) NOT NULL DEFAULT 1,
  `carpark_name` varchar(100) DEFAULT NULL,
  `facility_name` varchar(100) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `parking_blacklist`
--

CREATE TABLE `parking_blacklist` (
  `id` int(11) NOT NULL,
  `date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `ticket_id` varchar(50) NOT NULL,
  `blacklisting_reason` varchar(50) NOT NULL DEFAULT 'BackOut',
  `blacklisting_description` tinytext DEFAULT NULL,
  `device_number` int(11) DEFAULT NULL,
  `device_name` varchar(25) DEFAULT NULL,
  `movement_type` tinyint(1) NOT NULL DEFAULT 1,
  `carpark_number` int(11) DEFAULT NULL,
  `carpark_name` varchar(30) DEFAULT NULL,
  `facility_number` varchar(10) NOT NULL,
  `plate_number` varchar(22) NOT NULL,
  `chip_utid` varchar(50) DEFAULT NULL,
  `operation_mode` varchar(25) NOT NULL DEFAULT 'Normal',
  `create_datetime` datetime DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `parking_duration`
--

CREATE TABLE `parking_duration` (
  `id` int(11) NOT NULL,
  `report_date` date NOT NULL,
  `report_day` varchar(25) NOT NULL,
  `carpark_number` int(5) NOT NULL DEFAULT 1,
  `carpark_name` varchar(50) NOT NULL DEFAULT 'PARCX',
  `category` varchar(25) NOT NULL DEFAULT 'shortterm',
  `m0to30` int(5) NOT NULL DEFAULT 0,
  `m30to60` int(5) NOT NULL DEFAULT 0,
  `h1to2` int(5) NOT NULL DEFAULT 0,
  `h2to3` int(5) NOT NULL DEFAULT 0,
  `h3to4` int(5) NOT NULL DEFAULT 0,
  `h4to5` int(5) NOT NULL DEFAULT 0,
  `h5to6` int(11) NOT NULL DEFAULT 0,
  `h6to7` int(11) NOT NULL DEFAULT 0,
  `h7to8` int(11) NOT NULL DEFAULT 0,
  `h8to9` int(5) NOT NULL DEFAULT 0,
  `h9to10` int(5) NOT NULL DEFAULT 0,
  `h10to11` int(5) NOT NULL DEFAULT 0,
  `h11to12` int(5) NOT NULL DEFAULT 0,
  `h12to13` int(5) NOT NULL DEFAULT 0,
  `h13to14` int(5) NOT NULL DEFAULT 0,
  `h14to15` int(5) NOT NULL DEFAULT 0,
  `h15to16` int(5) NOT NULL DEFAULT 0,
  `h16to17` int(5) NOT NULL DEFAULT 0,
  `h17to18` int(5) NOT NULL DEFAULT 0,
  `h18to19` int(5) NOT NULL DEFAULT 0,
  `h19to20` int(5) NOT NULL DEFAULT 0,
  `h20to21` int(5) NOT NULL DEFAULT 0,
  `h21to22` int(5) NOT NULL DEFAULT 0,
  `h22to23` int(5) NOT NULL DEFAULT 0,
  `h23to24` int(5) NOT NULL DEFAULT 0,
  `d1to2` int(5) NOT NULL DEFAULT 0,
  `d2to3` int(5) NOT NULL DEFAULT 0,
  `maximum_parking_duration` int(11) NOT NULL DEFAULT 0,
  `minimum_parking_duration` int(11) NOT NULL DEFAULT 0,
  `average_parking_duration` float NOT NULL DEFAULT 0,
  `flag` int(1) NOT NULL DEFAULT 0,
  `cloud_upload_status` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `parking_movements`
--

CREATE TABLE `parking_movements` (
  `id` int(11) NOT NULL,
  `date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `ticket_id` varchar(50) NOT NULL,
  `device_number` int(11) DEFAULT NULL,
  `device_name` varchar(25) DEFAULT NULL,
  `movement_type` varchar(50) NOT NULL,
  `carpark_number` int(11) DEFAULT NULL,
  `facility_number` int(11) DEFAULT 1002,
  `chip_utid` varchar(50) DEFAULT NULL,
  `plate_number` varchar(25) DEFAULT NULL,
  `operation_mode` varchar(25) DEFAULT NULL,
  `create_date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `plate_captured_id` int(11) DEFAULT 0,
  `carpark_name` varchar(100) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `parking_movements_access`
--

CREATE TABLE `parking_movements_access` (
  `id` int(11) NOT NULL,
  `date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `tag_serial` varchar(25) NOT NULL DEFAULT '1',
  `tag` tinytext NOT NULL,
  `ticket_id` varchar(50) DEFAULT NULL,
  `plate_number` varchar(25) DEFAULT NULL,
  `status_message` text NOT NULL,
  `epc` tinytext DEFAULT NULL,
  `tid` tinytext DEFAULT NULL,
  `epc_count` int(11) DEFAULT NULL,
  `epc_list` text DEFAULT NULL,
  `device_number` int(3) NOT NULL,
  `device_name` varchar(25) NOT NULL,
  `movement_type` int(11) NOT NULL,
  `parking_zone` varchar(25) DEFAULT NULL,
  `carpark_number` int(11) NOT NULL,
  `carpark_name` varchar(30) DEFAULT NULL,
  `facility_number` int(11) NOT NULL DEFAULT 1,
  `operation_mode` varchar(25) DEFAULT NULL,
  `whitelist_added` int(11) NOT NULL DEFAULT 0,
  `upload_date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `plate_captured_id` int(11) DEFAULT NULL,
  `cloud_upload_status` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `parking_movements_manual`
--

CREATE TABLE `parking_movements_manual` (
  `id` int(11) NOT NULL,
  `facility_number` int(11) DEFAULT 1001,
  `carpark_number` int(11) DEFAULT NULL,
  `device_number` tinyint(3) NOT NULL,
  `device_name` varchar(50) DEFAULT NULL,
  `movement_type` tinyint(3) NOT NULL DEFAULT 2,
  `date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `operator_id` varchar(25) DEFAULT NULL,
  `operator_name` varchar(25) NOT NULL,
  `action` varchar(50) NOT NULL,
  `reason` tinytext NOT NULL,
  `description` tinytext NOT NULL,
  `create_date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `carpark_name` varchar(100) DEFAULT NULL,
  `cloud_upload_status` int(1) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `parking_movements_reservation`
--

CREATE TABLE `parking_movements_reservation` (
  `id` int(11) NOT NULL,
  `ticket_id` varchar(50) NOT NULL,
  `date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `device_number` int(11) NOT NULL,
  `device_name` varchar(25) DEFAULT NULL,
  `movement_type` varchar(50) NOT NULL,
  `plate_number` varchar(50) DEFAULT NULL,
  `carpark_number` int(11) NOT NULL,
  `facility_number` int(11) NOT NULL,
  `plate_captured_id` int(11) DEFAULT NULL,
  `carpark_name` varchar(100) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `parking_movement_exceptions`
--

CREATE TABLE `parking_movement_exceptions` (
  `id` int(11) NOT NULL,
  `ticket_id` varchar(30) NOT NULL,
  `chip_utid` varchar(10) NOT NULL,
  `plate_number` varchar(20) NOT NULL,
  `exception` text NOT NULL,
  `date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `device_number` int(11) NOT NULL,
  `device_name` varchar(50) NOT NULL,
  `carpark_number` int(11) NOT NULL,
  `facility_number` int(11) NOT NULL,
  `carpark_name` varchar(100) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `parking_reservation`
--

CREATE TABLE `parking_reservation` (
  `reservation_id` int(11) NOT NULL,
  `transaction_id` varchar(25) NOT NULL,
  `plate_number` varchar(15) DEFAULT NULL,
  `ticket_id` varchar(50) NOT NULL,
  `name` varchar(70) NOT NULL,
  `address` varchar(255) NOT NULL,
  `country` varchar(25) NOT NULL,
  `phone` varchar(20) NOT NULL,
  `email` varchar(255) NOT NULL,
  `carpark_number` int(11) NOT NULL DEFAULT 1,
  `facility_number` int(11) NOT NULL DEFAULT 1,
  `parking_bay_number` varchar(5) NOT NULL,
  `from_time` datetime NOT NULL,
  `to_time` datetime NOT NULL,
  `creation_date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `reserved_parking_duration` tinytext NOT NULL,
  `reserved_parking_fee` decimal(6,0) NOT NULL,
  `payment_reference_id` tinytext DEFAULT NULL COMMENT 'online payment gateway',
  `payment_flag` tinyint(1) NOT NULL DEFAULT 0,
  `cancel_flag` tinyint(1) NOT NULL DEFAULT 0,
  `used_flag` int(1) NOT NULL DEFAULT 0,
  `cloud_upload_status` int(11) NOT NULL DEFAULT 0,
  `mobile_app_user_id` int(11) NOT NULL DEFAULT 0,
  `operator_id` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `parking_validation`
--

CREATE TABLE `parking_validation` (
  `id` int(11) NOT NULL,
  `date_time` datetime NOT NULL,
  `ticket_id` varchar(20) NOT NULL,
  `chip_utid` varchar(15) DEFAULT NULL,
  `plate_number` varchar(20) DEFAULT NULL,
  `chip_serial_number` varchar(20) DEFAULT NULL,
  `validator_id` int(11) NOT NULL,
  `validator_name` varchar(50) DEFAULT NULL,
  `product_id` int(11) NOT NULL DEFAULT 0,
  `product_name` varchar(50) DEFAULT NULL,
  `validation_type` varchar(25) NOT NULL,
  `validation_value` int(11) NOT NULL,
  `carpark_number` int(11) NOT NULL,
  `carpark_name` varchar(100) DEFAULT NULL,
  `facility_number` varchar(10) NOT NULL DEFAULT '0',
  `facility_name` varchar(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `plates_captured`
--

CREATE TABLE `plates_captured` (
  `id` int(11) NOT NULL,
  `camera_device_number` int(5) DEFAULT 71,
  `camera_name` varchar(50) NOT NULL,
  `plate_number` varchar(15) NOT NULL,
  `plate_type` varchar(15) NOT NULL,
  `plate_image_name` varchar(60) NOT NULL,
  `capture_date_time` timestamp NOT NULL DEFAULT current_timestamp(),
  `plate_area` varchar(30) NOT NULL,
  `plate_country` varchar(30) NOT NULL,
  `initial_plate_number` varchar(50) DEFAULT NULL,
  `confidence_rate` int(11) NOT NULL DEFAULT 0,
  `plate_corrected_date_time` datetime DEFAULT NULL,
  `user_id` int(11) NOT NULL DEFAULT 0,
  `user_name` varchar(25) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `revenue_apm_cash_payout`
--

CREATE TABLE `revenue_apm_cash_payout` (
  `id` int(11) NOT NULL,
  `device_number` int(11) NOT NULL,
  `device_name` varchar(50) NOT NULL,
  `user` varchar(25) DEFAULT NULL,
  `datetime` timestamp NOT NULL DEFAULT current_timestamp(),
  `operation` varchar(25) DEFAULT NULL,
  `cashlevel_before_payout` varchar(250) DEFAULT NULL,
  `cashlevel_after_payout` varchar(250) DEFAULT NULL,
  `notes_payout` varchar(250) DEFAULT NULL,
  `total_payout_amount` int(11) DEFAULT NULL,
  `carpark_number` int(11) NOT NULL,
  `carpark_name` varchar(50) NOT NULL,
  `facility_number` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `revenue_apm_cash_refill`
--

CREATE TABLE `revenue_apm_cash_refill` (
  `id` int(11) NOT NULL,
  `device_number` int(11) NOT NULL,
  `device_name` varchar(50) NOT NULL,
  `user` varchar(25) DEFAULT NULL,
  `datetime` timestamp NOT NULL DEFAULT current_timestamp(),
  `cashlevel_before_refill` varchar(250) DEFAULT NULL,
  `cashlevel_after_refill` varchar(250) NOT NULL,
  `notes_refilled` varchar(250) DEFAULT NULL,
  `total_amount_refilled` int(11) NOT NULL DEFAULT 0,
  `carpark_number` int(11) NOT NULL,
  `carpark_name` varchar(50) NOT NULL,
  `facility_number` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `revenue_creditcard_payments`
--

CREATE TABLE `revenue_creditcard_payments` (
  `id` int(11) NOT NULL,
  `device_number` int(11) NOT NULL,
  `merchant_id` int(11) DEFAULT NULL,
  `terminal_id` int(11) DEFAULT NULL,
  `ticket_number` varchar(30) DEFAULT NULL,
  `comment` text DEFAULT NULL,
  `card_type` varchar(30) DEFAULT NULL,
  `amount` double NOT NULL DEFAULT 0,
  `authentication_code` varchar(30) DEFAULT NULL,
  `reference_number` varchar(30) DEFAULT NULL,
  `date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `status` varchar(30) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `revenue_payments`
--

CREATE TABLE `revenue_payments` (
  `id` int(11) NOT NULL,
  `device_number` int(5) NOT NULL DEFAULT 0,
  `device_name` varchar(50) NOT NULL DEFAULT 'Cashier',
  `operator_id` int(11) NOT NULL DEFAULT 0,
  `operator_name` varchar(50) NOT NULL DEFAULT 'Operator',
  `shift_id` varchar(25) NOT NULL DEFAULT '1',
  `carpark_name` varchar(50) NOT NULL DEFAULT 'Car Park ',
  `carpark_number` int(5) NOT NULL DEFAULT 2,
  `parking_rate_label` varchar(25) DEFAULT NULL,
  `parking_rate_name` varchar(25) DEFAULT NULL,
  `entry_grace_period` varchar(25) NOT NULL DEFAULT '0',
  `exit_grace_period` varchar(25) NOT NULL DEFAULT '0',
  `vat_type` text DEFAULT NULL,
  `vat_percentage` int(11) NOT NULL DEFAULT 0,
  `ticket_id` varchar(50) NOT NULL DEFAULT '0',
  `entry_date_time` datetime DEFAULT current_timestamp(),
  `payment_date_time` datetime DEFAULT current_timestamp(),
  `max_exit_date_time` datetime DEFAULT current_timestamp(),
  `parking_duration` varchar(20) NOT NULL DEFAULT '0',
  `payment_category` varchar(20) NOT NULL DEFAULT '0',
  `parking_fee` double NOT NULL DEFAULT 0,
  `vat_amount` double NOT NULL DEFAULT 0,
  `lost_fee` double NOT NULL DEFAULT 0,
  `admin_fixed_charges` double NOT NULL DEFAULT 0,
  `ticket_replacement_fee` double NOT NULL DEFAULT 0,
  `discount_amount` double NOT NULL DEFAULT 0,
  `gross_amount` double NOT NULL DEFAULT 0,
  `amount_received` double NOT NULL DEFAULT 0,
  `discount_id` int(5) DEFAULT 0,
  `discount_category` varchar(25) DEFAULT '0',
  `discount_name` varchar(50) NOT NULL DEFAULT '0',
  `coupon_id` varchar(50) DEFAULT NULL,
  `coupon_category` varchar(25) DEFAULT NULL,
  `coupon_source` varchar(25) DEFAULT NULL,
  `product_id` int(11) NOT NULL DEFAULT 0,
  `product_quantity` int(11) NOT NULL DEFAULT 0,
  `payment_type` varchar(25) NOT NULL DEFAULT 'cash',
  `authentication_code` varchar(25) DEFAULT NULL,
  `void` text DEFAULT NULL,
  `record_creation_time` datetime DEFAULT current_timestamp(),
  `facility_number` int(11) NOT NULL DEFAULT 0,
  `entry_plate_number` varchar(15) DEFAULT NULL,
  `exit_plate_number` varchar(15) DEFAULT NULL,
  `anpr_comparison_number` int(11) DEFAULT NULL,
  `void_user` varchar(50) DEFAULT NULL,
  `void_reason` text DEFAULT NULL,
  `void_datetime` datetime NOT NULL DEFAULT current_timestamp(),
  `receipt_number` int(11) NOT NULL DEFAULT 0,
  `customer_card_id` int(11) DEFAULT NULL,
  `bank_notes` varchar(100) DEFAULT NULL,
  `balance_returned` double NOT NULL DEFAULT 0,
  `credit_note` double NOT NULL DEFAULT 0,
  `cloud_upload_status` int(11) NOT NULL DEFAULT 0,
  `wallet_points` float NOT NULL DEFAULT 0,
  `validation_value` varchar(50) DEFAULT NULL,
  `validation_id` text DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `revenue_payment_exceptions`
--

CREATE TABLE `revenue_payment_exceptions` (
  `id` int(11) NOT NULL,
  `ticket_id` varchar(50) NOT NULL,
  `device_number` int(11) NOT NULL,
  `chipcoin_datetime` datetime NOT NULL,
  `paid_status` int(11) NOT NULL,
  `exception` varchar(100) NOT NULL,
  `date_time` datetime DEFAULT current_timestamp(),
  `amount_received` double NOT NULL DEFAULT 0,
  `balance_returned` double NOT NULL DEFAULT 0,
  `credit_note` double NOT NULL DEFAULT 0,
  `bank_notes` varchar(250) DEFAULT NULL,
  `device_name` varchar(50) NOT NULL,
  `carpark_number` int(11) NOT NULL,
  `carpark_name` varchar(50) NOT NULL,
  `facility_number` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `revenue_physical_cash`
--

CREATE TABLE `revenue_physical_cash` (
  `id` int(11) NOT NULL,
  `shift_id` varchar(25) NOT NULL,
  `user_id` int(11) NOT NULL,
  `user_name` varchar(25) NOT NULL,
  `device_number` int(11) NOT NULL DEFAULT 0,
  `device_name` varchar(25) NOT NULL,
  `carpark_number` int(11) NOT NULL,
  `carpark_name` varchar(25) NOT NULL,
  `facility_number` int(11) NOT NULL,
  `denomination1` int(11) DEFAULT NULL,
  `denomination2` int(11) DEFAULT NULL,
  `denomination3` int(11) DEFAULT NULL,
  `denomination4` int(11) DEFAULT NULL,
  `denomination5` int(11) DEFAULT NULL,
  `denomination6` int(11) DEFAULT NULL,
  `denomination7` int(11) DEFAULT NULL,
  `denomination8` int(11) DEFAULT NULL,
  `denomination9` int(11) DEFAULT NULL,
  `denomination10` int(11) DEFAULT NULL,
  `total` double NOT NULL,
  `date_time` varchar(35) NOT NULL,
  `create_date_time` datetime NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `revenue_product_usage`
--

CREATE TABLE `revenue_product_usage` (
  `id` int(11) NOT NULL,
  `report_date` date NOT NULL,
  `device_number` int(11) NOT NULL,
  `device_name` varchar(50) NOT NULL,
  `carpark_number` int(11) NOT NULL,
  `carpark_name` varchar(50) NOT NULL,
  `product_id` int(11) NOT NULL DEFAULT 1,
  `product_name` varchar(50) NOT NULL DEFAULT 'Parking Voucher',
  `unit_price` double NOT NULL DEFAULT 10,
  `product_quantity` int(11) NOT NULL DEFAULT 0,
  `product_sale_amount` double NOT NULL DEFAULT 0,
  `reproccessing_flag` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `revenue_shifts`
--

CREATE TABLE `revenue_shifts` (
  `id` int(11) NOT NULL,
  `device_number` tinyint(3) NOT NULL DEFAULT 0,
  `device_name` varchar(50) DEFAULT NULL,
  `shift_id` varchar(50) DEFAULT NULL,
  `carpark_number` int(11) NOT NULL,
  `carpark_name` varchar(25) NOT NULL,
  `facility_number` int(11) NOT NULL,
  `operator_id` int(11) DEFAULT 0,
  `operator_name` varchar(25) DEFAULT NULL,
  `login_time` datetime DEFAULT NULL,
  `logout_time` datetime DEFAULT NULL,
  `shift_duration` varchar(10) DEFAULT NULL,
  `parking_fees` double NOT NULL DEFAULT 0,
  `vat_amount` double NOT NULL DEFAULT 0,
  `lost_fee` double NOT NULL DEFAULT 0,
  `admin_fixed_charges` double NOT NULL DEFAULT 0,
  `ticket_replacement_fee` double NOT NULL DEFAULT 0,
  `discount_amount` decimal(15,2) NOT NULL DEFAULT 0.00,
  `shift_earnings` double NOT NULL DEFAULT 0,
  `payin_amount` decimal(15,2) NOT NULL DEFAULT 0.00,
  `payout_amount` decimal(15,2) NOT NULL DEFAULT 0.00,
  `shift_opening_amount` decimal(15,2) NOT NULL DEFAULT 0.00,
  `shift_closing_amount` decimal(15,2) NOT NULL DEFAULT 0.00,
  `physical_cash_collected` decimal(15,2) NOT NULL DEFAULT 0.00,
  `shift_status` tinyint(1) NOT NULL DEFAULT 0,
  `last_updated_date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `wallet_points` double(10,3) NOT NULL DEFAULT 0.000,
  `cloud_upload_status` tinyint(1) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `revenue_subscription`
--

CREATE TABLE `revenue_subscription` (
  `id` int(11) NOT NULL,
  `ticket_id` varchar(100) DEFAULT NULL,
  `customer_name` varchar(150) DEFAULT NULL,
  `plate_number` varchar(20) DEFAULT NULL,
  `company_name` varchar(100) DEFAULT NULL,
  `customer_email` varchar(50) DEFAULT NULL,
  `customer_mobile` varchar(50) DEFAULT NULL,
  `customer_phone` varchar(50) DEFAULT NULL,
  `description` varchar(500) DEFAULT NULL,
  `validity` tinyint(3) NOT NULL DEFAULT 0,
  `validity_start_date` date NOT NULL,
  `validity_end_date` date NOT NULL,
  `subscription_amount` decimal(10,3) NOT NULL DEFAULT 0.000,
  `carpark_number` varchar(25) DEFAULT NULL,
  `facility_number` int(11) NOT NULL DEFAULT 0,
  `shift_id` varchar(100) DEFAULT NULL,
  `date_time` timestamp NOT NULL DEFAULT current_timestamp(),
  `device_number` int(11) NOT NULL,
  `device_name` varchar(25) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `smartgate_request`
--

CREATE TABLE `smartgate_request` (
  `id` int(11) NOT NULL,
  `device_number` int(11) DEFAULT NULL,
  `device_name` varchar(50) DEFAULT NULL,
  `datetime_request` timestamp NOT NULL DEFAULT current_timestamp(),
  `datetime_response` datetime DEFAULT NULL,
  `plate_number` varchar(15) DEFAULT NULL,
  `plate_capture_id` int(11) DEFAULT NULL,
  `gate_code` varchar(50) DEFAULT NULL,
  `reference_number` varchar(50) DEFAULT NULL,
  `request` text DEFAULT NULL,
  `response_status` varchar(50) DEFAULT NULL,
  `response` text DEFAULT NULL,
  `fault_string` text DEFAULT NULL,
  `fault_code_text` text DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `summary_daily_revenue`
--

CREATE TABLE `summary_daily_revenue` (
  `id` int(11) NOT NULL,
  `report_date` date DEFAULT NULL,
  `report_day` varchar(20) DEFAULT '=DAY(report_date)',
  `device_number` int(4) NOT NULL,
  `device_name` varchar(50) NOT NULL,
  `carpark_number` int(3) NOT NULL DEFAULT 2,
  `carpark_name` varchar(100) NOT NULL DEFAULT 'VALET ',
  `network_status` tinyint(1) NOT NULL DEFAULT 0,
  `gross_amount` double NOT NULL DEFAULT 0,
  `parking_fee` double NOT NULL DEFAULT 0,
  `vat_amount` decimal(10,2) NOT NULL DEFAULT 0.00,
  `lost_ticket_fee` double NOT NULL DEFAULT 0,
  `admin_fixed_charges` double NOT NULL DEFAULT 0,
  `ticket_replacement_charges` double NOT NULL DEFAULT 0,
  `discount_amount` double NOT NULL DEFAULT 0,
  `product_sale_amount` double NOT NULL DEFAULT 0,
  `sms_payment_amount` double NOT NULL DEFAULT 0,
  `creditcard_payment_amount` double NOT NULL DEFAULT 0,
  `rfid_prepaid` double NOT NULL DEFAULT 0,
  `complimentary_coupons_count` int(11) NOT NULL DEFAULT 0,
  `complimentary_handicap_count` int(11) NOT NULL DEFAULT 0,
  `payable_entries_count` int(5) NOT NULL DEFAULT 0,
  `lost_ticket_count` int(5) NOT NULL DEFAULT 0,
  `discount_count` int(10) NOT NULL DEFAULT 0,
  `product_sale_count` int(11) NOT NULL DEFAULT 0,
  `sms_payment_count` int(11) NOT NULL DEFAULT 0,
  `creditcard_payment_count` int(11) NOT NULL DEFAULT 0,
  `facility_number` int(11) NOT NULL DEFAULT 0,
  `last_updated_date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `reproccessing_flag` int(11) NOT NULL DEFAULT 0,
  `comments` text NOT NULL,
  `cloud_upload_status` int(1) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `summary_parking_movements`
--

CREATE TABLE `summary_parking_movements` (
  `id` int(11) NOT NULL,
  `report_date` date NOT NULL,
  `report_day` varchar(25) NOT NULL,
  `category` varchar(25) DEFAULT NULL,
  `movement_type` int(11) NOT NULL,
  `device_number` int(3) DEFAULT NULL,
  `carpark_id` int(11) NOT NULL,
  `car_park_number` int(3) DEFAULT NULL,
  `carpark_name` varchar(50) NOT NULL,
  `facility_number` int(11) NOT NULL,
  `facility_name` varchar(50) NOT NULL,
  `h0to1` int(5) DEFAULT 0,
  `h1to2` int(5) DEFAULT 0,
  `h2to3` int(5) DEFAULT 0,
  `h3to4` int(5) DEFAULT 0,
  `h4to5` int(5) DEFAULT 0,
  `h5to6` int(11) DEFAULT 0,
  `h6to7` int(11) NOT NULL DEFAULT 0,
  `h7to8` int(11) DEFAULT NULL,
  `h8to9` int(5) DEFAULT NULL,
  `h9to10` int(5) DEFAULT NULL,
  `h10to11` int(5) DEFAULT NULL,
  `h11to12` int(5) DEFAULT NULL,
  `h12to13` int(5) DEFAULT NULL,
  `h13to14` int(5) DEFAULT NULL,
  `h14to15` int(5) DEFAULT NULL,
  `h15to16` int(5) DEFAULT NULL,
  `h16to17` int(5) DEFAULT NULL,
  `h17to18` int(5) DEFAULT NULL,
  `h18to19` int(5) DEFAULT NULL,
  `h19to20` int(5) DEFAULT NULL,
  `h20to21` int(5) DEFAULT NULL,
  `h21to22` int(5) DEFAULT NULL,
  `h22to23` int(5) DEFAULT NULL,
  `h23to24` int(5) DEFAULT NULL,
  `comment` text DEFAULT NULL,
  `total_count` int(10) DEFAULT NULL,
  `flag` int(11) DEFAULT 0,
  `cloud_upload_status` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `track_ticket`
--

CREATE TABLE `track_ticket` (
  `id` int(11) NOT NULL,
  `ticket_number` varchar(50) NOT NULL,
  `plate_number` varchar(50) DEFAULT NULL,
  `chip_utid` varchar(10) DEFAULT NULL,
  `entry_device_number` int(11) NOT NULL,
  `entry_device_name` varchar(50) DEFAULT NULL,
  `entry_date_time` datetime DEFAULT NULL,
  `entry_grace_period` datetime DEFAULT NULL,
  `payment_date_time` datetime DEFAULT NULL,
  `payment_device_number` int(11) DEFAULT NULL,
  `payment_device_name` varchar(50) DEFAULT NULL,
  `max_exit_date_time` datetime DEFAULT NULL,
  `total_amount_paid` decimal(10,3) DEFAULT 0.000,
  `exit_date_time` datetime DEFAULT NULL,
  `exit_device_number` int(11) DEFAULT NULL,
  `exit_device_name` varchar(50) DEFAULT NULL,
  `payment_id` varchar(100) DEFAULT NULL,
  `validation_id` varchar(20) DEFAULT NULL,
  `entry_type` tinyint(4) NOT NULL DEFAULT 0,
  `carpark_number` int(11) NOT NULL,
  `carpark_name` varchar(50) NOT NULL,
  `facility_number` int(11) NOT NULL,
  `plate_captured_id` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `valet_parking`
--

CREATE TABLE `valet_parking` (
  `id` int(11) NOT NULL,
  `carpark_number` int(3) NOT NULL DEFAULT 0,
  `customer_name` varchar(13) DEFAULT NULL,
  `mobile_no` varchar(10) DEFAULT NULL,
  `plate_number` varchar(12) DEFAULT NULL,
  `plate_prefix` varchar(5) DEFAULT NULL,
  `plate_country` varchar(25) DEFAULT NULL,
  `plate_emirate` varchar(25) DEFAULT NULL,
  `front_plate` varchar(50) DEFAULT NULL,
  `left_plate` varchar(50) DEFAULT NULL,
  `right_plate` varchar(50) DEFAULT NULL,
  `back_plate` varchar(50) DEFAULT NULL,
  `ticket_number` varchar(30) DEFAULT NULL,
  `customer_card_number` int(11) NOT NULL DEFAULT 0,
  `customer_card_id` varchar(15) DEFAULT NULL,
  `key_card_id` varchar(15) DEFAULT NULL,
  `key_card_number` int(11) NOT NULL DEFAULT 0,
  `key_position` varchar(25) DEFAULT NULL,
  `parking_bay` varchar(25) DEFAULT NULL,
  `paid_status` tinyint(1) NOT NULL DEFAULT 0,
  `payment_date_time` datetime DEFAULT NULL,
  `valet_out` tinyint(1) NOT NULL DEFAULT 0,
  `parking_in` tinyint(1) NOT NULL DEFAULT 0,
  `parking_out` tinyint(1) NOT NULL DEFAULT 0,
  `dropoff_datetime` datetime DEFAULT NULL,
  `ready_for_delivery` tinyint(1) NOT NULL DEFAULT 0,
  `valet_in_datetime` datetime DEFAULT current_timestamp(),
  `valet_out_datetime` datetime DEFAULT NULL,
  `parking_in_datetime` datetime DEFAULT NULL,
  `parking_out_datetime` datetime DEFAULT NULL,
  `pickup_driver_id` varchar(25) DEFAULT NULL,
  `pickup_driver_name` varchar(25) DEFAULT NULL,
  `delivery_driver_id` varchar(25) DEFAULT NULL,
  `delivery_driver_name` varchar(25) DEFAULT NULL,
  `duration_pickup_parking` int(5) NOT NULL DEFAULT 0,
  `duration_parking` int(5) NOT NULL DEFAULT 0,
  `duration_payment_parking` int(5) NOT NULL DEFAULT 0,
  `duration_parking_dropoff` int(5) NOT NULL DEFAULT 0,
  `duration_dropoff_delivery` int(5) NOT NULL DEFAULT 0,
  `device_number` int(5) NOT NULL DEFAULT 0,
  `operator_id` int(5) NOT NULL DEFAULT 0,
  `operator_name` varchar(25) DEFAULT NULL,
  `car_brand` varchar(25) DEFAULT NULL,
  `car_color` varchar(50) DEFAULT NULL,
  `car_model` varchar(25) DEFAULT NULL,
  `record_creation_datetime` datetime NOT NULL DEFAULT current_timestamp(),
  `cloud_upload_status` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `validation_transactions`
--

CREATE TABLE `validation_transactions` (
  `id` int(11) NOT NULL,
  `ticket_number` varchar(50) NOT NULL,
  `entry_date_time` datetime NOT NULL,
  `validator_id` int(11) NOT NULL,
  `validation_id` int(11) NOT NULL,
  `validation_name` varchar(50) NOT NULL,
  `validation_value` int(11) NOT NULL,
  `validation_type` varchar(50) NOT NULL,
  `validation_date_time` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `visitor_frequency`
--

CREATE TABLE `visitor_frequency` (
  `id` int(11) NOT NULL,
  `plate_number` varchar(20) NOT NULL,
  `plate_country` varchar(25) DEFAULT NULL,
  `plate_category` varchar(25) DEFAULT NULL,
  `plate_region` varchar(25) DEFAULT NULL,
  `customer_name` varchar(50) DEFAULT NULL,
  `tag_id` varchar(50) DEFAULT NULL,
  `ticket_id` varchar(50) DEFAULT NULL,
  `d7` int(11) NOT NULL DEFAULT 0,
  `d30` int(11) NOT NULL DEFAULT 0,
  `d60` int(11) NOT NULL DEFAULT 0,
  `d90` int(11) NOT NULL DEFAULT 0,
  `d120` int(11) NOT NULL DEFAULT 0,
  `d150` int(11) NOT NULL DEFAULT 0,
  `d180` int(11) NOT NULL DEFAULT 0,
  `category` varchar(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `watchdog_device_alarms`
--

CREATE TABLE `watchdog_device_alarms` (
  `id` int(11) NOT NULL,
  `device_number` int(3) NOT NULL,
  `device_name` varchar(50) DEFAULT NULL,
  `alarm_severity` int(3) NOT NULL DEFAULT 0,
  `alarm_code` varchar(11) NOT NULL,
  `alarm_description` text NOT NULL,
  `parktron_alarm_id` bigint(20) DEFAULT NULL,
  `alarm_date_time` datetime NOT NULL,
  `create_date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `dismiss` int(11) NOT NULL DEFAULT 0,
  `carpark_name` varchar(100) DEFAULT NULL,
  `carpark_number` int(11) NOT NULL DEFAULT 0,
  `facility_number` int(11) NOT NULL DEFAULT 0,
  `cloud_upload_status` int(1) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `watchdog_network_logs`
--

CREATE TABLE `watchdog_network_logs` (
  `id` int(11) NOT NULL,
  `device_ip` varchar(25) NOT NULL,
  `device_name` varchar(50) DEFAULT NULL,
  `device_number` int(11) NOT NULL,
  `carpark_number` int(11) NOT NULL,
  `carpark_name` varchar(50) NOT NULL,
  `facility_name` varchar(50) DEFAULT NULL,
  `facility_number` int(11) DEFAULT 0,
  `comment` text NOT NULL,
  `connection_failure` datetime NOT NULL DEFAULT current_timestamp(),
  `connection_restored` datetime DEFAULT NULL,
  `connection_loss_duration_seconds` int(11) DEFAULT NULL,
  `connection_loss_duration_days_hours_minutes_seconds` varchar(30) DEFAULT NULL,
  `ping_response_time` int(11) DEFAULT NULL,
  `flag` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `alerts_logs`
--
ALTER TABLE `alerts_logs`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `contract_parking_transactions`
--
ALTER TABLE `contract_parking_transactions`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `hourly_occupancy`
--
ALTER TABLE `hourly_occupancy`
  ADD PRIMARY KEY (`Id`);

--
-- Indexes for table `hourly_validation_summary`
--
ALTER TABLE `hourly_validation_summary`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `open_transactions`
--
ALTER TABLE `open_transactions`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `parking_blacklist`
--
ALTER TABLE `parking_blacklist`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `parking_duration`
--
ALTER TABLE `parking_duration`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `parking_movements`
--
ALTER TABLE `parking_movements`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `parking_movements_access`
--
ALTER TABLE `parking_movements_access`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `parking_movements_manual`
--
ALTER TABLE `parking_movements_manual`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `parking_movements_reservation`
--
ALTER TABLE `parking_movements_reservation`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `parking_movement_exceptions`
--
ALTER TABLE `parking_movement_exceptions`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `parking_reservation`
--
ALTER TABLE `parking_reservation`
  ADD PRIMARY KEY (`reservation_id`);

--
-- Indexes for table `parking_validation`
--
ALTER TABLE `parking_validation`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `plates_captured`
--
ALTER TABLE `plates_captured`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `revenue_apm_cash_payout`
--
ALTER TABLE `revenue_apm_cash_payout`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `revenue_apm_cash_refill`
--
ALTER TABLE `revenue_apm_cash_refill`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `revenue_creditcard_payments`
--
ALTER TABLE `revenue_creditcard_payments`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `revenue_payments`
--
ALTER TABLE `revenue_payments`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `revenue_payment_exceptions`
--
ALTER TABLE `revenue_payment_exceptions`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `revenue_physical_cash`
--
ALTER TABLE `revenue_physical_cash`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `revenue_product_usage`
--
ALTER TABLE `revenue_product_usage`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `revenue_shifts`
--
ALTER TABLE `revenue_shifts`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `revenue_subscription`
--
ALTER TABLE `revenue_subscription`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `smartgate_request`
--
ALTER TABLE `smartgate_request`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `summary_daily_revenue`
--
ALTER TABLE `summary_daily_revenue`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `summary_parking_movements`
--
ALTER TABLE `summary_parking_movements`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `track_ticket`
--
ALTER TABLE `track_ticket`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `valet_parking`
--
ALTER TABLE `valet_parking`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `validation_transactions`
--
ALTER TABLE `validation_transactions`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `visitor_frequency`
--
ALTER TABLE `visitor_frequency`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `watchdog_device_alarms`
--
ALTER TABLE `watchdog_device_alarms`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `watchdog_network_logs`
--
ALTER TABLE `watchdog_network_logs`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `alerts_logs`
--
ALTER TABLE `alerts_logs`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `contract_parking_transactions`
--
ALTER TABLE `contract_parking_transactions`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `hourly_occupancy`
--
ALTER TABLE `hourly_occupancy`
  MODIFY `Id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `hourly_validation_summary`
--
ALTER TABLE `hourly_validation_summary`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `open_transactions`
--
ALTER TABLE `open_transactions`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `parking_blacklist`
--
ALTER TABLE `parking_blacklist`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `parking_duration`
--
ALTER TABLE `parking_duration`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `parking_movements`
--
ALTER TABLE `parking_movements`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `parking_movements_access`
--
ALTER TABLE `parking_movements_access`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `parking_movements_manual`
--
ALTER TABLE `parking_movements_manual`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `parking_movements_reservation`
--
ALTER TABLE `parking_movements_reservation`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `parking_movement_exceptions`
--
ALTER TABLE `parking_movement_exceptions`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `parking_reservation`
--
ALTER TABLE `parking_reservation`
  MODIFY `reservation_id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `parking_validation`
--
ALTER TABLE `parking_validation`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `plates_captured`
--
ALTER TABLE `plates_captured`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `revenue_apm_cash_payout`
--
ALTER TABLE `revenue_apm_cash_payout`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `revenue_apm_cash_refill`
--
ALTER TABLE `revenue_apm_cash_refill`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `revenue_creditcard_payments`
--
ALTER TABLE `revenue_creditcard_payments`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `revenue_payments`
--
ALTER TABLE `revenue_payments`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `revenue_payment_exceptions`
--
ALTER TABLE `revenue_payment_exceptions`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `revenue_physical_cash`
--
ALTER TABLE `revenue_physical_cash`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `revenue_product_usage`
--
ALTER TABLE `revenue_product_usage`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `revenue_shifts`
--
ALTER TABLE `revenue_shifts`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `revenue_subscription`
--
ALTER TABLE `revenue_subscription`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `smartgate_request`
--
ALTER TABLE `smartgate_request`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `summary_daily_revenue`
--
ALTER TABLE `summary_daily_revenue`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `summary_parking_movements`
--
ALTER TABLE `summary_parking_movements`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `track_ticket`
--
ALTER TABLE `track_ticket`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `valet_parking`
--
ALTER TABLE `valet_parking`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `validation_transactions`
--
ALTER TABLE `validation_transactions`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `visitor_frequency`
--
ALTER TABLE `visitor_frequency`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `watchdog_device_alarms`
--
ALTER TABLE `watchdog_device_alarms`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `watchdog_network_logs`
--
ALTER TABLE `watchdog_network_logs`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
