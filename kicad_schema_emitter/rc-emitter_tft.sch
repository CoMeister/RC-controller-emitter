EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "RC-emitter"
Date ""
Rev "1.0"
Comp "2CoB9 GNU"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Microchip_ATmega:ATmega328P-PU U0
U 1 1 5F4BE18A
P 5550 3250
F 0 "U0" H 4906 3296 50  0000 R CNN
F 1 "ATmega328P-PU" H 4906 3205 50  0000 R CNN
F 2 "Package_DIP:DIP-28_W7.62mm" H 5550 3250 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega328_P%20AVR%20MCU%20with%20picoPower%20Technology%20Data%20Sheet%2040001984A.pdf" H 5550 3250 50  0001 C CNN
	1    5550 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:Crystal Y1
U 1 1 5F4C630D
P 6600 2700
F 0 "Y1" H 6600 2968 50  0000 C CNN
F 1 "Crystal-16Mhz" H 6600 2877 50  0000 C CNN
F 2 "Crystal:Crystal_HC49-4H_Vertical" H 6600 2700 50  0001 C CNN
F 3 "~" H 6600 2700 50  0001 C CNN
	1    6600 2700
	0    1    1    0   
$EndComp
$Comp
L Device:C C0
U 1 1 5F4C78CB
P 6600 2400
F 0 "C0" H 6715 2446 50  0000 L CNN
F 1 "22pF" H 6715 2355 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D4.7mm_W2.5mm_P5.00mm" H 6638 2250 50  0001 C CNN
F 3 "~" H 6600 2400 50  0001 C CNN
	1    6600 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C2
U 1 1 5F4D0EC3
P 3800 4450
F 0 "C2" H 3918 4496 50  0000 L CNN
F 1 "10uF" H 3918 4405 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 3838 4300 50  0001 C CNN
F 3 "~" H 3800 4450 50  0001 C CNN
	1    3800 4450
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C4
U 1 1 5F4D1F5C
P 8850 2800
F 0 "C4" H 8968 2846 50  0000 L CNN
F 1 "220uF" H 8968 2755 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D6.3mm_P2.50mm" H 8888 2650 50  0001 C CNN
F 3 "~" H 8850 2800 50  0001 C CNN
	1    8850 2800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5F4D2677
P 3450 4000
F 0 "R2" H 3520 4046 50  0000 L CNN
F 1 "330" H 3520 3955 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3380 4000 50  0001 C CNN
F 3 "~" H 3450 4000 50  0001 C CNN
	1    3450 4000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5F4D2FCE
P 3150 4000
F 0 "R1" H 3220 4046 50  0000 L CNN
F 1 "510" H 3220 3955 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3080 4000 50  0001 C CNN
F 3 "~" H 3150 4000 50  0001 C CNN
	1    3150 4000
	1    0    0    -1  
$EndComp
$Comp
L Device:C 100nF1
U 1 1 5F4D3F2D
P 4100 4450
F 0 "100nF1" H 4215 4496 50  0000 L CNN
F 1 "C3" H 4215 4405 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P5.00mm" H 4138 4300 50  0001 C CNN
F 3 "~" H 4100 4450 50  0001 C CNN
	1    4100 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 3350 6150 3350
Wire Wire Line
	6600 2850 6150 2850
Wire Wire Line
	6150 2850 6150 2750
Connection ~ 6600 2850
Wire Wire Line
	6600 2550 6400 2550
Wire Wire Line
	6400 2550 6400 2650
Wire Wire Line
	6400 2650 6150 2650
