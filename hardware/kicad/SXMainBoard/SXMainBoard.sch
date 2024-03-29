EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 3
Title "80960Sx Mainboard"
Date "2020-02-16"
Rev "20"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 1550 1700 2    50   ~ 0
VCC0
Text Label 3250 1700 0    50   ~ 0
GND0
Text Label 1550 1800 2    50   ~ 0
VCC1
Text Label 3250 1800 0    50   ~ 0
GND1
Text Label 3250 1900 0    50   ~ 0
GND2
Text Label 3250 2000 0    50   ~ 0
GND3
Text Label 3250 2100 0    50   ~ 0
GND4
Text Label 1550 1900 2    50   ~ 0
VCC2
Text Label 1550 2000 2    50   ~ 0
VCC3
Text Label 5950 1700 0    50   ~ 0
GND5
Text Label 5950 1800 0    50   ~ 0
GND6
Text Label 5950 1900 0    50   ~ 0
GND7
Text Label 5950 2000 0    50   ~ 0
GND8
Text Label 5950 2100 0    50   ~ 0
GND9
Text Label 4400 2200 2    50   ~ 0
VCC11
Text Label 3250 2300 0    50   ~ 0
GND10
Text Label 5950 2300 0    50   ~ 0
GND11
Text Label 4400 1700 2    50   ~ 0
VCC5
Text Label 1600 2200 2    50   ~ 0
VCC10
Text Label 4400 1800 2    50   ~ 0
VCC6
Text Label 4400 1900 2    50   ~ 0
VCC7
Text Label 4400 2000 2    50   ~ 0
VCC8
Text Label 4400 2100 2    50   ~ 0
VCC9
Text Label 2000 2500 2    50   ~ 0
A31
Text Label 2900 2500 0    50   ~ 0
A30
Text Label 2000 2600 2    50   ~ 0
A29
Text Label 2900 2600 0    50   ~ 0
A28
Text Label 2000 2700 2    50   ~ 0
A27
Text Label 2900 2700 0    50   ~ 0
A26
Text Label 2000 2800 2    50   ~ 0
A25
Text Label 2900 2800 0    50   ~ 0
A24
Text Label 2000 2900 2    50   ~ 0
A23
Text Label 2900 2900 0    50   ~ 0
A22
Text Label 2000 3000 2    50   ~ 0
A21
Text Label 2900 3000 0    50   ~ 0
A20
Text Label 2900 3100 0    50   ~ 0
A18
Text Label 2900 3200 0    50   ~ 0
A16
Text Label 2900 3300 0    50   ~ 0
AD14
Text Label 2900 3400 0    50   ~ 0
AD12
Text Label 2900 3500 0    50   ~ 0
AD10
Text Label 2900 3600 0    50   ~ 0
AD8
Text Label 2900 3700 0    50   ~ 0
AD6
Text Label 2900 3800 0    50   ~ 0
AD4
Text Label 2900 3900 0    50   ~ 0
AD2
Text Label 2900 4000 0    50   ~ 0
D0
Text Label 2000 3100 2    50   ~ 0
A19
Text Label 2000 3200 2    50   ~ 0
A17
Text Label 2000 3300 2    50   ~ 0
AD15
Text Label 2000 3400 2    50   ~ 0
AD13
Text Label 2000 3500 2    50   ~ 0
AD11
Text Label 2000 3600 2    50   ~ 0
AD9
Text Label 2000 3700 2    50   ~ 0
AD7
Text Label 2000 3800 2    50   ~ 0
AD5
Text Label 2000 3900 2    50   ~ 0
AD3
Text Label 2000 4000 2    50   ~ 0
AD1
Text Label 5600 3300 0    50   ~ 0
A2
Text Label 4700 3300 2    50   ~ 0
A1
Text Label 4700 3400 2    50   ~ 0
A3
Text Label 5600 2900 0    50   ~ 0
~BE0
Text Label 4700 2400 2    50   ~ 0
INT0
Text Label 5600 2400 0    50   ~ 0
INT1
Text Label 4700 2500 2    50   ~ 0
INT2
Text Label 5600 2500 0    50   ~ 0
INT3
Text Label 6200 3200 0    50   ~ 0
~RESET
Text Label 1550 2100 2    50   ~ 0
VCC4
Wire Wire Line
	1550 2100 2000 2100
Wire Wire Line
	2000 1900 1550 1900
Text Label 5600 2800 0    50   ~ 0
ALE
Text Label 5600 2700 0    50   ~ 0
HOLD
Text Label 5600 2600 0    50   ~ 0
WR
Text Label 4700 2800 2    50   ~ 0
~READY
Text Label 4700 2700 2    50   ~ 0
HLDA
Text Label 4700 2600 2    50   ~ 0
~LOCK
$Comp
L PCIE-098-02-X-D-TH:PCIE-098-02-X-D-TH_80960 J1
U 2 1 5E384F11
P 2000 1700
F 0 "J1" H 2450 1967 50  0000 C CNN
F 1 "PCIE-098-02-X-D-TH" H 2450 1876 50  0000 C CNN
F 2 "PCIE-098-02-X-D-TH:PCIE-098-02-X-D-TH" H 2000 1700 50  0001 L BNN
F 3 "" H 2000 1700 50  0001 C CNN
	2    2000 1700
	1    0    0    -1  
$EndComp
Text Label 4700 3200 2    50   ~ 0
~AS
Text Label 4700 4100 2    50   ~ 0
~DEN
Text Label 4700 3000 2    50   ~ 0
~BLAST
Text Label 4700 2900 2    50   ~ 0
~BE1
NoConn ~ 5600 3500
NoConn ~ 5600 3600
NoConn ~ 5600 3700
NoConn ~ 5600 3800
NoConn ~ 5600 3900
NoConn ~ 5600 4000
NoConn ~ 4700 4000
NoConn ~ 4700 3900
NoConn ~ 4700 3800
NoConn ~ 4700 3700
NoConn ~ 4700 3600
NoConn ~ 4700 3500
NoConn ~ 2900 4100
NoConn ~ 2000 4100
NoConn ~ 2000 4200
NoConn ~ 5600 3400
NoConn ~ 2900 2400
NoConn ~ 2000 2400
$Comp
L PCIE-098-02-X-D-TH:PCIE-098-02-X-D-TH_80960 J1
U 1 1 5E383F11
P 4700 1700
F 0 "J1" H 5150 1967 50  0000 C CNN
F 1 "PCIE-098-02-X-D-TH" H 5150 1876 50  0000 C CNN
F 2 "PCIE-098-02-X-D-TH:PCIE-098-02-X-D-TH" H 4700 1700 50  0001 L BNN
F 3 "" H 4700 1700 50  0001 C CNN
	1    4700 1700
	1    0    0    -1  
