#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <chrono> // Add chrono for time
#include "../inc/database_manager.h"

using namespace std::chrono;

class ChatServer
{
public:
    ChatServer(DatabaseManager* dbManager);
    ~ChatServer();

    int start();
    void startHandlingClients(int serverSocket);
    std::string getLocalTime();

    std::string generateWelcomeMessage();
    std::string generateFunnyResponse(const std::string &clientMessage);
    std::string getRandomString();
    std::string trimString(const std::string &str);

    // Additional functions for status dashboard
    std::string getUptime();
    int getActiveConnections();
    void printServerStatus();

    // Signal handler for Ctrl+C
    static void handleSignal(int signum);

private:
    static constexpr int PORT = 12345;
    static constexpr int MAX_CLIENTS = 100;
    static constexpr int MESSAGE_LENGTH = 256;

    DatabaseManager* pDBManager;

    //Members used to show server dashboard (uptime + active connections)
    std::chrono::time_point<std::chrono::system_clock> startTime; // To store server start time
    int activeConnections;//number of active connections on server    
    std::thread statusThread;// Thread for printing server status
    
    //static bool stopServer;

    void handleClient(int clientSocket);
};

#endif // CHAT_SERVER_H
