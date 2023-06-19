-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Jun 16, 2023 at 07:05 PM
-- Server version: 10.4.28-MariaDB
-- PHP Version: 8.0.28

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `esp32_mc_db`
--

-- --------------------------------------------------------

--
-- Table structure for table `esp32_table_dht11_leds_record`
--

CREATE TABLE `esp32_table_dht11_leds_record` (
  `id` varchar(255) NOT NULL,
  `board` varchar(255) NOT NULL,
  `temperature` float(10,2) NOT NULL,
  `humidity` int(3) NOT NULL,
  `soil` int(3) NOT NULL,
  `status_read_sensor_dht11` varchar(255) NOT NULL,
  `LED_01` varchar(255) NOT NULL,
  `LED_02` varchar(255) NOT NULL,
  `time` time NOT NULL,
  `date` date NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

--
-- Dumping data for table `esp32_table_dht11_leds_record`
--

-- --------------------------------------------------------

--
-- Table structure for table `esp32_table_dht11_leds_update`
--

CREATE TABLE `esp32_table_dht11_leds_update` (
  `id` varchar(255) NOT NULL,
  `temperature` float(10,2) NOT NULL,
  `humidity` int(3) NOT NULL,
  `soil` int(3) NOT NULL,
  `status_read_sensor_dht11` varchar(255) NOT NULL,
  `LED_01` varchar(255) NOT NULL,
  `LED_02` varchar(255) NOT NULL,
  `time` time NOT NULL,
  `date` date NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

--
-- Dumping data for table `esp32_table_dht11_leds_update`
--


-- --------------------------------------------------------

--
-- Table structure for table `esp32_table_test`
--

CREATE TABLE `esp32_table_test` (
  `id` varchar(255) NOT NULL,
  `temperature` float(10,2) NOT NULL,
  `humidity` int(3) NOT NULL,
  `soil` int(3) NOT NULL,
  `status_read_sensor_dht11` varchar(255) NOT NULL,
  `LED_01` varchar(255) NOT NULL,
  `LED_02` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

--
-- Dumping data for table `esp32_table_test`
--

--
-- Indexes for dumped tables
--

--
-- Indexes for table `esp32_table_dht11_leds_record`
--
ALTER TABLE `esp32_table_dht11_leds_record`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `esp32_table_dht11_leds_update`
--
ALTER TABLE `esp32_table_dht11_leds_update`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `esp32_table_test`
--
ALTER TABLE `esp32_table_test`
  ADD PRIMARY KEY (`id`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
