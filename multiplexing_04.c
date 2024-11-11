int sensorPin = A0; // TMP36 output pin
int sensorInput;

double tempFahr;
double tempKelvin;
const int buttonPin = A1;

int buttonState = 0;
int lastButtonState = 0;
int temperatureMode = 0; // 0 = Celsius, 1 = Fahrenheit, 2 = Kelvin

// Display kathode pinnen
const int displayPins[4] = {11, 10, 9, 8};

// Segment pinnnen (a-g + dot)
const int segmentPins[8] = {2, 3, 6, 5, 4, 1, 0, 7};

// Digitaal patroon voor nummers 0-9 op een 7-segment (a-g)
/*
const byte digits[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 0, 1, 1, 0, 0, 0}, // 1
  {1, 0, 1, 0, 1, 1, 1}, // 2
  {1, 0, 1, 1, 1, 0, 1}, // 3
  {0, 1, 1, 1, 0, 0, 1}, // 4
  {1, 1, 0, 1, 1, 0, 1}, // 5
  {1, 1, 0, 1, 1, 1, 1}, // 6
  {1, 0, 1, 1, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 1, 0, 1}  // 9
};
*/

const byte digits[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};



double temperatuurC() {
  double temp;
  sensorInput = analogRead(A0);
  temp = (double)sensorInput / 1024;
  temp = temp * 5;
  temp = temp - 0.5;
  temp = temp * 100;
  return temp;
}

double temperatuurF() {
  return (temperatuurC() * 9 / 5) + 32;
}

double temperatuurK() {
  return temperatuurC() + 273.15;
}

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);

  // Zet segment pins als output
  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }

  // Zet display mins als output
  for (int i = 8; i < 12; i++) {
    pinMode(displayPins[i], OUTPUT);
  }
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && lastButtonState == HIGH) {
    temperatureMode = (temperatureMode + 1) % 3;
  }
  lastButtonState = buttonState;

  double temperature = (temperatureMode == 0) ? temperatuurC() : (temperatureMode == 1) ? temperatuurF() : temperatuurK();
  displayTemperature(temperature);
  delay(100);

  if(temperatureMode == 0){
    Serial.print("Temperatuur in Celcius: ");
    Serial.println(temperatuurC());
  } else if(temperatureMode == 1){
    Serial.print("Temperatuur in Fahreneheit: ");
    Serial.println(temperatuurF());
  } else if(temperatureMode == 2){
    Serial.print("Temperatuur in Kelvin: ");
    Serial.println(temperatuurK());
  }

}

void displayTemperature(double temperature) {
  int integerPart = (int)temperature;
  int decimalPart = (int)((temperature - integerPart) * 10);

  int digitsToDisplay[4] = {0, 0, 0, 0};
  digitsToDisplay[0] = (integerPart / 100) % 10; // Hundreds Honderdtallen 100
  digitsToDisplay[1] = (integerPart / 10) % 10;  // Tientallen 10
  digitsToDisplay[2] = integerPart % 10;         // Eenheden 1
  digitsToDisplay[3] = decimalPart;              // Tienden (0.1)

  // Cycle door elke display
  for (int i = 0; i < 4; i++) {
    digitalWrite(displayPins[i], HIGH); // Zet huidige display aan
    
    // Altijd decimaal punt aan op het derde display (index 2)
    bool decimalPoint = (i == 2); 

    displayDigit(digitsToDisplay[i], decimalPoint);

    delay(5); // Delay voor multiplexing effect (mss niet nodig)
    digitalWrite(displayPins[i], LOW); // Zet display uit na tonen
  }
}




void displayDigit(int num, bool decimalPoint) {
  if (num >= 0 && num <= 9) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(segmentPins[i], digits[num][i]);
    }
  }

  // Optional: Geef decimaal punt weer als nodig (moet je nog zien, hoe je dit gaat doen)
  if (decimalPoint) {
    digitalWrite(segmentPins[7], HIGH);
  }
}

