#include <UnixTime.h>
UnixTime stamp(3);
#define ATrecivedelay 1000
#define RefreshGPS 5000


int timearray[6];
String serverURL="",serverPORT="",clientID="",PUBtopic,SUBtopic;
uint32_t unixGPRS=0,lastunix=0,unixGPS=0;

String sendData(String command){ 
        String response = "";
        char lastchar1=' ',lastchar2=' ';
        Serial.println(command);
        long int time = millis();
        while ((time + ATrecivedelay) > millis())
          { while (Serial.available()) {
            char c = Serial. read();
            response += c;
            if(response.endsWith("n\OK"))return response;
                }
            }
        if (DEBUGmode)Serial.println(response);
         return response;
        }

        void postData(String command){  Serial.println(command);delay(100);}

        String reciveATwithfilter(String command,String Startpoint,String endpoint) {
              // if (DEBUGmode)Serial.println(command+","+Startpoint+","+endpoint);
                  String response = "";
                  byte startpointlength=Startpoint.length();
                  byte endpointlength=endpoint.length();
                  byte wordstatus=findstartpoint;
                  int responselength=0;
                  String newword = "";
                  Serial.println(command);//send ATrequest  
                  long int time = millis();
                    while ((time + 1000) > millis())
                            {while (Serial.available()) { 
                              response +=char(Serial.read());   
                              responselength=response.length();      
                    if(wordstatus==findstartpoint&&(responselength>=startpointlength)){       
                        newword = response.substring(responselength-startpointlength);
                        if(newword.equals(Startpoint)){wordstatus=collectword;response = "";
                        // if (DEBUGmode)Serial.println("startfound"+newword);
                              }
                              }                    
                  if(wordstatus==collectword&&(responselength>=endpointlength)){                                 
                        newword = response.substring(responselength-endpointlength);
                        if(newword.equals(endpoint)){
                        // if (DEBUGmode)Serial.println("endfound"+newword);                  
                            newword = response.substring(0,responselength-endpointlength);
                            if (DEBUGmode)Serial.println("data="+newword);
                  
                          return newword;}
                        }  
                      }
    
                  }
              Serial.flush(); 
              return "";
              }


  void GPStoLatLang(String input){
  int inputlength=input.length();
  byte wordcount=0;
  String timestamp="",longitude="",Latitude="";
  float LAT=1,LNG=1;
  for(int i=0;i<inputlength;i++){
    if(input.charAt(i)==',')wordcount++;
    else{
      if(wordcount==0){timestamp+=input.charAt(i);}      
      if(wordcount==1)Latitude+=input.charAt(i);
      if((wordcount==2)&&(input.charAt(i)=='S'))LAT=-1;
      if(wordcount==3)longitude+=input.charAt(i);
      if((wordcount==4)&&(input.charAt(i)=='W'))LNG=-1;
      if(wordcount>4)break;  
      }
      }
      String lockword=timestamp.substring(0,2);
        timearray[3]=lockword.toInt();
        lockword=timestamp.substring(2,4);
        timearray[4]=lockword.toInt();
        lockword=timestamp.substring(4,6);
        timearray[5]=lockword.toInt();
      stamp.setDateTime(timearray[0],timearray[1],timearray[2],timearray[3],
                    timearray[4],timearray[5]); 
  unixGPS = stamp.getUnix()+timeOffset-9000;
   //if (DEBUGmode)Serial.println(unix);
      lockword=Latitude.substring(0,2);
  float def=lockword.toFloat();
      lockword=Latitude.substring(2,10);
      def+=lockword.toFloat()/60;
      LAT*=def;
      lockword=longitude.substring(0,3);
      def=lockword.toFloat();
      lockword=longitude.substring(3,10);
      def+=lockword.toFloat()/60; 
      LNG*=def;
         if (DEBUGmode)Serial.println(LAT, 8); 
         if (DEBUGmode)Serial.println(LNG, 8);  
}

void StringTodateStamp(String input){
  String lockword=input.substring(0,2);
  timearray[0]=lockword.toInt()+2000;
  lockword=input.substring(3,5);
  timearray[1]=lockword.toInt();
  lockword=input.substring(6,8);
  timearray[2]=lockword.toInt();
  lockword=input.substring(9,11);  
  timearray[3]=lockword.toInt();
  lockword=input.substring(12,14);
  timearray[4]=lockword.toInt();
  lockword=input.substring(15,17);
  timearray[5]=lockword.toInt();
  stamp.setDateTime(timearray[0],timearray[1],timearray[2],timearray[3],
                    timearray[4],timearray[5]); 
  unixGPRS = stamp.getUnix()+timeOffset-9000;
  //if (DEBUGmode)Serial.println(unix);
}

String getlastdigit(String clientId){
  String LastDigits;
  byte wordlength=clientId.length();
  if(wordlength==0)LastDigits="000";
  else if(wordlength==1)LastDigits="00"+clientId;
  else if(wordlength==2)LastDigits="0"+clientId;
  else LastDigits=clientId.substring(wordlength-3);
  return LastDigits;
  }
  

void MQTT_init(String url,String port,String clientID,String pubtopic,String Subtopic){
        delay(1000);  
//        sendData("AT" );sendData("AT+CCID");
//        sendData("AT+CREG=1");sendData("AT+CLTS=1");
//        sendData("AT+CGATT=1");sendData("AT+CGDCONT=1,\"IP\",\"CMNET\"");
//        sendData("AT+CGACT=1,1");sendData("AT+QGSMLOC=2");
        
        sendData("AT" );
        sendData("AT+CGATT=1" );       // Connect to the network
        sendData("AT+CGDCONT=1,”IP”,”CMNET”" );   // Set PDP Parameters
        sendData("AT+CGACT=1,1");      // Active the PDP
        sendData("AT+MQTTCONN=\"122.255.9.5\",1883,\"12345\",120,0,\"Ai-thinker\",\"123456\"" );  // Send MQTT connection packet
        sendData("AT+MQTTSUB=\""+Subtopic+"\",1,0" );      // Send MQTT subscribe packet
        sendData("AT+MQTTPUB=\""+pubtopic+"\",\"PAYLOAD\",0,0,0");   // Send MQTT publish packet   
        }
        
void GPS_init(){
        sendData("AT+LOCATION=1");
        String RGPS=String(RefreshGPS/500);
        sendData("AT+GPSRD="+RGPS);
        sendData("AT+GPS=1");
        }

         
void disconnectMQTT(){sendData("AT+MQTTDISCONN");} 
void connectMQTT(){sendData("AT+MQTTCONN=\"122.255.9.5\",1883,\"12345\",120,0,\"Ai-thinker\",\"123456\"");}

bool GPSupdates(){
return true; 
return false;}

String getGPScoordinates(){
  
  
  
  }
 
void sendMQTT(String topic,String data){
 sendData("AT+MQTTPUB=\""+topic+"\",\""+data+"\",0,0,0" );   // Send MQTT publish packet 
}



 
