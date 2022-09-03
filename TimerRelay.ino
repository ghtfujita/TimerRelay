//RTC
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

//7seg
const int p1	= 9;
const int p2	= 8;
const int p3	= 7;
const int p4	= 6;
const int p5	= 5;
const int p7	= 16;
const int p10	= A0;
const int p11	= A1;
const int v[]	= {p1, p2, p3, p4, p5, p7, p10, p11};

const int p6	= 10;
const int p8	= 14;
const int p9	= 15;
const int p12	= A2;
const int p13	= A3;
const int g[]	= {p6, p8, p9, p12, p13};

const int n[10][7] = {
	/*0*/{p1, p2, p4, p7, p10, p11},
	/*1*/{p4, p7},
	/*2*/{p1, p2, p5, p7, p11},
	/*3*/{p2, p4, p5, p7, p11},
	/*4*/{p4, p5, p7, p10},
	/*5*/{p2, p4, p5, p10, p11},
	/*6*/{p1, p2, p4, p5, p10, p11},
	/*7*/{p4, p7, p11},
	/*8*/{p1, p2, p4, p5, p7, p10, p11},
	/*9*/{p2, p4, p5, p7, p10, p11}
};

const int d[] = {p12, p9, p8, p6};

//relay
const int pinRelay = 1;
const int onTime = 5;
const int offTime = 22;


void setup() {
	//7seg
	for (int i = 0; i <= 10; i++) pinMode(v[i], OUTPUT);
	for (int j = 0; j <=5; j++) pinMode(g[j], OUTPUT);
	dispclear();

	//RTC
	Wire.begin();
	RTC.begin();
	if (! RTC.isrunning()) {
		dispCircle();
		RTC.adjust(DateTime(__DATE__, __TIME__)); //sets the RTC to the date & time this sketch was compiled 
	}

	//relay
	pinMode(pinRelay, OUTPUT);
}

void loop() {
	DateTime now = RTC.now();
	//display time
	disp(now.hour(), now.minute());

	//relay ctrl
	switch(now.hour()){
		case onTime ... offTime:
			digitalWrite(pinRelay, HIGH);
			break;
		default:
			digitalWrite(pinRelay, LOW);
			break;
	}
}

void disp(int hh, int mm) {
	int num[3];
	if (hh < 10){
		num[1] = hh;
		dispPerDigit(1, num[1]);
	}else{
		num[0] = hh / 10;
		num[1] = hh - (num[0] * 10);
		dispPerDigit(1, num[0]);
		dispPerDigit(2, num[1]);
	}
	num[2] = mm / 10;
	num[3] = mm - (num[2] *10);
	dispPerDigit(3, num[2]);
	dispPerDigit(4, num[3]);
	dispT();
}

//display numbers and decimal points
void dispPerDigit(int i, int j) {
	if(j == d) digitalWrite(p3, LOW);
	digitalWrite(d[i-1], HIGH);
	for (int k = 0; k <= 7; k++) {
	digitalWrite(n[j][k], LOW);
	};
	if(n[j][0] != p1) digitalWrite(p1, HIGH); //display error control
	if (j == 4) digitalWrite(p2, HIGH); //display error control
	delay(5);
	dispclear();
}

//display time divider
void dispT() {
	digitalWrite(p3, LOW);
	digitalWrite(p13, HIGH);
}

void dispclear() {
	for (int i = 0; i <= 10; i++) digitalWrite(v[i], HIGH);
	for (int j = 0; j <=5; j++) digitalWrite(g[j], LOW);
}

void dispCircle() {
  for (int i = 0; i <=5; i++) digitalWrite(g[i], HIGH);
  int order[] = {p10, p11, p7, p4, p2, p1};
  for (int i = 0; i < 6; i++) {
	digitalWrite(order[i], LOW);
	delay(100);
  }
  for (int i = 0; i < 6; i++) {
	digitalWrite(order[i], HIGH);
	delay(100);
  }
  dispclear();
}
