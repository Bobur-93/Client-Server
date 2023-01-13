#include <iostream>
#pragma comment (lib, "ws2_32.lib")
#include <WinSock2.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <string>
#include<Ws2tcpip.h>

#pragma warning(disable: 4996)

using namespace std;

SOCKET Connection;

void ClientHandler()
{
    int msg_size;
    while (true)
    {
        recv(Connection, (char*)&msg_size, sizeof(int), NULL);
        char* msg = new char[msg_size + 1];
        msg[msg_size] = '\0';

        recv(Connection, msg, msg_size, NULL);
        cout << msg << endl;

        delete[] msg;
    }    
}

int main(int argc, char* argv[])
{
    WSADATA wsData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsData) != 0)
    {
        cout << "Start error!";
        exit(1);
        //return 1;
    }
    int status;
    addrinfo hints;
    addrinfo* servinfo;

    hostent* adr_lock;
    char hn[1024];

    gethostname(hn, 1024);

    adr_lock = gethostbyname(hn);

    //cout << adr_lock << '\n';

    char* LocalIp = new char[15];

    
    //адрес сокета
    SOCKADDR_IN addr; //в addr храним интернет адрес
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //храним IP адрес
    addr.sin_port = htons(1111); // 
    addr.sin_family = AF_INET; // какой протокол будем использовать

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(NULL, "3490", &hints, &servinfo);

    if (status !=0)
    {
        cout << "getaddrinfo error:";
        cout << gai_strerror(status) << '\n';
    }
    cout << servinfo->ai_addr << '\n';
    //cout << hints.ai_flags << '\n';

    for (addrinfo* aip = servinfo; aip != NULL; aip=aip->ai_next)
    {
        cout << aip->ai_addr << '\n';
    }

    Connection = socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
    {
        cout << "Error: Failed connection to server.\n";
        return 1;
    }
    cout << "Connected!\n";
    //char msg[256];
    //recv(Connection, msg, sizeof(msg), NULL);
    //cout << msg << endl;

    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

    string msg1;
    while (true)
    {
        getline(cin, msg1);
        int msg_size = msg1.size();
        send(Connection, (char*)&msg_size, sizeof(int), NULL);
        send(Connection, msg1.c_str(), msg_size, NULL);
        Sleep(10);
    }

    
    system("pause");

    delete[] LocalIp;
    
    return 0;
}