#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>

#define LCD_D4 2
#define LCD_D5 3
#define LCD_D6 1
#define LCD_D7 4
#define LCD_RS 7
#define LCD_EN 0
#define PLUS 5 //add
#define MINUS 6 //sub
#define BT0 12
#define BT1 13
#define BT2 14
#define BT3 21
#define BT4 22
#define BT5 23
#define BT6 24
#define BT7 10
#define BT8 11
#define BT9 26
#define EQL 27

void write4bits(unsigned char command)
{
   digitalWrite(LCD_D4, (command & 1));
   command >>=1;
   digitalWrite(LCD_D5, (command & 1));
   command >>=1;
   digitalWrite(LCD_D6, (command & 1));
   command >>=1;
   digitalWrite(LCD_D7, (command & 1));
   digitalWrite(LCD_EN, 1);
   delayMicroseconds(10);
   digitalWrite(LCD_EN, 0);
   delayMicroseconds(10);
}
void sendDataCmd4(unsigned char data)
{
   write4bits( ((data>>4)&0x0f) );
   write4bits( (data&0x0f) );
   delayMicroseconds(100);
}

void putCmd4(unsigned char cmd)
{
    digitalWrite(LCD_RS, 0);
    sendDataCmd4(cmd);
}

void putChar(char c){

  digitalWrite(LCD_RS, 1);

  sendDataCmd4(c);
}
void initialize_textlcd(){
   pinMode(LCD_RS, OUTPUT);  pinMode(LCD_EN, OUTPUT);
   pinMode(LCD_D4, OUTPUT);  pinMode(LCD_D5, OUTPUT);
   pinMode(LCD_D6, OUTPUT);  pinMode(LCD_D7, OUTPUT);
   digitalWrite(LCD_RS, 0);  digitalWrite(LCD_EN, 0);
   digitalWrite(LCD_D4, 0);  digitalWrite(LCD_D5, 0);
   digitalWrite(LCD_D6, 0);  digitalWrite(LCD_D7, 0);
   //2 keys on clcd module    
   pinMode (PLUS, INPUT);    pullUpDnControl (PLUS, PUD_UP);
   pinMode (MINUS, INPUT);  pullUpDnControl (MINUS, PUD_UP);
   //keypad on bread
   pinMode(BT0, INPUT);    pullUpDnControl (BT0, PUD_DOWN);
   pinMode(BT1, INPUT);    pullUpDnControl (BT1, PUD_DOWN);
   pinMode(BT2, INPUT);    pullUpDnControl (BT2, PUD_DOWN);
   pinMode(BT3, INPUT);    pullUpDnControl (BT3, PUD_DOWN);
   pinMode(BT4, INPUT);    pullUpDnControl (BT4, PUD_DOWN);
   pinMode(BT5, INPUT);    pullUpDnControl (BT5, PUD_DOWN);
   pinMode(BT6, INPUT);    pullUpDnControl (BT6, PUD_DOWN);
   pinMode(BT7, INPUT);    pullUpDnControl (BT7, PUD_DOWN);
   pinMode(BT8, INPUT);    pullUpDnControl (BT8, PUD_DOWN);
   pinMode(BT9, INPUT);    pullUpDnControl (BT9, PUD_DOWN);
   pinMode(EQL, INPUT);    pullUpDnControl (EQL, PUD_DOWN);
   delay(35);

   putCmd4(0x28);
   putCmd4(0x28);
   putCmd4(0x28);
   putCmd4 (0x0e);
   putCmd4 (0x02);
   delay(2);
   putCmd4 (0x01);
   delay(2);
}
int DigitalRead(char buf[], int i) {
   if (digitalRead(BT0) == 1) {buf[i] = '0'; return 1;}
   if (digitalRead(BT1) == 1) {buf[i] = '1'; return 1;}
   if (digitalRead(BT2) == 1) {buf[i] = '2'; return 1;}
   if (digitalRead(BT3) == 1) {buf[i] = '3'; return 1;}
   if (digitalRead(BT4) == 1) {buf[i] = '4'; return 1;}
   if (digitalRead(BT5) == 1) {buf[i] = '5'; return 1;}
   if (digitalRead(BT6) == 1) {buf[i] = '6'; return 1;}
   if (digitalRead(BT7) == 1) {buf[i] = '7'; return 1;}
   if (digitalRead(BT8) == 1) {buf[i] = '8'; return 1;}
   if (digitalRead(BT9) == 1) {buf[i] = '9'; return 1;}
   if (digitalRead(EQL) == 1) {buf[i] = '='; return 1;}
   if (digitalRead(PLUS) == 0) {buf[i] = '+'; return 1;}
   if (digitalRead(MINUS) == 0) {buf[i] = '-'; return 1;}
   else return 0;
}
//Reference : Data structure
#define STACK_SIZE 5
typedef struct {
    int stack[STACK_SIZE];
    int top;
} stacktype;
void init(stacktype* s)
{
    s->top = -1;
}
int is_empty(stacktype* s)
{
    return (s->top == -1);
}
int is_full(stacktype* s)
{
    return (s->top == (STACK_SIZE - 1));
}
void push(stacktype* s, int item)
{
    if (is_full(s)) printf("stack full\n");
    else s->stack[++(s->top)] = item;
}
int pop(stacktype* s)
{
    if (is_empty(s)) printf("stack empty\n");
    else return s->stack[(s->top)--];
}
int calculate(char buf[], char R[]) {
    stacktype s_num; stacktype s_op;
    init(&s_num); init(&s_op);
    int k, num = 0, n1 = 0, n2 = 0; //k = index
    int minus = 0, num_check = 0;
    char op;
    for (k = 0; k < strlen(buf); k++) {
        if (buf[k] >= '0' && buf[k] <= '9') {
            num = num * 10 + (buf[k] - '0'); num_check = 1;
        }
        else if (buf[k] == '+') {
            if (k == 0);
            else {
                if (minus == 0 && num_check == 1) {
                    push(&s_num, num); num = 0; num_check = 0;
                }
                else if (minus == 1 && num_check == 1) {
                    push(&s_num, -num); num = 0; num_check = 0; minus = 0;
                }
                else return -1;

                if (s_op.top == -1) push(&s_op, '+');
                else if (s_op.top == 0 && s_num.top == 1) {
                    n2 = pop(&s_num); n1 = pop(&s_num); op = pop(&s_op);
                    if (op == '+') push(&s_num, n1 + n2);
                    else if (op == '-') push(&s_num, n1 - n2);
                    push(&s_op, '+');
                }
                else return -1;
            }
        }
        else if (buf[k] == '-') {
            if (k == 0) minus = 1;
            else {
                if (minus == 0 && num_check == 1) {
                    push(&s_num, num); num = 0; num_check = 0;
                }
                else if (minus == 1 && num_check == 1) {
                    push(&s_num, -num); num = 0; num_check = 0; minus = 0;
                }
                else return -1;

                if (s_op.top == -1) push(&s_op, '-');
                else if (s_op.top == 0 && s_num.top == 1) {
                    n2 = pop(&s_num); n1 = pop(&s_num); op = pop(&s_op);
                    if (op == '+') push(&s_num, n1 + n2);
                    else if (op == '-') push(&s_num, n1 - n2);
                    push(&s_op, '-');
                }
                else return -1;
            }
        }
        else if (buf[k] == '=') {
            if (minus == 0 && num_check == 1) {
                push(&s_num, num); num = 0; num_check = 0;
            }
            else if (minus == 1 && num_check == 1) {
                push(&s_num, -num); num = 0; num_check = 0; minus = 0;
            }
            else return -1;

            if (s_op.top == 0 && s_num.top == 1) {
                n2 = pop(&s_num); n1 = pop(&s_num); op = pop(&s_op);
                if (op == '+') push(&s_num, n1 + n2);
                else if (op == '-') push(&s_num, n1 - n2);
            }
        }
        else return -1;
    }
    if (s_num.top == 0 && s_op.top == -1) {
       sprintf(R, "%d", pop(&s_num));return 0;
    }
    else return -1;
}

