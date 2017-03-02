// GuanglingScoreHelper.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "General.h"
#include "StringHelper.h"
#include "CrawlFactory.h"
#include "GuanglingScoreHelper.h"
#include "resource.h"

using namespace std;

// ��ں���
int main()
{
	if (FCGX_Init() == -1)
	{
		printf("Status: 500 Internal Server Error\r\n");
		printf(GLOBAL_HEADER);
		printf("<p>FastCGI �ӿ��쳣���������ã�</p>");
		return -1;
	}

	// ������Դ
	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(IDR_HTML1), MAKEINTRESOURCE(RT_HTML));
	if (NULL == hRsrc)
	{
		cout << "Status: 500 Internal Server Error\r\n"
			<< GLOBAL_HEADER
			<< "<p>FindResource() ʧ�ܣ�</p>";
		return -1;
	}

	// ��ȡ��Դ�Ĵ�С
	DWORD dwSize = SizeofResource(NULL, hRsrc);
	if (0 == dwSize)
	{
		cout << "Status: 500 Internal Server Error\r\n"
			<< GLOBAL_HEADER
			<< "<p>SizeofResource() ʧ�ܣ�</p>";
		return -1;
	}

	// ������Դ
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	if (NULL == hGlobal)
	{
		cout << "Status: 500 Internal Server Error\r\n"
			<< GLOBAL_HEADER
			<< "<p>LoadResource() ʧ�ܣ�</p>";
		return -1;
	}

	// ������Դ
	ERROR_HTML = (char *)LockResource(hGlobal);
	if (NULL == ERROR_HTML)
	{
		cout << "Status: 500 Internal Server Error\r\n"
			<< GLOBAL_HEADER
			<< "<p>LockResource() ʧ�ܣ�</p>";
		return -1;
	}

	long long m_req_seq = 0;
	FCGX_Request m_fcgi_req;
	FCGX_InitRequest(&m_fcgi_req, 0, 0);

	while (FCGX_Accept_r(&m_fcgi_req) >= 0)
	{
		fcgi_streambuf cin_fcgi_streambuf(m_fcgi_req.in);
		fcgi_streambuf cout_fcgi_streambuf(m_fcgi_req.out);
		fcgi_streambuf cerr_fcgi_streambuf(m_fcgi_req.err);

		cin.rdbuf(&cin_fcgi_streambuf);
		cout.rdbuf(&cout_fcgi_streambuf);
		cerr.rdbuf(&cerr_fcgi_streambuf);

		CGI_REQUEST_METHOD = FCGX_GetParam("REQUEST_METHOD", m_fcgi_req.envp); // ���󷽷�
		CGI_CONTENT_LENGTH = FCGX_GetParam("CONTENT_LENGTH", m_fcgi_req.envp); // ���ݳ���
		CGI_SCRIPT_NAME = FCGX_GetParam("SCRIPT_NAME", m_fcgi_req.envp); // �ű�����
		CGI_QUERY_STRING = FCGX_GetParam("QUERY_STRING", m_fcgi_req.envp); // ��ѯ����
		CGI_PATH_TRANSLATED = FCGX_GetParam("PATH_TRANSLATED", m_fcgi_req.envp); // �ű�λ��
		CGI_HTTP_COOKIE = FCGX_GetParam("HTTP_COOKIE", m_fcgi_req.envp); // Cookie
		CGI_SERVER_SOFTWARE = FCGX_GetParam("SERVER_SOFTWARE", m_fcgi_req.envp); // ���������

		// ��ȡ�����û�ʹ�������ǵķ��� :)
		g_fQueryCount = fopen("QueryCount.bin", "r+");
		g_QueryCount = 0;
		if (g_fQueryCount != NULL)
		{
			fscanf(g_fQueryCount, "%ld", &g_QueryCount);
		}
		else
		{
			g_fQueryCount = fopen("QueryCount.bin", "w+");
		}
		if (g_fQueryCount == NULL)
		{
			cout << "Status: 500 Internal Server Error\r\n"
				<< GLOBAL_HEADER
				<< "<p>fopen() ʧ�ܣ�</p>";

			FCGX_Finish_r(&m_fcgi_req);
			ZeroMemory(JSESSIONID, 256);
			continue;
		}
		fseek(g_fQueryCount, 0, SEEK_SET);

		if (CGI_REQUEST_METHOD == NULL || CGI_CONTENT_LENGTH == NULL || CGI_SCRIPT_NAME == NULL || CGI_QUERY_STRING == NULL ||
			CGI_PATH_TRANSLATED == NULL || CGI_HTTP_COOKIE == NULL)
		{
			cout << "Status: 500 Internal Server Error\r\n"
				<< GLOBAL_HEADER
				<< "<p>FastCGI �ӿ��쳣���������á�</p>";
			FCGX_Finish_r(&m_fcgi_req);
			ZeroMemory(JSESSIONID, 256);
			continue;
		}

		// ��ʼ�� Socket �⡣
		if (!InitSocketLibrary())
		{
			FCGX_Finish_r(&m_fcgi_req);
			continue;
		}

		if (strcmp(CGI_REQUEST_METHOD, "GET") == 0) // ����� GET ����
		{
			if (strcmp(CGI_SCRIPT_NAME, "/") == 0 || strcmp(CGI_SCRIPT_NAME, "/index.fcgi") == 0)
			{
				if (strcmp(CGI_QUERY_STRING, "act=logout") == 0)
				{
					student_logout();
					cout << "Status: 302 Found\r\n" << "Location: index.fcgi\r\n" << GLOBAL_HEADER;
					FCGX_Finish_r(&m_fcgi_req);
					ZeroMemory(JSESSIONID, 256);
					continue;
				}

				parse_index();
				FCGX_Finish_r(&m_fcgi_req);
				ZeroMemory(JSESSIONID, 256);
				continue;
			}
			if (strcmp(CGI_SCRIPT_NAME, "/query.fcgi") == 0)
			{
				if (strcmp(CGI_QUERY_STRING, "act=system_registration") == 0)
				{
					system_registration();
					FCGX_Finish_r(&m_fcgi_req);
					ZeroMemory(JSESSIONID, 256);
					continue;
				}
				else
				{
					cout << "Status: 405 Method Not Allowed\r\n";
					Error("<p>��ҳ�治���� GET ����</p>");
					WSACleanup();
					FCGX_Finish_r(&m_fcgi_req);
					ZeroMemory(JSESSIONID, 256);
					continue;
				}
			}
			cout << "Status: 404 No Such CGI Page\r\n";
			Error("<p>�Ҳ�����ҳ�档</p>");
			WSACleanup();
			FCGX_Finish_r(&m_fcgi_req);
			ZeroMemory(JSESSIONID, 256);
			continue;
		}

		if (strcmp(CGI_REQUEST_METHOD, "POST") == 0) // ����� POST ����
		{
			if (strcmp(CGI_SCRIPT_NAME, "/query.fcgi") == 0)
			{
				parse_query(m_fcgi_req);
				FCGX_Finish_r(&m_fcgi_req);
				ZeroMemory(JSESSIONID, 256);
				continue;
			}
		}
	}

	puts("Status: 500 Internal Server Error");
	Error("<p>��������δ��������쳣��</p>");
	WSACleanup();
	return -1;
}

