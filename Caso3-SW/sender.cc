/*
 * sender.cc
 *
 *  Created on: Dec 24, 2017
 *      Author: David Franco
 */

#include <string.h>
#include <omnetpp.h>
#include "paquete_m.h"
using namespace omnetpp;

class sender : public cSimpleModule {

private:
    int state;
    int event;
    cQueue *txQueue;
    paquete *nuevoPaq;
    paquete *copiaPaq;
protected:
   virtual void initialize() override;
   virtual void handleMessage(cMessage *msg) override;
};

Define_Module(sender);

void sender::initialize() {

    state = 0;
    txQueue = new cQueue("txQueue");
}
void sender::handleMessage(cMessage *msg) {

    /* Maquina de estados completa */

    if(strcmp(msg -> getName(),"sourcePaq") == 0) {
        event = 0; //llegada de paq de la fuente
        nuevoPaq = new paquete("senderPaq");
        nuevoPaq -> setBitLength(exponential(150));
    }
    else if(strcmp(msg -> getName(),"ACK") == 0) {
        event = 1; //legada de ACK
    }
    else {
        event = 2; // llegada de NACK
    }

    switch (state) {

    case 0: //No ACK pendiente y cola vacia
        if(event == 0) {
            copiaPaq = new paquete("senderPaq");
            copiaPaq -> setBitLength(nuevoPaq -> getBitLength());
            send(nuevoPaq,"out");
            state = 1;
        }
        else if(event ==1 ) {
            //Esto no puede ocurrir
        }
        else {
            //Esto no puede ocurrir
        }
        break;

    case 1: //ACK pendiente y cola vacia
        if(event == 0) {
            txQueue -> insert(nuevoPaq);
            state = 2;
        }
        else if( event == 1) {
            state = 0;
        }
        else {
            send(copiaPaq,"out");//Se reenvia el paq
            copiaPaq = new paquete("senderPaq");
            copiaPaq -> setBitLength(nuevoPaq -> getBitLength());
        }
        break;

    case 2://ACK pediente y cola llena
        if (event == 0) {
            txQueue -> insert(nuevoPaq);
        }
        else if (event == 1) {
            nuevoPaq = (paquete*) txQueue -> pop();
            copiaPaq = new paquete("senderPaq");
            copiaPaq -> setBitLength(nuevoPaq -> getBitLength());
            send(nuevoPaq,"out");
            if(txQueue -> isEmpty()) {
                state = 1;
            }
        }
        else {
            send(copiaPaq,"out");//Se reenvia el paq
            copiaPaq = new paquete("senderPaq");
            copiaPaq -> setBitLength(nuevoPaq -> getBitLength());

        }
        break;

    }

}





