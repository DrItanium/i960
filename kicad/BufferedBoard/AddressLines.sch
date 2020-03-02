EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 5
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
AR Path="/5E5D51D3/5E65DE33/5E6626ED" Ref="U?"  Part="1" 
AR Path="/5E5D51D3/5E6A0ABB/5E6626ED" Ref="U?"  Part="1" 
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
$Comp
L dk_Logic-Shift-Registers:SN74HC165N U?
U 1 1 5E663DD0
P 2300 3650
AR Path="/5E5D51D3/5E65DE33/5E663DD0" Ref="U?"  Part="1" 
AR Path="/5E5D51D3/5E6A0ABB/5E663DD0" Ref="U?"  Part="1" 
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
Wire Wire Line
	1800 2050 1100 2050
Wire Wire Line
	1800 3950 1100 3950
Wire Wire Line
	1800 4050 1100 4050
Wire Wire Line
	1800 4250 1100 4250
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
AR Path="/5E5D51D3/5E65DE33/5E66B5EC" Ref="C?"  Part="1" 
AR Path="/5E5D51D3/5E6A0ABB/5E66B5EC" Ref="C?"  Part="1" 
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
AR Path="/5E5D51D3/5E65DE33/5E66C77C" Ref="C?"  Part="1" 
AR Path="/5E5D51D3/5E6A0ABB/5E66C77C" Ref="C?"  Part="1" 
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
AR Path="/5E5D51D3/5E65DE33/5E66E814" Ref="#PWR?"  Part="1" 
AR Path="/5E5D51D3/5E6A0ABB/5E66E814" Ref="#PWR?"  Part="1" 
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
AR Path="/5E5D51D3/5E65DE33/5E66EDF0" Ref="#PWR?"  Part="1" 
AR Path="/5E5D51D3/5E6A0ABB/5E66EDF0" Ref="#PWR?"  Part="1" 
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
AR Path="/5E5D51D3/5E65DE33/5E66F306" Ref="#PWR?"  Part="1" 
AR Path="/5E5D51D3/5E6A0ABB/5E66F306" Ref="#PWR?"  Part="1" 
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
AR Path="/5E5D51D3/5E65DE33/5E66F9C0" Ref="#PWR?"  Part="1" 
AR Path="/5E5D51D3/5E6A0ABB/5E66F9C0" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4100 2550 50  0001 C CNN
F 1 "+5V" V 4115 2828 50  0000 L CNN
F 2 "" H 4100 2700 50  0001 C CNN
F 3 "" H 4100 2700 50  0001 C CNN
	1    4100 2700
	0    1    1    0   
$EndComp
Connection ~ 4100 2700
$Comp
L dk_Logic-Shift-Registers:SN74HC165N U?
U 1 1 5E6B37D2
P 6750 1600
AR Path="/5E5D51D3/5E65DE33/5E6B37D2" Ref="U?"  Part="1" 
AR Path="/5E5D51D3/5E6A0ABB/5E6B37D2" Ref="U?"  Part="1" 
F 0 "U?" H 6650 2503 60  0000 C CNN
F 1 "SN74HC165N" H 6650 2397 60  0000 C CNN
F 2 "digikey-footprints:DIP-16_W7.62mm" H 6950 1800 60  0001 L CNN
F 3 "http://www.ti.com/general/docs/suppproductinfo.tsp?distId=10&gotoUrl=http%3A%2F%2Fwww.ti.com%2Flit%2Fgpn%2Fsn74hc165" H 6950 1900 60  0001 L CNN
F 4 "296-8251-5-ND" H 6950 2000 60  0001 L CNN "Digi-Key_PN"
F 5 "SN74HC165N" H 6950 2100 60  0001 L CNN "MPN"
F 6 "Integrated Circuits (ICs)" H 6950 2200 60  0001 L CNN "Category"
F 7 "Logic - Shift Registers" H 6950 2300 60  0001 L CNN "Family"
F 8 "http://www.ti.com/general/docs/suppproductinfo.tsp?distId=10&gotoUrl=http%3A%2F%2Fwww.ti.com%2Flit%2Fgpn%2Fsn74hc165" H 6950 2400 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/texas-instruments/SN74HC165N/296-8251-5-ND/376966" H 6950 2500 60  0001 L CNN "DK_Detail_Page"
F 10 "IC 8-BIT SHIFT REGISTER 16-DIP" H 6950 2600 60  0001 L CNN "Description"
F 11 "Texas Instruments" H 6950 2700 60  0001 L CNN "Manufacturer"
F 12 "Active" H 6950 2800 60  0001 L CNN "Status"
	1    6750 1600
	1    0    0    -1  
