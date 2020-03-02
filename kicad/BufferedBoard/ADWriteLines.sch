EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
Title ""
Date "2020-03-01"
Rev "1"
Comp "Joshua Scoggins"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L dk_Logic-Shift-Registers:SN74HC165N U?
U 1 1 5E6626ED
P 2300 1750
F 0 "U?" H 2200 2653 60  0000 C CNN
F 1 "SN74HC165N" H 2200 2547 60  0000 C CNN
F 2 "digikey-footprints:DIP-16_W7.62mm" H 2500 1950 60  0001 L CNN
F 3 "http://www.ti.com/general/docs/suppproductinfo.tsp?distId=10&gotoUrl=http%3A%2F%2Fwww.ti.com%2Flit%2Fgpn%2Fsn74hc165" H 2500 2050 60  0001 L CNN
F 4 "296-8251-5-ND" H 2500 2150 60  0001 L CNN "Digi-Key_PN"
F 5 "SN74HC165N" H 2500 2250 60  0001 L CNN "MPN"
F 6 "Integrated Circuits (ICs)" H 2500 2350 60  0001 L CNN "Category"
F 7 "Logic - Shift Registers" H 2500 2450 60  0001 L CNN "Family"
F 8 "http://www.ti.com/general/docs/suppproductinfo.tsp?distId=10&gotoUrl=http%3A%2F%2Fwww.ti.com%2Flit%2Fgpn%2Fsn74hc165" H 2500 2550 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/texas-instruments/SN74HC165N/296-8251-5-ND/376966" H 2500 2650 60  0001 L CNN "DK_Detail_Page"
F 10 "IC 8-BIT SHIFT REGISTER 16-DIP" H 2500 2750 60  0001 L CNN "Description"
F 11 "Texas Instruments" H 2500 2850 60  0001 L CNN "Manufacturer"
F 12 "Active" H 2500 2950 60  0001 L CNN "Status"
	1    2300 1750
	1    0    0    -1  
$EndComp
Text HLabel 1100 1250 0    50   Input ~ 0
D0
Wire Wire Line
	1100 1250 1800 1250
Wire Wire Line
	1800 1350 1100 1350
Text HLabel 1100 1350 0    50   Input ~ 0
AD1
Text HLabel 1100 1450 0    50   Input ~ 0
AD2
Text HLabel 1100 1550 0    50   Input ~ 0
AD3
Text HLabel 1100 1650 0    50   Input ~ 0
AD4
Text HLabel 1100 1750 0    50   Input ~ 0
AD5
Text HLabel 1100 1850 0    50   Input ~ 0
AD6
Text HLabel 1100 1950 0    50   Input ~ 0
AD7
Wire Wire Line
	1100 1450 1800 1450
Wire Wire Line
	1800 1550 1100 1550
Wire Wire Line
	1100 1650 1800 1650
Wire Wire Line
	1100 1750 1800 1750
Wire Wire Line
	1800 1850 1100 1850
Wire Wire Line
	1100 1950 1800 1950
$Comp
L dk_Logic-Shift-Registers:SN74HC165N U?
U 1 1 5E663DD0
P 2300 3650
F 0 "U?" H 2200 4553 60  0000 C CNN
F 1 "SN74HC165N" H 2200 4447 60  0000 C CNN
F 2 "digikey-footprints:DIP-16_W7.62mm" H 2500 3850 60  0001 L CNN
F 3 "http://www.ti.com/general/docs/suppproductinfo.tsp?distId=10&gotoUrl=http%3A%2F%2Fwww.ti.com%2Flit%2Fgpn%2Fsn74hc165" H 2500 3950 60  0001 L CNN
F 4 "296-8251-5-ND" H 2500 4050 60  0001 L CNN "Digi-Key_PN"
F 5 "SN74HC165N" H 2500 4150 60  0001 L CNN "MPN"
F 6 "Integrated Circuits (ICs)" H 2500 4250 60  0001 L CNN "Category"
F 7 "Logic - Shift Registers" H 2500 4350 60  0001 L CNN "Family"
F 8 "http://www.ti.com/general/docs/suppproductinfo.tsp?distId=10&gotoUrl=http%3A%2F%2Fwww.ti.com%2Flit%2Fgpn%2Fsn74hc165" H 2500 4450 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/texas-instruments/SN74HC165N/296-8251-5-ND/376966" H 2500 4550 60  0001 L CNN "DK_Detail_Page"
F 10 "IC 8-BIT SHIFT REGISTER 16-DIP" H 2500 4650 60  0001 L CNN "Description"
F 11 "Texas Instruments" H 2500 4750 60  0001 L CNN "Manufacturer"
F 12 "Active" H 2500 4850 60  0001 L CNN "Status"
	1    2300 3650
	1    0    0    -1  
$EndComp
Text HLabel 1100 3150 0    50   Input ~ 0
AD8
Text HLabel 1100 3250 0    50   Input ~ 0
AD9
Text HLabel 1100 3350 0    50   Input ~ 0
AD10
Text HLabel 1100 3450 0    50   Input ~ 0
AD11
Text HLabel 1100 3550 0    50   Input ~ 0
AD12
Text HLabel 1100 3650 0    50   Input ~ 0
AD13
Wire Wire Line
	2600 1450 3000 1450
