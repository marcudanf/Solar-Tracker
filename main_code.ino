
#include <Servo.h>
#include <Time.h>

#include <math.h>
#include <LiquidCrystal.h>
byte pb_reset = 8;
byte pb_set = 6;
byte pb_counter = 7;
byte LDR_S_pin = 0;
byte LDR_W_pin = 1;
byte LDR_N_pin = 2;
byte LDR_E_pin = 3;
unsigned int LDR_S;
unsigned int LDR_E;
unsigned int LDR_N;
unsigned int LDR_W;
byte Tolerance=100;
int Dif_S_N;
int Dif_E_W;
byte altitude_LDR;
byte azimuth_LDR;
unsigned long RefTime = 0;
unsigned long LoopTime = 0;
byte altitude_ang;
byte azimuth_ang;
unsigned int n = 0;
byte M;
String x;
String hr_STR;
String min_STR;
String day_STR;
String mon_STR;
String yr_STR;
String Date_STR;
String Time_STR;
String altitude_STR;
String azimuth_STR;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#define Pi 3.14
#define SL 30.0
#define LL 35.88 // longitude in degrees for faculity of engineering/University of jordan
#define DS 60.0 // Day light saving hour (60 min)
#define L 0.559 // latitude in Rad for faculity of engineering/University of jordan
Servo Altitude;
Servo Azimuth;
double altitude;
double altitude_rad;
double azimuth;
double B;
double ET;
double AST;
double Delta;
double h;
double TE;
double TW;
unsigned int DN; //Returns the number of day in the year
void setup() 
{
pinMode(pb_set, INPUT);
pinMode(pb_counter, INPUT);
pinMode(pb_reset, INPUT);
Altitude.attach(9);
Azimuth.attach(10);
setTime(9,0,0,1,1,2013);
lcd.begin(16, 2); // set up the LCD's number of columns and rows:
LCDPrint(1,0,"Solar Tracking");
LCDPrint(5,1,"System");
delay (5000);
lcd.clear();
LCDPrint(0,0,"By: Ayat Dawood");
LCDPrint(0,1,"Wala'a alaraideh");
delay (5000);
lcd.clear();
LCDPrint(0,0,"Supervised by: ");
LCDPrint(0,1,"Dr. Osama Ayyadi");
delay (5000);
altitude_ang=90;
azimuth_ang=87;
Altitude.write(altitude_ang*0.9+6);
Azimuth.write(azimuth_ang*0.9+6);
lcd.clear();
LCDPrint(0,0,"Modes: Time(set)");
LCDPrint(0,1," LDR(counter)");
while (true){
if (digitalRead(pb_set) == LOW) {
while (digitalRead(pb_set) == LOW) {}
PrintTime(hour(),minute(),day(),month(),year());
loop();
}
if (digitalRead(pb_counter) == LOW) {
while (digitalRead(pb_counter) == LOW) {}
LDRLoop();
}
}
}
void loop() {
LoopTime = millis();
lcd.clear();
LCDPrint(0,0," Time Mode");
delay(2000);
PrintTime(hour(),minute(),day(),month(),year());
M = minute();
while (millis() - LoopTime <= 600000) {
if (minute() > M) {
PrintTime(hour(),minute(),day(),month(),year());
M = minute();
}
if (digitalRead(pb_set) == LOW) {
while (digitalRead(pb_set) == LOW) {}
YearFun();
}
if (digitalRead(pb_reset) == LOW) {
while (digitalRead(pb_reset) == LOW) {}
setTime(0,0,0,1,1,2013);
PrintTime(hour(),minute(),day(),month(),year());
M = minute();

loop();
}
}
setMotors();
}
void LDRLoop(){
lcd.clear();
LCDPrint(0,0," LDR Mode");
while (true){
LDR_S=analogRead(LDR_S_pin);
LDR_E=analogRead(LDR_E_pin);
LDR_N=analogRead(LDR_N_pin);
LDR_W=analogRead(LDR_W_pin);
Dif_E_W=LDR_E-LDR_W;
Dif_S_N=LDR_S-LDR_N;
if (abs(Dif_E_W)>Tolerance){
if (Dif_E_W>0){
azimuth_LDR++;
Azimuth.write(azimuth_LDR);
}
else{
azimuth_LDR--;
Azimuth.write(azimuth_LDR);
}
}
if (abs(Dif_S_N)>Tolerance){
if (Dif_S_N >0){
altitude_LDR++;
Altitude.write(altitude_LDR);
}
else{
altitude_LDR--;
Altitude.write(altitude_LDR);
}
}
}
}