$EndComp
Text Label 4700 4200 2    50   ~ 0
CLK2
NoConn ~ 5600 3000
Text Label 5600 4100 0    50   ~ 0
DTR
NoConn ~ 5600 3100
NoConn ~ 4700 3100
$Comp
L SparkFun-Connectors:ATX24RH J3
U 1 1 5E519E9E
P 13600 8500
F 0 "J3" H 13600 9560 45  0000 C CNN
F 1 "ATX24RH" H 13600 9476 45  0000 C CNN
F 2 "Connectors:ATX24_RIGHT_ANGLE" H 13600 9450 20  0001 C CNN
F 3 "" H 13600 8500 50  0001 C CNN
F 4 "CONN-09674" H 13600 9381 60  0000 C CNN "Field4"
	1    13600 8500
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J2
U 1 1 5E49BA00
P 13500 6100
F 0 "J2" H 13550 6417 50  0000 C CNN
F 1 "Conn_02x03_Odd_Even" H 13550 6326 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x03_P2.54mm_Vertical" H 13500 6100 50  0001 C CNN
F 3 "~" H 13500 6100 50  0001 C CNN
	1    13500 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	14150 9000 14650 9000
Wire Wire Line
	14150 9100 14650 9100
Text Label 14650 9000 0    50   ~ 0
PWR_ON
Text Label 13300 6000 2    50   ~ 0
PWR_ON
Wire Wire Line
	13050 8900 12550 8900
Text Label 12550 8900 2    50   ~ 0
PWR_ON_GND
Text Label 13800 6000 0    50   ~ 0
PWR_ON_GND
Text Label 14650 9100 0    50   ~ 0
POWER_OK
Text Label 13300 6100 2    50   ~ 0
POWER_OK
Wire Wire Line
	13050 9100 12550 9100
Text Label 12550 9100 2    50   ~ 0
POWER_OK_GND
Text Label 13800 6100 0    50   ~ 0
POWER_OK_GND
NoConn ~ 14150 9300
Wire Wire Line
	5950 1700 5600 1700
Wire Wire Line
	5600 1800 5950 1800
Wire Wire Line
	5950 1900 5600 1900
Wire Wire Line
	5600 2000 5950 2000
Wire Wire Line
	5600 2100 5950 2100
Wire Wire Line
	5950 2300 5600 2300
Wire Wire Line
	4400 2200 4700 2200
Wire Wire Line
	4700 2100 4400 2100
Wire Wire Line
	4400 2000 4700 2000
Wire Wire Line
	4700 1900 4400 1900
Wire Wire Line
	4400 1800 4700 1800
Wire Wire Line
	4700 1700 4400 1700
Wire Wire Line
	1550 1800 2000 1800
Wire Wire Line
	1550 2000 2000 2000
Wire Wire Line
	1600 2200 2000 2200
Wire Wire Line
	2000 1700 1550 1700
Wire Wire Line
	2900 1700 3250 1700
Wire Wire Line
	3250 1800 2900 1800
Wire Wire Line
	2900 1900 3250 1900
Wire Wire Line
	3250 2000 2900 2000
Wire Wire Line
	2900 2100 3250 2100
Wire Wire Line
	3250 2300 2900 2300
Text Label 13050 7800 2    50   ~ 0
VCC_SIDEA
Text Label 13050 8600 2    50   ~ 0
GND_SIDEA
Text Label 1550 6500 2    50   ~ 0
VCC11
Text Label 1550 6000 2    50   ~ 0
VCC5
Text Label 1550 6100 2    50   ~ 0
VCC6
Text Label 1550 6200 2    50   ~ 0
VCC7
Text Label 1550 6300 2    50   ~ 0
VCC8
Text Label 1550 6400 2    50   ~ 0
VCC9
Text Label 2950 6250 0    50   ~ 0
VCC_SIDEA
Wire Wire Line
	1550 6000 2400 6000
Wire Wire Line
	2400 6300 1550 6300
Wire Wire Line
	1550 6400 2400 6400
Wire Wire Line
	2400 6400 2400 6300
Connection ~ 2400 6300
Wire Wire Line
	1550 6500 2400 6500
Wire Wire Line
	2400 6500 2400 6400
Connection ~ 2400 6400
Wire Wire Line
	2400 6000 2400 6100
Wire Wire Line
	1550 6100 2400 6100
Connection ~ 2400 6100
Wire Wire Line
	2400 6100 2400 6200
Wire Wire Line
	1550 6200 2400 6200
Connection ~ 2400 6200
Wire Wire Line
	2400 6200 2400 6250
Text Label 1550 7100 2    50   ~ 0
GND5
Text Label 1550 7000 2    50   ~ 0
GND6
Text Label 1550 6900 2    50   ~ 0
GND7
Text Label 1550 6800 2    50   ~ 0
GND8
Text Label 1550 6700 2    50   ~ 0
GND9
Text Label 1550 6600 2    50   ~ 0
GND11
Wire Wire Line
	2400 6250 2950 6250
Connection ~ 2400 6250
Wire Wire Line
	2400 6250 2400 6300
Text Label 3000 6800 0    50   ~ 0
GND_SIDEA
Wire Wire Line
	1550 6600 2400 6600
Wire Wire Line
	2400 6600 2400 6700
Wire Wire Line
	2400 7100 1550 7100
Wire Wire Line
	3000 6800 2400 6800
Connection ~ 2400 6800
Wire Wire Line
	2400 6800 2400 6900
Wire Wire Line
	1550 6700 2400 6700
Connection ~ 2400 6700
Wire Wire Line
	2400 6700 2400 6800
Wire Wire Line
	1550 6800 2400 6800
Wire Wire Line
	1550 6900 2400 6900
Connection ~ 2400 6900
Wire Wire Line
	2400 6900 2400 7000
Wire Wire Line
	1550 7000 2400 7000
Connection ~ 2400 7000
Wire Wire Line
	2400 7000 2400 7100
Text Label 13050 7900 2    50   ~ 0
VCC_SIDEB
Text Label 13050 8700 2    50   ~ 0
GND_SIDEB
Text Label 1500 5850 2    50   ~ 0
GND0
Text Label 1500 5750 2    50   ~ 0
GND1
Text Label 1500 5650 2    50   ~ 0
GND2
Text Label 1500 5550 2    50   ~ 0
GND3
Text Label 1500 5450 2    50   ~ 0
GND4
Text Label 1500 5350 2    50   ~ 0
GND10
Text Label 1450 4700 2    50   ~ 0
VCC0
Text Label 1450 4800 2    50   ~ 0
VCC1
Text Label 1450 4900 2    50   ~ 0
VCC2
Text Label 1450 5000 2    50   ~ 0
VCC3
Text Label 1500 5200 2    50   ~ 0
VCC10
Text Label 1450 5100 2    50   ~ 0
VCC4
Text Label 2650 4950 0    50   ~ 0
VCC_SIDEB
Text Label 3200 5600 0    50   ~ 0
GND_SIDEB
Wire Wire Line
	1500 5350 2500 5350
Wire Wire Line
	2500 5350 2500 5450
Wire Wire Line
	1500 5450 2500 5450
Connection ~ 2500 5450
Wire Wire Line
	2500 5450 2500 5550
Wire Wire Line
	1500 5550 2500 5550
Connection ~ 2500 5550
Wire Wire Line
	2500 5850 1500 5850
Wire Wire Line
	2500 5550 2500 5600
Wire Wire Line
	1500 5750 2500 5750
