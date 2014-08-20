class Equalizer
{
public:
  Equalizer(int pinOut, int pinStrobe, int pinReset) : pinOut(pinOut), pinStrobe(pinStrobe),
            pinReset(pinReset)
  {
  pinMode(pinReset, OUTPUT);
  pinMode(pinStrobe, OUTPUT);
  }
  
  void update() 
  {
    digitalWrite(pinReset, HIGH);          // reset the MSGEQ7's counter
    delay(5);
    digitalWrite(pinReset, LOW);
    char buffer [50];
    int d[7];
    for (int x = 0; x < 7; x++){
        digitalWrite(pinStrobe, LOW);      // output each DC value for each freq band
        delayMicroseconds(35); // to allow the output to settle
        data[x] = analogRead(pinOut);
        
        digitalWrite(pinStrobe, HIGH);
    }
  }
  
  short data[7];
  
private:
  
  const int pinOut;
  const int pinStrobe;
  const int pinReset;

};