void YearFun() {
RefTime = millis();
n=500;
while (millis() - RefTime <= 60000) {
if (n%1000!=0){
PrintTime(hour(),minute(),day(),month(),200);
while (millis() - RefTime <= n) {
if (digitalRead(pb_set) == LOW) {
while (digitalRead(pb_set) == LOW) {}
PrintTime(hour(),minute(),day(),month(),year());
M = minute();
MonthFun();
}
else if (digitalRead(pb_counter) == LOW) {
while (digitalRead(pb_counter) == LOW) {}
setTime(hour(),minute(),second(),day(),month(),year()+1);
PrintTime(hour(),minute(),day(),month(),year());
M = minute();
}
}
}
else {
PrintTime(hour(),minute(),day(),month(),year());
while (millis() - RefTime <= n) {
if (digitalRead(pb_set) == LOW) {
while (digitalRead(pb_set) == LOW) {}
MonthFun();
}
else if (digitalRead(pb_counter) == LOW) {
while (digitalRead(pb_counter) == LOW) {} // To endsure that the user unpress the set
pushbotton before go to Min function
setTime(hour(),minute(),second(),day(),month(),year()+1);
PrintTime(hour(),minute(),day(),month(),year());
M = minute();
}
}
}
n=n+500;
}
setMotors();
}

void MonthFun() {
RefTime = millis();
n=500;
while (millis() - RefTime <= 60000) {
if (n%1000!=0){
PrintTime(hour(),minute(),day(),200,year());
while (millis() - RefTime <= n) {
if (digitalRead(pb_set) == LOW) {
while (digitalRead(pb_set) == LOW) {}
PrintTime(hour(),minute(),day(),month(),year());
M = minute();
DayFun();
}
else if (digitalRead(pb_counter) == LOW) {
while (digitalRead(pb_counter) == LOW) {}
setTime(hour(),minute(),second(),day(),month()+1,year());
PrintTime(hour(),minute(),day(),month(),year());
M = minute();
}
}
}
else {
PrintTime(hour(),minute(),day(),month(),year());
while (millis() - RefTime <= n) {
if (digitalRead(pb_set) == LOW) {
while (digitalRead(pb_set) == LOW) {}
DayFun();
}
else if (digitalRead(pb_counter) == LOW) {
while (digitalRead(pb_counter) == LOW) {}
setTime(hour(),minute(),second(),day(),month()+1,year());
PrintTime(hour(),minute(),day(),month(),year());
M = minute();
}
}
}
n=n+500;
}
setMotors();
}

void DayFun() {
RefTime = millis();
n=500;
while (millis() - RefTime <= 60000) {
if (n%1000!=0){
PrintTime(hour(),minute(),200,month(),year());
while (millis() - RefTime <= n) {
if (digitalRead(pb_set) == LOW) {
while (digitalRead(pb_set) == LOW) {}
PrintTime(hour(),minute(),day(),month(),year());
M = minute();
HourFun();
}
else if (digitalRead(pb_counter) == LOW) {
while (digitalRead(pb_counter) == LOW) {}
setTime(hour(),minute(),second(),day()+1,month(),year());
PrintTime(hour(),minute(),day(),month(),year());
M = minute();
}
}
}
else {
PrintTime(hour(),minute(),day(),month(),year());
while (millis() - RefTime <= n) {
if (digitalRead(pb_set) == LOW) {
while (digitalRead(pb_set) == LOW) {}
HourFun();
}
else if (digitalRead(pb_counter) == LOW) {
while (digitalRead(pb_counter) == LOW) {}
setTime(hour(),minute(),second(),day()+1,month(),year());
PrintTime(hour(),minute(),day(),month(),year());
M = minute();
}
}
}
n=n+500;
}
setMotors();
}