Connection ~ 2500 5750
Wire Wire Line
	2500 5750 2500 5850
Wire Wire Line
	1500 5650 2500 5650
Connection ~ 2500 5650
Wire Wire Line
	2500 5650 2500 5750
Wire Wire Line
	2500 5600 3200 5600
Connection ~ 2500 5600
Wire Wire Line
	2500 5600 2500 5650
Wire Wire Line
	1450 4700 2400 4700
Wire Wire Line
	2400 4700 2400 4800
Wire Wire Line
	2400 5200 1500 5200
Wire Wire Line
	1450 5100 2400 5100
Connection ~ 2400 5100
Wire Wire Line
	2400 5100 2400 5200
Wire Wire Line
	1450 5000 2400 5000
Connection ~ 2400 5000
Wire Wire Line
	2400 5000 2400 5100
Wire Wire Line
	1450 4900 2400 4900
Connection ~ 2400 4900
Wire Wire Line
	2400 4900 2400 4950
Wire Wire Line
	1450 4800 2400 4800
Connection ~ 2400 4800
Wire Wire Line
	2400 4800 2400 4900
Wire Wire Line
	2650 4950 2400 4950
Connection ~ 2400 4950
Wire Wire Line
	2400 4950 2400 5000
$Comp
L SparkFun-Boards:ARDUINO_MEGA_R3FULL B1
U 1 1 5E5A60DB
P 10500 3250
F 0 "B1" H 10500 5610 45  0000 C CNN
F 1 "ARDUINO_MEGA_R3FULL" H 10500 5526 45  0000 C CNN
F 2 "Boards:ARDUINO_MEGA" H 10500 5400 20  0001 C CNN
F 3 "" H 10500 3250 50  0001 C CNN
F 4 "XXX-00000" H 10500 5431 60  0000 C CNN "Field4"
	1    10500 3250
	1    0    0    -1  
$EndComp
Text Label 11600 4850 0    50   ~ 0
A31
Text Label 11600 4650 0    50   ~ 0
A29
Text Label 11600 4450 0    50   ~ 0
A27
Text Label 11600 4250 0    50   ~ 0
A25
Text Label 11600 4050 0    50   ~ 0
A23
Text Label 11600 3850 0    50   ~ 0
A21
Text Label 11600 3650 0    50   ~ 0
A19
Text Label 11600 3450 0    50   ~ 0
A17
Text Label 11600 3250 0    50   ~ 0
AD15
Text Label 11600 3050 0    50   ~ 0
AD13
Text Label 11600 2850 0    50   ~ 0
AD11
Text Label 11600 2650 0    50   ~ 0
AD9
Text Label 11600 2450 0    50   ~ 0
AD7
Text Label 11600 2250 0    50   ~ 0
AD5
Text Label 11600 1850 0    50   ~ 0
AD3
Text Label 11600 1650 0    50   ~ 0
AD1
Wire Wire Line
	11600 4850 11200 4850
Wire Wire Line
	11600 4650 11200 4650
Wire Wire Line
	11600 4450 11200 4450
Wire Wire Line
	11600 4250 11200 4250
Wire Wire Line
	11600 4050 11200 4050
Wire Wire Line
	11600 3850 11200 3850
Wire Wire Line
	11600 3650 11200 3650
Wire Wire Line
	11600 3450 11200 3450
Wire Wire Line
	11600 3250 11200 3250
Wire Wire Line
	11600 3050 11200 3050
Wire Wire Line
	11600 2850 11200 2850
Wire Wire Line
	11600 2650 11200 2650
Wire Wire Line
	11600 2450 11200 2450
Wire Wire Line
	11600 2250 11200 2250
Wire Wire Line
	11600 1850 11200 1850
Wire Wire Line
	11600 1650 11200 1650
Text Label 11600 4750 0    50   ~ 0
A30
Text Label 11600 4550 0    50   ~ 0
A28
Text Label 11600 4350 0    50   ~ 0
A26
Text Label 11600 4150 0    50   ~ 0
A24
Text Label 11600 3950 0    50   ~ 0
A22
Text Label 11600 3750 0    50   ~ 0
A20
Text Label 11600 3550 0    50   ~ 0
A18
Text Label 11600 3350 0    50   ~ 0
A16
Text Label 11600 3150 0    50   ~ 0
AD14
Text Label 11600 2950 0    50   ~ 0
AD12
Text Label 11600 2750 0    50   ~ 0
AD10
Text Label 11600 2550 0    50   ~ 0
AD8
Text Label 11600 2350 0    50   ~ 0
AD6
Text Label 11600 2150 0    50   ~ 0
AD4
Text Label 11600 1750 0    50   ~ 0
AD2
Text Label 11600 1550 0    50   ~ 0
D0
Wire Wire Line
	11600 1550 11200 1550
Wire Wire Line
	11200 1750 11600 1750
Wire Wire Line
	11200 2150 11600 2150
Wire Wire Line
	11600 2350 11200 2350
Wire Wire Line
	11600 2550 11200 2550
Wire Wire Line
	11200 2750 11600 2750
Wire Wire Line
	11600 2950 11200 2950
Wire Wire Line
	11200 3150 11600 3150
Wire Wire Line
	11600 3350 11200 3350
Wire Wire Line
	11200 3550 11600 3550
Wire Wire Line
	11600 3750 11200 3750
Wire Wire Line
	11200 3950 11600 3950
Wire Wire Line
	11600 4150 11200 4150
Wire Wire Line
	11200 4350 11600 4350
Wire Wire Line
	11600 4550 11200 4550
Wire Wire Line
	11200 4750 11600 4750
Wire Wire Line
	6200 3200 5600 3200
Text Label 8500 1850 2    50   ~ 0
~RESET
Wire Wire Line
	9800 1850 8500 1850
Wire Wire Line
	9550 3950 9800 3950
Wire Wire Line
	9800 3750 9550 3750
Wire Wire Line
	9550 3850 9800 3850
Wire Wire Line
	9550 4050 9800 4050
Wire Wire Line
	9550 4150 9800 4150
NoConn ~ 9800 3550
NoConn ~ 9800 3650
Wire Wire Line
	9550 4550 9800 4550
Wire Wire Line
	9550 4650 9800 4650
Text Label 11600 1250 0    50   ~ 0
~BE1
Wire Wire Line
	11600 1250 11200 1250
Wire Wire Line
	9550 4750 9800 4750
Text Label 11600 1350 0    50   ~ 0
~BLAST
Wire Wire Line
	11600 1350 11200 1350
Text Label 11600 1450 0    50   ~ 0
~AS
Wire Wire Line
	11600 1450 11200 1450
Wire Wire Line
	9800 1950 8500 1950
Text Label 8500 1950 2    50   ~ 0
A1
Text Label 8500 2050 2    50   ~ 0
A2
Wire Wire Line
	8500 2050 9800 2050
Text Label 8500 2150 2    50   ~ 0
A3
Wire Wire Line
	8500 2150 9800 2150
Text Label 8500 2250 2    50   ~ 0
~DEN
Wire Wire Line
	8500 2250 9800 2250
