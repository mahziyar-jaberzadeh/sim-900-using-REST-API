#include "SIM900.h"  //external bayad librarisho berizid
#include <SoftwareSerial.h>
#include "inetGSM.h" //external 
//#include "sms.h" //age ghasde sms darid faal konid 
//#include "call.h" // age ghasde call darid faal konid

//baraye avaz kardane pin haye software serial do khat tuye  GSM.cpp. ro bayad avaz koni //kheili kheili moheme


SoftwareSerial s(5,6);//pin haye morede estefade 5 , 6 

InetGSM inet;//faal kardane call va sms sim 900
//CallGSM call; 
//SMSGSM sms;

char msg[50];
int numdata;
char inSerial[50];
int i=0;
boolean started=false;



void setup() 
{
  //Serial connection.
  Serial.begin(38400);/*بادریت مورد استفاده سیم 900*/
  Serial.println("GSM Shield testing.");
  s.begin(9600);/* ارتباط اردوینو و وای فای */
  //Start configuration of shield with baudrate.
  //baraye estefadeye http khate paeen gsm baud rate nabayad balaye 4800 bashe
  if (gsm.begin(2400)){
    Serial.println("\nstatus=READY");
    started=true;  
  }
  else Serial.println("\nstatus=IDLE");
  
  if(started){
    //gprs . apn .username. password ro bana be simcard khodetun tanzim konin
    //age az sim cardaye kharej az iran estefade kardid niaz be taghire chizi nist
    if (inet.attachGPRS("mtnirancell", "", ""))
      Serial.println("status=ATTACHED");
    else Serial.println("status=ERROR");
    delay(1000);
    
    //khoondane Ip address
    gsm.SimpleWriteln("AT+CIFSR");
    delay(5000);
    //ta moghei ke buffere serial khali beshe yani dige tu serial chizi nayad shoru kone be khoondan bekhune.
    gsm.WhileSimpleRead();
  
    //REST api begir ke umad
    //bad az ferestadane data chappesh mikone.
    //numdata=inet.httpPOST("46.4.195.114",3060, "/", "50",msg, 50); // dasture asliye 
    numdata=inet.httpGET("46.4.195.114",3060, "/", msg, 50);
    Serial.println("\nNumber of data received:");
    Serial.println(numdata);  
    Serial.println("\nData received:"); 
    Serial.println(msg); 
int data=s.read();
    Serial.println(data);
  }
};

void loop() 
{
  //harchi tu serial bud bekhoon ,
  //chap kon tu NewSoftSerial.
  serialhwread();
  //montazere vorudi jadid bemoon.
  serialswread();
};

void serialhwread(){   //tarife tabeee bala
  i=0;
  if (Serial.available() > 0){            
    while (Serial.available() > 0) {
      inSerial[i]=(Serial.read());
      delay(10);
      i++;      
    }
    
    inSerial[i]='\0';
    if(!strcmp(inSerial,"/END")){
      Serial.println("_");
      inSerial[0]=0x1a;
      inSerial[1]='\0';
      gsm.SimpleWriteln(inSerial);
    }
  
    if(!strcmp(inSerial,"TEST")){
      Serial.println("SIGNAL QUALITY");
      gsm.SimpleWriteln("AT+CSQ");
    }
    
    if(!strcmp(inSerial,"MSG")){
      Serial.println(msg);
    }
    else{
      Serial.println(inSerial);
      gsm.SimpleWriteln(inSerial);
    }    
    inSerial[0]='\0';
  }
}

void serialswread(){ //tarife tabeee bala
  gsm.SimpleRead();
}
