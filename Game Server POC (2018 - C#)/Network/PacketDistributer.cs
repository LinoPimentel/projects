using System;

using KongregateGameServer.Network.PacketDatabase;

namespace KongregateGameServer.Network
{
    class PacketDistributer
    {
        public static void DistributePacket(KeepAlive packet)
        {
            Console.WriteLine("KeepAlive: {0}", packet.id);
        }

        public static void DistributePacket(LogInRequest packet)
        {
            Console.WriteLine("LogInRequest: {0}, {1}", packet.id, packet.clientId);
        }

        public static void DistributePacket(LogInResponse packet)
        {
            Console.WriteLine("LogInRequest: {0}, {1}", packet.id, packet.status);
        }

        public static void DistributePacket(PlayerInfo packet)
        {
            Console.WriteLine("PlayerInfo: {0}, {1}, {2}", packet.id, packet.playerId, packet.playerName);
        }
    }
}
