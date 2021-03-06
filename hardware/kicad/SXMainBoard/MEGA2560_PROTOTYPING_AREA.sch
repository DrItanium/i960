EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
Title "Arduino Mega 2560 Pin Breakout/Prototyping Area"
Date "2020-02-16"
Rev "1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 950  950  0    50   BiDi ~ 0
P0
Text HLabel 950  1050 0    50   BiDi ~ 0
P1
Text HLabel 950  1150 0    50   BiDi ~ 0
P2
Text HLabel 950  1250 0    50   BiDi ~ 0
P3
Text HLabel 950  1350 0    50   BiDi ~ 0
P4
Text HLabel 950  1450 0    50   BiDi ~ 0
P5
Text HLabel 950  1550 0    50   BiDi ~ 0
P6
Text HLabel 950  1650 0    50   BiDi ~ 0
P7
Text HLabel 950  1750 0    50   BiDi ~ 0
P8
Text HLabel 950  1850 0    50   Input ~ 0
MOSI
Text HLabel 950  1950 0    50   Output ~ 0
MISO
Text HLabel 950  2050 0    50   Input ~ 0
SCK
Text HLabel 950  2250 0    50   BiDi ~ 0
SDA
Text HLabel 950  2150 0    50   Input ~ 0
SCL
$Comp
L power:+3.3V #PWR065
U 1 1 5F6E10C9
P 950 2350
F 0 "#PWR065" H 950 2200 50  0001 C CNN
F 1 "+3.3V" V 965 2478 50  0000 L CNN
F 2 "" H 950 2350 50  0001 C CNN
F 3 "" H 950 2350 50  0001 C CNN
	1    950  2350
	0    -1   -1   0   
$EndComp
$Comp
L power:+5V #PWR071
U 1 1 5F6E1255
P 950 2950
F 0 "#PWR071" H 950 2800 50  0001 C CNN
F 1 "+5V" V 965 3078 50  0000 L CNN
F 2 "" H 950 2950 50  0001 C CNN
F 3 "" H 950 2950 50  0001 C CNN
	1    950  2950
	0    -1   -1   0   
$EndComp
$Comp
L power:GNDREF #PWR066
U 1 1 5F6E15E2
P 950 2450
F 0 "#PWR066" H 950 2200 50  0001 C CNN
F 1 "GNDREF" V 955 2322 50  0000 R CNN
F 2 "" H 950 2450 50  0001 C CNN
F 3 "" H 950 2450 50  0001 C CNN
	1    950  2450
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_01x24 J46
U 1 1 5F6E7123
P 1700 2050
F 0 "J46" H 1780 2042 50  0000 L CNN
F 1 "Conn_01x24" H 1780 1951 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x24_P2.54mm_Vertical" H 1700 2050 50  0001 C CNN
F 3 "~" H 1700 2050 50  0001 C CNN
	1    1700 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 950  950  950 
Wire Wire Line
	950  1050 1500 1050
Wire Wire Line
	1500 1150 950  1150
Wire Wire Line
	1500 1250 950  1250
Wire Wire Line
	950  1350 1500 1350
Wire Wire Line
	1500 1450 950  1450
Wire Wire Line
	950  1550 1500 1550
Wire Wire Line
	1500 1650 950  1650
Wire Wire Line
	950  1750 1500 1750
Wire Wire Line
	1500 1850 950  1850
Wire Wire Line
	950  1950 1500 1950
Wire Wire Line
	1500 2050 950  2050
Wire Wire Line
	950  2150 1500 2150
Wire Wire Line
	950  2250 1500 2250
Wire Wire Line
	950  2350 1500 2350
Wire Wire Line
	950  2450 1500 2450
Wire Wire Line
	950  2550 1500 2550