// ������ҳ������ (GET / /index.fcgi)
int parse_index()
{
	int m_iResult = 0;
	bool m_need_update_cookie = false;

	if (strcmp(CGI_HTTP_COOKIE, "") != 0) // ����ͻ����Ѿ��õ� JSESSIONID
	{
		// �����ڴ棬�����ܷ���˷������ݡ�
		char * m_rep_header = (char *)malloc(512);
		ZeroMemory(m_rep_header, 512);
		char m_req_homepage_cookie[2048] = {0};
		sprintf(m_req_homepage_cookie, REQUEST_HOME_PAGE_WITH_COOKIE, CGI_HTTP_COOKIE);
		if (!CrawlRequest(m_req_homepage_cookie, m_rep_header, 512, &m_iResult))
		{
			free(m_rep_header);
			return -1;
		}

		// ����ԭ Cookie �Ƿ���ڡ���Ч�����������Ƿ��������� Cookie����
		char *pStr1 = strstr(m_rep_header, "JSESSIONID=");
		if (pStr1 != NULL)
		{
			char *pStr2 = strstr(pStr1 + 11, ";");
			if (pStr2 == NULL)
			{
				free(m_rep_header);
				WSACleanup();
				cout << "Status: 500 Internal Server Error\r\n";
				Error("<p>�޷���ȡ Servlet Session ID��</p><p>Cookie ��βʧ�ܡ�</p>");
				return -1;
			}
			mid(JSESSIONID, pStr1, pStr2 - pStr1 - 11, 11); // �ɹ������ Session ID��
			m_need_update_cookie = true;
		}
		else // ��� Cookie �����ã��ͻ�ȡ����
		{
			right(JSESSIONID, (char *)CGI_HTTP_COOKIE, strlen(CGI_HTTP_COOKIE) - 11);
		}
		free(m_rep_header);
	}
	else
	{
		// �����ڴ棬�����ܷ���˷������ݡ�
		char * m_rep_header = (char *)malloc(512);
		ZeroMemory(m_rep_header, 512);
		if (!CrawlRequest(REQUEST_HOME_PAGE, m_rep_header, 512, &m_iResult))
		{
			free(m_rep_header);
			return -1;
		}

		// ��ȡ Session ID��
		char *pStr1 = strstr(m_rep_header, "JSESSIONID=");
		if (pStr1 == NULL)
		{
			closesocket(g_so);
			WSACleanup();
			cout << "Status: 500 Internal Server Error\r\n";
			Error("<p>�޷���ȡ Servlet Session ID��</p><p>Cookie ��ͷʧ�ܡ�</p>");
			return -1;
		}
		char *pStr2 = strstr(pStr1 + 11, ";");
		if (pStr2 == NULL)
		{
			closesocket(g_so);
			WSACleanup();
			cout << "Status: 500 Internal Server Error\r\n";
			Error("<p>�޷���ȡ Servlet Session ID��</p><p>Cookie ��βʧ�ܡ�</p>");
			return -1;
		}
		mid(JSESSIONID, pStr1, pStr2 - pStr1 - 11, 11); // �ɹ���� Session ID��
		//printf(JSESSIONID);
		//printf("\n");
		free(m_rep_header);
		m_need_update_cookie = true;
	}

	// ����������ӣ���ȡ��һ������������ڻ�ȡ��֤�롣
	srand((int)time(0));
	int m_rand = rand();
	char Captcha[256] = { 0 };
	sprintf(Captcha, REQUEST_CAPTCHA, m_rand, JSESSIONID);

	// ������֤�����󣬻�ȡ��֤�����ݡ�
	char * m_rep_body = (char *)malloc(8192);
	ZeroMemory(m_rep_body, 8192);
	if (!CrawlRequest(Captcha, m_rep_body, 8192, &m_iResult))
	{
		free(m_rep_body);
		return -1;
	}

	// �ӷ����������л�ȡ��֤��ͼƬ��
	char *pStr1 = strstr(m_rep_body, "\r\n\r\n");
	if (pStr1 == NULL)
	{
		WSACleanup();
		cout << "Status: 500 Internal Server Error\r\n";
		Error("<p>�޷�������֤����ӦЭ�顣</p>");
		return -1;
	}
	pStr1 += 4; // ָ�������λ��ָ�� jpg ��ʼλ�á�
	int m_CaptchaLength = m_iResult - (pStr1 - m_rep_body); // ��֤��ͼƬ�Ĵ�С

	// ����֤��ת��Ϊ base64 ������ DataURL�������ֱ����ʾ�����û��鿴��
	char m_base64[8192] = { 0 };
	base64_encode((const unsigned char *)pStr1, m_base64, m_CaptchaLength);
	char m_DataURL[10240] = "data:image/jpg;base64,";
	strcat(m_DataURL, m_base64);
	free(m_rep_body);

	// ������ҳ���ļ�
	FILE *m_file_homepage = fopen(CGI_PATH_TRANSLATED, "rb");
	if (m_file_homepage == NULL)
	{
		cout << "Status: 500 Internal Server Error\r\n";
		Error("<p>�����Ҳ�����ҳ��ģ�塣</p>");
		return -1;
	}
	fseek(m_file_homepage, 0, SEEK_END); // �Ƶ�β
	int m_file_homepage_length = ftell(m_file_homepage); // ��ȡ�ļ�����
	fseek(m_file_homepage, 0, SEEK_SET); // �����ƻ���
	char *m_lpszHomepage = (char *)malloc(m_file_homepage_length + 1);
	ZeroMemory(m_lpszHomepage, m_file_homepage_length + 1);
	if (fread(m_lpszHomepage, m_file_homepage_length, 1, m_file_homepage) != 1) // ��Ӳ�����ݿ����ڴ�
	{
		cout << "Status: 500 Internal Server Error\r\n";
		Error("<p>�޷���ȡ��ҳģ�����ݡ�</p>");
		fclose(m_file_homepage);
		free(m_lpszHomepage);
		WSACleanup();
		return -1;
	}
	fclose(m_file_homepage); // �ر��ļ�

	 // �����ҳģ��
	int m_iBufferSize = m_file_homepage_length + strlen(m_DataURL) + strlen(__DATE__) + strlen(__TIME__)
						+ strlen(__FILE__) + strlen(CGI_SERVER_SOFTWARE); // ��û���������

	char *m_lpszCompleteHomepage = (char *)malloc(m_iBufferSize);
	ZeroMemory(m_lpszCompleteHomepage, m_iBufferSize);

	sprintf(m_lpszCompleteHomepage, m_lpszHomepage, g_QueryCount, m_DataURL, 
			__FILE__, __DATE__, __TIME__, CGI_SERVER_SOFTWARE);

	free(m_lpszHomepage);

	// �����ҳ
	if (m_need_update_cookie)
		cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\r\n";

	cout << GLOBAL_HEADER;
	cout << m_lpszCompleteHomepage;

	// �ͷ���Դ
	free(m_lpszCompleteHomepage);
	WSACleanup();
	return 0;
}

