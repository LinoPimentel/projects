using System.Runtime.InteropServices;

namespace KongregateGameServer.Network.PacketDatabase
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    struct KeepAlive
    {
        public byte id;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    struct LogInResponse
    {
        public byte id;
        public byte status;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    struct LogInRequest
    {
        public byte id;
        public int clientId;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    struct PlayerInfo
    {
        public byte id;
        public int playerId;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 24)]
        public string playerName;
    }
}
