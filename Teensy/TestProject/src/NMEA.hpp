#ifndef Arduino.h
#define Arduino.h

/// @brief Class used to read and parse NMEA messages collected from a GPS connected to a serial port.
class NMEA {
  public:
    bool valid;
    float latitude;
    float longitude;
    float UTCtime;

    NMEA(HardwareSerial *serial);
    boolean available();
    boolean read();
    void parseNMEA(String msg);

  private:
    HardwareSerial* uart;

};

/// @brief Initialize communications with GPS module.
/// @param serial Serial port that GPS is connected too.
NMEA::NMEA(HardwareSerial *serial) {
  uart = serial;

  valid = false;
  latitude = 0.0;
  longitude = 0.0;
  UTCtime = 0.0;
};

/// @brief Check if UART message buffer has incoming data.
/// @return True if new data is available, False otherwise.
boolean NMEA::available() {
  return (uart->available() > 0);
};

/// @brief Check to see if NMEA RMC message is available. 
/// If a new message is available, update valid data marker, coordinates, and UTC time.
/// @return True if message was read and values updated, False otherwise.
boolean NMEA::read(){
  // Check if a new message is available. If not return.
  if (!(available())) { return false; }

  // Read new message.
  String msg = uart->readStringUntil(13);
  msg.trim(); // Trim leading blankspaces.

  // Check if message is "Recomended Minimum Specific" (RMC). If not return.
  if (!msg.startsWith("$GPRMC")) { return false; }

  // Parse NMEA message and update values
  parseNMEA(msg);

  // Message received, values updated. Return true.
  return true;
}

/// @brief Take input NMEA RMC message and parse it into useable parts.
/// Updates "valid", "latitude", "longitude", and "UTCtime" internal variables.
/// @param msgString NMEA RMC message in plain text.
void NMEA::parseNMEA(String msgString){
  // Convert input string to character array.
  int strLength = msgString.length() + 1;
  char msgChar[strLength];
  msgString.toCharArray(msgChar, strLength);

  // Define delimeter and parsing token.
  const char delim[2] = ",";
  char *token;

  // index location of message format.
  token = strtok(msgChar, delim);

  // index location of UTC time.
  token = strtok(NULL, delim);
  UTCtime = atof(token);

  // index location of valid data marker.
  token = strtok(NULL, delim); 
  valid = (token[0] == 'A');

  // index location of latitude.
  token = strtok(NULL, delim); 
  latitude = atof(token);

  // index location of North/South marker.
  token = strtok(NULL, delim);
  if (token[0] == 'S') {
    latitude = -1.0*latitude;
  }

  // index location of longitude.
  token = strtok(NULL, delim); 
  longitude = atof(token);

  // index location of East/West marker.
  token = strtok(NULL, delim); 
  if (token[0] == 'W') {
    longitude = -1.0*longitude;
  }
}

#endif