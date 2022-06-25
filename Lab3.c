#include <stdio.h>
#include <stdlib.h> /* for exit */
#include <unistd.h> /* for open/close .. */
#include <fcntl.h> /* for O_RDWR */
#include <sys/ioctl.h> /* for ioctl */
#include <sys/mman.h> /* for mmap */
#include <linux/fb.h> /* for fb_var_screeninfo, FBIOGET_VSCREENINFO */
#include <string.h>
#include <wiringPi.h>

#define FBDEVFILE "/dev/fb2"
typedef unsigned char ubyte;

unsigned short makepixel(ubyte r, ubyte g, ubyte b)
{return (unsigned short)(((r>>3)<<11)|((g>>2)<<5)|(b>>3));}

#define BT1 24
#define BT2 23
#define BT3 27
#define BT4 22
#define BT5 21
#define BT6 26
#define BT7 7
#define BT8 6
#define BT9 11
#define LEFT 1
#define RIGHT 5
#define DEL 4

int alphabet_pixel[27][50][2] = {
  {     {4,18},{4,19},{5,17},{5,18},{5,19},{5,20},{6,18},{6,19}
  },
  {     {8,7},{7,8},{7,9},{7,10},{7,11},{8,12},{14,5},{9,6},{10,6},{11,6},{12,6},{13,6},{14,6},{14,7},{14,8},{14,9},{14,10},{14,11},{14,12},
        {9,13},{10,13},{11,13},{12,13},{13,13},{14,13},{14,14},{14,15},{14,16},{14,17},{14,18},{14,19},{14,20},{14,21}
  },
  {     {5,6},{6,6},{7,6},{8,6},{9,6},{10,6},{11,6},{12,6},{13,6},{14,6},{15,6},{16,6},{16,7},{15,8},{14,9},{13,10},{12,11},{11,12},{10,13},
        {9,14},{8,15},{7,16},{6,17},{5,18},{5,19},{6,19},{7,19},{8,19},{9,19},{10,19},{11,19},{12,19},{13,19},{14,19},{15,19},{16,19}
  },
  {     {6,8}, {6,7}, {7,7}, {7,6}, {8,6}, {8,5}, {9,5}, {10,5}, {11,5},{12,5}, {13,5}, {13,6}, {14,6}, {14,7}, {15,7}, {15,8}, {15,9},{15,10}, 
        {12,18}, {13,18}, {14,17},{15,11}, {15,12}, {15,13}, {15,14}, {15,15}, {15,16}, {15,17}, {15,18}, {15,19}, {8,11}, {9,11}, {10,11}, 
        {11,11}, {12,11},{13,11}, {14,11}, {7, 12}, {6,13}, {6,14}, {6,15}, {6,16},{6,17}, {7,7}, {7,18}, {8,18}, {9,18}, {10,18}, {11,18}
  },
  {     {7,10},{5,3},{5,4},{5,5},{5,6},{5,7},{5,8}, {5,9},{5,10},{5,11},{6,11},{5,12},{6,12}, {5,13}, {5,14}, {5,15}, {5,16}, {5,17}, {6,17},
        {5,18}, {6,18}, {5, 19}, {5,20},{8,9},{9,9}, {10,9}, {11,9}, {12,9},{13,10},{14,11}, {14,12},{15,13}, {15,14},{15,15},{15,16},{14,17},
        {14,18},{13,19},{8,20},{9,20},{10,20},{11,20}, {12,20},{7,19}
  },
  {     {18,7},{17,6},{16,5},{15,4},{14,4},{13,4},{12,4},{11,4},{10,4},{9,4},{8,5},{7,6},{6,7},{5,8},{5,9},{5,10},{5,11},{5,12},{5,13},{5,14},
        {6,15},{6,16},{7,17},{8,18},{9,19},{10,19},{11,19},{12,19},{13,19},{14,19},{15,19},{16,18},{17,17},{18,16}
  },
  {     {15,3},{15,4},{8,9},{9,9},{10,9},{11,9},{12,9},{13,10},{7,10},{6,11},{6,12},{15,5},{15,6},{15,7},{15,8},{15,9},{15,10},{14,11},{15,11},
        {15,12},{15,13},{15,14},{15,15},{15,16},{15,17},{14,18},{15,18},{15,19},{15,20},{5,13},
        {5,14},{5,15},{5,16},{6,17},{6,18},{7,19},{13,19},{8,20},{9,20},{10,20},{11,20},{12,20}
  },
  {     {8,9}, {9,9}, {10,9}, {11,9}, {12,9}, {7,10},{13,10}, {6,11}, {14,11}, {5,12}, {6,12},{14,12},{5,13}, {15,13}, {5,14}, {6,14}, {7,14},
        {8,14},{9,14}, {10,14}, {11,14}, {12,14}, {13,14},{14,14}, {15,14}, {5,15}, {5,16},{6,17},
        {15,17}, {6,18},{14,18}, {7,19}, {13,19},{8,20},{9,20}, {10,20}, {11,20},{12,20}
  },
  {     {12,3},{13,3},{11,4},{14,4},{10,5},{15,5},{10,6},{15,6},{10,7}, {10,8},{10,9},{10,10},{6,11},{7,11},{8,11},{9,11},{10,11},{11,11},
        {12,11},{13,11},{14,11},{10,12},{10,13},{10,14},{10,15},{10,16},{10,17},{10,18},{10,19},{10,20}
  },
  {     {8,5},{7,6},{7,7},{7,8},{7,9},{7,10},{8,11},{8,19},{9,4},{10,4},{11,4},{12,4},{14,4},{13,5},{7,18},{14,5},{14,6},{14,7},{14,8},
        {14,9},{7,17},{14,10},{13,11},{14,11},{9,12},{12,20},{10,12},{11,12},{12,12},{14,12},{11,20},
        {14,13},{14,14},{14,15},{14,16},{10,20},{14,17},{14,18},{13,19},{9,20}
  },
  {     {7,3},{7,4},{7,5},{7,6},{7,7},{7,8},{7,9},{7,10},{7,11},{7,12},{7,13},{7,14},{7,15},{7,16},{7,17},{7,18},{7,19},{7,20},{8,12},{9,11},
        {10,11},{11,11},{12,11},{13,12},{14,13},{14,14},{14,15},{14,16},{14,17},{14,18},{14,19},{14,20}
  },
  {     {10,3},{10,4},{10,5},{10,8},{10,9},{10,10},{10,11},{10,12},{10,13},{10,14},{10,15},{10,16},{10,17},{10,18},{10,19},{10,20},{9,4},{11,4}
  },
  {     {10,3},{10,4},{10,5},{10,8},{10,9},{10,10},{10,11},{10,12},{10,13},{10,14},{10,15},{10,16},{10,17},{10,18},{9,4},{11,4},{9,19},{8,20},{7,20},{6,19},{5,18}
  },
  {     {15,4},{14,5},{13,6},{12,7},{11,8},{10,9},{9,10},{6,2},{6,3},{6,4},{6,5},{6,6},{6,7},{6,8},{6,9},{6,10},{6,11},{8,11},{6,12},{7,12},{6,13},{8,13},
        {6,14},{6,15},{6,16},{6,17},{6,18},{6,19},{6,20},{9,14},{10,15},{11,16},{12,17},{13,18},{14,19},{15,20}
  },
  {     {11,2},{11,3},{11,4},{11,5},{11,6},{11,7},{11,8},{11,9},{11,10},{11,11},{11,12},{11,13},{11,14},{11,15},{11,16},{11,17},{11,18},{11,19},{11,20}
  },
  {     {6,9},{7,9},{8,9},{5,10},{3,9},{3,10},{3,11},{4,11},{3,12},{3,13},{3,14},{3,15},{3,16},
        {3,17},{3,18},{3,19},{3,20},{12,9},{13,9},{14,9},{9,10},{11,10},{15,10},{10,11},{16,11},{10,12},{16,12},{10,13},{16,13},{10,14},{16,14},{10,15},{16,15},
        {10,16},{16,16},{10,17},{16,17},{10,18},{16,18},{10,19},{16,19},{10,20},{16,20}
  },
  {     {6,9},{6,10},{6,11},{6,12},{6,13},{6,14},{6,15},{6,16},{6,17},{6,18},{6,19},{6,20},{7,11},{8,10},{9,9},{10,9},{11,9},{12,10},{13,11},{13,12},{13,13},
        {13,14},{13,15},{13,16},{13,17},{13,18},{13,19},{13,20}
  },
  {     {6,6},{5,7},{5,8},{4,9},{4,10},{4,11},{4,12},{4,13},{4,14},{5,15},{5,16},{8,4},{9,4},{10,4},{11,4},{12,4},{7,5},{13,5},{14,6},{15,7},{15,8},{16,9},
        {16,10},{16,11},{16,12},{16,13},{16,14},{15,15},{15,16},{14,17},{7,18},{13,18},{8,19},{9,19},{10,19},{11,19},{12,19},{6,17}
  },
  {     {6,5},{6,6},{6,7},{6,8},{6,9},{6,10},{6,11},{6,12},{6,13},{6,14},{6,15},{6,16},{6,17},{6,18},{6,19},{6,20},{6,21},{7,6},{8,6},{9,6},{10,6},{11,6},
        {12,7},{13,8},{13,9},{13,10},{13,11},{12,12},{11,13},{10,13},{9,13},{8,13},{7,13}
  },
  {     {6,5},{6,6},{6,7},{6,8},{6,9},{6,10},{6,11},{6,12},{6,13},{6,14},{6,15},{6,16},{6,17},{6,18},{6,19},{6,20},{7,8},{8,7},{9,6},{10,6},{11,6},{12,6},{13,6}
  },
  {     {15,8},{6,9},{6,10},{6,11},{14,7},{13,6},{12,6},{11,6},{10,6},{9,6},{8,7},{7,8},{7,12},{8,13},{9,13},{10,13},{11,13},{12,13},{13,13},{14,14},{15,15},
        {15,16},{15,17},{14,18},{13,19},{12,19},{11,19},{10,19},{9,19},{8,19},{7,19}
  },
  {     {11,4},{11,5},{11,6},{11,7},{11,8},{11,9},{11,10},{11,11},{11,12},{11,13},{11,14},{11,15},{11,16},{11,17},{11,18},{11,19},{11,20},{12,20},{13,20},
        {14,20},{6,8},{7,8},{8,8},{9,8},{10,8},{12,8},{13,8},{14,8},{15,8},{16,8}
  },
  {     {15,7},{15,8},{15,9},{15,10},{15,11},{15,12},{15,13},{15,14},{15,15},{15,16},{15,17},{15,18},{15,19},{15,20},{14,17},{13,18},{12,19},{11,20},{10,20},
        {9,20},{8,20},{7,20},{6,19},{5,18},{4,7},{4,8},{4,9},{4,10},{4,11},{4,12},{4,13},{4,14},{4,15},{4,16},{4,17}
  },
  {     {4,7},{4,8},{18,7},{18,8},{17,9},{17,10},{5,9},{5,10},{6,11},{6,12},{7,13},{7,14},{8,15},{8,16},{16,11},{16,12},{15,13},{15,14},{14,15},{14,16},{9,17},
        {13,17},{9,18},{13,18},{10,19},{12,19},{11,20}
  },
  {     {2,9},{2,10},{2,11},{3,12},{3,13},{3,14},{4,15},{4,16},{4,17},{5,18},{5,19},{20,9},{20,10},{20,11},{19,12},{19,13},{19,14},{8,15},{8,16},{8,17},{11,9},
        {10,10},{12,10},{10,11},{12,11},{9,12},{13,12},{9,13},{13,13},{9,14},{13,14},{14,15},{14,16},{14,17},{15,18},{17,18},{15,19},{17,19},{16,20},{18,15},
        {18,16},{18,17},{7,18},{7,19},{6,20}
  },
  {     {4,6},{18,6},{5,7},{17,7},{6,8},{16,8},{15,9},{7,9},{8,10},{14,10},{13,11},{9,11},{10,12},{12,12},{11,13},{10,14},{12,14},{9,15},{8,16},{13,15},{14,16},
        {15,17},{16,18},{7,17},{6,18},{17,19},{5,19},{4,20},{18,20}
  },
  {     {6,5},{6,6},{16,5},{16,6},{7,7},{7,8},{8,9},{8,10},{15,7},{15,8},{14,9},{14,10},{9,11},{13,11},{9,12},{13,12},{10,13},{12,13},{11,14},{10,15},{10,16},
        {9,17},{9,18},{8,19},{8,20},{7,21}
  }
};
int length[27] = {8,33,36,49,44,34,42,38,30,40,32,18,21,36,19,43,28,38,33,23,31,30,
                   35,27,45,29,26};
                //.,q, z, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, r, s, t,
                // u, v, w, x, y

