#include "chat_server.h"

int main() {
    ChatServer server;
    int serverSocket = server.start();
    server.startHandlingClients(serverSocket);

    return 0;
}
