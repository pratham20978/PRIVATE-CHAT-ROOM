#include <iostream>
#include "Header_Files.h"
#include "CR_Message.h"
#include "CR_TSQueue.h"
#include "CR_Client.h"
#include <fstream>
//#include <filesystem>

using namespace std;

// Function to save creadentials in file
void Save_Creadentials_To_File(const string& UserName, const string& Password){

    // File to store creadentials
    ofstream file("Creadentials.txt");

    if(file.is_open()){
        file << UserName <<endl;
        file << Password <<endl;
        
        file.close();

        cout<<"Creadentials Succesfully Saved to file."<<endl;
    }else{
        cerr<<"Unable to open file for writing."<<endl;
    }
}

bool Authenticate(const string& username, const string& password){

    ifstream file("Creadentials.txt");
    if(file.is_open()){
        string storedUsername, storedPassword;
        while(file >> storedUsername >> storedPassword){
            if(storedUsername == username && storedPassword == password){
                file.close();
                return true;
            }
        }
        file.close();
    }
    return false;
}

bool Check_If_User_Exists(const string& username, const string& password){
    ifstream file("Creadentials.txt");
    if(file.is_open()){
        string storedUsername, storedPassword;
        while(file >> storedUsername >> storedPassword){
            if(username == storedUsername){
                cout<<"User alreadu exists. Please login."<<endl;
                return true;
            }
        }
        file.close();
    }
    return false;
}

void Register(){
    string username,password;
    cout<<"Enter Username: ";
    cin>>username;
    cout<<"Enter Password: ";
    cin>>password;

    if(!Check_If_User_Exists(username,password)){
        Save_Creadentials_To_File(username,password);
    }
}



int main(){

    // Initilize the Client Interface
    olc::net::Client_Interface<olc::net::Message> client;

    // Connect to Other User
    string UserIP="127.0.0.1";
    uint16_t port = 60000;

    bool loggedIn = false; // Flag to trace login status;

    string UserName;


    // Manage Login Page
    while(!loggedIn){
        int choice;
        cout<<"1. Login"<<endl;
        cout<<"2. Register"<<endl;
        // cout<<"3. Change Username $ Password"<<endl;
        cout<<"Enter you choice: "<<endl;

        cin>>choice;

        switch (choice) {
            case 1: {
                string username, password;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                if (Authenticate(username, password)) {
                    cout << "Login successful!" << endl;
                    loggedIn = true;
                    UserName = username;
                } else {
                    cout << "Invalid username or password. Please try again." << endl;
                }
                break;
            }
            case 2:
                Register();
                break;
            default:
                cout << "Invalid choice." << endl;
        }
    }

    // Proceed with connecting only after succesful login
    if(loggedIn && !client.Connect(UserIP,port)){
        cerr <<"Failed To Connect to Other User. "<<endl;
        return 1;
    }

    // Manage Message Transfer
    // Real-time message interaction
    while(true){
        // Check for user input and send message
        while(!client.IncomingQueueEmpty()){
            // Get and display incoming message
            olc::net::Message incomingMsg = client.MessageGive();
            cout<<"Recived Message from "<< incomingMsg.READ_HDR()<<": "<< incomingMsg.READ_MSG() << endl;

        }

        // Check for user input and send messaage
        string userMessage;
        cout << "Enter Your message (type 'exit' to quit): ";
        cin.ignore(); // Ignore newLine charter left in buffr
        getline(cin, userMessage);

        if(userMessage == "exit"){
            break; // Exit loop if user type 'exit'
        } 

        // Send message to the other user
        olc::net::Message msg(UserName,userMessage);
        client.MessageSend(msg);
    }

    return 0;
}