int b[24][2] = {
    {0,23},{1,23},{2,23},{3,23},{4,23},{5,23},{6,23},{7,23},{8,23},{9,23},{10,23},
    {11,23},{12,23},{13,23},{14,23},{15,23},{16,23},{17,23},{18,23},{19,23},
    {20,23},{21,23},{22,23},{23,23}
};

typedef struct listnode{
    int data;
    int index;
    struct listnode* link;
}listnode;

listnode* insert_first(listnode* head, int value,int index)
{
    listnode* p = (listnode*)malloc(sizeof(listnode));
    p->data = value;
    p->index = index;
    p->link = head;
    head = p;
    return head;
}
listnode* insert(listnode* head, listnode* pre, int value, int index)
{
    listnode* p = (listnode*)malloc(sizeof(listnode));
    p->data = value;
    p->index = index;
    p->link = pre->link;
    pre->link = p;
    return head;
}
listnode* insert_last(listnode* head, int value, int index)
{
    if (head == NULL) {
        head = insert_first(head, value,index); return head;
    }
    listnode* p = (listnode*)malloc(sizeof(listnode));
    p->data = value;
    p->index = index;
    p->link = NULL;
    listnode* la = head;
    while ((la->link) != NULL) {
        la = la->link;
    }
    la->link = p;
    return head;
}
listnode* delete_first(listnode* head)
{
    listnode* removed;
    if (head == NULL) return NULL;
    removed = head;
    head = removed->link;
    free(removed);
    return head;
}
listnode* delete(listnode* head, listnode* pre)
{
    listnode* removed;
    removed = pre->link;
    pre->link = removed->link;
    free(removed);
    return head;
}
void print_list(listnode* head)
{
    listnode* p = head;
    for (; p != NULL; p = p->link)
        printf("%d->", p->data);
    printf("NULL \n");
}
listnode* search_list(listnode* head, int x)
{
    listnode* p = head;
    while (p != NULL) {
        if (p->index == x) return p;
        p = p->link;
    }
    return NULL;
}
int list_len(listnode* head)
{
    int  i = 0; listnode* p = head;
    for (; p != NULL; p = p->link) i++;
    return i;
}
int index_search(listnode* head)
{
    int index = 0; listnode* p = head;
    for (; p != NULL; p = p->link) index = p->index;
    return index;
}
void blink_b(struct fb_var_screeninfo fbvar, unsigned short* pfbdata,
    int offset, unsigned short pixel, int x, int y)
{
    int i; int X; int Y;
    for (i = 0; i < 24; i++) {
        X = b[i][0]; Y = b[i][1];
        offset = (y + Y) * fbvar.xres + (x + X);
        pixel = makepixel(0, 0, 0);
        *(pfbdata + offset) = pixel;
    }
    delay(80);
}
void blink_w(struct fb_var_screeninfo fbvar, unsigned short* pfbdata,
    int offset, unsigned short pixel, int x, int y)
{
    int i; int X; int Y;
    for (i = 0; i < 24; i++) {
        X = b[i][0]; Y = b[i][1];
        offset = (y + Y) * fbvar.xres + (x + X);
        pixel = makepixel(255, 255, 255);
        *(pfbdata + offset) = pixel;
    }
    delay(80);
}
void pixel_fn(struct fb_var_screeninfo fbvar, unsigned short* pfbdata,
    int offset, unsigned short pixel, int x, int y, int eng)
{
    int i; int X; int Y;
    int L = length[eng];
    for (i = 0; i < L; i++) {
        X = alphabet_pixel[eng][i][0]; Y = alphabet_pixel[eng][i][1];
        offset = (y + Y) * fbvar.xres + (x + X);
        pixel = makepixel(255, 255, 255);
        *(pfbdata + offset) = pixel;
    }
}
void pixel_fn_last(struct fb_var_screeninfo fbvar, unsigned short* pfbdata,
    int offset, unsigned short pixel, int x, int y, int eng)
{
    int i; int X; int Y;
    int L = length[eng];
    for (i = 0; i < L; i++) {
        X = alphabet_pixel[eng][i][0]; Y = alphabet_pixel[eng][i][1];
        if (Y > 15) continue;
        offset = (y + Y) * fbvar.xres + (x + X);
        pixel = makepixel(255, 255, 255);
        *(pfbdata + offset) = pixel;
    }
}
void initialize() {
    pinMode(LEFT, INPUT);    pullUpDnControl(LEFT, PUD_UP);
    pinMode(RIGHT, INPUT);  pullUpDnControl(RIGHT, PUD_UP);
    pinMode(DEL, INPUT);        pullUpDnControl(DEL, PUD_UP);

    pinMode(BT1, INPUT);    pullUpDnControl(BT1, PUD_DOWN);
    pinMode(BT2, INPUT);    pullUpDnControl(BT2, PUD_DOWN);
    pinMode(BT3, INPUT);    pullUpDnControl(BT3, PUD_DOWN);
    pinMode(BT4, INPUT);    pullUpDnControl(BT4, PUD_DOWN);
    pinMode(BT5, INPUT);    pullUpDnControl(BT5, PUD_DOWN);
    pinMode(BT6, INPUT);    pullUpDnControl(BT6, PUD_DOWN);
    pinMode(BT7, INPUT);    pullUpDnControl(BT7, PUD_DOWN);
    pinMode(BT8, INPUT);    pullUpDnControl(BT8, PUD_DOWN);
    pinMode(BT9, INPUT);    pullUpDnControl(BT9, PUD_DOWN);
}

