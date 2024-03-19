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
        class Connection : public enable_shared_from_this<Connection<T>>{
            protected:
                // Socket
                asio::ip::tcp::socket m_socket;

                // the context for socket
                asio::io_context& m_asioContext;

                TSQueue<T> QMessageOut;

                TSQueue<T>& QMessageIn;

                asio::ip::tcp::acceptor m_asioAcceptor;

                thread m_threadContext;

                Message m_msgTemporaryIn;

                asio::ip::tcp::resolver::results_type m_endpoint;

            // private:
            //     // serialze msg to write 
            //     string serilizeMsgwrite;

            //     // serialze msg to read
            //     string serilizeMsgread;

            public:
                Connection(asio::io_context& asioContext, asio::ip::tcp::socket socket,
                TSQueue<Message>& qIn, uint16_t port, asio::ip::tcp::resolver::results_type endpoint):
                m_asioContext(asioContext), m_socket(move(socket)),QMessageIn(qIn), 
                m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(),port)),
                m_endpoint(endpoint)
                {
                    // Starting to connect to client
                    m_asioContext.run();
                    m_threadContext = thread([this](){
                        cout<<"[SERVER] Started!"<<endl;
                        while(true){
                            try{
                                if(IsConnected()){
                                    WaitForClientConnection();
                                    ConnectToClient(m_endpoint);
                                }
                                this_thread::sleep_for(chrono::seconds(1));
                            }
                            catch(exception& ec){
                            
                                // Something prohibited the other user from listening
                                cerr<<"[SERVER] Exception: "<<ec.what()<<endl;
                                break;
                            }
                            
                        }
                    }); 
                    
                }
                virtual ~Connection(){
                    m_asioContext.stop();
                    if(m_threadContext.joinable()){
                        m_threadContext.join();
                    }
                }


            
            void ConnectToClient(const asio::ip::tcp::resolver::results_type& endpoints){

                asio::async_connect(m_socket,endpoints,
                [this](error_code ec, asio::ip::tcp::endpoint endpoint){
                    if(!ec){
                        ReadMessage();
                    }
                }
                );
            }

            void WaitForClientConnection(){

                m_asioAcceptor.async_accept([this](error_code ec,asio::ip::tcp::socket socket){
                    if(ec){
                        // Error has occured during acceptance
                        cout<<"[SERVER] New Connection Error: "<<ec.message()<<endl;
                    }
                });
            }


            void Disconnect(){
                if(IsConnected()){
                    post(m_asioContext,[this](){m_socket.close();});
                }
            }

            bool IsConnected() const{
                return m_socket.is_open();
            }

            void Send(const Message& msg){
                post(m_asioContext,
                [this,msg](){
                    bool bWriteMessage = QMessageOut.empty();
                    QMessageOut.push(msg);
                    if(!bWriteMessage){
                        WriteMessage();
                    }
                });
            }

            // Read and Write Msg Part
            void ReadMessage(){
                char buffer[1024];
                async_read(m_socket, asio::buffer(buffer,sizeof(buffer)),
                [this,buffer](error_code ec,size_t length){
                    if(!ec){
                        Message recivedMsg;
                        string data(buffer,buffer+length);
                        recivedMsg.DeserilizeMsg(data);
                        m_msgTemporaryIn=recivedMsg;
                        AddToIncomingQueue(recivedMsg);
                    }else{
                        cout<<"["<<m_msgTemporaryIn.getUserID()<<"] Read Msg Fail"<<endl;
                        m_socket.close();
                    }
                }
                );

            }
            
            void WriteMessage(){

                async_write(m_socket, asio::buffer(QMessageOut.get().SerilizeMsg()),
                [this](error_code ec,size_t length){
                    if(!ec){
                        QMessageOut.pop();
                        if(!QMessageOut.empty()){
                            WriteMessage();
                        }
                    }else{
                        cout<<"["<<QMessageOut.get().getUserID()<<"] Write Msg Fail."<<endl;
                        m_socket.close();
                    }
                }
                );

            }

            void AddToIncomingQueue(Message& mmsg){
                QMessageIn.push(mmsg);

                ReadMessage();
            }
        };
    }
}