Text Label 8500 2350 2    50   ~ 0
DTR
Wire Wire Line
	8500 2350 9800 2350
Text Label 8500 2450 2    50   ~ 0
CLK2
Wire Wire Line
	8500 2450 9800 2450
Wire Wire Line
	11200 1950 11600 1950
Wire Wire Line
	11200 2050 11600 2050
Text Label 11600 1950 0    50   ~ 0
SDA
Text Label 11600 2050 0    50   ~ 0
SCL
Text Label 8500 2550 2    50   ~ 0
~RESET
Wire Wire Line
	8500 2550 9800 2550
Wire Wire Line
	11200 5250 11600 5250
Wire Wire Line
	11200 5150 11600 5150
Wire Wire Line
	11200 5050 11600 5050
Wire Wire Line
	11200 4950 11600 4950
Text Label 11600 4950 0    50   ~ 0
MISO
Text Label 11600 5050 0    50   ~ 0
MOSI
Text Label 11600 5150 0    50   ~ 0
SCK
Text Label 11600 5250 0    50   ~ 0
~SS
Wire Wire Line
	9800 2650 8500 2650
Wire Wire Line
	9800 2750 8500 2750
Wire Wire Line
	9800 2850 8500 2850
Wire Wire Line
	9800 2950 8500 2950
Wire Wire Line
	9800 3050 8500 3050
Wire Wire Line
	9800 3150 8500 3150
Wire Wire Line
	9800 3250 8500 3250
Wire Wire Line
	9800 3350 8500 3350
Wire Wire Line
	9800 3450 8500 3450
$Comp
L Switch:SW_Push SW1
U 1 1 5E862D4E
P 14550 2550
F 0 "SW1" H 14550 2835 50  0000 C CNN
F 1 "SW_Push_Dual" H 14550 2744 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 14550 2750 50  0001 C CNN
F 3 "~" H 14550 2750 50  0001 C CNN
	1    14550 2550
	1    0    0    -1  
$EndComp
Text Label 15050 2550 0    50   ~ 0
~RESET
Wire Wire Line
	15050 2550 14750 2550
Wire Wire Line
	14350 2550 13950 2550
Wire Wire Line
	13950 2550 13950 2850
$Comp
L power:GNDREF #PWR04
U 1 1 5E881A75
P 13950 2850
F 0 "#PWR04" H 13950 2600 50  0001 C CNN
F 1 "GNDREF" H 13955 2677 50  0000 C CNN
F 2 "" H 13950 2850 50  0001 C CNN
F 3 "" H 13950 2850 50  0001 C CNN
	1    13950 2850
	1    0    0    -1  
$EndComp
NoConn ~ 9800 1450
NoConn ~ 9800 1550
NoConn ~ 9800 1650
NoConn ~ 9800 1750
NoConn ~ 9800 4850
NoConn ~ 9800 4950
NoConn ~ 9800 5050
NoConn ~ 9800 5150
NoConn ~ 9800 5250
Wire Wire Line
	13050 8800 12850 8800
Wire Wire Line
	12850 8800 12850 9000
Wire Wire Line
	13050 9300 12850 9300
Connection ~ 12850 9300
Wire Wire Line
	12850 9300 12850 9600
Wire Wire Line
	13050 9200 12850 9200
Connection ~ 12850 9200
Wire Wire Line
	12850 9200 12850 9300
Wire Wire Line
	13050 9000 12850 9000
Connection ~ 12850 9000
Wire Wire Line
	12850 9000 12850 9200
$Comp
L power:GNDREF #PWR03
U 1 1 5E905BEB
P 12850 9600
F 0 "#PWR03" H 12850 9350 50  0001 C CNN
F 1 "GNDREF" H 12855 9427 50  0000 C CNN
F 2 "" H 12850 9600 50  0001 C CNN
F 3 "" H 12850 9600 50  0001 C CNN
	1    12850 9600
	1    0    0    -1  
$EndComp
Text Label 8500 2650 2    50   ~ 0
A7
Text Label 8500 2750 2    50   ~ 0
A8
Text Label 8500 2850 2    50   ~ 0
A9
Text Label 8500 2950 2    50   ~ 0
A10
Text Label 8500 3050 2    50   ~ 0
A11
Text Label 8500 3150 2    50   ~ 0
A12
Text Label 8500 3250 2    50   ~ 0
A13
Text Label 8500 3350 2    50   ~ 0
A14
Text Label 8500 3450 2    50   ~ 0
A15
Text Label 13750 900  2    50   ~ 0
A7
Text Label 13750 1000 2    50   ~ 0
A8
Text Label 13750 1100 2    50   ~ 0
A9
Text Label 13750 1200 2    50   ~ 0
A10
Text Label 13750 1300 2    50   ~ 0
A11
Text Label 13750 1400 2    50   ~ 0
A12
Text Label 13750 1500 2    50   ~ 0
A13
Text Label 13750 1600 2    50   ~ 0
A14
Text Label 13750 1700 2    50   ~ 0
A15
Wire Wire Line
	13750 900  14100 900 
Wire Wire Line
	13750 1000 14100 1000
Wire Wire Line
	13750 1100 14100 1100
Wire Wire Line
	13750 1200 14100 1200
Wire Wire Line
	13750 1300 14100 1300
Wire Wire Line
	13750 1400 14100 1400
Wire Wire Line
	13750 1500 14100 1500
Wire Wire Line
	13750 1600 14100 1600
Wire Wire Line
	13750 1700 14100 1700
Text Label 15400 900  0    50   ~ 0
MISO
Text Label 15400 1000 0    50   ~ 0
MOSI
Text Label 15400 1100 0    50   ~ 0
SCK
Text Label 13100 4150 2    50   ~ 0
~SS
Wire Wire Line
	15400 900  15250 900 
Wire Wire Line
	15250 1000 15400 1000
Wire Wire Line
	15400 1100 15250 1100
Text Label 15400 1300 0    50   ~ 0
SDA
Text Label 15400 1200 0    50   ~ 0
SCL
Wire Wire Line
	15400 1200 15250 1200
Wire Wire Line
	15400 1300 15250 1300
Wire Wire Line
	14150 8100 14350 8100
Wire Wire Line
	14350 8100 14350 8000
Wire Wire Line
	14150 8000 14350 8000
Connection ~ 14350 8000
Wire Wire Line
	14350 8000 14350 7900
Wire Wire Line
	14150 7900 14350 7900
Connection ~ 14350 7900
Wire Wire Line
	14350 7900 14350 7800
Wire Wire Line
	14150 7800 14350 7800
Connection ~ 14350 7800
Wire Wire Line
	14350 7800 14350 7400
$Comp
L power:+3.3V #PWR05
U 1 1 5EAA251E
P 14350 7400
F 0 "#PWR05" H 14350 7250 50  0001 C CNN
F 1 "+3.3V" H 14365 7573 50  0000 C CNN
F 2 "" H 14350 7400 50  0001 C CNN
F 3 "" H 14350 7400 50  0001 C CNN
	1    14350 7400
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR02
U 1 1 5EAD51B4
P 12300 7800
F 0 "#PWR02" H 12300 7650 50  0001 C CNN
F 1 "+5V" H 12315 7973 50  0000 C CNN
F 2 "" H 12300 7800 50  0001 C CNN
F 3 "" H 12300 7800 50  0001 C CNN
	1    12300 7800
	1    0    0    -1  