int DigitalRead() {
    if (digitalRead(BT1) == 1) { return 1; }//0
    if (digitalRead(BT2) == 1) { return 4; }//3
    if (digitalRead(BT3) == 1) { return 7; }//6
    if (digitalRead(BT4) == 1) { return 10; }//9
    if (digitalRead(BT5) == 1) { return 13; }//12
    if (digitalRead(BT6) == 1) { return 16; }//15
    if (digitalRead(BT7) == 1) { return 19; }//18
    if (digitalRead(BT8) == 1) { return 22; }//21
    if (digitalRead(BT9) == 1) { return 25; }//24
    if (digitalRead(LEFT) == 0) { return 30; }
    if (digitalRead(DEL) == 0) { return 31; }
    if (digitalRead(RIGHT) == 0) { return 32; }
    else return -1;
}
int xy[80][2] = {
    {0,0}, {28,0}, {56,0}, {84,0}, {112,0},{140,0},{168,0}, {196,0},{224,0},{252,0},
    {0,32}, {28,32}, {56,32}, {84,32}, {112,32},{140,32},{168,32}, {196,32},{224,32},{252,32},
    {0,64}, {28,64}, {56,64}, {84,64}, {112,64},{140,64},{168,64}, {196,64},{224,64},{252,64},
    {0,96}, {28,96}, {56,96}, {84,96}, {112,96},{140,96},{168,96}, {196,96},{224,96},{252,96},
    {0,128}, {28,128}, {56,128}, {84,128}, {112,128},{140,128},{168,128}, {196,128},{224,128},{252,128},
    {0,160}, {28,160}, {56,160}, {84,160}, {112,160},{140,160},{168,160}, {196,160},{224,160},{252,160},
    {0,192}, {28,192}, {56,192}, {84,192}, {112,192},{140,192},{168,192}, {196,192},{224,192},{252,192},
    {0,224}, {28,224}, {56,224}, {84,224}, {112,224},{140,224},{168,224}, {196,224},{224,224},{252,224}
};

