#define DEBUGmode true //enable sirialprint
#define DefaultServerUpdateT 300000  //send data to server  in miliseconds



#define findstartpoint 0
#define collectword 1
#define timeOffset 19800 //+5.30 GTM time offset
#define defaultunix 1662367047


#include "HTTPconfiguration.h"
#include "A9Glib.h"



void setup() {
  delay(1000);
  Serial.begin(115200);
 // connectwifiserver();
  String param=readsavedata();
  Serial.println(param);
//  MQTT_init(getSplitword(param,",,,",0),getSplitword(param,",,,",1),
//            getSplitword(param,",,,",2),getSplitword(param,",,,",3),
//            getSplitword(param,",,,",4));



  
 delay(10000);
 MQTT_init("122.255.9.5","1883","A9G","A9G","A9G");//a9g.GPS_init();
//  byte whilecounter=0;
//  String Rdata="";
//while(1){whilecounter++;
//        if(whilecounter>5)break;
//        delay(5000);
//        //Rdata=a9g.reciveATwithfilter("AT","$GNGGA,","$GPGSA");
//      //if(Rdata.length()>25){a9g.GPStoLatLang(Rdata);break;}
//      lastMillis=millis();
// }
//
//      if(LNG==500&&LAT==500) ESP.reset();
}


void loop() {  
  delay(5000);


  
  sendMQTT("A9G","HIGHT");
  
//wifiwebserver();
if (millis() - lastMillis > DefaultServerUpdateT) {  
        lastMillis += DefaultServerUpdateT;
        
        String Rdata=reciveATwithfilter("AT","$GNGGA,","$GPGSA");


        
//        if(Rdata.length()>25){//GPStoLatLang(Rdata);
//        unixGPRS=0,lastunix=0,unixGPS=0;
//                     if(unixGPRS<lastunix||unixGPRS<defaultunix){
//                      
//                        Rdata=reciveATwithfilter("AT+CCLK?","CCLK: \"","\"");
//                        //if (DEBUGmode)Serial.println(Rdata);
//                        if(Rdata.length()>8){
//                                StringTodateStamp(Rdata);
//                                lastunix=unixGPRS-300;}
//                           }
//                      else {          
//                            Rdata=url+"?VID=" + clientId + "&tistm="+String(unixGPRS)+LastDigits+"&GPS=" + "['"+String(LNG,8)+"','"+String(LAT,8)+"','0','0']";
//                            //if (DEBUGmode)Serial.println(Rdata);
//                           postData("AT+HTTPGET=\""+Rdata+"\"");
//                            }
//        }
//        else gpsnotconnectflag++;
//        if(gpsnotconnectflag>5)ESP.reset();
//      }
}
