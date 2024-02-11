#include "chat_server.h"
#include <iostream>
#include <thread>
#include <cstring>
#include <ctime>
#include <cstdlib>

#include <unistd.h>
#include <arpa/inet.h>

ChatServer::ChatServer() {}

ChatServer::~ChatServer() {}

void ChatServer::start() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    while (true) {
        sockaddr_in clientAddress;
        socklen_t clientAddrLen = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddrLen);

        if (clientSocket == -1) {
            perror("Accept failed");
            continue;
        }

        std::thread clientThread(&ChatServer::handleClient, this, clientSocket);
        clientThread.detach();
    }
}

std::string ChatServer::generateWelcomeMessage() {
    std::time_t now = std::time(0);
    std::tm* localTime = std::localtime(&now);
    
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
    
    return "Welcome to the chat server! Login time: " + std::string(buffer);
}

std::string ChatServer::generateFunnyResponse(const std::string& clientMessage) {
    // Add your creativity to generate a funny response
    return "Crazy Robot says: " + clientMessage + " abracadabra";
}

void ChatServer::handleClient(int clientSocket) {
    std::string welcomeMessage = generateWelcomeMessage();
    send(clientSocket, welcomeMessage.c_str(), welcomeMessage.length(), 0);

    while (true) {
        char buffer[MESSAGE_LENGTH + 1] = {0};
        int bytesRead = recv(clientSocket, buffer, MESSAGE_LENGTH, 0);

        if (bytesRead <= 0) {
            std::cout << "Client disconnected." << std::endl;
            close(clientSocket);
            return;
        }

        std::string clientMessage(buffer);

        if (clientMessage == "quit") {
            send(clientSocket, "Goodbye! Closing connection.", 30, 0);
            close(clientSocket);
            return;
        }

        std::vector<std::string> randomWords = {"funny", "exciting", "random", "chat", "server"};
        std::string response = generateFunnyResponse(clientMessage);
        send(clientSocket, response.c_str(), response.length(), 0);
    }
}
