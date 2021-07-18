-- phpMyAdmin SQL Dump
-- version 4.6.6deb5
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Generation Time: Jul 18, 2021 at 12:58 PM
-- Server version: 10.3.23-MariaDB-0+deb10u1
-- PHP Version: 7.3.27-1~deb10u1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `ParcxTerminal`
--

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

--
-- Dumping data for table `access_whitelist`
--

INSERT INTO `access_whitelist` (`access_whitelist_id`, `access_id`, `customer_name`, `ticket_id`, `plate_number`, `corporate_parker`, `validity_start_date`, `validity_expiry_date`, `tag_serial`, `tag`, `tag_tid`, `access_zones`, `carpark_number`, `facility_number`, `creation_date_time`, `status`, `antipassback_enabled`, `last_visited_date_time`, `wallet_payment`, `season_card`, `wallet_user_id`, `company_name`, `last_sync`, `country`, `operator_id`, `user_qrcode`, `description`, `cloud_upload_status`, `personalized_message_line1`, `personalized_message_line2`) VALUES
(2, 35810209, 'Parcx Parcx1', '5a4a189d', 'KUW 1 12121', 201, '2021-02-18', '2021-03-11', '', '', '', '', '1', 200005, '2021-02-18 11:16:48', 1, 1, NULL, 1, 1, 234, '', '2021-02-25 11:01:50', '', 0, NULL, '', 1, NULL, NULL),
(3, 35810208, 'Parcx Parcx2', '5a4a1891', 'KUW 1 11111', 201, '2021-02-18', '2021-03-11', '', '', '', '', '1,1', 200005, '2021-02-18 12:05:33', 1, 1, NULL, 1, 1, 234, '', '2021-02-25 11:01:50', '', 0, NULL, '', 1, NULL, NULL),
(4, 35810201, 'Parcx Parcx3', '5a4a1892', 'h 95942', 201, '2021-02-18', '2021-03-11', '', '', '', '', '1,1', 200005, '2021-02-18 12:05:33', 1, 1, NULL, 1, 1, 234, '', '2021-02-25 11:01:50', '', 0, NULL, '', 1, NULL, NULL),
(5, 35810202, 'Parcx Parcx4', '5a4a1893', 'h 95944', 201, '2021-02-18', '2021-03-11', '', '', '', '', '1,1', 200005, '2021-02-18 12:05:33', 1, 1, NULL, 1, 1, 234, '', '2021-02-25 11:01:50', '', 0, NULL, '', 1, NULL, NULL),
(6, 35810203, 'Parcx Parcx5', '5a4a1894', 'OMN W 99790', 201, '2021-02-18', '2021-03-11', '', '', '', '', '1,1', 200005, '2021-02-18 12:05:33', 1, 1, NULL, 1, 1, 234, '', '2021-02-25 11:01:50', '', 0, NULL, '', 1, NULL, NULL),
(7, 35810203, 'Parcx Parcx6', '5a4a1894', 'OMN S 63899', 0, '2021-02-18', '2021-03-11', '', '', '', '', '1,1', 200005, '2021-02-18 12:05:33', 1, 1, NULL, 1, 1, 234, '', '2021-02-25 11:01:50', '', 0, NULL, '', 1, NULL, NULL),
(8, 35810203, 'Parcx Parcx7', '5a4a1894', 'OMN YR 3', 0, '2021-02-18', '2021-03-11', '', '', '', '', '1,1', 200005, '2021-02-18 12:05:33', 1, 1, NULL, 1, 1, 234, '', '2021-02-25 11:01:50', '', 0, NULL, '', 1, NULL, NULL),
(9, 35810203, 'Jeena 1', '123456789', 'OMN TB 2544', 0, '2021-02-18', '2021-03-11', '', '', '', '', '1,1', 200005, '2021-02-18 12:05:33', 1, 1, NULL, 1, 1, 234, '', '2021-02-25 11:01:50', '', 0, NULL, '', 1, NULL, NULL),
(10, 35810203, 'Jeena 1', '123456789', 'OMN HS 7125', 0, '2021-02-18', '2021-03-11', '', '', '', '', '1,1', 200005, '2021-02-18 12:05:33', 1, 1, NULL, 1, 1, 234, '', '2021-02-25 11:01:50', '', 0, NULL, '', 1, NULL, NULL),
(11, 36928423, 'jeena', '20210520112820', 'BAH A 11221', 0, '2021-03-03', '2021-03-31', '', '', '', '', '1', 200005, '2021-03-03 09:53:43', 1, 0, NULL, 0, 0, 0, NULL, '2021-03-03 09:53:43', 'BAH', 0, NULL, NULL, 1, '', ''),
(12, 36928466, 'Neethu', '20210303095400', 'KUW R 5678', 0, '2021-03-03', '2021-03-31', '', '', '', '', '1', 200005, '2021-03-03 09:54:26', 1, 0, NULL, 0, 0, 0, NULL, '2021-03-03 09:54:26', 'KUW', 0, NULL, NULL, 1, '', ''),
(13, 37036328, 'jeena', '20210520112820', 'DXB H 95943', 0, '2021-03-04', '2021-03-31', '', '', '', '', '1', 200005, '2021-03-04 15:52:08', 1, 0, NULL, 0, 0, 0, NULL, '2021-03-04 15:52:08', 'UAE', 0, NULL, NULL, 1, '', ''),
(14, 0, 'jeenalq', '20210217043734', 'AUH H 12331', 0, '2021-02-01', '2021-02-28', '', '', '', '', '1', 100001, '2021-02-17 16:38:06', 1, 0, NULL, 0, 0, 0, '', '2021-07-15 15:41:17', '', 0, NULL, '', 1, NULL, NULL),
(15, 35810208, 'Parcx Parcx', '5a4a189d', 'KUW 1 12121', 0, '2021-02-18', '2021-03-11', '', '', '', '', '1,1', 100001, '2021-02-18 11:16:48', 1, 1, NULL, 1, 1, 234, '', '2021-07-15 13:38:26', '', 0, NULL, '', 1, NULL, NULL),
(16, 35827721, 'Parcx Parcx', '5a4a189d', 'KUW 2 12345', 0, '2021-02-18', '2021-03-04', '', '', '', '', '1', 100001, '2021-02-18 16:08:41', 1, 1, NULL, 1, 1, 234, '', '2021-07-15 15:43:48', '', 0, NULL, '', 1, NULL, NULL),
(17, 35806747, 'John', '20210218101813', 'DXB A 21313', 0, '2021-02-17', '2021-02-28', '', '', '', '', '1', 100001, '2021-02-18 10:19:07', 1, 0, NULL, 0, 0, 0, '', '2021-07-18 10:59:15', '', 0, NULL, '', 1, NULL, NULL);

-- --------------------------------------------------------

--
-- Table structure for table `access_whitelist_bak`
--