$EndComp
Wire Wire Line
	12300 7800 12300 8000
Wire Wire Line
	12300 8000 13050 8000
Wire Wire Line
	12300 8100 12300 8000
Wire Wire Line
	12300 8100 13050 8100
Connection ~ 12300 8000
Wire Wire Line
	12300 8200 12300 8100
Wire Wire Line
	12300 8200 13050 8200
Connection ~ 12300 8100
$Comp
L power:+12V #PWR06
U 1 1 5EB1727F
P 15000 8100
F 0 "#PWR06" H 15000 7950 50  0001 C CNN
F 1 "+12V" H 15015 8273 50  0000 C CNN
F 2 "" H 15000 8100 50  0001 C CNN
F 3 "" H 15000 8100 50  0001 C CNN
	1    15000 8100
	1    0    0    -1  
$EndComp
Wire Wire Line
	15000 8100 15000 8500
Wire Wire Line
	15000 8600 14150 8600
Wire Wire Line
	14150 8500 15000 8500
Connection ~ 15000 8500
Wire Wire Line
	15000 8500 15000 8600
$Comp
L power:+12V #PWR01
U 1 1 5EB32B90
P 9400 1150
F 0 "#PWR01" H 9400 1000 50  0001 C CNN
F 1 "+12V" H 9415 1323 50  0000 C CNN
F 2 "" H 9400 1150 50  0001 C CNN
F 3 "" H 9400 1150 50  0001 C CNN
	1    9400 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9400 1150 9400 1250
Wire Wire Line
	9400 1250 9800 1250
$Comp
L power:-12V #PWR07
U 1 1 5EB427FA
P 15300 8100
F 0 "#PWR07" H 15300 8200 50  0001 C CNN
F 1 "-12V" H 15315 8273 50  0000 C CNN
F 2 "" H 15300 8100 50  0001 C CNN
F 3 "" H 15300 8100 50  0001 C CNN
	1    15300 8100
	1    0    0    -1  
$EndComp
Wire Wire Line
	15300 8100 15300 8800
Wire Wire Line
	15300 8800 14150 8800
Text Label 7850 7700 0    50   ~ 0
A23
Text Label 7850 7500 0    50   ~ 0
A21
Text Label 7850 7300 0    50   ~ 0
A19
Text Label 7850 7100 0    50   ~ 0
A17
Text Label 4150 8500 2    50   ~ 0
AD15
Text Label 7850 7600 0    50   ~ 0
A22
Text Label 7850 7400 0    50   ~ 0
A20
Text Label 7850 7200 0    50   ~ 0
A18
Text Label 7850 7000 0    50   ~ 0
A16
Text Label 4150 8400 2    50   ~ 0
AD14
Text Label 4150 7000 2    50   ~ 0
D0
Text Label 4150 7200 2    50   ~ 0
AD2
Text Label 4150 7400 2    50   ~ 0
AD4
Text Label 4150 7600 2    50   ~ 0
AD6
Text Label 4150 7800 2    50   ~ 0
AD8
Text Label 4150 8000 2    50   ~ 0
AD10
Text Label 4150 8200 2    50   ~ 0
AD12
Text Label 4150 7100 2    50   ~ 0
AD1
Text Label 4150 7300 2    50   ~ 0
AD3
Text Label 4150 7500 2    50   ~ 0
AD5
Text Label 4150 7700 2    50   ~ 0
AD7
Text Label 4150 7900 2    50   ~ 0
AD9
Text Label 4150 8100 2    50   ~ 0
AD11
Text Label 4150 8300 2    50   ~ 0
AD13
$Sheet
S 4750 6900 2650 4100
U 5EB6C543
F0 "i960 Protyping Area" 50
F1 "i960ProtypingArea.sch" 50
F2 "D0" B L 4750 7000 50 
F3 "AD1" B L 4750 7100 50 
F4 "AD2" B L 4750 7200 50 
F5 "AD3" B L 4750 7300 50 
F6 "AD4" B L 4750 7400 50 
F7 "AD5" B L 4750 7500 50 
F8 "AD6" B L 4750 7600 50 
F9 "AD7" B L 4750 7700 50 
F10 "AD8" B L 4750 7800 50 
F11 "AD9" B L 4750 7900 50 
F12 "AD10" B L 4750 8000 50 
F13 "AD11" B L 4750 8100 50 
F14 "AD12" B L 4750 8200 50 
F15 "AD13" B L 4750 8300 50 
F16 "AD14" B L 4750 8400 50 
F17 "AD15" B L 4750 8500 50 
F18 "A16" I R 7400 7000 50 
F19 "A17" I R 7400 7100 50 
F20 "A18" I R 7400 7200 50 
F21 "A19" I R 7400 7300 50 
F22 "A20" I R 7400 7400 50 
F23 "A21" I R 7400 7500 50 
F24 "A22" I R 7400 7600 50 
F25 "A23" I R 7400 7700 50 
F26 "A24" I R 7400 7800 50 
F27 "A25" I R 7400 7900 50 
F28 "A26" I R 7400 8000 50 
F29 "A27" I R 7400 8100 50 
F30 "A28" I R 7400 8200 50 
F31 "A29" I R 7400 8300 50 
F32 "A30" I R 7400 8400 50 
F33 "A31" I R 7400 8500 50 
F34 "A1" I L 4750 8650 50 
F35 "A2" I L 4750 8750 50 
F36 "A3" I L 4750 8850 50 
F37 "~BE0" I L 4750 10150 50 
F38 "~BE1" I L 4750 10250 50 
F39 "~DEN" I L 4750 8950 50 
F40 "DTR" I L 4750 9050 50 
F41 "INT0" I L 4750 9150 50 
F42 "INT1" I L 4750 9250 50 
F43 "INT2" I L 4750 9350 50 
F44 "INT3" I L 4750 9450 50 
F45 "~LOCK" I L 4750 9550 50 
F46 "WR" I L 4750 9650 50 
F47 "HLDA" I L 4750 9750 50 
F48 "HOLD" I L 4750 9850 50 
F49 "~READY" I L 4750 9950 50 
F50 "ALE" I L 4750 10050 50 
F51 "~BLAST" I L 4750 10350 50 
F52 "~AS" I L 4750 10450 50 
F53 "~RESET" I L 4750 10550 50 
$EndSheet
Text Label 7850 8500 0    50   ~ 0
A31
Text Label 7850 8400 0    50   ~ 0
A30
Text Label 7850 8300 0    50   ~ 0
A29
Text Label 7850 8200 0    50   ~ 0
A28
Text Label 7850 8100 0    50   ~ 0
A27
Text Label 7850 8000 0    50   ~ 0
A26
Text Label 7850 7900 0    50   ~ 0
A25
Text Label 7850 7800 0    50   ~ 0
A24
Wire Wire Line
	4150 7000 4750 7000
Wire Wire Line
	4750 7100 4150 7100
Wire Wire Line
	4150 7200 4750 7200
