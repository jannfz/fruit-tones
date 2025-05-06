#include "gpio.h"
#include "FruitKeyboard.h"
#include <SoftwareSerial.h>

// Static instance inside the file scope
static SoftwareSerial EspSerial(ESP_RX_PIN, ESP_TX_PIN);

int8_t espSetup(uint32_t baudrate) {
    EspSerial.begin(baudrate);
    return Error::Ok;
}

int8_t espSetNote(uint8_t note) {
    EspSerial.write(CMD_NOTE);
    EspSerial.write(note);
    return Error::Ok;
}

int8_t espGetNewGame(uint8_t &song, uint8_t &mode) {

    if (EspSerial.available() < 3) {
        return Error::NotReady;
    }

    uint8_t buffer[3];
    size_t len = EspSerial.readBytes(buffer, 3);

    if (len == 3 && buffer[CMD_NEW_POS] == CMD_NEW) {

        if (buffer[CMD_SONG_POS] >= NUM_SONGS) {
            song = buffer[CMD_SONG_POS];
            Serial.println("unoStartGame - song -> Invalid Parameter");
            return Error::InvParam;
        }

        if (buffer[CMD_MODE_POS] >= NUM_MODES) {
            mode = buffer[CMD_MODE_POS];
            Serial.println("unoStartGame - mode -> Invalid Parameter");
            return Error::InvParam;
        }

        return ;
    }

    return Error::Err;
}

