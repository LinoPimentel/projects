package com.company.tcp_server;

import com.company.StaticMethods;
import com.company.StaticMethods.CallingFrom;
import com.company.rmi_server.RMIServerMethods;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.SocketException;
import java.rmi.RemoteException;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Created by Dany on 20/10/2016.
 */
public class ClientHandler implements Runnable
{
    ClientSession client;
    RMIServerMethods rmiServer;
    ConcurrentHashMap<Long,PrintWriter> onlineUserPrintWriters, temporaryUsers;
    PrintWriter writeToClient;

    public ClientHandler(ClientSession __client, ConcurrentHashMap<Long,PrintWriter> __onlineUsers,
                         ConcurrentHashMap<Long,PrintWriter> __temporaryUsers) throws java.io.IOException
    {
        temporaryUsers = __temporaryUsers;
        onlineUserPrintWriters = __onlineUsers;
        client = __client;
        rmiServer = TCPServer.getRmiServerMethods();
    }

    public void run()
    {
        ////System.out.println("thread started");
        try {
            BufferedReader readFromClient = new BufferedReader(new InputStreamReader(client.getClientSocket().getInputStream()));
            writeToClient = new PrintWriter(client.getClientSocket().getOutputStream(), true);

            temporaryUsers.put((long) client.getSessionId(),writeToClient); // only for push load notifications

            ////System.out.println("thread " + Thread.currentThread().getId() +  "dealing with new client");

            while (true)
            {
                ////System.out.println("Waiting for data from client");
                try
                {
                    String data = readFromClient.readLine();
                    System.out.println("Recieved string: " + data);

                    data = parseAndCallRMIMethod(data, client.getSessionId());

                    //System.out.println("Parsed data: "+data);

                    if(data == null)
                    {
                        rmiServer.logout(client.getUserId(),CallingFrom.TCP);
                        quitConnection();
                    }

                    synchronized (writeToClient)
                    {
                        writeToClient.println(data);
                    }
                    System.out.println("Replied: " + data);

                }catch(SocketException e)
                {
                    e.printStackTrace();
                    rmiServer.logout(client.getUserId(),CallingFrom.TCP);
                    quitConnection();
                    return;
                }
            }
        } catch (Exception e)
        {
            ////System.out.println("Exception getting streams: " + e);
            TCPServerLoad.decrementServerConnectionCount();
            return;
        }
    }

    //also sets userID if login and creates the user entry in the onlineUserPrintWriters
    private String parseAndCallRMIMethod(String data, int __temporaryUserId)
    {
        //System.out.println("parseAndCallRMIMethod()");
        Map<String,String> parsedData = StaticMethods.parseDataToMap(data);
        //System.out.println("parsed data to map");
        long time = System.currentTimeMillis();
        while(System.currentTimeMillis() - time < 30000)
        {
            try
            {
                Map<StaticMethods.RmiReply, Object> reply = StaticMethods.callRMIMethod(rmiServer, parsedData, client.getUserId(),time);

                if (reply.containsKey(StaticMethods.RmiReply.USER_ID))
                {
                    int id = (int) reply.get(StaticMethods.RmiReply.USER_ID);
                    if(id != -1)
                    {
                        if(client.getUserId() != -1) {
                            rmiServer.logout(client.getUserId(), CallingFrom.TCP);
                            System.out.println("User logged out: " + client.getUserId());
                        }
                        client.setUserId(id);
                        onlineUserPrintWriters.put((long) client.getUserId(), writeToClient);
                        if (temporaryUsers.containsKey(__temporaryUserId))
                            temporaryUsers.remove(__temporaryUserId);
                    }
                    else
                    {
                        rmiServer.logout(client.getUserId(), CallingFrom.TCP);
                        onlineUserPrintWriters.remove(client.getUserId());
                        client.setUserId(id);
                    }
                }

                if (reply.containsKey(StaticMethods.RmiReply.REPLY_TO_CLIENT))
                    return (String) reply.get(StaticMethods.RmiReply.REPLY_TO_CLIENT);
                else
                    return null;

            } catch (RemoteException e)
            {
                System.out.println("Remote exception calling rmi server trying to reconnect: " + e);
                try{
                    Thread.sleep(100);
                }catch(Exception e2){}
                rmiServer = TCPServer.getRmiServerMethods();
                //e.printStackTrace();
            }
        }
        return null;
    }

    private void quitConnection()
    {
        if(onlineUserPrintWriters.containsKey(client.getUserId()))
            onlineUserPrintWriters.remove(client.getUserId());
        // //System.out.println("returning from thread: " + Thread.currentThread().getId());
        TCPServerLoad.decrementServerConnectionCount();
        return;
    }
}
