#include "chat_server.h"
#include <iostream>
#include <thread>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <string>

#include <unistd.h>
#include <arpa/inet.h>

ChatServer::ChatServer() {}

ChatServer::~ChatServer() {}

int ChatServer::start()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        perror("ERROR: Socket creation failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        perror("ERROR: Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, MAX_CLIENTS) == -1)
    {
        perror("ERROR: Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "-------Chat Server Started-------" << std::endl;
    std::cout << "Chat Server Listening on: IP = 127.0.0.1 , PORT = " << PORT << std::endl;
    return serverSocket;
}

void ChatServer::startHandlingClients(int serverSocket)
{

    while (true)
    {
        sockaddr_in clientAddress;
        socklen_t clientAddrLen = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddrLen);

        if (clientSocket == -1)
        {
            perror("ERROR: Accept failed");
            continue;
        }

        std::cout << "A new Client connection accepted" << std::endl;

        std::thread clientThread(&ChatServer::handleClient, this, clientSocket);

        clientThread.detach();
    }
}

std::string ChatServer::getLocalTime()
{
    std::time_t now = std::time(nullptr);
    std::string localTime = asctime(std::localtime(&now));
    return localTime;
}

std::string ChatServer::generateWelcomeMessage()
{

    std::string localTime = getLocalTime();
    return "Welcome to the chat server!!! Login time: " + localTime + "\n";
}

std::string ChatServer::getRandomString()
{
    std::vector<std::string> randomWords = {"Elon", "Musk", "Bill", "Gates", "Pak", "Server"};
    std::string randomStr = "";

    // Use current time as seed for random generator
    srand(time(0));

    for (int i = 0; i < 2; i++) // run two time
    {
        int randomNum = rand() % randomWords.size();
        randomStr += randomWords[randomNum];
    }

    return randomStr;
}

// Function to remove leading and trailing whitespace (including newline characters)
std::string ChatServer::trimString(const std::string &str)
{
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");

    if (start == std::string::npos || end == std::string::npos)
    {
        // The string is empty or contains only whitespace
        return str;
    }

    return str.substr(start, end - start + 1);
}

std::string ChatServer::generateFunnyResponse(const std::string &clientMessage)
{
    // Add your creativity to generate a funny response
    std::string resposne = getRandomString();
    return "Crazy Robot says: " + trimString(clientMessage) + " " + resposne + "\n";
}

void ChatServer::handleClient(int clientSocket)
{
    std::string welcomeMessage = generateWelcomeMessage();
    send(clientSocket, welcomeMessage.c_str(), welcomeMessage.length(), 0);

    while (true)
    {
        char buffer[MESSAGE_LENGTH + 1] = {0};
        int bytesRead = recv(clientSocket, buffer, MESSAGE_LENGTH, 0);

        if (bytesRead <= 0)
        {
            std::cout << "Client disconnected." << std::endl;
            close(clientSocket);
            return;
        }

        std::string clientMessage(buffer);

        if (trimString(clientMessage) == "quit")
        {
            send(clientSocket, "Goodbye! Closing connection.", 30, 0);
            close(clientSocket);
            return;
        }

        std::string response = generateFunnyResponse(clientMessage);
        send(clientSocket, response.c_str(), response.length(), 0);
    }
}
