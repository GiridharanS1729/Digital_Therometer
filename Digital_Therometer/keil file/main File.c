#include <reg51.h>
#define input P1
double newtemp,pass1,pass2,T;//variables used
//LCD
sbit rs = P3^0; //register select pin
sbit rw = P3^1; //read write pin
sbit e = P3^2; //enable pin
//ADC
sbit rd=P3^7; //defines rd pin of ADC use for reading purposes
sbit wr=P3^6; // define wr pin of ADC use for writing purposes
sbit intr=P3^4; //defines intr pin use for sending interrupts from microcontroller
void delay(unsigned int time) //Function to provide time delay in msec.
{
int i,j ;
for(i=0;i<time;i++)
for(j=0;j<1275;j++);
}
double adc() // Function to read the values from ADC and send to controller.
{
double temp;
rd=1;
wr=0;
delay(1);
wr=1;
while(intr==1);
{rd=0;
temp=input;
delay(3);}
return temp;
}
void lcdcmd(unsigned char item) //Function to send commands to LCD see
//command tables in LCD Link
{
P2 = item;
rs= 0;
rw=0;
e=1; //send to high to low pulse while writing
delay(1);
e=0;
}
void lcddata(double item) //Function to send data to LCD
{
P2 = item;
rs= 1;
rw=0;
e=1; //send high to low pulse while writing
delay(1);
e=0;
}
void disp_temp(double num) //displays number on LCD
{
unsigned char UnitDigit = 0; //It will contain unit digit of number
unsigned char TenthDigit = 0; //It will contain 10th position digit of number
unsigned char HundDigit = 0; //It will contain 100th position digit of number
unsigned char decimal=0; //It will contain the decimal position of number
int point;
point=num*10;
HundDigit=(num/100);
if( HundDigit != 0) // If it is zero, then don't display
lcddata(HundDigit+0x30); // Make Character of HundDigit and then display it on
TenthDigit = num - HundDigit*100; // Findout Tenth Digit
TenthDigit = TenthDigit/10;
if (HundDigit==0 &&TenthDigit==0){} // If it is zero, then don't display
else
lcddata(TenthDigit+0x30); // Make Char of TenthDigit and then display it on LCD
UnitDigit = num - HundDigit*100;
UnitDigit = UnitDigit - TenthDigit*10;
lcddata(UnitDigit+0x30); // Make Char of UnitDigit and then display it on LCD
lcddata('.');
decimal=(point%10);
lcddata (decimal+0x30); // Make Char of Decimal Digit and then display it on LCD
lcddata(' '); lcddata('C');
}
void read(){ // Displays "READING" while controller reads from ADC
lcdcmd(0x0E); //turn display ON for cursor blinking
lcdcmd(0x01); //clear screen
lcdcmd(0x06); //increment cursor
lcddata('R');lcddata('E');lcddata('A');lcddata('D');lcddata('I');lcddata('N');lcddata('G');
lcddata(' ');
}
void main()
{
P0=0x00; //intialize port 0 to low use while controller reads the
//temperature from
//ADC
read(); // show reading on LCD while controller reads from ADC
while(1){ // use for checking errors while reading the value from ADC
newtemp=adc(); //reads first value from ADC
delay(60); //waits 60 msec
pass1=adc(); // reads the Second value from ADC
delay(60); // waits 60 msec
if (newtemp==pass1){ //compare first and second value
break; // if first and second value is same breaks the loop
}
}
while(1){ //enters in the permanent loop
T=160; //set reference voltage acting multiplier factor for temperature accuration
newtemp=(((newtemp*T)/255)); //converts the temperature value according to
//reference adjusted in
//decimal
lcdcmd(0x0E); //turn display ON for cursor blinking
lcdcmd(0x01); //clear screen
lcdcmd(0x06); //increment
disp_temp(newtemp); //show temperature
delay(300); //waits 3sec before re-measure the value of temperature
while(1){ // re-measure the value from ADC but this time double check
newtemp=adc();
delay(60);
pass1=adc();
delay(60);
pass2=adc();
if (newtemp==pass1)
{
if(pass1==pass2)
{
break;
}
}
}
// end ADC while loop
} // end while permanent loop
} // end main loop