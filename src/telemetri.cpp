#include "telemetri.h"

void Rx::setup(){
    serial.setRX(pin);
    serial.begin(115200);
    ibus.begin(serial, IBUSBM_NOTIMER);

    lastPacketCount = ibus.cnt_rec;
    lastPacketTime = 0;
    receivedPacket = false;
}

int Rx::getValue(int channel){
    return ibus.readChannel(channel);
}


void Rx::update() {
    // Les nye iBUS-data frå UART-bufferet
    ibus.loop();

    if(ibus.cnt_rec != lastPacketCount){
        lastPacketCount = ibus.cnt_rec;
        lastPacketTime = millis();
        receivedPacket = true;
    }
}

bool Rx::isReceiving(){
    if(!receivedPacket){
        return false;
    }

    return millis() - lastPacketTime < timeout;
}