Text HLabel 1100 3750 0    50   Input ~ 0
AD14
Text HLabel 1100 3850 0    50   Input ~ 0
AD15
Wire Wire Line
	1100 3850 1800 3850
Wire Wire Line
	1800 3750 1100 3750
Wire Wire Line
	1800 4150 1100 4150
Text Label 1100 4150 0    50   ~ 0
LOWER_OUT
Text Label 3000 1450 2    50   ~ 0
LOWER_OUT
NoConn ~ 1800 2250
NoConn ~ 2600 3450
NoConn ~ 2600 1550
Wire Wire Line
	2600 3350 3250 3350
Text HLabel 3250 3350 2    50   Output ~ 0
AD_OUT
Wire Wire Line
	1800 2050 1100 2050
Wire Wire Line
	1800 3950 1100 3950
Wire Wire Line
	1800 4050 1100 4050
Wire Wire Line
	1800 4250 1100 4250
Wire Wire Line
	1100 3150 1800 3150
Wire Wire Line
	1800 3250 1100 3250
Wire Wire Line
	1100 3350 1800 3350
Wire Wire Line
	1800 3450 1100 3450
Wire Wire Line
	1100 3550 1800 3550
Wire Wire Line
	1800 3650 1100 3650
Wire Wire Line
	1800 2150 1100 2150
Wire Wire Line
	1800 2350 1100 2350
Wire Wire Line
	2300 2950 2300 2700
Wire Wire Line
	2300 2700 4100 2700
Wire Wire Line
	2300 4450 2300 4800
Wire Wire Line
	2300 4800 4100 4800
Text HLabel 1100 3950 0    50   Input ~ 0
SH,~LD
Text HLabel 1100 2050 0    50   Input ~ 0
SH,~LD
Text HLabel 1100 2150 0    50   Input ~ 0
CLK
Text HLabel 1100 4050 0    50   Input ~ 0
CLK
Text HLabel 1100 2350 0    50   Input ~ 0
~POPULATE
Text HLabel 1100 4250 0    50   Input ~ 0
~POPULATE
$Comp
L Device:C C?
U 1 1 5E66B5EC
P 4100 3700
F 0 "C?" H 4215 3746 50  0000 L CNN
F 1 "0.1uF" H 4215 3655 50  0000 L CNN
F 2 "" H 4138 3550 50  0001 C CNN
F 3 "~" H 4100 3700 50  0001 C CNN
	1    4100 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 2700 4100 3550
Wire Wire Line
	4100 3850 4100 4800
$Comp
L Device:C C?
U 1 1 5E66C77C
P 4100 1650
F 0 "C?" H 4215 1696 50  0000 L CNN
F 1 "0.1uF" H 4215 1605 50  0000 L CNN
F 2 "" H 4138 1500 50  0001 C CNN
F 3 "~" H 4100 1650 50  0001 C CNN
	1    4100 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 1050 2300 900 
Wire Wire Line
	2300 900  4100 900 
Wire Wire Line
	4100 900  4100 1500
Wire Wire Line
	4100 2600 4100 1800
Wire Wire Line
	2300 2550 2300 2600
Wire Wire Line
	2300 2600 4100 2600
$Comp
L power:GND #PWR?
U 1 1 5E66E814
P 4100 4800
F 0 "#PWR?" H 4100 4550 50  0001 C CNN
F 1 "GND" H 4105 4627 50  0000 C CNN
F 2 "" H 4100 4800 50  0001 C CNN
F 3 "" H 4100 4800 50  0001 C CNN
	1    4100 4800
	1    0    0    -1  
$EndComp
Connection ~ 4100 4800
$Comp
L power:GND #PWR?
U 1 1 5E66EDF0
P 4100 2600
F 0 "#PWR?" H 4100 2350 50  0001 C CNN
F 1 "GND" V 4105 2472 50  0000 R CNN
F 2 "" H 4100 2600 50  0001 C CNN
F 3 "" H 4100 2600 50  0001 C CNN
	1    4100 2600
	0    -1   -1   0   
$EndComp
Connection ~ 4100 2600
$Comp
L power:+5V #PWR?
U 1 1 5E66F306
P 4100 900
F 0 "#PWR?" H 4100 750 50  0001 C CNN
F 1 "+5V" H 4115 1073 50  0000 C CNN
F 2 "" H 4100 900 50  0001 C CNN
F 3 "" H 4100 900 50  0001 C CNN
	1    4100 900 
	1    0    0    -1  
$EndComp
Connection ~ 4100 900 
$Comp
L power:+5V #PWR?
U 1 1 5E66F9C0
P 4100 2700
F 0 "#PWR?" H 4100 2550 50  0001 C CNN
F 1 "+5V" V 4115 2828 50  0000 L CNN
F 2 "" H 4100 2700 50  0001 C CNN
F 3 "" H 4100 2700 50  0001 C CNN
	1    4100 2700
	0    1    1    0   
$EndComp
Connection ~ 4100 2700
$EndSCHEMATC
