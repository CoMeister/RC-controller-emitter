EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Trims board"
Date ""
Rev "1.0"
Comp "2CoB9 GNU"
Comment1 "Board for trim of 1 channel"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Switch:SW_Push SW1
U 1 1 5F524B50
P 4850 5150
F 0 "SW1" H 4850 5435 50  0000 C CNN
F 1 "SW_Push" H 4850 5344 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H4.3mm" H 4850 5350 50  0001 C CNN
F 3 "~" H 4850 5350 50  0001 C CNN
	1    4850 5150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW2
U 1 1 5F524B56
P 5350 5150
F 0 "SW2" H 5350 5435 50  0000 C CNN
F 1 "SW_Push" H 5350 5344 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H4.3mm" H 5350 5350 50  0001 C CNN
F 3 "~" H 5350 5350 50  0001 C CNN
	1    5350 5150
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J2
U 1 1 5F524B69
P 6150 5500
F 0 "J2" H 6230 5542 50  0000 L CNN
F 1 "JST_cable" H 6230 5451 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical" H 6150 5500 50  0001 C CNN
F 3 "~" H 6150 5500 50  0001 C CNN
	1    6150 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 5150 4650 5300
Wire Wire Line
	4650 5300 5150 5300
Wire Wire Line
	5150 5300 5150 5150
Wire Wire Line
	4650 5300 4650 5900
Wire Wire Line
	4650 5900 5850 5900
Wire Wire Line
	5850 5900 5850 5600
Connection ~ 4650 5300
Wire Wire Line
	5850 5400 5950 5400
Wire Wire Line
	5850 5600 5950 5600
Wire Wire Line
	5850 4500 5850 5400
Connection ~ 5550 4500
Wire Wire Line
	5550 5150 5550 4500
Wire Wire Line
	5050 5150 5050 4500
Wire Wire Line
	5350 4500 5550 4500
$Comp
L Device:R R2
U 1 1 5F524B62
P 5700 4500
F 0 "R2" H 5630 4454 50  0000 R CNN
F 1 "220ohm" H 5630 4545 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 5630 4500 50  0001 C CNN
F 3 "~" H 5700 4500 50  0001 C CNN
	1    5700 4500
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R1
U 1 1 5F524B5C
P 5200 4500
F 0 "R1" H 5130 4454 50  0000 R CNN
F 1 "220ohm" H 5130 4545 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 5130 4500 50  0001 C CNN
F 3 "~" H 5200 4500 50  0001 C CNN
	1    5200 4500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5950 5500 4550 5500
Wire Wire Line
	4550 5500 4550 4500
Wire Wire Line
	4550 4500 5050 4500
Connection ~ 5050 4500
Text Notes 4800 4350 0    50   ~ 0
6 items
Wire Notes Line
	4500 4400 6650 4400
Wire Notes Line
	6650 4400 6650 5950
Wire Notes Line
	6650 5950 4500 5950
Wire Notes Line
	4500 5950 4500 4400
Text Notes 5500 4350 0    50   ~ 0
!! Last module R2 = 2K !!
Text Notes 5750 5850 0    50   ~ 0
5V
Text Notes 5850 5000 0    50   ~ 0
Out\n
Text Notes 5700 5500 0    50   ~ 0
In
$EndSCHEMATC
