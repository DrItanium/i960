// A mega2560 sketch to inspect the data lines / address lines of the 80960SA
// processor. This will grow organically but first lets take a look at the
// BE1,BE0, and BLAST_ pins
constexpr auto END0 = 19;
constexpr auto START0 = 2;
constexpr auto READY_ = 18;
constexpr auto ALE = 17;
constexpr auto AS_ = 16;
constexpr auto LOCK_ = 15;
constexpr auto DTR_ = 14;
constexpr auto WR_ = 13;
constexpr auto DEN_ = 12;
constexpr auto HOLD = 11;
constexpr auto INT3_INTA_ = 10;
constexpr auto HLDA = 9;
constexpr auto INT3_ = 8;
constexpr auto INT2_INTR = 7;
constexpr auto RESET = 6;
constexpr auto INT0_ = 5;
constexpr auto BLAST = 4;
constexpr auto BE1 = 3;
constexpr auto BE0 = 2;
void setup() {
    Serial.begin(9600);
    for (int i = START0; i < END0; ++i) {
        pinMode(i, INPUT);
    }
}
void loop() {
    for (int i = START0; i < END0; ++i) {
        Serial.print((digitalRead(i) * 10) + (20 * (i - START0)));
        Serial.print(" ");
    }
    Serial.println();
    delay(1);
}