$Comp
L power:GNDREF #PWR068
U 1 1 5F6F1E10
P 950 2650
F 0 "#PWR068" H 950 2400 50  0001 C CNN
F 1 "GNDREF" V 955 2522 50  0000 R CNN
F 2 "" H 950 2650 50  0001 C CNN
F 3 "" H 950 2650 50  0001 C CNN
	1    950  2650
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR067
U 1 1 5F6F1FF3
P 950 2550
F 0 "#PWR067" H 950 2400 50  0001 C CNN
F 1 "+3.3V" V 965 2678 50  0000 L CNN
F 2 "" H 950 2550 50  0001 C CNN
F 3 "" H 950 2550 50  0001 C CNN
	1    950  2550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	950  2650 1500 2650
Wire Wire Line
	950  2750 1500 2750
$Comp
L power:GNDREF #PWR072
U 1 1 5F6F3C96
P 950 3050
F 0 "#PWR072" H 950 2800 50  0001 C CNN
F 1 "GNDREF" V 955 2922 50  0000 R CNN
F 2 "" H 950 3050 50  0001 C CNN
F 3 "" H 950 3050 50  0001 C CNN
	1    950  3050
	0    1    1    0   
$EndComp
Wire Wire Line
	950  2850 1500 2850
Wire Wire Line
	950  2950 1500 2950
$Comp
L power:+5V #PWR073
U 1 1 5F6F4737
P 950 3150
F 0 "#PWR073" H 950 3000 50  0001 C CNN
F 1 "+5V" V 965 3278 50  0000 L CNN
F 2 "" H 950 3150 50  0001 C CNN
F 3 "" H 950 3150 50  0001 C CNN
	1    950  3150
	0    -1   -1   0   
$EndComp
$Comp
L power:GNDREF #PWR074
U 1 1 5F6F49A5
P 950 3250
F 0 "#PWR074" H 950 3000 50  0001 C CNN
F 1 "GNDREF" V 955 3122 50  0000 R CNN
F 2 "" H 950 3250 50  0001 C CNN
F 3 "" H 950 3250 50  0001 C CNN
	1    950  3250
	0    1    1    0   
$EndComp
Wire Wire Line
	950  3050 1500 3050
$Comp
L power:GNDREF #PWR070
U 1 1 5F723E89
P 950 2850
F 0 "#PWR070" H 950 2600 50  0001 C CNN
F 1 "GNDREF" V 955 2722 50  0000 R CNN
F 2 "" H 950 2850 50  0001 C CNN
F 3 "" H 950 2850 50  0001 C CNN
	1    950  2850
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR069
U 1 1 5F723E93
P 950 2750
F 0 "#PWR069" H 950 2600 50  0001 C CNN
F 1 "+3.3V" V 965 2878 50  0000 L CNN
F 2 "" H 950 2750 50  0001 C CNN
F 3 "" H 950 2750 50  0001 C CNN
	1    950  2750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	950  3150 1500 3150
Wire Wire Line
	1500 3250 950  3250
$Comp
L power:GNDREF #PWR083
U 1 1 5F73ED00
P 1200 5250
F 0 "#PWR083" H 1200 5000 50  0001 C CNN
F 1 "GNDREF" V 1205 5122 50  0000 R CNN
F 2 "" H 1200 5250 50  0001 C CNN
F 3 "" H 1200 5250 50  0001 C CNN
	1    1200 5250
	0    1    1    0   
$EndComp
Wire Wire Line
	1200 5250 1450 5250
$Comp
L power:+5V #PWR078
U 1 1 5F73ED0B
P 1150 5350
F 0 "#PWR078" H 1150 5200 50  0001 C CNN
F 1 "+5V" V 1165 5478 50  0000 L CNN
F 2 "" H 1150 5350 50  0001 C CNN
F 3 "" H 1150 5350 50  0001 C CNN
	1    1150 5350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1150 5350 1450 5350
Text HLabel 1150 5550 0    50   Input ~ 0
SCL
Wire Wire Line
	1150 5550 1450 5550
Text HLabel 1150 5450 0    50   BiDi ~ 0
SDA
Wire Wire Line
	1150 5450 1450 5450
