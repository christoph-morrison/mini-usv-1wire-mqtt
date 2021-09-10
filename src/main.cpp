/*
 * Rui Santos
 * Complete Project Details https://randomnerdtutorials.com
*/

// 26 B0 C0 C3 00 00 00 F5
// 12 BA 96 A6 00 00 00 C2


#include <OneWire.h>
#include <DallasTemperature.h>
#include <DS2438.h>

// Data wire is plugged into port 4 on the Arduino
#define ONE_WIRE_BUS D4

float tempOffset = 60.0;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

uint8_t DS2438_address[] = { 0x26, 0xb0, 0xc0, 0xc3, 0x00, 0x00, 0x00, 0xf5 };
DS2438 ds2438(&oneWire, DS2438_address);

int numberOfDevices; // Number of temperature devices found

DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address

// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
}

void setup(void) {
  // start serial port
  Serial.begin(115200);
  
  // Start up the library
  sensors.begin();
  ds2438.begin();
  
  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();
  
  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  // Loop through each device, print out address
  for(int i=0;i<numberOfDevices; i++) {

  }
}

void loop(void) { 
  sensors.requestTemperatures(); // Send the command to get temperatures
  
  // Loop through each device, print out temperature data
  for(int i=0;i<numberOfDevices; i++) {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i)) {
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(tempDeviceAddress);
      Serial.println();
		} else {
		  Serial.print("Found ghost device at ");
		  Serial.print(i, DEC);
		  Serial.print(" but could not detect address. Check power and cabling");
		}

    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i)){
		
		// Output the device ID
		Serial.print("Temperature for device: ");
		Serial.println(i,DEC);

    // Print the data
    float tempC = sensors.getTempC(tempDeviceAddress) + tempOffset;
    Serial.print("Temp C: ");
    Serial.println(tempC);
    } 	
  }

  ds2438.update();
  if (ds2438.isError()) {
    Serial.println("Error reading from DS2438 device");
  } else {
    Serial.print("Temperature = ");
    Serial.print(ds2438.getTemperature(), 1);
    Serial.print("C, Channel A = ");
    Serial.print(ds2438.getVoltage(DS2438_CHA), 1);
    Serial.print("v, Channel B = ");
    Serial.print(ds2438.getVoltage(DS2438_CHB), 1);
    Serial.println("v.");
  }

  Serial.println(" ---------- ");
  delay(5000);
}

