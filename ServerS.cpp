#include <iostream>
#pragma comment (lib, "ws2_32.lib")
#include <WinSock2.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <string>

#pragma warning(disable: 4996)

using namespace std;

SOCKET Connection[10];
int Counter = 0;

void ClientHandler(int iter)
{
    int msg_size;
    while (true)
    {
        recv(Connection[iter], (char*)&msg_size, sizeof(int), NULL);

        char* msg = new char[msg_size + 1];
        msg[msg_size] = '\0';

        recv(Connection[iter], msg, msg_size, NULL);
        for (int i = 0; i < Counter; i++)
        {
            if (i==iter)
            {
                continue;
            }

            send(Connection[i], (char*)&msg_size, sizeof(int), NULL);
            send(Connection[i], msg, msg_size, NULL);

        }
        delete[] msg;
    }
}

int main(int argc, char * argv[])
{
    WSADATA wsData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsData) != 0)
    {
        cout << "Start error!";
        exit(1);
        //return 1;
    }



    //адрес сокета
    SOCKADDR_IN addr; //в addr храним интернет адрес
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //храним IP адрес
    addr.sin_port = htons(1111); // 
    addr.sin_family = AF_INET; // какой протокол будем использовать

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //Привязываем сокет
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); //Привязываем адрес сокета
    listen(sListen, SOMAXCONN); // слушаем наш сокет sListen

    SOCKET newConnection;    

    for (int i = 0; i < 10; i++)
    {        
        newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);
        if (newConnection == 0)
        {
            cout << "Error #2\n";
        }
        else
        {
            cout << "Client connected!\n";
            string msg = "It`s my first network programm!";
            int msg_size = msg.size();
            send(newConnection, (char*)&msg_size, sizeof(int), NULL);
            send(newConnection, msg.c_str(), msg_size, NULL);

            Connection[i] = newConnection;
            Counter++;

            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
        }
    }
    

    system("pause");


    return 0;
}