$EndComp
Text HLabel 5550 1100 0    50   Input ~ 0
A16
Text HLabel 5550 1200 0    50   Input ~ 0
A17
Text HLabel 5550 1300 0    50   Input ~ 0
A18
Text HLabel 5550 1400 0    50   Input ~ 0
A19
Text HLabel 5550 1500 0    50   Input ~ 0
A20
Text HLabel 5550 1600 0    50   Input ~ 0
A21
Text HLabel 5550 1700 0    50   Input ~ 0
A22
Text HLabel 5550 1800 0    50   Input ~ 0
A23
Wire Wire Line
	5550 1800 6250 1800
Wire Wire Line
	6250 1700 5550 1700
Wire Wire Line
	6250 2100 5550 2100
Text Label 5550 2100 0    50   ~ 0
HIGHER_OUT
NoConn ~ 7050 1400
Wire Wire Line
	7050 1300 7700 1300
Wire Wire Line
	6250 1900 5550 1900
Wire Wire Line
	6250 2000 5550 2000
Wire Wire Line
	6250 2200 5550 2200
Wire Wire Line
	5550 1100 6250 1100
Wire Wire Line
	6250 1200 5550 1200
Wire Wire Line
	5550 1300 6250 1300
Wire Wire Line
	6250 1400 5550 1400
Wire Wire Line
	5550 1500 6250 1500
Wire Wire Line
	6250 1600 5550 1600
Wire Wire Line
	6750 900  6750 650 
Wire Wire Line
	6750 650  8550 650 
Wire Wire Line
	6750 2400 6750 2750
Wire Wire Line
	6750 2750 8550 2750
Text HLabel 5550 1900 0    50   Input ~ 0
SH,~LD
Text HLabel 5550 2000 0    50   Input ~ 0
CLK
Text HLabel 5550 2200 0    50   Input ~ 0
~POPULATE
$Comp
L Device:C C?
U 1 1 5E6B37FB
P 8550 1650
AR Path="/5E5D51D3/5E65DE33/5E6B37FB" Ref="C?"  Part="1" 
AR Path="/5E5D51D3/5E6A0ABB/5E6B37FB" Ref="C?"  Part="1" 
F 0 "C?" H 8665 1696 50  0000 L CNN
F 1 "0.1uF" H 8665 1605 50  0000 L CNN
F 2 "" H 8588 1500 50  0001 C CNN
F 3 "~" H 8550 1650 50  0001 C CNN
	1    8550 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 650  8550 1500
Wire Wire Line
	8550 1800 8550 2750
$Comp
L power:GND #PWR?
U 1 1 5E6B3807
P 8550 2750
AR Path="/5E5D51D3/5E65DE33/5E6B3807" Ref="#PWR?"  Part="1" 
AR Path="/5E5D51D3/5E6A0ABB/5E6B3807" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8550 2500 50  0001 C CNN
F 1 "GND" H 8555 2577 50  0000 C CNN
F 2 "" H 8550 2750 50  0001 C CNN
F 3 "" H 8550 2750 50  0001 C CNN
	1    8550 2750
	1    0    0    -1  
