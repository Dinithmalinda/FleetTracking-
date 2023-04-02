  bool webserverdebug=false;
#include <FS.h>
#include <ESP8266WiFi.h>//wifi library
#include <ESP8266WebServer.h>
ESP8266WebServer server(80); 


IPAddress local_IP(192, 168, 1, 1); // Set your server's fixed IP address here
IPAddress subnet(255, 255, 255, 0);   // Set your network sub-network mask here
 
bool wifilock=false;


bool handleFileRead(String path) { // send the right file to the client (if it exists)
          wifilock=true;
          if(webserverdebug)Serial.println("handleFileRead: " + path);
  
        if (path.endsWith("/")){ path += "index.html";         // If a folder is requested, send the index file
          if (SPIFFS.exists(path)) {                            // If the file exists
          File file = SPIFFS.open(path, "r");                 // Open it
          size_t sent = server.streamFile(file,"text/html"); // And send it to the client
          file.close();                                       // Then close the file again
          return true;
            }}            
        else if (path.endsWith("/savedata")) {
                  path = "/data.txt";          // If a folder is requested, send the index file 
                  if(webserverdebug)Serial.println(path);
                if (SPIFFS.exists(path)) {                            // If the file exists
                if(webserverdebug)Serial.println(server.arg("data"));
                 File file = SPIFFS.open(path, "w");                 // Open it
                int bytesWritten = file.print(server.arg("data"));   
                server.send(200,"text/plane","DATA WRITTEN"); 
                // server.arg("APchflag");
                delay(1000);
                file.close(); 
                }}
        else if (path.endsWith("/readdata")){ 
              path = "/data.txt"; // If a folder is requested, send the index file
              if (SPIFFS.exists(path)) {                            // If the file exists
              File file = SPIFFS.open(path, "r");                 // Open it
              size_t sent = server.streamFile(file, "text/plain"); // And send it to the client
              file.close();  
              return true;
                }} 
        else if (path.endsWith("/Lock")){              
              server.send(200,"text/plane","SYSTEM/LOCKED"); 
              wifilock=false; 
              WiFi.mode(WIFI_OFF); 
              return true;
                } 
        }
        
String readsavedata(){
            String DD="";
             const String  path = "/data.txt"; // If a folder is requested, send the index file
              if (SPIFFS.exists(path)) {                            // If the file exists
              File file = SPIFFS.open(path, "r");                 // Open it
              while (file.available()) {DD+=file.read(); }  
              file.close();}
              return DD;
}
 

void connectwifiserver(){WiFi.mode(WIFI_AP);//wifimode - WAPM
                         WiFi.softAPConfig(local_IP,local_IP,subnet);
                         delay(300);
                         WiFi.softAP("N-ableGPS","n-ablegps");
                         SPIFFS.begin();  
                         server.onNotFound([]() {                              // If the client requests any URI
                         if (!handleFileRead(server.uri()))                  // send it if it exists
                         server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
                        });
                        server.begin();  
                        }

void wifiwebserver(){server.handleClient();
                        if(!wifilock){
                        long now = millis();
                        if (millis() > 50000) {
                        wifilock=true;
                         WiFi.mode(WIFI_OFF);    //This also works
                        }}
}
  
  
String getSplitword(String str,String splitter,int count){
  int indexpoint=0;
  int splitpoints[count+1]={0};
  
   while (indexpoint <= count){
    splitpoints[indexpoint+1]= str.indexOf(",,,",splitpoints[indexpoint++]);}

    return str.substring(count,count+1);

  
  }  
  
  
  
  
