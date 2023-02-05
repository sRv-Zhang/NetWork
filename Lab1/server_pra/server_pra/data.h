#pragma once

#include <iostream>
#include<stdlib.h>
#include<stdio.h>
#include<WinSock2.h>
#include<string.h>
# define _CRT_SECURE_NO_WARNINGS
# define _WINSOCK_DEPRECATED_NO_WARNINGS

#define BUF_SIZE 1024
#define CLIENTNUM 2
//报文格式
class MSG_FORM
{
public:
	u_char sendID;
	u_char recvID;
	char msg[BUF_SIZE];
	MSG_FORM() { memset(msg, 0, BUF_SIZE); }
	MSG_FORM(const u_char a, const u_char b, const char c[BUF_SIZE])
	{
		sendID = a;
		recvID = b;
		strcpy(msg, c);
	}
	MSG_FORM(u_char a,u_char b,char c[BUF_SIZE])
	{
		sendID = a;
		recvID = b;
		strcpy(msg, c);
	}

};

MSG_FORM str2msg(char a[])
{
	MSG_FORM msg_;
	msg_.sendID = a[0];
	msg_.recvID = a[1];
	for (int i = 0; a[i + 2]; i++)
	{
		msg_.msg[i] = a[i + 2];
	}
	return msg_;
}

char* msg2str(MSG_FORM a)
{
	char* c = new char[BUF_SIZE + 6];
	c[0] = a.sendID;
	c[1] = a.recvID;
	c[2] = 0;
	strcat(c, a.msg);
	return c;
}