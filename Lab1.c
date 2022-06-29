#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define LED0     22 // LED Pin - wiringPi pin 22 is Odroid-C1 GPIO 100.
#define LED1     23 // LED Pin - wiringPi pin 23 is Odroid-C1 GPIO 108.
#define LED2     24 // LED Pin - wiringPi pin 24 is Odroid-C1 GPIO 97.
#define LED3     10 // LED Pin - wiringPi pin 10 is Odroid-C1 GPIO CEO.
#define LED4     11 // LED Pin - wiringPi pin 11 is Odroid-C1 GPIO 118.
#define LED5     26 // LED Pin - wiringPi pin 26 is Odroid-C1 GPIO 99.
#define LED6     14 // LED Pin - wiringPi pin 14 is Odroid-C1 GPIO SCLK.

void Off(void);
void On1(int num1);
void On2(int num2);

int main(int argc, char **argv) {
//  printf ("Blink!\n") ;
  wiringPiSetup () ;
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);

  Off(); //Reset for App2

  srand(time(NULL));
  int temp, randNum[16], i = 0, k = 0;
  for (i = 0; i < 16; i++) //random number generation
  {
    temp = rand() % 16;
    randNum[i] = temp;
    for (k = 0; k < i; k++)
    {
        if (randNum[k] == temp) i--;
    }
  }

  if((strcmp(argv[1],"1") == 0)) {      //option 1
    if(argc > 2) {
        printf("App 1 does not require additional input options.\n");
        return -1;
    }
    int n = 0, j = 0;
    for(n = 0; n < 16; n++) {
      On1(n); delay(500); Off();
    }
    for (j = 0; j < 16; j++) { //random number on, off
      //printf ("%d ",randNum[j]) ;
      On1(randNum[j]); delay(500) ; Off();
    }
  }
  else if((strcmp(argv[1], "2")==0)) {   //option 2
    if(argc == 2) {
        printf("App2 requires an input of hexadecimal numbers.\n");
        printf("Please enter hexadecimal number.\n");
        return -1;
    }
    if(argc >= 4) {
        printf("App 2 requires only hexadecimal numeric input.\n");
        printf("Please enter only the option(2) and hexadecimal numbers.\n");
        return -1;
    }

    if((strlen(argv[2])) != 4) {
        printf("Please enter hexadecimal numbers as 2 digits.\n");
        printf("example : 0x71\n");
        return -1;
    }
    if((argv[2][0] != '0') || (argv[2][1] != 'x')) {
        printf("Please enter hexadecimal numbers.\n");
        printf("example : 0x71\n");
        return -1;
    }
    if((argv[2][2] < '0') || (argv[2][2] > 'f') || (argv[2][3] < '0') || (argv[2][3] > 'f')) {
        printf("Please enter hexadecimal numbers.\n");
        printf("example : 0x71\n");
        return -1;
    }

    int app2num = strtol(argv[2], NULL, 16);
    if(app2num > 0x7f) {
        printf("A number higher than 0x7f is not allowed\n");
        return -1;
    }
    int m = 0, num = 0; // num : number of led
    for(m = 0x01; m <= 0x40; m = m << 1) {
        if(app2num & m) {
            On2(num);
        }num++;
    }
  }
  else {
    printf("Only 1 and 2 area available for options\n");
    return -1;
  }
  return 0;
}

void Off(void) {
    digitalWrite(LED0, LOW);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);
    digitalWrite(LED6, LOW);
}

void On2(int num2) {
  switch(num2) {
  case 0 :
      digitalWrite(LED0, HIGH);
      break;
  case 1 :
      digitalWrite(LED1, HIGH);
      break;
  case 2 :
      digitalWrite(LED2, HIGH);
      break;
  case 3 :
      digitalWrite(LED3, HIGH);
      break;
  case 4 :
      digitalWrite(LED4, HIGH);
      break;
  case 5 :
      digitalWrite(LED5, HIGH);
      break;
  case 6 :
      digitalWrite(LED6, HIGH);
      break;
  }
}

void On1(int num1) {
  switch(num1) {
  case 0 :
      digitalWrite(LED0, HIGH);digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);digitalWrite(LED5, HIGH);
      break;
  case 1 :
      digitalWrite(LED1, HIGH);digitalWrite(LED2, HIGH);
      break;
  case 2 :
      digitalWrite(LED0, HIGH);digitalWrite(LED1, HIGH);
      digitalWrite(LED3, HIGH);digitalWrite(LED4, HIGH);
      digitalWrite(LED6, HIGH);
      break;
  case 3 :
      digitalWrite(LED0, HIGH);digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);digitalWrite(LED3, HIGH);
      digitalWrite(LED6, HIGH);
      break;
  case 4 :
      digitalWrite(LED1, HIGH);digitalWrite(LED2, HIGH);
      digitalWrite(LED5, HIGH);digitalWrite(LED6, HIGH);
      break;
  case 5 :
      digitalWrite(LED0, HIGH);digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);digitalWrite(LED5, HIGH);
      digitalWrite(LED6, HIGH);
      break;
  case 6 :
      digitalWrite(LED0, HIGH);digitalWrite(LED6, HIGH);
      digitalWrite(LED2, HIGH);digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);digitalWrite(LED5, HIGH);
      break;
  case 7 :
      digitalWrite(LED0, HIGH);digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);digitalWrite(LED5, HIGH);
      break;
  case 8 :
      digitalWrite(LED0, HIGH);digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);digitalWrite(LED5, HIGH);
      digitalWrite(LED6, HIGH);
      break;
  case 9 :
      digitalWrite(LED0, HIGH);digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);digitalWrite(LED3, HIGH);
      digitalWrite(LED5, HIGH);digitalWrite(LED6, HIGH);
      break;
  case 10 : //a
      digitalWrite(LED2, HIGH);digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);digitalWrite(LED6, HIGH);
      break;
  case 11 : //b
      digitalWrite(LED2, HIGH);digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);digitalWrite(LED5, HIGH);
      digitalWrite(LED6, HIGH);
      break;
  case 12 : //c
      digitalWrite(LED3, HIGH);digitalWrite(LED4, HIGH);
      digitalWrite(LED6, HIGH);
      break;
  case 13 : //d
      digitalWrite(LED1, HIGH);digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);digitalWrite(LED4, HIGH);
      digitalWrite(LED6, HIGH);
      break;
  case 14 : //e
      digitalWrite(LED0, HIGH);digitalWrite(LED1, HIGH);
      digitalWrite(LED3, HIGH);digitalWrite(LED4, HIGH);
      digitalWrite(LED5, HIGH);digitalWrite(LED6, HIGH);
      break;
  case 15 : //f
      digitalWrite(LED0, HIGH);digitalWrite(LED4, HIGH);
      digitalWrite(LED5, HIGH);digitalWrite(LED6, HIGH);
      break;
  }
}

