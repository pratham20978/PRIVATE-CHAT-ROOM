# Private Chat Room

Private Chat Room is a C++ project that implements a secure private chat room network for client-to-client communication. This README provides an overview of the project structure and key components.

## Overview

The Private Chat Room project aims to provide a secure and efficient platform for users to communicate privately over a network. It utilizes modern C++ features and libraries to implement key functionality such as message encryption, message queues, and network connections.

## Components

### CR_Message.h

#### Overview
CR_Message.h defines the Message class, which represents a message in the chat room network. It provides functionality for encryption and decryption using the AES algorithm, as well as methods for reading and manipulating message headers and bodies.

#### Key Features
- Encryption and decryption of messages using AES with CBC mode.
- Generation of random encryption keys and initialization vectors (IVs).
- Storage and manipulation of message headers and bodies.

### CR_TSQueue.h

#### Overview
CR_TSQueue.h defines the TSQueue (Thread-Safe Queue) class, which provides a thread-safe implementation of a queue data structure. It is used in the chat room network for storing incoming and outgoing messages.

#### Key Features
- Thread-safe implementation to ensure safe concurrent access by multiple threads.
- Methods for pushing, popping, and checking the size of the queue.

### CR_Connection.h

#### Overview
CR_Connection.h defines the Connection class, which represents a network connection in the chat room network. It handles asynchronous communication with clients, including connecting to clients, sending and receiving messages, and managing the network socket.

#### Key Features
- Asynchronous connection to clients using Boost.Asio library.
- Sending and receiving messages asynchronously.
- Integration with message queues for efficient message handling.

## Usage

To use the Private Chat Room project, follow these steps:

1. Include the necessary header files (`CR_Message.h`, `CR_TSQueue.h`, `CR_Connection.h`) in your project.
2. Implement the necessary network and user interface components based on your requirements.
3. Use the provided classes (`Message`, `TSQueue`, `Connection`) to handle message encryption, message queues, and network connections in your application.

## Dependencies

- OpenSSL library for AES encryption/decryption.
- Boost.Asio library for asynchronous network communication.

## Contributing

Contributions to the Private Chat Room project are welcome! Please fork the repository, make your changes, and submit a pull request.

## Authors

- [Pratham Verma](https://github.com/pratham20978) - Role or contribution description.


