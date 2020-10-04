#include <iostream>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

void connectSpec();
void sendSpec(string arg);
void disconnectSpec();

int main( int argc, char ** argv )
{
    if( argc < 2)
    {
        cout << "Usage: spec <IP> [-o offset][-cf freq][-s span][-m1 on/off][-m2 on/off]\n"
            << "-o offset: Offset frequency expressed in MHz\n"
            << "cf: Center frequency expressed in MHz\n"
            << "-s span: Span width expressed in MHz\n"
            << "-m1 on/off: Marker 1on or off\n"
            << "-m2 on/off: Marker 2 on or off";
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
    
    while(scanner < argc)
    {
        if( strcmp(argv[scanner], "-o") == 0 )
        {
            string offset(argv[scanner+1]);
            string sendbuf = "sense:freq:offs "+offset+"mhz\n";

            // Send an initial buffer
            iResult = send(ConnectSocket, &sendbuf[0], (int)sendbuf.length(), 0);
            if (iResult == SOCKET_ERROR) {
                printf("send failed: %d\n", WSAGetLastError());
                closesocket(ConnectSocket);
                WSACleanup();
                return 1;
            }

            printf("Offset Sent: %ld\n", iResult);
        }
        if( strcmp(argv[scanner], "-cf") == 0 )
        {
            string center(argv[scanner+1]);
            string sendbuf = "sense:freq:cent "+center+"mhz\n";

            // Send an initial buffer
            iResult = send(ConnectSocket, &sendbuf[0], (int)sendbuf.length(), 0);
            if (iResult == SOCKET_ERROR) {
                printf("send failed: %d\n", WSAGetLastError());
                closesocket(ConnectSocket);
                WSACleanup();
                return 1;
            }

            printf("CF Sent: %ld\n", iResult);
        }
        if( strcmp(argv[scanner], "-s") == 0 )
        {
            string span(argv[scanner+1]);
            string sendbuf = "sense:freq:span "+span+"mhz\n";

            // Send an initial buffer
            iResult = send(ConnectSocket, &sendbuf[0], (int)sendbuf.length(), 0);
            if (iResult == SOCKET_ERROR) {
                printf("send failed: %d\n", WSAGetLastError());
                closesocket(ConnectSocket);
                WSACleanup();
                return 1;
            }

            printf("Span Sent: %ld\n", iResult);
        }
        if( strcmp(argv[scanner], "-m1") == 0 )
        {
            if( strcmp( argv[scanner+1], "on" ) == 0 )
            {
                string span(argv[scanner+1]);
                string sendbuf = ":calc:mark1:mode pos\n";

                // Send an initial buffer
                iResult = send(ConnectSocket, &sendbuf[0], (int)sendbuf.length(), 0);
                if (iResult == SOCKET_ERROR) {
                    printf("send failed: %d\n", WSAGetLastError());
                    closesocket(ConnectSocket);
                    WSACleanup();
                    return 1;
                }

                printf("Mark1 Sent: %ld\n", iResult);
            }
            else
            {
                string span(argv[scanner+1]);
                string sendbuf = ":calc:mark1:mode off\n";

                // Send an initial buffer
                iResult = send(ConnectSocket, &sendbuf[0], (int)sendbuf.length(), 0);
                if (iResult == SOCKET_ERROR) {
                    printf("send failed: %d\n", WSAGetLastError());
                    closesocket(ConnectSocket);
                    WSACleanup();
                    return 1;
                }

                printf("Mark1 Sent: %ld\n", iResult);
            }
        }
        if( strcmp(argv[scanner], "-m2") == 0 )
        {
            if( strcmp( argv[scanner+1], "on" ) == 0 )
            {
                string span(argv[scanner+1]);
                string sendbuf = ":calc:mark2:mode pos\n";

                // Send an initial buffer
                iResult = send(ConnectSocket, &sendbuf[0], (int)sendbuf.length(), 0);
                if (iResult == SOCKET_ERROR) {
                    printf("send failed: %d\n", WSAGetLastError());
                    closesocket(ConnectSocket);
                    WSACleanup();
                    return 1;
                }

                printf("Mark2 Sent: %ld\n", iResult);
            }
            else
            {
                string span(argv[scanner+1]);
                string sendbuf = ":calc:mark2:mode off\n";

                // Send an initial buffer
                iResult = send(ConnectSocket, &sendbuf[0], (int)sendbuf.length(), 0);
                if (iResult == SOCKET_ERROR) {
                    printf("send failed: %d\n", WSAGetLastError());
                    closesocket(ConnectSocket);
                    WSACleanup();
                    return 1;
                }

                printf("Mark2 Sent: %ld\n", iResult);
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
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());
    } while (iResult > 0);
    
   
    return 0;
}

void connectSpec();
void sendSpec(string arg);
void disconnectSpec();