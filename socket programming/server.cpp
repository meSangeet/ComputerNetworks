#include<bits/stdc++.h>
#include<winsock2.h>

using namespace std;

#define PORT 9090
#pragma comment(lib, "ws2_32.lib");

void stringToUpper(char* str) {
    while (*str) {
        *str = toupper((*str));
        str++;
    }
}

int main()
{
    WSADATA wsadata;

    if(WSAStartup(MAKEWORD(2,2), &wsadata) != 0)
    {
        cout<<"WSAStartup failed\n";
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(serverSocket == -1)
    {
        cout<<"Error creating socket"<<endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)(&serverAddr), sizeof(serverAddr)) == -1) {
        cout << "Error binding socket." << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cout << "Error listening on socket." << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Server listening on port " << PORT << "..." << endl;

    while (1) {
        sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)(&clientAddr), &clientAddrLen);
        if (clientSocket == -1) {
            cout << "Error accepting client connection." << endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        cout << "Accepted connection from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << endl;

        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR) {
            cout << "Error receiving data from client." << endl;
            closesocket(clientSocket);
            continue;
        }

        buffer[bytesReceived] = '\0';
        stringToUpper(buffer);

        if (send(clientSocket, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
            cout << "Error sending data to client." << endl;
        }

        closesocket(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