int main(int argc, char **argv){
   char buf1[40]; buf1[0] = 0;
   char buf_over[40] = "Overflow";
   int len_o = strlen(buf_over);
   char buf_invalid[40] = "Invalid operation";
   int len_i = strlen(buf_invalid);

   int i = 0, temp = 0; //(i : index, temp : return value)
   char ch;
   int result = -1; char R[40]; R[0] = 0;//calculation result
   wiringPiSetup();
   initialize_textlcd();

    while (1) {
        temp = DigitalRead(buf1, i);
        if (temp != 0) {
            if (result == 0) {
                putCmd4(0x01); delay(2); result = -1;
            }
            ch = buf1[i];
            putChar(ch); i++;
            buf1[i] = 0; temp = 0; delay(500);
        }
        if (ch == '=') {
            int k;
            result = calculate(buf1,R);
            if (result == -1) {
                putCmd4(0x01); delay(2);
                for (k = 0; k < len_i; k++) {
                   putChar(buf_invalid[k]);
                   if (k == 15) { putCmd4(0xC0); }
                }
                delay(2000);
                putCmd4(0x01); i = 0; temp = 0; buf1[0] = 0; ch = 0;
            }
            else if(result != -1) {
                int l = strlen(R);
                for (k = 0; k < l; k++) {
                    if (i == 16) { putCmd4(0xC0);}
                    else if (i == 32) {
                        delay(500);
                        putCmd4(0x01); delay(2);
                        for (i = 0; i < len_o; i++) putChar(buf_over[i]);
                        delay(2000);
                        putCmd4(0x01);
                        break;
                    }
                    putChar(R[k]); i++;
                }i = 0; temp = 0; buf1[0] = 0; ch = 0; R[0] = 0;
            }
        }
        if (i == 16) { putCmd4(0xC0); }
        else if (i == 32) {
            putCmd4(0x01); delay(2);
            for (i = 0; i < len_o; i++) putChar(buf_over[i]);
            delay(2000);
            putCmd4(0x01); i = 0; temp = 0; buf1[0] = 0;
        }
    }
}
