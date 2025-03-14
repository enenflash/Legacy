#define BUZZER_PIN 33
#include <map>
using namespace std;

std::map<String, int> notes = {
  {"d", 294},
  {"e", 330},
  {"g", 392},
  {"b", 494},
  {"a", 440},
  {"f#", 370},
  {"d+", 587}

};

void setup() {
    pinMode(BUZZER_PIN, OUTPUT);
}
// 16th note = 100ms
void loop() {
    tone(BUZZER_PIN, notes["d"]);
    delay(100);
    tone(BUZZER_PIN, notes["e"]);
    delay(100);
    tone(BUZZER_PIN, notes["g"]);
    delay(100);
    tone(BUZZER_PIN, notes["e"]);
    delay(100);
    tone(BUZZER_PIN, notes["b"]);
    delay(300);
    noTone(BUZZER_PIN);
    delay(100);
    tone(BUZZER_PIN, notes["b"]);
    delay(200);
    tone(BUZZER_PIN, notes["a"]);
    delay(600);
    tone(BUZZER_PIN, notes["d"]);
    delay(100);
    tone(BUZZER_PIN, notes["e"]);
    delay(100);
    tone(BUZZER_PIN, notes["g"]);
    delay(100); // quater note
    tone(BUZZER_PIN, notes["e"]);
    delay(100);
    tone(BUZZER_PIN, notes["a"]);
    delay(300); 
    noTone(BUZZER_PIN);
    delay(100);
    tone(BUZZER_PIN, notes["a"]);
    delay(200);
    tone(BUZZER_PIN, notes["g"]);
    delay(300);
    tone(BUZZER_PIN, notes["f#"]);
    delay(100);
    tone(BUZZER_PIN, notes["e"]);
    delay(300);
    tone(BUZZER_PIN, notes["d"]);
    delay(100);
    tone(BUZZER_PIN, notes["e"]);
    delay(100);
    tone(BUZZER_PIN, notes["g"]);
    delay(100); 
    tone(BUZZER_PIN, notes["e"]);
    delay(100);
    tone(BUZZER_PIN, notes["g"]);
    delay(400);
    tone(BUZZER_PIN, notes["a"]);
    delay(200);
    tone(BUZZER_PIN, notes["f#"]);
    delay(300);
    tone(BUZZER_PIN, notes["e"]);
    delay(100);
    tone(BUZZER_PIN, notes["d"]);
    delay(200);
    noTone(BUZZER_PIN);
    delay(50);
    tone(BUZZER_PIN, notes["d"]);
    delay(200);
    noTone(BUZZER_PIN);
    delay(50);
    tone(BUZZER_PIN, notes["d"]);
    delay(200);
    tone(BUZZER_PIN, notes["a"]);
    delay(400);
    tone(BUZZER_PIN, notes["g"]);
    delay(800);
    tone(BUZZER_PIN, notes["d"]);
    delay(100);
    tone(BUZZER_PIN, notes["e"]);
    delay(100);
    tone(BUZZER_PIN, notes["g"]);
    delay(100);
    tone(BUZZER_PIN, notes["e"]);
    delay(100);
    tone(BUZZER_PIN, notes["b"]);
    delay(300);
    noTone(BUZZER_PIN);
    delay(100);
    tone(BUZZER_PIN, notes["b"]);
    delay(300);
    tone(BUZZER_PIN, notes["a"]);
    delay(600);
    tone(BUZZER_PIN, notes["d"]);
    delay(100);
    tone(BUZZER_PIN, notes["e"]);
    delay(100);
    tone(BUZZER_PIN, notes["g"]);
    delay(100);
    tone(BUZZER_PIN, notes["e"]);
    delay(100);
    tone(BUZZER_PIN, notes["d+"]);
    delay(400);
    tone(BUZZER_PIN, notes["f#"]);
    delay(200);
    tone(BUZZER_PIN, notes["g"]);
    delay(300);
    tone(BUZZER_PIN, notes["f#"]);
    delay(100);
    tone(BUZZER_PIN, notes["e"]);
    delay(200);
    tone(BUZZER_PIN, notes["d"]);
    delay(100);
    tone(BUZZER_PIN, notes["e"]);
    delay(100);
    tone(BUZZER_PIN, notes["g"]);
    delay(100);
    tone(BUZZER_PIN, notes["e"]);
    delay(100);
    tone(BUZZER_PIN, notes["g"]);
    delay(400);
    tone(BUZZER_PIN, notes["a"]);
    delay(200);
    tone(BUZZER_PIN, notes["f#"]);
    delay(300);
    tone(BUZZER_PIN, notes["e"]);
    delay(100);
    tone(BUZZER_PIN, notes["d"]);
    delay(400);
    noTone(BUZZER_PIN);
    delay(50);
    tone(BUZZER_PIN, notes["d"]);
    delay(200);
    tone(BUZZER_PIN, notes["a"]);
    delay(400);
    tone(BUZZER_PIN, notes["g"]);
    delay(800);
    noTone(BUZZER_PIN);
    delay(400);
    






    // END
    noTone(BUZZER_PIN);
    delay(2000);
}
