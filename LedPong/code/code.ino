/* Tale progetto è stato sviluppato dal gruppo formato da: Enrico Gnagnarella, Anis Lico, Tommaso Ghini, come specificato nel nome del file di codice*/
#define LED_FLASH 9
#define LED1 13
#define LED2 12
#define LED3 7


#define START_BUT 5
#define BUT_T1 2
#define BUT_T2 3

#define POTENTIOMETER A0

int i;
int game = 0;
int RT = 1000;
int startTime;
int elapsedTime;
int Speed ;
int dir;  // 0 --> sinistra, 1 --> destra
bool pressedButtonOne = false;
bool pressedButtonTwo = false;
int count = 0;
int loser = 0;
int brightness;
int fadeAmount;
int currIntensity;




void setup() {
  Serial.begin(9600);
  currIntensity = 0;
  fadeAmount = 5;
  pinMode(LED_FLASH, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(START_BUT, INPUT);
  randomSeed(analogRead(5));
  Serial.println("Welcome to Led Pong. Press Key T3 to Start");

  attachInterrupt(digitalPinToInterrupt(BUT_T1), check1, RISING);
  attachInterrupt(digitalPinToInterrupt(BUT_T2), check2, RISING);


}


void loop() {

  if (game == 0) {
    RT = 1000;
    count = 0;
    waitStart();
    readPotent();
    Serial.print("Velocità selezionata:");
    Serial.println(Speed);
    waitSecond();
  }
  if (dir == 1)
  {
    while (game == 1)
    {
      digitalWrite(LED2, HIGH);
      delay(Speed);
      runPlayerOne();

      decrementRT();
      if (game == 1)
      {
        digitalWrite(LED2, HIGH);
        delay(Speed);
        runPlayerTwo();

        decrementRT();
      }
    }// end while
  }

  if (dir == 0)
  {
    while (game == 1)
    {
      digitalWrite(LED2, HIGH);
      delay(Speed);
      runPlayerTwo();

      decrementRT();
      if (game == 1 )
      {
        digitalWrite(LED2, HIGH);
        delay(Speed);
        runPlayerOne();
        decrementRT();
      }
    }
  }

  if (loser == 1) {
    blinky(LED1);
    Serial.print("Game Over - The Winner is Player 2 after ");
    Serial.print(count);
    Serial.println(" shots");
  }
  if (loser == 2) {
    blinky(LED3);
    Serial.print("Game Over - The Winner is Player 1 after ");
    Serial.print(count);
    Serial.println(" shots");
  }
}

void runPlayerOne()
{
  //accende la sequenza di led
  digitalWrite(LED2, LOW);
  digitalWrite(LED1, HIGH);
  pressedButtonOne = true;
  if (game != 2 && game != 0) {
    noInterrupts();
    game = 2;
    loser = 1;
    interrupts();
  }

  delay(RT);
  pressedButtonOne = false;


  digitalWrite(LED1, LOW);

}

void runPlayerTwo()
{
  //Sequenza di led
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
  pressedButtonTwo = true;
  if (game != 2 && game != 0) {
    noInterrupts();
    game = 2;
    loser = 2;
    interrupts();
  }

  delay(RT);
  pressedButtonTwo = false;
  digitalWrite(LED3, LOW);
}


void waitSecond() {
    digitalWrite(LED2, HIGH);
    delay(2000);
    digitalWrite(LED2, LOW);
    randomDirection();
    game = 1;
    count = 0;
}

void waitStart() {

  while (game == 0) {
    if (digitalRead(START_BUT) == HIGH) {
      Serial.println("Go!");
      game = 1;
    }
    analogWrite(LED_FLASH, currIntensity);
    currIntensity = currIntensity + fadeAmount;
    if (currIntensity == 0 || currIntensity == 255) {
      fadeAmount = -fadeAmount ;
    }
    delay(15);
  }
  analogWrite(LED_FLASH, 0);
  game = 0;
}


void randomDirection() {
  dir = random(2);
}


void check1() {
  if (!pressedButtonOne) {
    game = 2;
    loser = 1;
  }
  else {
    count++;
    loser = 0;
    game = 1;
  }

}

void check2() {
  if (!pressedButtonTwo) {
    game = 2;
    loser = 2;
  }
  else {
    count++;
    loser = 0;
    game = 1;
  }
}

void blinky(int led) {
  startTime = millis();
  elapsedTime = 0;
  while (elapsedTime < 2000) //Il giocatore deve cliccale il bottone entro il tempo di react
  {
    elapsedTime = millis() - startTime;
    digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(led, LOW);
    delay(200);
  }
  game = 0;
  loser = 0;
}

void decrementRT() {
  RT  = ((RT / 8) * 7);
}

void readPotent() {
  if (analogRead(POTENTIOMETER) < 512) {
    Speed = 1000; // Speed sarebbe uguale a 1 e ha un tempo di 1000 ms
  } else {
    Speed = 500; // Speed sarebbe uguale a 2 e ha un tempo di 500 ms
  }
}