$EndComp
Connection ~ 8550 2750
$Comp
L power:+5V #PWR?
U 1 1 5E6B3812
P 8550 650
AR Path="/5E5D51D3/5E65DE33/5E6B3812" Ref="#PWR?"  Part="1" 
AR Path="/5E5D51D3/5E6A0ABB/5E6B3812" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8550 500 50  0001 C CNN
F 1 "+5V" V 8565 778 50  0000 L CNN
F 2 "" H 8550 650 50  0001 C CNN
F 3 "" H 8550 650 50  0001 C CNN
	1    8550 650 
	0    1    1    0   
$EndComp
Connection ~ 8550 650 
Text Label 3250 3350 0    50   ~ 0
HIGHER_OUT
$Comp
L dk_Logic-Shift-Registers:SN74HC165N U?
U 1 1 5E6BB806
P 6750 4300
AR Path="/5E5D51D3/5E65DE33/5E6BB806" Ref="U?"  Part="1" 
AR Path="/5E5D51D3/5E6A0ABB/5E6BB806" Ref="U?"  Part="1" 
F 0 "U?" H 6650 5203 60  0000 C CNN
F 1 "SN74HC165N" H 6650 5097 60  0000 C CNN
F 2 "digikey-footprints:DIP-16_W7.62mm" H 6950 4500 60  0001 L CNN
F 3 "http://www.ti.com/general/docs/suppproductinfo.tsp?distId=10&gotoUrl=http%3A%2F%2Fwww.ti.com%2Flit%2Fgpn%2Fsn74hc165" H 6950 4600 60  0001 L CNN
F 4 "296-8251-5-ND" H 6950 4700 60  0001 L CNN "Digi-Key_PN"
F 5 "SN74HC165N" H 6950 4800 60  0001 L CNN "MPN"
F 6 "Integrated Circuits (ICs)" H 6950 4900 60  0001 L CNN "Category"
F 7 "Logic - Shift Registers" H 6950 5000 60  0001 L CNN "Family"
F 8 "http://www.ti.com/general/docs/suppproductinfo.tsp?distId=10&gotoUrl=http%3A%2F%2Fwww.ti.com%2Flit%2Fgpn%2Fsn74hc165" H 6950 5100 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/texas-instruments/SN74HC165N/296-8251-5-ND/376966" H 6950 5200 60  0001 L CNN "DK_Detail_Page"
F 10 "IC 8-BIT SHIFT REGISTER 16-DIP" H 6950 5300 60  0001 L CNN "Description"
F 11 "Texas Instruments" H 6950 5400 60  0001 L CNN "Manufacturer"
F 12 "Active" H 6950 5500 60  0001 L CNN "Status"
	1    6750 4300
	1    0    0    -1  
$EndComp
Text HLabel 5550 3800 0    50   Input ~ 0
A24
Text HLabel 5550 3900 0    50   Input ~ 0
A25
Text HLabel 5550 4000 0    50   Input ~ 0
A26
Text HLabel 5550 4100 0    50   Input ~ 0
A27
Text HLabel 5550 4200 0    50   Input ~ 0
A28
Text HLabel 5550 4300 0    50   Input ~ 0
A29
Text HLabel 5550 4400 0    50   Input ~ 0
A30
Text HLabel 5550 4500 0    50   Input ~ 0
A31
Wire Wire Line
	5550 4500 6250 4500
Wire Wire Line
	6250 4400 5550 4400
Wire Wire Line
	6250 4800 5550 4800
Text Label 5550 4800 0    50   ~ 0
HIGHEST_OUT
NoConn ~ 7050 4100
Wire Wire Line
	7050 4000 7700 4000
Text HLabel 7700 4000 2    50   Output ~ 0
AD_OUT
Wire Wire Line
	6250 4600 5550 4600
Wire Wire Line
	6250 4700 5550 4700
Wire Wire Line
	6250 4900 5550 4900
Wire Wire Line
	5550 3800 6250 3800
Wire Wire Line
	6250 3900 5550 3900
Wire Wire Line
	5550 4000 6250 4000
Wire Wire Line
	6250 4100 5550 4100
Wire Wire Line
	5550 4200 6250 4200
Wire Wire Line
	6250 4300 5550 4300