void HourFun() {
RefTime = millis();
n=500;
while (millis() - RefTime <= 60000) {
if (n%1000!=0){
PrintTime(200,minute(),day(),month(),year());
while (millis() - RefTime <= n) {
if (digitalRead(pb_set) == LOW) {
while (digitalRead(pb_set) == LOW) {}
PrintTime(hour(),minute(),day(),month(),year());
M = minute();
MinuteFun();
}
else if (digitalRead(pb_counter) == LOW) {
while (digitalRead(pb_counter) == LOW) {}
setTime(hour()+1,minute(),second(),day(),month(),year());
PrintTime(hour(),minute(),day(),month(),year());
M = minute();
}
}
}
else {
PrintTime(hour(),minute(),day(),month(),year());
while (millis() - RefTime <= n) {
if (digitalRead(pb_set) == LOW) {
while (digitalRead(pb_set) == LOW) {}
MinuteFun();
}
else if (digitalRead(pb_counter) == LOW) {
while (digitalRead(pb_counter) == LOW) {}
setTime(hour()+1,minute(),second(),day(),month(),year());
PrintTime(hour(),minute(),day(),month(),year());
M = minute();
}
}
}
n=n+500;
}
setMotors();
}

void MinuteFun() {
RefTime = millis();
n=500;
while (millis() - RefTime <= 60000) {
if (n%1000!=0){
PrintTime(hour(),200,day(),month(),year());
while (millis() - RefTime <= n) {
if (digitalRead(pb_set) == LOW) {
while (digitalRead(pb_set) == LOW) {}
PrintTime(hour(),minute(),day(),month(),year());
M = minute();
setMotors();
}
else if (digitalRead(pb_counter) == LOW) {
while (digitalRead(pb_counter) == LOW) {}
setTime(hour(),minute()+1,second(),day(),month(),year());
PrintTime(hour(),minute(),day(),month(),year());
M = minute();
}
}
}
else {
PrintTime(hour(),minute(),day(),month(),year());
while (millis() - RefTime <= n) {
if (digitalRead(pb_set) == LOW) {
while (digitalRead(pb_set) == LOW) {}
setMotors();
}
else if (digitalRead(pb_counter) == LOW) {
while (digitalRead(pb_counter) == LOW) {}
setTime(hour(),minute()+1,second(),day(),month(),year());
PrintTime(hour(),minute(),day(),month(),year());
M = minute();
}
}
}
n=n+500;
}
setMotors();
}

