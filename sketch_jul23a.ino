int LED1 = D3;
int LED2 = D4;
int LED3 = D5;
int LED4 = D6;

int BUTTON1 = D10;
int BUTTON2 = D9;
int BUTTON3 = D7;
int BUTTON4 = D8;

int leds[] = { D3, D4, D5, D6 };
int buttons[] = { D10, D9, D7, D8 };
const int pairCount = 4;
int flasherIndex = 0;

enum GameMode { HOME,
                REACTIONTIME,
                SIMONSAYS };
GameMode currentGameMode = HOME;

#define MAX_SEQUENCE 32
int simonSequence[MAX_SEQUENCE];
int simonLength = 0;

void setup() {
  // put your setup code here, to run once:
  // set the button gpio pins
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);

  // set the led gpio pins
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  // turn off the leds
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);

  // start serial, as thats where im gonna output the data
  Serial.begin(9600);

  // set a random seed using analogue noise
  randomSeed(analogRead(A0));
}

void reactionTimeGame() {
  // random delay, to prevent cheating
  delay(random(1500, 8500));

  // pick a random pin
  int index = random(0, pairCount);
  digitalWrite(leds[index], LOW);

  unsigned long startTime = millis();

  while (true) {
	if (digitalRead(buttons[index]) == LOW) {
	  unsigned long reaction = millis() - startTime;

	  digitalWrite(leds[index], HIGH);

	  delay(10);

	  // Turn on all LEDs
	  digitalWrite(LED1, LOW);
	  digitalWrite(LED2, LOW);
	  digitalWrite(LED3, LOW);
	  digitalWrite(LED4, LOW);

	  // output the data to the user over serial
	  Serial.print("Reaction time for LED ");
	  Serial.print(index + 1);
	  Serial.print(" was ");
	  Serial.print(reaction);
	  Serial.print("ms");

	  // wait half a second before changing leds.
	  delay(500);

	  // turn off all leds
	  digitalWrite(LED1, HIGH);
	  digitalWrite(LED2, HIGH);
	  digitalWrite(LED3, HIGH);
	  digitalWrite(LED4, HIGH);
	}
  }
}

void simonSaysGame() {
  simonLength = 1;
  while (true) {
	simonSequence[simonLength - 1] = random(0, pairCount);

	// iterate over pattern
	for (int i = 0; i < simonLength; i++) {
	  int chosen = leds[simonSequence[i]];
	  digitalWrite(chosen, LOW);
	  delay(500);  // wait half a second to let the user see it
	  digitalWrite(chosen, HIGH);
	  delay(300);  // switch off and wait
	}
	for (int i = 0; i < simonLength; i++) {
	  bool inputReceived = false;
	  while (!inputReceived) {
		for (int j = 0; j < pairCount; j++) {
		  if (digitalRead(buttons[j]) == LOW) {
			inputReceived = true;
			if (j != simonSequence[i]) {
			  Serial.println("wrong button, game over");
			  delay(1000);
			  return;
			} else {
			  digitalWrite(leds[j], LOW);
			  delay(200);
			  digitalWrite(leds[j], HIGH);
			}
			delay(300);
		  }
		}
	  }
	}
	simonLength++;
	if (simonLength > MAX_SEQUENCE) simonLength = MAX_SEQUENCE;

	Serial.print("round ");
	Serial.print(simonLength - 1);
	Serial.println(" complete");
	delay(500);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  if (currentGameMode == HOME) {
	Serial.println("Select Game:");
	Serial.println("Press BUTTON 1 for Reaction Time");
	Serial.println("Press BUTTON 2 for Simon Says");
	while (currentGameMode == HOME) {
	  for (int i = 0; i < pairCount; i++) {
		digitalWrite(leds[i], HIGH);
	  }
	  digitalWrite(leds[flasherIndex], LOW);
	  flasherIndex = (flasherIndex + 1) % pairCount;
	  if (digitalRead(BUTTON1) == LOW) {
		delay(200);
		currentGameMode = REACTIONTIME;
		Serial.println("reaction time game selected");
	  } else if (digitalRead(BUTTON2) == LOW) {
		delay(200);
		currentGameMode = SIMONSAYS;
		Serial.println("simon says selected");
	  }
	}
  }
  if (currentGameMode == REACTIONTIME) {
	reactionTimeGame();
	// return to homepage afterg ame
	currentGameMode = HOME;
  }
  if (currentGameMode == SIMONSAYS) {
	simonSaysGame();
	// return to homepage after game
	currentGameMode = HOME;
  }
}
