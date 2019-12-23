// emulate the external interface of the 80960SA using an
// Adafruit Metro M4 Grand Central


// i960SB/SA pins according to the datasheet
constexpr auto SystemClock = -1; // unsure what to do with this yet so we'll ignore it
constexpr auto AddressBus16 = 39;
constexpr auto AddressBus17 = 40;
constexpr auto AddressBus18 = 41;
constexpr auto AddressBus19 = 42;
constexpr auto AddressBus20 = 43;
constexpr auto AddressBus21 = 44;
constexpr auto AddressBus22 = 45;
constexpr auto AddressBus23 = 46;
constexpr auto AddressBus24 = 47;
constexpr auto AddressBus25 = 48;
constexpr auto AddressBus26 = 49;
constexpr auto AddressBus27 = A2;
constexpr auto AddressBus28 = A3;
constexpr auto AddressBus29 = A4;
constexpr auto AddressBus30 = A5;
constexpr auto AddressBus31 = A6;
constexpr auto DataBus0 = 22;
constexpr auto AddressDataBus1 = 23;
constexpr auto AddressDataBus2 = 24;
constexpr auto AddressDataBus3 = 25;
constexpr auto AddressDataBus4 = 26;
constexpr auto AddressDataBus5 = 27;
constexpr auto AddressDataBus6 = 28;
constexpr auto AddressDataBus7 = 29;
constexpr auto AddressDataBus8 = 31;
constexpr auto AddressDataBus9 = 32;
constexpr auto AddressDataBus10 = 33;
constexpr auto AddressDataBus11 = 34;
constexpr auto AddressDataBus12 = 35;
constexpr auto AddressDataBus13 = 36;
constexpr auto AddressDataBus14 = 37;
constexpr auto AddressDataBus15 = 38;
constexpr auto AddressBus1Copy = A9;
constexpr auto AddressBus2Copy = A10;
constexpr auto AddressBus3Copy = A11;
constexpr auto AddressLatchEnable = 10;
constexpr auto AddressStatus_ = 9;
constexpr auto Write_Read_= 5;
constexpr auto DataEnable_ = 4;
constexpr auto DataTransmit_Recieve_ = 7;
constexpr auto Ready_ = 11;
constexpr auto BusLock_ = 8;
constexpr auto ByteEnable0_ = A7;
constexpr auto ByteEnable1_ = A8;
constexpr auto Hold = 3;
constexpr auto HoldAcknowledge = 2;
constexpr auto BurstLast_ = 6;
constexpr auto Reset_ = 12;
constexpr auto Interrupt0_ = A12;
constexpr auto Interrupt1_ = A13;
constexpr auto Interrupt2_InterruptRequest = A14;
constexpr auto Interrupt3__InterruptAcknowledge_ = A15;

constexpr decltype(DataBus0) DataLines[] = {
    DataBus0,
    AddressDataBus1,
    AddressDataBus2,
    AddressDataBus3,
    AddressDataBus4,
    AddressDataBus5,
    AddressDataBus6,
    AddressDataBus7,
    AddressDataBus8,
    AddressDataBus9,
    AddressDataBus10,
    AddressDataBus11,
    AddressDataBus12,
    AddressDataBus13,
    AddressDataBus14,
    AddressDataBus15,
};

constexpr decltype(AddressBus31) AddressLines[] = {
    AddressDataBus1,
    AddressDataBus2,
    AddressDataBus3,
    AddressDataBus4,
    AddressDataBus5,
    AddressDataBus6,
    AddressDataBus7,
    AddressDataBus8,
    AddressDataBus9,
    AddressDataBus10,
    AddressDataBus11,
    AddressDataBus12,
    AddressDataBus13,
    AddressDataBus14,
    AddressDataBus15,
    AddressBus16,
    AddressBus17,
    AddressBus18,
    AddressBus19,
    AddressBus20,
    AddressBus21,
    AddressBus22,
    AddressBus23,
    AddressBus24,
    AddressBus25,
    AddressBus26,
    AddressBus27,
    AddressBus28,
    AddressBus29,
    AddressBus30,
    AddressBus31,
};
void tristatePin(decltype(DataBus0) pin) noexcept {
    pinMode(pin, INPUT);
    digitalWrite(pin, LOW);
    
}
void setAddressLinesDirection(decltype(INPUT) direction)  noexcept {
    for (const auto& pin : AddressLines) {
        pinMode(pin, direction);
    }
}
void setDataLinesDirection(decltype(INPUT) direction) noexcept {
    for (const auto& pin : DataLines) {
        pinMode(pin, direction);
    }
}
void disableAddressLines() noexcept {
    for (const auto& pin : AddressLines) {
        tristatePin(pin);
    }
}
void disableDataLines() noexcept {
    for (const auto& pin : DataLines ) {
        tristatePin(pin);
    }
}

void setup() {
    disableAddressLines();
    disableDataLines();
    pinMode(Ready_, INPUT);
    tristatePin(AddressBus1Copy);
    tristatePin(AddressBus2Copy);
    tristatePin(AddressBus3Copy);
    pinMode(AddressLatchEnable, OUTPUT);
    digitalWrite(AddressLatchEnable, LOW);
    // driven high during reset
    pinMode(AddressStatus_, OUTPUT);
    digitalWrite(AddressStatus_, HIGH);
    tristatePin(Write_Read_);
    tristatePin(DataEnable_);
    tristatePin(DataTransmit_Recieve_);

}

void loop() {

}
