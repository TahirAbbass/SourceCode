#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <string>
#include <vector>

class ChatServer {
public:
    ChatServer();
    ~ChatServer();

    void start();

private:
    static constexpr int PORT = 12345;
    static constexpr int MAX_CLIENTS = 2;
    static constexpr int MESSAGE_LENGTH = 256;

    std::string generateWelcomeMessage();
    std::string generateFunnyResponse(const std::string& clientMessage);
    void handleClient(int clientSocket);
};

#endif  // CHAT_SERVER_H
