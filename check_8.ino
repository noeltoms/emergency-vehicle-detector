#include <TinyGPS.h>
#include <VirtualWire.h> //Includes the Library VirtualWire.h   
#include <SoftwareSerial.h>

char *nibble = "1010"; //Creates the variable nibble with the value of 0000  
char *lan1 = "1111";
char *lan2 = "1001";
char *nolan = "0000";
int le_pino; //Creates the variable to read the digital pins  
TinyGPS gps;

static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);

void setup()
{
  Serial.begin(9600);
  //++++++++++++++Initializes the transmitter module+++++++++++++++++++   
vw_set_ptt_inverted(true);   
vw_setup(2000);   
vw_set_tx_pin(3); 
//Configure the D3 pin to read the data   
//================================================================   
//Configure the pins 6 to 9 as input   
for(int i=6; i<=9; i++)   
{     
pinMode(i, INPUT_PULLUP);   
}
pinMode(13,OUTPUT);
//======================================
  
  Serial.print("Testing TinyGPS library v. "); Serial.println(TinyGPS::library_version());
  Serial.println("by Mikal Hart");
  Serial.println();
  Serial.println("Sats HDOP Latitude  Longitude  Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum");
  Serial.println("          (deg)     (deg)      Age                      Age  (m)    --- from GPS ----  ---- to London  ----  RX    RX        Fail");
  Serial.println("-------------------------------------------------------------------------------------------------------------------------------------");
}

void loop()
{
  float flat, flon;
  unsigned long age, date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
  
  print_int(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES, 5);
  print_int(gps.hdop(), TinyGPS::GPS_INVALID_HDOP, 5);
  gps.f_get_position(&flat, &flon, &age);
  print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
  print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
  Serial.print(flat,6);
  Serial.print("  ");
  Serial.print(flon,6);
  if((flat == 9.579572) && (flon == 76.624099))
  //if((flat == 30.24) && (flon == -97.81))
  {
      Serial.print("Lane 1");
      *nibble = *lan1;
  }
  else if((flat == 9.579672) && (flon == 76.625099))
  {
    Serial.print("LAne 2");
    *nibble = *lan2;
  }
  else
  {
    Serial.print("Lane 3");
    *nibble = *nolan;  
  }
  Serial.println();

  //TRANSMITTER
  digitalWrite(13,HIGH);
 //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   

//Increments the position for storage in nibble by i (0 to 4)   
//Increments the position of the digital pins by j (6 to 9)   
int j=0;   
vw_send((uint8_t *)nibble, strlen(nibble)); //Sends the variable nibble
//checking transmission
if(vw_tx_active()==HIGH)
{
 Serial.println("The emergency vehicle is ahead.pls make way "); 
}
else
{
  Serial.println("Reciever");
}
vw_wait_tx(); //Waits the end of transmission   
//=======================================================================================

  //TRANSMITTER END
  smartdelay(1000);
}

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (Serial.available())
      gps.encode(Serial.read());
  } while (millis() - start < ms);
}

static void print_float(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartdelay(0);
}

static void print_int(unsigned long val, unsigned long invalid, int len)
{
  char sz[32];
  if (val == invalid)
    strcpy(sz, "*******");
  else
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartdelay(0);
}

static void print_date(TinyGPS &gps)
{
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE)
    Serial.print("********** ******** ");
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d ",
        month, day, year, hour, minute, second);
    Serial.print(sz);
  }
  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
  smartdelay(0);
}

static void print_str(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartdelay(0);
}