$Comp
L power:GNDREF #PWR084
U 1 1 5F746B5F
P 1200 5750
F 0 "#PWR084" H 1200 5500 50  0001 C CNN
F 1 "GNDREF" V 1205 5622 50  0000 R CNN
F 2 "" H 1200 5750 50  0001 C CNN
F 3 "" H 1200 5750 50  0001 C CNN
	1    1200 5750
	0    1    1    0   
$EndComp
Wire Wire Line
	1200 5750 1450 5750
$Comp
L power:+5V #PWR079
U 1 1 5F746B6A
P 1150 5850
F 0 "#PWR079" H 1150 5700 50  0001 C CNN
F 1 "+5V" V 1165 5978 50  0000 L CNN
F 2 "" H 1150 5850 50  0001 C CNN
F 3 "" H 1150 5850 50  0001 C CNN
	1    1150 5850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1150 5850 1450 5850
Text HLabel 1150 6050 0    50   Input ~ 0
SCL
Wire Wire Line
	1150 6050 1450 6050
Text HLabel 1150 5950 0    50   BiDi ~ 0
SDA
Wire Wire Line
	1150 5950 1450 5950
$Comp
L Connector_Generic:Conn_01x06 J45
U 1 1 5F750481
P 1650 6450
F 0 "J45" H 1730 6487 50  0000 L CNN
F 1 "Conn_01x06" H 1730 6396 50  0000 L CNN
F 2 "Connector_JST:JST_PH_S6B-PH-K_1x06_P2.00mm_Horizontal" H 1650 6450 50  0001 C CNN
F 3 "~" H 1650 6450 50  0001 C CNN
F 4 "P8" H 1730 6305 50  0000 L CNN "Enable"
	1    1650 6450
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR086
U 1 1 5F755C5B
P 1300 6250
F 0 "#PWR086" H 1300 6000 50  0001 C CNN
F 1 "GNDREF" V 1305 6122 50  0000 R CNN
F 2 "" H 1300 6250 50  0001 C CNN
F 3 "" H 1300 6250 50  0001 C CNN
	1    1300 6250
	0    1    1    0   
$EndComp
Wire Wire Line
	1300 6250 1450 6250
$Comp
L power:+5V #PWR085
U 1 1 5F7572FF
P 1250 6350
F 0 "#PWR085" H 1250 6200 50  0001 C CNN
F 1 "+5V" V 1265 6478 50  0000 L CNN
F 2 "" H 1250 6350 50  0001 C CNN
F 3 "" H 1250 6350 50  0001 C CNN
	1    1250 6350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1250 6350 1450 6350
Wire Wire Line
	1450 6450 1200 6450
Wire Wire Line
	1450 6550 1200 6550
Wire Wire Line
	1450 6650 1200 6650
Wire Wire Line
	1450 6750 1200 6750
Text HLabel 1200 6450 0    50   Input ~ 0
MOSI
Text HLabel 1200 6550 0    50   Output ~ 0
MISO
Text HLabel 1200 6650 0    50   Input ~ 0
SCK
Text HLabel 1200 6750 0    50   BiDi ~ 0
P8
$Comp
L Connector_Generic:Conn_01x06 J47
U 1 1 5F76E92B
P 3850 1150
F 0 "J47" H 3930 1187 50  0000 L CNN
F 1 "Conn_01x06" H 3930 1096 50  0000 L CNN
F 2 "Connector_JST:JST_PH_S6B-PH-K_1x06_P2.00mm_Horizontal" H 3850 1150 50  0001 C CNN
F 3 "~" H 3850 1150 50  0001 C CNN
F 4 "P7" H 3930 1005 50  0000 L CNN "Enable"
	1    3850 1150
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0109
U 1 1 5F76E935
P 3500 950
F 0 "#PWR0109" H 3500 700 50  0001 C CNN
F 1 "GNDREF" V 3505 822 50  0000 R CNN
F 2 "" H 3500 950 50  0001 C CNN
F 3 "" H 3500 950 50  0001 C CNN
	1    3500 950 
	0    1    1    0   
$EndComp
Wire Wire Line
	3500 950  3650 950 
