// SimpleHorse.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WinSock2.h>
#include "mmsystem.h"
#pragma comment(lib,"ws2_32")
#pragma comment(lib,"winmm.lib")
#define  HELPMSG "help - Show Help Menu \r\n" \
	"getsysinfo - Get SysTem Infomation" \
	"open - Open the cdrom \r\n" \
	"swap - swap Mouse Button \r\n"\
	"restore - restore Mouse Button \r\n"\
	"exit - quit bdshell"
bool Dispatch(SOCKET sock,char* szCmd);
int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);
	SOCKET s=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	sockaddr_in sockaddr;
	sockaddr.sin_family=PF_INET;
	sockaddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
	sockaddr.sin_port=htons(827);
	bind(s,(SOCKADDR*)&sockaddr,sizeof(SOCKADDR));
	listen(s,5);
	SOCKADDR clientAddr;
	int nSize=sizeof(SOCKADDR);
	SOCKET clientSock;
	clientSock=accept(s,(SOCKADDR*)&clientAddr,&nSize);
	while(true){
		send(clientSock,"BdShell>",strlen("BdShell>")+sizeof(char),0);
		char buff[MAXBYTE]={0};
		recv(clientSock,buff,MAXBYTE,0);
		printf("%s is got \r\n",buff);
		if (!strcmp(buff,"exit"))
		{
			break;
		}
		
		bool bRet=Dispatch(clientSock,buff);
		if (bRet==false)
		{
			send(clientSock,"Command Unsuccessfully!",strlen("Command Unsuccessfully!")+sizeof(char),0);
		}
	}
	return 0;
}

bool Dispatch(SOCKET sock,char* szCmd)
{

	bool bRet=false;
	if (!strcmp(szCmd,"help"))
	{
		send(sock,HELPMSG,strlen(HELPMSG)+sizeof(char),0);
		bRet=true;
	}
	else if(!strcmp(szCmd,"open"))
	{
		mciSendStringA("open cdaudio",NULL,NULL,NULL);
		bRet=true;
	}
	else if (!strcmp(szCmd,"close"))
	{
		mciSendStringA("set cdaudio door closed",NULL,NULL,NULL);
		bRet=true;
	}
	else if(!strcmp(szCmd,"swap"))
	{

		SwapMouseButton(TRUE);
		bRet=true;
	}
	else if (!strcmp(szCmd,"restore"))
	{
		SwapMouseButton(FALSE);
		bRet=true;
	}
	else
	{

	}
	return bRet;
}