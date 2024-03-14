#pragma once
#include "Header_Files.h"
#include "CR_TSQueue.h"
#include "CR_Message.h"

using namespace std;

namespace olc
{
    namespace net
    {   
        template<typename T>
        class Connection {
            protected:
                // Socket
                asio::ip::tcp::socket socket;

                // the context for socket
                asio::io_context& m_asioContext;

                TSQueue<T> QMessageOut;

                TSQueue<T> QMessaageIn;

            public:
                Connection(){}
                virtual ~connection(){}

            
            bool ConnectToClient();
            bool Disconnect();
            bool IsConnected() const;

            bool send(const Message& msg);
        }
    }
}
