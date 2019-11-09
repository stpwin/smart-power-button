bool wait_turnoff;
volatile bool computer_power_button_trigger;

//POWER_PIN 2
#define POWER_STATUS_PIN 4
#define RELAY_PIN 5
#define COMPUTER_POWER_PIN 6

void setup(){
  EICRA = 0b10;
  EIMSK |= bit(INT0);

  pinMode(2, INPUT_PULLUP); //INTERUPT WHEN LOW
  pinMode(POWER_STATUS_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(COMPUTER_POWER_PIN, OUTPUT);

  Serial.begin(115200);
}

void powerOn(){
  digitalWrite(RELAY_PIN, HIGH);
  delay(300);
  digitalWrite(COMPUTER_POWER_PIN, HIGH);
  delay(100);
  digitalWrite(COMPUTER_POWER_PIN, LOW);
  delay(300);
}

void powerOff(){
  delay(1000);
  digitalWrite(RELAY_PIN, LOW);
}

unsigned long power_button_millis;
unsigned long power_status_millis;
void buttonCheckLoop(){
  if (millis() - power_button_millis > 50){
    power_button_millis = millis();
    if (computer_power_button_trigger){
      computer_power_button_trigger = false;

      if (digitalRead(POWER_STATUS_PIN)){
        Serial.println("Already on state.");
      } else {
        Serial.println("Turning on...");
        powerOn();
        Serial.println("Turned on.");
        wait_turnoff = true;
      }
    }
  }
}


void powerCheckLoop(){
  if (millis() - power_status_millis > 1000){
    power_status_millis = millis();
    if (wait_turnoff){
      if (!digitalRead(POWER_STATUS_PIN)){
        wait_turnoff = false;
        Serial.println("Turning off...");
        powerOff();
        Serial.println("Turned off.");
      }
      
    }
  }
}

void loop(){
  powerCheckLoop();
  buttonCheckLoop();
}

ISR(INT0_vect){
  if (PIND & bit(2) == bit(2)){
    computer_power_button_trigger = true;
  }
}

