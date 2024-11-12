// Pin definitie
const int sensorPin = A0;          // Pin voor TMP36 temperatuur sensor
const int buttonPin = A1;		  //Knop om van temperatuurmodus te veranderen
const int alarmTogglePin = A2;    // Pin voor de knop om alarm in/uit te schakelen
const int increaseTempPin = A3;   // Pin voor de knop om de drempel te verhogen
const int decreaseTempPin = A4;   // Pin voor de knop om de drempel te verlagen
const int alarmPin = 12;         // Pin voor het alarm
int buttonState = 0;
int lastButtonState = 0;
int temperatureMode = 0; // 0 = Celsius, 1 = Fahrenheit, 2 = Kelvin


// Variabelen
float tempThreshold = 30.0;      // Standaard drempel in graden Celsius
bool alarmEnabled = false;       // Status van het alarm
double tempFahr;
double tempKelvin;
int sensorInput;
double temperatuur;

// Voor debounce
bool alarmTogglePressed = false;
bool increaseTempPressed = false;
bool decreaseTempPressed = false;



double temperatuurC(){
	double temp;
	temp = (analogRead(A0) /  1024.0 * 5.0 - 0.5) * 100.0;
	return temp;
}

double temperatuurF(){
  tempFahr = (temperatuurC() * 9/5) + 32;
  return tempFahr;
}

double temperatuurK(){
  tempKelvin = temperatuurC() + 273.15;
  return tempKelvin;
}

double temperatuur(){
	if(temperatureMode == 0){
		temperatuur = temperatuurC();
  } else if (temperatureMode == 1){
	  temperatuur = temperatuurF();
  } else if (temperatureMode == 2){
	  temperatuur = temperatuurK();
  }
  return temperatuur;
}



void setup() {
  Serial.begin(9600);             // Start seriële communicatie
  pinMode(alarmTogglePin, INPUT);
  pinMode(increaseTempPin, INPUT);
  pinMode(decreaseTempPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(alarmPin, OUTPUT);
  digitalWrite(alarmPin, LOW);
}

void loop() {
	
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == LOW && lastButtonState == HIGH){
  	//isCelsius = !isCelsius;
    temperatureMode = (temperatureMode + 1) % 3; // switch tussen 0 (Celsius), 1 (Fahreneheeit), 2 (Kelvin)
  }
  lastButtonState = buttonState;
/*  
  if(temperatureMode == 0){
  	Serial.print("Temperatuur in Celsius: ");
    Serial.println(temperatuurC());
    delay(1000);
  } else if (temperatureMode == 1){
  	Serial.print("Temperatuur in Fahrenheit: ");
    Serial.println(temperatuurF());
    delay(1000);
  } else if (temperatureMode == 2){
  	Serial.print("Tempratuur in Kelvin: ");
    Serial.println(temperatuurK());
    delay(1000);
  }
*/
  // Lees de knoppen
  bool toggleState = digitalRead(alarmTogglePin);
  bool increaseState = digitalRead(increaseTempPin);
  bool decreaseState = digitalRead(decreaseTempPin);

  // Knop om het alarm in/uit te schakelen
  if (toggleState == LOW && !alarmTogglePressed) {
    alarmTogglePressed = true;

    alarmEnabled = !alarmEnabled; // Toggle alarmstatus
    Serial.println(alarmEnabled ? "Alarm ingeschakeld" : "Alarm uitgeschakeld");
  }
  
  if (toggleState == HIGH) {
    alarmTogglePressed = false;
  }

  // Knop om de drempel te verhogen
  if (increaseState == LOW && !increaseTempPressed) {
    increaseTempPressed = true;

    tempThreshold += 0.5;
    Serial.print("Nieuwe drempel: ");
    Serial.print(tempThreshold);
    Serial.println(" °C");
  }
  if (increaseState == HIGH) {
    increaseTempPressed = false;
  }

  // Knop om de drempel te verlagen
  if (decreaseState == LOW && !decreaseTempPressed) {
    decreaseTempPressed = true;

    tempThreshold -= 0.5;
    Serial.print("Nieuwe drempel: ");
    Serial.print(tempThreshold);
    Serial.println(" °C");
  }
  if (decreaseState == HIGH) {
    decreaseTempPressed = false;
  }

  // Controleer temperatuur en activeer alarm indien nodig
  if (alarmEnabled && temperatuurC() >= tempThreshold) {
    digitalWrite(alarmPin, HIGH); // Alarm aan
  } else {
    digitalWrite(alarmPin, LOW); // Alarm uit
  }

  // Print de huidige temperatuur en drempel
  Serial.print("Temperatuur: ");
  Serial.print(temperatuur());
  Serial.print(" °C, Drempel: ");
  Serial.print(tempThreshold);
  Serial.print(" °C, Alarm: ");
  Serial.println(alarmEnabled ? "Aan" : "Uit");
  
  delay(500); // Interval tussen metingen
}
