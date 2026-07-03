# Redis Clone in C++

A lightweight Redis server implemented from scratch in **C++17**. The goal of this project is to understand how Redis works internally by implementing its networking layer, protocol parsing, command execution, and in-memory data storage without relying on external frameworks.

The project has evolved from a **thread-per-client** architecture to an **event-driven server using Linux epoll (Edge Triggered)**, closely following the design principles used in high-performance network servers.

---

## Features

### Networking
- TCP server using POSIX sockets
- Event-driven architecture using `epoll` (Edge Triggered)
- Non-blocking sockets
- Handles multiple concurrent client connections
- Connection abstraction with buffered I/O

### Protocol
- RESP (Redis Serialization Protocol) parser
- RESP encoder
- Supports both RESP requests (used by `redis-cli`) and inline commands

### Commands Implemented

| Command | Description |
|----------|-------------|
| `PING` | Check server connectivity |
| `SET key value` | Store a key-value pair |
| `GET key` | Retrieve a value |
| `DEL key` | Delete a key |
| `EXISTS key` | Check whether a key exists |

### Storage
- In-memory key-value store
- Thread-safe implementation using `std::shared_mutex`

### Architecture
- Modular project structure
- Request processing pipeline
- Separation of networking, protocol parsing, command execution, and storage

---

# Project Structure

```
Redis-Clone/
│
├── CMakeLists.txt
├── README.md
│
├── src
│   ├── common
│   ├── connection
│   ├── datastore
│   ├── executor
│   ├── network
│   ├── parser
│   ├── protocol
│   ├── request
│   ├── server
│   └── main.cpp
```

---

# Architecture

```
                 Client (redis-cli)

                         │
                         │ TCP
                         ▼

                  RedisServer
                       │
              Event Loop (epoll)
                       │
         ┌─────────────┴─────────────┐
         │                           │
 Accept New Clients         Handle Client Events
                                     │
                              Connection
                                     │
                             RequestProcessor
                                     │
        ┌──────────────┬─────────────┴─────────────┐
        │              │                           │
   RESPParser    CommandParser             CommandExecutor
                                                │
                                           DataStore
                                                │
                                          RESPEncoder
                                                │
                                             Response
```

---

# Requirements

- Linux
- g++ (C++17 or later)
- CMake 3.16+

---

# Clone the Repository

```bash
git clone https://github.com/akarthiksagar/Redis-Clone.git
cd Redis-Clone
```

---

# Build

```bash
mkdir build
cd build

cmake ..
make
```

or if using Ninja

```bash
mkdir build
cd build

cmake -G Ninja ..
ninja
```

---

# Run

```bash
./redis_clone
```

The server listens on:

```
127.0.0.1:6380
```

---

# Using the Server

Connect using the official Redis client:

```bash
redis-cli -p 6380
```

Example session

```text
127.0.0.1:6380> PING
PONG

127.0.0.1:6380> SET name Karthik
OK

127.0.0.1:6380> GET name
"Karthik"

127.0.0.1:6380> EXISTS name
(integer) 1

127.0.0.1:6380> DEL name
(integer) 1
```

---

# Testing

### Basic Commands

```
PING
SET
GET
DEL
EXISTS
```

### Multiple Clients

Open multiple terminals

```bash
redis-cli -p 6380
```

and verify that all clients share the same datastore.

### Raw RESP

You can also connect using

```bash
nc localhost 6380
```

and manually send RESP requests.

---

# Technologies Used

- C++17
- POSIX Sockets
- epoll (Edge Triggered)
- CMake
- STL
- shared_mutex

---