$Comp
L power:+5V #PWR0107
U 1 1 5F76E940
P 3450 1050
F 0 "#PWR0107" H 3450 900 50  0001 C CNN
F 1 "+5V" V 3465 1178 50  0000 L CNN
F 2 "" H 3450 1050 50  0001 C CNN
F 3 "" H 3450 1050 50  0001 C CNN
	1    3450 1050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3450 1050 3650 1050
Wire Wire Line
	3650 1150 3400 1150
Wire Wire Line
	3650 1250 3400 1250
Wire Wire Line
	3650 1350 3400 1350
Wire Wire Line
	3650 1450 3400 1450
Text HLabel 3400 1150 0    50   Input ~ 0
MOSI
Text HLabel 3400 1250 0    50   Output ~ 0
MISO
Text HLabel 3400 1350 0    50   Input ~ 0
SCK
Text HLabel 3400 1450 0    50   BiDi ~ 0
P7
$Comp
L Connector_Generic:Conn_01x06 J48
U 1 1 5F776599
P 3850 1950
F 0 "J48" H 3930 1987 50  0000 L CNN
F 1 "Conn_01x06" H 3930 1896 50  0000 L CNN
F 2 "Connector_JST:JST_PH_S6B-PH-K_1x06_P2.00mm_Horizontal" H 3850 1950 50  0001 C CNN
F 3 "~" H 3850 1950 50  0001 C CNN
F 4 "P6" H 3930 1805 50  0000 L CNN "Enable"
	1    3850 1950
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0110
U 1 1 5F7765A3
P 3500 1750
F 0 "#PWR0110" H 3500 1500 50  0001 C CNN
F 1 "GNDREF" V 3505 1622 50  0000 R CNN
F 2 "" H 3500 1750 50  0001 C CNN
F 3 "" H 3500 1750 50  0001 C CNN
	1    3500 1750
	0    1    1    0   
$EndComp
Wire Wire Line
	3500 1750 3650 1750
$Comp
L power:+5V #PWR0108
U 1 1 5F7765AE
P 3450 1850
F 0 "#PWR0108" H 3450 1700 50  0001 C CNN
F 1 "+5V" V 3465 1978 50  0000 L CNN
F 2 "" H 3450 1850 50  0001 C CNN
F 3 "" H 3450 1850 50  0001 C CNN
	1    3450 1850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3450 1850 3650 1850
Wire Wire Line
	3650 1950 3400 1950
Wire Wire Line
	3650 2050 3400 2050
Wire Wire Line
	3650 2150 3400 2150
Wire Wire Line
	3650 2250 3400 2250
Text HLabel 3400 1950 0    50   Input ~ 0
MOSI
Text HLabel 3400 2050 0    50   Output ~ 0
MISO
Text HLabel 3400 2150 0    50   Input ~ 0
SCK
Text HLabel 3400 2250 0    50   BiDi ~ 0
P6
$Comp
L Connector_Generic:Conn_01x04 J41
U 1 1 6054F9F8
P 1650 5350
F 0 "J41" H 1422 5405 45  0000 R CNN
F 1 "CONN_04JST-PTH" H 1422 5489 45  0000 R CNN
F 2 "Connector_JST:JST_PH_S4B-PH-K_1x04_P2.00mm_Horizontal" H 1650 5850 20  0001 C CNN
F 3 "" H 1650 5350 50  0001 C CNN
F 4 "WIRE-13531" H 1422 5584 60  0000 R CNN "Field4"
	1    1650 5350
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J42
U 1 1 605503F9
P 1650 5850
F 0 "J42" H 1422 5905 45  0000 R CNN
F 1 "CONN_04JST-PTH" H 1422 5989 45  0000 R CNN
F 2 "Connector_JST:JST_PH_S4B-PH-K_1x04_P2.00mm_Horizontal" H 1650 6350 20  0001 C CNN
F 3 "" H 1650 5850 50  0001 C CNN
F 4 "WIRE-13531" H 1422 6084 60  0000 R CNN "Field4"
	1    1650 5850
	1    0    0    -1  
$EndComp
$EndSCHEMATC
