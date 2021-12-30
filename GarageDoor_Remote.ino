//https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/

#include "WiFi.h"
#include "EEPROM.h"
#include <Preferences.h>

Preferences preferences;

String password = "";
String ssid = "";

void displayNetworks(){
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            delay(10);
        }
    }
    Serial.println("");
}

void setup() {
  Serial.begin(115200);
  preferences.begin("credentials", false);

// this variable is for when you're developing on the setup section. It will cleat out the memory each time. If you 
// Don't want to clear the memory, or pushing to board to test if it's working, then set to false so the memory doesn;t 
// get cleared on startup
  bool dev = false;
  
  if(dev==true){
    preferences.clear();
  }

//  get the variables if they exist or have been set. Default to "not set"
  ssid = preferences.getString("ssid", "not set");
  password = preferences.getString("password", "not set");

//if ssid is not set, get it from the serial monitor input
  while (ssid == "not set"){
   Serial.println("The ssid has not been set. Type the name of the network you want to connect to from this list");
   displayNetworks();
   if (Serial.available() > 0){
     ssid = Serial.readString();
     preferences.putString("ssid", ssid);
   }
    delay(5000);
  }

//  if the password does not exist, then get it from the serial monitor input
  while (password == "not set"){
   Serial.println("type the password for the network");
   if (Serial.available() > 0){
     password = Serial.readString();
     preferences.putString("password", password);
   }
    delay(5000);
  }

//  get the ssid and password now that they should be set
  password = preferences.getString("password", "not set");
  ssid = preferences.getString("ssid", "not_set");

//create a new ssid and password variable which removes the last character because it came in from the serial.readString()
 String ssid_ = ssid.substring(0, ssid.length()-1);
 String password_ = password.substring(0, password.length()-1);

// connect to the network with the creds
  WiFi.begin(ssid_.c_str(), password_.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network"); 
}
void loop() {
  Serial.println("The ssid is");
  Serial.println(ssid);
  Serial.println("The Password is");
  Serial.println(password);
  delay(5000);
}
