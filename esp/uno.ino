#include "gpio.h"
#include "fruit_keyboard.h"
#include <SoftwareSerial.h>

// Static instance inside the file scope
static SoftwareSerial UnoSerial(UNO_RX_PIN, UNO_TX_PIN);


int8_t unoSetup(uint32_t baudrate) {
    UnoSerial.begin(baudrate);
    return Error::Ok;
}

int8_t unoSendCommand(uint8_t cmd) {
    UnoSerial.write(cmd);
    UnoSerial.write(0x00);
    return 1;
}

int8_t unoSendCommand(uint8_t cmd, uint8_t param) {
    UnoSerial.write(cmd);
    UnoSerial.write(param + 1);
    return 1;
}

uint8_t unoFlush() {
    uint8_t i = 0;
    while (UnoSerial.available()) {
        UnoSerial.read(); // Discard each byte
        i++;
    }
    return Error::Ok;
}

int8_t unoStartListen(uint8_t song) {
    UnoSerial.write(CMD_LISTEN);
    UnoSerial.write(song + 1);
    return 1;
}

int8_t unoStartFreeplay() {
    UnoSerial.write(CMD_FREEPLAY);
    return 1;
}


int8_t unoStartPlay(uint8_t song) {

    if (song >= NUM_SONGS) {
        Serial.println("unoStartGame - song -> Invalid Parameter");
        return 0;
    }

    UnoSerial.write(CMD_PLAY);
    UnoSerial.write(song + 1);

    return 1;
}


int8_t unoGetData(Command_t *msg) {
    if (UnoSerial.available()) {

        uint8_t cmd = UnoSerial.peek();
        uint8_t len = UnoSerial.available();

        if (cmd == CMD_DONE) {
            msg->cmd = UnoSerial.read();
            return Error::Ok;
        } else if (cmd == CMD_NOTE && len > 1) {
            msg->cmd = UnoSerial.read();
            msg->param = UnoSerial.read();
            return Error::Ok;
        } else if (cmd == CMD_RESULT && len > 1) {
            msg->cmd = UnoSerial.read();
            msg->param = UnoSerial.read();
            return Error::Ok;
        };
    }

    return Error::Err;
}
