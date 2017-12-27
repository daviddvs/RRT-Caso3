/*
 * sender.cc
 *
 *  Created on: Dec 27, 2017
 *      Author: David Franco
 */

#include <string.h>
#include <omnetpp.h>
#include "paquete_m.h"
using namespace omnetpp;

class sender: public cSimpleModule {
private:
    int state, event;
    int base, N, nextSeqNum;
    int n;
    cQueue *txQueue, *window, *copiaWindow;
    paquete *nuevoPaq, *copiaPaq, *copia2Paq, *recibPaq;
    cMessage *newMsg;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(sender);

void sender::initialize(){
    state = 0;
    txQueue = new cQueue("txQueue");
    window = new cQueue("window");
    copiaWindow = new cQueue("copiaWindow");
    base=0; N=4; nextSeqNum=0;
}

void sender::handleMessage(cMessage *msg) {

    if(strcmp(msg -> getName(),"sourcePaq") == 0) {
        event = 0; //llegada de paq de la fuente
        nuevoPaq = new paquete("senderPaq");
        nuevoPaq -> setBitLength(exponential(150));
        txQueue -> insert(nuevoPaq);
        //copiaPaq = new paquete("senderPaq");
        //copiaPaq -> setBitLength(nuevoPaq->getBitLength());
    }
    else if(strcmp(msg -> getName(),"ACK") == 0) {
        event = 1; //legada de ACK
        recibPaq = (paquete*) msg;
    }
    else if(strcmp(msg -> getName(),"TIMEOUT") == 0) {
        event = 2;
    }

    switch (state) {
    case 0: //ventana sin llenar

        if(event == 0) {
            if ( window -> isEmpty() ) { //Se inicia el temporizador
                cancelAndDelete(newMsg);
                newMsg = new cMessage("TIMEOUT");
                scheduleAt(simTime()+10, newMsg);
            }
            nuevoPaq = (paquete*) txQueue -> pop();
            nuevoPaq -> setMensaje(nextSeqNum);
            copiaPaq = new paquete("senderPaq");
            copiaPaq -> setBitLength(nuevoPaq->getBitLength());
            copia2Paq = new paquete("senderPaq");
            copia2Paq -> setBitLength(nuevoPaq->getBitLength());
            window -> insert(copiaPaq);
            copiaWindow -> insert(copia2Paq);
            send(nuevoPaq,"out");
            nextSeqNum++;
            if( nextSeqNum == (base+N) ) {
                state = 1;
                nextSeqNum=0;
             }
        }
        else if(event == 1) { //Se borran tantos como hayan seq haya llegado y se reinicia el timeout
            for(n=1; n <= (recibPaq->getMensaje() + 1) ; n++) {
                window -> pop();
                nextSeqNum--;
            }
            state = 0;
            cancelAndDelete(newMsg);
        }
        else if(event == 2) {//Enviar todos los que estan en la ventana de nuevo
            for(n=0; n<nextSeqNum; n++) {
                nuevoPaq = (paquete*) copiaWindow -> pop();
                nuevoPaq -> setMensaje(n);
                send(nuevoPaq,"out");
                if (n==0){
                    cancelAndDelete(newMsg);
                    newMsg = new cMessage("TIMEOUT");
                    scheduleAt(simTime()+10, newMsg);
                }
            }
        }
        break;
    case 1://ventana llena
        if(event==0) {//No se hace nada
            //No se hace nada
        }
        else if(event==1) {//Lo mismo que en state=0
            for(n=1; n <= (recibPaq->getMensaje() + 1) ; n++) {
                 window -> pop();
                 nextSeqNum--;
            }
            state = 0;
            cancelAndDelete(newMsg);
        }
        else if(event==2) {//Lo mismo que en state=0 pero cambiando N
            for(n=0; n<N; n++) {
                nuevoPaq = (paquete*) copiaWindow -> pop();
                nuevoPaq -> setMensaje(n);
                send(nuevoPaq,"out");
                if (n==0){
                    cancelAndDelete(newMsg);
                    newMsg = new cMessage("TIMEOUT");
                    scheduleAt(simTime()+10, newMsg);
                }
            }
        }
        break;
    }

}

