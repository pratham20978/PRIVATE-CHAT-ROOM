#pragma once
#include "Header_Files.h"
#include "CR_Message.h"
#include "CR_TSQueue.h"
#include "CR_Connection.h"

using namespace std;

namespace olc
{
    namespace net
    {   
        template<typename T>
        class Client_Interface{
            private:
            // Queue for incoming message from other user
            TSQueue<Message> m_qMessageIn;

            protected:
            // asio Context handle the data transfer....
            asio::io_context m_context;

            // ...but need a thread of its own to execute its work commands
            thread thrContext;

            // This is the hardware socket that is connected to the server
            asio::ip::tcp::socket m_socket;

            // The client has a single instance of a connection object, which handles data transfer
            unique_ptr<Connection<T>> m_connection;

            Message m_msgIn;


            public:
            Client_Interface():m_socket(m_context){
                // Initilise the socket with the io_contex, so it can do stuff
            }

            virtual ~Client_Interface(){
                Disconnect();
            }

            bool Connect(const string& host,const uint16_t port){
                
                try{
                    // Resolve hostname/ ip-address into tangiable physical address
                    asio::ip::tcp::resolver resolver(m_context);
                    asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(host,to_string(port));

                    // Create Connection
                    m_connection = make_unique<Connection<T>>(
                        m_context,
                        asio::ip::tcp::socket(m_context),
                        m_qMessageIn,
                        port
                    ); // Todof

                    // Start Context thread
                    thrContext = thread([this](){m_context.run();});
                }
                catch(exception& e){
                    cerr<<"Client Exception: "<<e.what()<<endl;
                    return false;
                }
                return true;
            }

            void Disconnect(){
                // If connection exixt, and it's connected then
                if(IsConnected()){
                    // ...disconnect form another user gracefully
                    m_connection->Disconnect();
                }

                // Either way we are also done with the asio context
                m_context.stop();

                // .. and its thread
                if(thrContext.joinable()){
                    thrContext.join();
                }

                m_connection.release();
            }

            bool IsConnected(){
                if(m_connection){
                    return m_connection->IsConnected();
                }else{
                    return false;
                }
            }

            void MessageSend(const Message& msg){
                m_connection->Send(msg);
            }

            Message MessageGive(){
                m_msgIn = m_qMessageIn.get();
                m_qMessageIn.pop();
                return m_msgIn;
            }
        };
    }
}