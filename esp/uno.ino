#include "gpio.h"
#include "fruit_keyboard.h"
#include <SoftwareSerial.h>

// Static instance inside the file scope
static SoftwareSerial UnoSerial(UNO_RX_PIN, UNO_TX_PIN);


int8_t unoSetup(uint32_t baudrate) {
    UnoSerial.begin(baudrate);
    return Error::Ok;
}

int8_t unoStartListen(uint8_t song) {

    if (song >= NUM_SONGS) {
        Serial.println("unoStartGame - song -> Invalid Parameter");
        return 0;
    }

    UnoSerial.write(CMD_LISTEN);
    UnoSerial.write(song + 1);

    return 1;
}
int8_t unoAvailable() {
    if (UnoSerial.available()) {
        return 1;
    }
    return 0;
}

int8_t unoCheckDone() {

    if (UnoSerial.available()) {
        uint8_t cmd = UnoSerial.read();
        if (cmd == CMD_DONE) {
            return 1;
        }
    }

    return 0;
}



int8_t unoGetData() {

    if (UnoSerial.available()) {

        uint8_t buffer[2];
        size_t len = UnoSerial.readBytes(buffer, 2);

        if (buffer[0] == CMD_DONE) {
            return CMD_DONE;
        }

        if (len == 2 && buffer[0] == CMD_NOTE) {
            if (buffer[1] >= NUM_NOTES) {
                return Error::InvParam;
            }
            return buffer[1];
        }
    }

    return Error::Err;
}