$Comp
L power:GND #PWR0115
U 1 1 5F520452
P 5550 4750
F 0 "#PWR0115" H 5550 4500 50  0001 C CNN
F 1 "GND" H 5555 4577 50  0000 C CNN
F 2 "" H 5550 4750 50  0001 C CNN
F 3 "" H 5550 4750 50  0001 C CNN
	1    5550 4750
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0116
U 1 1 5F520E4B
P 5550 1750
F 0 "#PWR0116" H 5550 1600 50  0001 C CNN
F 1 "+5V" H 5565 1923 50  0000 C CNN
F 2 "" H 5550 1750 50  0001 C CNN
F 3 "" H 5550 1750 50  0001 C CNN
	1    5550 1750
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0117
U 1 1 5F521654
P 8200 4350
F 0 "#PWR0117" H 8200 4200 50  0001 C CNN
F 1 "+5V" H 8215 4523 50  0000 C CNN
F 2 "" H 8200 4350 50  0001 C CNN
F 3 "" H 8200 4350 50  0001 C CNN
	1    8200 4350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8300 4350 8200 4350
Wire Wire Line
	6300 1750 6300 2350
Wire Wire Line
	6300 2350 6150 2350
Wire Wire Line
	6400 1850 6400 2450
Wire Wire Line
	6400 2450 6150 2450
Wire Wire Line
	6450 1950 6450 2500
Wire Wire Line
	6450 2500 6300 2500
Wire Wire Line
	6300 2500 6300 2550
Wire Wire Line
	6300 2550 6150 2550
Wire Wire Line
	6250 2050 6250 2250
Wire Wire Line
	6250 2250 6150 2250
Wire Wire Line
	7450 2250 7450 1650
Wire Wire Line
	7450 1650 6200 1650
Wire Wire Line
	6200 1650 6200 2150
Wire Wire Line
	6200 2150 6150 2150
Wire Wire Line
	8450 1750 7800 1750
Wire Wire Line
	7900 3750 7900 1950
Connection ~ 7900 1950
Wire Wire Line
	7900 1950 6450 1950
Wire Wire Line
	7800 3850 7800 1750
Connection ~ 7800 1750
Wire Wire Line
	7800 1750 6300 1750
Wire Wire Line
	7700 3950 7700 2250
Connection ~ 7700 2250
Wire Wire Line
	7700 2250 7450 2250
Wire Wire Line
	7650 4050 7650 4450
Wire Wire Line
	7650 4450 6150 4450
Wire Wire Line
	7400 4150 7400 1600
Wire Wire Line
	7400 1600 6150 1600
Wire Wire Line
	6150 1600 6150 2050
$Comp
L power:+5V #PWR0120
U 1 1 5F5499EE
P 8850 2650
F 0 "#PWR0120" H 8850 2500 50  0001 C CNN
F 1 "+5V" H 8865 2823 50  0000 C CNN
F 2 "" H 8850 2650 50  0001 C CNN
F 3 "" H 8850 2650 50  0001 C CNN
	1    8850 2650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0121
U 1 1 5F54ADB2
P 8850 2950
F 0 "#PWR0121" H 8850 2700 50  0001 C CNN
F 1 "GND" H 8855 2777 50  0000 C CNN
F 2 "" H 8850 2950 50  0001 C CNN
F 3 "" H 8850 2950 50  0001 C CNN
	1    8850 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 4300 3950 4300
Wire Wire Line
	3950 4300 3950 4150
Wire Wire Line
	4100 4300 3950 4300
Connection ~ 3950 4300
Wire Wire Line
	3800 4600 3800 4650
Wire Wire Line
	3800 4650 3950 4650
Wire Wire Line
	4100 4650 4100 4600
$Comp
L power:GND #PWR0122
U 1 1 5F558B7E
P 3950 4700
F 0 "#PWR0122" H 3950 4450 50  0001 C CNN
F 1 "GND" H 3955 4527 50  0000 C CNN
F 2 "" H 3950 4700 50  0001 C CNN
F 3 "" H 3950 4700 50  0001 C CNN
	1    3950 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 4700 3950 4650
Connection ~ 3950 4650
Wire Wire Line
	3950 4650 4100 4650
