#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <vector>
#include <sstream>
#include <iostream>
using namespace std;

int main()
{
    WSADATA wsadata;
    COORD point{ 0,0};// Координата точки
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); // получаем адрес собственного окна
	
	
	SetConsoleCursorPosition(h, point); // установка курсора в указанную позицию!
	SetConsoleTextAttribute(h, 7); // изменение цвета шрифта
    int res = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (res != NO_ERROR)
    {
        cout << "WSAStartup failked with error " << res << endl;
        return 1;
    }

    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET)
    {
        cout << "socket failed with error " << WSAGetLastError() << endl;
        return 2;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;    
    addr.sin_port = htons(23000);   
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    if (bind(udpSocket, (SOCKADDR*)&addr, sizeof(addr)) != NO_ERROR)
    {
        cout << "bind failed with error " << WSAGetLastError() << endl;
        return 3;
    }

    const size_t receiveBufSize = 1024;
    char receiveBuf[receiveBufSize];

    sockaddr_in senderAddr;
    int senderAddrSize = sizeof(senderAddr);

    cout << "Receiving data...." << endl;
    int bytesReceived = recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&senderAddr, &senderAddrSize);

    if (bytesReceived == SOCKET_ERROR)
    {
        cout << "recvfrom failed with error " << WSAGetLastError() << endl;
        return 4;
    }

    receiveBuf[bytesReceived] = '\0';

    //vector<TCHAR> buffer(100);
    //string text(buffer.begin(), buffer.end());

    string color;
    string nick;
    string dataText;
    istringstream stream(receiveBuf);
    string word;
    int i = 1;
    while (stream >> word)
    {
        if (i == 1)
        {
            nick = word;
        }
        if (i == 2)
        {
            color = word;
        }
        if (i > 2)
        {
            dataText += word;
            dataText += " ";
        }
        i++;

    }

    string command = "color " + color;
    int colorText = stoi(color);

    point.Y += 1;
    point.X = 0;
    SetConsoleCursorPosition(h, point); // установка курсора в указанную позицию!
    SetConsoleTextAttribute(h, 7); // изменение цвета шрифта

    point.Y += 1;
    point.X += 0;  // столбцы! 
    SetConsoleCursorPosition(h, point); 
    SetConsoleTextAttribute(h, 7);
    cout << "Received from: ";

    point.Y += 0;
    point.X += 15;  // столбцы! 
    SetConsoleCursorPosition(h, point);
    SetConsoleTextAttribute(h, colorText);
    cout << nick << endl;

    point.Y += 1;
    point.X = 0;  // столбцы! 
    SetConsoleCursorPosition(h, point);
    SetConsoleTextAttribute(h, 7);
    cout << "Data: ";

    point.Y += 0;
    point.X += 6;  // столбцы! 
    SetConsoleCursorPosition(h, point);
    SetConsoleTextAttribute(h, colorText);
    cout << dataText << endl;

    point.Y += 1;
    point.X = 0;  // столбцы! 
    SetConsoleCursorPosition(h, point);
    SetConsoleTextAttribute(h, 7);
    cout << "Color: ";

    point.Y += 0;
    point.X += 7;  // столбцы! 
    SetConsoleCursorPosition(h, point);
    SetConsoleTextAttribute(h, colorText);
    cout << colorText << endl;

    point.Y += 1;
    point.X += 0;  // столбцы! 
    SetConsoleCursorPosition(h, point);
    SetConsoleTextAttribute(h, 7);

    const size_t sendBufSize = 1024;
    char sendBuf[sendBufSize] = "Hello from UDPExample";

    int sendResult = sendto(udpSocket, sendBuf, strlen(sendBuf), 0, (SOCKADDR*)&senderAddr, senderAddrSize);
    if (sendResult == SOCKET_ERROR)
    {
        cout << "sendto failed with error " << WSAGetLastError() << endl;
        return 4;
    }


    closesocket(udpSocket);
    WSACleanup();
}

