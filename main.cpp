#include <iostream>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int sendSCPI( SOCKET socket, string data );

int main( int argc, char ** argv )
{
    if( argc < 2)
    {
        cout << "Usage: spec <IP> [-o offset] [-cf center_freq] [-s span] [-m1|2 on|off] [-t1|2|3 write|max|min|avg|off ]\n"
            << "-o: Offset frequency expressed in MHz\n"
            << "-cf: Center frequency expressed in MHz\n"
            << "-s: Span width expressed in MHz\n"
            << "-m1|2 on|off: Marker 1 or 2 on or off\n"
            << "-t1|2|3 write|max|min|avg|off Trace mode\n";
        return 0;
    }
    int scanner = 2;
    
    cout << "IP " << argv[1] << endl;
    WSADATA wsaData;
    int iResult;

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
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    SOCKET ConnectSocket = INVALID_SOCKET;

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
        return 1;
    }

    // Connect to server.
    iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    // Should really try the next address returned by getaddrinfo
    // if the connect call failed
    // But for this simple example we just free the resources
    // returned by getaddrinfo and print an error message

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    #define DEFAULT_BUFLEN 512

    int recvbuflen = DEFAULT_BUFLEN;
    char recvbuf[DEFAULT_BUFLEN];
    
    while(scanner < argc-1)
    {
        if( strcmp(argv[scanner], "-o") == 0 )
        {
            string offset(argv[scanner+1]);
            string data = "sense:freq:offs "+offset+"mhz\n";

            if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                return 0;
        }
        else if( strcmp(argv[scanner], "-cf") == 0 )
        {
            string center(argv[scanner+1]);
            string data = "sense:freq:cent "+center+"mhz\n";

            if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                return 0;
        }
        else if( strcmp(argv[scanner], "-s") == 0 )
        {
            string span(argv[scanner+1]);
            string data = "sense:freq:span "+span+"mhz\n";

            if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                return 0;
        }
        else if( strcmp(argv[scanner], "-m1") == 0 )
        {
            if( strcmp( argv[scanner+1], "on" ) == 0 )
            {
                string span(argv[scanner+1]);
                string data = ":calc:mark1:mode pos\n";

                if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                    return 0;
            }
            else
            {
                string span(argv[scanner+1]);
                string data = ":calc:mark1:mode off\n";

                if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                    return 0;
            }
        }
        else if( strcmp(argv[scanner], "-m2") == 0 )
        {
            if( strcmp( argv[scanner+1], "on" ) == 0 )
            {
                string span(argv[scanner+1]);
                string data = ":calc:mark2:mode pos\n";

                if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                return 0;
            }
            else
            {
                string span(argv[scanner+1]);
                string data = ":calc:mark2:mode off\n";

                if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                return 0;
            }
        }
        else if( strcmp(argv[scanner], "-t1") == 0 )
        {
            if( strcmp( argv[scanner+1], "write" ) == 0 )
            {
                {
                    string data = ":trac1:upd 1\n";
                    if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                    return 0;
                }
                string data = ":trac1:mode write\n";

                if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                return 0;
            }
            else if( strcmp( argv[scanner+1], "max" ) == 0 )
            {
                {
                    string data = ":trac1:upd 1\n";
                    if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                    return 0;
                }
                string data = ":trac1:mode maxh\n";

                if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                    return 0;
            }
            else if( strcmp( argv[scanner+1], "min" ) == 0 )
            {
                {
                    string data = ":trac1:upd 1\n";
                    if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                    return 0;
                }
                string data = ":trac1:mode minh\n";

                if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                    return 0;
            }
            else if( strcmp( argv[scanner+1], "avg" ) == 0 )
            {
                {
                    string data = ":trac1:upd 1\n";
                    if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                        return 0;
                }
                {
                    string data = ":trac1:mode aver\n";
                    if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                        return 0;
                }
            }
            else
            {
                string span(argv[scanner+1]);
                string data = ":trac1:upd 0\n";
                if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                   return 0;
                
                data = ":trac1:disp 0\n";
                if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                    return 0;
            }
        }
        else if( strcmp(argv[scanner], "-t2") == 0 )
        {
            if( strcmp( argv[scanner+1], "write" ) == 0 )
            {
                {
                    string data = ":trac2:upd 1\n";
                    if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                    return 0;
                }
                string data = ":trac2:mode write\n";

                if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                return 0;
            }
            else if( strcmp( argv[scanner+1], "max" ) == 0 )
            {
                {
                    string data = ":trac2:upd 1\n";
                    if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                    return 0;
                }
                string data = ":trac2:mode maxh\n";

                if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                    return 0;
            }
            else if( strcmp( argv[scanner+1], "min" ) == 0 )
            {
                {
                    string data = ":trac2:upd 1\n";
                    if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                    return 0;
                }
                string data = ":trac2:mode minh\n";

                if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                    return 0;
            }
            else if( strcmp( argv[scanner+1], "avg" ) == 0 )
            {
                {
                    string data = ":trac2:upd 1\n";
                    if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                        return 0;
                }
                {
                    string data = ":trac2:mode aver\n";
                    if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                        return 0;
                }
            }
            else
            {
                string span(argv[scanner+1]);
                string data = ":trac2:upd 0\n";
                if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                   return 0;
                
                data = ":trac2:disp 0\n";
                if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                    return 0;
            }
        }
        else if( strcmp(argv[scanner], "-t3") == 0 )
        {
            if( strcmp( argv[scanner+1], "write" ) == 0 )
            {
                {
                    string data = ":trac3:upd 1\n";
                    if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                    return 0;
                }
                string data = ":trac3:mode write\n";

                if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                return 0;
            }
            else if( strcmp( argv[scanner+1], "max" ) == 0 )
            {
                {
                    string data = ":trac3:upd 1\n";
                    if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                    return 0;
                }
                string data = ":trac3:mode maxh\n";

                if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                    return 0;
            }
            else if( strcmp( argv[scanner+1], "min" ) == 0 )
            {
                {
                    string data = ":trac3:upd 1\n";
                    if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                    return 0;
                }
                string data = ":trac3:mode minh\n";

                if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                    return 0;
            }
            else if( strcmp( argv[scanner+1], "avg" ) == 0 )
            {
                {
                    string data = ":trac3:upd 1\n";
                    if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                        return 0;
                }
                {
                    string data = ":trac3:mode aver\n";
                    if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                        return 0;
                }
            }
            else
            {
                string span(argv[scanner+1]);
                string data = ":trac3:upd 0\n";
                if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                   return 0;
                
                data = ":trac3:disp 0\n";
                if( SOCKET_ERROR == sendSCPI(ConnectSocket, data) )
                    return 0;
            }
        }
        scanner++;
    }

    // shutdown the connection for sending since no more data will be sent
    // the client can still use the ConnectSocket for receiving data
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Receive data until the server closes the connection
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
        {
            printf("Bytes received: %d\n", iResult);
            printf("%s\n", recvbuf);
        }
        else if (iResult == 0)
            ;//printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());
    } while (iResult > 0);
    
   
    return 0;
}

int sendSCPI( SOCKET socket, string data )
{
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