// �����ѯҳ������ (POST /query.fcgi)
int parse_query(FCGX_Request p_req)
{
	// ��ȡ POST ���ݡ�
	int m_post_length = atoi(CGI_CONTENT_LENGTH);
	if (m_post_length <= 0)
	{
		cout << "Status: 500 Internal Server Error\r\n";
		Error("<p>��������POST ���ݳ����쳣��</p>");
		WSACleanup();
		return -1;
	}
	char *m_post_data = (char *)malloc(m_post_length + 2);	// TORESEARCH
	if (FCGX_GetStr(m_post_data, m_post_length, p_req.in) == NULL)
	{
		cout << "Status: 500 Internal Server Error\r\n";
		Error("<p>��������POST ������ȡʧ�ܡ�</p>");
		WSACleanup();
		return -1;
	}

	// ��ȡѧ��
	char *pStr1 = strstr(m_post_data, "xh=");
	if (pStr1 == NULL)
	{
		WSACleanup();
		free(m_post_data);
		cout << "Status: 500 Internal Server Error\r\n";
		Error("<p>�޷���ȡѧ����Ϣ��</p>");
		return -1;
	}
	char *pStr2 = strstr(pStr1 + 3, "&");
	char m_xuehao[128] = { 0 };
	mid(m_xuehao, pStr1, pStr2 - pStr1 - 3, 3);
	pStr1 = NULL;
	pStr2 = NULL;

	// ��ȡ����
	pStr1 = strstr(m_post_data, "mm=");
	if (pStr1 == NULL)
	{
		WSACleanup();
		free(m_post_data);
		cout << "Status: 500 Internal Server Error\r\n";
		Error("<p>�޷���ȡ������Ϣ��</p>");
		return -1;
	}
	pStr2 = strstr(pStr1 + 3, "&");
	char m_password[128] = { 0 };
	mid(m_password, pStr1, pStr2 - pStr1 - 3, 3);
	pStr1 = NULL;
	pStr2 = NULL;

	// ��ȡ��֤��
	pStr1 = strstr(m_post_data, "yzm=");
	if (pStr1 == NULL)
	{
		WSACleanup();
		free(m_post_data);
		cout << "Status: 500 Internal Server Error\r\n";
		Error("<p>�޷���ȡ��֤����Ϣ��</p>");
		return -1;
	}
	char m_captcha[128] = { 0 };
	right(m_captcha, pStr1 + 4, 4);

	if (!student_login(m_xuehao, m_password, m_captcha))
	{
		// ������Դ��������ѧ����¼���������ˡ�
		free(m_post_data);
		return -1;
	}

	free(m_post_data);

	// ��ʼ��֡�
	int m_iResult = 0;
	char QUERY_SCORE[512] = { 0 };
	char *m_rep_body = (char *)malloc(81920);
	sprintf( QUERY_SCORE, REQUEST_QUERY_SCORE, CGI_HTTP_COOKIE );
	if (!CrawlRequest(QUERY_SCORE, m_rep_body, 81920, &m_iResult))
	{
		student_logout();
		free(m_rep_body);
		WSACleanup();
		return -1;
	}
	char *m_result = strstr(m_rep_body, "\r\n\r\n");
	if (m_result == NULL)
	{
		student_logout();
		WSACleanup();
		free(m_post_data);
		free(m_rep_body);
		cout << "Status: 500 Internal Server Error\r\n";
		Error("<p>�ӷ�������ȡ����ʧ�ܡ�</p>");
		return -1;
	}

	// �Ż����ܽ������ʾ��ѯҳ��
	parse_friendly_score(m_result);
	free(m_rep_body);

	// ������ϡ�
	WSACleanup();
	return 0;
}

