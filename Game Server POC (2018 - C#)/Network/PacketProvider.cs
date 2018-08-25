using System;
using System.Runtime.InteropServices;

using KongregateGameServer.Network.PacketDatabase;

namespace KongregateGameServer.Network
{
    enum EPackets
    {
        eKeepAlive = 0,
        eLogInResponse ,
        eLogInRequest,
        ePlayerInfo,

        eNone
    }

    struct Packet
    {
        public byte[] payload;
        public short size;
    }

    class PacketProvider
    {
        /// <summary>
        /// Builds a packet with a payload and a size from a packet id. Recieves any number of arguments corresponding to
        /// each field in the packet struct, same order and same type. Prints exception if mapping fails
        /// </summary>
        /// <param name="packet_id">The packet id to build</param>
        /// <param name="fields">The fields corresponding to the struct fields in the exact same order</param>
        /// <returns>A packet struct which can be used by TCP or websocket</returns>
        public static Packet BuildPacketPayload(EPackets packet_id, params object[] fields)
        {
            Packet packet = new Packet { payload = null, size = 0};
            object packetData = null;

            try
            {
                switch (packet_id)
                {
                    case EPackets.eKeepAlive:
                    {
                        packetData = new KeepAlive()
                        {
                            id = (byte)packet_id
                        };
                        packet.size = (short)Marshal.SizeOf<KeepAlive>();
                    }break;

                    case EPackets.eLogInRequest:
                    {
                        packetData = new LogInRequest()
                        {
                            id = (byte)packet_id,
                            clientId = (int)fields[0]
                        };
                        packet.size = (short)Marshal.SizeOf<LogInRequest>();
                    }break;

                    case EPackets.eLogInResponse:
                    {
                        packetData = new LogInResponse
                        {
                            id = (byte)packet_id,
                            status = (byte)fields[0]
                        };
                        packet.size = (short)Marshal.SizeOf<LogInResponse>();
                    }break;

                    case EPackets.ePlayerInfo:
                    {
                        packetData = new PlayerInfo
                        {
                            id = (byte)packet_id,
                            playerId = (int)fields[0],
                            playerName = (string)fields[1]
                        };
                        packet.size = (short)Marshal.SizeOf<PlayerInfo>();
                    }break;

                    default:
                    {
                        Console.WriteLine("Unrecognized packet id while building payload");
                    }
                    break;
                }
            }catch(Exception e)
            {
                Console.WriteLine("Malformed packet id {0}. Exception {1}", packet_id, e.Message);
                return packet;
            }

            IntPtr ptr = IntPtr.Zero;
            ptr = Marshal.AllocHGlobal(packet.size); //Allocs enough size in memory for the packet data in struct
            Marshal.StructureToPtr(packetData, ptr, true); // maps the struct into the + pointer

            //Allocs enough bytes for the payload and writes the packetlength to the first bytes
            packet.payload = new Byte[packet.size + sizeof(short)];
            for(int i=0;i<sizeof(short); i++)            
                packet.payload[i] = (byte)(packet.size >> 8 * i);            

            //Copy the packet data into the payload
            Marshal.Copy(ptr, packet.payload, sizeof(short) , packet.size);
            packet.size += sizeof(short);
            return packet;
        }
        
        /// <summary>
        /// Takes a byte array with packet data and length. Turns it into the corresponding packet structure
        /// and distributes it
        /// </summary>
        /// <param name="packetData">the byte array with packet data</param>
        /// <param name="length">the length of the packet</param>
        public static void ParsePacketData(byte[] packetData, short length)
        {
            IntPtr structurePtr = Marshal.AllocHGlobal(length);
            Marshal.Copy(packetData, 0, structurePtr, length);

            switch ((EPackets)packetData[0])
            {
                case EPackets.eKeepAlive: PacketDistributer.DistributePacket(Marshal.PtrToStructure<KeepAlive>(structurePtr)); break;
                case EPackets.eLogInRequest: PacketDistributer.DistributePacket(Marshal.PtrToStructure<LogInRequest>(structurePtr)); break;
                case EPackets.eLogInResponse: PacketDistributer.DistributePacket(Marshal.PtrToStructure<LogInResponse>(structurePtr)); break;
                case EPackets.ePlayerInfo: PacketDistributer.DistributePacket(Marshal.PtrToStructure<PlayerInfo>(structurePtr)); break;

                default: Console.WriteLine("Received unrecognized packet id"); break;
            }
        }
    }
}
