// Define the digital pin connected to your relay's IN pin
const int RELAY_PIN = 7;

void controlRelay(bool turnOn) {
  if (turnOn) {
    digitalWrite(RELAY_PIN, HIGH);  // HIGH turns your relay ON (Green LED lights up)
  } else {
    digitalWrite(RELAY_PIN, LOW);  // LOW turns your relay OFF (Green LED turns off)
  }
}

bool state = true;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);

  // Safe startup: keeps the relay and green LED OFF when the board boots up
  digitalWrite(RELAY_PIN, LOW);
}


void loop() {

  controlRelay(state);
  delay(2000);
  state = false;
  controlRelay(state);
  delay(2000);
  state = true;
}
