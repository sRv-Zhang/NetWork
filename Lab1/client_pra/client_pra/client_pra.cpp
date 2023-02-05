
#include "data.h"

#pragma comment(lib,"ws2_32.lib")//链接ws2_32.lib库文件到此项目中

using namespace std;

//创建socket
SOCKET sockSer, sockCli;
SOCKADDR_IN addrSer, addrCli;
int len = sizeof(sockaddr_in);

char ID;
char sendBuf[BUF_SIZE] = {};
char recvBuf[BUF_SIZE] = {};

DWORD WINAPI handlerRequest1(LPVOID lparam);
DWORD WINAPI handlerRequest2(LPVOID lparam);

HANDLE hThread1, hThread2;
DWORD dwThreadId1, dwThreadId2;

//状态码
int cond = 0;

int main()
{
    cout << "CLIENT" << endl;

    //加载socket库
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata)!=0)
    {
        cout << "socket初始化失败" << endl;
        return 0;
    }

    //创建一个socket，并将该socket绑定到一个特定的传输层
    SOCKET sockclient = socket(AF_INET, SOCK_STREAM, 0);

    addrCli.sin_addr.s_addr = inet_addr("192.168.47.1");
    addrCli.sin_family = AF_INET;
    addrCli.sin_port = htons(2333);

    addrSer.sin_addr.s_addr = inet_addr("192.168.47.1");
    addrSer.sin_family = AF_INET;
    addrSer.sin_port = htons(6666);

    cout << "connecting" << endl;
    sockCli = connect(sockclient, (sockaddr*)&addrSer, sizeof(sockaddr));
    
    if(sockCli!=SOCKET_ERROR)
	{
        cout << "connected" << endl;
        while (1)
        {
            char recvBuf[50] = {};
            recv(sockclient, recvBuf, 50, 0);
            if (recvBuf[0])
            {
                cout << recvBuf << endl;
                ID = recvBuf[10] - 48;
                break;
            }
        }
	}
    while (1)
    {
        hThread1 = ::CreateThread(NULL, NULL, handlerRequest1, LPVOID(sockclient), 0, &dwThreadId1);
        hThread2 = ::CreateThread(NULL, NULL, handlerRequest2, LPVOID(sockclient), 0, &dwThreadId2);
        WaitForSingleObject(hThread2, 200);
        WaitForSingleObject(hThread1, 200);
        CloseHandle(hThread2);
        CloseHandle(hThread1);
        if (cond)
            break;
    }
    closesocket(sockclient);
    WSACleanup();
    return 0;
}

DWORD WINAPI handlerRequest1(LPVOID lparam)
{
    memset(sendBuf, 0, BUF_SIZE);
    char buffer[BUF_SIZE] = {};
    SOCKET sockclient = (SOCKET)(LPVOID)lparam;
    cin.getline(buffer, 1024, '\n');
    sendBuf[0] = ID + 48;
    strcat(sendBuf, buffer);
    send(sockclient, sendBuf, 1024, 0);
    MSG_FORM m = str2msg(sendBuf);
    if (!strcmp(m.msg, "quit") || !strcmp(buffer, "quit"))
        cond = 1;
    return 0;
}

DWORD WINAPI handlerRequest2(LPVOID lparam)
{
    memset(recvBuf, 0, BUF_SIZE);
    SOCKET sockclient = (SOCKET)(LPVOID)lparam;
    recv(sockclient, recvBuf, 1024, 0);
    if (recvBuf[0])
    {
        MSG_FORM m = str2msg(recvBuf);
        cout << m.sendID << ": " << m.msg << endl;
    }
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
