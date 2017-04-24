#include <VirtualWire.h> //Includes library VirtualWire.h
char temp[]="1010";
void setup()
{ 
  Serial.begin(9600);
//++++++++++++++Initializes the receiver module+++++++++++++++++++  
//vw_set_ptt_inverted(true);  
vw_setup(2000);  
vw_set_rx_pin(2); //Configure the pin D2 to read the data  
vw_rx_start(); //Starts reading data from the receiver module  
//==============================================================  
//Configure the pins 6 and 9 as output  
for(int i=6;i<=9;i++)  
{    
pinMode(i, OUTPUT);  
}  
//======================================

}
void loop()
{  
uint8_t buf[VW_MAX_MESSAGE_LEN]; //Variable for storing the data buffer  
uint8_t buflen = VW_MAX_MESSAGE_LEN; //Variable for storing the buffer size
//Serial.println("transfer");
if(vw_get_message(buf, &buflen)) //If the buffer have any data (0 or 1)  
{    
  //Serial.println("received");
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    

//Increments the buffer for reading by i (0 or 4)    
//Increments the positions of digital pins by j (6 to 9)        
for (int i = 0; i < buflen; i++)    
{      
buf[i] = buf[i] - 48;
//Gets the data buffer that is received in hex and subtract 48      
if(buf[i] == 1) //If the data in the specified buffer position is equal to 1      
{        
Serial.print("1"); //Reverses the state of the respective digital pin
temp[i]='1';      
}
if(buf[i] == 0) //If the data in the specified buffer position is equal to 1      
{        
Serial.print("0"); //Reverses the state of the respective digital pin 
temp[i]='0';    
}
}
Serial.println("---");
Serial.println(temp);

//checking for lane
if(strcmp(temp ,"1111" ))
{
  Serial.println("Lane 1"); 
}
else if(strcmp(temp ,"1001" ))
{
  Serial.println("Lane 2");
}
else if(strcmp(temp ,"0000" ))
{
  Serial.println("Unknown Lane");
}
else
{
  Serial.println("Skipped the whole stuff ");
}
//========================================================================================  

}
}
