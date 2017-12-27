/*
 * source.cc
 *
 *  Created on: Dec 20, 2017
 *      Author: David Franco
 */
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "paquete_m.h"
using namespace omnetpp;



class source : public cSimpleModule
{
  protected:
    // Redefined virtual function to hold the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
  private:
    paquete * nuevoPqt;
    simtime_t startTime;
};

// The module class needs to be registered with OMNeT++
Define_Module(source);

void source::initialize()
{
    /* Initialize is called at the beginning of
     * the simulation */

    startTime=20;
    cMessage *msg = new cMessage("sourceMsg");//sourceMsg: name of the message object.
    scheduleAt(startTime, msg);

    /*startTime=20;
    nuevoPqt = new paquete();
    scheduleAt(startTime, nuevoPqt);
    seq=0;*/
}
void source::handleMessage(cMessage *msg)
{
    /* The handleMessage() method is called
     * whenever a message arrives at the module */

    paquete * paqSender = new paquete("sourcePaq");
    paqSender -> setBitLength(1024);
    send(paqSender, "out"); // send out the packet, aunque la función send pone que es solo para *msg
    cMessage *newMsg = new cMessage("sourceMsg");
    scheduleAt(simTime()+exponential(startTime), newMsg);

    /*paquete *pqt = generaPaquete();
    send(pqt,"out");
    scheduleAt(simTime()+exponential(startTime),nuevoPqt);*/
}
