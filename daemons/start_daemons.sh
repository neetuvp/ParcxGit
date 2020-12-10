#!/bin/bash
cd /etc/parcx/
sleep 10
./PX_ParkingDuration_DC
./PX_ParkingMovements_DC
./PX_RevenueSummary_DC
./PX_RevenueSummary_RT
./PX_DeviceWatchDog_RT
./PX_Counters_RT_Occupancy
