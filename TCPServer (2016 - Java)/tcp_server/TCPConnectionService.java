package com.company.tcp_server;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Created by Dany on 20/10/2016.
 */
public class TCPConnectionService extends Thread
{
    private static final int MAX_CLIENTS = 10000;
    private ServerSocket serverSocket = null;
    private final ExecutorService pool;
    private ClientSession[] clientSessions;
    private int port;

    ConcurrentHashMap<Long,PrintWriter> onlineUserPrintWriters, temporaryUsers;

    public TCPConnectionService(ConcurrentHashMap<Long,PrintWriter> __onlineUserPrintWriters,
                                ConcurrentHashMap<Long,PrintWriter> __temporaryUsers, int tcpPort) throws IOException
    {
        port = tcpPort;
        while(port < port+100)
        {
            try{
                serverSocket = new ServerSocket(port);
                //System.out.println("Successfully connected to port " + port);
                break;
            }
            catch(Exception e)
            {
                //System.out.println("TCP port already in use: " + port);
                port++;
            }
        }

        pool = Executors.newCachedThreadPool();
        clientSessions = new ClientSession[MAX_CLIENTS];
        //rmiServerMethods = TCPServer.getRmiServerMethods();
        onlineUserPrintWriters = __onlineUserPrintWriters;
        temporaryUsers = __temporaryUsers;
        start();
    }

    public void run()
    {
        int sessionId = 0;
        try
        {
            while(true)
            {
                if(sessionId >= MAX_CLIENTS) {
                    //System.out.println("Reached maximum ammount of clients");
                    break;
                }
                //System.out.println("Waiting for client request");
                Socket socket = serverSocket.accept();
                clientSessions[sessionId] = new ClientSession(socket, sessionId);
                //System.out.println("Client request accepted and session created, id :" + (sessionId));

                pool.execute(new ClientHandler(clientSessions[sessionId],onlineUserPrintWriters,temporaryUsers));
                TCPServerLoad.incrementServerConnectionCount();
                sessionId++;
            }
        }catch(IOException e)
        {
            pool.shutdown();
            //System.out.println("IOException 3: " + e);
        }
    }

    public boolean isConnectionAlive()
    {
        return !pool.isTerminated();
    }

    public int getConnectedPort()
    {
        return port;
    }

    public void exitTCPService()
    {
        try
        {
            serverSocket.close();
        }catch(IOException e)
        {
            //System.out.println("Error closing tcp server socket: " + e);
        }
    }
}
