EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 4
Title "Address Lines Read Register"
Date "2020-03-01"
Rev "1"
Comp "Joshua Scoggins"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 4950 900  2    50   Output ~ 0
D0
Wire Wire Line
	4950 900  4250 900 
Wire Wire Line
	4250 1000 4950 1000
Text HLabel 4950 1000 2    50   Output ~ 0
AD1
Text HLabel 4950 1100 2    50   Output ~ 0
AD2
Text HLabel 4950 1200 2    50   Output ~ 0
AD3
Text HLabel 4950 1300 2    50   Output ~ 0
AD4
Text HLabel 4950 1400 2    50   Output ~ 0
AD5
Text HLabel 4950 1500 2    50   Output ~ 0
AD6
Text HLabel 4950 1600 2    50   Output ~ 0
AD7
Wire Wire Line
	4950 1100 4250 1100
Wire Wire Line
	4250 1200 4950 1200
Wire Wire Line
	4950 1300 4250 1300
Wire Wire Line
	4950 1400 4250 1400
Wire Wire Line
	4250 1500 4950 1500
Wire Wire Line
	4950 1600 4250 1600
Text HLabel 4950 2800 2    50   Output ~ 0
AD8
Text HLabel 4950 2900 2    50   Output ~ 0
AD9
Text HLabel 4950 3000 2    50   Output ~ 0
AD10
Text HLabel 4950 3100 2    50   Output ~ 0
AD11
Text HLabel 4950 3200 2    50   Output ~ 0
AD12
Text HLabel 4950 3300 2    50   Output ~ 0
AD13
Text HLabel 4950 3400 2    50   Output ~ 0
AD14
Text HLabel 4950 3500 2    50   Output ~ 0
AD15
Wire Wire Line
	4950 3500 4250 3500
Wire Wire Line
	4250 3400 4950 3400
Wire Wire Line
	4950 2800 4250 2800
Wire Wire Line
	4250 2900 4950 2900
Wire Wire Line
	4950 3000 4250 3000
Wire Wire Line
	4250 3100 4950 3100
Wire Wire Line
	4950 3200 4250 3200
Wire Wire Line
	4250 3300 4950 3300
$Comp
L 74xx:74HC595 U?
U 1 1 5E67AF56
P 3850 1300
F 0 "U?" H 3850 2081 50  0000 C CNN
F 1 "74HC595" H 3850 1990 50  0000 C CNN
F 2 "" H 3850 1300 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74hc595.pdf" H 3850 1300 50  0001 C CNN
	1    3850 1300
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74HC595 U?
U 1 1 5E67D3F0
P 3850 3200
F 0 "U?" H 3850 3981 50  0000 C CNN
F 1 "74HC595" H 3850 3890 50  0000 C CNN
F 2 "" H 3850 3200 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74hc595.pdf" H 3850 3200 50  0001 C CNN
	1    3850 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 1800 4950 1800
Text Label 4950 1800 0    50   ~ 0
NEXT
Wire Wire Line
	3450 2800 2950 2800
Text Label 2950 2800 0    50   ~ 0
NEXT
NoConn ~ 4250 3700
Wire Wire Line
	3450 900  2500 900 
Text HLabel 2500 900  0    50   Input ~ 0
SERIAL_IN
Wire Wire Line
	3450 1100 2500 1100
Wire Wire Line
	3450 1200 2500 1200
Wire Wire Line
	3450 1400 2500 1400
Wire Wire Line
	3450 1500 2500 1500
Text HLabel 2500 1100 0    50   Input ~ 0
SRCLK
Text HLabel 2500 1200 0    50   Input ~ 0
~SRCLR
Text HLabel 2500 1400 0    50   Input ~ 0
RCLK
Text HLabel 2500 1500 0    50   Input ~ 0
~OE
Wire Wire Line
	3450 3000 2500 3000
Wire Wire Line
	3450 3100 2500 3100
Wire Wire Line
	3450 3300 2500 3300
Wire Wire Line
	3450 3400 2500 3400
Text HLabel 2500 3000 0    50   Input ~ 0
SRCLK
Text HLabel 2500 3100 0    50   Input ~ 0
~SRCLR
Text HLabel 2500 3300 0    50   Input ~ 0
RCLK
Text HLabel 2500 3400 0    50   Input ~ 0
~OE
$Comp
L Device:C C?
U 1 1 5E685FC5
P 6050 3000
F 0 "C?" H 6165 3046 50  0000 L CNN
F 1 "0.1uF" H 6165 2955 50  0000 L CNN
F 2 "" H 6088 2850 50  0001 C CNN
F 3 "~" H 6050 3000 50  0001 C CNN
	1    6050 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 2850 6050 2500
Wire Wire Line
	6050 2500 3850 2500
Wire Wire Line
	3850 2500 3850 2600
Wire Wire Line
	6050 3150 6050 4150
Wire Wire Line
	6050 4150 3850 4150
Wire Wire Line
	3850 4150 3850 3900
$Comp
L power:+5V #PWR?
U 1 1 5E68749B
P 6050 2500
F 0 "#PWR?" H 6050 2350 50  0001 C CNN
F 1 "+5V" H 6065 2673 50  0000 C CNN
F 2 "" H 6050 2500 50  0001 C CNN
F 3 "" H 6050 2500 50  0001 C CNN
	1    6050 2500
	1    0    0    -1  
$EndComp
Connection ~ 6050 2500
$Comp
L power:GND #PWR?
U 1 1 5E687897
P 6050 4150
F 0 "#PWR?" H 6050 3900 50  0001 C CNN
F 1 "GND" H 6055 3977 50  0000 C CNN
F 2 "" H 6050 4150 50  0001 C CNN
F 3 "" H 6050 4150 50  0001 C CNN
	1    6050 4150
	1    0    0    -1  
$EndComp
Connection ~ 6050 4150
$Comp
L Device:C C?
U 1 1 5E687B48
P 6050 1350
F 0 "C?" H 6165 1396 50  0000 L CNN
F 1 "0.1uF" H 6165 1305 50  0000 L CNN
F 2 "" H 6088 1200 50  0001 C CNN
F 3 "~" H 6050 1350 50  0001 C CNN
	1    6050 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 1200 6050 600 
Wire Wire Line
	6050 600  3850 600 
Wire Wire Line
	3850 600  3850 700 
Wire Wire Line
	3850 2000 3850 2100
Wire Wire Line
	3850 2100 6050 2100
Wire Wire Line
	6050 2100 6050 1500
$Comp
L power:+5V #PWR?
U 1 1 5E6896D5
P 6050 600
F 0 "#PWR?" H 6050 450 50  0001 C CNN
F 1 "+5V" V 6065 728 50  0000 L CNN
F 2 "" H 6050 600 50  0001 C CNN
F 3 "" H 6050 600 50  0001 C CNN
	1    6050 600 
	0    1    1    0   
$EndComp
Connection ~ 6050 600 
$Comp
L power:GND #PWR?
U 1 1 5E689AE2
P 6050 2100
F 0 "#PWR?" H 6050 1850 50  0001 C CNN
F 1 "GND" V 6055 1972 50  0000 R CNN
F 2 "" H 6050 2100 50  0001 C CNN
F 3 "" H 6050 2100 50  0001 C CNN
	1    6050 2100
	0    -1   -1   0   
$EndComp
Connection ~ 6050 2100
$EndSCHEMATC