void LCDPrint(byte col, byte row, String sms) {
lcd.setCursor(col,row);
lcd.print(sms); // Print a message to the LCD.
}
String Nom2Str(int dig2str) {
if (dig2str < 10 && dig2str >= 0) {
x = String(dig2str, DEC);
x = String('0'+x);
}
else {
x = String(dig2str, DEC);
}
return x;
}
void PrintTime(byte Hr,byte Min,byte Dy,byte Month,int Yr){
if (Hr==200){
hr_STR=" ";
}
else{
hr_STR=Nom2Str(Hr);
}
if (Min==200){
min_STR=" ";
}
else{
min_STR=Nom2Str(Min);
}
if (Dy==200){
day_STR=" ";
}
else{
day_STR=Nom2Str(Dy);
}
if (Month==200){
mon_STR=" ";
}
else{
mon_STR=Nom2Str(Month);

}
if (Yr==200){
yr_STR=" ";
}
else{
yr_STR=Nom2Str(Yr);
}
Date_STR=String(day_STR + " - " + mon_STR + " - " + yr_STR);
lcd.clear();
LCDPrint(1,0,Date_STR);
Time_STR=String(hr_STR + " : " + min_STR);
LCDPrint(4,1,Time_STR);
}
void setMotors() {
B = (yday(year(),month(),day())-1)*360.0/365.0; // in Rad
ET =229.2*(0.000075+0.001868*cos(B)-0.032077*sin(B)-0.014615*cos(2*B)-
0.04089*sin(2*B)); // in min
AST = 60.0*hour()+minute()+ET+4.0*(SL-LL)-DS; // in min
Delta = 23.45*Pi/180.0*sin((360.0/365.0)*(284.0+yday(year(),month(),day()))); // in Rad
h = Pi/180.0*(AST-720)/4; // in Rad
altitude_rad = asin((sin(Delta)*sin(L))+cos(Delta)*cos(h)*cos(L)); // in Rad
altitude =altitude_rad*180.0/Pi; // in Degrees
azimuth = asin(cos(Delta)*sin(h)/cos(altitude_rad))*180.0/Pi; // in Degrees
if (Delta > 0){
TE=((180.0-180.0/Pi*acos(tan(Delta)/tan(L)))/15.0)*60.0;
TW=((180.0/Pi*acos(tan(Delta)/tan(L)))/15.0+12.0)*60;
if (AST < TE || AST > TW){
azimuth=azimuth/abs(azimuth)*(180.0-abs(azimuth));
}
}
if (azimuth > 90.0){
azimuth=90.0;
}
else if (azimuth < -90.0){
azimuth = -90.0;
}
if (altitude < 0){
lcd.clear();
LCDPrint(3,0,"It's night..");

LCDPrint(0,1,"So, no rotation");
delay(5000);
PrintTime(hour(),minute(),day(),month(),year());
M = minute();
loop();
}
else if (altitude > 90){
altitude = 90;
}
altitude_STR=String("Altitude: " + Nom2Str(round(altitude)) + " deg");
azimuth_STR=String("Azimuth: " + Nom2Str(round(azimuth)) + " deg");
lcd.clear();
LCDPrint(0,0,altitude_STR);
LCDPrint(0,1,azimuth_STR);
if (altitude_ang < round(altitude)){
for (byte i=altitude_ang*0.9+6; i<round(altitude)*0.9+6;i+=1 ){
Altitude.write(i);
delay(15);}
}else{
for (byte i=altitude_ang*0.9+6; i<round(altitude)*0.9+6;i-=1 ){
Altitude.write(i);}
delay(15);}
altitude_ang=round(altitude);
if (azimuth_ang < round(altitude)){
for (byte i=azimuth_ang*0.9+6; i<round(90.0+azimuth)*0.9+6;i+=1 ){
Azimuth.write(i);
delay(15);}
}else{
for (byte i=azimuth_ang*0.9+6; i<round(90.0+azimuth)*0.9+6;i-=1 ){
Azimuth.write(i);}
delay(15);}
azimuth_ang=round(90.0+azimuth);
delay(5000);
PrintTime(hour(),minute(),day(),month(),year());
M = minute();
loop();
}

double yday (unsigned int y, unsigned int m, unsigned int d){
unsigned int days[]={0,31,59,90,120,151,181,212,243,273,304,334}; // Number of days at
the beginning of the month in a not leap year.
//Start to calculate the number of day
if (m==1 || m==2){
DN = days[(m-1)]+d; //for any type of year, it calculate the number of days for
January or february
} // Now, try to calculate for the other months
else if (y % 4 == 0){ //those are the conditions to have a leap year
DN = days[(m-1)]+d+1; // if leap year, calculate in the same way but increasing one day
}
else { //if not a leap year, calculate in the normal way, such as January or
February
DN = days[(m-1)]+d;
}
return DN;
}
