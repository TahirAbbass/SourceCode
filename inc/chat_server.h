#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <string>
#include <vector>
#include <memory>
#include "../inc/database_manager.h"

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

private:
    static constexpr int PORT = 12345;
    static constexpr int MAX_CLIENTS = 100;
    static constexpr int MESSAGE_LENGTH = 256;

    DatabaseManager* pDBManager;



    void handleClient(int clientSocket);
};

#endif // CHAT_SERVER_H
