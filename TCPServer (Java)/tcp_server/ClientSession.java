package com.company.tcp_server;

import java.net.Socket;

/**
 * Created by Dany on 10/15/2016.
 */
public class ClientSession
{
    private Socket clientSocket;
    private int sessionId, userId;

    public ClientSession(Socket s, int id)
    {
        clientSocket = s;
        sessionId = id;
        userId = -1;
    }

    public Socket getClientSocket() {
        return clientSocket;
    }

    public int getSessionId() {
        return sessionId;
    }

    public int getUserId() { return userId; }

    public void setUserId(int __userId){ userId = __userId; }
}
