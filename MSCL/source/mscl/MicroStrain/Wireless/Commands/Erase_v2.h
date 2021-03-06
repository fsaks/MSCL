/*******************************************************************************
Copyright(c) 2015-2016 LORD Corporation. All rights reserved.

MIT Licensed. See the included LICENSE.txt for a copy of the full MIT License.
*******************************************************************************/
#pragma once

#include "mscl/MicroStrain/ByteStream.h"
#include "mscl/MicroStrain/ResponsePattern.h"

namespace mscl
{
    class WirelessPacket;

    //Class: Erase_v2
    //  Contains logic for the Erase Node command (version 2).
    class Erase_v2
    {
    private:
        Erase_v2();                             //default constructor disabled
        Erase_v2(const Erase_v2&);              //copy constructor disabled
        Erase_v2& operator=(const Erase_v2&);   //assignment operator disabled 

    public:
        //Function: buildCommand
        //    Builds the Erase_v2 command packet
        //
        //Parameters:
        //    nodeAddress - the address of the Node to build the command for.
        //
        //Returns:
        //    A <ByteStream> containing the Erase_v2 command packet.
        static ByteStream buildCommand(NodeAddress nodeAddress);

        //Class: Response
        //    Handles the response to the Erase_v2 Node command.
        class Response : public ResponsePattern
        {
        public:
            //Constructor: Response
            //    Creates a Erase_v2 Response object.
            //
            //Parameters:
            //    nodeAddress - the node address to check for.
            //    collector - The <ResponseCollector> used to register and unregister the response.
            Response(NodeAddress nodeAddress, std::weak_ptr<ResponseCollector> collector);

        private:
            //Variable: m_nodeAddress
            //    The node address to look for in the response
            NodeAddress m_nodeAddress;

        public:
            //Function: match
            //    Checks if the <WirelessPacket> passed in matches the expected response pattern's bytes
            //
            //Parameters:
            //    packet - The <WirelessPacket> in which to try to find the pattern
            //
            //Returns:
            //    true if the packet matches a response pattern, false otherwise
            virtual bool match(const WirelessPacket& packet) override;
        };
    };
}