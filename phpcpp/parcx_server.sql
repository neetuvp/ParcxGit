-- phpMyAdmin SQL Dump
-- version 5.0.1
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: Apr 08, 2021 at 10:46 AM
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
-- Database: `parcx_server`
--

-- --------------------------------------------------------

--
-- Table structure for table `access_anpr_checks`
--

CREATE TABLE `access_anpr_checks` (
  `anpr_check_id` int(11) NOT NULL,
  `facility_number` int(11) NOT NULL,
  `carpark_number` int(3) NOT NULL,
  `carpark_name` varchar(50) NOT NULL,
  `check_plate_type` tinyint(1) NOT NULL,
  `check_plate_country` tinyint(1) NOT NULL DEFAULT 0,
  `check_plate_area` tinyint(1) NOT NULL DEFAULT 0 COMMENT 'emirate or city ',
  `check_plate_prefix` tinyint(1) NOT NULL DEFAULT 0,
  `check_plate_number` tinyint(1) NOT NULL DEFAULT 0,
  `check_partial_plate_number` tinyint(1) NOT NULL DEFAULT 0,
  `check_plate_number_begining` tinyint(2) NOT NULL DEFAULT 0 COMMENT 'number of didigts from start',
  `check_plate_number_end` tinyint(2) NOT NULL DEFAULT 0 COMMENT 'number of didigts from end',
  `allow_O_Q_mismatch` tinyint(1) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `access_whitelist`
--

CREATE TABLE `access_whitelist` (
  `access_whitelist_id` int(25) NOT NULL,
  `access_id` int(11) DEFAULT NULL,
  `customer_name` varchar(100) NOT NULL,
  `ticket_id` varchar(50) NOT NULL,
  `plate_number` varchar(50) DEFAULT NULL,
  `corporate_parker` int(11) NOT NULL DEFAULT 0,
  `validity_start_date` date NOT NULL,
  `validity_expiry_date` date NOT NULL,
  `tag_serial` varchar(30) DEFAULT NULL,
  `tag` varchar(35) DEFAULT NULL,
  `tag_tid` varchar(35) DEFAULT NULL,
  `access_zones` text DEFAULT NULL,
  `carpark_number` text NOT NULL DEFAULT '1',
  `facility_number` int(11) NOT NULL DEFAULT 1,
  `creation_date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `status` int(11) NOT NULL DEFAULT 1,
  `antipassback_enabled` tinyint(1) NOT NULL DEFAULT 0,
  `last_visited_date_time` datetime(1) DEFAULT NULL,
  `wallet_payment` tinyint(1) NOT NULL DEFAULT 0,
  `season_card` tinyint(1) NOT NULL DEFAULT 0,
  `wallet_user_id` int(11) DEFAULT 0,
  `company_name` varchar(100) DEFAULT NULL,
  `last_sync` datetime NOT NULL DEFAULT current_timestamp(),
  `country` varchar(100) DEFAULT NULL,
  `operator_id` int(11) NOT NULL DEFAULT 0,
  `user_qrcode` varchar(25) DEFAULT NULL,
  `description` varchar(50) DEFAULT NULL,
  `cloud_upload_status` int(11) NOT NULL DEFAULT 0,
  `personalized_message_line1` varchar(100) DEFAULT NULL,
  `personalized_message_line2` varchar(100) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `access_whitelist_customers`
--

CREATE TABLE `access_whitelist_customers` (
  `id` int(11) NOT NULL,
  `customer_name` varchar(50) NOT NULL,
  `country` varchar(50) NOT NULL,
  `creation_date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `operator_id` int(11) NOT NULL DEFAULT 0,
  `wallet_payment` tinyint(4) NOT NULL DEFAULT 0,
  `season_card` tinyint(4) NOT NULL DEFAULT 0,
  `wallet_user_id` int(11) NOT NULL DEFAULT 0,
  `corporate_parker` int(11) NOT NULL DEFAULT 0,
  `company_name` varchar(100) DEFAULT NULL,
  `whitelist_group` text DEFAULT NULL,
  `personalized_message_line2` varchar(100) DEFAULT NULL,
  `active` int(11) NOT NULL DEFAULT 1
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `alerts_alarms`
--

CREATE TABLE `alerts_alarms` (
  `id` int(11) NOT NULL,
  `start_date_time` datetime NOT NULL,
  `hour` int(11) NOT NULL,
  `minute` int(11) NOT NULL,
  `email` text NOT NULL,
  `mobile` text NOT NULL,
  `last_send_datetime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `alerts_counter`
--

CREATE TABLE `alerts_counter` (
  `id` int(11) NOT NULL,
  `counter_id` int(11) DEFAULT NULL,
  `counter_name` varchar(30) DEFAULT NULL,
  `counter_condition` varchar(30) DEFAULT NULL,
  `start_date_time` datetime DEFAULT current_timestamp(),
  `hour` int(11) NOT NULL,
  `minute` int(11) NOT NULL,
  `counterId_in_message` text NOT NULL,
  `counters_in_message` text NOT NULL,
  `email` text NOT NULL,
  `mobile` text NOT NULL,
  `last_send_datetime` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `cashier_user_roles`
--

CREATE TABLE `cashier_user_roles` (
  `id` int(11) NOT NULL,
  `user_level` int(11) NOT NULL,
  `user_role_name` varchar(50) NOT NULL,
  `lost_access` int(11) NOT NULL,
  `history_access` int(11) NOT NULL,
  `calculate_access` int(11) NOT NULL,
  `activate_access` int(11) NOT NULL,
  `discount_access` int(11) NOT NULL,
  `barrieropen_access` int(11) NOT NULL,
  `barrierclose_access` int(11) NOT NULL,
  `payout_access` int(11) NOT NULL,
  `payin_access` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `cashier_user_roles`
--

INSERT INTO `cashier_user_roles` (`id`, `user_level`, `user_role_name`, `lost_access`, `history_access`, `calculate_access`, `activate_access`, `discount_access`, `barrieropen_access`, `barrierclose_access`, `payout_access`, `payin_access`) VALUES
(1, 1, 'Administrator', 1, 0, 0, 0, 1, 1, 0, 0, 0),
(2, 2, 'Cashier', 1, 0, 0, 0, 1, 1, 0, 0, 0),
(3, 3, 'Finance', 1, 1, 0, 0, 1, 1, 1, 0, 0);

-- --------------------------------------------------------

--
-- Table structure for table `cooperate_users`
--

CREATE TABLE `cooperate_users` (
  `user_id` int(11) NOT NULL,
  `user_name` varchar(100) NOT NULL,
  `parking_spaces` int(11) NOT NULL,
  `date_validity` int(1) NOT NULL DEFAULT 0,
  `start_date` date DEFAULT NULL,
  `expiry_date` date DEFAULT NULL,
  `status` tinyint(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `customer_messages`
--

CREATE TABLE `customer_messages` (
  `id` int(11) NOT NULL,
  `device_type` int(3) NOT NULL DEFAULT 11,
  `message` varchar(100) NOT NULL,
  `media_path` text DEFAULT NULL,
  `english_line1` varchar(100) DEFAULT NULL,
  `english_line2` varchar(50) DEFAULT NULL,
  `english_line3` varchar(50) DEFAULT NULL,
  `arabic_line1` varchar(100) DEFAULT NULL,
  `arabic_line2` varchar(100) DEFAULT NULL,
  `arabic_line3` varchar(100) DEFAULT NULL,
  `spanish_line1` varchar(25) DEFAULT NULL,
  `spanish_line2` varchar(25) DEFAULT NULL,
  `spanish_line3` varchar(25) DEFAULT NULL,
  `status` tinyint(4) NOT NULL DEFAULT 1
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `customer_messages`
--

INSERT INTO `customer_messages` (`id`, `device_type`, `message`, `media_path`, `english_line1`, `english_line2`, `english_line3`, `arabic_line1`, `arabic_line2`, `arabic_line3`, `spanish_line1`, `spanish_line2`, `spanish_line3`, `status`) VALUES
(1, 1, 'EntryPresenceLoopActivatedChipcoin', '/parcx/Media/download.png', 'test', 'Test26', 'Test353', '1', 'NA', 'NA', 'sp1', 'sp2', 'sp6565656', 1),
(2, 1, 'TakeEntryTicket', '', 'Please take the ticket', 'test1', 'tes81', 'NA', 'NA', 'NA', '', '', '', 1),
(3, 1, 'Processing', '', 'Processing..', 'Please Wait', '', 'NA', 'NA', 'NA', '', '', '', 1),
(4, 1, 'EntryStandBy', '', 'Demo Carpark ', 'FREE PARKING', '', 'NA', 'NA', 'NA', '', '', '', 1),
(5, 1, 'AccessAllowedEntry', '', 'Welcome', '', '', '', '', '', '', '', '', 1),
(6, 2, 'AccessDeniedUnpaidTicket', '', 'Unpaid Parking Fee', 'Pay at Payment Machine', '', '', '', '', '', '', '', 1),
(7, 2, 'AccessAllowedExit', '', 'Thank You for visiting', '', '', '', '', '', '', '', '', 1),
(8, 2, 'AccessDeniedInvalidTicket', '', 'Invalid Chipcoin', '', '', '', '', '', '', '', '', 1),
(9, 2, 'AccessDeniedWrongCarpark', '', 'Incorrect Car Park ', '', '', '', '', '', '', '', '', 1),
(10, 2, 'AccessDeniedWrongFacility', '', 'Incorrect Car Park ', '', '', '', '', '', '', '', '', 1),
(11, 2, 'AccessDeniedExitGrace', '', 'test', 'test', 'testhtbjb', '', '', '', '', '', '', 1),
(12, 3, 'AccessDeniedAlreadyExited', '', 'Ticket Already Exited', '', '', '', '', '', '', '', '', 1),
(13, 3, 'AccessDeniedBlacklisted', '', 'Ticket Blacklisted', '', '', '', '', '', '', '', '', 1),
(14, 3, 'EntryTicketBlacklisted', '', 'Ticket Blacklisted', '', '', '', '', '', '', '', '', 1),
(15, 3, 'NoVehiclePresent', '', 'No Vehicle Present', '', '', '', '', '', '', '', '', 1),
(16, 4, 'AccessDeniedTagUnknown', '', 'Tag Unknown', '', '', '', '', '', '', '', '', 1),
(17, 4, 'AccessDeniedWrongParkingZone', '', 'Incorrect Parking Zone', '', '', '', '', '', '', '', '', 1),
(18, 4, 'AccessDeniedCardExpired', '', 'Card Expired', '', '', '', '', '', '', '', '', 1),
(19, 4, 'AccessDeniedAntipassback', '', 'AntiPassBack Error', '', '', '', '', '', '', '', '', 1),
(20, 4, 'AccessDeniedTagDisabled', '', 'Tag disabled', '', '', '', '', '', '', '', '', 1),
(21, 0, 'OpenBarrierFromServer', '', 'Open Barrier From Server', '', '', '', '', '', '', '', '', 1),
(22, 5, 'CloseBarrierFromServer', '', 'Close Barrier From Server', '', '', '', '', '', '', '', '', 1),
(25, 5, 'EntryBarrierOpened', '', 'Barrier already Opened,Wait', '', '', '', '', '', '', '', '', 1),
(26, 5, 'ExitPresenceLoopActivatedChipcoin', '', 'Please insert chipcoin', '', '', '', '', '', '', '', '', 1),
(27, 5, 'ExitStandBy', '', 'Demo Carpark ', '', '', '', '', '', '', '', '', 1),
(28, 5, 'AccessDeniedReservationNotInTimePeriod', '', 'Not In Reservation Time Period', '', '', '', '', '', '', '', '', 1),
(29, 5, 'BarrierAlreadyOpened', '', 'Barrier already opened', 'Please wait to close the barrier', '', '', '', '', '', '', '', 1),
(30, 5, 'NoChipCoin', '', 'No chipcoin available on machine', '', '', '', '', '', '', '', '', 1),
(31, 5, 'OutOfService', '', 'Out Of Service', '', '', '', '', '', '', '', '', 1),
(32, 5, 'SetProtocolFailed', '', 'Cant connect to rfid', 'Please check the connection', '', '', '', '', '', '', '', 1),
(33, 5, 'AccessDeniedReservationNotUsed', '', 'Not Used Reservation', '', '', '', '', '', '', '', '', 1),
(34, 5, 'AccessDeniedReservationUsed', '', 'Used Reservation', '', '', '', '', '', '', '', '', 1),
(35, 5, 'AccessDeniedRegisteredAnotherCarpark', '', 'Not Registered to use this carpark', '', '', '', '', '', '', '', '', 1),
(36, 5, 'AccessDeniedRegistrationExpired', '', 'Parking Access  expired', '', '', '', '', '', '', '', '', 1),
(37, 5, 'ProcessingNumberPlate', '', 'Processing number plate', 'Please Wait', '', '', '', '', '', '', '', 1),
(38, 5, 'AccessDeniedReservation', '', 'Not in reservation time slot', '', '', '', '', '', '', '', '', 1),
(39, 5, 'NoWaletBalance', '', 'No balance in walet', '', '', '', '', '', '', '', '', 1),
(40, 5, 'AccessDeniedReservationExitGrace', '', 'Reservation Exit Grace period exceeded', '', '', '', '', '', '', '', '', 1),
(41, 5, 'AccessDeniedReservationAlreadyExited', '', 'Reservation Ticket Already Exited', '', '', '', '', '', '', '', '', 1),
(42, 5, 'NoReservation', '', 'No Reservation', '', '', '', '', '', '', '', '', 1),
(43, 5, 'OutOfTickets', '', 'Machine out of Tickets', '', '', '', '', '', '', '', '', 1),
(44, 5, 'PrintTicketFailed', '', 'Ticket printing failed', '', '', '', '', '', '', '', '', 1),
(45, 5, 'PrintFailedWrongPassword', '', 'Print failed wrong password', '', '', '', '', '', '', '', '', 1),
(46, 5, 'EntryPresenceLoopActivatedTicket', '', 'Push button for Ticket', 'Or Scan Access Card', '', 'NA', 'NA', 'NA', '', '', '', 1),
(47, 5, 'ExitPresenceLoopActivatedTicket', '', 'Please Sacn ticket', '', '', '', '', '', '', '', '', 1),
(48, 5, 'TakeEntryChipcoin', '', 'Please take the chipcoin', '', '', 'NA', 'NA', 'NA', '', '', '', 1),
(49, 5, 'EntryChipcoinBlacklisted', '', 'Chipcoin Blacklisted', '', '', '', '', '', '', '', '', 1),
(50, 5, 'InvalidEntry', '', 'Invalid Entry', '', '', '', '', '', '', '', '', 1),
(51, 5, 'WalletExpired', '', 'Wallet expired', '', '', '', '', '', '', '', '', 1),
(52, 5, 'WaletDisabled', '', 'Walet Disabled', '', '', '', '', '', '', '', '', 1),
(53, 5, 'NoPlate', '', 'No plate captured', '', '', '', '', '', '', '', '', 1),
(54, 5, 'RFIDError', '', 'Chip coin reading not working', '', '', '', '', '', '', '', '', 1),
(55, 5, 'RestartTerminal', '', 'Restarting Terminal ', '', '', 'NA', 'NA', 'NA', '', '', '', 1),
(56, 5, 'FreePassage', '', 'Free Passage Mode', '', '', 'NA', 'NA', 'NA', '', '', '', 1),
(57, 5, 'LaneClosed', '', 'Lane Temporarily Closed', '', '', 'NA', 'NA', 'NA', '', '', '', 1),
(58, 5, 'CarparkFull', '', 'NA', 'Carpark FULL', 'Entry Temporarily Blocked', 'NA', 'NA', 'NA', '', '', '', 1),
(59, 5, 'FreePassageEntry', '', 'Free Passage Entry', '', '', 'NA', 'NA', 'NA', '', '', '', 1),
(60, 5, 'OutOfServiceTicket', '', 'Out Of Service', 'Machine out of tickets', '', '', '', '', '', '', '', 1),
(62, 5, 'ExternalOpen ', '', 'Manual Open ', 'Please Enter ', '', '', '', '', '', '', '', 1),
(63, 5, 'TicketBoxError', '', 'Ticket Box Error', 'Unable to process entry ticket ', '', '', '', '', '', '', '', 1),
(64, 1, 'Message', '', '1', '2', '3', 'NA', 'NA', 'NA', '', '', '', 1),
(65, 3, 'Message', '', 'Line 1', 'Line 2', 'Line 3', 'NA', 'NA', 'NA', '', '', '', 1),
(66, 1, 'Entry Message', '', '', '', 'ssdsaddddddddddddddddddddddddddddddddddddddddddddd', '', '', '', '', '', '', 1),
(67, 1, 'Entry Message', '', '', '', 'ssdsaddddddddddddddddddddddddddddddddddddddddddddd', '', '', '', '', '', '', 1),
(91, 1, 'Testing', '', '1', '2', '3', '5', '4', '6', '', '', '', 1),
(92, 1, 'Testing1', '', '1', '', '3', '5', '4', '6', '', '', '', 1),
(97, 1, 'testtttt', '', '', '', '', '', '', '', '', '', '', 1),
(98, 1, 'EEEE', '', '123', '', '', '', '', '', '', '', '', 1),
(99, 1, 'Spanish Testing', '', '1', '2', '', '', '', '', '3', '4', '5', 1),
(100, 1, 'Test Test', '', '', '', '', '', '', '', '', '', '', 1),
(101, 1, 'Test44', '', '2323', 'rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr', '', '', '', '', '', '', '', 1),
(102, 1, 'Tes4343', '', '', 'kljlkjklkljkljjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj', '', '', '', '', '', '', '', 1),
(103, 1, 'Entry Message', '', '', '', 'ssdsaddddddddddddddddddddddddddddddddddddddddddddd', '', '', '', '', '', '', 1),
(104, 1, 'test', '', '1234', '', '', '1234', '', '', '1234', '', '', 1),
(105, 5, 'testentry', '', 'ewew', '', '', 'ewe', '', '', 'ewe', '', '', 1),
(106, 1, 'wert', '', 'wert', '', '', 'wert', '', '', 'wert', '', '', 1);

-- --------------------------------------------------------

--
-- Table structure for table `daemon_status`
--

CREATE TABLE `daemon_status` (
  `id` int(11) NOT NULL,
  `daemon_name` varchar(50) NOT NULL,
  `path` varchar(50) NOT NULL,
  `daemon_label` varchar(50) NOT NULL,
  `status` int(11) NOT NULL DEFAULT 1,
  `pid` int(11) DEFAULT 0,
  `start_time` datetime DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `daemon_status`
--

INSERT INTO `daemon_status` (`id`, `daemon_name`, `path`, `daemon_label`, `status`, `pid`, `start_time`) VALUES
(1, 'PX-RealTimeDaemon-V103-20200924', '/opt/parcx/Daemons', 'Parc Realtime Daemon', 1, 19721, '2020-09-28 12:07:16'),
(2, 'PX_DC_V_103_20200728', '/opt/parcx/Daemons', 'Parcx Dayclosure Daemon', 1, 0, NULL),
(5, 'PX_CloudUpload_V01_03_02_20210215', '/opt/parcx/Daemons', 'Cloud Download Daemon', 1, 10585, '2021-02-25 10:53:19'),
(6, 'PX_CloudDownload_V01_03_02_20210223', '/opt/parcx/Daemons', 'Cloud Download Daemon', 1, 25970, '2021-02-17 15:19:54'),
(7, 'ANPR/PXANPR_CAMERA1_V01_03_03', '/opt/parcx/Daemons', 'ANPR CAMERA 1', 1, 0, NULL),
(10, 'PX_ValetCloudUpload_V01_03_02_20210208', '/opt/parcx/Daemons', 'Cloud Valet Upload Daemon', 1, 30172, '2021-02-17 10:23:25');

-- --------------------------------------------------------

--
-- Table structure for table `DataExportSettings`
--

CREATE TABLE `DataExportSettings` (
  `id` int(11) NOT NULL,
  `table_name` varchar(50) NOT NULL,
  `export_flag` int(11) NOT NULL DEFAULT 0,
  `ssh` int(11) NOT NULL DEFAULT 0,
  `ftp` int(11) NOT NULL DEFAULT 0,
  `webservice` int(11) NOT NULL DEFAULT 0,
  `folder_path` varchar(250) NOT NULL,
  `label` varchar(50) NOT NULL,
  `ssh_interface_id` int(11) NOT NULL,
  `ftp_interface_id` int(11) NOT NULL,
  `local_folder_path` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `DataExportSettings`
--

INSERT INTO `DataExportSettings` (`id`, `table_name`, `export_flag`, `ssh`, `ftp`, `webservice`, `folder_path`, `label`, `ssh_interface_id`, `ftp_interface_id`, `local_folder_path`) VALUES
(1, 'revenue_payments', 0, 1, 1, 0, '', 'Revenue Payments', 3, 2, '/home/administrator/Documents/RevenuePaymentsExport/'),
(2, 'parking_movements', 0, 1, 1, 0, '', 'Parking Movements', 3, 2, '/home/ubuntu/Documents/Export/LocalExport/ParkingMovements/'),
(3, 'parking_movements_reservation', 0, 0, 0, 0, '', 'Parking Movements Reservation', 0, 0, '/home/ubuntu/Documents/Export/LocalExport/ParkingMovementsReservation/'),
(4, 'watchdog_device_alarms', 0, 0, 0, 0, '', 'Watchdog Device Alarms', 0, 0, '/home/ubuntu/Documents/Export/LocalExport/WatchdogDeviceAlarms/');

-- --------------------------------------------------------

--
-- Table structure for table `device_application_label`
--

CREATE TABLE `device_application_label` (
  `id` int(11) NOT NULL,
  `message` varchar(100) DEFAULT NULL,
  `english` varchar(200) DEFAULT NULL,
  `arabic` varchar(200) DEFAULT NULL,
  `status` int(11) NOT NULL DEFAULT 1
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `device_application_label`
--

INSERT INTO `device_application_label` (`id`, `message`, `english`, `arabic`, `status`) VALUES
(1, 'receipt_title', 'RECEIPT', 'إيصال', 1),
(2, 'shift_no', 'Shift No', 'رقم فترة المناوبة', 1),
(3, 'operator_title', 'Operator', 'المشغل أو العامل', 1),
(4, 'ticket_no', 'Ticket No', 'رقم التذكرة', 1),
(5, 'entry_time', 'Entry Time', 'وقت الدخول', 1),
(6, 'payment_time', 'Payment Time', 'وقت الدفع', 1),
(7, 'parking_duration', 'Parking Duration', 'مدة وقوف السيارات', 1),
(8, 'parking_fee', 'Parking Fees', 'رسوم الموقف', 1),
(9, 'discount', 'Discount', 'خصم', 1),
(10, 'vat', 'VAT', 'ضريبة القيمة المضافة', 1),
(11, 'vat_perc', 'vat', 'نسبة ضريبة القيمة المضافة', 1),
(12, 'amount_topay', 'Amount To Pay', 'المبلغ المستحق', 1),
(13, 'amount_paid', 'Amount Received', 'المبلغ المدفوع', 1),
(14, 'balance_amount', 'Change', 'يتغيرون', 1),
(15, 'lost_fees', 'Lost Fees', 'الرسوم المفقودة', 1),
(16, 'shift_starttime', 'Shift Start time', 'وقت بدء المناوبة', 1),
(17, 'shift_endtime', 'Shift End time', 'وقت نهاية التحول', 1),
(18, 'parkings', 'Parkings', 'موقف السيارات', 1),
(19, 'losts', 'Lost', 'ضائع', 1),
(20, 'discounts', 'Discounts', 'خصومات', 1),
(21, 'sales_recorded', 'Sales Recorded', 'المبيعات المسجلة', 1),
(22, 'payout_time', 'Payout Time', 'وقت السحب', 1),
(23, 'payout_amount', 'Payout Amount', 'المبلغ المسحوب', 1),
(24, 'lookup', 'Lookup', 'بحث', 1),
(25, 'payout', 'Payout', 'لمناوباتسحب', 1),
(26, 'shift_report', 'Shift Report', 'تقرير', 1),
(27, 'payment_report', 'Payment Report', 'تقرير الدفع', 1),
(28, 'barrier_report', 'Barrier Report', 'تقرير الحاجز', 1),
(29, 'barrier_open', 'Barrier Open', 'فتح الحاجز ', 1),
(30, 'pay', 'PAY', 'دفع', 1),
(31, 'receipt', 'RECEIPT', 'إيصال', 1),
(32, 'cancel', 'Cancel', 'إلغاء', 1),
(33, 'total_fee', 'Total Fee', 'المبلغ الإجمالي', 1),
(34, 'received', 'Received', 'تم الاستلام', 1),
(35, 'balance_amt', 'Change', 'المبلغ المتبقي', 1),
(36, 'ready_to_scan', 'Ready To Scan', 'على استعداد للمسح الضوئي', 1),
(37, 'issue_lost_ticket', 'Issue Lost Ticket', 'التذكرة المفقودة', 1),
(38, 'payment_successfull', 'Payment Successfull', 'تم الدفع بنجاح', 1),
(39, 'payment_not_successfull', 'Payment Not Successfull', 'لم تنجح عملية الدفع', 1),
(40, 'language_status', 'Active', 'Active', 1),
(41, 'reports', 'Reports', 'تقارير', 1),
(42, 'barrier_close', 'Barrier Close', 'إغلاق الحاجز', 1),
(43, 'manual_operation_report', 'Manual Operation Report', 'تقرير التشغيل اليدوي', 1),
(44, 'ticket_invalid', 'Ticket Invalid ', 'تذكرة غير صالحة', 1),
(45, 'no_vehicle_on_loop', 'No vehicle present on the loop ', 'لا يوجد سيارة فوق الحساس المعدني', 1),
(46, 'net_amt', 'Net Amount ', 'القيمة الصافية', 1),
(47, 'ticket_category', 'Category', 'الفئة', 1),
(48, 'warning', 'Warning', 'تحذير', 1),
(49, 'ticket_already_paid', 'Ticket Already Paid', 'تذكرة المدفوعة بالفعل', 1),
(50, 'shift_id', 'Shift ID', 'معرف التحول', 1),
(51, 'fee', 'Fee', 'رسوم', 1),
(52, 'gross_amt', 'Gross Amount', 'المبلغ الإجمالي', 1),
(53, 'search', 'Search', 'بحث', 1),
(54, 'export', 'Export', 'تصدير', 1),
(55, 'start_date', 'Start Date', 'تاريخ البدء', 1),
(56, 'end_date', 'End Date', 'تاريخ الانتهاء', 1),
(57, 'date', 'Date', 'تاريخ', 1),
(58, 'description', 'Description', 'وصف', 1),
(59, 'operation_type', 'Operation Type', 'نوع العملية', 1),
(60, 'none', 'None', 'لا شيء', 1),
(61, 'grace_time', 'Grace Time', 'وقت الشكر', 1),
(62, 'parking_ticket', 'Parking Ticket', 'تذكرة وقوف السيارات', 1),
(63, 'lost_ticket', 'Lost Ticket', 'تذكرة خسر', 1),
(64, 'pdf_export', 'PDF Export', 'تصدير قوات الدفاع الشعبي', 1),
(65, 'pdf_export_successfully', 'Report Exported Successfully', 'تقرير تصدير بنجاح', 1),
(66, 'error_while_exportng_pdf', 'Error while exporting PDF', 'خطأ أثناء تصدير PDF', 1),
(67, 'ok', 'OK', 'حسنا', 1),
(68, 'report_validation', 'No report for the selected date', 'لا تقرير للتاريخ المحدد', 1),
(69, 'discount_validation', 'Discount cannot be applied', 'لا يمكن تطبيق الخصم', 1),
(70, 'received_amt_validation', 'Received amount is less than gross amount ', 'الإجمالي المبلغ المستلم أقل من المبلغ', 1),
(71, 'controller_validation', 'Controller not available', 'المراقب غير متوفر', 1),
(72, 'tariff_swap', 'Tariff Swap', 'تبادل الرسوم الجمركية', 1),
(73, 'description_validation', 'Enter Description', 'أدخل الوصف', 1),
(74, 'date_selection_validation', 'Select date for report', 'حدد التاريخ للتقرير', 1),
(75, 'logout', 'LOGOUT', 'الخروج', 1),
(76, 'username', 'Username', 'اسم المستخدم', 1),
(77, 'password', 'Password', 'كلمه السر', 1),
(78, 'invalid_password', 'Invalid Password', 'رمز مرور خاطئ', 1),
(79, 'enter_password', 'Enter Password', 'أدخل كلمة المرور', 1),
(80, 'payment_unsuccessfull', 'Payment Unsuccessfull', 'دفع غير ناجحة', 1),
(81, 'shift_end_report', 'SHIFT END REPORT', 'تقرير نهاية التحول', 1),
(82, 'scan', 'READ', 'اقرأ', 1),
(83, 'shiftout', 'Close Shift', 'تحول قريب', 1),
(84, 'coupon', 'Coupon', 'كوبون', 1),
(85, 'no_data_to_print', 'No data to print', 'لا توجد بيانات مطبوعة', 1),
(86, 'invalid_credential', 'Invalid credential', 'شهادة اعتماد غير صالحة', 1),
(87, 'used_coupon', 'Used coupon', 'القسيمة المستخدمة', 1),
(88, 'coupon_no', 'Coupon No', 'رقم القسيمة', 1),
(89, 'discount_not_available', 'Discounts not available', 'الخصومات غير متوفرة', 1),
(90, 'carpark_name', 'Car park name', 'اسم موقف السيارات', 1),
(91, 'device_name', 'Device name', 'اسم الجهاز', 1),
(92, 'max_exit_grace_time', 'Maximum exit grace time', 'الحد الأقصى لوقت السماح للخروج', 1),
(93, 'shiftClosure', 'SHIFT CLOSURE', 'إغلاق التحول', 1),
(94, 'number_plate', 'Number Plate', 'لوحة الأرقام', 1),
(95, 'plate_prefix', 'Plate Prefix', 'بادئة اللوحة', 1),
(96, 'product_name', 'Product Name', 'اسم المنتج', 1),
(97, 'unit_price', 'Unit Price', 'سعر الوحده', 1),
(98, 'quantity', 'Quantity', 'كمية', 1),
(99, 'vouchers', 'Vouchers', 'القسائم', 1),
(100, 'manual', 'Manual', 'كتيب', 1),
(101, 'search_ticket', 'Search Ticket', 'تذكرة البحث', 1),
(102, 'search_plateno', 'Search Plate', 'لوحة البحث', 1),
(103, 'get_details', 'Get Details', 'احصل على التفاصيل', 1),
(104, 'subscriptions', 'Subscriptions', 'الاشتراكات', 1),
(105, 'from_time', 'From Time', 'من وقت', 1),
(106, 'to_time', 'To Time', 'الى وقت', 1),
(107, 'validation', 'Validation', 'التحقق من الصحة', 1),
(108, 'expiry_date', 'Expiry Date', 'تاريخ الانتهاء', 1),
(109, 'available_wallet_points', 'Available Wallet Point', 'نقاط المحفظة المتاحة', 1),
(110, 'wallet_expiry', 'Wallet Expiry', 'انتهاء المحفظة', 1),
(111, 'wallet_username', 'Wallet User', 'مستخدم المحفظة', 1),
(112, 'customer_id', 'Customer ID', 'هوية الزبون', 1),
(113, 'new_subscription', 'New Subscription', 'اشتراك جديد', 1),
(114, 'renew_subscription', 'Renew Subscription', 'تجديد الاشتراك', 1),
(115, 'customer_name', 'Customer Name', 'اسم الزبون', 1),
(116, 'customer_contact', 'Customer Contact', 'جهة اتصال العميل', 1),
(117, 'customer_email', 'Customer Email', 'البريد الإلكتروني للعميل', 1),
(118, 'product_quantity', 'Product Quantity', 'كمية المنتج', 1),
(119, 'validity', 'Validity', 'صلاحية', 1),
(120, 'already_exited', 'Already Exited', 'تم الخروج بالفعل', 1),
(121, 'time_to_exit_carpark', 'Time To Exit Carpark', 'حان الوقت للخروج من موقف السيارات', 1),
(122, 'collect_parking_fee', 'Collect Parking Fee', 'تحصيل رسوم المواقف', 1),
(123, 'invalid_entry', 'Invalid Entry', 'دخول غير صالح', 1),
(124, 'plate_unavilable', 'Plate Unavilable', 'لوحة غير متوفرة', 1),
(125, 'reservation_already_exited', 'Reservation Already Exited', 'تم الخروج من الحجز بالفعل', 1),
(126, 'invalid_carpark', 'Invalid Carpark', 'موقف سيارات غير صالح', 1),
(127, 'invalid_facility', 'Invalid Facility', 'منشأة غير صالحة', 1),
(128, 'invalid_reservation_carpark', 'Invalid Reservation Carpark', 'موقف سيارات غير صالح للحجز', 1),
(129, 'invalid_reservation_facility', 'Invalid Reservation Facility', 'تسهيل حجز غير صالح', 1),
(130, 'barrier_opened', 'Barrier Opened', 'تم فتح الحاجز', 1),
(131, 'reservation_access_allowed', 'Reservation Access Allowed', 'مسموح الوصول للحجز', 1),
(132, 'reservation_not_in_time_slot', 'Reservation not in time slot', 'الحجز ليس في الفترة الزمنية', 1),
(133, 'reservation_exit_grace_exceeded', 'Reservation exit grace excceeded', 'تم تجاوز مهلة الخروج بالحجز', 1),
(134, 'invalid_parking_zone', 'Invalid Parking Zone', 'منطقة وقوف السيارات غير صالحة', 1),
(135, 'access_not_started', 'Access Not started', 'الوصول لم يبدأ', 1),
(136, 'invalid_access_carpark', 'Inavlid Access Carpark', 'دخول غير صالح لموقف السيارات', 1),
(137, 'invalid_access_facility', 'Invalid Access Facility', 'تسهيل وصول غير صالح', 1),
(138, 'access_allowed', 'Access Allowed', 'الوصول المسموح', 1),
(139, 'access_expired', 'Access Expired', 'انتهت صلاحية الوصول', 1),
(140, 'sync', 'Sync Server', 'خادم المزامنة', 1),
(141, 'error_in_processing_request', 'Error in processing request', 'خطأ في معالجة الطلب', 1),
(142, 'product_sales', 'Product Sales', 'مبيعات المنتجات', 1),
(143, 'subscription_sales', 'Subscription Sales', 'مبيعات الاشتراك', 1),
(144, 'subscription_renewal', 'Subscription Renewal', 'تجديد الاشتراك', 1),
(145, 'physical_cash_collectection', 'Physical Cash Collection', 'التحصيل النقدي المادي', 1),
(146, 'close_shift', 'Close Shift', 'إغلاق التحول', 1),
(147, 'proceed', 'Proceed', 'تقدم', 1),
(148, 'product_doesnt_exists', 'Product Doesnot Exists', 'المنتج غير موجود', 1),
(149, 'subscription_doesnt_exists', 'Subscription Doesnot Exists', 'الاشتراك غير موجود', 1),
(150, 'amount_received_less', 'Received amount less than amount to pay', 'المبلغ المستلم أقل من المبلغ المطلوب دفعه', 1),
(151, 'physical_cash_form_title_line1', 'Log Physical Cash Collected', 'تسجيل النقد المادي المحصل', 1),
(152, 'physical_cash_form_title_line2', 'Enter the number of banknotes for each denomination and count of vouchers/coupons used', 'أدخل عدد الأوراق النقدية لكل فئة وعدد القسائم / القسائم المستخدمة', 1),
(153, 'continue_shift', 'Continue Shift', 'تواصل التحول', 1),
(155, 'voucher_quantity', 'Vouchers Quantity', 'كمية القسائم / كوبونات الخصم', 1),
(156, 'logged_in_as', 'Logged in As', 'تسجيل دخولك', 1),
(157, 'shift_closure_title_line1', 'Shift Closure', 'إغلاق التحول', 1),
(158, 'shift_closure_title_line2', 'Enter password to close shift', 'أدخل كلمة المرور لإغلاق التحول', 1),
(159, 'logout_title_line1', 'Logout', 'تسجيل خروج', 1),
(160, 'logout_title_line2', 'Enter password to logout', 'أدخل كلمة المرور لتسجيل الخروج', 1),
(161, 'manual_operation_title_line1', 'Manual Operation', 'التشغيل اليدوي', 1),
(162, 'manual_operation_title_line2', 'Select manual operation and reason for manual operation', 'حدد التشغيل اليدوي وسبب التشغيل اليدوي', 1),
(163, 'manual_operation', 'Manual Operation', 'التشغيل اليدوي', 1),
(166, 'operation_reason', 'Operation Reason', 'سبب العملية', 1),
(167, 'description_for_manual_operation', 'Description for manual operation', 'وصف للتشغيل اليدوي', 1),
(168, 'scan_coupon', 'Scan Coupon', 'مسح الكوبون', 1),
(169, 'scan_coupon_info_msg', 'Scan Coupon for discounts', 'امسح الكوبون للحصول على خصومات', 1),
(170, 'valid_coupon', 'Valid Coupon', 'قسيمة صالحة', 1),
(171, 'already_scanned_coupon', 'Already Scanned Coupon', 'تم مسح القسيمة بالفعل', 1),
(172, 'tag_id', 'Tag ID', 'معرف العلامة', 1),
(173, 'verify_subscription', 'Verify Subscription', 'تحقق من الاشتراك', 1),
(174, 'enter_data_to_search_subscription', 'Enter detail to search existing subscription for the customer', 'أدخل التفاصيل للبحث في الاشتراك الحالي للعميل', 1),
(175, 'no_existing_subscription', 'No Existing Subscription', 'لا يوجد اشتراك موجود', 1),
(176, 'existing_subscription_details', 'Existing Subscription Details', 'تفاصيل الاشتراك الحالية', 1),
(177, 'select_subscription_for_renewal', 'Select subscription for renewal', 'حدد الاشتراك للتجديد', 1),
(178, 'add_details_to_sell_new_subscription', 'Add details to sell new subscription', 'أضف التفاصيل لبيع اشتراك جديد', 1),
(179, 'company_name', 'Company Name', 'اسم الشركة', 1),
(180, 'fill_all_required_fields', 'Fill all the required fields', 'املأ جميع الحقول المطلوبة', 1),
(181, 'select_subscription', 'Select subscription', 'حدد الاشتراك', 1),
(182, 'select_customer_detail_to_search_subscription', 'Select customer detail to check existing subscription', 'حدد تفاصيل العميل للتحقق من الاشتراك الحالي', 1),
(183, 'purchased_date_time', 'Purchased Date Time', 'وقت تاريخ الشراء', 1),
(184, 'ticket_check', 'Ticket Check', 'فحص التذكرة', 1),
(185, 'scan_ticket_or_enter_plate_no', 'Scan a ticket or enter plate number to get details', 'امسح تذكرة أو أدخل رقم اللوحة للحصول على التفاصيل', 1),
(186, 'scan_ticket_to_search', 'Scan Ticket', 'مسح التذكرة', 1),
(187, 'enter_plate_no_to_search', 'Enter Plate Number to search', 'أدخل رقم اللوحة للبحث', 1),
(188, 'server_unavilable', 'Server not avilable', 'الخادم غير متوفر', 1),
(189, 'enter_scan_ticket_to_search', 'Enter or Scan ticket to search', 'أدخل أو امسح التذكرة للبحث', 1),
(190, 'ticket_details', 'Ticket Details', 'تفاصيل التذكرة', 1),
(191, 'scane_ticket_or_enter_plate_number', 'Scan ticket or enter plate number', 'امسح التذكرة أو أدخل رقم اللوحة', 1),
(192, 'available_plate_suggestions', 'Available plate suggestions', 'اقتراحات اللوحات المتاحة', 1),
(193, 'no_plate_suggestion_available', 'No plate suggestions available', 'لا توجد اقتراحات لوحة متاحة', 1),
(194, 'show_ticket_details', 'Show ticket details', 'إظهار تفاصيل التذكرة', 1),
(195, 'wallet_disabled', 'Wallet Disabled', 'المحفظة معطلة', 1),
(196, 'no_wallet_balance', 'No wallet balance', 'لا يوجد رصيد في المحفظة', 1),
(197, 'wallet_expired', 'wallet expired', 'المحفظة منتهية الصلاحية', 1),
(198, 'points_to_be_deducted', 'Points to be deducted', 'النقاط المراد حسمها', 1),
(200, 'wallet_details', 'Wallet Status', 'حالة المحفظة', 1),
(201, 'yes', 'Yes', 'نعم', 1),
(202, 'no', 'No', 'لا', 1),
(203, 'wallet_payment', 'Wallet Payment', 'دفع المحفظة', 1),
(204, 'wallet_payment_msg', 'You want to proceed with wallet payment', 'تريد متابعة دفع المحفظة', 1),
(205, 'cash', 'Cash', 'السيولة النقدية', 1),
(206, 'credit_card', 'Credit Card', 'Credit Card', 1),
(207, 'cash_credit_payment', 'Select cash or credit card for payment', 'حدد الدفع النقدي أو بطاقة الائتمان للدفع', 1),
(208, 'enter_authentication_code_title', 'Enter Credit Card authentication Code', 'أدخل رمز توثيق بطاقة الائتمان', 1),
(209, 'enter_authentication_code', 'Enter authentication code', 'أدخل رمز المصادقة', 1),
(210, 'reservation_exited_successfully', 'Reservation exited successfully', 'تم الخروج من الحجز بنجاح', 1),
(211, 'access_exited_successfully', 'Access exited successfully', 'تم الخروج من الوصول بنجاح', 1),
(212, 'wallet_points', 'Wallet Points', 'نقاط المحفظة', 1),
(213, 'download_settings_title', 'Download settings', 'إعدادات التنزيل', 1),
(214, 'download_settings_message', 'Proceed for downloading settings from server', 'تابع تنزيل الإعدادات من الخادم', 1),
(215, 'unable_to_procee_number_plate', 'Unable to process number plate', 'غير قادر على معالجة لوحة الأرقام', 1),
(216, 'chip_coin_connection_reader_failed', 'Chip coin connection reader failed', 'Chip coin connection reader failed', 1),
(217, 'perform_logout_or_shoftout', 'Perform logout or shiftout', 'نفذ الخروج أو التحول', 1),
(218, 'transaction_cancelled', 'Transaction cancelled', 'تم إلغاء المعاملة', 1),
(219, 'blacklisted_ticket', 'Blacklist Ticket', 'تذكرة القائمة السوداء', 1),
(220, 'unable_to_process_chipcoin', 'Unable to process chipcoin', 'غير قادر على معالجة تشيبكوين', 1),
(221, 'access_denied', 'Access Denied', 'تم الرفض', 1),
(222, 'place_chipcoin_to_finish_transaction', 'Place chipcoin to finish transaction', 'ضع تشيبكوين لإنهاء المعاملة', 1),
(223, 'one_time_exit_ticket', 'One time exit ticket', 'تذكرة خروج لمرة واحدة', 1),
(224, 'error_in_getting_plate_data', 'Error in getting plate data', 'خطأ في الحصول على بيانات اللوحة', 1),
(225, 'printer_unavilable', 'Printer Unavilable', 'الطابعة غير متوفرة', 1),
(226, 'unable_to_get_entry_details', 'Error in getting entry details', 'خطأ في الحصول على تفاصيل الدخول', 1),
(227, 'exit_grace_exceeded', 'Exit grace exceeded', 'تم تجاوز نعمة الخروج', 1),
(229, 'allow_database_update', 'Allow database update', 'السماح بتحديث قاعدة البيانات', 1),
(230, 'product_voucher_sales', 'Product/Voucher Sales', 'مبيعات المنتج / القسيمة', 1),
(231, 'select_product_for_sale_and_enter_quantity', 'Select product for sale and enter quantity', 'اختر منتجًا للبيع وأدخل الكمية', 1),
(232, 'no_products_vouchers_to_sale', 'No products or vouchers for sale', 'لا توجد منتجات أو قسائم للبيع', 1),
(233, 'enter_units_for_sale', 'Enter quantity', 'أدخل الكمية', 1),
(234, 'total_price', 'Total Price', 'السعر الكلي', 1),
(235, 'enter_proper_quantity_for_product_sale', 'Enter proper quantity for product sale', 'أدخل الكمية المناسبة لبيع المنتج', 1),
(236, 'issue_entry_ticket', 'Issue Entry Ticket', 'إصدار تذكرة دخول', 1);

-- --------------------------------------------------------

--
-- Table structure for table `device_maintenance`
--

CREATE TABLE `device_maintenance` (
  `id` int(11) NOT NULL,
  `setting_name` varchar(50) NOT NULL,
  `setting_value` varchar(100) NOT NULL,
  `setting_label` varchar(50) DEFAULT NULL,
  `description` varchar(100) NOT NULL,
  `enabled` int(11) NOT NULL DEFAULT 1
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `device_maintenance`
--

INSERT INTO `device_maintenance` (`id`, `setting_name`, `setting_value`, `setting_label`, `description`, `enabled`) VALUES
(1, 'db_list_for_backup', 'parcx_server,parcx_reporting', 'DB Backup List', 'comma separated values ', 1),
(2, 'db_backup_interval', 'day_closure', 'DB Backup Interval', 'day_closure', 1),
(3, 'db_backup_retention_period', '0', 'DB backup retention period', 'number of days', 1),
(4, 'db_backup_path_primary', '/opt/parcx/DBBackup', 'DB backup path primary', 'local machine', 1),
(5, 'db_backup_path_secondary', '', 'DB backup path secondary', 'ssh transfer secondaery db path', 1),
(6, 'db_for_transaction_deletion', 'parcx_reporting', 'DB for transaction deletion', 'name of the database', 1),
(7, 'transaction_retention_raw', '10', 'Transaction retention raw', 'this is for tables where we have all the individual transactions and are not summarized data or sett', 1),
(8, 'transaction_retention_summary', '1', 'Transaction retention summary', 'this is for the summarized data tables like revenue summary , traffic summary etc.', 1),
(9, 'log_file_backup_path_primary', '/opt/parcx/LogBackup', 'log file backup path primary', 'this is on the local machine', 1),
(10, 'log_file_backup_path_secondary', '/home/ubuntu/LogBackup/SSHExport', 'log file backup path secondary', 'this is on a secondary machine - ssh', 0),
(11, 'log_file_retention_period', '1', 'log file retention period', 'applicable only for the files in the Logs folder', 1),
(12, 'open_transaction_retention', '10', 'open transaction retention', '', 1),
(16, 'list_tables_for_transaction_deletion_raw', 'revenue_payments,parking_movements,parking_movements_access', 'list tables for transaction deletion raw', 'list of tables raw', 1),
(17, 'list_tables_for_transaction_deletion_summary', 'summary_daily_revenue,summary_parking_movements,parking_duration', 'list tables for transaction deletion summary', 'list of tables summary', 1),
(18, 'actual_log_path', '/opt/parcx', 'actual log path', 'Log folder path minus the Log flder', 1),
(19, 'anpr_image_path', '/opt/parcx/Logs/ANPR/Images', 'anpr image path', 'ANPR Image Folder', 1),
(20, 'anpr_retention', '30', 'anpr retention', 'Delete anpr images after retention days', 1),
(21, 'dbbackup_enabled', '1', 'dbbackup enabled', '1 for enable,0 for disabled', 1),
(22, 'logbackup_enabled', '1', 'logbackup enabled', '1 to enable,0 to disable', 1),
(23, 'deleteOpenTransactions_enabled', '1', 'deleteOpenTransactions enabled', '1 to enable,0 to disable', 0),
(24, 'deleteOldDBBackup_enabled', '1', 'deleteOldDBBackup enabled', '1 to enable,0 to disable', 0),
(25, 'deleteOldLogBackup_enabled', '1', 'deleteOldLogBackup enabled', '1 to enable,0 to disable', 0),
(26, 'deleteOldTransactions_enabled', '1', 'deleteOldTransactions enabled', '1 to enable,0 to disable', 0),
(27, 'deleteOldANPRImages_enabled', '1', 'deleteOldANPRImages enabled', '1 to enable,0 to disable', 0),
(28, 'log_backup_retention_period', '1', 'log backup retention period', 'applicable only for the local folder', 1),
(29, 'deleteOldLogFile_enabled', '1', 'deleteOldLogFile enabled', '1 to enable,0 to disable', 1),
(30, 'ssh_interface_id', '1', 'ssh interface id', 'Interface id from interface_settings table', 1),
(31, 'db_ssh_enabled', '1', 'db ssh enabled', '1 to enable ssh export,0 to disable', 1),
(32, 'log_ssh_enabled', '1', 'log ssh enabled', '1 to enable ssh export,0 to disable', 1);

-- --------------------------------------------------------

--
-- Table structure for table `interface_settings`
--

CREATE TABLE `interface_settings` (
  `id` int(11) NOT NULL,
  `interface_name` varchar(50) NOT NULL,
  `interface_type` varchar(50) NOT NULL,
  `host` varchar(100) NOT NULL,
  `username` varchar(20) NOT NULL,
  `password` varchar(25) NOT NULL,
  `url` varchar(100) NOT NULL,
  `folder_path` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `interface_settings`
--

INSERT INTO `interface_settings` (`id`, `interface_name`, `interface_type`, `host`, `username`, `password`, `url`, `folder_path`) VALUES
(1, 'SSH-Ubuntu', 'ssh', '192.168.1.207', 'ubuntu', '123', '', '/home/ubuntu/Documents/Export/SSHExport/'),
(2, 'FTP-Windows', 'ftp', '192.168.1.202', 'Alfalak', '123', 'ftp://192.168.1.202/', ''),
(3, 'SSH-JeenaPC', 'ssh', '192.168.1.183', 'jeena', '123456', '', '/home/jeena/Documents/SSHExport/');

-- --------------------------------------------------------

--
-- Table structure for table `manual_movements_reasons`
--

CREATE TABLE `manual_movements_reasons` (
  `id` int(11) NOT NULL,
  `reason` varchar(500) NOT NULL,
  `carpark_number` int(11) NOT NULL,
  `facility_number` int(11) NOT NULL,
  `status` int(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `manual_movements_reasons`
--

INSERT INTO `manual_movements_reasons` (`id`, `reason`, `carpark_number`, `facility_number`, `status`) VALUES
(1, 'Operation Reason', 1, 200005, 1),
(2, 'Ambulance Arrival', 1, 200005, 1),
(3, 'Police Van', 1, 200005, 1),
(4, 'Demo Open', 1, 200005, 0),
(5, 'Demo Close', 1, 200005, 0);

-- --------------------------------------------------------

--
-- Table structure for table `parking_ewallet`
--

CREATE TABLE `parking_ewallet` (
  `id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `user_name` varchar(50) DEFAULT NULL,
  `plate_number` varchar(50) DEFAULT NULL,
  `last_top_up_amount` double NOT NULL DEFAULT 0,
  `last_deducted_amount` double NOT NULL DEFAULT 0,
  `wallet_balance` double NOT NULL DEFAULT 0,
  `last_payment_date_time` varchar(25) DEFAULT NULL,
  `last_top_up_datetime` varchar(25) NOT NULL,
  `expiry_date` varchar(25) NOT NULL,
  `wallet_enabled` int(1) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `pgs_counter_alarm`
--

CREATE TABLE `pgs_counter_alarm` (
  `id` int(11) NOT NULL,
  `alarm_description` text NOT NULL,
  `date_time` datetime NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `pgs_counter_alarm`
--

INSERT INTO `pgs_counter_alarm` (`id`, `alarm_description`, `date_time`) VALUES
(1, 'Can\'t connect to MySQL server on \'192.168.1.12\' (111)', '2020-09-28 14:36:43'),
(2, 'Can\'t connect to MySQL server on \'192.168.1.12\' (111)', '2020-09-28 14:37:28');

-- --------------------------------------------------------

--
-- Table structure for table `pgs_interface_config`
--

CREATE TABLE `pgs_interface_config` (
  `id` int(11) NOT NULL,
  `setting_name` varchar(100) NOT NULL,
  `setting_value` varchar(100) NOT NULL,
  `setting_description` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `pgs_interface_config`
--

INSERT INTO `pgs_interface_config` (`id`, `setting_name`, `setting_value`, `setting_description`) VALUES
(1, 'pgs_interface', 'ake', 'Pgs interface'),
(2, 'parksol_webservice_url', 'http://localhost/parksol/', 'Parksol webservice URL'),
(4, 'ake_server_ip', '192.168.1.123', 'AKE server ip'),
(5, 'ake_db_user', 'parcx', 'AKE server user'),
(6, 'ake_db_password', 'Parcx123!', 'AKE server password'),
(7, 'ake_db', 'ips', 'AKE server database'),
(8, 'daemon_delay', '15', 'Daemon delay'),
(9, 'review_mode', '0', 'Daemon delay');

-- --------------------------------------------------------

--
-- Table structure for table `plates_ewallet`
--

CREATE TABLE `plates_ewallet` (
  `id` int(25) NOT NULL,
  `user_id` int(11) NOT NULL,
  `operator_id` int(11) NOT NULL,
  `country` varchar(50) DEFAULT NULL,
  `plate_number` varchar(50) DEFAULT NULL,
  `ticket_id` varchar(50) DEFAULT NULL,
  `status` int(11) NOT NULL DEFAULT 1,
  `create_datetime` datetime NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `pxcloud_download_settings`
--

CREATE TABLE `pxcloud_download_settings` (
  `id` int(11) NOT NULL,
  `table_name` varchar(25) NOT NULL,
  `label` varchar(100) NOT NULL,
  `day_closure` int(11) NOT NULL DEFAULT 0,
  `time_interval` int(10) NOT NULL COMMENT 'in seconds',
  `download_row_limit` int(11) NOT NULL DEFAULT 25,
  `enabled` int(11) NOT NULL DEFAULT 1,
  `cloud_download_date_time` timestamp NULL DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `pxcloud_download_settings`
--

INSERT INTO `pxcloud_download_settings` (`id`, `table_name`, `label`, `day_closure`, `time_interval`, `download_row_limit`, `enabled`, `cloud_download_date_time`) VALUES
(1, 'access_whitelist', 'Access whitelist', 0, 30, 10, 1, '2021-02-25 07:01:50'),
(2, 'parking_validation', 'Validations', 0, 30, 5, 1, '2021-02-25 05:45:58'),
(3, 'parking_reservation', 'Reservations', 0, 30, 5, 1, '2021-02-25 05:47:58'),
(4, 'plates_ewallet', 'Plates E wallet', 1, 30, 5, 1, '2021-02-25 20:30:00'),
(5, 'parking_ewallet', 'Revenue E wallet', 1, 30, 5, 1, '2021-02-25 06:32:17'),
(6, 'cooperate_users', 'Corporate user', 1, 30, 5, 1, '2021-03-01 20:30:00');

-- --------------------------------------------------------

--
-- Table structure for table `pxcloud_upload_settings`
--

CREATE TABLE `pxcloud_upload_settings` (
  `id` int(11) NOT NULL,
  `table_name` varchar(25) NOT NULL,
  `label` varchar(100) DEFAULT NULL,
  `day_closure` int(11) NOT NULL DEFAULT 0,
  `time_interval` int(10) NOT NULL COMMENT 'in seconds',
  `upload_row_limit` int(11) NOT NULL DEFAULT 25,
  `task` int(11) NOT NULL,
  `enabled` int(11) NOT NULL DEFAULT 1,
  `cloud_upload_date_time` timestamp NULL DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `pxcloud_upload_settings`
--

INSERT INTO `pxcloud_upload_settings` (`id`, `table_name`, `label`, `day_closure`, `time_interval`, `upload_row_limit`, `task`, `enabled`, `cloud_upload_date_time`) VALUES
(1, 'counters', 'Live Occupancy', 0, 60, 30, 1, 1, '2021-02-16 09:42:05'),
(2, 'watchdog_device_alarms', 'Device alarms', 0, 60, 5, 2, 1, '2021-02-16 09:45:05'),
(3, 'watchdog_device_status', 'Device status', 0, 60, 5, 3, 1, '2021-02-16 09:42:05'),
(4, 'open_transactions', 'Open transacions', 0, 60, 5, 4, 1, '2021-02-16 09:46:05'),
(5, 'parking_duration', 'Parking duration', 1, 30, 10, 5, 1, '2021-02-16 09:46:35'),
(6, 'summary_daily_revenue', 'Daily revenue summary', 1, 30, 10, 6, 1, '2021-02-16 09:47:06'),
(7, 'summary_parking_movements', 'Parking movements summary', 1, 30, 10, 7, 1, '2021-02-16 09:46:35'),
(8, 'parking_movements_access', 'Access movements', 0, 30, 5, 8, 1, '2021-02-18 06:16:08'),
(9, 'revenue_payments', 'Revenue payments', 0, 30, 5, 9, 1, '2021-02-17 11:20:24'),
(10, 'hourly_occupancy', 'Hourly occupancy', 0, 60, 5, 10, 1, '2021-02-16 10:38:37'),
(11, 'parking_revenue', 'Live revenue', 0, 60, 5, 11, 1, '2021-02-16 09:42:05'),
(12, 'parking_movements_manual', 'Manual movements', 0, 60, 5, 12, 1, '2021-02-16 09:45:05'),
(13, 'revenue_shifts', 'Revenue Shift', 0, 60, 5, 13, 1, '2021-02-16 09:53:36'),
(14, 'access_whitelist', 'Access wihtelist', 0, 60, 5, 14, 1, '2021-02-18 06:06:41'),
(15, 'parking_reservation', 'Reservation', 0, 60, 5, 15, 1, '2021-02-16 11:07:30'),
(16, 'valet_parking', 'Valet Parking', 0, 60, 5, 1, 0, '2021-02-14 08:06:17');

-- --------------------------------------------------------

--
-- Table structure for table `receipt_settings`
--

CREATE TABLE `receipt_settings` (
  `id` int(11) NOT NULL,
  `field_label` varchar(200) NOT NULL,
  `english` varchar(500) DEFAULT NULL,
  `arabic` varchar(500) CHARACTER SET utf8 DEFAULT NULL,
  `spanish` varchar(500) CHARACTER SET utf8 DEFAULT NULL,
  `status` int(11) NOT NULL DEFAULT 0,
  `category` int(11) NOT NULL DEFAULT 0,
  `order_of_printing` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `receipt_settings`
--

INSERT INTO `receipt_settings` (`id`, `field_label`, `english`, `arabic`, `spanish`, `status`, `category`, `order_of_printing`) VALUES
(1, 'receipt_no', 'Receipt No', 'عدد إيصال', 'Recibo No', 1, 1, 2),
(2, 'device_id', 'Device ID', 'معرف الجهاز', 'b', 0, 1, 5),
(3, 'trn', 'TRN NO', 'تريليون', 'TRN NO', 1, 1, 3),
(4, 'shift_no', 'Shift No', 'رقم فترة المناوبة', 'Shift No', 1, 1, 4),
(5, 'operator_name', 'Operator Name', 'اسم المشغل', 'Nombre del operador', 1, 1, 1),
(6, 'ticket_no', 'Ticket No', 'رقم التذكرة', 'Ticket No', 1, 2, 9),
(7, 'entry_time', 'Entry Time', 'وقت الدخول', 'Hora de entrada', 1, 2, 11),
(8, 'payment_time', 'Payment Time', 'وقت الدفع', 'Tiempo de pago', 1, 2, 10),
(9, 'parking_duration', 'Parking Duration', 'مدة وقوف السيارات', 'Duracion del estacionamiento', 1, 2, 12),
(10, 'payment_type', 'Payment Type', 'نوع الدفع', 'Tipo de pago', 0, 3, 23),
(11, 'exit_time', 'Exit Time', 'وقت الخروج', 'a', 0, 2, 0),
(12, 'parking_fee', 'Parking Fee', 'رسوم الموقف', 'Tarifa de estacionamiento', 1, 3, 14),
(13, 'net_amt', 'Net Amount', 'القيمة الصافية', 'Importe neto', 1, 3, 17),
(14, 'vat', 'VAT', 'ضريبة القيمة المضافة', 'IVA', 1, 3, 18),
(15, 'discount', 'Discount', 'خصم', 'Descuento', 1, 3, 15),
(16, 'discount_amt', 'Discount Amount', 'مقدار الخصم', 'Importe de descuento', 0, 3, 16),
(17, 'gross_amt', 'Gross Amount', 'المبلغ الإجمالي', 'Cantidad bruta', 1, 3, 20),
(18, 'parking_fee_receipt_title', 'TAX INVOICE', 'فاتورة ضريبية', 'FACTURA FISCAL', 1, 0, 0),
(19, 'lost_ticket_receipt_title', 'LOST RECEIPT', 'إيصال مفقود', 'RECIBO PERDIDO', 1, 0, 0),
(20, 'lost_fee', 'Lost Fee', 'الرسوم المفقودة', 'Tarifa de estacionamiento', 1, 3, 14),
(21, 'vat_perc', 'vat %', 'نسبة ضريبة القيمة ال', 'IVA %', 1, 3, 19),
(22, 'instruction', 'PLEASE SCAN ON EXIT', 'إيصال مفقود', 'POR FAVOR ESCANEAR AL SALIR', 1, 4, 30),
(23, 'greeting', 'THANK YOU FOR VISIT', 'شكرا لكم على الزيارة', 'GRACIAS POR VISITA', 1, 4, 30),
(24, 'carpark_name', 'CarPark', 'موقف السيارات', 'Estacionamiento', 1, 1, 7),
(25, 'device_name', 'DeviceName', 'اسم الجهاز', 'Nombre del dispositivo', 1, 1, 6),
(26, 'max_exit_datetime', 'Max Time to exit', 'الحد الأقصى لوقت السماح للخروج', 'Tiempo maximo para salir', 1, 2, 13),
(27, 'plate_number', 'Plate Number', 'رقم لوحة', 'Numero de placa', 1, 2, 8),
(28, 'amt_received', 'Amount Received', 'المبلغ المدفوع', 'Cantidad recibida', 1, 3, 21),
(29, 'change', 'Change', 'يتغيرون', 'equilibrar', 1, 3, 22),
(30, 'previous_wallet_balance', 'Previous Points', 'رصيد المحفظة السابق', 'Puntos anteriores', 1, 5, 25),
(31, 'wallet_expiry', 'Expiry', 'انتهاء صلاحية المحفظة', 'Expiracion', 1, 5, 24),
(32, 'current_wallet_balance', 'Available Points', 'رصيد المحفظة المتاح', 'Puntos disponibles', 1, 5, 27),
(33, 'deducted_wallet_balance', 'Points Deducted', 'رصيد المحفظة المقتطع', 'Puntos deducidos', 1, 5, 26),
(34, 'wallet_username', 'User', 'اسم مستخدم المحفظة', 'nombre de usuario', 1, 5, 28),
(35, 'exit_pass', 'EXIT PASS', 'ممر الخروج', 'PASE DE SALIDA', 1, 0, 0),
(36, 'product_name', 'Product Name', 'اسم المنتج', 'nombre del producto', 1, 2, 11);

-- --------------------------------------------------------

--
-- Table structure for table `reservation_user`
--

CREATE TABLE `reservation_user` (
  `user_id` int(11) NOT NULL,
  `password` varchar(100) NOT NULL,
  `first_name` varchar(25) NOT NULL,
  `last_name` varchar(25) NOT NULL,
  `company_name` varchar(50) NOT NULL,
  `email` varchar(30) NOT NULL,
  `address_line1` varchar(100) NOT NULL,
  `address_line2` varchar(100) DEFAULT NULL,
  `city` varchar(100) DEFAULT NULL,
  `country` varchar(50) DEFAULT NULL,
  `mobile` varchar(15) DEFAULT NULL,
  `phone` varchar(15) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `revenue_banknote_denominations`
--

CREATE TABLE `revenue_banknote_denominations` (
  `id` int(11) NOT NULL,
  `country` varchar(30) NOT NULL,
  `currency` varchar(5) NOT NULL,
  `denomination1` varchar(10) NOT NULL,
  `type1` varchar(10) NOT NULL DEFAULT '''Banknote''',
  `denomination2` varchar(10) NOT NULL,
  `type2` varchar(10) NOT NULL DEFAULT '''Banknote''',
  `denomination3` varchar(10) NOT NULL,
  `type3` varchar(10) NOT NULL DEFAULT '''Banknote''',
  `denomination4` varchar(10) NOT NULL,
  `type4` varchar(10) NOT NULL DEFAULT '''Banknote''',
  `denomination5` varchar(10) NOT NULL,
  `type5` varchar(10) NOT NULL DEFAULT '''Banknote''',
  `denomination6` varchar(10) NOT NULL,
  `type6` varchar(10) NOT NULL DEFAULT '''Banknote''',
  `denomination7` varchar(10) NOT NULL,
  `type7` varchar(10) NOT NULL DEFAULT '''Banknote''',
  `denomination8` varchar(10) NOT NULL,
  `type8` varchar(10) NOT NULL DEFAULT '''Banknote''',
  `type9` varchar(10) NOT NULL DEFAULT '''Banknote''',
  `denomination9` varchar(10) NOT NULL,
  `denomination10` varchar(10) NOT NULL,
  `type10` varchar(10) NOT NULL DEFAULT 'Banknote'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `revenue_banknote_denominations`
--

INSERT INTO `revenue_banknote_denominations` (`id`, `country`, `currency`, `denomination1`, `type1`, `denomination2`, `type2`, `denomination3`, `type3`, `denomination4`, `type4`, `denomination5`, `type5`, `denomination6`, `type6`, `denomination7`, `type7`, `denomination8`, `type8`, `type9`, `denomination9`, `denomination10`, `type10`) VALUES
(1, 'UAE', 'AED', '5', 'Banknote', '10', 'Banknote', '20', 'Banknote', '50', 'Banknote', '100', 'Banknote', '200', 'Banknote', '500', 'Banknote', '1000', 'Banknote', 'Banknote', '0', '0', 'Banknote');

-- --------------------------------------------------------

--
-- Table structure for table `revenue_coupons_whitelist`
--

CREATE TABLE `revenue_coupons_whitelist` (
  `id` int(25) NOT NULL,
  `discount_id` int(5) NOT NULL,
  `carpark_number` int(5) NOT NULL DEFAULT 1,
  `discount_category` int(5) NOT NULL DEFAULT 1,
  `coupon_id` varchar(50) NOT NULL,
  `transaction_id` varchar(25) NOT NULL,
  `DiscountName` varchar(50) NOT NULL DEFAULT 'Ecommerce',
  `status` int(1) NOT NULL DEFAULT 1,
  `coupon_usage` tinyint(1) NOT NULL DEFAULT 0,
  `ticket_id` varchar(50) NOT NULL DEFAULT '0',
  `coupon_usage_datetime` datetime DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `revenue_discounts`
--

CREATE TABLE `revenue_discounts` (
  `id` int(11) NOT NULL,
  `discount_id` int(5) NOT NULL,
  `discount_name` tinytext CHARACTER SET latin1 NOT NULL,
  `discount_type` varchar(25) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'percentage',
  `discount_option` varchar(25) COLLATE utf8_unicode_ci NOT NULL,
  `discount_category` int(4) NOT NULL,
  `discount_value` int(11) NOT NULL,
  `carpark_number` varchar(25) COLLATE utf8_unicode_ci NOT NULL,
  `status` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

--
-- Table structure for table `revenue_dynamic_parking_rates`
--

CREATE TABLE `revenue_dynamic_parking_rates` (
  `id` int(11) NOT NULL,
  `parking_rate_label` varchar(50) NOT NULL,
  `datetime` date DEFAULT NULL,
  `day` varchar(15) DEFAULT NULL,
  `rate_type` int(11) NOT NULL,
  `parking_rate` varchar(25) NOT NULL,
  `status` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `revenue_fixed_parking_rates`
--

CREATE TABLE `revenue_fixed_parking_rates` (
  `id` int(11) NOT NULL,
  `fixed_fees` varchar(50) NOT NULL,
  `parking_rates1` int(11) NOT NULL,
  `parking_rates2` int(11) NOT NULL,
  `parking_rates3` int(11) NOT NULL,
  `parking_rates4` int(11) NOT NULL,
  `parking_rates5` int(11) NOT NULL,
  `rate_labels` varchar(50) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `revenue_fixed_parking_rates`
--

INSERT INTO `revenue_fixed_parking_rates` (`id`, `fixed_fees`, `parking_rates1`, `parking_rates2`, `parking_rates3`, `parking_rates4`, `parking_rates5`, `rate_labels`) VALUES
(1, 'lost_ticket_fee', 10, 250, 30, 20, 20, 'Lost fee'),
(2, 'entry_grace_period_mins', 2, 5, 1, 30, 22, 'Entry grace period minute'),
(3, 'entry_grace_period_hours', 0, 0, 0, 1, 1, 'Entry grace period hours'),
(4, 'recurring_fee', 5, 0, 0, 0, 0, 'Recurring fee'),
(5, 'administration_fee', 0, 1, 5, 0, 0, 'Administration fee'),
(6, 'ticket_replacement_fee', 0, 1, 10, 0, 0, 'Ticket replacement fee'),
(7, 'exit_grace_period_mins', 5, 5, 15, 0, 0, 'Exit grace period minutes'),
(8, 'exit_grace_period_hours', 0, 0, 0, 0, 0, 'Exit grace period hours'),
(9, 'fixed_parking_rate', 50, 200, 30, 70, 150, 'Fixed parking rate'),
(10, 'reservation_entry_grace_period_mins', 5, 5, 1, 30, 22, 'Reservation Entry grace period minutes'),
(11, 'reservation_entry_grace_period_hours', 0, 0, 0, 1, 1, 'Reservation Entry grace period hours'),
(12, 'reservation_exit_grace_period_mins', 5, 5, 15, 0, 0, 'Reservation Exit grace period minutes'),
(13, 'reservation_exit_grace_period_hours', 0, 0, 0, 0, 0, 'Reservation Exit grace period hours');

-- --------------------------------------------------------

--
-- Table structure for table `revenue_parking_rates`
--

CREATE TABLE `revenue_parking_rates` (
  `id` int(11) NOT NULL,
  `time_unit_parking_rates1` text NOT NULL,
  `time_duration_parking_rates1` int(11) NOT NULL,
  `parking_rates1` int(11) NOT NULL,
  `time_unit_parking_rates2` text NOT NULL,
  `time_duration_parking_rates2` int(11) NOT NULL,
  `parking_rates2` int(11) NOT NULL,
  `time_unit_parking_rates3` text NOT NULL,
  `time_duration_parking_rates3` int(11) NOT NULL,
  `parking_rates3` int(11) NOT NULL,
  `time_unit_parking_rates4` text NOT NULL,
  `time_duration_parking_rates4` int(11) NOT NULL,
  `parking_rates4` int(11) NOT NULL,
  `time_unit_parking_rates5` text NOT NULL,
  `time_duration_parking_rates5` int(11) NOT NULL,
  `parking_rates5` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `revenue_parking_rates`
--

INSERT INTO `revenue_parking_rates` (`id`, `time_unit_parking_rates1`, `time_duration_parking_rates1`, `parking_rates1`, `time_unit_parking_rates2`, `time_duration_parking_rates2`, `parking_rates2`, `time_unit_parking_rates3`, `time_duration_parking_rates3`, `parking_rates3`, `time_unit_parking_rates4`, `time_duration_parking_rates4`, `parking_rates4`, `time_unit_parking_rates5`, `time_duration_parking_rates5`, `parking_rates5`) VALUES
(1, 'mins', 30, 100, 'mins', 30, 10, 'hours', 1, 20, 'hours', 1, 10, 'hours', 1, 30),
(2, 'hours', 1, 100, 'hours', 1, 20, 'hours', 2, 30, 'hours', 2, 20, 'hours', 2, 30),
(3, 'hours', 2, 100, 'hours', 2, 30, 'hours', 3, 40, 'mins', 3, 22, 'hours', 3, 30),
(4, 'hours', 3, 20, 'hours', 3, 40, 'hours', 4, 50, 'hours', 4, 30, 'hours', 4, 50),
(5, 'hours', 4, 25, 'hours', 4, 50, 'hours', 5, 60, 'hours', 5, 30, 'hours', 5, 50),
(6, 'hours', 5, 30, 'hours', 5, 60, 'hours', 6, 70, 'hours', 6, 30, 'hours', 6, 50),
(7, 'hours', 6, 35, 'hours', 6, 70, 'hours', 7, 80, 'hours', 7, 40, 'hours', 7, 70),
(8, 'hours', 7, 40, 'hours', 7, 80, 'hours', 8, 90, 'hours', 8, 40, 'hours', 8, 70),
(9, 'hours', 8, 45, 'hours', 8, 90, 'hours', 9, 100, 'hours', 9, 40, 'hours', 9, 70),
(10, 'hours', 9, 50, 'hours', 9, 100, 'hours', 10, 110, 'hours', 10, 40, 'hours', 10, 70);

-- --------------------------------------------------------

--
-- Table structure for table `revenue_parking_rate_label`
--

CREATE TABLE `revenue_parking_rate_label` (
  `id` int(11) NOT NULL,
  `parking_rate1` varchar(50) NOT NULL,
  `parking_rate2` varchar(50) NOT NULL,
  `parking_rate3` varchar(50) NOT NULL,
  `parking_rate4` varchar(50) NOT NULL,
  `parking_rate5` varchar(50) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `revenue_parking_rate_label`
--

INSERT INTO `revenue_parking_rate_label` (`id`, `parking_rate1`, `parking_rate2`, `parking_rate3`, `parking_rate4`, `parking_rate5`) VALUES
(1, 'General Rate ', 'Handicap Rate ', 'Holiday Rate ', 'Special Rate', 'Staff Rate');

-- --------------------------------------------------------

--
-- Table structure for table `revenue_products`
--

CREATE TABLE `revenue_products` (
  `id` int(11) NOT NULL,
  `product_name` varchar(50) NOT NULL,
  `unit_price` int(11) NOT NULL DEFAULT 0,
  `status` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `system_carparks`
--

CREATE TABLE `system_carparks` (
  `carpark_id` int(11) NOT NULL,
  `carpark_number` int(11) NOT NULL,
  `carpark_name` varchar(100) NOT NULL,
  `rate_plan` varchar(50) DEFAULT NULL,
  `rate_type` int(1) NOT NULL DEFAULT 1,
  `reservation_rate_plan` varchar(50) DEFAULT NULL,
  `total_spaces` int(11) NOT NULL,
  `occupancy_threshold` int(11) NOT NULL,
  `shortterm_spaces` int(11) NOT NULL,
  `reservation_spaces` int(11) NOT NULL,
  `access_spaces` int(11) NOT NULL,
  `facility_number` int(11) NOT NULL,
  `facility_name` varchar(100) DEFAULT NULL,
  `status` int(11) NOT NULL DEFAULT 1,
  `user_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `system_chipcoin`
--

CREATE TABLE `system_chipcoin` (
  `serial_number` varchar(20) NOT NULL,
  `tag_id` varchar(25) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `system_devices`
--

CREATE TABLE `system_devices` (
  `id` int(5) NOT NULL,
  `device_category` int(5) NOT NULL DEFAULT 0,
  `device_category_name` varchar(50) DEFAULT NULL,
  `device_name` varchar(25) DEFAULT NULL,
  `device_ip` varchar(50) DEFAULT NULL,
  `device_number` int(3) NOT NULL DEFAULT 0,
  `device_enabled` tinyint(1) NOT NULL DEFAULT 0,
  `carpark_number` int(5) DEFAULT 0,
  `carpark_name` varchar(50) DEFAULT NULL,
  `parking_zone` varchar(50) DEFAULT NULL,
  `rate_plan` text DEFAULT NULL,
  `reservation_rate_plan` varchar(25) DEFAULT NULL,
  `rate_type` int(11) DEFAULT NULL,
  `camera_id` int(11) NOT NULL DEFAULT 0,
  `camera_index` int(11) NOT NULL DEFAULT 0,
  `customer_receipt_mandatory` tinyint(1) NOT NULL DEFAULT 0,
  `shift_receipt_mandatory` tinyint(11) NOT NULL DEFAULT 0,
  `shift_physical_cash_count_required` tinyint(1) NOT NULL DEFAULT 0,
  `synch_whitelist_flag` int(11) NOT NULL DEFAULT 0,
  `last_updated_whitelist` datetime DEFAULT NULL,
  `issue_lost` int(11) NOT NULL DEFAULT 1,
  `anpr_enabled` tinyint(1) NOT NULL DEFAULT 0,
  `wiegand_enabled` tinyint(1) NOT NULL DEFAULT 0,
  `access_enabled` tinyint(1) NOT NULL DEFAULT 0,
  `reservation_enabled` tinyint(1) NOT NULL DEFAULT 0,
  `review_mode` int(1) NOT NULL DEFAULT 0,
  `device_function` int(1) NOT NULL DEFAULT 0,
  `barrier_open_time_limit` int(11) DEFAULT 10,
  `duration_hold_barrier_open` int(11) NOT NULL DEFAULT 100,
  `display_anpr_image` int(1) DEFAULT 0,
  `barrier_open_status_type` int(1) NOT NULL DEFAULT 0,
  `bms_status_enabled` int(1) NOT NULL DEFAULT 0,
  `barrier_status_monitoring` int(1) NOT NULL DEFAULT 0,
  `wiegand2_enabled` int(1) NOT NULL DEFAULT 0,
  `server_handshake_interval` int(11) NOT NULL DEFAULT 90,
  `plate_capturing_wait_delay` int(11) NOT NULL DEFAULT 7,
  `quick_barrier_close` int(1) NOT NULL DEFAULT 0,
  `payment_enabled_exit` int(1) NOT NULL DEFAULT 0,
  `facility_number` int(11) NOT NULL DEFAULT 1,
  `facility_name` varchar(50) NOT NULL DEFAULT 'AFME',
  `user_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `system_device_category`
--

CREATE TABLE `system_device_category` (
  `id` int(11) NOT NULL,
  `category_name` varchar(50) NOT NULL,
  `image_url` varchar(100) NOT NULL,
  `group` varchar(25) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `system_device_category`
--

INSERT INTO `system_device_category` (`id`, `category_name`, `image_url`, `group`) VALUES
(1, 'Entry Column', 'column', 'columns'),
(2, 'Exit Column', 'column', 'columns'),
(3, 'Cashier', 'cashier-pos', 'payment_machines'),
(4, 'APM', 'payment-machine', 'payment_machines'),
(5, 'Handheld', 'handheld-pos', 'payment_machines'),
(6, 'Entry Controller', 'controller', 'controllers'),
(7, 'Exit Controller', 'controller', 'controllers'),
(8, 'Camera', 'cctv', 'camera'),
(9, 'VMS', 'vms', 'vms'),
(10, 'Key Manager', 'cashier-pos', 'valet'),
(11, 'Podium Manager', 'cashier-pos', 'valet');

-- --------------------------------------------------------

--
-- Table structure for table `system_navigation`
--

CREATE TABLE `system_navigation` (
  `menu_id` int(11) NOT NULL,
  `group_id` int(11) NOT NULL,
  `menu_name` varchar(50) NOT NULL,
  `menu_link` varchar(60) NOT NULL,
  `menu_icon` varchar(25) NOT NULL,
  `menu_order_index` int(11) NOT NULL,
  `menu_status` tinyint(1) NOT NULL DEFAULT 1
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `system_navigation`
--

INSERT INTO `system_navigation` (`menu_id`, `group_id`, `menu_name`, `menu_link`, `menu_icon`, `menu_order_index`, `menu_status`) VALUES
(23, 7, 'Devices', 'modules/config/system_device.php', 'far fa-circle', 4, 1),
(33, 5, 'Finance', 'modules/dashboard/finance_facility.php', 'fa fa-university', 3, 1),
(34, 5, 'Occupancy', 'modules/dashboard/occupancy.php', 'fas fa-tachometer-alt', 2, 1),
(35, 1, 'Shift Report', 'modules/reports/revenue/shift_report.php', 'far fa-circle', 3, 1),
(37, 1, 'Payment transactions', 'modules/reports/revenue/payment_transactions.php', 'far fa-circle', 1, 1),
(45, 2, 'Manual Movements', 'modules/reports/parking/manual_movements.php', 'far fa-circle', 3, 1),
(46, 1, 'Revenue Summary', 'modules/reports/revenue/revenue_summary.php', 'far fa-circle', 2, 1),
(47, 3, 'Watchdog Network Logs', 'modules/reports/system/watchdog_network.php', 'far fa-circle', 2, 1),
(48, 3, 'Watchdog Device Alarms', 'modules/reports/system/watchdog_device_alarms.php', 'far fa-circle', 1, 1),
(49, 4, 'Manage User', 'modules/users/manage_users.php', 'far fa-circle', 5, 1),
(50, 7, 'User Role', 'modules/users/user_roles.php', 'far fa-circle', 5, 1),
(51, 7, 'Counter Alerts', 'modules/config/counter_alerts.php', 'far fa-circle', 12, 1),
(52, 7, 'Correction Factor', 'modules/counters/correction_factor.php', 'far fa-circle', 9, 1),
(54, 2, 'Access report', 'modules/reports/parking/access_reports.php', 'far fa-circle', 2, 1),
(56, 2, 'Parking duration', 'modules/reports/parking/parking_duration.php', 'far fa-circle', 10, 1),
(57, 2, 'Traffic report', 'modules/reports/parking/traffic_report.php', 'far fa-circle', 9, 1),
(58, 2, 'Visitor frequency', 'modules/reports/parking/visitor_frequency.php', 'far fa-circle', 11, 1),
(59, 5, 'Valet', 'modules/dashboard/valet.php', 'fa fa-car', 6, 1),
(60, 2, 'Valet Parking', 'modules/reports/parking/valet_parking.php', 'far fa-circle', 17, 1),
(61, 5, 'Device Status', 'modules/dashboard/device_status_facility.php', 'fa fa-flag', 1, 1),
(63, 4, 'Dynamic Parking Rates', 'modules/config/dynamic_parking_rates.php', 'far fa-circle', 2, 1),
(64, 4, 'Discounts', 'modules/config/discounts.php', 'far fa-circle', 3, 1),
(65, 1, 'Discount Summary', 'modules/reports/revenue/discount_usage.php', 'far fa-circle', 5, 0),
(66, 4, 'Parking Rates', 'modules/config/parking_rates.php', 'far fa-circle', 1, 1),
(67, 2, 'Validation Report', 'modules/reports/parking/validation_report.php', 'far fa-circle', 12, 1),
(68, 2, 'Blacklist Report', 'modules/reports/parking/blacklist_report.php', 'far fa-circle', 4, 1),
(69, 4, 'Access Whitelist', 'modules/config/access_whitelist.php', 'far fa-circle', 4, 1),
(71, 1, 'Credit card Payment transactions', 'modules/reports/revenue/creditcard_payment_transactions.php', 'far fa-circle', 6, 1),
(72, 1, 'APM Refill Report', 'modules/reports/revenue/apm_refill_report.php', 'far fa-circle', 8, 1),
(73, 2, 'Open Transactions', 'modules/reports/parking/open_transactions.php', 'far fa-circle', 1, 1),
(74, 1, 'APM Cash Levels', 'modules/reports/revenue/cash_levels.php', 'far fa-circle', 7, 1),
(76, 7, 'APM Threshold', 'modules/config/cash_levels.php', 'far fa-circle', 7, 0),
(77, 5, 'Overview', 'modules/dashboard/overview.php', 'far fa-circle', 1, 0),
(78, 6, 'Manual Operations', 'modules/operation/manualOperation.php', 'fa fa-hand-paper-o', 1, 1),
(79, 7, 'VMS Shedule', 'modules/config/vms_shedule.php', 'far fa-circle', 11, 1),
(80, 7, 'VMS Devices', 'modules/config/vms_devices.php', 'far fa-circle', 10, 1),
(81, 7, 'System Settings', 'modules/config/system_settings.php', 'far fa-circle', 1, 1),
(82, 1, 'APM Payout Report', 'modules/reports/revenue/apm_payout_report.php', 'far fa-circle', 9, 1),
(83, 7, 'Export settings', 'modules/config/export_settings.php', 'far fa-circle', 8, 1),
(84, 2, 'Plates captured', 'modules/reports/parking/plates_captured.php', 'far fa-circle', 5, 1),
(85, 1, 'Transaction Exceptions', 'modules/reports/revenue/payment_transaction_exceptions.php', 'far fa-circle', 10, 1),
(86, 7, 'Carpark', 'modules/config/system_carpark.php', 'far fa-circle', 3, 1),
(87, 7, 'Facility', 'modules/config/system_facility.php', 'far fa-circle', 2, 1),
(88, 5, 'ANPR Review', 'modules/dashboard/plate_review.php', 'fa fa-taxi', 4, 1),
(89, 4, 'Customer Messages', 'modules/config/customer_messages.php', 'far fa-circle', 5, 1),
(90, 2, 'Parking Movements', 'modules/reports/parking/parking_movements.php', 'far fa-circle', 1, 1),
(91, 1, 'VAT Report', 'modules/reports/revenue/vat_report.php', 'far fa-circle', 4, 1),
(92, 5, 'Live Report', 'modules/dashboard/live_parking.php', 'fa fa-car', 5, 1),
(93, 7, 'Daemon status', 'modules/config/daemon_status.php', 'far fa-circle', 6, 1),
(94, 7, 'View logs', 'modules/config/view_logs.php', 'far fa-circle', 7, 1),
(95, 4, 'VMS configuraton', 'modules/config/vms_config.php', 'far fa-circle', 6, 1),
(98, 2, 'Visitor Frequency RealTime', 'modules/reports/parking/visitor_frequency_realtime.php', 'far fa-circle', 8, 1),
(99, 4, 'Products', 'modules/config/system_product.php', 'far fa-circle', 6, 1),
(100, 4, 'Validators', 'modules/config/validator-user.php', 'far fa-circle', 7, 1),
(102, 2, 'Ticket Details', 'modules/reports/parking/ticket_details.php', 'far fa-circle', 7, 1),
(103, 7, 'Whitelist group policy', 'modules/config/whitelist_group_policy.php', 'far fa-circle', 2, 1),
(104, 5, 'Key/Podium Manager', 'modules/dashboard/key_podium_manager.php', 'fa fa-car', 7, 1);

-- --------------------------------------------------------

--
-- Table structure for table `system_navigation_group`
--

CREATE TABLE `system_navigation_group` (
  `group_id` int(11) NOT NULL,
  `group_name` varchar(50) NOT NULL,
  `group_icon` varchar(50) NOT NULL,
  `group_order_index` int(11) NOT NULL,
  `header_id` int(11) NOT NULL,
  `group_expand` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `system_navigation_group`
--

INSERT INTO `system_navigation_group` (`group_id`, `group_name`, `group_icon`, `group_order_index`, `header_id`, `group_expand`) VALUES
(1, 'REVENUE', 'fa fa-money-bill', 1, 2, 1),
(2, 'PARKING', 'fa fa-car', 2, 2, 1),
(3, 'WATCHDOG', 'fa fa-server', 3, 2, 1),
(4, 'CONFIGURATION', 'fa fa-sliders', 2, 3, 1),
(5, 'DASHBOARD', 'fa fa-server', 1, 1, 0),
(6, 'MANUAL OPERATION', 'fa-hand-paper-o', 1, 3, 0),
(7, 'SERVICE TECHNICIAN', 'fa fa-wrench', 3, 3, 1);

-- --------------------------------------------------------

--
-- Table structure for table `system_navigation_header`
--

CREATE TABLE `system_navigation_header` (
  `header_id` int(11) NOT NULL,
  `header_name` varchar(50) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `system_navigation_header`
--

INSERT INTO `system_navigation_header` (`header_id`, `header_name`) VALUES
(1, 'DASHBOARD'),
(2, 'REPORTS'),
(3, 'OPERATIONS');

-- --------------------------------------------------------

--
-- Table structure for table `system_parking_zone`
--

CREATE TABLE `system_parking_zone` (
  `parking_zone_number` int(11) NOT NULL,
  `parking_zone_name` varchar(25) NOT NULL,
  `carpark_number` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `system_products`
--

CREATE TABLE `system_products` (
  `product_id` int(11) NOT NULL,
  `product_name` varchar(100) NOT NULL,
  `date_validity` int(1) NOT NULL DEFAULT 0,
  `start_date` date DEFAULT NULL,
  `expiry_date` date DEFAULT NULL,
  `validity_days` text DEFAULT NULL,
  `validity_time_slots` text DEFAULT NULL,
  `product_type` varchar(100) NOT NULL,
  `product_value` int(11) NOT NULL DEFAULT 0,
  `product_price` decimal(10,2) NOT NULL DEFAULT 0.00,
  `description` text DEFAULT NULL,
  `carparks` text DEFAULT NULL,
  `user_id` int(11) NOT NULL DEFAULT 0,
  `status` int(11) NOT NULL DEFAULT 1,
  `create_datetime` datetime NOT NULL DEFAULT current_timestamp(),
  `validation_type` varchar(25) NOT NULL,
  `facility_number` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `system_role_rights`
--

CREATE TABLE `system_role_rights` (
  `role_rights_id` int(11) NOT NULL,
  `user_role_id` int(11) NOT NULL,
  `menu_id` int(11) NOT NULL,
  `rr_view` tinyint(1) NOT NULL DEFAULT 0,
  `rr_edit` tinyint(1) NOT NULL DEFAULT 0,
  `rr_delete` tinyint(1) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `system_role_rights`
--

INSERT INTO `system_role_rights` (`role_rights_id`, `user_role_id`, `menu_id`, `rr_view`, `rr_edit`, `rr_delete`) VALUES
(1, 105, 23, 1, 1, 1),
(2, 105, 81, 1, 1, 1),
(3, 105, 83, 1, 1, 1),
(4, 105, 86, 1, 1, 1),
(5, 105, 87, 1, 1, 1),
(6, 105, 50, 1, 1, 1),
(7, 105, 103, 1, 1, 1),
(8, 105, 76, 0, 0, 0),
(9, 105, 93, 1, 1, 1),
(10, 105, 94, 1, 1, 1),
(11, 105, 78, 1, 1, 1),
(12, 105, 88, 1, 1, 1),
(13, 105, 34, 1, 1, 1),
(14, 105, 33, 1, 1, 1),
(15, 105, 61, 1, 1, 1),
(16, 105, 92, 1, 1, 1),
(17, 105, 77, 0, 0, 0),
(18, 105, 89, 1, 1, 1),
(19, 105, 95, 1, 1, 1),
(20, 105, 99, 1, 1, 1),
(21, 105, 100, 1, 1, 1),
(22, 105, 64, 1, 1, 1),
(23, 105, 69, 1, 1, 1),
(24, 105, 49, 1, 1, 1),
(25, 105, 63, 1, 1, 1),
(26, 105, 66, 1, 1, 1),
(27, 105, 47, 1, 1, 1),
(28, 105, 48, 1, 1, 1),
(29, 105, 54, 1, 1, 1),
(30, 105, 56, 1, 1, 1),
(31, 105, 90, 1, 1, 1),
(32, 105, 57, 1, 1, 1),
(33, 105, 84, 1, 1, 1),
(34, 105, 59, 1, 1, 1),
(35, 105, 98, 1, 1, 1),
(36, 105, 68, 1, 1, 1),
(37, 105, 73, 1, 1, 1),
(38, 105, 102, 1, 1, 1),
(39, 105, 67, 1, 1, 1),
(40, 105, 45, 1, 1, 1),
(41, 105, 60, 1, 1, 1),
(42, 105, 58, 1, 1, 1),
(43, 105, 37, 1, 1, 1),
(44, 105, 46, 1, 1, 1),
(45, 105, 91, 1, 1, 1),
(46, 105, 71, 1, 1, 1),
(47, 105, 85, 1, 1, 1),
(48, 105, 82, 1, 1, 1),
(49, 105, 72, 1, 1, 1),
(50, 105, 65, 0, 0, 0),
(51, 105, 74, 1, 1, 1),
(52, 105, 35, 1, 1, 1),
(53, 105, 104, 1, 1, 1);

-- --------------------------------------------------------

--
-- Table structure for table `system_settings`
--

CREATE TABLE `system_settings` (
  `settings_id` int(11) NOT NULL,
  `setting_name` varchar(100) NOT NULL DEFAULT '0',
  `setting_value` varchar(100) NOT NULL DEFAULT '0',
  `setting_label` text DEFAULT NULL,
  `setting_status` int(1) NOT NULL DEFAULT 1
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `system_settings`
--

INSERT INTO `system_settings` (`settings_id`, `setting_name`, `setting_value`, `setting_label`, `setting_status`) VALUES
(1, 'facility_name', 'Training Area', 'Facility name', 1),
(2, 'facility_number', '200005', 'Facility number', 1),
(3, 'dayclosure_start_time', '00:00:00', 'Day closure start time', 1),
(4, 'dayclosure_end_time', '10:00:00', 'Day closure end time', 1),
(5, 'currency', 'AED', 'Currency', 1),
(6, 'vat_percentage', '5', 'Vat Percentage', 1),
(7, 'vat_type', 'inclusive', 'Vat type', 1),
(8, 'vat_id', '100292268800003', 'Vat id', 1),
(9, 'address_line1', 'Dubai,UAE', 'Address line 1', 1),
(10, 'address_line2', 'PO Box:19514', 'Address line 2', 1),
(11, 'address_line3', 'PO Box:19514', 'Address line 3', 1),
(12, 'decimal_places', '2', 'Currency decimal places', 1),
(13, 'ticket_validity', '5', 'Ticket validity', 1),
(14, 'primary_language', 'english', 'Primary language', 1),
(15, 'secondary_language', 'arabic', 'Secondary language', 1),
(16, 'realtime_daemon_delay', '20', 'Real time daemon delay', 1),
(17, 'send_count_to_device', '1', 'Send carpark count to devices', 1),
(18, 'validation_enabled', '1', 'Validation Enabled', 1),
(19, 'site_code', 'afme', 'Site code', 1),
(20, 'cloud_enabled', '0', 'Cloud Enabled', 1),
(21, 'cloud_download_webservice', 'http://localhost/PXCloudService/FieldServerDownload.php', 'Cloud download webservice', 1),
(22, 'cloud_upload_webservice', 'http://localhost/PXCloudService/FieldServerUpload.php', 'Cloud upload webservice', 1),
(23, 'wallet_enabled', '0', 'Wallet Enabled', 1),
(24, 'cloud_wallet_webservice', 'http://10.195.14.213/pxservices/walet_operations.php', 'Cloud wallet webservice', 1),
(43, 'max_validation_allowed', '5', 'Maximum validation allowed', 1),
(44, 'chip_validity_days', '5', 'Chip validity days', 1),
(45, 'image_dir_path', '/var/www/html/images', 'Image directory path', 1),
(46, 'log_dir_path', '/var/www/html/Dnata/logs', 'Log directory path', 1),
(47, 'retention_time_images', '+2', 'Retension time images', 1),
(48, 'retention_time_logs', '-1', 'Retention time logs', 1),
(68, 'customer_message_limit', '25', 'Customer message limit', 1),
(69, 'customer_media_upload_limit', '100', 'Customer media upload limit', 1),
(70, 'customer_media_path', '/parcx/Media', 'Customer media path', 1),
(71, 'plate_review_confidence_rate', '120', 'Plate review confidence rate', 1),
(73, 'anpr_mismatch_check', '0', 'ANPR mismatch check', 1),
(76, 'cloud_operator_id', '43', 'Cloud operator_id\r\n', 1),
(77, 'cloud_get_details_webservice', 'http://10.195.14.161/pxservices/PX_AppGetRecords.php', 'Cloud get details webservice', 1),
(79, 'short_term_entry_after_contract_parking_space_exceeded', '1', 'short term enabled after Contract parking space exceeded', 1),
(80, 'full_payment_after_contract_parking_space_exceeded', '1', 'full payment after Contract parking space exceeded . 0 = partial payment , 1 = full shortterm payment', 1);

-- --------------------------------------------------------

--
-- Table structure for table `system_users`
--

CREATE TABLE `system_users` (
  `user_id` int(11) NOT NULL,
  `operator_name` varchar(50) NOT NULL,
  `username` varchar(25) NOT NULL,
  `company_name` varchar(50) DEFAULT NULL,
  `email` varchar(100) NOT NULL,
  `phone` varchar(25) DEFAULT NULL,
  `password` varchar(100) NOT NULL,
  `user_role_id` int(11) NOT NULL,
  `language` varchar(250) DEFAULT NULL,
  `validity_from_date` date DEFAULT NULL,
  `validity_to_date` date DEFAULT NULL,
  `account_status` tinyint(1) NOT NULL DEFAULT 1
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `system_users`
--

INSERT INTO `system_users` (`user_id`, `operator_name`, `username`, `company_name`, `email`, `phone`, `password`, `user_role_id`, `language`, `validity_from_date`, `validity_to_date`, `account_status`) VALUES
(30, 'parcx', 'parcx', 'afme', 'parcx@test.com', '0503029959', '7aa84b18e17a847b3dcc315126acb56d', 105, 'English', '2021-04-01', '2021-07-31', 1),
(46, 'Reporting User', 'reportuser', 'Al Falak Middle East LLC Dubai', 'report@gmail.com', '561154055', '7aa84b18e17a847b3dcc315126acb56d', 106, '', '2020-07-01', '2020-08-07', 1),
(47, 'Jeena volga', 'jeena', '', 'jeenavolga@gmail.com', '', '7aa84b18e17a847b3dcc315126acb56d', 105, 'English', '2021-03-03', '2021-03-31', 1);

-- --------------------------------------------------------

--
-- Table structure for table `system_user_role`
--

CREATE TABLE `system_user_role` (
  `user_role_id` int(11) NOT NULL,
  `user_role_name` varchar(50) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `system_user_role`
--

INSERT INTO `system_user_role` (`user_role_id`, `user_role_name`) VALUES
(105, 'Admin'),
(107, 'Report User'),
(106, 'Service Technician');

-- --------------------------------------------------------

--
-- Table structure for table `system_validators`
--

CREATE TABLE `system_validators` (
  `user_id` int(11) NOT NULL,
  `display_name` varchar(100) NOT NULL,
  `email` varchar(100) NOT NULL,
  `password` varchar(100) NOT NULL,
  `date_validity` int(1) NOT NULL,
  `start_date` date DEFAULT NULL,
  `expiry_date` date DEFAULT NULL,
  `status` int(1) NOT NULL,
  `mobile` varchar(15) NOT NULL,
  `create_datetime` datetime NOT NULL DEFAULT current_timestamp(),
  `carparks` text NOT NULL,
  `logo` varchar(40) NOT NULL,
  `products` text NOT NULL,
  `ticket_age` int(11) NOT NULL,
  `total_maximum_validation_hours` int(11) NOT NULL,
  `daily_maximum_validation_hours` int(11) NOT NULL,
  `weekly_maximum_validation_hours` int(11) NOT NULL,
  `monthly_maximum_validation_hours` int(11) NOT NULL,
  `validation_report_view` int(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `system_vms_devices`
--

CREATE TABLE `system_vms_devices` (
  `vms_id` int(11) NOT NULL,
  `vms_name` varchar(50) NOT NULL,
  `location` varchar(100) NOT NULL,
  `port_name` varchar(50) NOT NULL,
  `hold_status` int(11) NOT NULL,
  `device_enabled` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `valet_drivers`
--

CREATE TABLE `valet_drivers` (
  `id` int(11) NOT NULL,
  `valet_driver_name` varchar(25) NOT NULL,
  `valet_driver_id` varchar(25) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `vms_communication_config`
--

CREATE TABLE `vms_communication_config` (
  `id` int(11) NOT NULL,
  `device_number` int(11) NOT NULL DEFAULT 0,
  `ip_address` varchar(30) NOT NULL,
  `serial_port` varchar(20) NOT NULL,
  `group_address` int(11) NOT NULL,
  `vms_address` int(11) NOT NULL DEFAULT 1,
  `vms_name` varchar(50) NOT NULL,
  `vms_width` int(11) NOT NULL,
  `vms_height` int(11) NOT NULL,
  `daemon_port_id` int(11) NOT NULL,
  `icon_font_size` int(11) NOT NULL,
  `icon_color` int(11) NOT NULL,
  `left_icon` varchar(100) NOT NULL,
  `left_icon_text` varchar(25) NOT NULL,
  `left_icon_name` varchar(25) NOT NULL,
  `left_color` int(1) NOT NULL,
  `left_font_size` int(1) NOT NULL,
  `left_character_count` int(2) NOT NULL,
  `left_counter` varchar(100) NOT NULL,
  `center_icon` varchar(100) NOT NULL,
  `center_icon_text` varchar(25) NOT NULL,
  `center_icon_name` varchar(25) NOT NULL,
  `center_color` int(11) NOT NULL,
  `center_font_size` int(11) NOT NULL,
  `center_character_count` int(2) NOT NULL,
  `center_counter` varchar(100) NOT NULL,
  `right_icon` varchar(100) NOT NULL,
  `right_icon_text` varchar(25) NOT NULL,
  `right_icon_name` varchar(25) NOT NULL,
  `right_character_count` int(2) NOT NULL,
  `right_counter` varchar(100) NOT NULL,
  `right_color` int(11) NOT NULL,
  `right_font_size` int(11) NOT NULL,
  `test` int(11) NOT NULL,
  `status` int(1) NOT NULL,
  `data_sent` text NOT NULL,
  `last_communication_datetime` datetime NOT NULL DEFAULT current_timestamp(),
  `brightness_time1` int(11) NOT NULL,
  `brightness_time2` int(11) NOT NULL,
  `brightness_time3` int(11) NOT NULL,
  `brightness_time4` int(11) NOT NULL,
  `brightness_time5` int(11) NOT NULL,
  `brightness_value1` int(11) NOT NULL,
  `brightness_value2` int(11) NOT NULL,
  `brightness_value3` int(11) NOT NULL,
  `brightness_value4` int(11) NOT NULL,
  `brightness_value5` int(11) NOT NULL,
  `delay` int(11) NOT NULL,
  `connect_start_stop` int(11) NOT NULL,
  `delay_between_signs` int(11) NOT NULL,
  `wait_for_response` tinyint(4) NOT NULL DEFAULT 0,
  `sign_type` int(11) NOT NULL,
  `review_mode` tinyint(4) NOT NULL DEFAULT 0,
  `last_daemon_update` datetime NOT NULL,
  `status_message` varchar(100) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `vms_device_shedule`
--

CREATE TABLE `vms_device_shedule` (
  `id` int(11) NOT NULL,
  `vms_id` int(11) NOT NULL,
  `vms_name` varchar(50) NOT NULL,
  `message` text NOT NULL,
  `colour` varchar(30) NOT NULL,
  `action` varchar(50) NOT NULL,
  `shedule_time` varchar(30) NOT NULL,
  `shedule_interval` varchar(100) NOT NULL,
  `hold_time` int(11) NOT NULL,
  `status` int(11) NOT NULL,
  `start_datetime` datetime NOT NULL DEFAULT current_timestamp(),
  `last_updated_datetime` datetime NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `web_application_labels`
--

CREATE TABLE `web_application_labels` (
  `id` int(11) NOT NULL,
  `message` varchar(100) DEFAULT NULL,
  `english` varchar(100) DEFAULT NULL,
  `arabic` varchar(100) DEFAULT NULL,
  `spanish` varchar(100) DEFAULT NULL,
  `status` int(11) NOT NULL DEFAULT 1
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `web_application_labels`
--

INSERT INTO `web_application_labels` (`id`, `message`, `english`, `arabic`, `spanish`, `status`) VALUES
(1, 'date_time', 'Date Time', 'وقت التاريخ', 'Fecha y hora', 1),
(2, 'device_number', 'Device number', 'رقم الجهاز', 'Número del dispositivo', 1),
(3, 'device_name', 'Device name', 'اسم الجهاز', 'Nombre del dispositivo', 1),
(4, 'result', 'Result', 'نتيجة', '', 1),
(5, 'live_parking', 'Live Review', 'مراجعة مباشرة', '', 1),
(6, 'manual_movement', 'Manual Movements', 'مراجعة مباشرة', '', 1),
(7, 'transactions', 'Transactions', 'المعاملات', '', 1),
(8, 'operator_name', 'Operator Name', 'المشغل أو العامل', '', 1),
(9, 'action', 'Action', 'عمل', '', 1),
(10, 'reason', 'Reason', 'السبب', '', 1),
(11, 'description', 'Description', 'وصف', 'Descripción', 1),
(12, 'camera_name', 'Camera Name', 'اسم الكاميرا', '', 1),
(13, 'plate_number', 'Plate Number', 'رقم لوحة', '', 1),
(14, 'plate_area', 'Plate Area', 'منطقة', '', 1),
(15, 'plate_country', 'Plate Country', 'بلد', '', 1),
(16, 'cropped_image', 'Cropped Image', 'الصورة التي تم اقتصاصها', '', 1),
(17, 'full_image', 'Full Image', 'الصورة الكاملة', '', 1),
(18, 'plates_captured', 'Plates Captured', 'تم التقاط لوحات الأرقام', '', 1),
(19, 'shift_number', 'Shift Number', 'رقم التحول', '', 1),
(20, 'operator_number', 'OperatorNumber', 'رقم المشغل', '', 1),
(21, 'shift_start', 'Shift Start', 'بدء التحول', '', 1),
(22, 'shift_end', 'Shift End', 'نهاية التحول', '', 1),
(23, 'shift_earnings', 'Shift Earnings', 'أرباح', '', 1),
(24, 'over_short', 'Over/Short', 'فوق/قصيرة', '', 1),
(25, 'last_updated', 'Last Updated', 'آخر تحديث', '', 1),
(26, 'status', 'Status', 'الحالة', '', 1),
(27, 'open_amount', 'Shift Open Amount', 'مبلغ مفتوح', '', 1),
(28, 'close_amount', 'Shift Close Amount', 'مبلغ الإغلاق', '', 1),
(29, 'payin_amount', 'Pay In Amount', 'المبلغ المدفوع', '', 1),
(30, 'payout_amount', 'Pay Out Amount', 'مبلغ الدفع', '', 1),
(31, 'duration', 'Parking Duration', 'المدة الزمنية', '', 1),
(32, 'parking_fee', 'Parking Fee', 'رسوم الإصطفاف', '', 1),
(33, 'lost_fee', 'Lost Ticket Fee', 'رسوم التذكرة المفقودة', '', 1),
(34, 'vat', 'VAT', 'ضريبة القيمة المضافة', '', 1),
(35, 'shift_earnings', 'Shift Earnings', 'أرباح', '', 1),
(36, 'physical_cash', 'Physical Cash Collected', 'جمع النقد', '', 1),
(37, 'shift_report', 'Shift Report', 'تقرير التحول', '', 1),
(38, 'ticket_id', 'Ticket Id', 'رقم التذكرة', '', 1),
(39, 'blacklisting_reason', 'Blacklisting Reason', 'سبب القائمة السوداء', '', 1),
(40, 'blacklist_report', 'Blacklist Report', 'تقرير القائمة السوداء', '', 1),
(41, 'parking_movement', 'Parking Movements', 'حركة وقوف السيارات', '', 1),
(42, 'entry_details', 'Entry Details', 'تفاصيل الدخول', '', 1),
(43, 'entry_type', 'Entry Type', 'نوع الإدخال', '', 1),
(44, 'entry_date_time', 'Entry Date Time', 'موعد الدخول', '', 1),
(45, 'entry_grace_period', 'Entry Grace Period', 'فترة السماح دخول', '', 1),
(46, 'entry_device_name', 'Entry Device Name', 'اسم الجهاز عند الدخول', '', 1),
(47, 'entry_carpark_name', 'Entry Carpark Name', 'اسم موقف السيارات عند الدخول', '', 1),
(48, 'access_details', 'Access Details', 'تفاصيل الوصول', '', 1),
(49, 'customer_name', 'Customer Name', 'اسم الزبون', '', 1),
(50, 'validity_start_date', 'Validity Start Date', 'تاريخ البدء', '', 1),
(51, 'validity_expiry_date', 'Validity Expiry Date', 'تاريخ انتهاء الصلاحية', '', 1),
(52, 'reservation_details', 'Reservation Details', 'تفاصيل الحجز', '', 1),
(53, 'payment_details', 'Payment Details', 'بيانات الدفع', '', 1),
(54, 'payment_date_time', 'Payment Date Time', 'تاريخ ووقت الدفع', '', 1),
(55, 'maximum_exit_date_time', 'Maximum Exit Date Time', 'وقت الخروج الأقصى', '', 1),
(56, 'payment_device_name ', 'Payment Device Name', 'اسم جهاز الدفع', '', 1),
(57, 'payment_carpark_name ', 'Payment Carpark Name', 'اسم موقف سيارات الدفع', '', 1),
(58, 'amount_paid', 'Amount Paid', 'المبلغ المدفوع', '', 1),
(59, 'discount_name', 'Discount Name', 'اسم الخصم', '', 1),
(60, 'validation_value', 'Validation Value', 'التحقق من الصحة', '', 1),
(61, 'exit_details', 'Exit Details', 'تفاصيل الخروج', '', 1),
(62, 'exit_time', 'Exit Date Time', 'وقت الخروج', '', 1),
(63, 'exit_device_name', 'Exit Device Name', 'اسم الجهاز عند الخروج', '', 1),
(64, 'exit_carpark_name', 'Exit Carpark Name', 'اسم موقف السيارات عند المخرج', '', 1),
(65, 'invalid_ticket', 'Invalid Ticket', 'تذكرة غير صالحة', '', 1),
(66, 'ticket_details', 'Ticket Details', 'تفاصيل التذكرة', '', 1),
(67, 'visitor_frequency_realtime', 'Realtime Visitor Frequency', 'تردد الزائر في الوقت الفعلي', '', 1),
(68, 'no_of_visits', 'No of Visits', 'عدد الزيارات', '', 1),
(69, 'tag', 'Tag', 'بطاقة', '', 1),
(70, 'access_movements', 'Access Movements', 'حركة الوصول', '', 1),
(71, 'carpark_name', 'Carpark Name', 'اسم موقف السيارات', '', 1),
(72, 'open_transactions', 'Open Transactions', 'فتح المعاملات', '', 1),
(73, 'exit_grace_period', 'Exit Grace Period', 'فترة سماح الخروج', '', 1),
(74, 'image', 'Image', 'صورة', '', 1),
(75, 'validation_report', 'Validation Report', 'تقرير التحقق من صحة', '', 1),
(76, 'validator', 'Validator', 'المدقق', '', 1),
(77, 'product', 'Product', 'المنتج', '', 1),
(78, 'no_records', 'No records/transactions available for the current search criteria', 'لا توجد سجلات / معاملات متاحة لمعايير البحث الحالية', 'No hay registros / transacciones disponibles para los criterios de búsqueda actuales', 1),
(79, 'banknote', 'Banknote x Quantity', 'الأوراق النقدية × الكمية', '', 1),
(80, 'amount', 'Amount', 'قيمة', '', 1),
(81, 'apm_refill_report', 'APM Refill Report', 'APM ملأ ثانية', '', 1),
(82, 'total_amount_refilled', 'Total Amount Refilled', 'إجمالي المبلغ المعاد تعبئته', '', 1),
(83, 'banknotes', 'Banknote', 'الأوراق النقدية', '', 1),
(84, 'recycler_level', 'Recycler Level', 'مستوى إعادة التدوير', '', 1),
(85, 'cashbox_level', 'Cashbox Level', 'مستوى كاش بوكس', '', 1),
(86, 'total', 'Total', 'مجموع', '', 1),
(87, 'cash_levels', 'APM Cash Level', 'المستوى النقدي APM', '', 1),
(88, 'operation', 'Operation', 'عملية', '', 1),
(89, 'apm_payout_report', 'APM Payout Report', 'تقرير عائد APM', '', 1),
(90, 'total_payout', 'Total Payout', 'إجمالي المدفوعات', '', 1),
(91, 'exception', 'Exception', 'استثناء', '', 1),
(92, 'amount_received', 'Amount Received', 'المبلغ الذي تسلمه', '', 1),
(93, 'balance_returned', 'Balance Returned', 'توازن', '', 1),
(94, 'credit_note', 'Credit Note', 'دين', '', 1),
(95, 'payment_transaction_exception', 'Payment Transaction Exceptions', 'استثناءات معاملات الدفع', '', 1),
(96, 'creditcard_transactions', 'Creditcard Transactions', 'معاملات بطاقات الائتمان', '', 1),
(97, 'total_revenue', 'Total Revenue', 'إجمالي الإيرادات', '', 1),
(98, 'merchant_id', 'Merchant id', 'معرف التاجر', '', 1),
(99, 'terminal_id', 'Terminal id', 'معرف المحطة', '', 1),
(100, 'comment', 'Comment', 'تعليق', '', 1),
(101, 'card_type', 'Card Type', 'نوع البطاقة', '', 1),
(102, 'authentication_code', 'Authentication Code', 'رمز الدخول', '', 1),
(103, 'reference_number', 'Reference Number', 'رقم المرجع', '', 1),
(104, 'report_date', 'Report Date', 'تاريخ التقرير', '', 1),
(105, 'product_sale_amount', 'Product Sale Amount', 'مبلغ بيع المنتج', '', 1),
(106, 'discount_amount', 'Discount Amount', 'مقدار الخصم', '', 1),
(107, 'notifications', 'Notifications', 'إشعارات', '', 1),
(108, 'carpark_number', 'Carpark Number', 'رقم موقف السيارات', '', 1),
(109, 'report_by_date', 'Report by Date', 'تقرير حسب التاريخ', '', 1),
(110, 'total_earnings', 'Total Earnings', 'الأرباح الكلية', '', 1),
(111, 'cash_amount', 'Cash Amount', 'مبلغ نقدي', '', 1),
(112, 'creditcard_amount', 'Creditcard Amount', 'مبلغ بطاقة الائتمان', '', 1),
(113, 'wallet_amount', 'Wallet Amount', 'مبلغ المحفظة', '', 1),
(114, 'payable_entries_count', 'Payable Entries Count', 'إدخالات مستحقة الدفع', '', 1),
(115, 'lost_ticket_count', 'Lost Ticket Count', 'عدد التذاكر المفقودة', '', 1),
(116, 'discount_count', 'Discount Count', 'عدد الخصومات', '', 1),
(117, 'product_sale_count', 'Product Sale Cout', 'عدد بيع المنتج', '', 1),
(118, 'cash_transaction_count', 'Cash Transaction Count', 'عدد المعاملات النقدية', '', 1),
(119, 'creditcard_transaction_count', 'Creditcard Transaction Count', 'عدد معاملات بطاقة الائتمان', '', 1),
(120, 'cash', 'Cash', 'السيولة النقدية', '', 1),
(121, 'credit_card', 'Creditcard', 'بطاقة ائتمان', '', 1),
(122, 'sms', 'SMS', 'رسالة قصيرة', '', 1),
(123, 'product_name', 'Product Name', 'اسم المنتج', '', 1),
(124, 'quantity', 'Quantity', 'كمية', '', 1),
(125, 'unit_price', 'Unit Price', 'سعر الوحدة', '', 1),
(126, 'total_amount', 'Total Amount', 'المبلغ الإجمالي', '', 1),
(127, 'payable_entries', 'Payable Entries', 'إدخالات مستحقة الدفع', '', 1),
(128, 'discounts', 'Discounts', 'الخصومات', '', 1),
(129, 'product_sales', 'Product Sales', 'مبيعات المنتج', '', 1),
(130, 'earnings', 'Earnings', 'الأرباح', '', 1),
(131, 'earnings_by_payment_type', 'Earnings by Payment Type', 'الأرباح حسب نوع الدفع', '', 1),
(132, 'transaction_count', 'Transaction Count', 'عدد المعاملات', '', 1),
(133, 'revenue_report', 'Revenue Report', 'تقرير الإيرادات', '', 1),
(134, 'lost_ticket', 'Lost Ticket', 'تذكرة ضائعة', '', 1),
(135, 'traffic_report', 'Traffic Report', 'تقرير المرور', '', 1),
(136, 'entries', 'Entries', 'إدخالات', '', 1),
(137, 'exits', 'Exits', 'مخارج', '', 1),
(138, 'manual_operation', 'Manual Operation', 'التشغيل اليدوي', '', 1),
(139, 'short_term', 'Short Term/Contract', 'عقد على المدى القصير', '', 1),
(140, 'entry_exit', 'Entry/Exit', 'دخول / خروج', '', 1),
(141, 'shortterm_entries', 'Entries Short-term', 'إدخالات قصيرة المدى', '', 1),
(142, 'shortterm_exits', 'Exits Short-term', 'مخارج قصيرة المدى', '', 1),
(143, 'contract_entries', 'Entries Contract', 'إدخالات العقد', '', 1),
(144, 'contract_exits', 'Exits Contract', 'مخارج العقد', '', 1),
(145, 'manual_entry', 'Manual Entry', 'الإدخال اليدوي', '', 1),
(146, 'manual_exit', 'Manual Exit', 'خروج يدوي', '', 1),
(147, 'from', 'From', 'من عند', '', 1),
(148, 'to', 'To', 'ل', '', 1),
(149, 'payment_transactions', 'Payment Transactions', 'معاملات الدفع', '', 1),
(150, 'category', 'Category', 'الفئة', '', 1),
(151, 'payment_type', 'Payment Type', 'نوع الدفع', '', 1),
(152, 'gross_amount', 'Gross Amount', 'المبلغ الإجمالي', '', 1),
(153, 'all_carparks', 'All Carparks', 'جميع مواقف السيارات', '', 1),
(154, 'select_carparks', 'Select Carparks', 'حدد موقف السيارات', '', 1),
(155, 'all_parking_zone', 'All Parking Zone', 'كل منطقة وقوف السيارات', '', 1),
(156, 'select_parking_zone', 'Select Parking Zone', 'حدد منطقة وقوف السيارات', '', 1),
(157, 'all_devices', 'All Devices', 'جميع الاجهزة', 'Todos los dispositivos', 1),
(158, 'select_devices', 'Select Devices', 'حدد الأجهزة', 'Seleccionar dispositivos', 1),
(159, 'all_operators', 'All Operators', 'جميع المشغلين', '', 1),
(160, 'select_operators', 'Select Operators', 'حدد المشغلين', '', 1),
(161, 'all_category', 'All Category', 'كل الفئات', '', 1),
(162, 'select_category', 'Select Category', 'اختر الفئة', '', 1),
(163, 'all_payment', 'All Payment Type', 'جميع أنواع الدفع', '', 1),
(164, 'select_payment', 'Select Payment Type', 'حدد أنواع الدفع', '', 1),
(165, 'all_discount', 'All Discounts', 'كل خصم', '', 1),
(166, 'select_discount', 'Select Discounts', 'حدد الخصومات', '', 1),
(167, 'all_days', 'All Days of the Week', 'كل أيام الأسبوع', '', 1),
(168, 'select_days', 'Select Day of the Week', 'حدد يوم من الأسبوع', '', 1),
(169, 'daily', 'Daily', 'اليومي', '', 1),
(170, 'select_interval', 'Select Interval', 'حدد الفاصل الزمني', '', 1),
(171, 'view_report', 'View Report', 'عرض التقرير', 'Vista del informe', 1),
(172, 'logout', 'Logout', 'تسجيل خروج', 'Cerrar sesión', 1),
(173, 'choose_date_range', 'Choose Date Range', 'اختر النطاق الزمني', '', 1),
(174, 'parking_rate', 'Parking Rate', 'معدل وقوف السيارات', '', 1),
(175, 'validations', 'validations', 'التحقق من صحة', '', 1),
(176, 'validation_hours', 'Validation Hours', 'التحقق من الصحة', '', 1),
(177, 'pdf_receipt', 'PDF Receipt', 'إيصال PDF', '', 1),
(178, 'detailed_payment', 'Detailed Payment Information', 'معلومات دفع مفصلة', '', 1),
(179, 'trn_no', 'TRN No', 'رقم TRN', '', 1),
(180, 'net_amount', 'Net Amount', 'كمية الشبكة', '', 1),
(181, 'detailed_shift', 'Detailed Shift Information', 'معلومات التحول التفصيلية', '', 1),
(182, 'choose_datetime_range', 'Choose Date and Time Range', 'اختر نطاق التاريخ والوقت', '', 1),
(183, 'plate_ticket', 'Plate Number/Ticket Id', 'رقم لوحة  / رقم التذكرة', '', 1),
(184, 'view_details', 'View Details', 'عرض التفاصيل', '', 1),
(185, 'contract', 'Contract', 'خطب', '', 1),
(186, 'search', 'Search', 'بحث', '', 1),
(187, 'select', 'Select', 'تحديد', '', 1),
(188, 'sunday', 'Sunday', 'الأحد', '', 1),
(189, 'monday', 'Monday', 'الإثنين', '', 1),
(190, 'tuesday', 'Tuesday', 'الثلاثاء', '', 1),
(191, 'wednesday', 'Wednesday', 'الأربعاء', '', 1),
(192, 'thursday', 'Thursday', 'الخميس', '', 1),
(193, 'friday', 'Friday', 'يوم الجمعة', '', 1),
(194, 'saturday', 'Saturday', 'يوم السبت', '', 1),
(195, 'vat_report', 'VAT Report', 'تقرير ضريبة القيمة المضافة', '', 1),
(196, 'vat_amount', 'VAT Amount', 'قيمة الضريبة', '', 1),
(197, 'total_vat', 'Total VAT', 'إجمالي ضريبة القيمة المضافة', '', 1),
(198, 'severity', 'Severity', 'خطورة', 'Gravedad', 1),
(199, 'ip_address', 'IP Address', 'عنوان IP', '', 1),
(200, 'connection_lost', 'Connection Lost', 'فقد الاتصال', '', 1),
(201, 'connection_restored', 'Connection Restored', 'تمت استعادة الاتصال', '', 1),
(202, 'watchdog_device_alarms', 'Watchdog device alarms', 'أجهزة الإنذار جهاز المراقبة', 'Alarmas de dispositivos de vigilancia', 1),
(203, 'export', 'Export', 'تصدير', 'Exportar', 1),
(204, 'export_to_excel', 'Export to Excel', 'تصدير إلى Excel', 'Exportar a Excel', 1),
(205, 'export_to_pdf', 'Export to PDF', 'تصدير إلى PDF', 'Exportar a PDF', 1),
(206, 'high', 'High', 'عالي', 'Alto', 1),
(207, 'low', 'Low', 'منخفض', 'Bajo', 1),
(208, 'medium', 'Medium', 'متوسط', 'Medio', 1),
(209, 'select_severity', 'Select Severity', 'حدد الخطورة', 'Seleccionar gravedad', 1),
(210, 'search', 'Search', 'بحث', 'Buscar', 1),
(211, 'entries_label', 'Show _MENU_ entries', 'إظهار _MENU_ مداخل', 'Mostrar _MENU_ entradas', 1),
(212, 'info_label', 'Showing _START_ to _END_ of _TOTAL_ entries', 'إظهار_START_ إلى _END_ من _TOTAL_ مدخلات', 'Mostrando _START_ a _END_ de _TOTAL_ entradas', 1),
(213, 'previous', 'Previous', 'السابق', 'Previo', 1),
(214, 'next', 'Next', 'التالى', 'Próximo', 1),
(215, 'receipt_details', 'Receipt Details', 'تفاصيل الإيصال', 'Detalles del recibo', 1),
(216, 'close', 'Close', 'قريب', 'Cerca', 1),
(217, 'print', 'Print', 'طباعة', 'Impresión', 1),
(218, 'tax_invoice', 'Tax invoice', 'فاتورة ضريبية', 'Factura fiscal', 1),
(219, 'discount', 'Discount', 'خصم', 'Descuento', 1),
(220, 'already_paid_chipcoin', 'Already Paid Chipcoin', 'مدفوع بالفعل', '', 1),
(221, 'cancelled_transaction', 'Cancelled Transaction', 'صفقة ملغاة', '', 1),
(222, 'invalid_chipcoin', 'Invalid Chipcoin', 'تشيبكوين غير صالح', '', 1),
(223, 'blacklisted', 'Blacklisted', 'مدرج في القائمة السوداء', '', 1),
(224, 'invalid_carpark', 'Invalid Carpark', 'موقف سيارات غير صالح', '', 1),
(225, 'select_exception', 'Select Exception', 'حدد استثناء', '', 1),
(226, 'visitor_frequency', 'Visitor Frequency', 'تردد الزوار', '', 1),
(227, 'last_7_days', 'Last 7 days', 'اخر 7 ايام', '', 1),
(228, 'last_30_days', 'Last 30 days', 'آخر 30 يومًا', '', 1),
(229, 'last_60_days', 'Last 60 days', 'آخر 60 يومًا', '', 1),
(230, 'last_90_days', 'Last 90 days', 'آخر 90 يومًا', '', 1),
(231, 'last_120_days', 'Last 120 days', 'آخر 120 يومًا', '', 1),
(232, 'last_150_days', 'Last 150 days', 'آخر 150 يومًا', '', 1),
(233, 'last_180_days', 'Last 180 days', 'آخر 180 يومًا', '', 1),
(234, 'access_allowed', 'Access Allowed', 'الوصول المسموح', '', 1),
(235, 'access_denied', 'Access Denied', 'تم الرفض', '', 1),
(236, 'tag_unknown', 'Tag Unknown', 'علامة غير معروفة', '', 1),
(237, 'select_status', 'Select Status', 'حدد الحالة', '', 1),
(238, 'plate_ticket_tag', 'Plate/Ticket/Tag', 'رقم اللوحة / التذكرة / البطاقة', '', 1),
(239, 'add_to_whitelist', 'ADD TO WHITELIST', 'إضافة إلى القائمة البيضاء', '', 1),
(240, 'country', 'Country', 'بلد', '', 1),
(241, 'select_country', 'Select Country', 'حدد الدولة', '', 1),
(242, 'emirate', 'Emirate', 'الإمارات', '', 1),
(243, 'uae', 'UAE', 'الإمارات العربية المتحدة', '', 1),
(244, 'bahrain', 'Bahrain', 'البحرين', '', 1),
(245, 'kuwait', 'Kuwait', 'الكويت', '', 1),
(246, 'oman', 'Oman', 'سلطنة عمان', '', 1),
(247, 'qatar', 'Qatar', 'دولة قطر', '', 1),
(248, 'saudi', 'Saudi Arabia', 'المملكة العربية السعودية', '', 1),
(249, 'abudhabi', 'Abu Dhabi', 'أبو ظبي', '', 1),
(250, 'dubai', 'Dubai', 'دبي', '', 1),
(251, 'ajman', 'Ajman', 'عجمان', '', 1),
(252, 'sharjah', 'Sharjah', 'الشارقة', '', 1),
(253, 'rasalkhaimah', 'Ras Al Khaimah', 'رأس الخيمة', '', 1),
(254, 'fujairah', 'Fujairah', 'الفجيرة', '', 1),
(255, 'ummalquwwain', 'Umm Al Quwwain', 'أم القيوين', '', 1),
(256, 'qrcode', 'QR Code', 'رمز الاستجابة السريعة', '', 1),
(257, 'activation_date', 'Activation Date', 'تفعيل التسجيل', '', 1),
(258, 'expiry_date', 'Expiry Date', 'تاريخ انتهاء الصلاحية', '', 1),
(259, 'facility_name', 'Facility Name', 'إسم المنشأة', '', 1),
(260, 'personal_message_1', 'Personal Message 1', 'رسالة شخصية 1', '', 1),
(261, 'personal_message_2', 'Personal Message 2', 'رسالة شخصية 2', '', 1),
(262, 'anti_passback', 'Anti Passback', 'رمكافحة Passback', '', 1),
(263, 'submit', 'Submit', 'إرسال', '', 1),
(264, 'prefix', 'Prefix', 'اختصار', '', 1),
(265, 'carpark', 'Carpark', 'موقف السيارات', '', 1),
(266, 'all_operation_methods', 'All Operation Methods', 'جميع طرق التشغيل', '', 1),
(267, 'manual_open_from_cashier', 'Manual Open from Cashier', 'فتح يدوي من أمين الصندوق', '', 1),
(268, 'remote_pushbutton_open', 'Remote/Push Button Open', 'فتح عن بعد / زر الضغط', '', 1),
(269, 'sel_duration', 'Select Parking Duration', 'حدد مدة وقوف السيارات', '', 1),
(270, 'less_1_day', 'Less than 1 day', 'أقل من يوم واحد', '', 1),
(271, 'greater_1_day', 'Greater than 1 Day', 'أكثر من يوم واحد', '', 1),
(272, 'greater_2_day', 'Greater than 2 Days', 'أكثر من يومين', '', 1),
(273, 'greater_3_day', 'Greater than 3 Days', 'أكثر من 3 أيام', '', 1),
(274, 'greater_4_day', 'Greater than 4 Days', 'أكثر من 4 أيام', '', 1),
(275, 'greater_5_day', 'Greater than 5 Days', 'أكثر من 5 أيام', '', 1),
(276, 'greater_6_day', 'Greater than 6 Days', 'أكثر من 6 أيام', '', 1),
(277, 'greater_7_day', 'Greater than 7 Days', 'أكثر من 7 أيام', '', 1),
(278, 'anpr_image', 'ANPR Image', 'صورة ANPR', '', 1),
(279, 'plate_type', 'Type', 'نوع لوحة', '', 1),
(280, 'plate_area', 'Area', 'منطقة لوحة', '', 1),
(281, 'police', 'Police', 'شرطة', '', 1),
(282, 'transport', 'Transport', 'المواصلات', '', 1),
(283, 'commercial', 'Commercial', 'تجاري', '', 1),
(284, 'private', 'Private', 'خاص', '', 1),
(285, 'learning', 'Learning', 'تعلم', '', 1),
(286, 'cddiplomat', 'CD Diplomat', 'دبلوماسي CD', '', 1),
(287, 'ccdiplomat', 'CC Diplomat', 'دبلوماسي CC', '', 1),
(288, 'protocol', 'Protocol', 'بروتوكول', '', 1),
(289, 'government', 'Government', 'حكومة', '', 1),
(290, 'export', 'Export', 'تصدير', '', 1),
(291, 'taxi', 'Taxi', 'سيارة اجره', '', 1),
(292, 'testing', 'Testing', 'اختبارات', '', 1),
(293, 'transfer', 'Transfer', 'نقل', '', 1),
(294, 'truck', 'Truck', 'شاحنة نقل', '', 1),
(295, 'bus', 'Bus', 'حافلة', '', 1),
(296, 'publictransport', 'Public Transport', 'النقل العام', '', 1),
(297, 'privatetransport', 'Private Transport', 'النقل الخاص', '', 1),
(298, 'select_validator', 'Select Validator', 'حدد المدقق', '', 1),
(299, 'select_product', 'Select Products', 'حدد المنتج', '', 1),
(300, 'all_validators', 'All Validator', 'كل المدقق', '', 1),
(301, 'all_products', 'All Products', 'جميع المنتجات', '', 1),
(302, 'parking_duration', 'Parking Duration', 'مدة وقوف السيارات', '', 1),
(303, 'maximum_parking_duration', 'Maximum Parking Duration', 'المدة القصوى لوقوف السيارات', '', 1),
(304, 'minimum_parking_duration', 'Minimum Parking Duration', 'الحد الأدنى لمدة وقوف السيارات', '', 1),
(305, 'average_parking_duration', 'Average Parking Duration', 'متوسط ​​مدة وقوف السيارات', '', 1),
(306, 'shortterm', 'Short-Term', 'المدى القصير', '', 1),
(307, 'shortterm_parkers', 'Short-Term Parkers', 'مواقف سيارات قصيرة المدى', '', 1),
(308, 'contract_parkers', 'Contract Parkers', 'باركرز العقد', '', 1),
(309, 'watchdog_network_logs', 'Watchdog Network Logs', 'سجلات شبكة المراقبة', '', 1),
(310, 'ip_address', 'IP Address', 'عنوان IP', '', 1),
(311, 'connection_lost', 'Connection Lost', 'فقد الاتصال', '', 1),
(312, 'connection_restored', 'Connection Restored', 'تمت استعادة الاتصال', '', 1);

-- --------------------------------------------------------

--
-- Table structure for table `web_application_messages`
--

CREATE TABLE `web_application_messages` (
  `id` int(11) NOT NULL,
  `message` varchar(100) DEFAULT NULL,
  `english` text DEFAULT NULL,
  `arabic` text DEFAULT NULL,
  `spanish` text DEFAULT NULL,
  `status` int(11) NOT NULL DEFAULT 1
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `web_application_messages`
--

INSERT INTO `web_application_messages` (`id`, `message`, `english`, `arabic`, `spanish`, `status`) VALUES
(1, 'invalid_carpark', 'Access denied.Invalid car park ', 'الدخول مرفوض.موقف سيارات غير صالح', NULL, 1),
(2, 'invalid_facility', 'Access denied Invalid facility', 'الوصول مرفوض. منشأة غير صالحة', NULL, 1),
(3, 'invalid_access_parking_zone', 'Access denied.Invalid access parking zone', 'الوصول مرفوض. منشأة غير صالحة', NULL, 1),
(4, 'access_expired', 'Access card expired', 'الوصول مرفوض. منشأة غير صالحة', NULL, 1),
(5, 'access_not_started', 'Access card validity not started', 'الوصول مرفوض. منشأة غير صالحة', NULL, 1),
(6, 'invalid_access_carpark', 'Access denied.Invalid car park ', 'الوصول مرفوض. منشأة غير صالحة', NULL, 1),
(7, 'invalid_access_facility', 'Access denied.Invalid Facility', 'الوصول مرفوض. منشأة غير صالحة', NULL, 1),
(8, 'allow_access', 'Access Allowed using access card', 'الوصول مرفوض. منشأة غير صالحة', NULL, 1),
(9, 'not_in_access_whitelist', 'Access denied.Not in access whitelist', 'الوصول مرفوض. منشأة غير صالحة', NULL, 1),
(10, 'invalid_reservation_carpark', 'Access denied.Invalid reservation car park ', 'الدخول مرفوض.موقف سيارات غير صالح', NULL, 1),
(11, 'invalid_reservation_facility', 'Access denied Invalid reservation facility', 'الوصول مرفوض. منشأة غير صالحة', NULL, 1),
(12, 'reservation_allow_access', 'access allowed.Using reservation', 'الوصول مرفوض. منشأة غير صالحة', NULL, 1),
(13, 'not_in_reservation_time_slot', 'AccessDenied.Not in reservatin timeslot', 'الوصول مرفوض. منشأة غير صالحة', NULL, 1),
(14, 'already_exited_reservation', 'AccessDenied.Reservation already used', 'الوصول مرفوض. منشأة غير صالحة', NULL, 1),
(15, 'blacklisted', 'AccessDenied.Blacklisted ticket', 'الوصول مرفوض. منشأة غير صالحة', NULL, 1),
(16, 'within_validation_hours', 'Access allowed.Within validation hours', 'الوصول مرفوض. منشأة غير صالحة', NULL, 1),
(17, 'within_entry_grace_period', 'Access allowed.Within entry grace period', 'الوصول مرفوض. منشأة غير صالحة', NULL, 1),
(18, 'within_exit_grace_period', 'Access allowed.Within exit grace period', 'الوصول مرفوض. منشأة غير صالحة', NULL, 1),
(19, 'unpaid', 'AccessDenied.Unpaid', 'الوصول مرفوض. منشأة غير صالحة', NULL, 1),
(20, 'already_exited', 'AccessDenied.Already exited', 'الوصول مرفوض. منشأة غير صالحة', NULL, 1),
(21, 'exit_grace_period_exceeded', 'AccessDenied.Exit grace period exceeded', 'الوصول مرفوض. منشأة غير صالحة', NULL, 1),
(22, 'wallet_not_present', 'Wallet not present', 'الوصول مرفوض. منشأة غير صالحة', NULL, 1),
(23, 'entry_with_no_plate', 'ANPR entry with no plate', 'دخول بدون لوحة', NULL, 1),
(24, 'alarm_121', 'Printer unavailable to print receipt', 'الطابعة غير متوفرة لطباعة الإيصال', NULL, 1),
(25, 'alarm_111', 'Unable to open USB PORT', 'تعذر فتح منفذ USB', NULL, 1),
(26, 'alarm_131', 'No chip coin present in the machine,Machine is in out of service mode', 'لا توجد عملة معدنية في الجهاز ، الجهاز في وضع خارج الخدمة', NULL, 1),
(27, 'alarm_141', 'Server Offline', 'الخادم غير متصل', NULL, 1),
(28, 'alarm_111', 'Network Offline', 'الشبكة غير متصل', NULL, 1),
(29, 'alarm_161', 'Ticket printing failed', 'فشلت طباعة التذكرة', NULL, 1),
(30, 'plate_msmatch', 'plate mismatch', 'عدم تطابق اللوحة', NULL, 1),
(31, 'alarm_142', 'Server Online', 'الخادم عبر الإنترنت', NULL, 1);

-- --------------------------------------------------------

--
-- Table structure for table `whitelist_group_policy`
--

CREATE TABLE `whitelist_group_policy` (
  `id` int(11) NOT NULL,
  `policy_name` varchar(25) DEFAULT NULL,
  `description` text DEFAULT NULL,
  `start_date` date NOT NULL,
  `expiry_date` date NOT NULL,
  `validity_days` text NOT NULL,
  `validity_timeslot` text NOT NULL,
  `facility_number` int(11) NOT NULL,
  `carpark_number` varchar(25) DEFAULT NULL,
  `parking_zone` varchar(100) NOT NULL,
  `status` int(1) NOT NULL DEFAULT 1
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `access_anpr_checks`
--
ALTER TABLE `access_anpr_checks`
  ADD PRIMARY KEY (`anpr_check_id`);

--
-- Indexes for table `access_whitelist`
--
ALTER TABLE `access_whitelist`
  ADD PRIMARY KEY (`access_whitelist_id`);

--
-- Indexes for table `access_whitelist_customers`
--
ALTER TABLE `access_whitelist_customers`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `alerts_alarms`
--
ALTER TABLE `alerts_alarms`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `alerts_counter`
--
ALTER TABLE `alerts_counter`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `cashier_user_roles`
--
ALTER TABLE `cashier_user_roles`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `cooperate_users`
--
ALTER TABLE `cooperate_users`
  ADD PRIMARY KEY (`user_id`);

--
-- Indexes for table `customer_messages`
--
ALTER TABLE `customer_messages`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `daemon_status`
--
ALTER TABLE `daemon_status`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `DataExportSettings`
--
ALTER TABLE `DataExportSettings`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `device_application_label`
--
ALTER TABLE `device_application_label`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `device_maintenance`
--
ALTER TABLE `device_maintenance`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `interface_settings`
--
ALTER TABLE `interface_settings`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `manual_movements_reasons`
--
ALTER TABLE `manual_movements_reasons`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `parking_ewallet`
--
ALTER TABLE `parking_ewallet`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `pgs_counter_alarm`
--
ALTER TABLE `pgs_counter_alarm`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `pgs_interface_config`
--
ALTER TABLE `pgs_interface_config`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `plates_ewallet`
--
ALTER TABLE `plates_ewallet`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `pxcloud_download_settings`
--
ALTER TABLE `pxcloud_download_settings`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `pxcloud_upload_settings`
--
ALTER TABLE `pxcloud_upload_settings`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `receipt_settings`
--
ALTER TABLE `receipt_settings`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `reservation_user`
--
ALTER TABLE `reservation_user`
  ADD PRIMARY KEY (`user_id`),
  ADD UNIQUE KEY `email` (`email`);

--
-- Indexes for table `revenue_banknote_denominations`
--
ALTER TABLE `revenue_banknote_denominations`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `revenue_coupons_whitelist`
--
ALTER TABLE `revenue_coupons_whitelist`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `revenue_discounts`
--
ALTER TABLE `revenue_discounts`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `revenue_dynamic_parking_rates`
--
ALTER TABLE `revenue_dynamic_parking_rates`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `revenue_fixed_parking_rates`
--
ALTER TABLE `revenue_fixed_parking_rates`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `revenue_parking_rates`
--
ALTER TABLE `revenue_parking_rates`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `revenue_parking_rate_label`
--
ALTER TABLE `revenue_parking_rate_label`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `revenue_products`
--
ALTER TABLE `revenue_products`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `system_carparks`
--
ALTER TABLE `system_carparks`
  ADD PRIMARY KEY (`carpark_id`);

--
-- Indexes for table `system_chipcoin`
--
ALTER TABLE `system_chipcoin`
  ADD PRIMARY KEY (`serial_number`);

--
-- Indexes for table `system_devices`
--
ALTER TABLE `system_devices`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `system_device_category`
--
ALTER TABLE `system_device_category`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `system_navigation`
--
ALTER TABLE `system_navigation`
  ADD PRIMARY KEY (`menu_id`),
  ADD UNIQUE KEY `menu_name` (`menu_name`);

--
-- Indexes for table `system_navigation_group`
--
ALTER TABLE `system_navigation_group`
  ADD PRIMARY KEY (`group_id`);

--
-- Indexes for table `system_navigation_header`
--
ALTER TABLE `system_navigation_header`
  ADD PRIMARY KEY (`header_id`);

--
-- Indexes for table `system_parking_zone`
--
ALTER TABLE `system_parking_zone`
  ADD PRIMARY KEY (`parking_zone_number`);

--
-- Indexes for table `system_products`
--
ALTER TABLE `system_products`
  ADD PRIMARY KEY (`product_id`);

--
-- Indexes for table `system_role_rights`
--
ALTER TABLE `system_role_rights`
  ADD PRIMARY KEY (`role_rights_id`),
  ADD KEY `role_id` (`user_role_id`),
  ADD KEY `menu_id` (`menu_id`);

--
-- Indexes for table `system_settings`
--
ALTER TABLE `system_settings`
  ADD PRIMARY KEY (`settings_id`);

--
-- Indexes for table `system_users`
--
ALTER TABLE `system_users`
  ADD PRIMARY KEY (`user_id`),
  ADD UNIQUE KEY `email` (`email`),
  ADD KEY `user_role_id` (`user_role_id`);

--
-- Indexes for table `system_user_role`
--
ALTER TABLE `system_user_role`
  ADD PRIMARY KEY (`user_role_id`),
  ADD UNIQUE KEY `user_role_name` (`user_role_name`);

--
-- Indexes for table `system_validators`
--
ALTER TABLE `system_validators`
  ADD PRIMARY KEY (`user_id`);

--
-- Indexes for table `system_vms_devices`
--
ALTER TABLE `system_vms_devices`
  ADD PRIMARY KEY (`vms_id`);

--
-- Indexes for table `valet_drivers`
--
ALTER TABLE `valet_drivers`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `vms_communication_config`
--
ALTER TABLE `vms_communication_config`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `vms_device_shedule`
--
ALTER TABLE `vms_device_shedule`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `web_application_labels`
--
ALTER TABLE `web_application_labels`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `web_application_messages`
--
ALTER TABLE `web_application_messages`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `whitelist_group_policy`
--
ALTER TABLE `whitelist_group_policy`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `access_anpr_checks`
--
ALTER TABLE `access_anpr_checks`
  MODIFY `anpr_check_id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `access_whitelist`
--
ALTER TABLE `access_whitelist`
  MODIFY `access_whitelist_id` int(25) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `access_whitelist_customers`
--
ALTER TABLE `access_whitelist_customers`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `alerts_alarms`
--
ALTER TABLE `alerts_alarms`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `alerts_counter`
--
ALTER TABLE `alerts_counter`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `cashier_user_roles`
--
ALTER TABLE `cashier_user_roles`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;

--
-- AUTO_INCREMENT for table `customer_messages`
--
ALTER TABLE `customer_messages`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=107;

--
-- AUTO_INCREMENT for table `daemon_status`
--
ALTER TABLE `daemon_status`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=11;

--
-- AUTO_INCREMENT for table `DataExportSettings`
--
ALTER TABLE `DataExportSettings`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;

--
-- AUTO_INCREMENT for table `device_application_label`
--
ALTER TABLE `device_application_label`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=237;

--
-- AUTO_INCREMENT for table `device_maintenance`
--
ALTER TABLE `device_maintenance`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=33;

--
-- AUTO_INCREMENT for table `interface_settings`
--
ALTER TABLE `interface_settings`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;

--
-- AUTO_INCREMENT for table `manual_movements_reasons`
--
ALTER TABLE `manual_movements_reasons`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

--
-- AUTO_INCREMENT for table `parking_ewallet`
--
ALTER TABLE `parking_ewallet`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `pgs_counter_alarm`
--
ALTER TABLE `pgs_counter_alarm`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

--
-- AUTO_INCREMENT for table `pgs_interface_config`
--
ALTER TABLE `pgs_interface_config`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=10;

--
-- AUTO_INCREMENT for table `plates_ewallet`
--
ALTER TABLE `plates_ewallet`
  MODIFY `id` int(25) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `pxcloud_download_settings`
--
ALTER TABLE `pxcloud_download_settings`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;

--
-- AUTO_INCREMENT for table `pxcloud_upload_settings`
--
ALTER TABLE `pxcloud_upload_settings`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=17;

--
-- AUTO_INCREMENT for table `receipt_settings`
--
ALTER TABLE `receipt_settings`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=37;

--
-- AUTO_INCREMENT for table `reservation_user`
--
ALTER TABLE `reservation_user`
  MODIFY `user_id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `revenue_banknote_denominations`
--
ALTER TABLE `revenue_banknote_denominations`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT for table `revenue_coupons_whitelist`
--
ALTER TABLE `revenue_coupons_whitelist`
  MODIFY `id` int(25) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `revenue_discounts`
--
ALTER TABLE `revenue_discounts`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `revenue_dynamic_parking_rates`
--
ALTER TABLE `revenue_dynamic_parking_rates`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `revenue_fixed_parking_rates`
--
ALTER TABLE `revenue_fixed_parking_rates`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=14;

--
-- AUTO_INCREMENT for table `revenue_parking_rates`
--
ALTER TABLE `revenue_parking_rates`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=11;

--
-- AUTO_INCREMENT for table `revenue_parking_rate_label`
--
ALTER TABLE `revenue_parking_rate_label`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT for table `revenue_products`
--
ALTER TABLE `revenue_products`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `system_carparks`
--
ALTER TABLE `system_carparks`
  MODIFY `carpark_id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `system_devices`
--
ALTER TABLE `system_devices`
  MODIFY `id` int(5) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `system_device_category`
--
ALTER TABLE `system_device_category`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=12;

--
-- AUTO_INCREMENT for table `system_navigation`
--
ALTER TABLE `system_navigation`
  MODIFY `menu_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=105;

--
-- AUTO_INCREMENT for table `system_navigation_group`
--
ALTER TABLE `system_navigation_group`
  MODIFY `group_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=12;

--
-- AUTO_INCREMENT for table `system_navigation_header`
--
ALTER TABLE `system_navigation_header`
  MODIFY `header_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

--
-- AUTO_INCREMENT for table `system_parking_zone`
--
ALTER TABLE `system_parking_zone`
  MODIFY `parking_zone_number` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `system_products`
--
ALTER TABLE `system_products`
  MODIFY `product_id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `system_role_rights`
--
ALTER TABLE `system_role_rights`
  MODIFY `role_rights_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=54;

--
-- AUTO_INCREMENT for table `system_settings`
--
ALTER TABLE `system_settings`
  MODIFY `settings_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=81;

--
-- AUTO_INCREMENT for table `system_users`
--
ALTER TABLE `system_users`
  MODIFY `user_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=48;

--
-- AUTO_INCREMENT for table `system_user_role`
--
ALTER TABLE `system_user_role`
  MODIFY `user_role_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=108;

--
-- AUTO_INCREMENT for table `system_validators`
--
ALTER TABLE `system_validators`
  MODIFY `user_id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `system_vms_devices`
--
ALTER TABLE `system_vms_devices`
  MODIFY `vms_id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `valet_drivers`
--
ALTER TABLE `valet_drivers`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `vms_communication_config`
--
ALTER TABLE `vms_communication_config`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `vms_device_shedule`
--
ALTER TABLE `vms_device_shedule`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `web_application_labels`
--
ALTER TABLE `web_application_labels`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=313;

--
-- AUTO_INCREMENT for table `web_application_messages`
--
ALTER TABLE `web_application_messages`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=32;

--
-- AUTO_INCREMENT for table `whitelist_group_policy`
--
ALTER TABLE `whitelist_group_policy`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
