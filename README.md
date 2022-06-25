# Embedded-System
Embedded System Lab
    
### Lab3 컴파일 및 실행
    sudo modprobe spicc
    sudo modprobe fbtft_device name=odroidc_tft32 rotate=270 gpios=reset:116,dc:115 speed=32000000 cs=0
    gcc Lab3.c -o LAB3 -lwiringPi -lwiringPiDev -lpthread -lcrypt -lm -lrt