//Mole game for arduino mega with 10 lightup buttons

#include <Wire.h>
#include <rgb_lcd.h>

rgb_lcd lcd;

/*
 * OFS stands for offset
 * It means that 40 to 49 are the pins
 * that take the button input.
 * And 30 to 39 are the pins used to control
 * the leds
 * AMT stands for the amount of buttons
 * RSG_D is: ready set go delay
 */
#define LED_OFS 30
#define BTN_OFS 40
#define AMT 10
#define RSG_D 1000

#define SCORE_SHOW_DELAY 4000
#define PLAYS 5
#define POINT_TIME 1500
#define MAX_SINGLE_SCORE 500
#define PENALTY 40

void setup() {
  lcd.begin(16, 2);
  for (int i = 0; i < AMT; ++i) {
    pinMode(LED_OFS + i, OUTPUT);
    pinMode(BTN_OFS + i, INPUT);
    digitalWrite(LED_OFS + i, LOW);
  }
  randomSeed(analogRead(0));
}

int prevScore = 0;
void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press to play ->");
  lcd.setCursor(0, 1);
  lcd.print("score: ");
  lcd.print(prevScore);
  digitalWrite(LED_OFS + 3, HIGH);
  while (true) {
    if (digitalRead(BTN_OFS + 3) == HIGH) {
      break;
    }
  }
  digitalWrite(LED_OFS + 3, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ready");
  delay(RSG_D);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Set");
  delay(RSG_D);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GO!!!");
  delay(RSG_D);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Current score:");
  int score = 0;
  for (int i = 0; i < PLAYS; ++i) {
    lcd.setCursor(0, 1);
    lcd.print(score);
    int btn = random(0, AMT);
    delay(1000);
    int theDel = random(0, 20) * 100;
    bool penaltyGiven = false;
    for (int i = 0; i <= theDel; ++i) {
      for (int cBtn = 0; cBtn < AMT; ++cBtn) {
        if (digitalRead(BTN_OFS + cBtn) == HIGH) {
          if (!penaltyGiven) {
            score -= PENALTY;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Penalty -");
            lcd.print(PENALTY);
            lcd.setCursor(0, 1);
            lcd.print(score);
            penaltyGiven = true;
          }
          i = 0;
        }
      }
      delay(1);
    }
    lcd.setCursor(0, 0);
    lcd.print("Current score:");
    digitalWrite(LED_OFS + btn, HIGH);
    unsigned long start = millis();
    while (true) {
      if (digitalRead(BTN_OFS + btn) == HIGH) {
        int diff = millis() - start;
        digitalWrite(LED_OFS + btn, LOW);
        if (diff < POINT_TIME) {
          score += MAX_SINGLE_SCORE * (1.0 - (float)diff / POINT_TIME);
        }
        lcd.setCursor(0, 1);
        lcd.print(score);
        break;
      }
    }
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game over");
  lcd.setCursor(0, 1);
  lcd.print("score: ");
  lcd.print(score);
  delay(SCORE_SHOW_DELAY);
  prevScore = score;
}