CREATE TABLE `access_whitelist_bak` (
  `id` int(25) NOT NULL,
  `customer_name` varchar(100) NOT NULL,
  `ticket_id` varchar(50) DEFAULT 'NULL',
  `plate_number` varchar(50) NOT NULL,
  `tag` varchar(32) NOT NULL,
  `tag_serial` varchar(30) NOT NULL,
  `start_date` date NOT NULL,
  `expiry_date` date NOT NULL,
  `access_zones` text NOT NULL,
  `carpark_number` varchar(50) NOT NULL DEFAULT '1',
  `facility_number` int(11) NOT NULL DEFAULT 1,
  `tag_tid` text NOT NULL,
  `antipassback` int(11) NOT NULL DEFAULT 0,
  `status` int(11) NOT NULL DEFAULT 1,
  `personalized_message_line1` varchar(250) DEFAULT NULL,
  `personalized_message_line2` text DEFAULT NULL,
  `creation_date` datetime NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `access_whitelist_bak`
--

INSERT INTO `access_whitelist_bak` (`id`, `customer_name`, `ticket_id`, `plate_number`, `tag`, `tag_serial`, `start_date`, `expiry_date`, `access_zones`, `carpark_number`, `facility_number`, `tag_tid`, `antipassback`, `status`, `personalized_message_line1`, `personalized_message_line2`, `creation_date`) VALUES
(372, 'Sam', '20201215121205', 'KUW 93 11345', '', '', '2020-12-15', '2021-06-03', '', '1', 100001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(373, 'Fahad Hasware', '20210111030205', 'KUW 37 8135', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(374, 'Nawwaf Al Shammari', '20210111033259', 'KUW 16 79244', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(375, 'Nawwaf Al Shammari', '20210111033729', 'KUW 17 33646', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(376, 'Nawwaf Al Shammari', '20210111033922', 'KUW 30 19442', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(377, 'Naeema Hamad', '20210111034028', 'KUW 16 89731', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(378, 'Shukri Othman', '20210111034157', 'KUW 2 65297', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(379, 'Mohammed Khulaif', '20210111034822', 'KUW 10 6527', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(380, 'Rami Ibrahim', '20210111034915', 'KUW 2 70735', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(381, 'Sameer Al Masri', '20210111035056', 'KUW 16 87123', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(382, 'Sameer Al Masri', '20210111041608', 'KUW 10 86078', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(383, 'Samar Ahmed', '20210111041703', 'KUW 15 37952', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(384, 'Fahad Hasware', '20210111041851', 'KUW 18 1186', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(385, 'Yara Muneer', '20210111042238', 'KUW 7 15325', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(386, 'Abdullah AL Khamiri', '20210111042325', 'KUW 17 73265', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(387, 'Nasser AL Adwani', '20210111042423', 'KUW 30 34364', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(388, 'Amr Ghazali', '20210111042516', 'KUW 20 33461', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 0, '', '', '2021-07-15 08:31:43'),
(389, 'Amjad Karim', '20210111042552', 'KUW 1 76686', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 0, '', '', '2021-07-15 08:31:43'),
(390, 'Khaja Asif', '20210111042640', 'KUW 4 72302', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(391, 'Sabiha Abdullah', '20210112023654', 'KUW 7 54353', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(392, 'Zakaria Ateyya', '20210112023802', 'KUW 6 57293', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 0, '', '', '2021-07-15 08:31:43'),
(393, 'Ahmad Abdulaal', '20210112023923', 'KUW 10 90454', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(394, 'Saqer Al Shammari', '20210112024323', 'KUW 4 5777', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 0, '', '', '2021-07-15 08:31:43'),
(395, 'Ahmad Shakeel', '20210112024410', 'KUW 1 48656', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(396, 'Bodour AL Mulla', '20210112024552', 'KUW 50 19334', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(397, 'Qutaiba AL Dakheel', '20210112024721', 'KUW 50 23246', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:43'),
(398, 'Mahmoud Abdulfattah', '20210112024805', 'KUW 2 49110', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 0, '', '', '2021-07-15 08:31:43'),
(399, 'Hamad Al Awadhi', '20210112024855', 'KUW 14 40013', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:44'),
(400, 'Faiz Rashid', '20210112024944', 'KUW 4 72352', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:44'),
(401, 'Rami Ibrahim', '20210112025046', 'KUW 47 7690', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:44'),
(402, 'AL Sayed Abdulaal', '20210113112551', 'KUW 10 54553', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:44'),
(403, 'Faisal Al Quraini', '20210211043808', 'KUW 40 75933', '', '', '2021-02-10', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:44'),
(404, 'Sondos ', '20210211044253', 'KUW 13 42676', '', '', '2021-02-10', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:44'),
(405, 'Sharath Rao', '20210211044901', 'KUW 13 82223', '', '', '2021-02-10', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:44'),
(406, 'Ahmad Al Kandari', '20210211045834', 'KUW 10 6646', '', '', '2021-02-10', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:44'),
(407, 'Rolla', '20210211045858', 'KUW 2 55748', '', '', '2021-02-10', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:44'),
(408, 'Yasser Ahmad ', '20210214103324', 'KUW 9 43947', '', '', '2021-02-11', '2021-12-31', '', '1', 200001, '', 0, 0, '', '', '2021-07-15 08:31:44'),
(409, 'Ahmad Al Kandari', '20210214115413', 'KUW 17 18418', '', '', '2021-02-11', '2021-12-31', '', '1', 100001, '', 0, 1, '', '', '2021-07-15 08:31:44'),
(410, 'Sara Al Moued', '20210214122402', 'KUW 50 97509', '', '', '2021-02-11', '2021-12-31', '', '1', 200001, '', 0, 0, '', '', '2021-07-15 08:31:44'),
(411, 'Abdelrahman Magdy Abdelsatar', '20210214124548', 'KUW 15 29729', '', '', '2021-01-01', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:44'),
(412, 'Azzaz Al Hothail', '20210214035552', 'KUW 8 56605', '', '', '2021-02-11', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:44'),
(413, 'Azzaz Al Hothail', '20210214035626', 'KUW 43 6380', '', '', '2021-02-11', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:44'),
(414, 'Yousef ', '20210215100536', 'KUW 10 70855', '', '', '2021-02-11', '2021-12-31', '', '1', 200001, '', 0, 0, '', '', '2021-07-15 08:31:44'),
(416, 'Talar', '20210215100951', 'KUW 15 20520', '', '', '2021-02-11', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:44'),
(417, 'Abbas Al Busabeeh', '20210222022254', 'KUW 16 38860', '', '', '2021-02-22', '2021-12-31', '', '1', 100001, '', 0, 0, '', '', '2021-07-15 08:31:44'),
(418, 'nthadani nthadani', '5a51fdfe', 'KUW 1 11111', '', '', '2021-02-25', '2022-03-10', '', '1', 200001, '', 1, 1, '', '', '2021-07-15 08:31:44'),
(419, 'nthadani nthadani', '5a51fdfe', 'AUH 1 11111', '', '', '2021-02-25', '2022-03-03', '', '1', 200001, '', 1, 1, '', '', '2021-07-15 08:31:44'),
(421, 'Abdullah Al Khamiri', '20210224010851', 'KUW 50 93489', '', '', '2021-02-17', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:44'),
(422, 'NeetuPillai', '5a4e258e', 'DUB M 72702', '', '', '2021-02-22', '2021-04-07', '', '1', 200001, '', 1, 1, '', '', '2021-07-15 08:31:44'),
(424, 'Ravin Bernard ', '5a5241e4', 'KUW 7 79727', '', '', '2021-02-25', '2021-03-27', '', '1', 200001, '', 1, 1, '', '', '2021-07-15 08:31:44'),
(425, 'Ravin Bernard ', '5a5241e4', 'KUW 16 52599', '', '', '2021-02-25', '2021-03-27', '', '1', 200001, '', 1, 1, '', '', '2021-07-15 08:31:44'),
(426, 'John', '5a5241e4', 'KUW 17 52599', '', '', '2021-02-25', '2021-07-31', '', '1', 100001, '', 1, 1, '', '', '2021-07-15 08:31:44'),
(427, 'Rawan Al Muallem', '20210321122610', 'KUW 40 18199', '', '', '2021-03-15', '2021-12-31', '', '1', 200001, '', 0, 0, '', '', '2021-07-15 08:31:44'),
(428, 'Tamer', '20210321124304', 'KUW 40 69108', '', '', '2021-03-20', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:44'),
(429, 'Daifallah Husain', '20210404042810', 'KUW 16 91991', '', '', '2021-04-04', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:44'),
(430, 'Tamer Taj AL Arefeen', '20210404042959', 'KUW 5 87353', '', '', '2021-04-04', '2021-12-31', '', '1', 200001, '', 0, 0, '', '', '2021-07-15 08:31:44'),
(431, 'Ismaiel Mahmoud', '20210404043928', 'KUW 30 57973', '', '', '2021-04-04', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:44'),
(432, 'Ibrahim Zakaria', '20210404045636', 'KUW 44 5244', '', '', '2021-04-04', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:44'),
(433, 'Sameh Farouq', '20210413031710', 'KUW 16 81641', '', '', '2021-04-12', '2021-12-31', '', '1', 200001, '', 0, 0, '', '', '2021-07-15 08:31:44'),
(434, 'Saqer Awad', '20210413031750', 'KUW 5 36578', '', '', '2021-04-12', '2021-12-31', '', '1', 200001, '', 0, 1, '', '', '2021-07-15 08:31:44'),
(435, 'Neetu Pillai', '20210627025825', 'DXB M 72702', '', '', '2021-06-28', '2021-07-28', '', '1', 100001, '', 0, 1, 'Welcome', 'Personal Message 2', '2021-07-15 08:31:44'),
(436, 'Maya kartha', '856936', 'AUH Y 36524', '856936', '', '2021-07-04', '2021-08-31', '', '1', 1001, '', 0, 1, '', '', '2021-07-15 08:31:44');

-- --------------------------------------------------------

--
-- Table structure for table `customer_messages`
--

CREATE TABLE `customer_messages` (
  `id` int(11) NOT NULL,
  `message` varchar(100) NOT NULL,
  `media_path` text NOT NULL,
  `english_line1` varchar(100) NOT NULL DEFAULT 'NA',
  `english_line2` varchar(50) NOT NULL DEFAULT 'NA',
  `english_line3` varchar(50) NOT NULL DEFAULT 'NA',
  `arabic_line1` varchar(100) NOT NULL DEFAULT 'NA',
  `arabic_line2` varchar(100) NOT NULL DEFAULT 'NA',
  `arabic_line3` varchar(100) NOT NULL DEFAULT 'NA',
  `spanish_line1` varchar(100) DEFAULT 'NULL',
  `spanish_line2` varchar(100) DEFAULT 'NULL',
  `spanish_line3` varchar(100) DEFAULT 'NULL',
  `status` tinyint(4) NOT NULL DEFAULT 1
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `customer_messages`
--

INSERT INTO `customer_messages` (`id`, `message`, `media_path`, `english_line1`, `english_line2`, `english_line3`, `arabic_line1`, `arabic_line2`, `arabic_line3`, `spanish_line1`, `spanish_line2`, `spanish_line3`, `status`) VALUES
(1, 'EntryPresenceLoopActivatedChipcoin', 'EntryPushButtonChipcoin.gif', 'Push button for chipcoin', '', '', '', '', '', '', '', '', 1),
(2, 'TakeEntryTicket', 'EntryTakeTicket.gif', 'Please take the ticket', '', '', '', '', '', '', '', '', 1),
(3, 'Processing', 'Processing.gif', 'Processing..', 'Please Wait', '', '', '', '', '', '', '', 1),
(4, 'EntryStandBy', 'TerminalBackgroundEntry.jpg', 'CARPARK NAME', '', '', '', '', '', '', '', '', 1),
(5, 'AccessAllowedEntry', 'AccessAllowed.gif', 'Welcome', '', '', '', '', '', '', '', '', 1),
(6, 'AccessDeniedUnpaidTicket', 'UnpaidTicket.gif', 'Unpaid Parking Fee', 'Pay at Payment Machine ', 'Or Scan coupons', '', '', '', '', '', '', 1),
(7, 'AccessAllowedExit', 'ThankYou.gif', 'Thank You for visiting', '', '', '', '', '', '', '', '', 1),
(8, 'AccessDeniedInvalidTicket', 'InvalidTicket.gif', 'Invalid Ticket', '', '', '', '', '', '', '', '', 1),
(9, 'AccessDeniedWrongCarpark', 'InvalidTicket.gif', 'Incorrect Car Park ', '', '', '', '', '', '', '', '', 1),
(10, 'AccessDeniedWrongFacility', 'InvalidTicket.gif', 'Incorrect Car Park ', '', '', '', '', '', '', '', '', 1),
(11, 'AccessDeniedExitGrace', 'GracePeriodExceeded.jpg', 'Exit Grace period exceeded', '', '', '', '', '', '', '', '', 1),
(12, 'AccessDeniedAlreadyExited', 'TktAlreadyExited.jpg', 'Ticket Already Exited', '', '', '', '', '', '', '', '', 1),
(13, 'EntryTicketBlacklisted', 'TicketBlacklisted.gif', 'Ticket Blacklisted', '', '', '', '', '', '', '', '', 1),
(14, 'NoVehiclePresent', 'NoVehiclePresent.gif', 'No Vehicle Present', '', '', '', '', '', '', '', '', 1),
(15, 'AccessDeniedTagUnknown', 'InvalidTag.gif', 'Tag Unknown', '', '', '', '', '', '', '', '', 1),
(16, 'AccessDeniedWrongParkingZone', 'InvalidTicket.gif', 'Incorrect Parking Zone', '', '', '', '', '', '', '', '', 1),
(17, 'AccessDeniedCardExpired', 'AccessDenied.gif', 'Access Card Expired', '', '', '', '', '', '', '', '', 1),
(18, 'AccessDeniedAntipassbackEntryExist', 'InvalidTag.gif', 'AntiPassBack Error-Entry exist', '', '', '', '', '', '', '', '', 1),
(19, 'AccessDeniedTagDisabled', 'InvalidTag.gif', 'Tag disabled', '', '', '', '', '', '', '', '', 1),
(20, 'OpenBarrierFromServer', 'AccessAllowed.gif', 'Open Barrier From Server', '', '', '', '', '', '', '', '', 1),
(21, 'CloseBarrierFromServer', 'Barrier-Close.gif', 'Close Barrier From Server', '', '', '', '', '', '', '', '', 1),
(22, 'EntryBarrierOpened', 'AccessAllowed.gif', 'Barrier already Opened,Wait', '', '', '', '', '', '', '', '', 1),
(23, 'ExitPresenceLoopActivatedChipcoin', 'ExitScanChipcoin.gif', 'Please insert chipcoin', '', '', '', '', '', '', '', '', 1),
(24, 'ExitStandBy', 'TerminalBackgroundExit.jpg', 'Demo Carpark ', '', '', '', '', '', '', '', '', 1),
(28, 'AccessDeniedReservationNotInTimePeriod', 'GracePeriodExceeded.jpg', 'Not In Reservation Time Period', '', '', '', '', '', '', '', '', 1),
(29, 'BarrierAlreadyOpened', 'AccessAllowed.gif', 'Barrier already opened', 'Please wait to close the barrier', '', '', '', '', '', '', '', 1),
(30, 'NoChipCoin', 'InvalidChipcoin.png', 'No chipcoin available on machine', '', '', '', '', '', '', '', '', 1),
(31, 'OutOfService', 'OutOfService.gif', 'Out Of Service', '', '', '', '', '', '', '', '', 1),
(32, 'SetProtocolFailed', 'InvalidChipcoin.png', 'Cant connect to rfid', 'Please check the connection', '', '', '', '', '', '', '', 1),
(33, 'AccessDeniedReservationNotUsed', 'AccessDenied.gif', 'Not Used Reservation', '', '', '', '', '', '', '', '', 1),
(34, 'AccessDeniedReservationUsed', 'AccessDenied.gif', 'Used Reservation', '', '', '', '', '', '', '', '', 1),
(35, 'AccessDeniedRegisteredAnotherCarpark', 'InvalidTicket.gif', 'Not Registered to use this carpark', '', '', '', '', '', '', '', '', 1),
(36, 'AccessDeniedRegistrationExpired', 'AccessDenied.gif', 'Parking Access  expired', '', '', '', '', '', '', '', '', 1),
(37, 'ProcessingNumberPlate', 'Processing.gif', 'Processing number plate', 'Please Wait', '', '', '', '', '', '', '', 1),
(38, 'AccessDeniedReservation', 'AccessDenied.gif', 'Not in reservation time slot', '', '', '', '', '', '', '', '', 1),
(39, 'NoWaletBalance', 'UnpaidTicket.gif', 'No balance in walet', '', '', '', '', '', '', '', '', 1),
(40, 'AccessDeniedReservationExitGrace', 'GracePeriodExceeded.jpg', 'Reservation Exit Grace period exceeded', '', '', '', '', '', '', '', '', 1),
(41, 'AccessDeniedReservationAlreadyExited', 'AccessDenied.gif', 'Reservation Ticket Already Exited', '', '', '', '', '', '', '', '', 1),
(42, 'NoReservation', 'TerminalBackgroundEntry.jpg', 'No Reservation', '', '', '', '', '', '', '', '', 1),
(43, 'OutOfTickets', 'OutOfService.gif', 'Machine out of Tickets', '', '', '', '', '', '', '', '', 1),
(44, 'PrintTicketFailed', 'OutOfService.gif', 'Ticket printing failed', '', '', '', '', '', '', '', '', 1),
(45, 'PrintFailedWrongPassword', 'OutOfService.gif', 'Print failed wrong password', '', '', '', '', '', '', '', '', 1),
(46, 'EntryPresenceLoopActivatedTicket', 'EntryPushButton.gif', 'PLEASE SCAN QR CODE', '', '', 'NA', 'NA', 'NA', '', '', '', 1),
(47, 'ExitPresenceLoopActivatedTicket', 'ExitScanTicket.gif', 'Please Scan ticket', '', '', '', '', '', '', '', '', 1),
(48, 'TakeEntryChipcoin', 'EntryTakeChipcoin.gif', 'Please take the chipcoin', '', '', 'NA', 'NA', 'NA', '', '', '', 1),
(49, 'EntryChipcoinBlacklisted', 'InvalidChipcoin.png', 'Chipcoin Blacklisted', '', '', '', '', '', '', '', '', 1),
(50, 'InvalidEntry', 'TerminalBackgroundEntry.jpg', 'Invalid Entry', '', '', '', '', '', '', '', '', 1),
(53, 'NoPlate', 'TerminalBackgroundEntry.jpg', 'No plate captured', '', '', '', '', '', '', '', '', 1),
(54, 'RFIDError', 'InvalidChipcoin.png', 'Chip coin reading not working', '', '', '', '', '', '', '', '', 1),
(55, 'RestartTerminal', 'TerminalBackgroundEntry.jpg', 'Restarting Terminal ', '', '', 'NA', 'NA', 'NA', '', '', '', 1),
(56, 'FreePassage', 'TerminalBackgroundEntry.jpg', 'Free Passage Mode', '', '', 'NA', 'NA', 'NA', '', '', '', 1),
(57, 'LaneClosed', 'TerminalBackgroundEntry.jpg', 'Lane Temporarily Closed', '', '', 'NA', 'NA', 'NA', '', '', '', 1),
(58, 'CarparkFull', 'TerminalBackgroundEntry.jpg', 'Carpark FULL', 'Entry Temporarily Blocked', '', 'NA', 'NA', 'NA', '', '', '', 1),
(59, 'FreePassageEntry', 'TerminalBackgroundEntry.jpg', 'Free Passage Entry', '', '', 'NA', 'NA', 'NA', '', '', '', 1),
(60, 'OutOfServiceTicket', 'OutOfService.gif', 'Out Of Service', 'Machine out of tickets', '', '', '', '', '', '', '', 1),
(62, 'ExternalOpen ', 'AccessAllowed.gif', 'Manual Open ', 'Please Enter ', '', '', '', '', '', '', '', 1),
(63, 'TicketBoxError', 'TerminalBackgroundEntry.jpg', 'Ticket Box Error', 'Unable to process entry ticket ', '', '', '', '', '', '', '', 1),
(64, 'WiegandBlocked', 'InvalidTag.gif', 'Access Card Reader ', 'Blocked ', '', '', '', '', '', '', '', 1),
(65, 'LaneOccupied', 'TerminalBackgroundEntry.jpg', 'Lane Occupied ', 'Barrier Loop Active ', '', '', '', '', '', '', '', 1),
(66, 'NoTicketsWiegand', 'TerminalBackgroundEntry.jpg', 'Please use Access Card ', '', '', '', '', '', '', '', '', 1),
(67, 'ExternalInputOpen', 'TerminalBackgroundEntry.jpg', 'External Input Barrier Open', '', '', '', '', '', '', '', '', 1),
(68, 'TicketBoxErrorWiegand', 'TerminalBackgroundEntry.jpg', 'Ticket Box Error', 'Please use Access Card ', '', '', '', '', '', '', '', 1),
(70, 'TicketBlockedWiegand', 'TerminalBackgroundEntry.jpg', 'Ticket Issuing Blocked', 'Please use Access Card ', '', '', '', '', '', '', '', 1),
(71, 'NoTicketBarrierOpenPosition', 'TerminalBackgroundEntry.jpg', 'Cannot Process Ticket', 'Barrier in Open Position', '', '', '', '', '', '', '', 1),
(72, 'WiegandOnly', 'TerminalBackgroundEntry.jpg', 'Out of Tickets', 'Use Access Card', '', 'NA', 'NA', 'NA', '', '', '', 1),
(73, 'BarrierTimeout', 'TerminalBackgroundEntry.jpg', 'Closing Barrier', 'Timeout ', '', 'NA', 'NA', 'NA', '', '', '', 1),
(74, 'TicketOnly', 'ExitScanTicket.gif', 'Push Button for Ticket', 'Access Card Blocked', '', 'NA', 'NA', 'NA', '', '', '', 1),
(75, 'TicketsOnlyNoWiegand', 'ExitScanTicket.gif', 'Push Button for Ticket', 'Access Card Blocked', '', 'NA', 'NA', 'NA', '', '', '', 1),
(76, 'TicketPrinterErrorOnlyWiegand', 'InvalidTag.gif', 'Ticket Printer Error ', 'Please use Access Card ', '', 'NA', 'NA', 'NA', '', '', '', 1),
(77, 'ExitOfflineMode', 'ExitScanTicket.gif', 'Offline Mode ', 'Scan Ticket  Or Receipt QR Code ', 'Or Use Access Card ', 'NA', 'NA', 'NA', '', '', '', 1),
(78, 'EntryPresenceLoopActivated', 'EntryPushButtonChipcoin.gif', 'Presence Loop Activated', '', '', 'NA', 'NA', 'NA', '', '', '', 1),
(79, 'AccessDeniedAccessNotStarted', 'GracePeriodExceeded.jpg', 'Access validity not started', '', '', '', '', '', '', '', '', 1),
(80, 'AccessDeniedAntipassbackAlreadyExited', 'InvalidTag.gif', 'AntiPassBack Error-Already exited', '', '', '', '', '', '', '', '', 1),
(81, 'AccessDeniedAntipassbackEntryNotExist', 'InvalidTag.gif', 'AntiPassBack Error-Entry not exist', '', '', '', '', '', '', '', '', 1),
(82, 'DownloadSettings', 'Processing.gif', 'DownLoad settings.', 'Please wait...', '', 'NA', 'NA', 'NA', '', '', '', 1),
(83, 'AccessDeniedAntipassback', 'InvalidTag.gif', 'AntiPassBack Error', '', '', '', '', '', '', '', '', 1),
(84, 'AmountToPay', 'AccessAllowed.gif', 'Amount to pay ', '', '', '', '', '', '', '', '', 1),
(85, 'ParkingDuration', 'AccessAllowed.gif', 'Parking duration', '', '', '', '', '', '', '', '', 1),
(86, 'ApplicationLogo', 'terminal_logo.jpg', '', '', '', '', '', '', '', '', '', 1),
(87, 'ServerOnline', 'server_online.png', '', '', '', '', '', '', '', '', '', 1),
(88, 'ServerOffline', 'server_offline.png', '', '', '', '', '', '', '', '', '', 1),
(89, 'LanOffline', 'lan_offline.png', '', '', '', '', '', '', '', '', '', 1),
(90, 'LanOnline', 'lan_online.png', '', '', '', '', '', '', '', '', '', 1),
(91, 'AccessDeniedServer', 'InvalidTag.gif', 'Access Denied ', 'By Server ', '', '', '', '', '', '', '', 1),
(92, 'AccessDeniedPlateMismatch', 'InvalidTicket.gif', 'Plate mismatch', '', '', '', '', '', '', '', '', 1),
(93, 'AccessDeniedDisabledCoupon', 'InvalidTicket.gif', 'Disabled coupon', '', '', '', '', '', '', '', '', 1),
(94, 'AccessDeniedUsedCoupon', 'InvalidTicket.gif', 'Already used coupon', '', '', '', '', '', '', '', '', 1),
(95, 'AccessDeniedCouponInvalidCarpark', 'InvalidTicket.gif', 'Coupon from another carpark', '', '', '', '', '', '', '', '', 1),
(96, 'ValidCoupon', 'InvalidTicket.gif', 'Valid Coupon.Please scan entry ticket', '', '', '', '', '', '', '', '', 1),
(97, 'AccessDeniedCouponInvalidCoupon', 'InvalidTicket.gif', 'Invalid coupon', '', '', '', '', '', '', '', '', 1),
(98, 'ScanCoupon', '', 'Or Scan coupon', '', '', '', '', '', '', '', '', 1),
(99, 'CouponApplied', '', 'Coupon Applied', '', '', '', '', '', '', '', '', 1),
(100, 'ExitOfflineTicket', 'ExitScanTicket.gif', 'Please Scan ticket', '', '', '', '', '', '', '', '', 1),
(101, 'ExitOfflineTicketWeigand', 'ExitScanTicket.gif', 'Please scan tag or ticket', '', '', '', '', '', '', '', '', 1);

-- --------------------------------------------------------

--
-- Table structure for table `customer_messages2`
--

CREATE TABLE `customer_messages2` (
  `id` int(11) NOT NULL,
  `message` varchar(100) NOT NULL,
  `media_path` text NOT NULL,
  `english_line1` varchar(100) NOT NULL DEFAULT 'NA',
  `english_line2` varchar(50) NOT NULL DEFAULT 'NA',
  `english_line3` varchar(50) NOT NULL DEFAULT 'NA',
  `arabic_line1` varchar(100) NOT NULL DEFAULT 'NA',
  `arabic_line2` varchar(100) NOT NULL DEFAULT 'NA',
  `arabic_line3` varchar(100) NOT NULL DEFAULT 'NA',
  `spanish_line1` varchar(100) DEFAULT 'NULL',
  `spanish_line2` varchar(100) DEFAULT 'NULL',
  `spanish_line3` varchar(100) DEFAULT 'NULL',
  `status` tinyint(4) NOT NULL DEFAULT 1
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `customer_messages2`
--

INSERT INTO `customer_messages2` (`id`, `message`, `media_path`, `english_line1`, `english_line2`, `english_line3`, `arabic_line1`, `arabic_line2`, `arabic_line3`, `spanish_line1`, `spanish_line2`, `spanish_line3`, `status`) VALUES
(1, 'EntryPresenceLoopActivatedChipcoin', 'EntryPushButtonChipcoin.gif', 'Push button for chipcoin', '', '', '', '', '', '', '', '', 1),
(2, 'TakeEntryTicket', 'EntryTakeTicket.gif', 'Please take the ticket', '', '', '', '', '', '', '', '', 1),
(3, 'Processing', 'Processing.gif', 'Processing..', 'Please Wait', '', '', '', '', '', '', '', 1),
(4, 'EntryStandBy', 'TerminalBackgroundEntry.jpg', 'CARPARK NAME', '', '', '', '', '', '', '', '', 1),
(5, 'AccessAllowedEntry', 'AccessAllowed.gif', 'Welcome', '', '', '', '', '', '', '', '', 1),
(6, 'AccessDeniedUnpaidTicket', 'UnpaidTicket.gif', 'Unpaid Parking Fee', 'Pay at Payment Machine ', 'Or Scan coupons', '', '', '', '', '', '', 1),
(7, 'AccessAllowedExit', 'ThankYou.gif', 'Thank You for visiting', '', '', '', '', '', '', '', '', 1),
(8, 'AccessDeniedInvalidTicket', 'InvalidTicket.gif', 'Invalid Ticket', '', '', '', '', '', '', '', '', 1),
(9, 'AccessDeniedWrongCarpark', 'InvalidTicket.gif', 'Incorrect Car Park ', '', '', '', '', '', '', '', '', 1),
(10, 'AccessDeniedWrongFacility', 'InvalidTicket.gif', 'Incorrect Car Park ', '', '', '', '', '', '', '', '', 1),
(11, 'AccessDeniedExitGrace', 'GracePeriodExceeded.jpg', 'Exit Grace period exceeded', '', '', '', '', '', '', '', '', 1),
(12, 'AccessDeniedAlreadyExited', 'TktAlreadyExited.jpg', 'Ticket Already Exited', '', '', '', '', '', '', '', '', 1),
(13, 'EntryTicketBlacklisted', 'TicketBlacklisted.gif', 'Ticket Blacklisted', '', '', '', '', '', '', '', '', 1),
(14, 'NoVehiclePresent', 'NoVehiclePresent.gif', 'No Vehicle Present', '', '', '', '', '', '', '', '', 1),
(15, 'AccessDeniedTagUnknown', 'InvalidTag.gif', 'Tag Unknown', '', '', '', '', '', '', '', '', 1),
(16, 'AccessDeniedWrongParkingZone', 'InvalidTicket.gif', 'Incorrect Parking Zone', '', '', '', '', '', '', '', '', 1),
(17, 'AccessDeniedCardExpired', 'AccessDenied.gif', 'Access Card Expired', '', '', '', '', '', '', '', '', 1),
(18, 'AccessDeniedAntipassbackEntryExist', 'InvalidTag.gif', 'AntiPassBack Error-Entry exist', '', '', '', '', '', '', '', '', 1),
(19, 'AccessDeniedTagDisabled', 'InvalidTag.gif', 'Tag disabled', '', '', '', '', '', '', '', '', 1),
(20, 'OpenBarrierFromServer', 'AccessAllowed.gif', 'Open Barrier From Server', '', '', '', '', '', '', '', '', 1),
(21, 'CloseBarrierFromServer', 'Barrier-Close.gif', 'Close Barrier From Server', '', '', '', '', '', '', '', '', 1),
(22, 'EntryBarrierOpened', 'AccessAllowed.gif', 'Barrier already Opened,Wait', '', '', '', '', '', '', '', '', 1),
(23, 'ExitPresenceLoopActivatedChipcoin', 'ExitScanChipcoin.gif', 'Please insert chipcoin', '', '', '', '', '', '', '', '', 1),
(24, 'ExitStandBy', 'TerminalBackgroundExit.jpg', 'Demo Carpark ', '', '', '', '', '', '', '', '', 1),
(28, 'AccessDeniedReservationNotInTimePeriod', 'GracePeriodExceeded.jpg', 'Not In Reservation Time Period', '', '', '', '', '', '', '', '', 1),
(29, 'BarrierAlreadyOpened', 'AccessAllowed.gif', 'Barrier already opened', 'Please wait to close the barrier', '', '', '', '', '', '', '', 1),
(30, 'NoChipCoin', 'InvalidChipcoin.png', 'No chipcoin available on machine', '', '', '', '', '', '', '', '', 1),
(31, 'OutOfService', 'OutOfService.gif', 'Out Of Service', '', '', '', '', '', '', '', '', 1),
(32, 'SetProtocolFailed', 'InvalidChipcoin.png', 'Cant connect to rfid', 'Please check the connection', '', '', '', '', '', '', '', 1),
(33, 'AccessDeniedReservationNotUsed', 'AccessDenied.gif', 'Not Used Reservation', '', '', '', '', '', '', '', '', 1),
(34, 'AccessDeniedReservationUsed', 'AccessDenied.gif', 'Used Reservation', '', '', '', '', '', '', '', '', 1),
(35, 'AccessDeniedRegisteredAnotherCarpark', 'InvalidTicket.gif', 'Not Registered to use this carpark', '', '', '', '', '', '', '', '', 1),
(36, 'AccessDeniedRegistrationExpired', 'AccessDenied.gif', 'Parking Access  expired', '', '', '', '', '', '', '', '', 1),
(37, 'ProcessingNumberPlate', 'Processing.gif', 'Processing number plate', 'Please Wait', '', '', '', '', '', '', '', 1),
(38, 'AccessDeniedReservation', 'AccessDenied.gif', 'Not in reservation time slot', '', '', '', '', '', '', '', '', 1),
(39, 'NoWaletBalance', 'UnpaidTicket.gif', 'No balance in walet', '', '', '', '', '', '', '', '', 1),
(40, 'AccessDeniedReservationExitGrace', 'GracePeriodExceeded.jpg', 'Reservation Exit Grace period exceeded', '', '', '', '', '', '', '', '', 1),
(41, 'AccessDeniedReservationAlreadyExited', 'AccessDenied.gif', 'Reservation Ticket Already Exited', '', '', '', '', '', '', '', '', 1),
(42, 'NoReservation', 'TerminalBackgroundEntry.jpg', 'No Reservation', '', '', '', '', '', '', '', '', 1),
(43, 'OutOfTickets', 'OutOfService.gif', 'Machine out of Tickets', '', '', '', '', '', '', '', '', 1),
(44, 'PrintTicketFailed', 'OutOfService.gif', 'Ticket printing failed', '', '', '', '', '', '', '', '', 1),
(45, 'PrintFailedWrongPassword', 'OutOfService.gif', 'Print failed wrong password', '', '', '', '', '', '', '', '', 1),
(46, 'EntryPresenceLoopActivatedTicket', 'EntryPushButton.gif', 'Wave hand over button for ticket', 'TOUCHLESS TICKET REQUEST', '#We Are All Responsible', 'NA', 'NA', 'NA', '', '', '', 1),
(47, 'ExitPresenceLoopActivatedTicket', 'ExitScanTicket.gif', 'Please Scan ticket', '', '', '', '', '', '', '', '', 1),
(48, 'TakeEntryChipcoin', 'EntryTakeChipcoin.gif', 'Please take the chipcoin', '', '', 'NA', 'NA', 'NA', '', '', '', 1),
(49, 'EntryChipcoinBlacklisted', 'InvalidChipcoin.png', 'Chipcoin Blacklisted', '', '', '', '', '', '', '', '', 1),
(50, 'InvalidEntry', 'TerminalBackgroundEntry.jpg', 'Invalid Entry', '', '', '', '', '', '', '', '', 1),
(53, 'NoPlate', 'TerminalBackgroundEntry.jpg', 'No plate captured', '', '', '', '', '', '', '', '', 1),
(54, 'RFIDError', 'InvalidChipcoin.png', 'Chip coin reading not working', '', '', '', '', '', '', '', '', 1),
(55, 'RestartTerminal', 'TerminalBackgroundEntry.jpg', 'Restarting Terminal ', '', '', 'NA', 'NA', 'NA', '', '', '', 1),
(56, 'FreePassage', 'TerminalBackgroundEntry.jpg', 'Free Passage Mode', '', '', 'NA', 'NA', 'NA', '', '', '', 1),
(57, 'LaneClosed', 'TerminalBackgroundEntry.jpg', 'Lane Temporarily Closed', '', '', 'NA', 'NA', 'NA', '', '', '', 1),
(58, 'CarparkFull', 'TerminalBackgroundEntry.jpg', 'Carpark FULL', 'Entry Temporarily Blocked', '', 'NA', 'NA', 'NA', '', '', '', 1),
(59, 'FreePassageEntry', 'TerminalBackgroundEntry.jpg', 'Free Passage Entry', '', '', 'NA', 'NA', 'NA', '', '', '', 1),
(60, 'OutOfServiceTicket', 'OutOfService.gif', 'Out Of Service', 'Machine out of tickets', '', '', '', '', '', '', '', 1),
(62, 'ExternalOpen ', 'AccessAllowed.gif', 'Manual Open ', 'Please Enter ', '', '', '', '', '', '', '', 1),
(63, 'TicketBoxError', 'TerminalBackgroundEntry.jpg', 'Ticket Box Error', 'Unable to process entry ticket ', '', '', '', '', '', '', '', 1),
(64, 'WiegandBlocked', 'InvalidTag.gif', 'Access Card Reader ', 'Blocked ', '', '', '', '', '', '', '', 1),
(65, 'LaneOccupied', 'TerminalBackgroundEntry.jpg', 'Lane Occupied ', 'Barrier Loop Active ', '', '', '', '', '', '', '', 1),
(66, 'NoTicketsWiegand', 'TerminalBackgroundEntry.jpg', 'Please use Access Card ', '', '', '', '', '', '', '', '', 1),
(67, 'ExternalInputOpen', 'TerminalBackgroundEntry.jpg', 'External Input Barrier Open', '', '', '', '', '', '', '', '', 1),
(68, 'TicketBoxErrorWiegand', 'TerminalBackgroundEntry.jpg', 'Ticket Box Error', 'Please use Access Card ', '', '', '', '', '', '', '', 1),
(70, 'TicketBlockedWiegand', 'TerminalBackgroundEntry.jpg', 'Ticket Issuing Blocked', 'Please use Access Card ', '', '', '', '', '', '', '', 1),
(71, 'NoTicketBarrierOpenPosition', 'TerminalBackgroundEntry.jpg', 'Cannot Process Ticket', 'Barrier in Open Position', '', '', '', '', '', '', '', 1),
(72, 'WiegandOnly', 'TerminalBackgroundEntry.jpg', 'Out of Tickets', 'Use Access Card', '', 'NA', 'NA', 'NA', '', '', '', 1),
(73, 'BarrierTimeout', 'TerminalBackgroundEntry.jpg', 'Closing Barrier', 'Timeout ', '', 'NA', 'NA', 'NA', '', '', '', 1),
(74, 'TicketOnly', 'ExitScanTicket.gif', 'Push Button for Ticket', 'Access Card Blocked', '', 'NA', 'NA', 'NA', '', '', '', 1),
(75, 'TicketsOnlyNoWiegand', 'ExitScanTicket.gif', 'Push Button for Ticket', 'Access Card Blocked', '', 'NA', 'NA', 'NA', '', '', '', 1),
(76, 'TicketPrinterErrorOnlyWiegand', 'InvalidTag.gif', 'Ticket Printer Error ', 'Please use Access Card ', '', 'NA', 'NA', 'NA', '', '', '', 1),
(77, 'ExitOfflineMode', 'ExitScanTicket.gif', 'Offline Mode ', 'Scan Ticket  Or Receipt QR Code ', 'Or Use Access Card ', 'NA', 'NA', 'NA', '', '', '', 1),
(78, 'EntryPresenceLoopActivated', 'EntryPushButtonChipcoin.gif', 'Presence Loop Activated', '', '', 'NA', 'NA', 'NA', '', '', '', 1),
(79, 'AccessDeniedAccessNotStarted', 'GracePeriodExceeded.jpg', 'Access validity not started', '', '', '', '', '', '', '', '', 1),
(80, 'AccessDeniedAntipassbackAlreadyExited', 'InvalidTag.gif', 'AntiPassBack Error-Already exited', '', '', '', '', '', '', '', '', 1),
(81, 'AccessDeniedAntipassbackEntryNotExist', 'InvalidTag.gif', 'AntiPassBack Error-Entry not exist', '', '', '', '', '', '', '', '', 1),
(82, 'DownloadSettings', 'Processing.gif', 'DownLoad settings.', 'Please wait...', '', 'NA', 'NA', 'NA', '', '', '', 1),
(83, 'AccessDeniedAntipassback', 'InvalidTag.gif', 'AntiPassBack Error', '', '', '', '', '', '', '', '', 1),
(84, 'AmountToPay', 'AccessAllowed.gif', 'Amount to pay ', '', '', '', '', '', '', '', '', 1),
(85, 'ParkingDuration', 'AccessAllowed.gif', 'Parking duration', '', '', '', '', '', '', '', '', 1),
(86, 'ApplicationLogo', 'terminal_logo.jpg', '', '', '', '', '', '', '', '', '', 1),
(87, 'ServerOnline', 'server_online.png', '', '', '', '', '', '', '', '', '', 1),
(88, 'ServerOffline', 'server_offline.png', '', '', '', '', '', '', '', '', '', 1),
(89, 'LanOffline', 'lan_offline.png', '', '', '', '', '', '', '', '', '', 1),
(90, 'LanOnline', 'lan_online.png', '', '', '', '', '', '', '', '', '', 1),
(91, 'AccessDeniedServer', 'InvalidTag.gif', 'Access Denied ', 'By Server ', '', '', '', '', '', '', '', 1),
(92, 'AccessDeniedPlateMismatch', 'InvalidTicket.gif', 'Plate mismatch', '', '', '', '', '', '', '', '', 1),
(93, 'AccessDeniedDisabledCoupon', 'InvalidTicket.gif', 'Disabled coupon', '', '', '', '', '', '', '', '', 1),
(94, 'AccessDeniedUsedCoupon', 'InvalidTicket.gif', 'Already used coupon', '', '', '', '', '', '', '', '', 1),
(95, 'AccessDeniedCouponInvalidCarpark', 'InvalidTicket.gif', 'Coupon from another carpark', '', '', '', '', '', '', '', '', 1),
(96, 'ValidCoupon', 'InvalidTicket.gif', 'Valid Coupon.Please scan entry ticket', '', '', '', '', '', '', '', '', 1),
(97, 'AccessDeniedCouponInvalidCoupon', 'InvalidTicket.gif', 'Invalid coupon', '', '', '', '', '', '', '', '', 1),
(98, 'ScanCoupon', '', 'Or Scan coupon', '', '', '', '', '', '', '', '', 1),
(99, 'CouponApplied', '', 'Coupon Applied', '', '', '', '', '', '', '', '', 1),
(100, 'ExitOfflineTicket', 'ExitScanTicket.gif', 'Please Scan ticket', '', '', '', '', '', '', '', '', 1),
(101, 'ExitOfflineTicketWeigand', 'ExitScanTicket.gif', 'Please scan tag or ticket', '', '', '', '', '', '', '', '', 1);

-- --------------------------------------------------------

--
-- Table structure for table `daemon_status`
--

CREATE TABLE `daemon_status` (
  `id` int(11) NOT NULL,
  `daemon_name` varchar(50) NOT NULL,
  `path` varchar(50) NOT NULL,
  `daemon_label` varchar(50) NOT NULL,
  `file_type` varchar(50) DEFAULT NULL,
  `status` int(11) NOT NULL DEFAULT 1,
  `pid` int(11) NOT NULL DEFAULT 0,
  `start_time` datetime DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `daemon_status`
--

INSERT INTO `daemon_status` (`id`, `daemon_name`, `path`, `daemon_label`, `file_type`, `status`, `pid`, `start_time`) VALUES
(1, 'mysqld', '', 'Mysql server', NULL, 1, 0, NULL),
(2, 'PX_UploadToServer_20210428', '/home/pi//parcx/Daemons', 'Upload to server', 'daemon', 1, 0, NULL),
(3, 'PX_Access_Wiegand_controller_20201125', '/home/pi//parcx/Daemons', 'Wiegand', 'daemon', 1, 0, NULL);

-- --------------------------------------------------------

--
-- Table structure for table `device_maintenance_terminal`
--

CREATE TABLE `device_maintenance_terminal` (
  `id` int(11) NOT NULL,
  `setting_name` varchar(50) NOT NULL,
  `setting_value` varchar(100) NOT NULL,
  `setting_label` varchar(50) DEFAULT NULL,
  `description` varchar(100) NOT NULL,
  `enabled` int(11) NOT NULL DEFAULT 1
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

--
-- Dumping data for table `device_maintenance_terminal`
--

INSERT INTO `device_maintenance_terminal` (`id`, `setting_name`, `setting_value`, `setting_label`, `description`, `enabled`) VALUES
(1, 'db_list_for_backup', 'ParcxTerminal', 'DB Backup List', 'comma separated values ', 1),
(2, 'db_backup_interval', 'day_closure', 'DB Backup Interval', 'day_closure', 1),
(3, 'db_backup_retention_period', '0', 'DB backup retention period', 'number of days', 1),
(4, 'db_backup_path_primary', '/opt/parcx/DBBackup', 'DB backup path primary', 'local machine', 1),
(6, 'db_for_transaction_deletion', 'ParcxTerminal', 'DB for transaction deletion', 'name of the database', 1),
(7, 'transaction_retention_raw', '5', 'Transaction retention raw', 'this is for tables where we have all the individual transactions and are not summarized data or sett', 1),
(8, 'transaction_retention_summary', '5', 'Transaction retention summary', 'this is for the summarized data tables like revenue summary , traffic summary etc.', 1),
(9, 'log_file_backup_path_primary', '/opt/parcx/LogBackup', 'log file backup path primary', 'this is on the local machine', 1),
(11, 'log_file_retention_period', '20', 'log file retention period', 'applicable only for the files in the Logs folder', 1),
(12, 'open_transaction_retention', '10', 'open transaction retention', '', 1),
(16, 'list_tables_for_transaction_deletion_raw', 'parking_movements,parking_movements_access', 'list tables for transaction deletion raw', 'list of tables raw', 1),
(17, 'list_tables_for_transaction_deletion_summary', '', 'list tables for transaction deletion summary', 'list of tables summary', 1),
(18, 'actual_log_path', '/opt/parcx', 'actual log path', 'Log folder path minus the Log flder', 1),
(19, 'anpr_image_path', '/opt/parcx/Logs/ANPR/Images', 'anpr image path', 'ANPR Image Folder', 1),
(20, 'anpr_retention', '30', 'anpr retention', 'Delete anpr images after retention days', 1),
(21, 'dbbackup_enabled', '1', 'dbbackup enabled', '1 for enable,0 for disabled', 1),
(22, 'logbackup_enabled', '1', 'logbackup enabled', '1 to enable,0 to disable', 1),
(23, 'deleteOpenTransactions_enabled', '0', 'deleteOpenTransactions enabled', '1 to enable,0 to disable', 0),
(24, 'deleteOldDBBackup_enabled', '0', 'deleteOldDBBackup enabled', '1 to enable,0 to disable', 0),
(25, 'deleteOldLogBackup_enabled', '0', 'deleteOldLogBackup enabled', '1 to enable,0 to disable', 0),
(26, 'deleteOldTransactions_enabled', '1', 'deleteOldTransactions enabled', '1 to enable,0 to disable', 0),
(27, 'deleteOldANPRImages_enabled', '0', 'deleteOldANPRImages enabled', '1 to enable,0 to disable', 0),
(28, 'log_backup_retention_period', '30', 'log backup retention period', 'applicable only for the local folder', 1),
(29, 'deleteOldLogFile_enabled', '1', 'deleteOldLogFile enabled', '1 to enable,0 to disable', 1);

-- --------------------------------------------------------

--
-- Table structure for table `device_settings`
--

CREATE TABLE `device_settings` (
  `id` int(11) NOT NULL,
  `setting_name` varchar(100) NOT NULL DEFAULT '0',
  `setting_value` varchar(100) NOT NULL DEFAULT '0',
  `setting_label` varchar(100) DEFAULT 'NULL',
  `description` text NOT NULL,
  `setting_status` int(1) NOT NULL DEFAULT 1,
  `setting_type` varchar(50) DEFAULT NULL,
  `setting_options` text DEFAULT NULL,
  `display_order` int(11) NOT NULL DEFAULT 1 COMMENT '0 - To hide the setting',
  `settings_control` int(11) NOT NULL DEFAULT 2 COMMENT '0-not editable,1-editable,2-disable	'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `device_settings`
--

INSERT INTO `device_settings` (`id`, `setting_name`, `setting_value`, `setting_label`, `description`, `setting_status`, `setting_type`, `setting_options`, `display_order`, `settings_control`) VALUES
(1, 'facility_name', 'Demo facility', 'Facility name', '', 1, '', '', 1, 2),
(2, 'facility_number', '100001', 'Facility number', '', 1, '', '', 1, 2),
(3, 'carpark_name', 'P1', 'Carpark name', '', 1, '', '', 1, 2),
(4, 'carpark_number', '1', 'Carpark number', '', 1, '', '', 1, 2),
(5, 'device_name', 'Entry', 'Device name', '', 1, '', '', 1, 2),
(6, 'device_number', '11', 'Device number', '', 1, '', '', 1, 2),
(7, 'device_type', '1', 'Device type', '1-Entry 2-Exit', 1, 'dropdown', '1:Entry,2:Exit,3:Cashier,4:APM', 1, 2),
(8, 'movement_type', '1', 'Movement type', '', 1, 'dropdown', '1:Entry,2:Exit', 1, 2),
(9, 'parking_zone', '', 'Parking zone', '', 1, '', '', 1, 2),
(10, 'server_ip', '10.195.14.213', 'Server IP', '', 1, '', '', 1, 2),
(11, 'rfid_port', '/dev/ttyUSB0', 'RFID port', '', 1, '', '', 1, 2),
(12, 'primary_language', 'english', 'Primary language', '', 1, 'dropdown', 'english:English,arabic:Arabic', 1, 2),
(13, 'secondary_language', 'arabic', 'Secondary language', '', 0, 'dropdown', 'english:English,arabic:Arabic', 1, 2),
(14, 'device_function', '1', 'Device function', '1=QrCode, 2=ChipCOin , 3 = Ticketless', 1, 'dropdown', '1:QR Code,2:Chipcoin,3:TicketLess', 1, 2),
(15, 'mode_of_operation', '0', 'Mode of operation', '0= normal \r\n1=freepassage\r\n2=lane closed ', 1, 'dropdown', '0:Normal,1:Free passage,2:Lane Enabled', 1, 2),
(16, 'reservation_enabled', '1', 'Reservation Enabled', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(17, 'reservation_entry_grace_period', '0', 'Reservation entry grace period', 'in minutes', 1, '', '', 1, 2),
(18, 'reservation_exit_grace_period', '0', 'Reservation exit grace period', 'in minutes', 1, '', '', 1, 2),
(19, 'review_mode', '0', 'Review mode', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(20, 'output_pulse_length', '150', 'Output pulse length', '', 1, '', '', 1, 2),
(21, 'camera_id', '0', 'Camera id', '', 1, '', '', 1, 2),
(22, 'validation_enabled', '1', 'Validation enabled', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(23, 'anpr_enabled', '0', 'ANPR enabled', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(24, 'access_enabled', '1', 'access enabled', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(26, 'entry_grace_period', '5', 'Entry grace period', 'in minutes', 1, '', '', 1, 2),
(27, 'exit_grace_period', '5', 'Exit grace period', 'in minutes', 1, '', '', 1, 2),
(29, 'barrier_status_monitoring', '0', 'Barrier status monitoring', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(30, 'wiegand_whitelist_check_enabled', '0', 'Wiegand whitelist check enabled', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(31, 'wiegand_whitelist_parking_zone_check_enabled', '0', 'Wiegand shitelist parking zone check enabled', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(32, 'wiegand_whitelist_expiry_check_enabled', '0', 'Wiegand whitelist expiry check enabled', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(33, 'wiegand_whitelist_carpark_check_enabled', '0', 'Wiegand whitelist carpark check enabled', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(34, 'wiegand_whitelist_facility_check_enabled', '0', 'Wiegand whitelist facility check enabled', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(35, 'send_wiegand_response_to_port', '0', 'Send wiegand response to port', '', 1, '', '', 1, 2),
(36, 'personal_message_enabled', '1', 'Personal message enabled', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(37, 'wiegand_enabled', '0', 'Wiegand enabled', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(38, 'wiegand2_enabled', '0', 'Wiegand-2 enabled', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(39, 'wiegand_daemon', '/home/pi/parcx/Daemons/PX_Access_Wiegand_controller_20201125', 'Wiegand daemon', '', 1, '', '', 1, 2),
(40, 'movement_type2', '2', 'Movement type wiegant-2', '', 1, 'dropdown', '0:,1:Entry,2:Exit', 1, 2),
(41, 'blacklist_details_webservice', '/parcx/webservice/getBlacklistDetails.php', 'Blacklist webservice', '', 1, '', '', 1, 2),
(42, 'settings_webservice', '/parcx/webservice/download_settings.php', 'Settings webservice', '', 1, '', '', 1, 2),
(43, 'handshake_webservice', '/parcx/webservice/server_handshake.php', 'Handshake webservice', '', 1, '', '', 1, 2),
(44, 'post_to_server_webservice_primary', '/parcx/webservice/post_to_server.php', 'Post to service web service-primary', '', 1, '', '', 1, 2),
(45, 'post_to_server_webservice_secondary', '', 'Post to service web service-secondary', '', 1, '', '', 1, 2),
(46, 'webservice_vms_count', '/parcx/webservice/getVmsCount.php', 'VMS webservice', '', 1, '', '', 1, 2),
(47, 'ticket_check_webservice', 'http://10.195.14.214/PXCloudTicketCheck/TicketCheck.php', 'Ticket check webservice', '', 1, '', '', 1, 2),
(48, 'antipassback_webservice', '/parcx/webservice/antipassback.php', 'Antipassback webservice', '', 1, '', '', 1, 2),
(50, 'post_to_server_daemon', '/home/pi/parcx/Daemons/PX_UploadToServer_20210428', 'Upload to server daemon', '', 1, '', '', 1, 2),
(51, 'anpr_image_url', '/ANPR/Images/', 'ANPR image url', '', 1, '', '', 1, 2),
(52, 'screensaver_timeout', '30', 'Screen saver timeout', 'seconds', 1, '', '', 1, 2),
(53, 'barrier_open_time_limit', '10', 'Barrier open time limit', 'time limit in seconds for barrier to stay open ', 1, '', '', 1, 2),
(54, 'available_tickets', '497', 'Available tickets', '', 1, '', '', 1, 2),
(55, 'bms_status_enabled', '0', 'BMS status enabled', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(56, 'ticket_count_threshold', '500', 'Ticket count threshold', '', 1, '', '', 1, 2),
(57, 'operation_condition', '1', 'Operation condition', '', 1, '', '', 1, 2),
(58, 'vms_delay', '15', 'VMS delay', '', 1, '', '', 1, 2),
(59, 'upload_to_server_delay', '10', 'Upload to server delay', '', 1, '', '', 1, 2),
(60, 'display_anpr_image', '0', 'Display ANPR image', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(61, 'duration_hold_barrier_open', '100', 'Duration hold barrier open', 'wait to open barrier for ticketless entry in milliseconds', 1, '', '', 1, 2),
(62, 'barrier_open_status_type', '0', 'barrier open status type', '1 -> NO Barrier Open\r\n0- > NC Barrier Open ', 1, 'dropdown', '0:NC Barrier Open,1:NO Barrier open', 1, 2),
(63, 'server_handshake_interval', '90', 'Server handshake interval', 'seconds', 1, '', '', 1, 2),
(64, 'quick_barrier_close', '0', 'Quick barrier close', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(65, 'test_terminal', '0', 'Test Terminal', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(66, 'plate_capturing_wait_delay', '7', 'Plate capturing wait delay', '', 1, '', '', 1, 2),
(67, 'printer_type', '1', 'Printer type', '0-Lp0\r\n1-Star printer', 1, 'dropdown', '0:Lp0,1:Star Printer', 1, 2),
(68, 'payment_enabled_exit', '0', 'Payment Enabled Exit', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(69, 'mobile_qrcode_time_limit', '10', 'mobile qrcode check-minutes', '', 1, '', '', 1, 2),
(156, 'short_term_ticket_enabled', '0', 'short_term_ticket_enabled', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(157, 'send_loop_status_to_port', '0', 'send_loop_status_to_port', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(158, 'coupon_enabled', '0', 'coupon Enabled', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(159, 'currency', 'AED', 'Currency', '', 1, '', '', 1, 2),
(160, 'network_interface', 'eth0', 'Network interface', '', 1, '', '', 1, 2),
(161, 'issue_ticket_carpark_full', '1', 'issue ticket when carpark is full', '', 1, 'dropdown', '0:No,1:Yes', 1, 2),
(162, 'media_path', '/home/pi/parcx/Terminal/Media/', 'Media path', '', 1, '', '', 1, 2),
(163, 'secondary_server_ip', '', 'Secondary server ip', '', 1, '', '', 1, 2),
(164, 'dayclosure_start_time', '00:00:00', 'Dayclosure start time', '', 1, '', '', 1, 2),
(165, 'dayclosure_end_time', '23:59:49', 'Dayclosure end time', '', 1, '', '', 1, 2),
(166, 'cloud_upload_webservice', 'http://10.195.14.214/PXCloudService/FieldServerUpload.php', 'Cloud Upload Service', '', 1, '', '', 1, 2),
(167, 'cloud_operator_id', '43', 'Cloud Operator ID', '', 1, '', '', 1, 2),
(168, 'cloud_download_webservice', 'http://10.195.14.214/PXCloudService/FieldServerDownload.php', 'Cloud Download Service', '', 1, '', '', 1, 2);

-- --------------------------------------------------------

--
-- Table structure for table `device_users`
--

CREATE TABLE `device_users` (
  `user_id` int(11) NOT NULL,
  `operator_name` varchar(50) NOT NULL,
  `username` varchar(25) NOT NULL,
  `password` varchar(100) NOT NULL,
  `language` varchar(250) DEFAULT NULL,
  `validity_from_date` date DEFAULT NULL,
  `validity_to_date` date DEFAULT NULL,
  `account_status` tinyint(1) NOT NULL DEFAULT 1
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `device_users`
--

INSERT INTO `device_users` (`user_id`, `operator_name`, `username`, `password`, `language`, `validity_from_date`, `validity_to_date`, `account_status`) VALUES
(30, 'parcx', 'parcx', '7aa84b18e17a847b3dcc315126acb56d', 'English', '2021-04-01', '2021-07-31', 1);

-- --------------------------------------------------------

--
-- Table structure for table `live_device_alarms`
--

CREATE TABLE `live_device_alarms` (
  `id` int(11) NOT NULL,
  `alarm_date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `alarm_code` int(10) NOT NULL,
  `alarm_severity` int(2) NOT NULL,
  `alarm` varchar(50) NOT NULL DEFAULT 'Alarm',
  `alarm_description` tinytext NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `live_device_alarms`
--

INSERT INTO `live_device_alarms` (`id`, `alarm_date_time`, `alarm_code`, `alarm_severity`, `alarm`, `alarm_description`) VALUES
(70, '2021-07-11 15:45:50', 1231, 7, 'Barrier quick close on timeout', 'Barrier quick close on timeout'),
(71, '2021-07-11 15:45:50', 1121, 7, 'Barrier Closed on timout', 'Barrier open for seconds =11 Limit=10');

-- --------------------------------------------------------

--
-- Table structure for table `parking_blacklist`
--

CREATE TABLE `parking_blacklist` (
  `id` int(11) NOT NULL,
  `date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `ticket_id` varchar(50) NOT NULL,
  `blacklisting_reason` varchar(50) NOT NULL DEFAULT 'BackOut',
  `blacklisting_description` tinytext NOT NULL,
  `plate_number` varchar(22) DEFAULT NULL,
  `chip_utid` varchar(50) DEFAULT NULL,
  `upload_status_server1` int(11) NOT NULL DEFAULT 0,
  `upload_status_server2` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `parking_movements`
--

CREATE TABLE `parking_movements` (
  `id` int(11) NOT NULL,
  `movement_type` tinyint(1) NOT NULL DEFAULT 1,
  `date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `ticket_id` varchar(50) NOT NULL,
  `plate_number` varchar(20) DEFAULT NULL,
  `chip_utid` varchar(50) DEFAULT NULL,
  `upload_status_server1` int(11) NOT NULL DEFAULT 0,
  `upload_status_server2` int(11) NOT NULL DEFAULT 0,
  `plate_captured_id` int(11) NOT NULL,
  `cloud_upload_status` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `parking_movements`
--

INSERT INTO `parking_movements` (`id`, `movement_type`, `date_time`, `ticket_id`, `plate_number`, `chip_utid`, `upload_status_server1`, `upload_status_server2`, `plate_captured_id`, `cloud_upload_status`) VALUES
(905, 1, '2021-07-11 08:48:25', '100001010b005b06895e', 'M 72702 %$$', '', 1, 0, 45424, 1),
(906, 1, '2021-07-11 08:48:37', '100001010b005b068969', 'KUW 5 5436', '', 1, 0, 14375, 1),
(907, 1, '2021-07-11 08:48:49', '100001010b005b068975', 'KUW 7 3694', '', 1, 0, 39214, 1),
(908, 1, '2021-07-11 08:49:00', '100001010b005b068981', 'KUW 13 9501', '', 1, 0, 37642, 1),
(909, 1, '2021-07-11 08:49:12', '100001010b005b06898d', 'KUW 1 6746', '', 1, 0, 9183, 1),
(910, 1, '2021-07-11 08:49:24', '100001010b005b068999', 'KUW 11 4981', '', 1, 0, 11631, 1),
(911, 1, '2021-07-11 08:49:36', '100001010b005b0689a5', 'KUW 12 8633', '', 1, 0, 16956, 1),
(912, 1, '2021-07-11 08:49:48', '100001010b005b0689b1', 'KUW 3 1103', '', 1, 0, 28408, 1),
(913, 1, '2021-07-11 08:50:00', '100001010b005b0689bd', 'KUW 14 4956', '', 1, 0, 52435, 1),
(914, 1, '2021-07-11 08:50:12', '100001010b005b0689c8', 'KUW 4 1058', '', 1, 0, 7730, 1),
(915, 1, '2021-07-11 08:50:24', '100001010b005b0689d4', 'KUW 9 9088', '', 1, 0, 4937, 1),
(916, 1, '2021-07-11 08:50:35', '100001010b005b0689e0', 'KUW 3 9461', '', 1, 0, 18661, 1),
(917, 1, '2021-07-11 08:50:47', '100001010b005b0689ec', 'KUW 10 8011', '', 1, 0, 44344, 1),
(918, 1, '2021-07-11 08:50:59', '100001010b005b0689f8', 'KUW 13 4385', '', 1, 0, 28283, 1),
(919, 1, '2021-07-11 08:51:11', '100001010b005b068a04', 'KUW 11 5972', '', 1, 0, 9521, 1),
(920, 1, '2021-07-11 08:51:23', '100001010b005b068a10', 'KUW 5 7468', '', 1, 0, 49785, 1),
(921, 1, '2021-07-11 08:51:35', '100001010b005b068a1c', 'KUW 6 9417', '', 1, 0, 49743, 1),
(922, 1, '2021-07-11 08:51:47', '100001010b005b068a27', 'KUW 5 4828', '', 1, 0, 21039, 1),
(923, 1, '2021-07-11 08:51:59', '100001010b005b068a33', 'KUW 9 3233', '', 1, 0, 22809, 1),
(924, 1, '2021-07-11 08:52:10', '100001010b005b068a3f', 'KUW 7 2845', '', 1, 0, 27257, 1),
(925, 1, '2021-07-11 08:52:22', '100001010b005b068a4b', 'KUW 9 1659', '', 1, 0, 44302, 1),
(926, 1, '2021-07-11 08:52:34', '100001010b005b068a57', 'KUW 14 5835', '', 1, 0, 48922, 1),
(927, 1, '2021-07-11 08:52:46', '100001010b005b068a63', 'KUW 1 1475', '', 1, 0, 7910, 1),
(928, 1, '2021-07-11 08:52:58', '100001010b005b068a6e', 'KUW 15 109', '', 1, 0, 45051, 1),
(929, 1, '2021-07-11 08:53:10', '100001010b005b068a7a', 'KUW 7 3159', '', 1, 0, 40097, 1),
(930, 1, '2021-07-11 08:53:21', '100001010b005b068a86', 'KUW 11 9927', '', 1, 0, 6299, 1),
(931, 1, '2021-07-11 08:53:33', '100001010b005b068a92', 'KUW 3 3740', '', 1, 0, 35632, 1),
(932, 1, '2021-07-11 08:53:45', '100001010b005b068a9e', 'KUW 15 8036', '', 1, 0, 13770, 1),
(933, 1, '2021-07-11 08:53:57', '100001010b005b068aaa', 'KUW 6 6745', '', 1, 0, 17806, 1),
(934, 1, '2021-07-11 08:54:09', '100001010b005b068ab6', 'KUW 5 6156', '', 1, 0, 19466, 1),
(935, 1, '2021-07-11 08:54:21', '100001010b005b068ac1', 'KUW 11 7708', '', 1, 0, 36762, 1),
(936, 1, '2021-07-11 08:54:32', '100001010b005b068acd', 'KUW 12 7734', '', 1, 0, 31416, 1),
(937, 1, '2021-07-11 08:54:44', '100001010b005b068ad9', 'KUW 12 2473', '', 1, 0, 24243, 1),
(938, 1, '2021-07-11 08:54:56', '100001010b005b068ae5', 'KUW 2 8855', '', 1, 0, 28629, 1),
(939, 1, '2021-07-11 08:55:08', '100001010b005b068af1', 'KUW 7 5174', '', 1, 0, 25531, 1),
(940, 1, '2021-07-11 08:55:20', '100001010b005b068afd', 'KUW 12 3958', '', 1, 0, 12282, 1),
(941, 1, '2021-07-11 08:55:32', '100001010b005b068b08', 'KUW 4 9553', '', 1, 0, 17361, 1),
(942, 1, '2021-07-11 08:55:44', '100001010b005b068b14', 'KUW 4 6858', '', 1, 0, 31985, 1),
(943, 1, '2021-07-11 08:55:55', '100001010b005b068b20', 'KUW 1 9036', '', 1, 0, 1666, 1),
(944, 1, '2021-07-11 08:56:07', '100001010b005b068b2c', 'KUW 13 6468', '', 1, 0, 52141, 1),
(945, 1, '2021-07-11 08:56:19', '100001010b005b068b38', 'KUW 13 461', '', 1, 0, 17110, 1),
(946, 1, '2021-07-11 08:56:31', '100001010b005b068b44', 'KUW 14 6384', '', 1, 0, 30398, 1),
(947, 1, '2021-07-11 08:56:43', '100001010b005b068b4f', 'KUW 11 5608', '', 1, 0, 9012, 1),
(948, 1, '2021-07-11 08:56:55', '100001010b005b068b5b', 'KUW 7 5991', '', 1, 0, 16190, 1),
(949, 1, '2021-07-11 08:57:06', '100001010b005b068b67', 'KUW 10 3080', '', 1, 0, 39009, 1),
(950, 1, '2021-07-11 08:57:18', '100001010b005b068b73', 'KUW 6 4491', '', 1, 0, 44240, 1),
(951, 1, '2021-07-11 08:57:30', '100001010b005b068b7f', 'KUW 7 7559', '', 1, 0, 22896, 1),
(952, 1, '2021-07-11 08:57:42', '100001010b005b068b8b', 'KUW 13 721', '', 1, 0, 39496, 1),
(953, 1, '2021-07-11 08:57:54', '100001010b005b068b97', 'KUW 3 8627', '', 1, 0, 27723, 1),
(954, 1, '2021-07-11 08:58:06', '100001010b005b068ba2', 'KUW 10 579', '', 1, 0, 49222, 1),
(955, 1, '2021-07-11 08:58:18', '100001010b005b068bae', 'KUW 13 6418', '', 1, 0, 32101, 1),
(956, 1, '2021-07-11 08:58:29', '100001010b005b068bba', 'KUW 15 4371', '', 1, 0, 22731, 1),
(957, 1, '2021-07-11 08:58:41', '100001010b005b068bc6', 'KUW 2 170', '', 1, 0, 45559, 1),
(958, 1, '2021-07-11 08:58:53', '100001010b005b068bd2', 'KUW 11 5753', '', 1, 0, 51005, 1),
(959, 1, '2021-07-11 08:59:05', '100001010b005b068bde', 'KUW 6 5336', '', 1, 0, 52571, 1),
(960, 1, '2021-07-11 08:59:17', '100001010b005b068bea', 'KUW 1 4451', '', 1, 0, 6723, 1),
(961, 1, '2021-07-11 08:59:29', '100001010b005b068bf5', 'KUW 5 163', '', 1, 0, 16482, 1),
(962, 1, '2021-07-11 08:59:41', '100001010b005b068c01', 'KUW 3 8283', '', 1, 0, 37981, 1),
(963, 1, '2021-07-11 08:59:53', '100001010b005b068c0d', 'KUW 4 1852', '', 1, 0, 19991, 1),
(964, 1, '2021-07-11 09:00:05', '100001010b005b068c19', 'KUW 12 4130', '', 1, 0, 48364, 1),
(965, 1, '2021-07-11 09:00:16', '100001010b005b068c25', 'KUW 6 3454', '', 1, 0, 30623, 1),
(966, 1, '2021-07-11 09:00:28', '100001010b005b068c31', 'KUW 10 887', '', 1, 0, 42011, 1),
(967, 1, '2021-07-11 09:00:40', '100001010b005b068c3d', 'KUW 8 2951', '', 1, 0, 42331, 1),
(968, 1, '2021-07-11 09:00:52', '100001010b005b068c49', 'KUW 12 5202', '', 1, 0, 32572, 1),
(969, 1, '2021-07-11 09:01:04', '100001010b005b068c55', 'KUW 10 505', '', 1, 0, 31482, 1),
(970, 1, '2021-07-11 09:01:16', '100001010b005b068c60', 'KUW 1 9900', '', 1, 0, 15682, 1),
(971, 1, '2021-07-11 09:01:28', '100001010b005b068c6c', 'KUW 9 9471', '', 1, 0, 19361, 1),
(972, 1, '2021-07-11 09:01:39', '100001010b005b068c78', 'KUW 1 3559', '', 1, 0, 24594, 1),
(973, 1, '2021-07-11 09:01:51', '100001010b005b068c84', 'KUW 1 6506', '', 1, 0, 45910, 1),
(974, 1, '2021-07-11 09:02:03', '100001010b005b068c90', 'KUW 3 2803', '', 1, 0, 27269, 1),
(975, 1, '2021-07-11 09:02:15', '100001010b005b068c9c', 'KUW 13 2685', '', 1, 0, 2226, 1),
(976, 1, '2021-07-11 09:02:27', '100001010b005b068ca7', 'KUW 2 3551', '', 1, 0, 14309, 1),
(977, 1, '2021-07-11 09:02:39', '100001010b005b068cb3', 'KUW 13 1884', '', 1, 0, 2277, 1),
(978, 1, '2021-07-11 09:02:51', '100001010b005b068cbf', 'KUW 1 5790', '', 1, 0, 10117, 1),
(979, 1, '2021-07-11 09:03:02', '100001010b005b068ccb', 'KUW 11 4120', '', 1, 0, 13095, 1),
(980, 1, '2021-07-11 09:03:14', '100001010b005b068cd7', 'KUW 9 9607', '', 1, 0, 13312, 1),
(981, 1, '2021-07-11 09:03:26', '100001010b005b068ce3', 'KUW 9 3259', '', 1, 0, 4294, 1),
(982, 1, '2021-07-11 09:03:38', '100001010b005b068cef', 'KUW 7 7155', '', 1, 0, 40428, 1),
(983, 1, '2021-07-11 09:03:50', '100001010b005b068cfa', 'KUW 8 1955', '', 1, 0, 16810, 1),
(984, 1, '2021-07-11 09:04:02', '100001010b005b068d06', 'KUW 15 789', '', 1, 0, 18482, 1),
(985, 1, '2021-07-11 09:04:13', '100001010b005b068d12', 'KUW 7 1948', '', 1, 0, 34872, 1),
(986, 1, '2021-07-11 09:04:25', '100001010b005b068d1e', 'KUW 14 2197', '', 1, 0, 36042, 1),
(987, 1, '2021-07-11 09:04:37', '100001010b005b068d2a', 'KUW 7 5196', '', 1, 0, 49039, 1),
(988, 1, '2021-07-11 09:04:49', '100001010b005b068d36', 'KUW 3 1276', '', 1, 0, 21707, 1),
(989, 1, '2021-07-11 09:05:01', '100001010b005b068d42', 'KUW 1 7038', '', 1, 0, 40782, 1),
(990, 1, '2021-07-11 09:05:13', '100001010b005b068d4e', 'KUW 4 3478', '', 1, 0, 31758, 1),
(991, 1, '2021-07-11 09:05:25', '100001010b005b068d59', 'KUW 6 6609', '', 1, 0, 25773, 1),
(992, 1, '2021-07-11 09:05:37', '100001010b005b068d65', 'KUW 4 5159', '', 1, 0, 9091, 1),
(993, 1, '2021-07-11 09:05:48', '100001010b005b068d71', 'KUW 14 8662', '', 1, 0, 25831, 1),
(994, 1, '2021-07-11 09:06:00', '100001010b005b068d7d', 'KUW 3 9157', '', 1, 0, 47324, 1),
(995, 1, '2021-07-11 09:06:12', '100001010b005b068d89', 'KUW 3 5705', '', 1, 0, 22558, 1),
(996, 1, '2021-07-11 09:06:24', '100001010b005b068d95', 'KUW 7 8747', '', 1, 0, 11085, 1),
(997, 1, '2021-07-11 09:06:36', '100001010b005b068da0', 'KUW 3 2728', '', 1, 0, 48069, 1),
(998, 1, '2021-07-11 09:06:48', '100001010b005b068dac', 'KUW 15 7508', '', 1, 0, 12115, 1),
(999, 1, '2021-07-11 09:06:59', '100001010b005b068db8', 'KUW 11 2570', '', 1, 0, 8139, 1),
(1000, 1, '2021-07-11 09:07:11', '100001010b005b068dc4', 'KUW 9 6688', '', 1, 0, 32702, 1),
(1001, 1, '2021-07-11 09:07:23', '100001010b005b068dd0', 'KUW 15 1075', '', 1, 0, 36248, 1),
(1002, 1, '2021-07-11 09:07:35', '100001010b005b068ddc', 'KUW 13 5159', '', 1, 0, 1756, 1),
(1003, 1, '2021-07-11 09:07:47', '100001010b005b068de8', 'KUW 1 4523', '', 1, 0, 13699, 1),
(1004, 1, '2021-07-11 09:07:59', '100001010b005b068df3', 'KUW 3 5613', '', 1, 0, 20545, 1),
(1005, 1, '2021-07-11 09:08:11', '100001010b005b068dff', 'KUW 6 9556', '', 1, 0, 23225, 1),
(1006, 1, '2021-07-11 09:08:22', '100001010b005b068e0b', 'KUW 4 7527', '', 1, 0, 23230, 1),
(1007, 1, '2021-07-11 09:08:34', '100001010b005b068e17', 'KUW 7 7280', '', 1, 0, 41090, 1),
(1008, 1, '2021-07-11 09:08:46', '100001010b005b068e23', 'KUW 15 289', '', 1, 0, 3332, 1),
(1009, 1, '2021-07-11 09:08:58', '100001010b005b068e2f', 'KUW 7 5252', '', 1, 0, 42626, 1),
(1010, 1, '2021-07-11 09:09:10', '100001010b005b068e3b', 'KUW 9 5202', '', 1, 0, 36657, 1),
(1011, 1, '2021-07-11 09:09:22', '100001010b005b068e46', 'KUW 4 3738', '', 1, 0, 45917, 1),
(1012, 1, '2021-07-11 09:09:34', '100001010b005b068e52', 'KUW 15 6419', '', 1, 0, 17289, 1),
(1013, 1, '2021-07-11 09:09:45', '100001010b005b068e5e', 'KUW 9 2805', '', 1, 0, 36312, 1),
(1014, 1, '2021-07-11 09:09:57', '100001010b005b068e6a', 'KUW 10 9569', '', 1, 0, 15493, 1),
(1015, 1, '2021-07-11 09:10:09', '100001010b005b068e76', 'KUW 4 7021', '', 1, 0, 47437, 1),
(1016, 1, '2021-07-11 09:10:21', '100001010b005b068e82', 'KUW 14 9709', '', 1, 0, 20739, 1),
(1017, 1, '2021-07-11 09:10:33', '100001010b005b068e8e', 'KUW 11 6710', '', 1, 0, 51512, 1),
(1018, 1, '2021-07-11 09:10:45', '100001010b005b068e9a', 'KUW 4 8576', '', 1, 0, 46053, 1),
(1019, 1, '2021-07-11 09:10:57', '100001010b005b068ea5', 'KUW 14 7189', '', 1, 0, 48000, 1),
(1020, 1, '2021-07-11 09:11:09', '100001010b005b068eb1', 'KUW 14 1142', '', 1, 0, 7941, 1),
(1021, 1, '2021-07-11 09:11:20', '100001010b005b068ebd', 'KUW 5 1371', '', 1, 0, 30536, 1),
(1022, 1, '2021-07-11 09:11:32', '100001010b005b068ec9', 'KUW 3 7405', '', 1, 0, 42817, 1),
(1023, 1, '2021-07-11 09:11:44', '100001010b005b068ed5', 'KUW 14 7733', '', 1, 0, 14831, 1),
(1024, 1, '2021-07-11 09:11:56', '100001010b005b068ee1', 'KUW 14 8156', '', 1, 0, 21776, 1),
(1025, 1, '2021-07-11 09:12:08', '100001010b005b068eed', 'KUW 1 3528', '', 1, 0, 6430, 1),
(1026, 1, '2021-07-11 09:12:20', '100001010b005b068ef8', 'KUW 1 8044', '', 1, 0, 7467, 1),
(1027, 1, '2021-07-11 09:12:31', '100001010b005b068f04', 'KUW 2 7004', '', 1, 0, 29344, 1),
(1028, 1, '2021-07-11 09:12:43', '100001010b005b068f10', 'KUW 3 8147', '', 1, 0, 46392, 1),
(1029, 1, '2021-07-11 09:12:55', '100001010b005b068f1c', 'KUW 4 2382', '', 1, 0, 11912, 1),
(1030, 1, '2021-07-11 09:13:07', '100001010b005b068f28', 'KUW 6 5507', '', 1, 0, 17855, 1),
(1031, 1, '2021-07-11 09:13:19', '100001010b005b068f34', 'KUW 9 9593', '', 1, 0, 38819, 1),
(1032, 1, '2021-07-11 09:13:31', '100001010b005b068f3f', 'KUW 3 9088', '', 1, 0, 50384, 1),
(1033, 1, '2021-07-11 09:13:43', '100001010b005b068f4b', 'KUW 11 9961', '', 1, 0, 12381, 1),
(1034, 1, '2021-07-11 09:13:54', '100001010b005b068f57', 'KUW 9 3461', '', 1, 0, 2167, 1),
(1035, 1, '2021-07-11 09:14:06', '100001010b005b068f63', 'KUW 7 7732', '', 1, 0, 7781, 1),
(1036, 1, '2021-07-11 09:14:18', '100001010b005b068f6f', 'KUW 11 1574', '', 1, 0, 19925, 1),
(1037, 1, '2021-07-11 09:14:30', '100001010b005b068f7b', 'KUW 1 3428', '', 1, 0, 5282, 1),
(1038, 1, '2021-07-11 09:14:42', '100001010b005b068f87', 'KUW 3 181', '', 1, 0, 35917, 1),
(1039, 1, '2021-07-11 09:14:54', '100001010b005b068f93', 'KUW 12 3006', '', 1, 0, 9630, 1),
(1040, 1, '2021-07-11 09:15:06', '100001010b005b068f9e', 'KUW 6 8787', '', 1, 0, 18955, 1),
(1041, 1, '2021-07-11 09:15:18', '100001010b005b068faa', 'KUW 9 2254', '', 1, 0, 37854, 1),
(1042, 1, '2021-07-11 09:15:29', '100001010b005b068fb6', 'KUW 13 4648', '', 1, 0, 17980, 1),
(1043, 1, '2021-07-11 09:15:53', '100001010b005b068fce', 'KUW 5 6007', '', 1, 0, 3092, 1),
(1044, 1, '2021-07-11 09:16:05', '100001010b005b068fda', 'KUW 6 2604', '', 1, 0, 28503, 1),
(1045, 1, '2021-07-11 09:16:17', '100001010b005b068fe6', 'KUW 1 5000', '', 1, 0, 33066, 1),
(1046, 1, '2021-07-11 13:14:34', '100001010b005b06c7be', 'KUW 14 4875', '', 1, 0, 32895, 1),
(1047, 1, '2021-07-11 13:14:53', '100001010b005b06c7d1', 'KUW 7 8458', '', 1, 0, 15656, 1),
(1048, 1, '2021-07-11 13:15:12', '100001010b005b06c7e4', 'KUW 14 6882', '', 1, 0, 23324, 1),
(1049, 1, '2021-07-11 13:15:31', '100001010b005b06c7f7', 'KUW 11 5839', '', 1, 0, 4878, 1),
(1050, 1, '2021-07-11 13:15:50', '100001010b005b06c809', 'KUW 11 5422', '', 1, 0, 35272, 1),
(1051, 1, '2021-07-11 13:16:09', '100001010b005b06c81c', 'KUW 12 2807', '', 1, 0, 20389, 1),
(1052, 1, '2021-07-11 13:16:28', '100001010b005b06c82f', 'KUW 2 422', '', 1, 0, 24047, 1),
(1053, 1, '2021-07-11 13:16:46', '100001010b005b06c842', 'KUW 6 6288', '', 1, 0, 18060, 1),
(1054, 1, '2021-07-11 13:17:05', '100001010b005b06c855', 'KUW 10 9948', '', 1, 0, 41251, 1),
(1055, 1, '2021-07-11 13:17:24', '100001010b005b06c868', 'KUW 8 1419', '', 1, 0, 12010, 1),
(1056, 1, '2021-07-11 13:17:43', '100001010b005b06c87b', 'KUW 10 6751', '', 1, 0, 37891, 1),
(1057, 1, '2021-07-11 13:18:02', '100001010b005b06c88d', 'KUW 11 3793', '', 1, 0, 15118, 1),
(1058, 1, '2021-07-11 13:18:21', '100001010b005b06c8a0', 'KUW 1 3807', '', 1, 0, 31866, 1),
(1059, 1, '2021-07-11 13:18:39', '100001010b005b06c8b3', 'KUW 11 5462', '', 1, 0, 41802, 1),
(1060, 1, '2021-07-11 13:18:58', '100001010b005b06c8c6', 'KUW 4 3037', '', 1, 0, 37057, 1),
(1061, 1, '2021-07-11 13:19:17', '100001010b005b06c8d9', 'KUW 4 668', '', 1, 0, 46046, 1),
(1062, 1, '2021-07-11 13:19:36', '100001010b005b06c8ec', 'KUW 7 1050', '', 1, 0, 24010, 1),
(1063, 1, '2021-07-11 13:19:55', '100001010b005b06c8fe', 'KUW 11 6428', '', 1, 0, 48395, 1),
(1064, 1, '2021-07-11 13:20:14', '100001010b005b06c911', 'KUW 12 4830', '', 1, 0, 6264, 1),
(1065, 1, '2021-07-11 13:20:32', '100001010b005b06c924', 'KUW 15 5815', '', 1, 0, 29250, 1),
(1066, 1, '2021-07-11 13:20:51', '100001010b005b06c937', 'KUW 13 6895', '', 1, 0, 16722, 1),
(1067, 1, '2021-07-11 13:21:10', '100001010b005b06c94a', 'KUW 11 6728', '', 1, 0, 5185, 1),
(1068, 1, '2021-07-11 13:21:29', '100001010b005b06c95d', 'KUW 4 1734', '', 1, 0, 23208, 1),
(1069, 1, '2021-07-11 13:21:48', '100001010b005b06c970', 'KUW 4 7039', '', 1, 0, 17669, 1),
(1070, 1, '2021-07-11 13:22:07', '100001010b005b06c982', 'KUW 2 5306', '', 1, 0, 22656, 1),
(1071, 1, '2021-07-11 13:22:25', '100001010b005b06c995', 'KUW 11 6638', '', 1, 0, 14744, 1),
(1072, 1, '2021-07-11 13:22:44', '100001010b005b06c9a8', 'KUW 6 4002', '', 1, 0, 19147, 1),
(1073, 1, '2021-07-11 13:23:03', '100001010b005b06c9bb', 'KUW 13 9456', '', 1, 0, 42360, 1),
(1074, 1, '2021-07-11 13:23:22', '100001010b005b06c9ce', 'KUW 9 4895', '', 1, 0, 26601, 1),
(1075, 1, '2021-07-11 13:23:41', '100001010b005b06c9e0', 'KUW 6 2398', '', 1, 0, 41757, 1),
(1076, 1, '2021-07-11 13:23:59', '100001010b005b06c9f3', 'KUW 15 5426', '', 1, 0, 42687, 1),
(1077, 1, '2021-07-11 13:24:18', '100001010b005b06ca06', 'KUW 7 1284', '', 1, 0, 30380, 1),
(1078, 1, '2021-07-11 13:24:37', '100001010b005b06ca19', 'KUW 12 7163', '', 1, 0, 29652, 1),
(1079, 1, '2021-07-11 13:24:56', '100001010b005b06ca2c', 'KUW 10 8606', '', 1, 0, 21308, 1),
(1080, 1, '2021-07-11 13:25:15', '100001010b005b06ca3f', 'KUW 3 1385', '', 1, 0, 49000, 1),
(1081, 1, '2021-07-11 13:25:34', '100001010b005b06ca52', 'KUW 9 5971', '', 1, 0, 10777, 1),
(1082, 1, '2021-07-11 13:25:53', '100001010b005b06ca64', 'KUW 6 8657', '', 1, 0, 49492, 1),
(1083, 1, '2021-07-11 13:26:12', '100001010b005b06ca77', 'KUW 2 8365', '', 1, 0, 15318, 1),
(1084, 1, '2021-07-11 13:26:30', '100001010b005b06ca8a', 'KUW 13 9519', '', 1, 0, 14236, 1),
(1085, 1, '2021-07-11 13:26:49', '100001010b005b06ca9d', 'KUW 5 8339', '', 1, 0, 38034, 1),
(1086, 1, '2021-07-11 13:27:08', '100001010b005b06cab0', 'KUW 10 234', '', 1, 0, 31825, 1),
(1087, 1, '2021-07-11 13:27:27', '100001010b005b06cac3', 'KUW 8 756', '', 1, 0, 1195, 1),
(1088, 1, '2021-07-11 13:27:46', '100001010b005b06cad6', 'KUW 1 8278', '', 1, 0, 29856, 1),
(1089, 1, '2021-07-11 13:28:05', '100001010b005b06cae8', 'KUW 1 840', '', 1, 0, 52726, 1),
(1090, 1, '2021-07-11 13:28:24', '100001010b005b06cafb', 'KUW 1 9894', '', 1, 0, 24799, 1),
(1091, 1, '2021-07-11 13:28:42', '100001010b005b06cb0e', 'KUW 2 2257', '', 1, 0, 16359, 1),
(1092, 1, '2021-07-11 13:29:01', '100001010b005b06cb21', 'KUW 7 7384', '', 1, 0, 15361, 1),
(1093, 1, '2021-07-11 13:29:20', '100001010b005b06cb34', 'KUW 2 6792', '', 1, 0, 49708, 1),
(1094, 1, '2021-07-11 13:29:39', '100001010b005b06cb46', 'KUW 14 6720', '', 1, 0, 50897, 1),
(1095, 1, '2021-07-11 13:29:58', '100001010b005b06cb59', 'KUW 8 5944', '', 1, 0, 18986, 1),
(1096, 1, '2021-07-11 13:30:16', '100001010b005b06cb6c', 'KUW 1 4916', '', 1, 0, 34596, 1),
(1097, 1, '2021-07-11 13:30:35', '100001010b005b06cb7f', 'KUW 6 5105', '', 1, 0, 26059, 1),
(1098, 1, '2021-07-11 13:30:54', '100001010b005b06cb92', 'KUW 2 5588', '', 1, 0, 2696, 1),
(1099, 1, '2021-07-11 13:31:13', '100001010b005b06cba5', 'KUW 9 1825', '', 1, 0, 38137, 1),
(1100, 1, '2021-07-11 13:31:32', '100001010b005b06cbb7', 'KUW 7 6862', '', 1, 0, 51455, 1),
(1101, 1, '2021-07-11 13:31:51', '100001010b005b06cbca', 'KUW 5 7498', '', 1, 0, 6668, 1),
(1102, 1, '2021-07-11 13:32:09', '100001010b005b06cbdd', 'KUW 4 4223', '', 1, 0, 39817, 1),
(1103, 1, '2021-07-11 13:32:28', '100001010b005b06cbf0', 'KUW 7 1576', '', 1, 0, 16798, 1),
(1104, 1, '2021-07-11 13:32:47', '100001010b005b06cc03', 'KUW 14 259', '', 1, 0, 3193, 1),
(1105, 1, '2021-07-11 13:33:06', '100001010b005b06cc16', 'KUW 11 6226', '', 1, 0, 10259, 1),
(1106, 1, '2021-07-11 13:33:25', '100001010b005b06cc28', 'KUW 1 7800', '', 1, 0, 27098, 1),
(1107, 1, '2021-07-11 13:33:44', '100001010b005b06cc3b', 'KUW 9 2282', '', 1, 0, 29260, 1),
(1108, 1, '2021-07-11 13:34:02', '100001010b005b06cc4e', 'KUW 8 8375', '', 1, 0, 26047, 1),
(1109, 1, '2021-07-11 13:34:21', '100001010b005b06cc61', 'KUW 8 3238', '', 1, 0, 42370, 1),
(1110, 1, '2021-07-11 13:34:40', '100001010b005b06cc74', 'KUW 10 3570', '', 1, 0, 23151, 1),
(1111, 1, '2021-07-11 13:34:59', '100001010b005b06cc87', 'KUW 6 683', '', 1, 0, 50789, 1),
(1112, 1, '2021-07-11 13:35:18', '100001010b005b06cc99', 'KUW 3 4544', '', 1, 0, 36767, 1),
(1113, 1, '2021-07-11 13:35:36', '100001010b005b06ccac', 'KUW 15 5193', '', 1, 0, 28878, 1),
(1114, 1, '2021-07-11 13:35:55', '100001010b005b06ccbf', 'KUW 9 7659', '', 1, 0, 8713, 1),
(1115, 1, '2021-07-11 13:36:14', '100001010b005b06ccd2', 'KUW 6 1956', '', 1, 0, 5442, 1),
(1116, 1, '2021-07-11 13:36:33', '100001010b005b06cce5', 'KUW 10 5632', '', 1, 0, 7128, 1),
(1117, 1, '2021-07-11 13:36:52', '100001010b005b06ccf7', 'KUW 11 8761', '', 1, 0, 46643, 1),
(1118, 1, '2021-07-11 13:37:10', '100001010b005b06cd0a', 'KUW 5 4049', '', 1, 0, 19430, 1),
(1119, 1, '2021-07-11 13:37:29', '100001010b005b06cd1d', 'KUW 12 7532', '', 1, 0, 50001, 1),
(1120, 1, '2021-07-11 13:37:48', '100001010b005b06cd30', 'KUW 13 1426', '', 1, 0, 41653, 1),
(1121, 1, '2021-07-11 13:38:07', '100001010b005b06cd43', 'KUW 3 3232', '', 1, 0, 23264, 1),
(1122, 1, '2021-07-11 13:38:26', '100001010b005b06cd55', 'KUW 9 5764', '', 1, 0, 17922, 1),
(1123, 1, '2021-07-11 13:38:45', '100001010b005b06cd68', 'KUW 13 2100', '', 1, 0, 12552, 1),
(1124, 1, '2021-07-11 13:39:04', '100001010b005b06cd7b', 'KUW 3 5402', '', 1, 0, 50614, 1),
(1125, 1, '2021-07-11 13:39:22', '100001010b005b06cd8e', 'KUW 12 5295', '', 1, 0, 28776, 1),
(1126, 1, '2021-07-11 13:39:41', '100001010b005b06cda1', 'KUW 3 5482', '', 1, 0, 50056, 1),
(1127, 1, '2021-07-11 13:40:00', '100001010b005b06cdb4', 'KUW 9 2313', '', 1, 0, 42688, 1),
(1128, 1, '2021-07-11 13:40:19', '100001010b005b06cdc7', 'KUW 2 348', '', 1, 0, 22405, 1),
(1129, 1, '2021-07-11 13:40:38', '100001010b005b06cdd9', 'KUW 13 7302', '', 1, 0, 11038, 1),
(1130, 1, '2021-07-11 13:40:56', '100001010b005b06cdec', 'KUW 9 6079', '', 1, 0, 23713, 1),
(1131, 1, '2021-07-11 13:41:15', '100001010b005b06cdff', 'KUW 11 6208', '', 1, 0, 44869, 1),
(1132, 1, '2021-07-11 13:41:34', '100001010b005b06ce12', 'KUW 6 3642', '', 1, 0, 39628, 1),
(1133, 1, '2021-07-11 13:41:53', '100001010b005b06ce25', 'KUW 15 7853', '', 1, 0, 14160, 1),
(1134, 1, '2021-07-11 13:42:12', '100001010b005b06ce37', 'KUW 6 1842', '', 1, 0, 33968, 1),
(1135, 1, '2021-07-11 13:42:30', '100001010b005b06ce4a', 'KUW 6 5507', '', 1, 0, 46020, 1),
(1136, 1, '2021-07-11 13:42:49', '100001010b005b06ce5d', 'KUW 9 8268', '', 1, 0, 26204, 1),
(1137, 1, '2021-07-11 13:43:08', '100001010b005b06ce70', 'KUW 13 6681', '', 1, 0, 7679, 1),
(1138, 1, '2021-07-11 13:43:27', '100001010b005b06ce83', 'KUW 13 3305', '', 1, 0, 32465, 1),
(1139, 1, '2021-07-11 13:43:46', '100001010b005b06ce95', 'KUW 13 3566', '', 1, 0, 45228, 1),
(1140, 1, '2021-07-11 13:44:05', '100001010b005b06cea8', 'KUW 1 821', '', 1, 0, 15601, 1),
(1141, 1, '2021-07-11 13:44:23', '100001010b005b06cebb', 'KUW 15 3705', '', 1, 0, 2222, 1),
(1142, 1, '2021-07-11 13:44:42', '100001010b005b06cece', 'KUW 10 9516', '', 1, 0, 41347, 1),
(1143, 1, '2021-07-11 13:45:01', '100001010b005b06cee1', 'KUW 1 8124', '', 1, 0, 10887, 1),
(1144, 1, '2021-07-11 13:45:20', '100001010b005b06cef3', 'KUW 12 4874', '', 1, 0, 19196, 1),
(1145, 1, '2021-07-11 13:45:39', '100001010b005b06cf06', 'KUW 8 1606', '', 1, 0, 14038, 1),
(1146, 1, '2021-07-11 13:45:57', '100001010b005b06cf19', 'KUW 4 9755', '', 1, 0, 6090, 1),
(1147, 1, '2021-07-11 13:46:16', '100001010b005b06cf2c', 'KUW 2 9389', '', 1, 0, 26547, 1),
(1148, 1, '2021-07-11 13:46:35', '100001010b005b06cf3f', 'KUW 4 1197', '', 1, 0, 2865, 1),
(1149, 1, '2021-07-11 13:46:54', '100001010b005b06cf52', 'KUW 15 4646', '', 1, 0, 49836, 1),
(1150, 1, '2021-07-11 13:47:13', '100001010b005b06cf64', 'KUW 5 1382', '', 1, 0, 23300, 1),
(1151, 1, '2021-07-11 13:47:32', '100001010b005b06cf77', 'KUW 13 2524', '', 1, 0, 29889, 1),
(1152, 1, '2021-07-11 13:47:50', '100001010b005b06cf8a', 'KUW 8 8142', '', 1, 0, 27131, 1),
(1153, 1, '2021-07-11 13:48:09', '100001010b005b06cf9d', 'KUW 14 4790', '', 1, 0, 27541, 1),
(1154, 1, '2021-07-11 13:48:28', '100001010b005b06cfb0', 'KUW 10 2458', '', 1, 0, 6203, 1),
(1155, 1, '2021-07-11 13:48:47', '100001010b005b06cfc3', 'KUW 14 6328', '', 1, 0, 831, 1),
(1156, 1, '2021-07-11 13:49:06', '100001010b005b06cfd6', 'KUW 6 3090', '', 1, 0, 5241, 1),
(1157, 1, '2021-07-11 13:49:25', '100001010b005b06cfe8', 'KUW 1 3124', '', 1, 0, 34753, 1),
(1158, 1, '2021-07-11 13:49:43', '100001010b005b06cffb', 'KUW 1 8096', '', 1, 0, 22984, 1),
(1159, 1, '2021-07-11 13:50:02', '100001010b005b06d00e', 'KUW 6 1081', '', 1, 0, 35702, 1),
(1160, 1, '2021-07-11 13:50:21', '100001010b005b06d021', 'KUW 9 905', '', 1, 0, 29595, 1),
(1161, 1, '2021-07-11 13:50:40', '100001010b005b06d034', 'KUW 1 5212', '', 1, 0, 16670, 1),
(1162, 1, '2021-07-11 13:50:59', '100001010b005b06d046', 'KUW 2 1101', '', 1, 0, 12703, 1),
(1163, 1, '2021-07-11 13:51:17', '100001010b005b06d059', 'KUW 14 5432', '', 1, 0, 13772, 1),
(1164, 1, '2021-07-11 13:51:36', '100001010b005b06d06c', 'KUW 3 9460', '', 1, 0, 26602, 1),
(1165, 1, '2021-07-11 13:51:55', '100001010b005b06d07f', 'KUW 7 5634', '', 1, 0, 1442, 1),
(1166, 1, '2021-07-11 13:52:14', '100001010b005b06d092', 'KUW 8 9279', '', 1, 0, 17000, 1),
(1167, 1, '2021-07-11 13:52:33', '100001010b005b06d0a4', 'KUW 5 6819', '', 1, 0, 41421, 1),
(1168, 1, '2021-07-11 13:52:52', '100001010b005b06d0b7', 'KUW 4 1571', '', 1, 0, 3472, 1),
(1169, 1, '2021-07-11 13:53:10', '100001010b005b06d0ca', 'KUW 12 5106', '', 1, 0, 37700, 1),
(1170, 1, '2021-07-11 13:53:29', '100001010b005b06d0dd', 'KUW 11 2346', '', 1, 0, 9494, 1),
(1171, 1, '2021-07-11 13:53:48', '100001010b005b06d0f0', 'KUW 7 7280', '', 1, 0, 1201, 1),
(1172, 1, '2021-07-11 13:54:07', '100001010b005b06d103', 'KUW 11 2958', '', 1, 0, 12281, 1),
(1173, 1, '2021-07-11 13:54:26', '100001010b005b06d115', 'KUW 1 7946', '', 1, 0, 37431, 1),
(1174, 1, '2021-07-11 13:54:45', '100001010b005b06d128', 'KUW 6 1652', '', 1, 0, 39041, 1),
(1175, 1, '2021-07-11 13:55:03', '100001010b005b06d13b', 'KUW 12 5374', '', 1, 0, 2205, 1),
(1176, 1, '2021-07-11 13:55:22', '100001010b005b06d14e', 'KUW 8 1333', '', 1, 0, 44478, 1),
(1177, 1, '2021-07-11 13:55:41', '100001010b005b06d161', 'KUW 13 2850', '', 1, 0, 37900, 1),
(1178, 1, '2021-07-11 13:56:00', '100001010b005b06d174', 'KUW 5 4811', '', 1, 0, 7706, 1),
(1179, 1, '2021-07-11 13:56:19', '100001010b005b06d187', 'KUW 7 564', '', 1, 0, 19486, 1),
(1180, 1, '2021-07-11 13:56:38', '100001010b005b06d199', 'KUW 13 9072', '', 1, 0, 43407, 1),
(1181, 1, '2021-07-11 13:56:56', '100001010b005b06d1ac', 'KUW 7 207', '', 1, 0, 25000, 1),
(1182, 1, '2021-07-11 13:57:15', '100001010b005b06d1bf', 'KUW 2 3745', '', 1, 0, 34046, 1),
(1183, 1, '2021-07-11 13:57:34', '100001010b005b06d1d2', 'KUW 13 3554', '', 1, 0, 30021, 1),
(1184, 1, '2021-07-11 13:57:53', '100001010b005b06d1e5', 'KUW 3 5380', '', 1, 0, 13884, 1),
(1185, 1, '2021-07-11 13:58:12', '100001010b005b06d1f7', 'KUW 1 9465', '', 1, 0, 25660, 1),
(1186, 1, '2021-07-11 13:58:30', '100001010b005b06d20a', 'KUW 6 1746', '', 1, 0, 39190, 1),
(1187, 1, '2021-07-11 13:58:49', '100001010b005b06d21d', 'KUW 6 272', '', 1, 0, 6490, 1),
(1188, 1, '2021-07-11 13:59:08', '100001010b005b06d230', 'KUW 2 2339', '', 1, 0, 45742, 1),
(1189, 1, '2021-07-11 13:59:27', '100001010b005b06d243', 'KUW 10 1476', '', 1, 0, 36978, 1),
(1190, 1, '2021-07-11 13:59:46', '100001010b005b06d256', 'KUW 13 5624', '', 1, 0, 1476, 1),
(1191, 1, '2021-07-11 14:00:05', '100001010b005b06d268', 'KUW 9 1763', '', 1, 0, 5051, 1),
(1192, 1, '2021-07-11 14:00:24', '100001010b005b06d27b', 'KUW 5 1063', '', 1, 0, 5216, 1),
(1193, 1, '2021-07-11 14:00:42', '100001010b005b06d28e', 'KUW 10 7943', '', 1, 0, 15357, 1),
(1194, 1, '2021-07-11 14:01:01', '100001010b005b06d2a1', 'KUW 3 250', '', 1, 0, 47441, 1),
(1195, 1, '2021-07-11 14:01:20', '100001010b005b06d2b4', 'KUW 11 6642', '', 1, 0, 50102, 1),
(1196, 1, '2021-07-11 14:01:39', '100001010b005b06d2c7', 'KUW 9 532', '', 1, 0, 3195, 1),
(1197, 1, '2021-07-11 14:01:58', '100001010b005b06d2d9', 'KUW 14 8502', '', 1, 0, 13719, 1),
(1198, 1, '2021-07-11 14:02:17', '100001010b005b06d2ec', 'KUW 1 4492', '', 1, 0, 30782, 1),
(1199, 1, '2021-07-11 14:02:35', '100001010b005b06d2ff', 'KUW 6 6746', '', 1, 0, 22697, 1),
(1200, 1, '2021-07-11 14:02:54', '100001010b005b06d312', 'KUW 14 9518', '', 1, 0, 15195, 1),
(1201, 1, '2021-07-11 14:03:13', '100001010b005b06d325', 'KUW 2 1284', '', 1, 0, 15948, 1),
(1202, 1, '2021-07-11 14:03:32', '100001010b005b06d338', 'KUW 8 7422', '', 1, 0, 15718, 1),
(1203, 1, '2021-07-11 14:03:51', '100001010b005b06d34b', 'KUW 10 2314', '', 1, 0, 42664, 1),
(1204, 1, '2021-07-11 14:04:10', '100001010b005b06d35d', 'KUW 5 7851', '', 1, 0, 32132, 1),
(1205, 1, '2021-07-11 14:04:29', '100001010b005b06d370', 'KUW 15 3538', '', 1, 0, 7703, 1),
(1206, 1, '2021-07-11 14:04:47', '100001010b005b06d383', 'KUW 15 6234', '', 1, 0, 17902, 1),
(1207, 1, '2021-07-11 14:05:06', '100001010b005b06d396', 'KUW 12 8100', '', 1, 0, 13305, 1),
(1208, 1, '2021-07-11 14:05:25', '100001010b005b06d3a9', 'KUW 9 3339', '', 1, 0, 29218, 1),
(1209, 1, '2021-07-11 14:05:44', '100001010b005b06d3bb', 'KUW 9 3957', '', 1, 0, 12904, 1),
(1210, 1, '2021-07-11 14:06:03', '100001010b005b06d3ce', 'KUW 13 6010', '', 1, 0, 22551, 1),
(1211, 1, '2021-07-11 14:06:21', '100001010b005b06d3e1', 'KUW 5 5135', '', 1, 0, 32096, 1),
(1212, 1, '2021-07-11 14:06:40', '100001010b005b06d3f4', 'KUW 12 459', '', 1, 0, 8807, 1),
(1213, 1, '2021-07-11 14:06:59', '100001010b005b06d407', 'KUW 7 7663', '', 1, 0, 16334, 1),
(1214, 1, '2021-07-11 14:07:18', '100001010b005b06d41a', 'KUW 11 9339', '', 1, 0, 5479, 1),
(1215, 1, '2021-07-11 14:07:37', '100001010b005b06d42c', 'KUW 12 4232', '', 1, 0, 581, 1),
(1216, 1, '2021-07-11 14:07:56', '100001010b005b06d43f', 'KUW 6 7913', '', 1, 0, 27886, 1),
(1217, 1, '2021-07-11 14:08:14', '100001010b005b06d452', 'KUW 13 3554', '', 1, 0, 30021, 1),
(1218, 1, '2021-07-11 14:08:33', '100001010b005b06d465', 'KUW 7 763', '', 1, 0, 2971, 1),
(1219, 1, '2021-07-11 14:08:52', '100001010b005b06d478', 'KUW 8 3082', '', 1, 0, 32583, 1),
(1220, 1, '2021-07-11 14:09:11', '100001010b005b06d48b', 'KUW 12 8956', '', 1, 0, 17495, 1),
(1221, 1, '2021-07-11 14:09:30', '100001010b005b06d49d', 'KUW 14 2863', '', 1, 0, 29966, 1),
(1222, 1, '2021-07-11 14:09:49', '100001010b005b06d4b0', 'KUW 9 7186', '', 1, 0, 29607, 1),
(1223, 1, '2021-07-11 14:10:07', '100001010b005b06d4c3', 'KUW 1 3166', '', 1, 0, 20341, 1),
(1224, 1, '2021-07-11 14:10:26', '100001010b005b06d4d6', 'KUW 2 7708', '', 1, 0, 20084, 1),
(1225, 1, '2021-07-11 14:10:45', '100001010b005b06d4e9', 'KUW 2 5004', '', 1, 0, 27799, 1),
(1226, 1, '2021-07-11 14:11:04', '100001010b005b06d4fc', 'KUW 9 2073', '', 1, 0, 5180, 1),
(1227, 1, '2021-07-11 14:11:23', '100001010b005b06d50e', 'KUW 7 3445', '', 1, 0, 3862, 1),
(1228, 1, '2021-07-11 14:11:42', '100001010b005b06d521', 'KUW 1 6106', '', 1, 0, 47478, 1),
(1229, 1, '2021-07-11 14:12:00', '100001010b005b06d534', 'KUW 12 9226', '', 1, 0, 30414, 1),
(1230, 1, '2021-07-11 14:12:19', '100001010b005b06d547', 'KUW 5 8056', '', 1, 0, 48427, 1),
(1231, 1, '2021-07-11 14:12:38', '100001010b005b06d55a', 'KUW 14 5830', '', 1, 0, 35800, 1),
(1232, 1, '2021-07-11 14:12:57', '100001010b005b06d56d', 'KUW 15 8377', '', 1, 0, 38443, 1),
(1233, 1, '2021-07-11 14:13:16', '100001010b005b06d57f', 'KUW 6 7193', '', 1, 0, 45334, 1),
(1234, 1, '2021-07-11 14:13:35', '100001010b005b06d592', 'KUW 1 5435', '', 1, 0, 34060, 1),
(1235, 1, '2021-07-11 14:13:53', '100001010b005b06d5a5', 'KUW 2 5265', '', 1, 0, 28317, 1),
(1236, 1, '2021-07-11 14:14:12', '100001010b005b06d5b8', 'KUW 10 3895', '', 1, 0, 35061, 1),
(1237, 1, '2021-07-11 14:14:31', '100001010b005b06d5cb', 'KUW 8 530', '', 1, 0, 8219, 1),
(1238, 1, '2021-07-11 14:14:50', '100001010b005b06d5de', 'KUW 7 3259', '', 1, 0, 14501, 1),
(1239, 1, '2021-07-11 14:15:09', '100001010b005b06d5f0', 'KUW 8 4543', '', 1, 0, 4012, 1),
(1240, 1, '2021-07-11 14:15:27', '100001010b005b06d603', 'KUW 9 5231', '', 1, 0, 6102, 1),
(1241, 1, '2021-07-11 14:15:46', '100001010b005b06d616', 'KUW 3 6590', '', 1, 0, 5348, 1),
(1242, 1, '2021-07-11 14:16:05', '100001010b005b06d629', 'KUW 3 661', '', 1, 0, 51393, 1),
(1243, 1, '2021-07-11 14:16:24', '100001010b005b06d63c', 'KUW 3 8322', '', 1, 0, 11006, 1),
(1244, 1, '2021-07-11 14:16:43', '100001010b005b06d64f', 'KUW 9 8335', '', 1, 0, 44863, 1),
(1245, 1, '2021-07-11 14:17:02', '100001010b005b06d661', 'KUW 13 4574', '', 1, 0, 29267, 1),
(1246, 1, '2021-07-11 14:17:21', '100001010b005b06d674', 'KUW 6 7956', '', 1, 0, 10624, 1),
(1247, 1, '2021-07-11 14:17:39', '100001010b005b06d687', 'KUW 10 4093', '', 1, 0, 48314, 1),
(1248, 1, '2021-07-11 14:17:58', '100001010b005b06d69a', 'KUW 8 8215', '', 1, 0, 12971, 1),
(1249, 1, '2021-07-11 14:18:17', '100001010b005b06d6ad', 'KUW 5 4861', '', 1, 0, 38517, 1),
(1250, 1, '2021-07-11 14:18:36', '100001010b005b06d6c0', 'KUW 14 6081', '', 1, 0, 34132, 1),
(1251, 1, '2021-07-11 14:18:55', '100001010b005b06d6d3', 'KUW 8 2656', '', 1, 0, 15973, 1),
(1252, 1, '2021-07-11 14:19:14', '100001010b005b06d6e5', 'KUW 12 3368', '', 1, 0, 28551, 1),
(1253, 1, '2021-07-11 14:19:33', '100001010b005b06d6f8', 'KUW 6 2949', '', 1, 0, 15923, 1),
(1254, 1, '2021-07-11 14:19:52', '100001010b005b06d70b', 'KUW 5 9991', '', 1, 0, 27611, 1),
(1255, 1, '2021-07-11 14:20:10', '100001010b005b06d71e', 'KUW 12 4683', '', 1, 0, 2398, 1),
(1256, 1, '2021-07-11 14:20:29', '100001010b005b06d731', 'KUW 6 550', '', 1, 0, 10810, 1),
(1257, 1, '2021-07-11 14:20:48', '100001010b005b06d744', 'KUW 4 7137', '', 1, 0, 47083, 1),
(1258, 1, '2021-07-11 14:21:07', '100001010b005b06d757', 'KUW 12 760', '', 1, 0, 40852, 1),
(1259, 1, '2021-07-11 14:21:26', '100001010b005b06d769', 'KUW 5 6374', '', 1, 0, 38901, 1),
(1260, 1, '2021-07-11 14:21:44', '100001010b005b06d77c', 'KUW 11 1729', '', 1, 0, 30971, 1),
(1261, 1, '2021-07-11 14:22:03', '100001010b005b06d78f', 'KUW 13 1094', '', 1, 0, 26950, 1),
(1262, 1, '2021-07-11 14:22:22', '100001010b005b06d7a2', 'KUW 13 523', '', 1, 0, 3346, 1),
(1263, 1, '2021-07-11 14:22:41', '100001010b005b06d7b5', 'KUW 13 7060', '', 1, 0, 50502, 1),
(1264, 1, '2021-07-11 14:23:00', '100001010b005b06d7c8', 'KUW 11 7127', '', 1, 0, 41581, 1),
(1265, 1, '2021-07-11 14:23:19', '100001010b005b06d7da', 'KUW 4 3920', '', 1, 0, 25889, 1),
(1266, 1, '2021-07-11 14:31:37', '100001010b005b06d9cd', 'KUW 8 6092', '', 1, 0, 3828, 1),
(1267, 1, '2021-07-11 14:31:56', '100001010b005b06d9e0', 'KUW 1 5108', '', 1, 0, 2732, 1),
(1268, 1, '2021-07-11 14:32:15', '100001010b005b06d9f3', 'KUW 1 3776', '', 1, 0, 18380, 1),
(1269, 1, '2021-07-11 14:32:34', '100001010b005b06da06', 'KUW 3 5444', '', 1, 0, 1922, 1),
(1270, 1, '2021-07-11 14:32:53', '100001010b005b06da18', 'KUW 12 6943', '', 1, 0, 39257, 1),
(1271, 1, '2021-07-11 14:33:12', '100001010b005b06da2b', 'KUW 13 1871', '', 1, 0, 20430, 1),
(1272, 1, '2021-07-11 14:33:30', '100001010b005b06da3e', 'KUW 1 6340', '', 1, 0, 6929, 1),
(1273, 1, '2021-07-11 14:33:49', '100001010b005b06da51', 'KUW 14 1222', '', 1, 0, 48777, 1),
(1274, 1, '2021-07-11 14:34:08', '100001010b005b06da64', 'KUW 4 9762', '', 1, 0, 6070, 1),
(1275, 1, '2021-07-11 14:34:27', '100001010b005b06da76', 'KUW 10 1559', '', 1, 0, 48724, 1),
(1276, 1, '2021-07-11 14:34:46', '100001010b005b06da89', 'KUW 5 5758', '', 1, 0, 42568, 1),
(1277, 1, '2021-07-11 14:35:04', '100001010b005b06da9c', 'KUW 8 8076', '', 1, 0, 14925, 1),
(1278, 1, '2021-07-11 14:35:23', '100001010b005b06daaf', 'KUW 12 9931', '', 1, 0, 24419, 1),
(1279, 1, '2021-07-11 14:35:42', '100001010b005b06dac2', 'KUW 14 9370', '', 1, 0, 45590, 1),
(1280, 1, '2021-07-11 14:36:01', '100001010b005b06dad5', 'KUW 8 9888', '', 1, 0, 23842, 1),
(1281, 1, '2021-07-11 14:36:20', '100001010b005b06dae7', 'KUW 8 872', '', 1, 0, 18054, 1),
(1282, 1, '2021-07-11 14:36:39', '100001010b005b06dafa', 'KUW 7 8534', '', 1, 0, 11260, 1),
(1283, 1, '2021-07-11 14:36:57', '100001010b005b06db0d', 'KUW 4 1421', '', 1, 0, 31104, 1),
(1284, 1, '2021-07-11 14:37:16', '100001010b005b06db20', 'KUW 15 1460', '', 1, 0, 18771, 1),
(1285, 1, '2021-07-11 14:37:35', '100001010b005b06db33', 'KUW 5 9202', '', 1, 0, 40722, 1),
(1286, 1, '2021-07-11 14:37:54', '100001010b005b06db46', 'KUW 8 2986', '', 1, 0, 26608, 1),
(1287, 1, '2021-07-11 14:38:13', '100001010b005b06db59', 'KUW 15 6123', '', 1, 0, 12521, 1),
(1288, 1, '2021-07-11 14:38:32', '100001010b005b06db6c', 'KUW 1 7320', '', 1, 0, 50235, 1),
(1289, 1, '2021-07-11 14:38:51', '100001010b005b06db7e', 'KUW 1 2475', '', 1, 0, 4645, 1),
(1290, 1, '2021-07-11 14:39:10', '100001010b005b06db91', 'KUW 7 2478', '', 1, 0, 32193, 1),
(1291, 1, '2021-07-11 14:39:28', '100001010b005b06dba4', 'KUW 12 4302', '', 1, 0, 46615, 1),
(1292, 1, '2021-07-11 14:39:47', '100001010b005b06dbb7', 'KUW 7 5358', '', 1, 0, 36377, 1),
(1293, 1, '2021-07-11 14:40:06', '100001010b005b06dbca', 'KUW 14 9299', '', 1, 0, 46900, 1),
(1294, 1, '2021-07-11 14:40:25', '100001010b005b06dbdd', 'KUW 7 431', '', 1, 0, 52297, 1),
(1295, 1, '2021-07-11 14:40:44', '100001010b005b06dbef', 'KUW 1 6175', '', 1, 0, 26586, 1),
(1296, 1, '2021-07-11 14:41:03', '100001010b005b06dc02', 'KUW 15 8490', '', 1, 0, 44525, 1),
(1297, 1, '2021-07-11 14:41:21', '100001010b005b06dc15', 'KUW 4 9469', '', 1, 0, 42512, 1),
(1298, 1, '2021-07-11 14:41:40', '100001010b005b06dc28', 'KUW 12 2364', '', 1, 0, 24781, 1),
(1299, 1, '2021-07-11 14:41:59', '100001010b005b06dc3b', 'KUW 7 7280', '', 1, 0, 38193, 1),
(1300, 1, '2021-07-11 14:42:18', '100001010b005b06dc4e', 'KUW 15 6423', '', 1, 0, 29849, 1),
(1301, 1, '2021-07-11 14:42:37', '100001010b005b06dc60', 'KUW 5 8380', '', 1, 0, 31815, 1),
(1302, 1, '2021-07-11 14:42:56', '100001010b005b06dc73', 'KUW 3 148', '', 1, 0, 12467, 1),
(1303, 1, '2021-07-11 14:43:14', '100001010b005b06dc86', 'KUW 6 3180', '', 1, 0, 11892, 1),
(1304, 1, '2021-07-11 14:43:33', '100001010b005b06dc99', 'KUW 4 8386', '', 1, 0, 19046, 1),
(1305, 1, '2021-07-11 14:43:52', '100001010b005b06dcac', 'KUW 5 2322', '', 1, 0, 25127, 1),
(1306, 1, '2021-07-11 14:44:11', '100001010b005b06dcbf', 'KUW 8 6717', '', 1, 0, 15329, 1),
(1307, 1, '2021-07-11 14:44:30', '100001010b005b06dcd1', 'KUW 4 49', '', 1, 0, 36238, 1),
(1308, 1, '2021-07-11 14:44:48', '100001010b005b06dce4', 'KUW 10 6645', '', 1, 0, 40902, 1),
(1309, 1, '2021-07-11 14:45:07', '100001010b005b06dcf7', 'KUW 5 6937', '', 1, 0, 26285, 1),
(1310, 1, '2021-07-11 14:45:26', '100001010b005b06dd0a', 'KUW 13 3507', '', 1, 0, 19504, 1),
(1311, 1, '2021-07-11 14:45:45', '100001010b005b06dd1d', 'KUW 9 2723', '', 1, 0, 1385, 1),
(1312, 1, '2021-07-11 14:46:04', '100001010b005b06dd2f', 'KUW 3 5368', '', 1, 0, 18264, 1),
(1313, 1, '2021-07-11 14:46:23', '100001010b005b06dd42', 'KUW 6 8947', '', 1, 0, 15048, 1),
(1314, 1, '2021-07-11 14:46:41', '100001010b005b06dd55', 'KUW 2 8976', '', 1, 0, 39184, 1),
(1315, 1, '2021-07-11 14:47:00', '100001010b005b06dd68', 'KUW 12 6385', '', 1, 0, 25391, 1),
(1316, 1, '2021-07-11 14:47:19', '100001010b005b06dd7b', 'KUW 14 4462', '', 1, 0, 25877, 1),
(1317, 1, '2021-07-11 14:47:38', '100001010b005b06dd8e', 'KUW 13 7856', '', 1, 0, 468, 1),
(1318, 1, '2021-07-11 14:47:57', '100001010b005b06dda0', 'KUW 8 4976', '', 1, 0, 25116, 1),
(1319, 1, '2021-07-11 14:48:16', '100001010b005b06ddb3', 'KUW 15 3102', '', 1, 0, 43275, 1),
(1320, 1, '2021-07-11 14:48:34', '100001010b005b06ddc6', 'KUW 9 6286', '', 1, 0, 6140, 1),
(1321, 1, '2021-07-11 14:48:53', '100001010b005b06ddd9', 'KUW 11 5937', '', 1, 0, 25208, 1),
(1322, 1, '2021-07-11 14:49:12', '100001010b005b06ddec', 'KUW 9 5654', '', 1, 0, 18122, 1),
(1323, 1, '2021-07-11 14:49:31', '100001010b005b06ddff', 'KUW 14 2036', '', 1, 0, 7050, 1),
(1324, 1, '2021-07-11 14:49:50', '100001010b005b06de11', 'KUW 13 39', '', 1, 0, 49706, 1),
(1325, 1, '2021-07-11 14:50:09', '100001010b005b06de24', 'KUW 6 9094', '', 1, 0, 50022, 1),
(1326, 1, '2021-07-11 14:50:28', '100001010b005b06de37', 'KUW 11 3933', '', 1, 0, 9779, 1),
(1327, 1, '2021-07-11 14:50:46', '100001010b005b06de4a', 'KUW 10 1354', '', 1, 0, 49699, 1),
(1328, 1, '2021-07-11 14:51:05', '100001010b005b06de5d', 'KUW 2 5221', '', 1, 0, 46294, 1),
(1329, 1, '2021-07-11 14:51:24', '100001010b005b06de70', 'KUW 13 7827', '', 1, 0, 24995, 1),
(1330, 1, '2021-07-11 14:51:43', '100001010b005b06de83', 'KUW 12 1017', '', 1, 0, 11405, 1),
(1331, 1, '2021-07-11 14:52:02', '100001010b005b06de95', 'KUW 4 2368', '', 1, 0, 38246, 1),
(1332, 1, '2021-07-11 14:52:21', '100001010b005b06dea8', 'KUW 13 2568', '', 1, 0, 45466, 1),
(1333, 1, '2021-07-11 14:52:39', '100001010b005b06debb', 'KUW 15 8825', '', 1, 0, 51490, 1),
(1334, 1, '2021-07-11 14:52:58', '100001010b005b06dece', 'KUW 3 2770', '', 1, 0, 51385, 1),
(1335, 1, '2021-07-11 14:53:17', '100001010b005b06dee1', 'KUW 4 122', '', 1, 0, 37896, 1),
(1336, 1, '2021-07-11 14:53:36', '100001010b005b06def4', 'KUW 7 9184', '', 1, 0, 17566, 1),
(1337, 1, '2021-07-11 14:53:55', '100001010b005b06df06', 'KUW 7 4752', '', 1, 0, 11538, 1),
(1338, 1, '2021-07-11 14:54:14', '100001010b005b06df19', 'KUW 8 4698', '', 1, 0, 34267, 1),
(1339, 1, '2021-07-11 14:54:32', '100001010b005b06df2c', 'KUW 13 7430', '', 1, 0, 50680, 1),
(1340, 1, '2021-07-11 14:54:51', '100001010b005b06df3f', 'KUW 3 9481', '', 1, 0, 42788, 1),
(1341, 1, '2021-07-11 14:55:10', '100001010b005b06df52', 'KUW 14 5875', '', 1, 0, 49361, 1),
(1342, 1, '2021-07-11 14:55:29', '100001010b005b06df65', 'KUW 5 8315', '', 1, 0, 40028, 1),
(1343, 1, '2021-07-11 14:55:48', '100001010b005b06df77', 'KUW 15 8074', '', 1, 0, 18279, 1),
(1344, 1, '2021-07-11 14:56:07', '100001010b005b06df8a', 'KUW 14 7512', '', 1, 0, 9516, 1),
(1345, 1, '2021-07-11 14:56:25', '100001010b005b06df9d', 'KUW 5 5269', '', 1, 0, 13628, 1),
(1346, 1, '2021-07-11 14:56:44', '100001010b005b06dfb0', 'KUW 13 4918', '', 1, 0, 19585, 1),
(1347, 1, '2021-07-11 14:57:03', '100001010b005b06dfc3', 'KUW 10 3679', '', 1, 0, 49044, 1),
(1348, 1, '2021-07-11 14:57:22', '100001010b005b06dfd6', 'KUW 9 3910', '', 1, 0, 23132, 1),
(1349, 1, '2021-07-11 14:57:41', '100001010b005b06dfe8', 'KUW 1 4803', '', 1, 0, 24821, 1),
(1350, 1, '2021-07-11 14:58:00', '100001010b005b06dffb', 'KUW 1 6153', '', 1, 0, 21446, 1),
(1351, 1, '2021-07-11 14:58:18', '100001010b005b06e00e', 'KUW 9 2696', '', 1, 0, 27580, 1),
(1352, 1, '2021-07-11 14:58:37', '100001010b005b06e021', 'KUW 3 1554', '', 1, 0, 15134, 1),
(1353, 1, '2021-07-11 14:58:56', '100001010b005b06e034', 'KUW 14 5506', '', 1, 0, 3893, 1),
(1354, 1, '2021-07-11 14:59:15', '100001010b005b06e047', 'KUW 9 2101', '', 1, 0, 34619, 1),
(1355, 1, '2021-07-11 14:59:34', '100001010b005b06e05a', 'KUW 15 1502', '', 1, 0, 12062, 1),
(1356, 1, '2021-07-11 14:59:53', '100001010b005b06e06c', 'KUW 8 5932', '', 1, 0, 46088, 1),
(1357, 1, '2021-07-11 15:00:12', '100001010b005b06e07f', 'KUW 13 4818', '', 1, 0, 34938, 1),
(1358, 1, '2021-07-11 15:00:30', '100001010b005b06e092', 'KUW 2 634', '', 1, 0, 1616, 1),
(1359, 1, '2021-07-11 15:00:49', '100001010b005b06e0a5', 'KUW 2 825', '', 1, 0, 31940, 1),
(1360, 1, '2021-07-11 15:01:08', '100001010b005b06e0b8', 'KUW 9 4969', '', 1, 0, 36187, 1),
(1361, 1, '2021-07-11 15:01:27', '100001010b005b06e0ca', 'KUW 4 6644', '', 1, 0, 19133, 1),
(1362, 1, '2021-07-11 15:01:46', '100001010b005b06e0dd', 'KUW 2 4367', '', 1, 0, 37593, 1),
(1363, 1, '2021-07-11 15:02:04', '100001010b005b06e0f0', 'KUW 11 8075', '', 1, 0, 41636, 1),
(1364, 1, '2021-07-11 15:02:23', '100001010b005b06e103', 'KUW 10 7359', '', 1, 0, 22863, 1),
(1365, 1, '2021-07-11 15:02:42', '100001010b005b06e116', 'KUW 13 6459', '', 1, 0, 14622, 1),
(1366, 1, '2021-07-11 15:03:01', '100001010b005b06e129', 'KUW 13 8976', '', 1, 0, 17284, 1),
(1367, 1, '2021-07-11 15:03:20', '100001010b005b06e13c', 'KUW 12 5083', '', 1, 0, 10963, 1),
(1368, 1, '2021-07-11 15:03:39', '100001010b005b06e14e', 'KUW 3 1155', '', 1, 0, 11234, 1),
(1369, 1, '2021-07-11 15:03:57', '100001010b005b06e161', 'KUW 14 1011', '', 1, 0, 22766, 1),
(1370, 1, '2021-07-11 15:04:16', '100001010b005b06e174', 'KUW 8 7055', '', 1, 0, 37666, 1),
(1371, 1, '2021-07-11 15:04:35', '100001010b005b06e187', 'KUW 14 4072', '', 1, 0, 45094, 1),
(1372, 1, '2021-07-11 15:04:54', '100001010b005b06e19a', 'KUW 13 7082', '', 1, 0, 29735, 1),
(1373, 1, '2021-07-11 15:05:13', '100001010b005b06e1ac', 'KUW 10 9619', '', 1, 0, 4860, 1),
(1374, 1, '2021-07-11 15:05:32', '100001010b005b06e1bf', 'KUW 7 1329', '', 1, 0, 29234, 1),
(1375, 1, '2021-07-11 15:05:50', '100001010b005b06e1d2', 'KUW 11 6110', '', 1, 0, 49216, 1),
(1376, 1, '2021-07-11 15:06:09', '100001010b005b06e1e5', 'KUW 13 9609', '', 1, 0, 47549, 1),
(1377, 1, '2021-07-11 15:06:28', '100001010b005b06e1f8', 'KUW 7 1576', '', 1, 0, 30780, 1),
(1378, 1, '2021-07-11 15:06:47', '100001010b005b06e20a', 'KUW 15 9012', '', 1, 0, 20773, 1),
(1379, 1, '2021-07-11 15:07:06', '100001010b005b06e21d', 'KUW 3 1729', '', 1, 0, 36221, 1),
(1380, 1, '2021-07-11 15:07:25', '100001010b005b06e230', 'KUW 7 2951', '', 1, 0, 40132, 1),
(1381, 1, '2021-07-11 15:07:43', '100001010b005b06e243', 'KUW 3 8386', '', 1, 0, 11551, 1),
(1382, 1, '2021-07-11 15:08:02', '100001010b005b06e256', 'KUW 3 6066', '', 1, 0, 5775, 1),
(1383, 1, '2021-07-11 15:08:21', '100001010b005b06e269', 'KUW 10 7977', '', 1, 0, 52598, 1),
(1384, 1, '2021-07-11 15:08:40', '100001010b005b06e27b', 'KUW 2 3002', '', 1, 0, 757, 1),
(1385, 1, '2021-07-11 15:08:59', '100001010b005b06e28e', 'KUW 9 6833', '', 1, 0, 38870, 1),
(1386, 1, '2021-07-11 15:09:17', '100001010b005b06e2a1', 'KUW 12 7287', '', 1, 0, 14951, 1),
(1387, 1, '2021-07-11 15:09:36', '100001010b005b06e2b4', 'KUW 12 6498', '', 1, 0, 11962, 1),
(1388, 1, '2021-07-11 15:09:55', '100001010b005b06e2c7', 'KUW 7 5786', '', 1, 0, 10857, 1),
(1389, 1, '2021-07-11 15:10:14', '100001010b005b06e2da', 'KUW 2 9343', '', 1, 0, 29685, 1),
(1390, 1, '2021-07-11 15:10:33', '100001010b005b06e2ed', 'KUW 15 220', '', 1, 0, 43604, 1),
(1391, 1, '2021-07-11 15:10:52', '100001010b005b06e2ff', 'KUW 12 6899', '', 1, 0, 29630, 1),
(1392, 1, '2021-07-11 15:11:10', '100001010b005b06e312', 'KUW 2 7622', '', 1, 0, 31107, 1),
(1393, 1, '2021-07-11 15:11:29', '100001010b005b06e325', 'KUW 12 1841', '', 1, 0, 28721, 1),
(1394, 1, '2021-07-11 15:11:48', '100001010b005b06e338', 'KUW 15 814', '', 1, 0, 30987, 1),
(1395, 1, '2021-07-11 15:12:07', '100001010b005b06e34b', 'KUW 12 6070', '', 1, 0, 21189, 1),
(1396, 1, '2021-07-11 15:12:26', '100001010b005b06e35e', 'KUW 10 3872', '', 1, 0, 25721, 1),
(1397, 1, '2021-07-11 15:12:45', '100001010b005b06e370', 'KUW 11 8636', '', 1, 0, 43658, 1),
(1398, 1, '2021-07-11 15:13:03', '100001010b005b06e383', 'KUW 3 2976', '', 1, 0, 52782, 1),
(1399, 1, '2021-07-11 15:13:22', '100001010b005b06e396', 'KUW 7 6139', '', 1, 0, 742, 1),
(1400, 1, '2021-07-11 15:13:41', '100001010b005b06e3a9', 'KUW 13 9126', '', 1, 0, 35996, 1),
(1401, 1, '2021-07-11 15:14:00', '100001010b005b06e3bc', 'KUW 12 6845', '', 1, 0, 9273, 1),
(1402, 1, '2021-07-11 15:14:19', '100001010b005b06e3ce', 'KUW 15 4543', '', 1, 0, 21368, 1),
(1403, 1, '2021-07-11 15:14:38', '100001010b005b06e3e1', 'KUW 8 783', '', 1, 0, 30569, 1),
(1404, 1, '2021-07-11 15:14:56', '100001010b005b06e3f4', 'KUW 12 5522', '', 1, 0, 19423, 1),
(1405, 1, '2021-07-11 15:15:15', '100001010b005b06e407', 'KUW 10 396', '', 1, 0, 5087, 1),
(1406, 1, '2021-07-11 15:15:34', '100001010b005b06e41a', 'KUW 10 2812', '', 1, 0, 41140, 1),
(1407, 1, '2021-07-11 15:15:53', '100001010b005b06e42d', 'KUW 1 3789', '', 1, 0, 6121, 1),
(1408, 1, '2021-07-11 15:16:12', '100001010b005b06e43f', 'KUW 8 6816', '', 1, 0, 30116, 1),
(1409, 1, '2021-07-11 15:16:31', '100001010b005b06e452', 'KUW 4 424', '', 1, 0, 37835, 1),
(1410, 1, '2021-07-11 15:16:50', '100001010b005b06e465', 'KUW 1 2243', '', 1, 0, 50706, 1),
(1411, 1, '2021-07-11 15:17:08', '100001010b005b06e478', 'KUW 5 8876', '', 1, 0, 31254, 1),
(1412, 1, '2021-07-11 15:17:27', '100001010b005b06e48b', 'KUW 6 2957', '', 1, 0, 41975, 1),
(1413, 1, '2021-07-11 15:17:46', '100001010b005b06e49e', 'KUW 11 2122', '', 1, 0, 29874, 1),
(1414, 1, '2021-07-11 15:18:05', '100001010b005b06e4b1', 'KUW 1 6784', '', 1, 0, 1776, 1),
(1415, 1, '2021-07-11 15:18:24', '100001010b005b06e4c3', 'KUW 4 6515', '', 1, 0, 32694, 1),
(1416, 1, '2021-07-11 15:18:43', '100001010b005b06e4d6', 'KUW 14 3588', '', 1, 0, 31338, 1),
(1417, 1, '2021-07-11 15:19:01', '100001010b005b06e4e9', 'KUW 11 5236', '', 1, 0, 780, 1),
(1418, 1, '2021-07-11 15:19:20', '100001010b005b06e4fc', 'KUW 9 235', '', 1, 0, 13489, 1),
(1419, 1, '2021-07-11 15:19:39', '100001010b005b06e50f', 'KUW 10 3964', '', 1, 0, 26785, 1),
(1420, 1, '2021-07-11 15:19:58', '100001010b005b06e521', 'KUW 10 6445', '', 1, 0, 36046, 1),
(1421, 1, '2021-07-11 15:20:17', '100001010b005b06e534', 'KUW 14 567', '', 1, 0, 32840, 1),
(1422, 1, '2021-07-11 15:20:35', '100001010b005b06e547', 'KUW 1 8300', '', 1, 0, 34556, 1),
(1423, 1, '2021-07-11 15:20:54', '100001010b005b06e55a', 'KUW 3 4957', '', 1, 0, 48145, 1),
(1424, 1, '2021-07-11 15:21:13', '100001010b005b06e56d', 'KUW 7 6857', '', 1, 0, 51301, 1),
(1425, 1, '2021-07-11 15:21:32', '100001010b005b06e580', 'KUW 12 4822', '', 1, 0, 50457, 1),
(1426, 1, '2021-07-11 15:21:51', '100001010b005b06e593', 'KUW 2 9716', '', 1, 0, 2219, 1),
(1427, 1, '2021-07-11 15:22:10', '100001010b005b06e5a5', 'KUW 4 4918', '', 1, 0, 14851, 1),
(1428, 1, '2021-07-11 15:22:29', '100001010b005b06e5b8', 'KUW 12 9114', '', 1, 0, 6221, 1),
(1429, 1, '2021-07-11 15:22:47', '100001010b005b06e5cb', 'KUW 14 8047', '', 1, 0, 47560, 1),
(1430, 1, '2021-07-11 15:23:06', '100001010b005b06e5de', 'KUW 15 2361', '', 1, 0, 29140, 1),
(1431, 1, '2021-07-11 15:23:25', '100001010b005b06e5f1', 'KUW 8 8416', '', 1, 0, 17293, 1),
(1432, 1, '2021-07-11 15:23:44', '100001010b005b06e603', 'KUW 4 200', '', 1, 0, 22489, 1),
(1433, 1, '2021-07-11 15:24:03', '100001010b005b06e616', 'KUW 8 4020', '', 1, 0, 42141, 1),
(1434, 1, '2021-07-11 15:24:21', '100001010b005b06e629', 'KUW 6 686', '', 1, 0, 40945, 1),
(1435, 1, '2021-07-11 15:24:40', '100001010b005b06e63c', 'KUW 9 542', '', 1, 0, 26090, 1),
(1436, 1, '2021-07-11 15:24:59', '100001010b005b06e64f', 'KUW 9 4553', '', 1, 0, 16843, 1),
(1437, 1, '2021-07-11 15:25:18', '100001010b005b06e662', 'KUW 8 3982', '', 1, 0, 19429, 1),
(1438, 1, '2021-07-11 15:25:37', '100001010b005b06e675', 'KUW 8 6128', '', 1, 0, 18139, 1),
(1439, 1, '2021-07-11 15:25:56', '100001010b005b06e687', 'KUW 14 5605', '', 1, 0, 27139, 1),
(1440, 1, '2021-07-11 15:26:14', '100001010b005b06e69a', 'KUW 2 6439', '', 1, 0, 2307, 1),
(1441, 1, '2021-07-11 15:26:33', '100001010b005b06e6ad', 'KUW 2 4112', '', 1, 0, 16861, 1),
(1442, 1, '2021-07-11 15:26:52', '100001010b005b06e6c0', 'KUW 1 412', '', 1, 0, 20201, 1),
(1443, 1, '2021-07-11 15:27:11', '100001010b005b06e6d3', 'KUW 1 9592', '', 1, 0, 51725, 1),
(1444, 1, '2021-07-11 15:27:30', '100001010b005b06e6e5', 'KUW 15 1202', '', 1, 0, 28050, 1),
(1445, 1, '2021-07-11 15:27:49', '100001010b005b06e6f8', 'KUW 7 3184', '', 1, 0, 24954, 1),
(1446, 1, '2021-07-11 15:28:08', '100001010b005b06e70b', 'KUW 8 8865', '', 1, 0, 51660, 1),
(1447, 1, '2021-07-11 15:28:26', '100001010b005b06e71e', 'KUW 11 3501', '', 1, 0, 13512, 1),
(1448, 1, '2021-07-11 15:28:45', '100001010b005b06e731', 'KUW 10 4863', '', 1, 0, 43654, 1),
(1449, 1, '2021-07-11 15:29:04', '100001010b005b06e744', 'KUW 4 3789', '', 1, 0, 34234, 1),
(1450, 1, '2021-07-11 15:29:23', '100001010b005b06e757', 'KUW 7 5383', '', 1, 0, 41767, 1),
(1451, 1, '2021-07-11 15:29:42', '100001010b005b06e76a', 'KUW 7 1295', '', 1, 0, 30507, 1),
(1452, 1, '2021-07-11 15:30:01', '100001010b005b06e77c', 'KUW 9 4290', '', 1, 0, 42997, 1),
(1453, 1, '2021-07-11 15:30:19', '100001010b005b06e78f', 'KUW 11 1115', '', 1, 0, 33996, 1),
(1454, 1, '2021-07-11 15:30:38', '100001010b005b06e7a2', 'KUW 2 7981', '', 1, 0, 33480, 1),
(1455, 1, '2021-07-11 15:30:57', '100001010b005b06e7b5', 'KUW 14 1912', '', 1, 0, 11662, 1),
(1456, 1, '2021-07-11 15:31:16', '100001010b005b06e7c8', 'KUW 2 2899', '', 1, 0, 51433, 1),
(1457, 1, '2021-07-11 15:31:35', '100001010b005b06e7da', 'KUW 7 830', '', 1, 0, 45451, 1);
INSERT INTO `parking_movements` (`id`, `movement_type`, `date_time`, `ticket_id`, `plate_number`, `chip_utid`, `upload_status_server1`, `upload_status_server2`, `plate_captured_id`, `cloud_upload_status`) VALUES
(1458, 1, '2021-07-11 15:31:54', '100001010b005b06e7ed', 'KUW 3 4762', '', 1, 0, 9988, 1),
(1459, 1, '2021-07-11 15:32:12', '100001010b005b06e800', 'KUW 15 549', '', 1, 0, 5427, 1),
(1460, 1, '2021-07-11 15:32:31', '100001010b005b06e813', 'KUW 2 7457', '', 1, 0, 18259, 1),
(1461, 1, '2021-07-11 15:32:50', '100001010b005b06e826', 'KUW 11 8164', '', 1, 0, 31402, 1),
(1462, 1, '2021-07-11 15:33:09', '100001010b005b06e839', 'KUW 5 7994', '', 1, 0, 7826, 1),
(1463, 1, '2021-07-11 15:33:28', '100001010b005b06e84b', 'KUW 3 1089', '', 1, 0, 5123, 1),
(1464, 1, '2021-07-11 15:33:47', '100001010b005b06e85e', 'KUW 3 2203', '', 1, 0, 24695, 1),
(1465, 1, '2021-07-11 15:34:05', '100001010b005b06e871', 'KUW 6 1530', '', 1, 0, 47206, 1),
(1466, 1, '2021-07-11 15:34:24', '100001010b005b06e884', 'KUW 7 6991', '', 1, 0, 26359, 1),
(1467, 1, '2021-07-11 15:34:43', '100001010b005b06e897', 'KUW 12 8859', '', 1, 0, 49612, 1),
(1468, 1, '2021-07-11 15:35:02', '100001010b005b06e8aa', 'KUW 1 7870', '', 1, 0, 42635, 1),
(1469, 1, '2021-07-11 15:35:21', '100001010b005b06e8bd', 'KUW 7 4046', '', 1, 0, 28437, 1),
(1470, 1, '2021-07-11 15:35:40', '100001010b005b06e8cf', 'KUW 2 2989', '', 1, 0, 37531, 1),
(1471, 1, '2021-07-11 15:35:59', '100001010b005b06e8e2', 'KUW 12 353', '', 1, 0, 1569, 1),
(1472, 1, '2021-07-11 15:36:17', '100001010b005b06e8f5', 'KUW 5 7160', '', 1, 0, 14360, 1),
(1473, 1, '2021-07-11 15:36:36', '100001010b005b06e908', 'KUW 6 4984', '', 1, 0, 8168, 1),
(1474, 1, '2021-07-11 15:36:55', '100001010b005b06e91b', 'KUW 13 3372', '', 1, 0, 12966, 1),
(1475, 1, '2021-07-11 15:37:14', '100001010b005b06e92e', 'KUW 1 9924', '', 1, 0, 42216, 1),
(1476, 1, '2021-07-11 15:37:33', '100001010b005b06e941', 'KUW 11 2657', '', 1, 0, 41215, 1),
(1477, 1, '2021-07-11 15:37:52', '100001010b005b06e953', 'KUW 8 2040', '', 1, 0, 44328, 1),
(1478, 1, '2021-07-11 15:38:11', '100001010b005b06e966', 'KUW 2 6057', '', 1, 0, 31174, 1),
(1479, 1, '2021-07-11 15:38:29', '100001010b005b06e979', 'KUW 1 1569', '', 1, 0, 20030, 1),
(1480, 1, '2021-07-11 15:38:48', '100001010b005b06e98c', 'KUW 10 8116', '', 1, 0, 39166, 1),
(1481, 1, '2021-07-11 15:39:07', '100001010b005b06e99f', 'KUW 6 3260', '', 1, 0, 17590, 1),
(1482, 1, '2021-07-11 15:39:26', '100001010b005b06e9b2', 'KUW 5 1914', '', 1, 0, 19419, 1),
(1483, 1, '2021-07-11 15:39:45', '100001010b005b06e9c5', 'KUW 14 6028', '', 1, 0, 14840, 1),
(1484, 1, '2021-07-11 15:40:04', '100001010b005b06e9d7', 'KUW 15 6758', '', 1, 0, 37367, 1),
(1485, 1, '2021-07-11 15:40:22', '100001010b005b06e9ea', 'KUW 11 8757', '', 1, 0, 26815, 1),
(1486, 1, '2021-07-11 15:40:41', '100001010b005b06e9fd', 'KUW 13 4454', '', 1, 0, 13933, 1),
(1487, 1, '2021-07-11 15:41:00', '100001010b005b06ea10', 'KUW 8 5540', '', 1, 0, 43732, 1),
(1488, 1, '2021-07-11 15:41:19', '100001010b005b06ea23', 'KUW 1 3059', '', 1, 0, 35572, 1),
(1489, 1, '2021-07-11 15:41:38', '100001010b005b06ea36', 'KUW 13 9326', '', 1, 0, 39046, 1),
(1490, 1, '2021-07-11 15:41:57', '100001010b005b06ea48', 'KUW 9 6637', '', 1, 0, 32894, 1),
(1491, 1, '2021-07-11 15:42:15', '100001010b005b06ea5b', 'KUW 12 7611', '', 1, 0, 13666, 1),
(1492, 1, '2021-07-11 15:42:34', '100001010b005b06ea6e', 'KUW 2 8275', '', 1, 0, 25713, 1),
(1493, 1, '2021-07-11 15:42:53', '100001010b005b06ea81', 'KUW 13 4486', '', 1, 0, 49283, 1),
(1494, 1, '2021-07-11 15:43:12', '100001010b005b06ea94', 'KUW 4 5525', '', 1, 0, 29139, 1),
(1495, 1, '2021-07-11 15:43:31', '100001010b005b06eaa6', 'KUW 6 9954', '', 1, 0, 36191, 1),
(1496, 1, '2021-07-11 15:43:50', '100001010b005b06eaba', 'KUW 15 7487', '', 1, 0, 22491, 1),
(1497, 1, '2021-07-11 15:44:09', '100001010b005b06eacc', 'KUW 4 9229', '', 1, 0, 11672, 1),
(1498, 1, '2021-07-11 15:44:28', '100001010b005b06eadf', 'KUW 14 5152', '', 1, 0, 16840, 1),
(1499, 1, '2021-07-11 15:44:46', '100001010b005b06eaf2', 'KUW 6 4370', '', 1, 0, 934, 1),
(1500, 1, '2021-07-11 15:45:05', '100001010b005b06eb05', 'KUW 7 5412', '', 1, 0, 2790, 1),
(1501, 1, '2021-07-11 15:45:24', '100001010b005b06eb18', 'KUW 10 1124', '', 1, 0, 33344, 1);

-- --------------------------------------------------------

--
-- Table structure for table `parking_movements_access`
--

CREATE TABLE `parking_movements_access` (
  `id` int(11) NOT NULL,
  `movement_type` tinyint(1) NOT NULL DEFAULT 0,
  `date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `ticket_id` varchar(100) DEFAULT '000',
  `plate_number` varchar(25) DEFAULT NULL,
  `tag` tinytext NOT NULL,
  `tag_serial` varchar(50) DEFAULT NULL,
  `status_message` text NOT NULL,
  `tid` tinytext DEFAULT NULL,
  `epc` tinytext DEFAULT NULL,
  `epc_list` varchar(20) DEFAULT NULL,
  `epc_count` int(11) NOT NULL DEFAULT 0,
  `upload_status_server1` int(11) NOT NULL DEFAULT 0,
  `upload_status_server2` int(11) NOT NULL DEFAULT 0,
  `plate_captured_id` int(11) NOT NULL DEFAULT 0,
  `cloud_upload_status` int(11) DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `parking_movements_access`
--

INSERT INTO `parking_movements_access` (`id`, `movement_type`, `date_time`, `ticket_id`, `plate_number`, `tag`, `tag_serial`, `status_message`, `tid`, `epc`, `epc_list`, `epc_count`, `upload_status_server1`, `upload_status_server2`, `plate_captured_id`, `cloud_upload_status`) VALUES
(3, 1, '2021-07-14 09:59:23', '20210219090505', 'KUW 1 123', '', '', 'Access Allowed', NULL, NULL, NULL, 0, 1, 0, 0, 1),
(4, 1, '2021-07-14 10:03:17', '20210219090505', 'KUW 1 123', '', '', 'Access Allowed', NULL, NULL, NULL, 0, 1, 0, 0, 1),
(5, 1, '2021-07-14 10:08:54', '20210219090505', 'KUW 1 123', '', '', 'Access Allowed', NULL, NULL, NULL, 0, 1, 0, 0, 1);

-- --------------------------------------------------------

--
-- Table structure for table `parking_movements_manual`
--

CREATE TABLE `parking_movements_manual` (
  `id` int(11) NOT NULL,
  `date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `operator_id` int(11) DEFAULT NULL,
  `operator_name` varchar(25) DEFAULT NULL,
  `action` varchar(50) NOT NULL,
  `reason` tinytext NOT NULL,
  `description` tinytext NOT NULL,
  `upload_status_server1` tinyint(4) NOT NULL DEFAULT 0,
  `upload_status_server2` tinyint(4) NOT NULL DEFAULT 0,
  `cloud_upload_status` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `parking_movements_manual`
--

INSERT INTO `parking_movements_manual` (`id`, `date_time`, `operator_id`, `operator_name`, `action`, `reason`, `description`, `upload_status_server1`, `upload_status_server2`, `cloud_upload_status`) VALUES
(1, '2021-07-15 10:00:00', 1, 'parcx', 'Barrier Open From Server', 'Reason', 'Description', 1, 0, 1);

-- --------------------------------------------------------

--
-- Table structure for table `parking_movements_reservation`
--

CREATE TABLE `parking_movements_reservation` (
  `id` int(11) NOT NULL,
  `ticket_id` varchar(50) NOT NULL,
  `movement_type` int(11) NOT NULL,
  `date_time` varchar(30) NOT NULL,
  `plate_number` varchar(50) DEFAULT NULL,
  `upload_status_server1` int(11) NOT NULL DEFAULT 0,
  `upload_status_server2` int(11) NOT NULL DEFAULT 0,
  `plate_captured_id` int(11) NOT NULL DEFAULT 0,
  `cloud_upload_status` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `parking_movements_reservation`
--

INSERT INTO `parking_movements_reservation` (`id`, `ticket_id`, `movement_type`, `date_time`, `plate_number`, `upload_status_server1`, `upload_status_server2`, `plate_captured_id`, `cloud_upload_status`) VALUES
(8, '000001010b0058fcd05e1', 1, '2021-07-14 10:22:34', 'DUB Q 9428', 1, 0, 0, 1);

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
  `upload_status_server1` int(11) NOT NULL DEFAULT 0,
  `upload_status_server2` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `parking_movement_exceptions`
--

INSERT INTO `parking_movement_exceptions` (`id`, `ticket_id`, `chip_utid`, `plate_number`, `exception`, `date_time`, `upload_status_server1`, `upload_status_server2`) VALUES
(1, '', '', 'KUW 5 8473', 'Shortterm Entry', '2021-07-08 12:06:08', 1, 0),
(2, '', '', 'AUH 7 53463', 'Shortterm Entry', '2021-07-08 12:06:50', 1, 0),
(3, '121212121', '', '', 'Access disabled.No ewallet record found', '2021-07-14 09:26:42', 1, 0),
(4, '3232323', '', '', 'Access disabled.No ewallet record found', '2021-07-14 09:28:13', 1, 0),
(5, 'allowed', '', '', 'Access disabled.No ewallet record found', '2021-07-14 09:32:43', 1, 0),
(6, '23232', '', '', 'Access disabled.No ewallet record found', '2021-07-14 09:35:09', 1, 0),
(7, '1212212', '', '', 'Access disabled.No ewallet record found', '2021-07-14 09:44:57', 1, 0),
(8, '121212121', '', '', 'Access disabled.No ewallet record found', '2021-07-14 09:45:42', 1, 0),
(9, '20210219090505', '', '', 'Access disabled.No ewallet record found', '2021-07-14 09:46:21', 1, 0);

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

--
-- Dumping data for table `parking_reservation`
--

INSERT INTO `parking_reservation` (`reservation_id`, `transaction_id`, `plate_number`, `ticket_id`, `name`, `address`, `country`, `phone`, `email`, `parking_bay_number`, `from_time`, `to_time`, `creation_date_time`, `reserved_parking_duration`, `reserved_parking_fee`, `payment_reference_id`, `payment_flag`, `cancel_flag`, `used_flag`, `cloud_upload_status`, `mobile_app_user_id`, `operator_id`) VALUES
(15, '000043352e31000031363035', 'OMN W2 65653', '', 'Test User', '', 'OMN', '561154055', 'parcx@gmail.com', '', '2020-09-23 10:00:00', '2020-09-23 12:00:00', '2020-09-20 07:38:33', '2H0M0S', '11', '', 0, 0, 0, 0, 130, 43),
(18, '22952583', 'AUH AS 1234', '22952583', 'nikhilvolga', '', 'UAE', '', 'jeenavolga@gmail.com', '', '2020-09-23 00:00:00', '2020-09-23 03:00:00', '2020-09-22 15:43:03', '3H0M0S', '0', '', 0, 0, 0, 0, 0, 1),
(21, '36064306', 'DXB H 95943', '36064306', 'test', '', 'UAE', '', 'test@gmail.com', '', '2021-02-21 09:51:00', '2021-02-21 10:54:00', '2021-02-21 09:51:46', '1H3M0S', '0', '', 0, 0, 0, 0, 0, 1),
(22, '898989', 'DUB Q 94284', '000001010b0058fcd05e1', 'Jude', '', '', '0490595577', 'judedcoutho1@gmail.com', 'P1', '2021-07-14 10:00:00', '2021-07-14 15:00:00', '2020-03-31 10:08:10', '', '10', '', 0, 0, 0, 1, 0, 62);

-- --------------------------------------------------------

--
-- Table structure for table `parking_validation`
--

CREATE TABLE `parking_validation` (
  `id` int(11) NOT NULL,
  `ticket_id` varchar(30) NOT NULL,
  `validation_value` int(11) NOT NULL,
  `validation_type` int(11) NOT NULL,
  `validation_provider` int(11) NOT NULL,
  `date_time` datetime NOT NULL,
  `upload_status_server1` int(11) NOT NULL DEFAULT 0,
  `upload_status_server2` int(11) NOT NULL DEFAULT 0
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
(1, 'access_whitelist', 'Access whitelist', 0, 30, 10, 1, '2021-07-18 06:59:15'),
(2, 'parking_validation', 'Validations', 0, 30, 5, 0, '2021-02-25 05:45:58'),
(3, 'parking_reservation', 'Reservations', 0, 30, 5, 1, '2021-07-15 11:32:16'),
(4, 'plates_ewallet', 'Plates E wallet', 1, 30, 5, 0, '2021-02-25 20:30:00'),
(5, 'parking_ewallet', 'Revenue E wallet', 1, 30, 5, 0, '2021-02-25 06:32:17'),
(6, 'cooperate_users', 'Corporate user', 1, 30, 5, 0, '2021-03-01 20:30:00');

-- --------------------------------------------------------

--
-- Table structure for table `pxcloud_upload_settings`
--

CREATE TABLE `pxcloud_upload_settings` (
  `id` int(11) NOT NULL,
  `table_name` varchar(50) NOT NULL,
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
(1, 'parking_movements_access', 'Access movements', 0, 30, 5, 8, 1, '2021-07-18 07:16:18'),
(2, 'parking_movements_manual', 'Manual movements', 0, 60, 5, 12, 1, '2021-07-15 07:48:16'),
(3, 'parking_movements_reservation', 'Reservation', 0, 60, 5, 18, 1, '2021-07-15 07:48:16'),
(17, 'parking_movements', 'Parking Movements', 0, 60, 5, 17, 1, '2021-07-18 07:04:07');

-- --------------------------------------------------------

--
-- Table structure for table `revenue_coupon_usage`
--

CREATE TABLE `revenue_coupon_usage` (
  `id` int(11) NOT NULL,
  `coupon_id` text NOT NULL,
  `ticket_id` varchar(50) NOT NULL,
  `date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `upload_status_server1` int(1) NOT NULL DEFAULT 0,
  `upload_status_server` int(1) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

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
-- Table structure for table `watchdog_device_alarms`
--

CREATE TABLE `watchdog_device_alarms` (
  `id` int(11) NOT NULL,
  `alarm_date_time` datetime NOT NULL DEFAULT current_timestamp(),
  `alarm` varchar(50) NOT NULL DEFAULT 'Alarm',
  `alarm_description` tinytext NOT NULL,
  `alarm_severity` int(2) NOT NULL,
  `alarm_code` varchar(10) NOT NULL,
  `upload_status_server1` int(11) NOT NULL DEFAULT 0,
  `upload_status_server2` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `watchdog_device_alarms`
--

INSERT INTO `watchdog_device_alarms` (`id`, `alarm_date_time`, `alarm`, `alarm_description`, `alarm_severity`, `alarm_code`, `upload_status_server1`, `upload_status_server2`) VALUES
(124, '2021-07-08 11:09:35', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(125, '2021-07-08 11:13:17', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(126, '2021-07-08 11:17:24', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(127, '2021-07-08 11:18:20', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(128, '2021-07-08 11:26:15', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(129, '2021-07-08 12:05:42', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(130, '2021-07-08 12:08:20', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(131, '2021-07-08 12:47:59', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(132, '2021-07-08 12:54:14', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(133, '2021-07-08 13:02:35', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(134, '2021-07-08 13:02:48', 'Barrier quick close on timeout', 'Barrier quick close on timeout', 7, '1231', 1, 0),
(135, '2021-07-08 13:02:48', 'Barrier Closed on timout', 'Barrier open for seconds =11 Limit=10', 7, '1121', 1, 0),
(136, '2021-07-11 08:48:15', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(137, '2021-07-11 13:14:23', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(138, '2021-07-11 14:31:19', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(139, '2021-07-11 14:31:25', 'Barrier quick close on timeout', 'Barrier quick close on timeout', 7, '1231', 1, 0),
(140, '2021-07-11 14:31:25', 'Barrier Closed on timout', 'Barrier open for seconds =4 Limit=10', 7, '1121', 1, 0),
(141, '2021-07-11 15:45:50', 'Barrier quick close on timeout', 'Barrier quick close on timeout', 7, '1231', 1, 0),
(142, '2021-07-11 15:45:50', 'Barrier Closed on timout', 'Barrier open for seconds =11 Limit=10', 7, '1121', 1, 0),
(143, '2021-07-14 08:51:11', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(144, '2021-07-14 08:56:02', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(145, '2021-07-14 08:57:50', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(146, '2021-07-14 09:23:07', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(147, '2021-07-14 09:26:17', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(148, '2021-07-14 09:34:40', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(149, '2021-07-14 09:36:46', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(150, '2021-07-14 09:39:33', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(151, '2021-07-14 09:42:06', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(152, '2021-07-14 09:44:38', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(153, '2021-07-14 09:56:19', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(154, '2021-07-14 09:59:02', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(155, '2021-07-14 10:02:46', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(156, '2021-07-14 10:08:30', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(157, '2021-07-14 10:22:14', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(158, '2021-07-14 15:49:06', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0),
(159, '2021-07-15 08:31:47', 'Local network connection available', 'Network status Online', 9, '1022', 1, 0);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `access_whitelist`
--
ALTER TABLE `access_whitelist`
  ADD PRIMARY KEY (`access_whitelist_id`);

--
-- Indexes for table `access_whitelist_bak`
--
ALTER TABLE `access_whitelist_bak`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `customer_messages`
--
ALTER TABLE `customer_messages`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `customer_messages2`
--
ALTER TABLE `customer_messages2`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `daemon_status`
--
ALTER TABLE `daemon_status`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `device_maintenance_terminal`
--
ALTER TABLE `device_maintenance_terminal`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `device_settings`
--
ALTER TABLE `device_settings`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `device_users`
--
ALTER TABLE `device_users`
  ADD PRIMARY KEY (`user_id`);

--
-- Indexes for table `live_device_alarms`
--
ALTER TABLE `live_device_alarms`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `parking_blacklist`
--
ALTER TABLE `parking_blacklist`
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
-- Indexes for table `revenue_coupon_usage`
--
ALTER TABLE `revenue_coupon_usage`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `vms_communication_config`
--
ALTER TABLE `vms_communication_config`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `watchdog_device_alarms`
--
ALTER TABLE `watchdog_device_alarms`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `access_whitelist`
--
ALTER TABLE `access_whitelist`
  MODIFY `access_whitelist_id` int(25) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=18;
--
-- AUTO_INCREMENT for table `access_whitelist_bak`
--
ALTER TABLE `access_whitelist_bak`
  MODIFY `id` int(25) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=437;
--
-- AUTO_INCREMENT for table `customer_messages`
--
ALTER TABLE `customer_messages`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=102;
--
-- AUTO_INCREMENT for table `customer_messages2`
--
ALTER TABLE `customer_messages2`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=102;
--
-- AUTO_INCREMENT for table `daemon_status`
--
ALTER TABLE `daemon_status`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=13;
--
-- AUTO_INCREMENT for table `device_maintenance_terminal`
--
ALTER TABLE `device_maintenance_terminal`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=33;
--
-- AUTO_INCREMENT for table `device_settings`
--
ALTER TABLE `device_settings`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=169;
--
-- AUTO_INCREMENT for table `device_users`
--
ALTER TABLE `device_users`
  MODIFY `user_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=48;
--
-- AUTO_INCREMENT for table `live_device_alarms`
--
ALTER TABLE `live_device_alarms`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=72;
--
-- AUTO_INCREMENT for table `parking_blacklist`
--
ALTER TABLE `parking_blacklist`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;
--
-- AUTO_INCREMENT for table `parking_movements`
--
ALTER TABLE `parking_movements`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1502;
--
-- AUTO_INCREMENT for table `parking_movements_access`
--
ALTER TABLE `parking_movements_access`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;
--
-- AUTO_INCREMENT for table `parking_movements_manual`
--
ALTER TABLE `parking_movements_manual`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;
--
-- AUTO_INCREMENT for table `parking_movements_reservation`
--
ALTER TABLE `parking_movements_reservation`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;
--
-- AUTO_INCREMENT for table `parking_movement_exceptions`
--
ALTER TABLE `parking_movement_exceptions`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=10;
--
-- AUTO_INCREMENT for table `parking_reservation`
--
ALTER TABLE `parking_reservation`
  MODIFY `reservation_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=23;
--
-- AUTO_INCREMENT for table `parking_validation`
--
ALTER TABLE `parking_validation`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=11;
--
-- AUTO_INCREMENT for table `pxcloud_download_settings`
--
ALTER TABLE `pxcloud_download_settings`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;
--
-- AUTO_INCREMENT for table `pxcloud_upload_settings`
--
ALTER TABLE `pxcloud_upload_settings`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=18;
--
-- AUTO_INCREMENT for table `revenue_coupon_usage`
--
ALTER TABLE `revenue_coupon_usage`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `vms_communication_config`
--
ALTER TABLE `vms_communication_config`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;
--
-- AUTO_INCREMENT for table `watchdog_device_alarms`
--
ALTER TABLE `watchdog_device_alarms`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=160;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