int main()
{
    int fbfd;
    int ret;
    struct fb_var_screeninfo fbvar;
    unsigned short pixel;
    int offset;

    unsigned short* pfbdata;

    fbfd = open(FBDEVFILE, O_RDWR);
    if (fbfd < 0) { perror("fbdev open"); exit(1); }

    ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &fbvar);
    if (ret < 0) { perror("fbdev ioctl"); exit(1); }

    if (fbvar.bits_per_pixel != 16)
    {
        fprintf(stderr, "bpp is not 16\n"); exit(1);
    }

    pfbdata = (unsigned short*)mmap(0, fbvar.xres * fbvar.yres * 16 / 8,
        PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

    if ((unsigned)pfbdata == (unsigned)-1)
    {
        perror("fbdev mmap"); exit(1);
    }

    //lcd clear
    int j, k;
    for (j = 0; j < 320; j++) {
        for (k = 0; k < 240; k++) {
            offset = (k)*fbvar.xres + (j);
            pixel = makepixel(0, 0, 0);
            *(pfbdata + offset) = pixel;
        }
    }
    wiringPiSetup();
    initialize();

    listnode* head = NULL;
    listnode* p = NULL;

    int first = 0;
    int index = 0;

    int i = -1, temp = 0, but_count = 0;
    int temp_i = 0;
    while (1) {
        if (index > 79) {
            for (j = 0; j < 320; j++) {
                for (k = 0; k < 240; k++) {
                    offset = (k)*fbvar.xres + (j);
                    pixel = makepixel(0, 0, 0);
                    *(pfbdata + offset) = pixel;
                }
            }
            k = list_len(head);
            for (j = 0; j < k; j++) {
                head = delete_first(head);
            }
            first = 0; index = 0;
            i = -1; temp = 0; but_count = 0; temp_i = 0;
        }
        if (index < 70) { //blink
            blink_b(fbvar, pfbdata, offset, pixel, xy[index][0], xy[index][1]);
        }
        i = DigitalRead(); //button
        if (i == 30) // left
        {
            if (index == 0) { but_count = 0; temp = -1; temp_i = -1; }
            else {
                if(temp != -1) 
                    head = insert_last(head, temp_i, index);
                temp = -1; temp_i = -1; index--;
            }
            i = -1;
            delay(200);

        }
        if (i == 32) //right
        {
            if (index == 79) {but_count = 0; temp = -1; temp_i = -1;}
            else {
                head = insert_last(head, temp_i, index);
                temp = -1; temp_i = -1; index++;
            }
            i = -1;
            delay(200);
        }
        if (i >= 1 && i <= 25) {
            if (first == 0 && index == 0) { //first
               temp = i; temp_i = i; first = 1;
               if (index < 70) {
                 pixel_fn(fbvar, pfbdata, offset, pixel, xy[index][0], xy[index][1], i - 1);
                 delay(500);
               }
               else {
                 pixel_fn_last(fbvar, pfbdata, offset, pixel, xy[index][0], xy[index][1], i - 1);
                 delay(500);
               }
               i = -1;
            }
            else if (i == temp) {
                for (j = 0; j < 24; j++) {
                    for (k = 0; k < 24; k++) {
                        offset = (xy[index][1] + k) * fbvar.xres + (xy[index][0] + j);
                        pixel = makepixel(0, 0, 0);
                        *(pfbdata + offset) = pixel;
                    }
                }
                temp = i; temp_i = i;

                but_count++;
                if (but_count == 3) but_count = 0;
                i += but_count;
                temp_i = i;
                if (index < 70) {
                    pixel_fn(fbvar, pfbdata, offset, pixel, xy[index][0], xy[index][1], i - 1);
                    delay(500);
                }
                else {
                    pixel_fn_last(fbvar, pfbdata, offset, pixel, xy[index][0], xy[index][1], i - 1);
                    delay(500);
                }
                i = -1;
            }
            else if (temp == -1) {
                if (index > 79) continue;
                temp = i; temp_i = i;
                p = search_list(head, index);
                if (p == NULL) {
                    if (index < 70) {
                        pixel_fn(fbvar, pfbdata, offset, pixel, xy[index][0], xy[index][1], i - 1);
                        delay(500);
                    }
                    else {
                        pixel_fn_last(fbvar, pfbdata, offset, pixel, xy[index][0], xy[index][1], i - 1);
                        delay(500);
                    }
                    i = -1;
                    but_count = 0;
                }
                else {
                    if (index > 78) continue;
                    int endi = index_search(head) + 1;
                    int deletei = index + 1;            
                    for (; deletei < endi; deletei++) {
                        for (j = 0; j < 24; j++) {
                            for (k = 0; k < 24; k++) {
                                offset = (xy[deletei][1] + k) * fbvar.xres + (xy[deletei][0] + j);
                                pixel = makepixel(0, 0, 0);
                                *(pfbdata + offset) = pixel;
                            }
                        }
                    }
                    int p_index;
                    head = insert(head, p, i, index);
                    p = p->link;
                    for (; p != NULL; p = p->link) {
                        p->index++;
                        p_index = p->index;
                        if (p_index < 70) {
                            pixel_fn(fbvar, pfbdata, offset, pixel, xy[p_index][0], xy[p_index][1], p->data - 1);
                        }
                        else {
                            pixel_fn_last(fbvar, pfbdata, offset, pixel, xy[p_index][0], xy[p_index][1], p->data - 1);
                        }
                    }
                    i = -1; but_count = 0;
                }
                k = list_len(p);
                for (j = 0; j < k; j++) {
                    p = delete_first(p);
                }
            }
            else {
                head = insert_last(head, temp_i, index);
                temp = i; temp_i = i; index++;
                if (index > 79) continue;
                p = search_list(head, index);
                if (p == NULL) {
                    if (index < 70) {
                        pixel_fn(fbvar, pfbdata, offset, pixel, xy[index][0], xy[index][1], i - 1);
                        delay(500);
                    }
                    else {
                        pixel_fn_last(fbvar, pfbdata, offset, pixel, xy[index][0], xy[index][1], i - 1);
                        delay(500);
                    }i = -1;but_count = 0;
                }
                else {
                    if (index > 78) continue;
                    int j = index_search(head) + 1;
                    int deletei = index + 1; int p_index;
                    head = insert(head, p, i, index);
                    for (p = p->link; p->index != j; p = p->link) {
                        p->index++;
                        p_index = p->index;
                        for (j = 0; j < 24; j++) {
                            for (k = 0; k < 24; k++) {
                                offset = (xy[deletei][1] + k) * fbvar.xres + (xy[deletei][0] + j);
                                pixel = makepixel(0, 0, 0);
                                *(pfbdata + offset) = pixel;
                            }
                        }deletei++;
                        if (deletei != p_index) continue;
                        if (index < 70) {
                            pixel_fn(fbvar, pfbdata, offset, pixel, xy[p_index][0], xy[p_index][1], p->data - 1);
                        }
                        else {
                            pixel_fn_last(fbvar, pfbdata, offset, pixel, xy[p_index][0], xy[p_index][1], p->data - 1);
                        }
                    }
                    i = -1; but_count = 0;
                }
            }
        }
        if (index < 70) { //blink
            blink_w(fbvar, pfbdata, offset, pixel, xy[index][0], xy[index][1]);
        }
    }
    munmap(pfbdata, fbvar.xres * fbvar.yres * 16 / 8);
    close(fbfd);
    return 0;
}