Wire Wire Line
	4150 7400 4750 7400
Wire Wire Line
	4750 7300 4150 7300
Wire Wire Line
	4150 7500 4750 7500
Wire Wire Line
	4150 7600 4750 7600
Wire Wire Line
	4150 7700 4750 7700
Wire Wire Line
	4150 7800 4750 7800
Wire Wire Line
	4150 7900 4750 7900
Wire Wire Line
	4150 8000 4750 8000
Wire Wire Line
	4750 8100 4150 8100
Wire Wire Line
	4150 8200 4750 8200
Wire Wire Line
	4150 8300 4750 8300
Wire Wire Line
	4150 8400 4750 8400
Wire Wire Line
	4750 8500 4150 8500
Wire Wire Line
	7400 8500 7850 8500
Wire Wire Line
	7850 8400 7400 8400
Wire Wire Line
	7400 8300 7850 8300
Wire Wire Line
	7850 8200 7400 8200
Wire Wire Line
	7400 8100 7850 8100
Wire Wire Line
	7850 8000 7400 8000
Wire Wire Line
	7400 7900 7850 7900
Wire Wire Line
	7850 7800 7400 7800
Wire Wire Line
	7400 7700 7850 7700
Wire Wire Line
	7850 7600 7400 7600
Wire Wire Line
	7400 7500 7850 7500
Wire Wire Line
	7850 7400 7400 7400
Wire Wire Line
	7400 7300 7850 7300
Wire Wire Line
	7850 7200 7400 7200
Wire Wire Line
	7400 7100 7850 7100
Wire Wire Line
	7850 7000 7400 7000
Text Label 4100 8650 2    50   ~ 0
A1
Text Label 4100 8750 2    50   ~ 0
A2
Text Label 4100 8850 2    50   ~ 0
A3
Text Label 4100 8950 2    50   ~ 0
~DEN
Text Label 4100 9050 2    50   ~ 0
DTR
Wire Wire Line
	4750 8850 4100 8850
Wire Wire Line
	4100 8750 4750 8750
Wire Wire Line
	4750 8650 4100 8650
Wire Wire Line
	4100 8950 4750 8950
Wire Wire Line
	4750 9050 4100 9050
Text Label 4100 9150 2    50   ~ 0
INT0
Text Label 4100 9350 2    50   ~ 0
INT2
Text Label 4100 9250 2    50   ~ 0
INT1
Text Label 4100 9450 2    50   ~ 0
INT3
Text Label 4100 9550 2    50   ~ 0
~LOCK
Text Label 4050 9750 2    50   ~ 0
HLDA
Text Label 4050 9650 2    50   ~ 0
WR
Text Label 4050 9850 2    50   ~ 0
HOLD
Text Label 4100 9950 2    50   ~ 0
~READY
Text Label 4100 10050 2    50   ~ 0
ALE
Text Label 4100 10150 2    50   ~ 0
~BE0
Wire Wire Line
	4750 10150 4100 10150
Text Label 4100 10250 2    50   ~ 0
~BE1
Text Label 4100 10350 2    50   ~ 0
~BLAST
Text Label 4100 10450 2    50   ~ 0
~AS
Wire Wire Line
	4750 10250 4100 10250
Wire Wire Line
	4750 9850 4050 9850
Wire Wire Line
	4050 9750 4750 9750
Wire Wire Line
	4750 9650 4050 9650
Wire Wire Line
	4100 9550 4750 9550
Wire Wire Line
	4750 9450 4100 9450
Wire Wire Line
	4100 9350 4750 9350
Wire Wire Line
	4750 9250 4100 9250
Wire Wire Line
	4100 9150 4750 9150
Wire Wire Line
	4750 10450 4100 10450
Wire Wire Line
	4100 10350 4750 10350
Wire Wire Line
	4750 10050 4100 10050
Wire Wire Line
	4100 9950 4750 9950
$Comp
L 74xx:74LS245 U1
U 1 1 5F0CE52F
P 9950 6700
F 0 "U1" H 9950 7681 50  0000 C CNN
F 1 "74LS245" H 9950 7590 50  0000 C CNN
F 2 "Package_DIP:DIP-20_W7.62mm" H 9950 6700 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS245" H 9950 6700 50  0001 C CNN
	1    9950 6700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5F0D47B0
P 8150 6650
F 0 "C1" H 8265 6696 50  0000 L CNN
F 1 "0.1uF" H 8265 6605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 8188 6500 50  0001 C CNN
F 3 "~" H 8150 6650 50  0001 C CNN
	1    8150 6650
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR08
U 1 1 5F10825C
P 8150 5800
F 0 "#PWR08" H 8150 5650 50  0001 C CNN
F 1 "+3.3V" H 8165 5973 50  0000 C CNN
F 2 "" H 8150 5800 50  0001 C CNN
F 3 "" H 8150 5800 50  0001 C CNN
	1    8150 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9950 5900 9150 5900
Wire Wire Line
	9150 5900 9150 7100
Wire Wire Line
	9150 7100 9450 7100
Wire Wire Line
	9450 7200 9450 7500
Wire Wire Line
	9450 7500 9950 7500
Connection ~ 9950 7500
Wire Wire Line
	9450 6200 8750 6200
Text Label 8750 6200 2    50   ~ 0
~BLAST
$Comp
L Device:R R1
U 1 1 5F15E545
P 11100 6200
F 0 "R1" V 11100 6000 50  0000 C CNN
F 1 "330" V 11100 6200 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 11030 6200 50  0001 C CNN
F 3 "~" H 11100 6200 50  0001 C CNN
	1    11100 6200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10950 6200 10450 6200
$Comp
L Device:LED D1
U 1 1 5F17B740
P 11650 6200
F 0 "D1" H 11800 6200 50  0000 C CNN
F 1 "LED" H 11400 6200 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm" H 11650 6200 50  0001 C CNN
F 3 "~" H 11650 6200 50  0001 C CNN
	1    11650 6200
	-1   0    0    1   
$EndComp
Wire Wire Line
	11500 6200 11250 6200
Wire Wire Line
	11800 6200 12050 6200
Text Label 8750 6300 2    50   ~ 0
~LOCK
Wire Wire Line
	8750 6300 9450 6300
$Comp
L Device:R R2
U 1 1 5F1D9E52
P 11100 6300
F 0 "R2" V 11100 6100 50  0000 C CNN
F 1 "330" V 11100 6300 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 11030 6300 50  0001 C CNN
F 3 "~" H 11100 6300 50  0001 C CNN
	1    11100 6300
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D2
U 1 1 5F1D9E5C
P 11650 6300
F 0 "D2" H 11800 6300 50  0000 C CNN
F 1 "LED" H 11400 6300 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm" H 11650 6300 50  0001 C CNN
F 3 "~" H 11650 6300 50  0001 C CNN
	1    11650 6300
	-1   0    0    1   
$EndComp
Wire Wire Line
	11500 6300 11250 6300
Wire Wire Line
	11800 6300 12050 6300
Wire Wire Line
	12050 7500 12050 6900
Connection ~ 12050 6300
Wire Wire Line
	12050 6300 12050 6200
