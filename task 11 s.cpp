#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

#define PORT 7070
#define MAX_BUFFER 1024

using namespace std;

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr{}, clientAddr{};
    char buffer[MAX_BUFFER];

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cerr << "❌ Error: Failed to create socket\n";
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 1);

    cout << "📡 Server is active on port " << PORT << ". Waiting for client...\n";

    socklen_t addrLen = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
    cout << "✅ Client connected!\n";

    while (true) {
        memset(buffer, 0, MAX_BUFFER);
        read(clientSocket, buffer, MAX_BUFFER);
        cout << "Client says: " << buffer << "\n";

        string response;
        cout << "You: ";
        getline(cin, response);
        send(clientSocket, response.c_str(), response.size(), 0);
    }

    close(serverSocket);
    return 0;
}
