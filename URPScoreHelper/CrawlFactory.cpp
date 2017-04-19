#include "stdafx.h"
#include "General.h"
#include "CrawlFactory.h"

bool InitSocketLibrary()
{
	// ��ʼ�� Windows Socket �⡣
	WSADATA WSAData;
	int m_dwRet = WSAStartup(MAKEWORD(2, 2), &WSAData);
	if (m_dwRet != 0)
	{
		puts("Status: 500 Internal Server Error");
		Error("<p>�޷���ʼ�� Windows Socket��</p>");
		return false;
	}
	return true;
}

bool CrawlRequest(const char *p_rq, char *p_lpvBuffer, int p_iLength, int *p_iTotalRead)
{
	// ���� IPv4 TCP �׽��ֶ���
	g_so = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (WSAGetLastError() == WSANOTINITIALISED) // û�г�ʼ������ WSACleanup() ��������
	{
		InitSocketLibrary(); // �ٴγ�ʼ��
		g_so = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	if (g_so == INVALID_SOCKET)
	{
		WSACleanup();
		char m_ErrMsg[1024] = { 0 };
		puts("Status: 500 Internal Server Error");
		sprintf(m_ErrMsg, "<p>�޷����� Socket��</p>\r\n<p>������룺 %d</p>", WSAGetLastError());
		Error(m_ErrMsg);
		return false;
	}

	// ����ѧԺ����ϵͳ Apache ������
	sockaddr_in m_sockaddr;
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_addr.S_un.S_addr = inet_addr(SERVER);
	m_sockaddr.sin_port = htons(80);

	int m_iResult = connect(g_so, (sockaddr *)&m_sockaddr, sizeof(m_sockaddr));
	if (m_iResult == SOCKET_ERROR)
	{
		closesocket(g_so);
		WSACleanup();
		puts("Status: 500 Internal Server Error");
		Error("<p>���ӽ���ϵͳ������ʧ�ܣ�</p>");
		return false;
	}

	// ������ҳ�����൱�ڷ��� http://SERVER_IP/ ��
	m_iResult = send(g_so, p_rq, strlen(p_rq), 0);
	if (m_iResult != strlen(p_rq))
	{
		closesocket(g_so);
		WSACleanup();
		puts("Status: 500 Internal Server Error");
		char m_ErrMsg[1024] = { 0 };
		sprintf(m_ErrMsg, "<p>�޷������ϵͳͶ������</p>\r\n<p>������룺 %d</p>", WSAGetLastError());
		Error(m_ErrMsg);
		return false;
	}

	// �������ݣ�д�뻺������
	m_iResult = recv(g_so, p_lpvBuffer, p_iLength, 0); /* �˴����ʵ����������ܷ��������� */
	while (m_iResult > 0)
	{
		*p_iTotalRead += m_iResult;
		m_iResult = recv(g_so, p_lpvBuffer + *p_iTotalRead, p_iLength - *p_iTotalRead, 0); /* �˴����ʵ����������ܷ��������� */
	}
	if (m_iResult != SOCKET_ERROR)
	{
		*p_iTotalRead += m_iResult;
	}
	closesocket(g_so);
	WSACleanup();
	return true;
}