$Comp
L power:+5V #PWR0123
U 1 1 5F55DEA3
P 4250 3850
F 0 "#PWR0123" H 4250 3700 50  0001 C CNN
F 1 "+5V" H 4265 4023 50  0000 C CNN
F 2 "" H 4250 3850 50  0001 C CNN
F 3 "" H 4250 3850 50  0001 C CNN
	1    4250 3850
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0124
U 1 1 5F5634B6
P 3150 4150
F 0 "#PWR0124" H 3150 3900 50  0001 C CNN
F 1 "GND" H 3155 3977 50  0000 C CNN
F 2 "" H 3150 4150 50  0001 C CNN
F 3 "" H 3150 4150 50  0001 C CNN
	1    3150 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 4150 3950 4150
Connection ~ 3950 4150
Wire Wire Line
	3150 3850 3450 3850
Wire Wire Line
	3950 4150 4250 4150
Text GLabel 4250 4150 2    50   Input ~ 0
3.2V
Text GLabel 9200 1650 2    50   Input ~ 0
3.2V
$Comp
L Connector_Generic:Conn_02x04_Top_Bottom J1
U 1 1 5F515514
P 8900 1750
F 0 "J1" H 8950 2067 50  0000 C CNN
F 1 "nrf24L01+PA+lna" H 8950 1976 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x04_P2.54mm_Vertical" H 8900 1750 50  0001 C CNN
F 3 "~" H 8900 1750 50  0001 C CNN
	1    8900 1750
	1    0    0    -1  
$EndComp
Text GLabel 8300 3650 1    50   Input ~ 0
3.2V
Wire Wire Line
	8300 4150 7400 4150
Wire Wire Line
	8300 4050 7650 4050
Wire Wire Line
	8300 3950 7700 3950
Wire Wire Line
	8300 3850 7800 3850
Wire Wire Line
	8300 3750 7900 3750
$Comp
L power:GND #PWR0114
U 1 1 5F51FB80
P 8300 4250
F 0 "#PWR0114" H 8300 4000 50  0001 C CNN
F 1 "GND" H 8305 4077 50  0000 C CNN
F 2 "" H 8300 4250 50  0001 C CNN
F 3 "" H 8300 4250 50  0001 C CNN
	1    8300 4250
	0    1    1    0   
$EndComp
$Comp
L Display_Graphipersoc:ST7735 screen1
U 1 1 5F4CE562
P 9500 3750
F 0 "screen1" H 10278 3421 50  0000 L CNN
F 1 "ST7735" H 10278 3330 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 9500 2350 50  0001 C CNN
F 3 "" H 9500 2350 50  0001 C CNN
	1    9500 3750
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0118
U 1 1 5F526096
P 8700 1650
F 0 "#PWR0118" H 8700 1400 50  0001 C CNN
F 1 "GND" H 8705 1477 50  0000 C CNN
F 2 "" H 8700 1650 50  0001 C CNN
F 3 "" H 8700 1650 50  0001 C CNN
	1    8700 1650
	0    1    1    0   
$EndComp
Wire Wire Line
	9500 2050 9500 1750
Wire Wire Line
	9500 1750 9200 1750
Wire Wire Line
	8600 2250 8600 1750
Wire Wire Line
	8600 1750 8700 1750
Wire Wire Line
	7700 2250 8600 2250
Wire Wire Line
	8550 1950 8550 1850
Wire Wire Line
	8550 1850 8700 1850
Wire Wire Line
	7900 1950 8550 1950
Wire Wire Line
	8500 1850 8500 1900
Wire Wire Line
	8500 1900 8700 1900
Wire Wire Line
	8700 1900 8700 1950
Wire Wire Line
	6400 1850 8500 1850
Wire Wire Line
	6250 2050 9500 2050
Wire Wire Line
	8450 1750 8450 2150
Wire Wire Line
	8450 2150 9300 2150
Wire Wire Line
	9300 2150 9300 1850
Wire Wire Line
	9300 1850 9200 1850
