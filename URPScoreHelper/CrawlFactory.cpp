#include "stdafx.h"
#include "General.h"
#include "CrawlFactory.h"

void InitSocketLibrary()
{
	// ��ʼ�� Windows Socket �⡣
	WSADATA WSAData;
	int m_dwRet = WSAStartup(MAKEWORD(2, 2), &WSAData);
}

bool CrawlRequest(const char *p_rq, char *p_lpvBuffer, int p_iLength, int *p_iTotalRead, bool no_error_page)
{
	// ���� IPv4 TCP �׽��ֶ���
	g_so = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (g_so == INVALID_SOCKET)
	{
		char m_ErrMsg[1024] = { 0 };
		std::cout << "Status: 500 Internal Server Error\r\n";
		sprintf(m_ErrMsg, "<p>�޷����� Socket��</p>\r\n<p>������룺 %d</p>", WSAGetLastError());
		Error(m_ErrMsg);
		return false;
	}

	// ����ѧԺ����ϵͳ Apache ������
	sockaddr_in m_sockaddr;
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_addr.S_un.S_addr = inet_addr(SERVER);
	m_sockaddr.sin_port = htons(atoi(SERVER_PORT));

	int error = -1;
	int len = sizeof(int);
	timeval tm;
	fd_set set;
	unsigned long ul = 1;
	ioctlsocket(g_so, FIONBIO, &ul); //����Ϊ������ģʽ
	bool ret = false;

	int m_iResult = connect(g_so, (sockaddr *)&m_sockaddr, sizeof(m_sockaddr));
	if (m_iResult == SOCKET_ERROR)
	{
		tm.tv_sec = 1;
		tm.tv_usec = 0;
		FD_ZERO(&set);
		FD_SET(g_so, &set);
		if (select(g_so + 1, NULL, &set, NULL, &tm) > 0)
		{
			getsockopt(g_so, SOL_SOCKET, SO_ERROR, (char *)&error, /*(socklen_t *)*/&len);
			if (error == 0)
				ret = true;
			else
				ret = false;
		}
		else
			ret = false;
	}
	else
	{
		ret = true;
	}
	ul = 0;
	ioctlsocket(g_so, FIONBIO, &ul);
	if(!ret)
	{
		int errid = WSAGetLastError();
		closesocket(g_so);
		if (!no_error_page)
		{
			std::cout << "Status: 500 Internal Server Error\r\n";
			char buff[10] = { 0 };
			_itoa(errid, buff, 10);
			std::string err_msg("<p><b>����ѧУ������ʧ�ܣ�</b></p><p>OS����: (");
			err_msg.append(buff);
			err_msg.append(") ");
			char *wsamsg = (char *)malloc(MAX_PATH);
			FormatMessageA(4096, NULL, errid, NULL, wsamsg, MAX_PATH, NULL);
			err_msg.append(wsamsg);
			if (errid == 0)
				err_msg.append("/ ���ӳ�ʱ");
			err_msg.append("</p><p>����ѧУ���������ˣ�������ô�죬��Ҳ�ܾ�����</p>");
			free(wsamsg);
			Error((char *)err_msg.c_str());
		}
		return false;
	}
	// ������ҳ�����൱�ڷ��� http://SERVER_IP/ ��
	m_iResult = send(g_so, p_rq, strlen(p_rq), 0);
	if (m_iResult != strlen(p_rq))
	{
		closesocket(g_so);
		if (!no_error_page)
		{
			std::cout << "Status: 500 Internal Server Error\r\n";
			char m_ErrMsg[1024] = { 0 };
			sprintf(m_ErrMsg, "<p>�޷������ϵͳͶ������</p>\r\n<p>������룺 %d</p>", WSAGetLastError());
			Error(m_ErrMsg);
		}
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
	return true;
}

/*
bool CurlCrawl(const char *url, bool isPOST = false, const char *postdata = "", const char *cookie = "", struct curl_slist *list = NULL)
{
	CURL* curl = curl_easy_init();
	if (curl == NULL)
	{
		return false;
	}

	char* html = (char *)malloc(1024);
	memset(html, 0, 1024);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
	if (isPOST)
	{
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
	}
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, html);
	curl_easy_setopt(curl, CURLOPT_COOKIE, cookie);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_receive);
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "iEdon-URPScoreHelper");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
	CURLcode ret = curl_easy_perform(curl);

	if (ret != CURLE_OK)
	{
		return false;
	}
}
int shift = 0;
size_t curl_receive(void *buffer, size_t size, size_t nmemb, char *html)
{
	int resize = size * nmemb;
	html = (char *)realloc(html, resize + shift + 1);
	memcpy(html + shift, (char *)buffer, resize);
	shift += resize;
	return resize;
}
*/