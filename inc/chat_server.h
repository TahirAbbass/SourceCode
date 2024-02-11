#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <string>
#include <vector>

class ChatServer
{
public:
    ChatServer();
    ~ChatServer();

    int start();
    void startHandlingClients(int serverSocket);

private:
    static constexpr int PORT = 12345;
    static constexpr int MAX_CLIENTS = 100;
    static constexpr int MESSAGE_LENGTH = 256;

    std::string getLocalTime();

    std::string generateWelcomeMessage();
    std::string generateFunnyResponse(const std::string &clientMessage);
    std::string getRandomString();
    std::string trimString(const std::string &str);

    void handleClient(int clientSocket);
};

#endif // CHAT_SERVER_H