// �������ҳ��
void parse_friendly_score(char *p_lpszScore)
{
	char m_Student[64] = { 0 };
	get_student_name(m_Student);

	// �������ҳ���ļ�
	FILE *m_file_query = fopen(CGI_PATH_TRANSLATED, "rb");
	if (m_file_query == NULL)
	{
		student_logout();
		Error("<p>�����Ҳ�������ҳ��ģ�塣</p>");
		return;
	}
	fseek(m_file_query, 0, SEEK_END); // �Ƶ�β
	int m_file_query_length = ftell(m_file_query); // ��ȡ�ļ�����
	fseek(m_file_query, 0, SEEK_SET); // �����ƻ���
	char *m_lpszQuery = (char *)malloc(m_file_query_length + 1);
	ZeroMemory(m_lpszQuery, m_file_query_length + 1);
	if (fread(m_lpszQuery, m_file_query_length, 1, m_file_query) != 1) // ��Ӳ�����ݿ����ڴ�
	{
		student_logout();
		Error("<p>�޷���ȡ����ҳģ�����ݡ�</p>");
		fclose(m_file_query);
		free(m_lpszQuery);
		return;
	}
	fclose(m_file_query); // �ر��ļ�

	char *m_query_not_reg = strstr(p_lpszScore, "û��ע��");
	if (m_query_not_reg != NULL)
	{
		free(m_lpszQuery);
		cout << "Status: 403 Forbidden\r\n";

		char *m_original_str = "<p><b>�װ���%s��ϵͳ��˵�㱾ѧ�ڻ�û�е���ע�� 0.0</b></p><p>�ҿ���ʩչ������\
һ�������ڽ���ϵͳע��Ŷ~</p><p>--&gt; �㰴�·���ť��ֱ���ֽ��� :P &lt;--</p>\
<div class=\"col-100\"><a href=\"query.fcgi?act=system_registration\" class=\"button button-big but\
ton-fill button-success\">һ��ע��</a></div>";

		char m_output_str[1024] = { 0 };
		sprintf(m_output_str, m_original_str, m_Student);
		Error(m_output_str);
		return;
	}

	// ��λ����һ��ɼ�
	char *pStr1 = strstr(p_lpszScore,"<tr class=\"odd\" onMouseOut=\"this.className='even';\" onMouseOver=\"this.className='evenfocus';\">");
	if (pStr1 == NULL)
	{
		student_logout();
		free(m_lpszQuery);
		cout << GLOBAL_HEADER;
		cout << p_lpszScore;
		Error("<p>�����ҳ�������� 0.0���Ժ������԰�~</p>");
		return;
	}

	bool m_success = false;
	char m_Output[81920] = { 0 };
	char *pStr2 = NULL;
	char *pStr3 = NULL;

	// ѭ����ȡÿһ��ɼ���Ϣ

	double m_Total_xuefen = 0.000000;
	double m_Total_pointsxxuefen = 0.000000;

	while (pStr1 != NULL) 
	{
		pStr2 = pStr1;
		for (int i = 0; i < 3; i++)
		{
			if (pStr2 == NULL) break;
			pStr2 = strstr(pStr2 + 19, "<td align=\"center\">");
		}
		if (pStr2 == NULL) break;
		pStr3 = strstr(pStr2, "</td>");
		if (pStr3 == NULL) break;
		char m_subName[512] = { 0 };
		mid(m_subName, pStr2, pStr3 - pStr2 - 19, 19);

		pStr2 = pStr3;
		pStr2 = strstr(pStr2 + 19, "<td align=\"center\">");
		if (pStr2 == NULL) break;
		pStr3 = strstr(pStr2, "</td>");
		if (pStr3 == NULL) break;
		char m_subXuefen[128] = { 0 };
		mid(m_subXuefen, pStr2, pStr3 - pStr2 - 19, 19);
		if (atof(m_subXuefen) == 0) sprintf(m_subXuefen, "��������");

		pStr2 = pStr3;
		pStr2 = strstr(pStr2 + 19, "<td align=\"center\">");
		if (pStr2 == NULL) break;
		pStr3 = strstr(pStr2, "</td>");
		if (pStr3 == NULL) break;
		char m_subzuigaofen[128] = { 0 };
		mid(m_subzuigaofen, pStr2, pStr3 - pStr2 - 19, 19);
		if (atof(m_subzuigaofen) == 0) sprintf(m_subzuigaofen, "��������");

		pStr2 = strstr(pStr3, "<td align=\"center\">");
		if (pStr2 == NULL) break;
		pStr3 = strstr(pStr2, "</td>");
		if (pStr3 == NULL) break;
		char m_subzuidifen[128] = { 0 };
		mid(m_subzuidifen, pStr2, pStr3 - pStr2 - 19, 19);
		if (atof(m_subzuidifen) == 0) sprintf(m_subzuidifen, "��������");

		pStr2 = strstr(pStr3, "<td align=\"center\">");
		if (pStr2 == NULL) break;
		pStr3 = strstr(pStr2, "</td>");
		if (pStr3 == NULL) break;
		char m_subjunfen[128] = { 0 };
		mid(m_subjunfen, pStr2, pStr3 - pStr2 - 19, 19);
		if (atof(m_subjunfen) == 0) sprintf(m_subjunfen, "��������");

		pStr2 = strstr(pStr3, "<td align=\"center\">");
		if (pStr2 == NULL) break;
		pStr3 = strstr(pStr2, "</td>");
		if (pStr3 == NULL) break;
		char m_subchengji[256] = { 0 };
		mid(m_subchengji, pStr2, pStr3 - pStr2 - 19, 19);
		//if (atoi(m_subchengji) == 0) strcpy(m_subchengji, "��������");
		if (atof(m_subchengji) < 60) 
		{

			char m_completecj[256] = "<b style=\"color:#f6383a\">";
			strcat(m_completecj, m_subchengji);
			strcat(m_completecj, "</b>");
			ZeroMemory(m_subchengji, 256);
			strcpy(m_subchengji, m_completecj);
		}

		pStr2 = strstr(pStr3, "<td align=\"center\">");
		if (pStr2 == NULL) break;
		pStr3 = strstr(pStr2, "</td>");
		if (pStr3 == NULL) break;
		char m_submingci[128] = { 0 };
		mid(m_submingci, pStr2, pStr3 - pStr2 - 19, 19);
		if (atof(m_submingci) == 0) sprintf(m_submingci, "��������");

		char m_StrTmp[10240] = { 0 };
		sprintf(m_StrTmp, SCORE_TEMPLATE, m_subName, m_subchengji, m_subjunfen, m_subzuigaofen, m_subzuidifen,
				m_submingci, m_subXuefen);
		strcat(m_Output, m_StrTmp);

		// ������xѧ�֣�ȫ��������/��ѧ�� = ��Ȩ�֣��ų������Ϳγ����
		if (strstr(m_subName, "����") == NULL)
		{
			float m_xuefen = atof(m_subXuefen);
			float m_chengji = atof(m_subchengji);
			if (m_xuefen != 0 || m_chengji !=0) // �ų��γ���ƣ���ͳ����ѧ��
			{
				m_Total_xuefen += m_xuefen;
			}
			if (m_chengji != 0)
			{
				double m_pointsxxuefen = m_xuefen * m_chengji;
				if (m_pointsxxuefen != 0)
				{
					m_Total_pointsxxuefen += m_pointsxxuefen;
				}
			}
		}
		m_success = true; // �鵽һ����һ��
		pStr1 = strstr(pStr3, "<tr class=\"odd\" onMouseOut=\"this.className='even';\" onMouseOver=\"this.className='evenfocus';\">");

	}

	// ���緢���˴���
	if (!m_success) 
	{
		student_logout();
		free(m_lpszQuery);
		Error("<p>�ڸ������������������</p>");
		return;
	}

	cout << GLOBAL_HEADER;

	// ��䷵��ҳ��
	if (m_Total_pointsxxuefen != 0 || m_Total_xuefen != 0)
	{
		char m_jiaquanfen[81920] = { 0 };
		sprintf(m_jiaquanfen, "<div id=\"i_total\"><p>��ѧ��ѧҵ�ۺϷ֣��������ݣ������ο�����</p><center>%.1f</center></div>", 
				m_Total_pointsxxuefen / m_Total_xuefen);
		strcat(m_jiaquanfen, m_Output);
		ZeroMemory(m_Output, 81920);
		strcpy(m_Output, m_jiaquanfen);
	}
	char m_lpszCompleteQuery[81920] = { 0 };
	sprintf(m_lpszCompleteQuery, m_lpszQuery, m_Student, m_Student, m_Output);
	
	cout << m_lpszCompleteQuery;

	free(m_lpszQuery);
	fprintf(g_fQueryCount, "%ld", ++g_QueryCount);
	fclose(g_fQueryCount);

	// ��ȫ�ǳ�����ϵͳ��
	student_logout();

}

