/*
 * receiver.cc
 *
 *  Created on: Dec 27, 2017
 *      Author: David Franco
 */

#include <string.h>
#include <omnetpp.h>
#include "paquete_m.h"
using namespace omnetpp;

class receiver : public cSimpleModule {

private:
    //int state;
    //int event;
    //cQueue *txQueue;
    paquete *nuevoPaq;
    paquete *recibPaq;
    bool esperar;
    int expectedSeqNum;
protected:
   virtual void initialize() override;
   virtual void handleMessage(cMessage *msg) override;
};

Define_Module(receiver);

void receiver::initialize() {

    //state = 0;
    //txQueue = new cQueue("txQueue");
    esperar = false;
}
void receiver::handleMessage(cMessage *msg) {

    recibPaq = (paquete*) msg;
    if( esperar == true && recibPaq->getMensaje() == expectedSeqNum ) {
        esperar=false;
    }

    if (recibPaq -> hasBitError()) {
        //recibPaq -> removeFromOwnershipTree();
        //nuevoPaq = new paquete("NACK");
        //nuevoPaq -> setBitLength(64);
        //send(nuevoPaq,"out");
        //numNACK++;
        esperar=true;
        expectedSeqNum=recibPaq->getMensaje();
    }
    else if (!esperar) {
        nuevoPaq = new paquete("ACK");
        nuevoPaq -> setBitLength(64);
        nuevoPaq -> setMensaje(recibPaq->getMensaje());
        send(nuevoPaq,"out");
        //numACK++;
    }

}




