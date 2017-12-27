/*
 * receiver.cc
 *
 *  Created on: Dec 24, 2017
 *      Author: David Franco
 */

#include <string.h>
#include <omnetpp.h>
#include "paquete_m.h"
using namespace omnetpp;

class receiver : public cSimpleModule {

private:
    paquete *nuevoPaq;
    /*Statistics*/
    long numACK;
    long numNACK;
    cLongHistogram ackCountStats;
    cOutVector ackCountVector;
    cLongHistogram nackCountStats;
    cOutVector nackCountVector;
protected:
   virtual void initialize() override;
   virtual void handleMessage(cMessage *msg) override;
   virtual void finish() override;

};

Define_Module(receiver);

void receiver::initialize() {
    numACK = 0;
    numNACK = 0;

}
void receiver::handleMessage(cMessage *msg) {


    paquete* recibPaq = (paquete*) msg;
    if (recibPaq -> hasBitError()) {
        //recibPaq -> removeFromOwnershipTree();
        nuevoPaq = new paquete("NACK");
        nuevoPaq -> setBitLength(64);
        send(nuevoPaq,"out");
        numNACK++;
    }
    else {
        nuevoPaq = new paquete("ACK");
        nuevoPaq -> setBitLength(64);
        send(nuevoPaq,"out");
        numACK++;

    }

    /*Statistics*/
    ackCountVector.record(numACK);
    ackCountStats.collect(numACK);

    nackCountVector.record(numNACK);
    nackCountStats.collect(numNACK);

}

void receiver::finish()
{
    // This function is called by OMNeT++ at the end of the simulation.
    EV << "ACK number:        " << numACK << endl;
    EV << "NACK number:       " << numNACK << endl;
    //EV << "ACK count, min:    " << ackCountStats.getMin() << endl;
    //EV << "ACK count, max:    " << ackCountStats.getMax() << endl;
    //EV << "ACK count, mean:   " << ackCountStats.getMean() << endl;
    //EV << "ACK count, stddev: " << ackCountStats.getStddev() << endl;
    recordScalar("#ACK", numACK);
    recordScalar("#NACK", numNACK);
    ackCountStats.recordAs("ACK count");
    ackCountStats.recordAs("NACK count");
}




