# UDP Learning for DDS Understanding

ROS2 DDS 시스템 이해를 위한 C++17 UDP 통신 학습 프로젝트

## 목표

UDP 소켓 프로그래밍 학습.

## UDP Unicast

두 장치 간 기본적인 UDP 통신 구현을 통해 소켓 프로그래밍의 핵심 개념을 익힙니다.

### 구현 완료 항목

- [x] **환경 구축**: CMake 기반 크로스 플랫폼 빌드 환경 (macOS / Linux)
- [x] **Basic UDP Sender/Receiver**: 특정 IP:Port로 메시지 송수신
- [x] **Non-blocking Socket**: 데이터 유무와 관계없이 프로그램이 계속 동작

### 프로젝트 구조

```
udp_learning/
├── CMakeLists.txt
├── README.md
├── build/
└── src/
    ├── sender.cpp      # UDP 메시지 송신
    └── receiver.cpp    # UDP 메시지 수신 (Non-blocking)
```

### 빌드 방법

```bash
mkdir build && cd build
cmake ..
make
```

### 실행 방법

**터미널 1 - Receiver 실행:**
```bash
./receiver
```

**터미널 2 - Sender 실행:**
```bash
./sender
```

### 핵심 개념 정리

#### 1. UDP 소켓 통신 흐름

```
[Sender]                              [Receiver]
socket() 생성                          socket() 생성
    │                                      │
    │                                  bind() 포트 등록
    │                                      │
sendto() ─────── UDP Datagram ───────► recvfrom()
    │                                      │
close()                                close()
```

#### 2. 주요 시스템 콜

| 함수 | 역할 |
|------|------|
| `socket()` | 통신용 파일 디스크립터 생성 |
| `bind()` | 소켓을 특정 포트에 바인딩 (Receiver) |
| `sendto()` | 연결 없이 데이터 전송 (UDP 특징) |
| `recvfrom()` | 데이터 수신 + 송신자 정보 획득 |
| `fcntl()` | 소켓 속성 변경 (Non-blocking 설정) |

#### 3. 바이트 순서 변환

네트워크는 Big-endian, 대부분의 PC는 Little-endian을 사용하므로 변환이 필요합니다.

| 함수 | 설명 |
|------|------|
| `htons()` | Host to Network Short (포트 변환) |
| `htonl()` | Host to Network Long |
| `ntohs()` | Network to Host Short |
| `inet_pton()` | IP 문자열 → 바이너리 |
| `inet_ntop()` | 바이너리 → IP 문자열 |

#### 4. Blocking vs Non-blocking

```
[Blocking]
recvfrom() 호출 → 데이터 올 때까지 대기 → 수신 → 다음 코드

[Non-blocking]
recvfrom() 호출 → 데이터 있으면 수신, 없으면 EWOULDBLOCK 반환 → 다음 코드
```

Non-blocking 설정:
```cpp
int flags = fcntl(sock, F_GETFL, 0);
fcntl(sock, F_SETFL, flags | O_NONBLOCK);
```

#### 5. 시그널 처리

Ctrl+C(SIGINT)를 graceful하게 처리하여 소켓을 정상 종료합니다.

```cpp
volatile bool g_running = true;

void signal_handler(int signum) {
    g_running = false;
}

signal(SIGINT, signal_handler);
```

### TODO

- [ ] UDP Multicast: 1:N 통신 (DDS Discovery의 기초)
- [ ] 구조체 직렬화: 단순 문자열이 아닌 복잡한 데이터 전송
- [ ] QoS 개념 도입: 신뢰성, 순서 보장 등

## 개발 환경

- C++17
- CMake 3.16+
- macOS / Linux (Ubuntu)

## 참고 자료

- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [ROS2 DDS Documentation](https://docs.ros.org/en/rolling/Concepts/About-Different-Middleware-Vendors.html)