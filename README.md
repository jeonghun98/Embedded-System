# Embedded-System
Embedded System Lab

### Team Project 컴파일 및 실행
    sudo modprobe spicc
    sudo modprobe fbtft_device name=odroidc_tft32 rotate=270 gpios=reset:116,dc:115 speed=32000000 cs=0
    gcc Lab4.c -o lab4 -lwiringPi -lwiringPiDev -lpthread -lcrypt -lm -lrt
    
### Lab1 컴파일 및 실행
    gcc Lab1.c -o Lab -lwiringPi -lwiringPiDev -lpthread -lcrypt -lm -lrt
    ./Lab 1
    ./Lab 2 0x71
    ./Lab 2 0x41

### Lab2 컴파일 및 실행
    gcc Lab2.c -o clcd -lwiringPi -lwiringPiDev -lpthread -lcrypt -lm -lrt