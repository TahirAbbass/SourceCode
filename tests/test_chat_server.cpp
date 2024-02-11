#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include "../inc/chat_server.h"

class ChatServerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialization code specific to each test
        dbManager = std::make_unique<DatabaseManager>("test_database.db");
        server = std::make_unique<ChatServer>(dbManager.get());
    }

    void TearDown() override {
        // Cleanup code specific to each test
    }

    std::unique_ptr<DatabaseManager> dbManager;
    std::unique_ptr<ChatServer> server;
};

TEST_F(ChatServerTest, ExampleTest) {
    // Your test logic goes here
    EXPECT_EQ(1, 1);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}