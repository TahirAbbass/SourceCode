#include "chat_server.h"
#include "../inc/database_manager.h"

int main()
{
    DatabaseManager dbManager("chat_database.db");
    dbManager.createTable();

    ChatServer server (&dbManager);
    int serverSocket = server.start();
    server.startDashboard();
    server.startHandlingClients(serverSocket);

    return 0;
}
