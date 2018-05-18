int sensorPin = A0;      // select the input pin for the potentiometer
int ledPin = 4;          // select the pin for the LED
int sensorValue = 0;     // variable to store the value coming from the sensor
int waitfor = 100;       // wait in loop
const int nsamples = 25; // number of samples
int isample = 0;         // sample counter
float vsample[nsamples];
float alert =0.2;        // alert level for standard deviation

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  isample++;
  if (isample==nsamples) {isample=0;}
  sensorValue = analogRead(sensorPin);
  
  vsample[isample] = sensorValue * (5.0 / 1023.0);

  float mn = average(vsample, nsamples);
  float st = stdev(vsample, nsamples);
  
  if (st > alert) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  
  Serial.print(isample);
  Serial.print(" ");
  Serial.print(vsample[isample]);
  Serial.print(" ");
  Serial.print(mn);
  Serial.print(" ");
  Serial.print(st);
  Serial.println();
  
  delay(waitfor);
}

/*********************************
 * Gemiddelde van een array floats
 */
float average(float * v, int n) {
  float sum = 0.0;
  for (int is=0;is<n;is++) {sum+=v[is];}
  sum = sum / n;
  return sum;
}

/******************************************
 * Standaard afwijking van een array floats
 */
float stdev(float * v, int n) {
  float mean  = average(v, n);
  float stdev = 0.0;
  for (int is=0;is<n;is++) {stdev+=(v[is]-mean)*(v[is]-mean);}
  stdev = stdev / n;
  stdev = sqrt(stdev);
  return stdev;
}

