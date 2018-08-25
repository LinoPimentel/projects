using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KongregateGameServer
{
    using Network;
    class User
    {
        ISocketInterface socket;
        string displayName;
        int userId;
        int kongregateUserId;

        public string PlayerName { get { return displayName; } set { displayName = value; } }

        public User(ISocketInterface socket, int user_id)
        {
            userId = user_id;
            this.socket = socket;
        }
    }
}