// ��ȡѧ������
void get_student_name(char *p_lpszBuffer)
{
	if (strcmp(CGI_HTTP_COOKIE, "") == 0)
	{
		return;
	}

	int m_iResult = 0;
	char * m_rep_header = (char *)malloc(10240);
	ZeroMemory(m_rep_header, 10240);
	char GET_TOP[1024] = { 0 };
	sprintf(GET_TOP, REQUEST_GET_REGISTER_INTERFACE, CGI_HTTP_COOKIE);
	if (!CrawlRequest(GET_TOP, m_rep_header, 10240, &m_iResult))
	{
		free(m_rep_header);
		return;
	}

	// ��ȡѧ������
	char *pStr1 = strstr(m_rep_header, "<td class=\"fieldName\">����:&nbsp;</td>");
	if (pStr1 == NULL)
	{
		free(m_rep_header);
		return;
	}
	pStr1 = strstr(pStr1 + 39, "<td>");
	if (pStr1 == NULL)
	{
		free(m_rep_header);
		return;
	}
	char *pStr2 = strstr(pStr1 + 5,"</td>");
	if (pStr2 == NULL)
	{
		free(m_rep_header);
		return;
	}
	mid(p_lpszBuffer, pStr1, pStr2 - pStr1 - 4, 4);
	free(m_rep_header);
}

