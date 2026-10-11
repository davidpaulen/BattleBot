#include "telemetri.h"

void Rx::setup(){
    // serial.setRX(pin);
    // serial.setTX(-1);
    // ibus.begin(serial, IBUSBM_NOTIMER);
    serial.setRX(pin);
    serial.begin(115200);
    ibus.begin(serial, IBUSBM_NOTIMER);
}

int Rx::getValue(int channel){
    return ibus.readChannel(channel);
}


void Rx::update() {
    // Les nye iBUS-data frå UART-bufferet
    ibus.loop();
}