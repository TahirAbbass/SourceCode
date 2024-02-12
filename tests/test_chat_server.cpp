#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include "../inc/chat_server.h"

// Mock class to simulate a DatabaseManager
class MockDatabaseManager : public DatabaseManager {
public:
    // Mock insertMessage function
    MOCK_METHOD(void, insertMessage, (const std::string&, const std::string&));
    MOCK_METHOD(void, DatabaseManager, (const std::string&));
};

// Test fixture for ChatServer
class ChatServerTest : public ::testing::Test {
protected:
    // Set up before each test
    void SetUp() override {
        // Create a mock database manager
        //mockDBManager = std::make_unique<MockDatabaseManager>("test.db");
        dbManager = new DatabaseManager("testDB.db");
        dbManager->createTable();

        // Create a ChatServer instance with the mock database manager
        chatServer = new ChatServer(dbManager);
    }

    // Tear down after each test
    void TearDown() override {
        //delete chatServer;//chat server will also free dbManager object as it owns it
    }

    //std::unique_ptr<ChatServer> chatServer;
    ChatServer* chatServer;

    //std::unique_ptr<MockDatabaseManager> mockDBManager;
    DatabaseManager* dbManager;
};

// Test fixture for DatabaseManager
class DatabaseManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a DatabaseManager instance
        dbManager = new DatabaseManager("testDB.db");
    }

    void TearDown() override {
        delete dbManager;
    }

    DatabaseManager* dbManager;
};

//Test case for successful server start
TEST_F(ChatServerTest, StartServerSuccess) {
    // Call the start function
    int serverSocket = chatServer->start();

    // Check if the server socket is greater than 0, indicating successful start
    ASSERT_GT(serverSocket, 0);
}

TEST_F(ChatServerTest, GetLocalTime) {
    // Call the getLocalTime function
    std::string localTime = chatServer->getLocalTime();

    // Check if the returned local time is not empty
    ASSERT_FALSE(localTime.empty());
}

TEST_F(ChatServerTest, GenerateWelcomeMessage) {
    // Call the generateWelcomeMessage function
    std::string welcomeMessage = chatServer->generateWelcomeMessage();

    // Check if the generated welcome message contains the expected substring
    ASSERT_NE(welcomeMessage.find("Welcome to the chat server!!! Login time:"), std::string::npos);
}

TEST_F(ChatServerTest, GetRandomString) {
    // Call the getRandomString function
    std::string randomStr = chatServer->getRandomString();

    // Check if the returned random string is not empty
    ASSERT_FALSE(randomStr.empty());
}

TEST_F(ChatServerTest, TrimString) {
    // Call the trimString function with leading and trailing whitespace
    std::string inputString = "   Hello, World!   ";
    std::string trimmedString = chatServer->trimString(inputString);

    // Check if the trimmed string is the expected result
    ASSERT_EQ(trimmedString, "Hello, World!");
}

TEST_F(ChatServerTest, GenerateFunnyResponse) {
    // Call the generateFunnyResponse function with a client message
    std::string clientMessage = "  Tell me a joke!  ";
    std::string funnyResponse = chatServer->generateFunnyResponse(clientMessage);

    // Check if the generated funny response contains the expected substring
    ASSERT_NE(funnyResponse.find("Crazy Robot says: Tell me a joke!"), std::string::npos);
}

// check if the table was created successfully
TEST_F(DatabaseManagerTest, CreateTableSuccess) {
    // Call the createTable function
    dbManager->createTable();

    bool tableExists = dbManager->tableExists("chat_messages");
    ASSERT_EQ(tableExists, true);
}

//check if the message was inserted successfully
TEST_F(DatabaseManagerTest, InsertMessageSuccess)
{
    // Call the insertMessage function
    dbManager->insertMessage("Hello, World!", "2024-02-11 12:34:56");

    bool status = dbManager->doesMessageExist("Hello, World!");

    ASSERT_EQ(status, true);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}