Notes on How to Run the Multi-threaded Chat Application

Steps to Compile & Run Chat Server:
    *make clean
    *make
    *./build/chat_server


To run Tests:
    *make test

Generate Package for chat server:
    *mkdir build 
    *cd build
    *cmake ..
    *make package

To Run Client use:
nc 127.0.0.1 12345

