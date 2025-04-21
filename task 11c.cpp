#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

#define PORT 7070
#define MAX_BUFFER 1024

using namespace std;

int main() {
    int socketFD;
    struct sockaddr_in serverAddr{};
    char buffer[MAX_BUFFER];

    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0) {
        cerr << "❌ Error: Could not create socket\n";
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);  // localhost

    if (connect(socketFD, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "❌ Connection to server failed\n";
        return 1;
    }

    cout << "✅ Connected to server. Start chatting!\n";

    while (true) {
        string message;
        cout << "You: ";
        getline(cin, message);
        send(socketFD, message.c_str(), message.size(), 0);

        memset(buffer, 0, MAX_BUFFER);
        read(socketFD, buffer, MAX_BUFFER);
        cout << "Server: " << buffer << "\n";
    }

    close(socketFD);
    return 0;
}
