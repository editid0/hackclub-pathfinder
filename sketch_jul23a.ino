int LED1 = D0;
int LED2 = D1;
int LED3 = D2;
int BUTTON = D6;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  int button_state = digitalRead(BUTTON);
  if (button_state == LOW) {
    // the button is pressed, so turn on the LEDs
    // from what i can tell low means on because low resistance allows power flow, so high resistance would disable leds
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH); // i set this to add a bit of variance, so while others turn on, this turns off
    digitalWrite(LED3, LOW);
  } else {
    // button isn't pressed so disable leds
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW); // opposite to above, so when others turn off, this turns on
    digitalWrite(LED3, HIGH);
  };
}
