#include <iostream>
#include <thread>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream> // Include the necessary header for std::ostringstream
#include <csignal>
#include <unistd.h>
#include <arpa/inet.h>
#include <atomic>
#include "../inc/chat_server.h"

ChatServer::ChatServer(DatabaseManager* dbManager) {
    pDBManager = dbManager;
    activeConnections = 0;
    //stopServer = false;

    // Start the thread for printing server status
    statusThread = std::thread(&ChatServer::printServerStatus, this);
}

ChatServer::~ChatServer() {
    delete pDBManager;
}

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

    // Set up signal handler for Ctrl+C
    signal(SIGINT, ChatServer::handleSignal);

    std::cout << "-------Chat Server Started-------" << std::endl;
    std::cout << "Chat Server Listening on: IP = 127.0.0.1 , PORT = " << PORT << std::endl;
    return serverSocket;
}

void ChatServer::startHandlingClients(int serverSocket)
{
    // Set the server start time
    startTime = std::chrono::system_clock::now();

    //while (!stopServer)
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

        activeConnections++;

        // std::cout << "A new Client connection accepted" << std::endl;

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
            //std::cout << "Client disconnected." << std::endl;
            close(clientSocket);
            activeConnections--;
            return;
        }

        std::string clientMessage(buffer);

        if (trimString(clientMessage) == "quit")
        {
            send(clientSocket, "Goodbye! Closing connection.", 29, 0);
            close(clientSocket);
            activeConnections--;
            return;
        }

        std::string clientMsgTime = getLocalTime();

        //send message back to client from Chat Server
        std::string response = generateFunnyResponse(clientMessage);
        send(clientSocket, response.c_str(), response.length(), 0);

        pDBManager->insertMessage(clientMessage, clientMsgTime);
    }
}

std::string ChatServer::getUptime()
{
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> uptime = now - startTime;

    int secCount = (int)uptime.count();
    int minutes = secCount / 60;

    std::string time = std::to_string(minutes) + " min " + std::to_string(secCount) + " sec";
    return time;
}

int ChatServer::getActiveConnections()
{
    return activeConnections;
}

void ChatServer::printServerStatus()
{
    std::cout << std::endl;
    while (true)
    {
        // Print active connections and uptime every 500 milliseconds
        std::cout << "\rActive Connections: " << getActiveConnections() 
                  << " | Uptime: " <<  getUptime() 
                  << std::flush;  // Flush the output to ensure it's displayed immediately
                  
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    // Print a newline at the end to move to the next line after the loop ends
    std::cout << std::endl;
}

void ChatServer::handleSignal(int signum)
{
    if (signum == SIGINT)
    {
        std::cout << "\n\nReceived Ctrl+C. Shutting down the server..." << std::endl;
        //stopServer = true;
        exit(EXIT_FAILURE);
    }
}