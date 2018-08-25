using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KongregateGameServer
{
    class Lobby
    {
        int nextUserId = 1;
        List<User> usersList;
        Queue<int> idRecycle;

        public Lobby()
        {
            usersList = new List<User>(200);
            idRecycle = new Queue<int>();
        }

        public void AddUser(int kong_id)
        {
            int playerId;
            if (idRecycle.Count > 0)
                playerId = idRecycle.Dequeue();
            else
                playerId = nextUserId++;
        }

        public void ChangeUserDisplayName(int player_id, string new_name)
        {
            usersList[player_id].PlayerName = new_name;
        }
    }
}
