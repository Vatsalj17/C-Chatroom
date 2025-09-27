# Chat Over TCP

A simple **multi-client chatroom** built in **C** using raw **TCP sockets** and **POSIX threads**.
It allows multiple clients to connect to a server, exchange messages in real time, and see when participants join or leave.

---

## ✨ Features

* Multi-client support (up to `BACKLOG` concurrent clients).
* Real-time messaging with broadcast to all connected clients.
* Usernames are sent once on connection.
* Colored terminal output for join/leave notifications and messages.
* Graceful handling of client disconnections.
* Lightweight: only uses C standard libraries + POSIX sockets and threads.

---

## 📂 Project Structure

```
.
├── Client
│   └── client.c
├── Server
│   ├── connection.c
│   ├── connection.h
│   ├── main.c
│   ├── message.c
│   └── message.h
├── makefile
├── README.md
└── utils
    ├── colors.h
    ├── socketutils.c
    └── socketutils.h
```

---

## ⚙️ How It Works

1. **Server**

   * Listens for incoming connections.
   * Assigns each client a slot in `acceptedSockets[]`.
   * Spawns a thread per client to handle incoming messages.
   * Broadcasts all messages (except back to the sender).

2. **Client**

   * Connects to the server and sends a username.
   * Spawns a thread to continuously listen for incoming messages.
   * Main thread handles user input (`getline`), sending messages until `exit`.

---

## 🚀 Getting Started

### 1. Compile

```bash
make server
make client
```

### 2. Run the Server

```bash
./server
```

### 3. Run Clients (in separate terminals)

```bash
./client
```

Enter your name when prompted, then start chatting.
Type `exit` to leave the chatroom.

---

## 🛠️ Future Improvements

* Add private messaging (`/msg <user> <text>`).
* Support for file transfers.
* Authentication (password-based login).
* Replace thread-per-client model with `select()`/`poll()` for scalability.
* Docker container for easy setup.
