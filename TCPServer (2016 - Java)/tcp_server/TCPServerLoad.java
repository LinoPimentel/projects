package com.company.tcp_server;

import java.util.concurrent.atomic.AtomicInteger;

/**
 * Created by Dany on 21/10/2016.
 */
public class TCPServerLoad
{
    private static AtomicInteger activeConnectionCount = new AtomicInteger(0);

    public static int incrementServerConnectionCount()
    {
        return activeConnectionCount.incrementAndGet();
    }
    public static int decrementServerConnectionCount()
    {
        return activeConnectionCount.decrementAndGet();
    }
    public static int getServerConnectionCount()
    {
        return activeConnectionCount.get();
    }
}
