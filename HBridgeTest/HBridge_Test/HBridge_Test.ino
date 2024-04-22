int encA = 15;
int in1 = 11;
int in2 = 10;
int input = 14;
int val;

void setup()
{               
  Serial.begin(38400);
  pinMode(15, OUTPUT);
  pinMode(14, INPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
}

void loop()                     
{
  val = analogRead(A0);
  Serial.println(val);
  delay(100);
  if(val > 512) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(encA, (val-512)/2);
    Serial.println((val-512)/2);
    Serial.println("Backward");
  }
  if(val <= 512) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(encA, 256-(val/2));
    Serial.println(256-(val/2));
    Serial.println("Forward");
  }
  
}
