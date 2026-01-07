#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    const int LISTEN_PORT = 9090;
    const int BUFFER_SIZE = 1024;

    // MARK: - Create Socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }
    std::cout << "[1] Socket Created (fd=" << sock << ")" << std::endl;

    sockaddr_in my_addr{};
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(LISTEN_PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;       // Receive every network interface

    // MARK: - Binding
    if (bind(sock, reinterpret_cast<sockaddr*>(&my_addr), sizeof(my_addr)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
        close(sock);
        return 1;
    }

    std::cout << "[2] Bound to port " << LISTEN_PORT << std::endl;
    std::cout << "[3] Waiting for data ... (Cmd + C to quit)" << std::endl;

    // MARK: - Receive data
    char buffer[BUFFER_SIZE];
    sockaddr_in sender_addr{};
    socklen_t sender_addr_len = sizeof(sender_addr);

    ssize_t received_bytes = recvfrom(
        sock, buffer, BUFFER_SIZE - 1, 0, reinterpret_cast<sockaddr*>(&sender_addr), &sender_addr_len
    );

    if (received_bytes < 0) {
        std::cerr << "Failed to receive data" << std::endl;
        close(sock);
        return 1;
    }

    buffer[received_bytes] = '\0';

    char sender_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &sender_addr.sin_addr, sender_ip, INET_ADDRSTRLEN);

    std::cout << "[4] Received " << received_bytes << " bytes from " << sender_ip << ":" << ntohs(sender_addr.sin_port) << std::endl;
    std::cout << "[5] Message: \"" << buffer << "\"" << std::endl;

    // MARK: - Close Socket
    close(sock);
    std::cout << "[6] Socket Closed" << std::endl;

    return 0;
}