NoConn ~ 9950 3700
NoConn ~ 9950 3900
NoConn ~ 9950 4100
NoConn ~ 9950 4300
$Comp
L power:GND #PWR0119
U 1 1 5F55634C
P 6600 3250
F 0 "#PWR0119" H 6600 3000 50  0001 C CNN
F 1 "GND" H 6605 3077 50  0000 C CNN
F 2 "" H 6600 3250 50  0001 C CNN
F 3 "" H 6600 3250 50  0001 C CNN
	1    6600 3250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0125
U 1 1 5F5590A6
P 6600 2250
F 0 "#PWR0125" H 6600 2000 50  0001 C CNN
F 1 "GND" H 6605 2077 50  0000 C CNN
F 2 "" H 6600 2250 50  0001 C CNN
F 3 "" H 6600 2250 50  0001 C CNN
	1    6600 2250
	-1   0    0    1   
$EndComp
Wire Wire Line
	6600 2950 6600 2850
$Comp
L Device:C C1
U 1 1 5F4C8B29
P 6600 3100
F 0 "C1" H 6715 3146 50  0000 L CNN
F 1 "22pF" H 6715 3055 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D4.7mm_W2.5mm_P5.00mm" H 6638 2950 50  0001 C CNN
F 3 "~" H 6600 3100 50  0001 C CNN
	1    6600 3100
	1    0    0    -1  
$EndComp
Connection ~ 6600 2550
NoConn ~ 5650 1750
NoConn ~ 4950 2050
NoConn ~ 6150 2950
NoConn ~ 6150 3050
NoConn ~ 6150 3150
NoConn ~ 6150 3250
NoConn ~ 6150 3450
NoConn ~ 6150 3550
NoConn ~ 6150 3750
NoConn ~ 6150 3850
NoConn ~ 6150 3950
NoConn ~ 6150 4050
NoConn ~ 6150 4150
NoConn ~ 6150 4250
NoConn ~ 6150 4350
NoConn ~ 9200 1950
$Comp
L Regulator_Linear:LM317_TO-220 U1
U 1 1 5F4CF46E
P 3950 3850
F 0 "U1" H 3950 4092 50  0000 C CNN
F 1 "LM317_TO-220" H 3950 4001 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 3950 4100 50  0001 C CIN
F 3 "http://www.ti.com/lit/ds/symlink/lm317.pdf" H 3950 3850 50  0001 C CNN
	1    3950 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 3850 3450 3850
Connection ~ 3450 3850
Wire Wire Line
	7750 4800 7750 4850
$Comp
L power:GND #PWR0101
U 1 1 5F4ECAFF
P 7750 4800
F 0 "#PWR0101" H 7750 4550 50  0001 C CNN
F 1 "GND" H 7755 4627 50  0000 C CNN
F 2 "" H 7750 4800 50  0001 C CNN
F 3 "" H 7750 4800 50  0001 C CNN
	1    7750 4800
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J2
U 1 1 5F52FD47
P 4300 6100
F 0 "J2" V 4172 6280 50  0000 L CNN
F 1 "Conn_01x03" V 4263 6280 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical" H 4300 6100 50  0001 C CNN
F 3 "~" H 4300 6100 50  0001 C CNN
	1    4300 6100
	0    1    1    0   
$EndComp
Wire Wire Line
	4700 5600 4700 5900
Connection ~ 6150 5800
Wire Wire Line
	5600 5800 6150 5800
Wire Wire Line
	5600 5900 5600 5800
Wire Wire Line
	5500 5600 5500 5900
Wire Wire Line
	5200 5600 5500 5600
Wire Wire Line
	5200 5900 5200 5600
Wire Wire Line
	5100 5600 5100 5900
Wire Wire Line
	4800 5600 5100 5600
Wire Wire Line
	4800 5900 4800 5600
Wire Wire Line
	4400 5600 4700 5600
Wire Wire Line
	4400 5900 4400 5600