Wire Wire Line
	10950 6300 10450 6300
Text Label 8750 6400 2    50   ~ 0
~AS
Wire Wire Line
	8750 6400 9450 6400
$Comp
L Device:R R3
U 1 1 5F25BB1F
P 11100 6400
F 0 "R3" V 11100 6200 50  0000 C CNN
F 1 "330" V 11100 6400 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 11030 6400 50  0001 C CNN
F 3 "~" H 11100 6400 50  0001 C CNN
	1    11100 6400
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D3
U 1 1 5F25BB25
P 11650 6400
F 0 "D3" H 11800 6400 50  0000 C CNN
F 1 "LED" H 11400 6400 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm" H 11650 6400 50  0001 C CNN
F 3 "~" H 11650 6400 50  0001 C CNN
	1    11650 6400
	-1   0    0    1   
$EndComp
Wire Wire Line
	11500 6400 11250 6400
Wire Wire Line
	11800 6400 12050 6400
Connection ~ 12050 6400
Wire Wire Line
	12050 6400 12050 6300
Wire Wire Line
	10450 6400 10950 6400
Wire Wire Line
	9150 5900 8150 5900
Wire Wire Line
	8150 5900 8150 5800
Connection ~ 9150 5900
Wire Wire Line
	8150 6500 8150 5900
Connection ~ 8150 5900
Wire Wire Line
	8150 6800 8150 7500
Wire Wire Line
	8150 7500 9450 7500
Connection ~ 9450 7500
Wire Wire Line
	9950 7500 12050 7500
$Comp
L power:GNDREF #PWR09
U 1 1 5F35E2EC
P 9950 7500
F 0 "#PWR09" H 9950 7250 50  0001 C CNN
F 1 "GNDREF" H 9955 7327 50  0000 C CNN
F 2 "" H 9950 7500 50  0001 C CNN
F 3 "" H 9950 7500 50  0001 C CNN
	1    9950 7500
	1    0    0    -1  
$EndComp
Text Label 8750 6500 2    50   ~ 0
~READY
Wire Wire Line
	8750 6800 9450 6800
Wire Wire Line
	8750 6700 9450 6700
Wire Wire Line
	8750 6600 9450 6600
Wire Wire Line
	8750 6500 9450 6500
Text Label 8750 6600 2    50   ~ 0
~DEN
$Comp
L Device:R R4
U 1 1 5F438ACE
P 11100 6500
F 0 "R4" V 11100 6300 50  0000 C CNN
F 1 "330" V 11100 6500 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 11030 6500 50  0001 C CNN
F 3 "~" H 11100 6500 50  0001 C CNN
	1    11100 6500
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D4
U 1 1 5F438AD8
P 11650 6500
F 0 "D4" H 11800 6500 50  0000 C CNN
F 1 "LED" H 11400 6500 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm" H 11650 6500 50  0001 C CNN
F 3 "~" H 11650 6500 50  0001 C CNN
	1    11650 6500
	-1   0    0    1   
$EndComp
Wire Wire Line
	11500 6500 11250 6500
$Comp
L Device:R R5
U 1 1 5F458E7B
P 11100 6600
F 0 "R5" V 11100 6400 50  0000 C CNN
F 1 "330" V 11100 6600 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 11030 6600 50  0001 C CNN
F 3 "~" H 11100 6600 50  0001 C CNN
	1    11100 6600
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D5
U 1 1 5F458E85
P 11650 6600
F 0 "D5" H 11800 6600 50  0000 C CNN
F 1 "LED" H 11400 6600 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm" H 11650 6600 50  0001 C CNN
F 3 "~" H 11650 6600 50  0001 C CNN
	1    11650 6600
	-1   0    0    1   
$EndComp
Wire Wire Line
	11500 6600 11250 6600
$Comp
L Device:R R6
U 1 1 5F479A0E
P 11100 6700
F 0 "R6" V 11100 6500 50  0000 C CNN
F 1 "330" V 11100 6700 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 11030 6700 50  0001 C CNN
F 3 "~" H 11100 6700 50  0001 C CNN
	1    11100 6700
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D6
U 1 1 5F479A18
P 11650 6700
F 0 "D6" H 11800 6700 50  0000 C CNN
F 1 "LED" H 11400 6700 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm" H 11650 6700 50  0001 C CNN
F 3 "~" H 11650 6700 50  0001 C CNN
	1    11650 6700
	-1   0    0    1   
$EndComp
Wire Wire Line
	11500 6700 11250 6700
$Comp
L Device:R R7
U 1 1 5F49AE2E
P 11100 6800
F 0 "R7" V 11100 6600 50  0000 C CNN
F 1 "330" V 11100 6800 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 11030 6800 50  0001 C CNN
F 3 "~" H 11100 6800 50  0001 C CNN
	1    11100 6800
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D7
U 1 1 5F49AE38
P 11650 6800
F 0 "D7" H 11800 6800 50  0000 C CNN
F 1 "LED" H 11400 6800 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm" H 11650 6800 50  0001 C CNN
F 3 "~" H 11650 6800 50  0001 C CNN
	1    11650 6800
	-1   0    0    1   
$EndComp
Wire Wire Line
	11500 6800 11250 6800
$Comp
L Device:R R8
U 1 1 5F4BBEEC
P 11100 6900
F 0 "R8" V 11100 6700 50  0000 C CNN
F 1 "330" V 11100 6900 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 11030 6900 50  0001 C CNN
F 3 "~" H 11100 6900 50  0001 C CNN
	1    11100 6900
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D8
U 1 1 5F4BBEF6
P 11650 6900
F 0 "D8" H 11800 6900 50  0000 C CNN
F 1 "LED" H 11400 6900 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm" H 11650 6900 50  0001 C CNN
F 3 "~" H 11650 6900 50  0001 C CNN
	1    11650 6900
	-1   0    0    1   
$EndComp
Wire Wire Line
	11500 6900 11250 6900
Wire Wire Line
	11800 6500 12050 6500
Connection ~ 12050 6500
Wire Wire Line
	12050 6500 12050 6400
Wire Wire Line
	11800 6600 12050 6600
Connection ~ 12050 6600
Wire Wire Line
	12050 6600 12050 6500
Wire Wire Line
	11800 6700 12050 6700
Connection ~ 12050 6700
Wire Wire Line
	12050 6700 12050 6600
Wire Wire Line
	11800 6800 12050 6800
Connection ~ 12050 6800
Wire Wire Line
	12050 6800 12050 6700
Wire Wire Line
	11800 6900 12050 6900
Connection ~ 12050 6900
Wire Wire Line
	12050 6900 12050 6800
Wire Wire Line
	10450 6900 10950 6900
Wire Wire Line
	10950 6800 10450 6800
Wire Wire Line
	10450 6700 10950 6700
Wire Wire Line
	10950 6600 10450 6600
Wire Wire Line
	10450 6500 10950 6500
Wire Wire Line
	9450 6900 8750 6900
Wire Wire Line
	9800 1350 8900 1350
Text Label 8900 1350 2    50   ~ 0
MEGA_ACTIVE
Text Label 8750 6900 2    50   ~ 0
MEGA_ACTIVE
Wire Wire Line
	13050 8400 12350 8400