// ����ϵͳ����ע�� (GET /query.fcgi?act=system_registration)
int system_registration()
{
	if (strcmp(CGI_HTTP_COOKIE, "") == 0)
	{
		cout << "Status: 302 Found\r\n" << "Location: index.fcgi\r\n" << GLOBAL_HEADER;
		cout << GLOBAL_HEADER;
		return -1;
	}

	int m_iResult = 0;

	// �������󣬻�ȡ����ע����Ϣ��
	char * m_rep_header = (char *)malloc(10240);
	ZeroMemory(m_rep_header, 10240);
	char m_req[1024] = { 0 };
	sprintf(m_req, REQUEST_GET_REGISTER_INTERFACE, CGI_HTTP_COOKIE);
	if (!CrawlRequest(m_req, m_rep_header, 10240, &m_iResult))
	{
		free(m_rep_header);
		student_logout();
		WSACleanup();
		return -1;
	}

	// ������Ҫ����ע���ѧ����Ϣ��
	char *pStr1 = strstr(m_rep_header, "selected>");
	if (pStr1 == NULL)
	{
		free(m_rep_header);
		Error("<p>���ݴ��󡣲�����˼���Զ�ע��ʧ�ܣ��������ȥ����ϵͳ������~ (1)</p>");
		student_logout();
		WSACleanup();
		return -1;
	}
	pStr1 -= 70;
	char *pStr2 = strstr(pStr1, "<option value=\"");
	if (pStr2 == NULL)
	{
		free(m_rep_header);
		Error("<p>���ݴ��󡣲�����˼���Զ�ע��ʧ�ܣ��������ȥ����ϵͳ������~ (2)</p>");
		student_logout();
		WSACleanup();
		return -1;
	}
	pStr1 = pStr2;
	pStr2 = strstr(pStr1 + 16, "\"");
	if (pStr2 == NULL)
	{
		free(m_rep_header);
		Error("<p>���ݴ��󡣲�����˼���Զ�ע��ʧ�ܣ��������ȥ����ϵͳ������~ (3)</p>");
		student_logout();
		WSACleanup();
		return -1;
	}

	char m_regval[4096] = { 0 };
	mid(m_regval, pStr1, pStr2 - pStr1 - 15, 15);
	free(m_rep_header);

	// ������ע����Ϣ
	char m_post_reg_info[4096] = "zxjxjhh=";
	strcat(m_post_reg_info, m_regval);
	int m_post_reg_info_length = strlen(m_post_reg_info);

	// ���ע������
	char m_post_req[8192] = { 0 };
	sprintf(m_post_req, REQUEST_POST_REGISTER_INTERFACE, m_regval, CGI_HTTP_COOKIE, m_post_reg_info_length, 
			m_post_reg_info);

	// ��ʼ����ע��
	m_rep_header = (char *)malloc(10240);
	ZeroMemory(m_rep_header, 10240);
	if (!CrawlRequest(m_post_req, m_rep_header, 10240, &m_iResult))
	{
		student_logout();
		free(m_rep_header);
		WSACleanup();
		return -1;
	}

	// ����Ƿ�ע��ɹ���
	pStr1 = strstr(m_rep_header, "ע��ɹ�");
	if (pStr1 == NULL)
	{
		free(m_rep_header);
		student_logout();
		WSACleanup();
		Error("<p>������˼���Զ�ע��ʧ�ܣ��������ȥ����ϵͳ������~ (4)</p>");
		return -1;
	}

	// ע��ɹ��ˡ�
	free(m_rep_header);

	// Ϊ�յ���ע���ͬѧ��ѯ�ɼ�
	char QUERY_SCORE[512] = { 0 };
	m_rep_header = (char *)malloc(81920);
	sprintf(QUERY_SCORE, REQUEST_QUERY_SCORE, CGI_HTTP_COOKIE);
	if (!CrawlRequest(QUERY_SCORE, m_rep_header, 81920, &m_iResult))
	{
		free(m_rep_header);
		WSACleanup();
		return -1;
	}
	char *m_result = strstr(m_rep_header, "\r\n\r\n");
	if (m_result == NULL)
	{
		student_logout();
		free(m_rep_header);
		cout << "Status: 500 Internal Server Error\r\n";
		Error("<p>�ӷ�������ȡ����ʧ�ܡ�</p>");
		WSACleanup();
		return -1;
	}

	// �Ż����ܽ������ʾ��ѯҳ��
	parse_friendly_score(m_result);
	free(m_rep_header);
	
	// ����~
	WSACleanup();
	return 0;
}

