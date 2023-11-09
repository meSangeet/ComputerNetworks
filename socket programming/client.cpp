#include <iostream>
#include <cstring>
#include <string>
#include <winsock2.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib") // Link with the Winsock library

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed." << endl;
        return 1;
    }

    const char* serverIP = "127.0.0.1";
    int serverPort = 9090;

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Error creating socket." << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);

    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "Error connecting to the server." << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    cout << "Connected to the server." << endl;

    const char* message = "hello, server!";
    if (send(clientSocket, message, strlen(message), 0) == SOCKET_ERROR) {
        cout << "Error sending data to the server." << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    char buffer[1024];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == SOCKET_ERROR) {
        cout << "Error receiving data from the server." << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    buffer[bytesRead] = '\0';
    cout << "Received from server: " << buffer << endl;

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