Text Label 12350 8400 2    50   ~ 0
PSU_STANDBY
Text Label 8750 6800 2    50   ~ 0
PSU_STANDBY
Text Label 8750 6700 2    50   ~ 0
POWER_OK
$Sheet
S 14100 800  1150 1100
U 5E94E0B6
F0 "Mega2560 Prototyping Area" 50
F1 "MEGA2560_PROTOTYPING_AREA.sch" 50
F2 "P0" B L 14100 900 50 
F3 "P1" B L 14100 1000 50 
F4 "P2" B L 14100 1100 50 
F5 "P3" B L 14100 1200 50 
F6 "P4" B L 14100 1300 50 
F7 "P5" B L 14100 1400 50 
F8 "P6" B L 14100 1500 50 
F9 "P7" B L 14100 1600 50 
F10 "P8" B L 14100 1700 50 
F11 "MISO" O R 15250 900 50 
F12 "MOSI" I R 15250 1000 50 
F13 "SCK" I R 15250 1100 50 
F14 "SCL" I R 15250 1200 50 
F15 "SDA" B R 15250 1300 50 
$EndSheet
Wire Wire Line
	4750 10550 4100 10550
Text Label 4100 10550 2    50   ~ 0
~RESET
$Comp
L Memory_EEPROM:25LCxxx U2
U 1 1 5FF6EA6C
P 13950 4050
F 0 "U2" H 13950 4531 50  0000 C CNN
F 1 "23LC1024" H 13950 4440 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_Socket" H 13950 4050 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21832H.pdf" H 13950 4050 50  0001 C CNN
	1    13950 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	13100 4150 13350 4150
Wire Wire Line
	13550 4050 12900 4050
$Comp
L power:+5V #PWR010
U 1 1 5FFFA351
P 12900 3650
F 0 "#PWR010" H 12900 3500 50  0001 C CNN
F 1 "+5V" H 12915 3823 50  0000 C CNN
F 2 "" H 12900 3650 50  0001 C CNN
F 3 "" H 12900 3650 50  0001 C CNN
	1    12900 3650
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 6001D45A
P 15200 4000
F 0 "C2" H 15315 4046 50  0000 L CNN
F 1 "0.1uF" H 15315 3955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 15238 3850 50  0001 C CNN
F 3 "~" H 15200 4000 50  0001 C CNN
	1    15200 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	15200 3850 15200 3750
Wire Wire Line
	15200 3750 13950 3750
Wire Wire Line
	13950 4350 15200 4350
Wire Wire Line
	15200 4350 15200 4150
Text Label 14350 3950 0    50   ~ 0
SCK
Text Label 14350 4050 0    50   ~ 0
MOSI
Text Label 14350 4150 0    50   ~ 0
MISO
$Comp
L Device:R R11
U 1 1 60064FFF
P 13350 4300
F 0 "R11" H 13420 4346 50  0000 L CNN
F 1 "10k" H 13420 4255 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 13280 4300 50  0001 C CNN
F 3 "~" H 13350 4300 50  0001 C CNN
	1    13350 4300
	1    0    0    -1  
$EndComp
Connection ~ 13350 4150
Wire Wire Line
	13350 4150 13550 4150
Wire Wire Line
	13350 4450 12950 4450
$Comp
L Device:R R10
U 1 1 60089A73
P 13250 3950
F 0 "R10" V 13457 3950 50  0000 C CNN
F 1 "10k" V 13366 3950 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 13180 3950 50  0001 C CNN
F 3 "~" H 13250 3950 50  0001 C CNN
	1    13250 3950
	0    -1   -1   0   
$EndComp
Wire Wire Line
	13400 3950 13550 3950
Wire Wire Line
	13100 3950 12950 3950
Wire Wire Line
	12900 3950 12900 3650
$Comp
L Device:R R9
U 1 1 600D34D7
P 12750 4050
F 0 "R9" V 12543 4050 50  0000 C CNN
F 1 "10k" V 12634 4050 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 12680 4050 50  0001 C CNN
F 3 "~" H 12750 4050 50  0001 C CNN
	1    12750 4050
	0    -1   1    0   
$EndComp
Wire Wire Line
	12600 3650 12600 4050
Wire Wire Line
	12600 3650 12900 3650
Connection ~ 12900 3650
Wire Wire Line
	12950 4450 12950 3950
Connection ~ 12950 3950
Wire Wire Line
	12950 3950 12900 3950
NoConn ~ 13800 6200
NoConn ~ 13300 6200
$Comp
L power:PWR_FLAG #FLG02
U 1 1 60211748
P 9900 8550
F 0 "#FLG02" H 9900 8625 50  0001 C CNN
F 1 "PWR_FLAG" H 9900 8723 50  0000 C CNN
F 2 "" H 9900 8550 50  0001 C CNN
F 3 "~" H 9900 8550 50  0001 C CNN
	1    9900 8550
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 8550 9900 8800
$Comp
L power:+5V #PWR0122
U 1 1 602362D5
P 9900 8800
F 0 "#PWR0122" H 9900 8650 50  0001 C CNN
F 1 "+5V" H 9915 8973 50  0000 C CNN
F 2 "" H 9900 8800 50  0001 C CNN
F 3 "" H 9900 8800 50  0001 C CNN
	1    9900 8800
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR0125
U 1 1 6028593D
P 15200 3750
F 0 "#PWR0125" H 15200 3600 50  0001 C CNN
F 1 "+5V" H 15215 3923 50  0000 C CNN
F 2 "" H 15200 3750 50  0001 C CNN
F 3 "" H 15200 3750 50  0001 C CNN
	1    15200 3750
	1    0    0    -1  
$EndComp
Connection ~ 15200 3750
$Comp
L power:GNDREF #PWR0126
U 1 1 60287B49
P 15200 4350
F 0 "#PWR0126" H 15200 4100 50  0001 C CNN
F 1 "GNDREF" H 15205 4177 50  0000 C CNN
F 2 "" H 15200 4350 50  0001 C CNN
F 3 "" H 15200 4350 50  0001 C CNN
	1    15200 4350
	1    0    0    -1  
$EndComp
Connection ~ 15200 4350
Text Label 9550 3750 2    50   ~ 0
~BE0
Text Label 9550 3850 2    50   ~ 0
ALE
Text Label 9550 3950 2    50   ~ 0
~READY
Wire Wire Line
	9500 4450 9800 4450
Text Label 9550 4050 2    50   ~ 0
HOLD
Wire Wire Line
	9500 4250 9800 4250
Text Label 9500 4250 2    50   ~ 0
WR
Wire Wire Line
	9500 4350 9800 4350
Text Label 9550 4150 2    50   ~ 0
HLDA
Text Label 9500 4350 2    50   ~ 0
~LOCK
Text Label 9500 4450 2    50   ~ 0
INT3
Text Label 9550 4650 2    50   ~ 0
INT1
Text Label 9550 4550 2    50   ~ 0
INT2
Text Label 9550 4750 2    50   ~ 0
INT0
$EndSCHEMATC
