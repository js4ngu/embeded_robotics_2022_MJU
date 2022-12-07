#define POT_X 0
#define POT_Y 1

int potVal[2];
bool SW;

void setup() {
    Serial.begin(9600);
    pinMode(A5, INPUT);
}

void loop() {
    SW = digitalRead(A5);
    potVal[POT_X] = analogRead(A4);
    potVal[POT_Y] = analogRead(A4);
    Serial.print(SW);
    Serial.print("\t");
    Serial.print(potVal[0]);
    Serial.print("\t");
    Serial.println(potVal[1]);
}
