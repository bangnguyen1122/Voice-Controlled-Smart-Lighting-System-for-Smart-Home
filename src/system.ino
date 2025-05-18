#include <Wire.h>

// DS3231 address
const byte DS3231 = 0x68;
// Number of data bytes read from DS3231
const byte NumberOfFields = 3;

// Declare time variable
int second=10, minute=0, hour=0;                      // Variable to store the time set for the RTC
int second_tmp, minute_tmp, hour_tmp;                 // Temporary variable to store the time value read from the RTC
int led = 10, led_mode_0 = 12, led_mode_1 = 11;       // LED displays Auto mode = 11, Timer mode = 12
unsigned char led_str[30];                            // Temporary variable to store the string
unsigned char ch;
int i =0;                                     
int mode=0;
int sensor=0;

// --------------------------------------------------------------------------------------------------------------------
void readDS3231()
{ Wire.beginTransmission(DS3231);       // Pass the address of the RTC
  Wire.write((byte)0x00);               // Define the register address to start reading time as 0x00
  Wire.endTransmission();
  Wire.requestFrom(DS3231,NumberOfFields);

  second_tmp = bcd2dec(Wire.read() & 0x7f);     // Read seconds
  minute_tmp = bcd2dec(Wire.read() );           // Read minutes
  hour_tmp = bcd2dec(Wire.read() & 0x3f);       // Read hour in 24-hour format
}

// --------------------------------------------------------------------------------------------------------------------
// Convert from BCD to DEC
int bcd2dec(byte num)
{ 
  return  ((num/16*10) + (num%16));
}
// Convert from DEC to BCD
int dec2bcd(byte num)
{
  return ((num/10*16)+(num%10));  
}

// --------------------------------------------------------------------------------------------------------------------
// Set time for DS3231
void setTime(byte second, byte minute, byte hour)
{
  Wire.beginTransmission(DS3231);
  Wire.write(byte(0x00));                       // Reset Pointer
  Wire.write(dec2bcd(second));
  Wire.write(dec2bcd(minute));
  Wire.write(dec2bcd(hour));
  Wire.endTransmission();
}

// --------------------------------------------------------------------------------------------------------------------
// Read time value from string and convert from string to int
int get_time(char mystring[40])
{  char time_str[6];
  int i, n = 0, time_int ;
  for (i=0;i<strlen(mystring);i++)
  { if ((mystring[i]>='0') && (mystring[i]<='9'))
    { time_str[n] = mystring[i];
      n++;
    }
  }
  if (n == 0) return 0;
  else
  { time_int = atoi(time_str);
    return time_int;
  }
}

// --------------------------------------------------------------------------------------------------------------------
// Read the mode of the light
void get_mode(char mystring[30])
{  char mode_str[7];
  strncpy(mode_str,mystring+7,7);                 // Extract the first 3 characters from the string
  mode_str[7] = '\0';
  if ((strcmp(mode_str,"tu dong")) ==0){
    mode = 0;
   //Auto mode = 0, Hen gio = 1
  }
  else if ((strcmp(mode_str,"hen gio")) == 0){
    mode = 1;
  }
  else{
    mode = mode;  
  }
}

// --------------------------------------------------------------------------------------------------------------------
// Read the status of the light
int get_state(char mystring[30])
{   
  String state =  "123";

  for(int i=0; i<3;i++)
  {
    state[i] = char(mystring[i]);
   }
   Serial.print("state: ");
   if(state == "mo "){
    Serial.println(1);
    return 1;
   }else if(state == "tat"){
    Serial.println(0);
      return 0;
   }
}

// --------------------------------------------------------------------------------------------------------------------
// Clear data in the array
void clr_str (char string[30])
{
  for (int i = 0; i<30;i++)
  {
    string[i] = 0x00;
  }
}

// --------------------------------------------------------------------------------------------------------------------
// Calculate the set time
void time_cal(int time_tmp)
{    int time_div=0;
     second = time_tmp%60;      // Convert to seconds
     time_div = time_tmp/60;
     hour = time_div/60;        // Convert to hours
     minute = time_div%60;      // Convert to minutes
}
void setup() {
    // Put your setup code here, to run once:
    Wire.begin();               // Initialize I2C
    // Set time for RTC
    Serial.begin(9600);

    pinMode(led, OUTPUT);       // Set pin 13 as OUTPUT
    digitalWrite(led, HIGH);    // Turn off the light
    pinMode(2,INPUT_PULLUP);   
    pinMode(11, OUTPUT); 
    pinMode(12, OUTPUT);    
}

// --------------------------------------------------------------------------------------------------------------------
// Reset time
void reset_Time()
{
  second=0;
  minute=0;
  hour=0;
}
void loop() {
  // Put your main code here, to run repeatedly:
  if (Serial.available())
  {
    ch = Serial.read();               // Read data from Buffer
    if (ch!='\n')
    {  led_str[i]=ch;
       i++;
    }
    
    else 
    {
      
      i = 0;
      Serial.print("get mode: ");
      get_mode(led_str);
      Serial.println(mode);
      
      int state_ = get_state(led_str); 
      if ( (state_ == 1) && (mode==1) )
      {
        Serial.print("doc trang thai: ");
        
        int time_tmp=0;
        time_tmp = get_time(led_str);       // Read time from string
        Serial.println(time_tmp);
        time_cal(time_tmp);                 // Calculate the set time
        setTime(0,0,0);                     // Set time for RTC
        digitalWrite(led,HIGH);             // Turn on the light
        delay(1);                           // Delay 1ms
        readDS3231();                       // Read time from RTC for the first time
        clr_str(led_str);
      }
     else if (state_ == 0) 
     {
      clr_str(led_str);
      Serial.println("tat den");
      digitalWrite(led,LOW);                // Turn off the light
      // Clear the string
     }
  
   }
 }
  readDS3231(); // Read time from RTC for the first time
//   Serial.print("Mode:");
//   Serial.println(mode);
  if ( (second_tmp==second) && (minute_tmp==minute) && (hour_tmp==hour) && ((second!=0)|(minute!=0)|(hour!=0)) ) // Time's up, turn off the light
     {  digitalWrite(led, LOW);           // Turn off the light
        setTime(0,0,0);                   // Reset time for RTC
        reset_Time();                     // Reset the set time
     }
  if (mode==0)                            // Auto mode
  { 
    digitalWrite(led_mode_1, HIGH);
    digitalWrite(led_mode_0, LOW);
    sensor = digitalRead(2);
//    Serial.println("che do auto");
    if (sensor==1) digitalWrite(led,HIGH);
    else digitalWrite(led,LOW); 
//    Serial.println(sensor);
  }
  else if (mode==1)
  {
    digitalWrite(led_mode_0, HIGH);
    digitalWrite(led_mode_1, LOW);
  }
}
