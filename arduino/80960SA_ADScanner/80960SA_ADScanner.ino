// A mega2560 sketch to inspect the data lines / address lines of the 80960SA
// processor. This will grow organically but first lets take a look at the
// BE1,BE0, and BLAST_ pins
constexpr auto BLAST = 4;
constexpr auto BE1 = 3;
constexpr auto BE0 = 2;
void setup() {
    Serial.begin(9600);
    pinMode(BLAST, INPUT);
    pinMode(BE1, INPUT);
    pinMode(BE0, INPUT);
}

void loop() {
    Serial.print(digitalRead(BLAST));
    Serial.print(" ");
    Serial.print(digitalRead(BE0));
    Serial.print(" ");
    Serial.print(digitalRead(BE1));
    Serial.println();
    delay(1);
}