// ��¼ѧ��
bool student_login(char *p_xuehao, char *p_password, char *p_captcha)
{
	// ���͵�½����
	int m_iResult = 0;
	char * m_rep_body = (char *)malloc(40960);
	ZeroMemory(m_rep_body, 40960);
	char POST_LOGIN[1024] = { 0 };
	char *m_origin = "zjh1=&tips=&lx=&evalue=&eflag=&fs=&dzslh=&zjh=%s&mm=%s&v_yzm=%s";
	char m_padding[1024] = { 0 };
	sprintf(m_padding, m_origin, p_xuehao, p_password, p_captcha);
	int m_ContentLength = strlen(m_padding); // TODO: ���ﲻ�ü�Ī������Ľ�������
	sprintf(POST_LOGIN, REQUEST_LOGIN, m_ContentLength, CGI_HTTP_COOKIE, p_xuehao, p_password, p_captcha);
	if (!CrawlRequest(POST_LOGIN, m_rep_body, 40960, &m_iResult))
	{
		free(m_rep_body);
		return false;
	}

	// ��ȡ��¼�����
	char *m_result = strstr(m_rep_body, "\r\n\r\n");
	if (m_result == NULL)
	{
		WSACleanup();
		free(m_rep_body);
		cout << "Status: 500 Internal Server Error\r\n";
		Error("<p>�ӷ�������ȡ��¼���ʧ�ܡ�</p>");
		return false;
	}

	// �����¼�����
	char *m_login_not_auth = strstr(m_result, "֤����");
	if (m_login_not_auth != NULL)
	{
		WSACleanup();
		free(m_rep_body);
		cout << "Status: 403 Forbidden\r\n";
		Error("<p>ѧ�Ż����벻�԰������С� TAT��</p><p>Ĭ�������Ǿ���ѧ��Ŷ��������޸Ĺ����룬�㶮��~</p>");
		return false;
	}
	m_login_not_auth = strstr(m_result, "��֤��");
	if (m_login_not_auth != NULL)
	{
		WSACleanup();
		free(m_rep_body);
		cout << "Status: 403 Forbidden\r\n";
		Error("<p>��֤�벻�԰������С� TAT��</p>");
		return false;
	}
	m_login_not_auth = strstr(m_result, "���ݿ�");
	if (m_login_not_auth != NULL)
	{
		WSACleanup();
		free(m_rep_body);
		cout << "Status: 403 Forbidden\r\n";
		Error("<p>����ϵͳ��˵���ݿⷱæ :P</p><p>����<b>���ݿ���·</b>���⣬�Ǿ͵ȵ��ȿ�~</p>");
		return false;
	}
	char *m_login_success = strstr(m_result, "ѧ�����ۺϽ���");
	if (m_login_success == NULL)
	{
		WSACleanup();
		free(m_rep_body);
		cout << "Status: 403 Forbidden\r\n";
		Error("<p>���š���������һ������⣡����ϵͳ������ 0.0</p><p>�������Ժ������԰ɡ�</p>");
		return false;
	}

	// ���ˣ�ѧ����¼�ɹ����ͷ���Դ��
	free(m_rep_body);
	return true;
}

// �ǳ�ѧ��
void student_logout()
{
	if (strcmp(CGI_HTTP_COOKIE, "") == 0) return;

	int m_iResult = 0;
	char m_logout[1024] = { 0 };
	sprintf(m_logout, REQUEST_LOGOUT, CGI_HTTP_COOKIE);
	char *m_outbuffer = (char *)malloc(1024);
	CrawlRequest(m_logout, m_outbuffer, 1024, &m_iResult);
	free(m_outbuffer);
}