NoConn ~ 4300 5900
$Comp
L Connector_Generic:Conn_01x03 J5
U 1 1 5F597C52
P 5500 6100
F 0 "J5" V 5372 6280 50  0000 L CNN
F 1 "Conn_01x03" V 5463 6280 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical" H 5500 6100 50  0001 C CNN
F 3 "~" H 5500 6100 50  0001 C CNN
	1    5500 6100
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0109
U 1 1 5F597C4C
P 5400 5900
F 0 "#PWR0109" H 5400 5750 50  0001 C CNN
F 1 "+5V" H 5415 6073 50  0000 C CNN
F 2 "" H 5400 5900 50  0001 C CNN
F 3 "" H 5400 5900 50  0001 C CNN
	1    5400 5900
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J4
U 1 1 5F59480F
P 5100 6100
F 0 "J4" V 4972 6280 50  0000 L CNN
F 1 "Conn_01x03" V 5063 6280 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical" H 5100 6100 50  0001 C CNN
F 3 "~" H 5100 6100 50  0001 C CNN
	1    5100 6100
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0108
U 1 1 5F594809
P 5000 5900
F 0 "#PWR0108" H 5000 5750 50  0001 C CNN
F 1 "+5V" H 5015 6073 50  0000 C CNN
F 2 "" H 5000 5900 50  0001 C CNN
F 3 "" H 5000 5900 50  0001 C CNN
	1    5000 5900
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J3
U 1 1 5F591718
P 4700 6100
F 0 "J3" V 4572 6280 50  0000 L CNN
F 1 "Conn_01x03" V 4663 6280 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical" H 4700 6100 50  0001 C CNN
F 3 "~" H 4700 6100 50  0001 C CNN
	1    4700 6100
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0107
U 1 1 5F591712
P 4600 5900
F 0 "#PWR0107" H 4600 5750 50  0001 C CNN
F 1 "+5V" H 4615 6073 50  0000 C CNN
F 2 "" H 4600 5900 50  0001 C CNN
F 3 "" H 4600 5900 50  0001 C CNN
	1    4600 5900
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW10
U 1 1 5F4C0B93
P 6500 5450
F 0 "SW10" H 6500 5735 50  0000 C CNN
F 1 "SW_Push_up" H 6500 5644 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H4.3mm" H 6500 5650 50  0001 C CNN
F 3 "~" H 6500 5650 50  0001 C CNN
	1    6500 5450
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW11
U 1 1 5F4C194C
P 7050 5450
F 0 "SW11" H 7050 5735 50  0000 C CNN
F 1 "SW_Push_down" H 7050 5644 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H4.3mm" H 7050 5650 50  0001 C CNN
F 3 "~" H 7050 5650 50  0001 C CNN
	1    7050 5450
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW12
U 1 1 5F4C2046
P 7550 5450
F 0 "SW12" H 7550 5735 50  0000 C CNN
F 1 "SW_Push_ok/modify" H 7550 5644 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H4.3mm" H 7550 5650 50  0001 C CNN
F 3 "~" H 7550 5650 50  0001 C CNN
	1    7550 5450
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW9
U 1 1 5F4C2341
P 5950 5450
F 0 "SW9" H 5950 5735 50  0000 C CNN
F 1 "SW_Push_Menu/back" H 5950 5644 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H4.3mm" H 5950 5650 50  0001 C CNN
F 3 "~" H 5950 5650 50  0001 C CNN
	1    5950 5450
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0102
U 1 1 5F4ED492
P 7350 5450
F 0 "#PWR0102" H 7350 5300 50  0001 C CNN
F 1 "+5V" H 7365 5623 50  0000 C CNN
F 2 "" H 7350 5450 50  0001 C CNN
F 3 "" H 7350 5450 50  0001 C CNN
	1    7350 5450
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR0103
U 1 1 5F4EF986
P 6850 5450
F 0 "#PWR0103" H 6850 5300 50  0001 C CNN
F 1 "+5V" H 6865 5623 50  0000 C CNN
F 2 "" H 6850 5450 50  0001 C CNN
F 3 "" H 6850 5450 50  0001 C CNN
	1    6850 5450
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR0104
U 1 1 5F4EFBC9
P 6300 5450
F 0 "#PWR0104" H 6300 5300 50  0001 C CNN
F 1 "+5V" H 6315 5623 50  0000 C CNN
F 2 "" H 6300 5450 50  0001 C CNN
F 3 "" H 6300 5450 50  0001 C CNN
	1    6300 5450
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR0105
U 1 1 5F4EFD6B
P 5750 5450
F 0 "#PWR0105" H 5750 5300 50  0001 C CNN
F 1 "+5V" H 5765 5623 50  0000 C CNN
F 2 "" H 5750 5450 50  0001 C CNN
F 3 "" H 5750 5450 50  0001 C CNN
	1    5750 5450
	-1   0    0    1   
