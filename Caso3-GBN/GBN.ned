//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//
import ned.DatarateChannel;

simple source
{
    parameters:
        @display("i=block/source");
    gates:
        output out;
}

simple sender
{
    parameters:
        @display("i=block/routing");
    gates:
        input inPacket;
        input in;
        output out;
}

simple receiver
{
    parameters:
        @display("i=block/routing");
    gates:
        input in;
        output out;
}
network GoBackN
{
    @display("bgb=258,225");

    types:
        channel C extends DatarateChannel
        {
            datarate = 100000bps;
            per = 0.5;
            delay = 1000ms;
        }
    submodules:
        source: source {
            @display("p=117.02667,33.18667");
        }
        sender: sender {
            @display("p=30.566668,123.14001");
        }
        receiver: receiver {
            @display("p=193.88,123.14001");
        }
    connections:
        source.out --> sender.inPacket;
        sender.out --> C --> receiver.in;
        sender.in <-- C <-- receiver.out;
            
}
