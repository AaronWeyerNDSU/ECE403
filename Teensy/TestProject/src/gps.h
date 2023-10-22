#ifndef Arduino.h
#define Arduino.h


struct GPSmessage
{
  bool valid;
  double latitude;
  double longitude;
};

GPSmessage parseGPSmessage(String msgString){
  // Convert input string to character array.
  int strLength = msgString.length() + 1;
  char msgChar[strLength];
  msgString.toCharArray(msgChar, strLength);

  // Define delimeter and parsing token.
  const char delim[2] = ",";
  char *token;

  // Define return structure.
  struct GPSmessage msg;

  token = strtok(msgChar, delim); // index location of message format.
  token = strtok(NULL, delim); // index location of UTC time.
  token = strtok(NULL, delim); // index location of valid data marker.
  if (token[0] == 'A') {
    msg.valid = true;
  } else {
    msg.valid = false;
    msg.latitude = 0.0;
    msg.longitude = 0.0;
    return msg;
  }
  token = strtok(NULL, delim); // index location of latitude.
  msg.latitude = atof(token);
  token = strtok(NULL, delim); // index location of North/South marker.
  if (token[0] == 'S') {
    msg.latitude = -1.0*msg.latitude;
  }
  token = strtok(NULL, delim); // index location of longitude.
  msg.longitude = atof(token);
  token = strtok(NULL, delim); // index location of East/West marker.
  if (token[0] == 'W') {
    msg.longitude = -1.0*msg.longitude;
  }

  return msg;
}
#endif