$EndComp
Wire Wire Line
	7550 5800 7750 5800
Connection ~ 7250 5800
Wire Wire Line
	7250 5800 7250 5450
Wire Wire Line
	7000 5800 7250 5800
Connection ~ 6700 5800
Wire Wire Line
	6700 5450 6700 5800
Wire Wire Line
	6450 5800 6700 5800
Wire Wire Line
	6150 5450 6150 5800
$Comp
L Device:R R13
U 1 1 5F5089FC
P 7400 5800
F 0 "R13" H 7330 5754 50  0000 R CNN
F 1 "220ohm" H 7330 5845 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7330 5800 50  0001 C CNN
F 3 "~" H 7400 5800 50  0001 C CNN
	1    7400 5800
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R12
U 1 1 5F508359
P 6850 5800
F 0 "R12" H 6780 5754 50  0000 R CNN
F 1 "220ohm" H 6780 5845 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 6780 5800 50  0001 C CNN
F 3 "~" H 6850 5800 50  0001 C CNN
	1    6850 5800
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R11
U 1 1 5F508016
P 6300 5800
F 0 "R11" H 6230 5754 50  0000 R CNN
F 1 "220ohm" H 6230 5845 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 6230 5800 50  0001 C CNN
F 3 "~" H 6300 5800 50  0001 C CNN
	1    6300 5800
	0    -1   -1   0   
$EndComp
Connection ~ 7750 5450
Wire Wire Line
	7750 5800 7750 5450
$Comp
L power:+5V #PWR0106
U 1 1 5F53F044
P 4200 5900
F 0 "#PWR0106" H 4200 5750 50  0001 C CNN
F 1 "+5V" H 4215 6073 50  0000 C CNN
F 2 "" H 4200 5900 50  0001 C CNN
F 3 "" H 4200 5900 50  0001 C CNN
	1    4200 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R15
U 1 1 5F4E9DD2
P 7750 5300
F 0 "R15" H 7680 5254 50  0000 R CNN
F 1 "2Kohm" H 7680 5345 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7680 5300 50  0001 C CNN
F 3 "~" H 7750 5300 50  0001 C CNN
	1    7750 5300
	-1   0    0    1   
$EndComp
$Comp
L Device:R R14
U 1 1 5F4EA099
P 7750 5000
F 0 "R14" H 7680 4954 50  0000 R CNN
F 1 "10Kohm" H 7680 5045 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7680 5000 50  0001 C CNN
F 3 "~" H 7750 5000 50  0001 C CNN
	1    7750 5000
	-1   0    0    1   
$EndComp
Wire Wire Line
	7750 5150 6400 5150
Connection ~ 7750 5150
Wire Wire Line
	6400 3350 6400 5150
Text Notes 4100 6350 0    79   ~ 0
4X JST for trims
Text Notes 1750 4250 0    79   ~ 0
5V to 3.2V for \nnRF24L01+ and \nLED of ST7735
Wire Notes Line
	3000 3500 3000 5000
Wire Notes Line
	3000 5000 4500 5000
Wire Notes Line
	4500 5000 4500 3500
Wire Notes Line
	4500 3500 3000 3500
$EndSCHEMATC