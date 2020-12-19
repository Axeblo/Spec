#include <iostream>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;
bool verbose = false;

int sendData(SOCKET socket, string data);
int sendQuery(SOCKET socket, string data);
SOCKET myConnect( char *IP );

int main( int argc, char ** argv )
{
    if( argc < 2)
    {
        cout << "Use: spec [dst_ip] [options]\n"
            << "Options:\n"
            << "-o offset\tOffset frequency expressed in MHz\n"
            << "-f frequency\tCenter frequency expressed in MHz\n"
            << "-s span\t\tSpan width expressed in MHz\n"
            << "-r reference\tReference level in dBm\n"
            << "-a attenuation\tAttenuation in increments of 10 in dB\n"
            << "-d scale/div\tScale/div setting in dB\n"
            << "-b res bw\tResolution bandwidth in kHz\n"
            << "-i video bw\tVideo bandwidth in Hz\n"
            << "-m[1..2] on|off\tMarker on or off. X is integer between 1 and 2. Example -m1 on\n"
            << "-t[1..3] mode\tTrace mode: write|max|min|avg|off. Example -t1 write\n"
            << "-v\t\tVerbose mode\n";
        return 0;
    }

    int scanner = 1;
    while( scanner < argc )
    {
        if( strcmp(argv[scanner], "-v")  == 0 )//Verbose mode
        {
            verbose = true;
            break;
        }
        scanner++;
    }
    
    SOCKET ConnectSocket = myConnect(argv[1]);
    if( ConnectSocket == SOCKET_ERROR ) return 1;

    scanner = 2;
    while(scanner < argc-1)
    {
        if(      strcmp(argv[scanner], "-o")  == 0 )//Offset frequency
        {
            string offset(argv[scanner+1]);
            string data = "sense:freq:offs "+offset+"mhz\n";

            if (offset.compare("?") != 0)
            {
                if (SOCKET_ERROR == sendData(ConnectSocket, data))
                {
                    printf("Socket error, exiting\n");
                    return 0;
                }
            }
            else
            {
                if (SOCKET_ERROR == sendQuery(ConnectSocket, "sense:freq:offs?\n"))
                {
                    printf("Socket error, exiting\n");
                    return 0;
                }
            }
        }
        else if( strcmp(argv[scanner], "-f")  == 0 )//Center frequency
        {
            string f(argv[scanner+1]);
            string data = "sense:freq:cent "+f+"mhz\n";

            if (f.compare("?") != 0)
            {
                if (SOCKET_ERROR == sendData(ConnectSocket, data))
                {
                    printf("Socket error, exiting\n");
                    return 0;
                }
            }
            else
            {
                if (SOCKET_ERROR == sendQuery(ConnectSocket, "sense:freq:cent?\n"))
                {
                    printf("Socket error, exiting\n");
                    return 0;
                }
            }
        }
        else if( strcmp(argv[scanner], "-s")  == 0 )//Span
        {
            string span(argv[scanner+1]);
            string data = "sense:freq:span "+span+"mhz\n";

            if (span.compare("?") != 0)
            {
                if (SOCKET_ERROR == sendData(ConnectSocket, data))
                {
                    printf("Socket error, exiting\n");
                    return 0;
                }
            }
            else
            {
                if (SOCKET_ERROR == sendQuery(ConnectSocket, "sense:freq:span?\n"))
                {
                    printf("Socket error, exiting\n");
                    return 0;
                }
            }
        }
        else if (strcmp(argv[scanner], "-a") == 0)//Span
        {
            string attenuation(argv[scanner + 1]);
            string data = "pow:att " + attenuation + "\n";

            if (attenuation.compare("?") != 0)
            {
                if (SOCKET_ERROR == sendData(ConnectSocket, data))
                {
                    printf("Socket error, exiting\n");
                    return 0;
                }
            }
            else
            {
                if (SOCKET_ERROR == sendQuery(ConnectSocket, "pow:att?\n"))
                {
                    printf("Socket error, exiting\n");
                    return 0;
                }
            }
        }
        else if (strcmp(argv[scanner], "-d") == 0)//scale/div
        {
            string scalediv(argv[scanner + 1]);
            string data = "disp:wind:trac:y:pdiv "+scalediv+"db\n";

            if (scalediv.compare("?") != 0)
            {
                if (SOCKET_ERROR == sendData(ConnectSocket, data))
                {
                    printf("Socket error, exiting\n");
                    return 0;
                }
            }
            else
            {
                if (SOCKET_ERROR == sendQuery(ConnectSocket, "disp:wind:trac:y:pdiv?\n"))
                {
                    printf("Socket error, exiting\n");
                    return 0;
                }
            }
        }
        else if (strcmp(argv[scanner], "-r") == 0)//Reference level
        {
            string ref(argv[scanner + 1]);
            string data = "disp:wind:trac:y:rlev " + ref + "dbm\n";

            if (ref.compare("?") != 0)
            {
                if (SOCKET_ERROR == sendData(ConnectSocket, data))
                {
                    printf("Socket error, exiting\n");
                    return 0;
                }
            }
            else
            {
                if (SOCKET_ERROR == sendQuery(ConnectSocket, "disp:wind:trac:y:rlev?\n"))
                {
                    printf("Socket error, exiting\n");
                    return 0;
                }
            }
        }
        else if (strcmp(argv[scanner], "-b") == 0)//resolution bandwidth
        {
            string rbw(argv[scanner + 1]);
            string data = "band " + rbw + "khz\n";

            if (rbw.compare("?") != 0)
            {
                if (SOCKET_ERROR == sendData(ConnectSocket, data))
                {
                    printf("Socket error, exiting\n");
                    return 0;
                }
            }
            else
            {
                if (SOCKET_ERROR == sendQuery(ConnectSocket, "band?\n"))
                {
                    printf("Socket error, exiting\n");
                    return 0;
                }
            }
        }
        else if (strcmp(argv[scanner], "-i") == 0)//video bandwidth
        {
        string vbw(argv[scanner + 1]);
        string data = "band:vid " + vbw + "hz\n";

        if (vbw.compare("?") != 0)
        {
            if (SOCKET_ERROR == sendData(ConnectSocket, data))
            {
                printf("Socket error, exiting\n");
                return 0;
            }
        }
        else
        {
            if (SOCKET_ERROR == sendQuery(ConnectSocket, "band:vid?\n"))
            {
                printf("Socket error, exiting\n");
                return 0;
            }
        }
        }
        else if( strcmp(argv[scanner], "-m1") == 0 )//Marker 1
        {
            if( strcmp( argv[scanner+1], "on" ) == 0 )
            {
                string span(argv[scanner+1]);
                string data = ":calc:mark1:mode pos\n";

                if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                    return 0;
            }
            else
            {
                string span(argv[scanner+1]);
                string data = ":calc:mark1:mode off\n";

                if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                    return 0;
            }
        }
        else if( strcmp(argv[scanner], "-m2") == 0 )//Marker 2
        {
            if( strcmp( argv[scanner+1], "on" ) == 0 )
            {
                string span(argv[scanner+1]);
                string data = ":calc:mark2:mode pos\n";

                if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                return 0;
            }
            else
            {
                string span(argv[scanner+1]);
                string data = ":calc:mark2:mode off\n";

                if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                return 0;
            }
        }
        else if( strcmp(argv[scanner], "-t1") == 0 )//Trace 1
        {
            if( strcmp( argv[scanner+1], "write" ) == 0 )
            {
                {
                    string data = ":trac1:upd 1\n";
                    if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                    return 0;
                }
                string data = ":trac1:mode write\n";

                if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                return 0;
            }
            else if( strcmp( argv[scanner+1], "max" ) == 0 )
            {
                {
                    string data = ":trac1:upd 1\n";
                    if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                    return 0;
                }
                string data = ":trac1:mode maxh\n";

                if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                    return 0;
            }
            else if( strcmp( argv[scanner+1], "min" ) == 0 )
            {
                {
                    string data = ":trac1:upd 1\n";
                    if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                    return 0;
                }
                string data = ":trac1:mode minh\n";

                if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                    return 0;
            }
            else if( strcmp( argv[scanner+1], "avg" ) == 0 )
            {
                {
                    string data = ":trac1:upd 1\n";
                    if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                        return 0;
                }
                {
                    string data = ":trac1:mode aver\n";
                    if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                        return 0;
                }
            }
            else
            {
                string span(argv[scanner+1]);
                string data = ":trac1:upd 0\n";
                if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                   return 0;
                
                data = ":trac1:disp 0\n";
                if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                    return 0;
            }
        }
        else if( strcmp(argv[scanner], "-t2") == 0 )//Trace 2
        {
            if( strcmp( argv[scanner+1], "write" ) == 0 )
            {
                {
                    string data = ":trac2:upd 1\n";
                    if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                    return 0;
                }
                string data = ":trac2:mode write\n";

                if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                return 0;
            }
            else if( strcmp( argv[scanner+1], "max" ) == 0 )
            {
                {
                    string data = ":trac2:upd 1\n";
                    if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                    return 0;
                }
                string data = ":trac2:mode maxh\n";

                if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                    return 0;
            }
            else if( strcmp( argv[scanner+1], "min" ) == 0 )
            {
                {
                    string data = ":trac2:upd 1\n";
                    if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                    return 0;
                }
                string data = ":trac2:mode minh\n";

                if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                    return 0;
            }
            else if( strcmp( argv[scanner+1], "avg" ) == 0 )
            {
                {
                    string data = ":trac2:upd 1\n";
                    if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                        return 0;
                }
                {
                    string data = ":trac2:mode aver\n";
                    if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                        return 0;
                }
            }
            else
            {
                string span(argv[scanner+1]);
                string data = ":trac2:upd 0\n";
                if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                   return 0;
                
                data = ":trac2:disp 0\n";
                if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                    return 0;
            }
        }
        else if( strcmp(argv[scanner], "-t3") == 0 )//Trace 3
        {
            if( strcmp( argv[scanner+1], "write" ) == 0 )
            {
                {
                    string data = ":trac3:upd 1\n";
                    if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                    return 0;
                }
                string data = ":trac3:mode write\n";

                if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                return 0;
            }
            else if( strcmp( argv[scanner+1], "max" ) == 0 )
            {
                {
                    string data = ":trac3:upd 1\n";
                    if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                    return 0;
                }
                string data = ":trac3:mode maxh\n";

                if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                    return 0;
            }
            else if( strcmp( argv[scanner+1], "min" ) == 0 )
            {
                {
                    string data = ":trac3:upd 1\n";
                    if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                    return 0;
                }
                string data = ":trac3:mode minh\n";

                if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                    return 0;
            }
            else if( strcmp( argv[scanner+1], "avg" ) == 0 )
            {
                {
                    string data = ":trac3:upd 1\n";
                    if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                        return 0;
                }
                {
                    string data = ":trac3:mode aver\n";
                    if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                        return 0;
                }
            }
            else
            {
                string span(argv[scanner+1]);
                string data = ":trac3:upd 0\n";
                if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                   return 0;
                
                data = ":trac3:disp 0\n";
                if( SOCKET_ERROR == sendData(ConnectSocket, data) )
                    return 0;
            }
        }
        scanner++;
    }

    if (shutdown(ConnectSocket, SD_SEND) == SOCKET_ERROR) { // Shutdown socket
            printf("shutdown failed: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;
        }
    printf("Everything went well.\n");
    return 0;
}

int sendData( SOCKET socket, string data )
{
    char recvbuf[1000];
    if(verbose) cout << "Data: " << data;
    // Send an initial buffer
    int iResult = send(socket, &data[0], (int)data.length(), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(socket);
        WSACleanup();
        return SOCKET_ERROR;
    }

    return iResult;
}
int sendQuery(SOCKET socket, string data)
{
    char recvbuf[1000];
    for (int i = 0; i < 1000; ++i)
    {
        recvbuf[i] = '\0';;
    }
    if (verbose) cout << "Data: " << data;
    // Send an initial buffer
    int iResult = send(socket, &data[0], (int)data.length(), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(socket);
        WSACleanup();
        return SOCKET_ERROR;
    }

    iResult = recv(socket, recvbuf, 1000, 0);
    if (iResult > 0)
        printf("Bytes received: %d: %s\n", iResult, recvbuf);
    else if (iResult == 0)
        printf("Connection closed\n");
    else
        printf("recv failed with error: %d\n", WSAGetLastError());


    return iResult;
}
SOCKET myConnect( char *IP )
{
    SOCKET ConnectSocket;
    WSADATA wsaData;
    int iResult;

    if( verbose )
        printf( "IP: %s\n", IP );

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    struct addrinfo *result = NULL,
                *ptr = NULL,
                hints;

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    #define DEFAULT_PORT "5025"

    // Resolve the server address and port
    iResult = getaddrinfo(IP, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return SOCKET_ERROR;
    }

    // Attempt to connect to the first address returned by
    // the call to getaddrinfo
    ptr=result;

    // Create a SOCKET for connecting to server
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
        ptr->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return ConnectSocket;
    }

    // Connect to server.
    iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return ConnectSocket;
    }
    return ConnectSocket;
}