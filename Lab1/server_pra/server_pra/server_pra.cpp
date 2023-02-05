// server_pra.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "data.h"
#pragma comment(lib,"ws2_32.lib")//链接ws2_32.lib库文件到此项目中

using namespace std;

SOCKET sockSer, sockCli;//创建socket
SOCKET sockConn[CLIENTNUM];
SOCKADDR_IN addrSer, addrCli;//保存IP和端口号
int len = sizeof(sockaddr_in);

int conNum = 0;

DWORD WINAPI handlerRequest(LPVOID lparam);
HANDLE hThread, hThread2;
DWORD dwThreadId1, dwThreadId2;

int main()
{
    cout << "SERVER" << endl;

    //加载socket库
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata)!=0)//成功返回0
    {
        cout << "socket初始化失败" << endl;
        return 0;
    }

    //创建一个socket，并将该socket绑定到一个特定的传输层
    sockSer = socket(AF_INET, SOCK_STREAM, 0);//地址类型(ipv4)，服务类型(流式套接字)

    //初始化地址
    addrSer.sin_addr.s_addr = inet_addr("192.168.47.1");
    addrSer.sin_family = AF_INET;
    addrSer.sin_port = htons(6666);

    //绑定bind
    if (bind(sockSer, (sockaddr*)&addrSer, sizeof(sockaddr)) == -1)
        cout << "bind error" << endl;

    cout << "listening" << endl;
    for (int i = 0; i < CLIENTNUM; i++)
    {
        listen(sockSer, 5);//5是内核为此套接口排队的最大连接个数
        sockConn[i] = accept(sockSer, (sockaddr*)&addrCli, &len);
        if (sockConn[i] != INVALID_SOCKET)
        {
            conNum++;
            char buf[12] = "你的ID是：";
            buf[10] = i + 48;//i是int型，buf是char型，i+48再隐式转换为char型
            send(sockConn[i], buf, 50, 0);
        } 
    }
    if (conNum == CLIENTNUM)
    {
        cout << conNum << " clients have connected" << endl;
        while (1)
        {
            for (int i = 0; i < CLIENTNUM; i++)
            {
                hThread = ::CreateThread(NULL, NULL, handlerRequest, LPVOID(i), 0, &dwThreadId1);
                WaitForSingleObject(hThread, 200);
            }
            if (!conNum)
                break;
        }
    }
    closesocket(sockSer);
    WSACleanup();
    return 0;
}


DWORD WINAPI handlerRequest(LPVOID lparam)
{
    SOCKET sockTemp = sockConn[(int)lparam];
    char recvBuf[BUF_SIZE] = {};
    recv(sockTemp, recvBuf, 1024, 0);
    if (recvBuf[0])
    {
        cout << recvBuf << endl;
        MSG_FORM m = str2msg(recvBuf);
        int id = m.recvID - 48;
        if (id != 9)
			send(sockConn[id], recvBuf, 1024, 0);
        else
            for (int i = 0; i < CLIENTNUM; i++)
            {
                send(sockConn[i], recvBuf, 1024, 0);
            }
        if (!strcmp(m.msg, "quit"))
        {
            cout << "client " << (int)lparam << " quit" << endl;
            conNum--;
            if (!conNum)
                return 0;
        }
    }
    return  0;
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
