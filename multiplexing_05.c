int sensorPin = A0; // TMP36 output pin
int sensorInput;

double tempFahr;
double tempKelvin;
const int buttonPin = A1;

int buttonState = 0;
int lastButtonState = 0;
int temperatureMode = 0; // 0 = Celsius, 1 = Fahrenheit, 2 = Kelvin


int A = 2;
int B = 3;
int C = 6;
int D = 5;
int E = 4;
int F = 13;
int G = 12;
int DP = 7;

int GND_CC1 = 11;
int GND_CC2 = 10;
int GND_CC3 = 9;
int GND_CC4 = 8;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Temperatuur

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
/*
double temperatuurK() {
  return temperatuurC() + 273.15;
}
*/

double temperatuur(){
	double temperatuur;
	
	if(temperatureMode == 0){
		temperatuur = temperatuurC();
	} else if (temperatureMode == 1){
		temperatuur = temperatuurF();
	} /*else if (temperatureMode == 2){
		temperatuur = temperatuurK();
	}*/
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Setup 	Loop
void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(DP, OUTPUT);
  
  pinMode(GND_CC1, OUTPUT);
  pinMode(GND_CC2, OUTPUT);
  pinMode(GND_CC3, OUTPUT);
  pinMode(GND_CC4, OUTPUT);
  

}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && lastButtonState == HIGH) {
    temperatureMode = (temperatureMode + 1) % 3;
  }
  lastButtonState = buttonState;
  
  showSegments(temperatuur());
  Serial.println(temperatuur());
  
/*
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
*/


}

////////////////////////////////////////////////////////////////////////////////
//	Multiplexing

void digit_0(){
	digitalWrite(A, HIGH);
	digitalWrite(B, HIGH);
	digitalWrite(C, HIGH);
	digitalWrite(D, HIGH);
	digitalWrite(E, HIGH);
	digitalWrite(F, HIGH);
	digitalWrite(G, LOW);
}

void digit_1(){
	digitalWrite(A, LOW);
	digitalWrite(B, HIGH);
	digitalWrite(C, HIGH);
	digitalWrite(D, LOW);
	digitalWrite(E, LOW);
	digitalWrite(F, LOW);
	digitalWrite(G, LOW);
}

void digit_2(){
	digitalWrite(A, HIGH);
	digitalWrite(B, HIGH);
	digitalWrite(C, LOW);
	digitalWrite(D, HIGH);
	digitalWrite(E, HIGH);
	digitalWrite(F, LOW);
	digitalWrite(G, HIGH);
}

void digit_3(){
	digitalWrite(A, HIGH);
	digitalWrite(B, HIGH);
	digitalWrite(C, LOW);
	digitalWrite(D, HIGH);
	digitalWrite(E, HIGH);
	digitalWrite(F, LOW);
	digitalWrite(G, HIGH);
}

void digit_4(){
	digitalWrite(A, LOW);
	digitalWrite(B, HIGH);
	digitalWrite(C, HIGH);
	digitalWrite(D, LOW);
	digitalWrite(E, LOW);
	digitalWrite(F, HIGH);
	digitalWrite(G, HIGH);
}

void digit_5(){
	digitalWrite(A, HIGH);
	digitalWrite(B, LOW);
	digitalWrite(C, HIGH);
	digitalWrite(D, HIGH);
	digitalWrite(E, LOW);
	digitalWrite(F, HIGH);
	digitalWrite(G, HIGH);
}

void digit_6(){
	digitalWrite(A, HIGH);
	digitalWrite(B, HIGH);
	digitalWrite(C, HIGH);
	digitalWrite(D, HIGH);
	digitalWrite(E, HIGH);
	digitalWrite(F, HIGH);
	digitalWrite(G, HIGH);
}

void digit_7(){
	digitalWrite(A, HIGH);
	digitalWrite(B, HIGH);
	digitalWrite(C, HIGH);
	digitalWrite(D, LOW);
	digitalWrite(E, LOW);
	digitalWrite(F, LOW);
	digitalWrite(G, LOW);
}

void digit_8(){
	digitalWrite(A, HIGH);
	digitalWrite(B, HIGH);
	digitalWrite(C, HIGH);
	digitalWrite(D, HIGH);
	digitalWrite(E, HIGH);
	digitalWrite(F, HIGH);
	digitalWrite(G, HIGH);
}

void digit_9(){
	digitalWrite(A, HIGH);
	digitalWrite(B, HIGH);
	digitalWrite(C, HIGH);
	digitalWrite(D, HIGH);
	digitalWrite(E, LOW);
	digitalWrite(F, HIGH);
	digitalWrite(G, HIGH);
}

void showDigit(int digit){
	
	switch (digit){
		case 0:
		digit_0();
		break;
		
		case 1:
		digit_1();
		break;

		case 2:
		digit_2();
		break;
		
		case 3:
		digit_3();
		break;

		case 4:
		digit_4();
		break;
		
		case 5:
		digit_5();
		break;
		
		case 6:
		digit_6();
		break;
		
		case 7:
		digit_7();
		break;
		
		case 8:
		digit_8();
		break;
		
		case 9:
		digit_9();
		break;
		
		default:
		break;
	}
}

void showSegments (double number){
	
	int geheel_deel = (int) number;
	int deci_deel = (int)((number - geheel_deel) * 10);
	
	int honderdtallen = geheel_deel / 100;
	int tientallen = (geheel_deel / 10) % 10;
	int eenheden = geheel_deel % 10;
	
	
		
	showDigit(tientallen);
	digitalWrite(DP, LOW);
	digitalWrite(GND_CC1, HIGH);
	digitalWrite(GND_CC2, LOW);
	digitalWrite(GND_CC3, LOW);
	//digitalWrite(GND_CC4, LOW);
	

	digitalWrite(GND_CC1, LOW);
	showDigit(eenheden);
	digitalWrite(DP, HIGH);
	digitalWrite(GND_CC2, HIGH);
	

	digitalWrite(GND_CC2, LOW);
	showDigit(deci_deel);
	digitalWrite(DP, LOW);
	digitalWrite(GND_CC3, HIGH);
	
	/*
	digitalWrite(GND_CC3, LOW);
	showDigit(deci_deel);
	digitalWrite(DP, LOW);
	digitalWrite(GND_CC4, HIGH);
	*/
}