Wire Wire Line
	6750 3600 6750 3350
Wire Wire Line
	6750 3350 8550 3350
Wire Wire Line
	6750 5100 6750 5450
Wire Wire Line
	6750 5450 8550 5450
Text HLabel 5550 4600 0    50   Input ~ 0
SH,~LD
Text HLabel 5550 4700 0    50   Input ~ 0
CLK
Text HLabel 5550 4900 0    50   Input ~ 0
~POPULATE
$Comp
L Device:C C?
U 1 1 5E6BB82F
P 8550 4350
AR Path="/5E5D51D3/5E65DE33/5E6BB82F" Ref="C?"  Part="1" 
AR Path="/5E5D51D3/5E6A0ABB/5E6BB82F" Ref="C?"  Part="1" 
F 0 "C?" H 8665 4396 50  0000 L CNN
F 1 "0.1uF" H 8665 4305 50  0000 L CNN
F 2 "" H 8588 4200 50  0001 C CNN
F 3 "~" H 8550 4350 50  0001 C CNN
	1    8550 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 3350 8550 4200
Wire Wire Line
	8550 4500 8550 5450
$Comp
L power:GND #PWR?
U 1 1 5E6BB83B
P 8550 5450
AR Path="/5E5D51D3/5E65DE33/5E6BB83B" Ref="#PWR?"  Part="1" 
AR Path="/5E5D51D3/5E6A0ABB/5E6BB83B" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8550 5200 50  0001 C CNN
F 1 "GND" H 8555 5277 50  0000 C CNN
F 2 "" H 8550 5450 50  0001 C CNN
F 3 "" H 8550 5450 50  0001 C CNN
	1    8550 5450
	1    0    0    -1  
$EndComp
Connection ~ 8550 5450
$Comp
L power:+5V #PWR?
U 1 1 5E6BB846
P 8550 3350
AR Path="/5E5D51D3/5E65DE33/5E6BB846" Ref="#PWR?"  Part="1" 
AR Path="/5E5D51D3/5E6A0ABB/5E6BB846" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8550 3200 50  0001 C CNN
F 1 "+5V" V 8565 3478 50  0000 L CNN
F 2 "" H 8550 3350 50  0001 C CNN
F 3 "" H 8550 3350 50  0001 C CNN
	1    8550 3350
	0    1    1    0   
$EndComp
Connection ~ 8550 3350
Text Label 7700 1300 0    50   ~ 0
HIGHEST_OUT
Wire Wire Line
	1100 3150 1800 3150
Text HLabel 1100 3150 0    50   Input ~ 0
AD8
Wire Wire Line
	1100 1950 1800 1950
Wire Wire Line
	1800 1850 1100 1850
Wire Wire Line
	1100 1750 1800 1750
Wire Wire Line
	1100 1650 1800 1650
Wire Wire Line
	1800 1550 1100 1550
Wire Wire Line
	1100 1450 1800 1450
Text HLabel 1100 1950 0    50   Input ~ 0
AD7
Text HLabel 1100 1850 0    50   Input ~ 0
AD6
Text HLabel 1100 1750 0    50   Input ~ 0
AD5
Text HLabel 1100 1650 0    50   Input ~ 0
AD4
Text HLabel 1100 1550 0    50   Input ~ 0
AD3
Text HLabel 1100 1450 0    50   Input ~ 0
AD2
Text HLabel 1100 1350 0    50   Input ~ 0
AD1
Wire Wire Line
	1800 1350 1100 1350
Wire Wire Line
	1800 1250 1100 1250
$Comp
L power:GND #PWR?
U 1 1 5E6CE5B5
P 1100 1250
F 0 "#PWR?" H 1100 1000 50  0001 C CNN
F 1 "GND" H 1105 1077 50  0000 C CNN
F 2 "" H 1100 1250 50  0001 C CNN
F 3 "" H 1100 1250 50  0001 C CNN
	1    1100 1250
	-1   0    0    1   
$EndComp
$EndSCHEMATC
