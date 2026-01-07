#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    /**
     * Setting Values
     */
    const char* TARGET_IP = "127.0.0.1";
    const int TARGET_PORT = 9090;
    const char* MESSAGE = "Hello from UDP Sender!";

    // MARK: - Create Socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    std::cout << "[1] Socket Created (fd=" << sock << ")" << std::endl;

    // MARK: - Setup Destination Address
    sockaddr_in target_addr{};
    target_addr.sin_family = AF_INET;                           // IPv4
    target_addr.sin_port = htons(TARGET_PORT);                  // Port
    inet_pton(AF_INET, TARGET_IP, &target_addr.sin_addr);       // IP String -> Binary
    std::cout << "[2] Target Address: " << TARGET_IP << ":" << TARGET_PORT << std::endl;

    // MARK: - Data Transfer
    ssize_t sent_bytes = sendto(
        sock,
        MESSAGE,
        strlen(MESSAGE),
        0,
        reinterpret_cast<sockaddr*>(&target_addr),
        sizeof(target_addr)
    );

    if (sent_bytes < 0) {
        std::cerr << "Failed to send data" << std::endl;
        close(sock);
        return 1;
    }

    std::cout << "[3] Sent " << sent_bytes << " bytes: \"" << MESSAGE << "\"" << std::endl;

    close(sock);
    std::cout << "[4] Socket Closed" << std::endl;

    return 0;
}