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
	// ������Դ
	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(IDR_HTML1), MAKEINTRESOURCE(RT_HTML));
	if (NULL == hRsrc)
	{
		cout << "Status: 500 Internal Server Error\n"
			<< GLOBAL_HEADER
			<< "<p>FindResource() ʧ�ܣ�</p>";
		return -1;
	}

	// ��ȡ��Դ�Ĵ�С
	DWORD dwSize = SizeofResource(NULL, hRsrc);
	if (0 == dwSize)
	{
		cout << "Status: 500 Internal Server Error\n"
			<< GLOBAL_HEADER
			<< "<p>SizeofResource() ʧ�ܣ�</p>";
		return -1;
	}

	// ������Դ
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	if (NULL == hGlobal)
	{
		cout << "Status: 500 Internal Server Error\n"
			<< GLOBAL_HEADER
			<< "<p>LoadResource() ʧ�ܣ�</p>";
		return -1;
	}

	// ������Դ
	ERROR_HTML = (char *)LockResource(hGlobal);
	if (NULL == ERROR_HTML)
	{
		cout << "Status: 500 Internal Server Error\n"
			<< GLOBAL_HEADER
			<< "<p>LockResource() ʧ�ܣ�</p>";
		return -1;
	}

	// ��ʼ�� Socket �⡣
	if (!InitSocketLibrary())
	{
		cout << "Status: 500 Internal Server Error\n"
			<< GLOBAL_HEADER
			<< "<p>Socket ��ʼ��ʧ�ܣ�</p>";
		return -1;
	}

		CGI_REQUEST_METHOD = getenv("REQUEST_METHOD"); // ���󷽷�
		CGI_CONTENT_LENGTH = getenv("CONTENT_LENGTH"); // ���ݳ���
		CGI_SCRIPT_NAME = getenv("SCRIPT_NAME"); // �ű�����
		CGI_QUERY_STRING = getenv("QUERY_STRING"); // ��ѯ����
		CGI_PATH_TRANSLATED = getenv("PATH_TRANSLATED"); // �ű�λ��
		CGI_HTTP_COOKIE = getenv("HTTP_COOKIE"); // Cookie
		CGI_SERVER_SOFTWARE = getenv("SERVER_SOFTWARE"); // ���������

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
			cout << "Status: 500 Internal Server Error\n"
				<< GLOBAL_HEADER
				<< "<p>fopen() ʧ�ܣ�</p>";
			return 0;
		}
		fseek(g_fQueryCount, 0, SEEK_SET);

		if (CGI_REQUEST_METHOD == NULL || CGI_CONTENT_LENGTH == NULL || CGI_SCRIPT_NAME == NULL || CGI_QUERY_STRING == NULL ||
			CGI_PATH_TRANSLATED == NULL || CGI_HTTP_COOKIE == NULL)
		{
			cout << "Status: 500 Internal Server Error\n"
				<< GLOBAL_HEADER
				<< "<p>FastCGI �ӿ��쳣���������á�</p>";
			return 0;
		}

		if (strcmp(CGI_REQUEST_METHOD, "GET") == 0) // ����� GET ����
		{
			if (strcmp(CGI_SCRIPT_NAME, "/") == 0 || strcmp(CGI_SCRIPT_NAME, "/index.cgi") == 0)
			{
				if (strcmp(CGI_QUERY_STRING, "act=logout") == 0)
				{
					student_logout();
					cout << "Status: 302 Found\n" << "Location: index.cgi\n" << GLOBAL_HEADER;
					return 0;
				}

				parse_index();
				return 0;
			}
			if (strcmp(CGI_SCRIPT_NAME, "/query.cgi") == 0)
			{
				if (strcmp(CGI_QUERY_STRING, "act=system_registration") == 0)
				{
					system_registration();
					
					ZeroMemory(JSESSIONID, 256);
					return -1;
				}
				else
				{
					cout << "Status: 405 Method Not Allowed\n";
					Error("<p>��ҳ�治���� GET ����</p>");
					WSACleanup();
					return 0;
				}
			}

			if (strcmp(CGI_SCRIPT_NAME, "/QuickQuery.cgi") == 0)
			{
				parse_QuickQuery_Intro();
				return 0;
			}

			cout << "Status: 404 No Such CGI Page\n";
			Error("<p>�Ҳ�����ҳ�档</p>");
			WSACleanup();
			
			ZeroMemory(JSESSIONID, 256);
			return -1;
		}

		if (strcmp(CGI_REQUEST_METHOD, "POST") == 0) // ����� POST ����
		{
			if (strcmp(CGI_SCRIPT_NAME, "/query.cgi") == 0)
			{
				if (strcmp(CGI_QUERY_STRING, "act=QuickQuery") == 0)
				{
					parse_QuickQuery_Result();
					return 0;
				}
				parse_query();
				return 0;
			}
		}

	puts("Status: 500 Internal Server Error");
	Error("<p>��������δ��������쳣��</p>");
	WSACleanup();
	return -1;
}

// ���� Cookie
int process_cookie(bool *p_need_update_cookie)
{
	int m_iResult = 0;

	if (strcmp(CGI_HTTP_COOKIE, "") != 0) // ����ͻ����Ѿ��õ� JSESSIONID
	{
		// �����ڴ棬�����ܷ���˷������ݡ�
		char * m_rep_header = (char *)malloc(1024);
		ZeroMemory(m_rep_header, 1024);
		char m_req_homepage_cookie[2048] = { 0 };
		sprintf(m_req_homepage_cookie, REQUEST_HOME_PAGE_WITH_COOKIE, CGI_HTTP_COOKIE);
		if (!CrawlRequest(m_req_homepage_cookie, m_rep_header, 1024, &m_iResult))
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
				cout << "Status: 500 Internal Server Error\n";
				Error("<p>�޷���ȡ Servlet Session ID��</p><p>Cookie ��βʧ�ܡ�</p>");
				return -1;
			}
			mid(JSESSIONID, pStr1, pStr2 - pStr1 - 11, 11); // �ɹ������ Session ID��
			*p_need_update_cookie = true;
			free(m_rep_header);
			return 0;
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
		char * m_rep_header = (char *)malloc(1024);
		ZeroMemory(m_rep_header, 1024);
		if (!CrawlRequest(REQUEST_HOME_PAGE, m_rep_header, 1024, &m_iResult))
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
			cout << "Status: 500 Internal Server Error\n";
			Error("<p>�޷���ȡ Servlet Session ID��</p><p>Cookie ��ͷʧ�ܡ�</p>");
			return -1;
		}
		char *pStr2 = strstr(pStr1 + 11, ";");
		if (pStr2 == NULL)
		{
			closesocket(g_so);
			WSACleanup();
			cout << "Status: 500 Internal Server Error\n";
			Error("<p>�޷���ȡ Servlet Session ID��</p><p>Cookie ��βʧ�ܡ�</p>");
			return -1;
		}
		mid(JSESSIONID, pStr1, pStr2 - pStr1 - 11, 11); // �ɹ���� Session ID��

		free(m_rep_header);
		*p_need_update_cookie = true;
	}
}

// ������ҳ������ (GET / /index.cgi)
int parse_index()
{
	int m_iResult = 0;
	bool m_need_update_cookie = false;

	process_cookie(&m_need_update_cookie);

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
		cout << "Status: 500 Internal Server Error\n";
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
		cout << "Status: 500 Internal Server Error\n";
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
		cout << "Status: 500 Internal Server Error\n";
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
		cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\n";

	cout << GLOBAL_HEADER;
	cout << m_lpszCompleteHomepage;

	// �ͷ���Դ
	free(m_lpszCompleteHomepage);
	WSACleanup();
	return 0;
}

// �����ѯҳ������ (POST /query.cgi)
int parse_query()
{
	// ��ȡ POST ���ݡ�
	int m_post_length = atoi(CGI_CONTENT_LENGTH);
	if (m_post_length <= 0)
	{
		cout << "Status: 500 Internal Server Error\n";
		Error("<p>��������POST ���ݳ����쳣��</p>");
		WSACleanup();
		return -1;
	}
	char *m_post_data = (char *)malloc(m_post_length + 2);	// TORESEARCH
	if (fgets(m_post_data, m_post_length + 1, stdin) == NULL)
	{
		cout << "Status: 500 Internal Server Error\n";
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
		cout << "Status: 500 Internal Server Error\n";
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
		cout << "Status: 500 Internal Server Error\n";
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
		cout << "Status: 500 Internal Server Error\n";
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
		cout << "Status: 500 Internal Server Error\n";
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
		cout << "Status: 403 Forbidden\n";

		char *m_original_str = "<p><b>�װ���%s��ϵͳ��˵�㱾ѧ�ڻ�û�е���ע�� 0.0</b></p><p>�ҿ���ʩչ������\
һ�������ڽ���ϵͳע��Ŷ~</p><p>--&gt; �㰴�·���ť��ֱ���ֽ��� :P &lt;--</p>\
<div class=\"col-100\"><a href=\"query.cgi?act=system_registration\" class=\"button button-big but\
ton-fill button-success\">һ��ע��</a></div>";

		char m_output_str[1024] = { 0 };
		sprintf(m_output_str, m_original_str, m_Student);
		Error(m_output_str);
		return;
	}

	// ��ȫ�ǳ�����ϵͳ��
	student_logout();

	// ��λ����һ��ɼ�
	char *pStr1 = strstr(p_lpszScore,"<tr class=\"odd\" onMouseOut=\"this.className='even';\" onMouseOver=\"this.className='evenfocus';\">");
	if (pStr1 == NULL)
	{
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

	double m_Total_xuefen = 0.0;
	double m_Total_pointsxxuefen = 0.0;

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
		if (strstr(m_subchengji, "����") != NULL)
		{
			strcpy(m_subchengji,"95");
		}
		if (strstr(m_subchengji, "����") != NULL)
		{
			strcpy(m_subchengji, "85");
		}
		if (strstr(m_subchengji, "�е�") != NULL)
		{
			strcpy(m_subchengji, "75");
		}
		if (strstr(m_subchengji, "����") != NULL)
		{
			strcpy(m_subchengji, "65");
		}
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
		if (strstr(m_subName, "����") == NULL && strstr(m_subName, "����ѵ��") == NULL 
			&& strstr(m_subName, "ʵ��") == NULL)
		{
			float m_xuefen = atof(m_subXuefen);
			float m_chengji = atof(m_subchengji);
			if (m_xuefen != 0) // ��0ѧ��...����ͳ����ѧ��
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
		free(m_lpszQuery);
		Error("<p>�ڸ������������������</p>");
		return;
	}

	// ��䷵��ҳ��
	if (m_Total_pointsxxuefen != 0 || m_Total_xuefen != 0)
	{
		char m_jiaquanfen[81920] = { 0 };
		sprintf(m_jiaquanfen, "<div id=\"i_total\"><p>��ѧ�ڼ�Ȩ�ۺϷ֣����š��������ݣ����ο�����</p><center>%.1f</center></div>", 
				m_Total_pointsxxuefen / m_Total_xuefen);
		strcat(m_jiaquanfen, m_Output);
		ZeroMemory(m_Output, 81920);
		strcpy(m_Output, m_jiaquanfen);
	}
	char m_lpszCompleteQuery[81920] = { 0 };
	sprintf(m_lpszCompleteQuery, m_lpszQuery, m_Student, m_Student, m_Output);
	
	cout << GLOBAL_HEADER << m_lpszCompleteQuery;

	free(m_lpszQuery);
	fprintf(g_fQueryCount, "%ld", ++g_QueryCount);
	fclose(g_fQueryCount);
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

// ����ϵͳ����ע�� (GET /query.cgi?act=system_registration)
int system_registration()
{
	if (strcmp(CGI_HTTP_COOKIE, "") == 0)
	{
		cout << "Status: 302 Found\n" << "Location: index.cgi\n" << GLOBAL_HEADER;
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
		cout << "Status: 500 Internal Server Error\n";
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
		cout << "Status: 500 Internal Server Error\n";
		Error("<p>�ӷ�������ȡ��¼���ʧ�ܡ�</p>");
		return false;
	}

	// �����¼�����
	char *m_login_not_auth = strstr(m_result, "֤����");
	if (m_login_not_auth != NULL)
	{
		WSACleanup();
		free(m_rep_body);
		cout << "Status: 403 Forbidden\n";
		Error("<p>ѧ�Ż����벻�԰������С� TAT��</p><p>Ĭ�������Ǿ���ѧ��Ŷ��������޸Ĺ����룬�㶮��~</p>");
		return false;
	}
	m_login_not_auth = strstr(m_result, "��֤��");
	if (m_login_not_auth != NULL)
	{
		WSACleanup();
		free(m_rep_body);
		cout << "Status: 403 Forbidden\n";
		Error("<p>��֤�벻�԰������С� TAT��</p>");
		return false;
	}
	m_login_not_auth = strstr(m_result, "���ݿ�");
	if (m_login_not_auth != NULL)
	{
		WSACleanup();
		free(m_rep_body);
		cout << "Status: 403 Forbidden\n";
		Error("<p>����ϵͳ��˵���ݿⷱæ :P</p><p>����<b>���ݿ���·</b>���⣬�Ǿ͵ȵ��ȿ�~</p>");
		return false;
	}
	char *m_login_success = strstr(m_result, "ѧ�����ۺϽ���");
	if (m_login_success == NULL)
	{
		WSACleanup();
		free(m_rep_body);
		cout << "Status: 403 Forbidden\n";
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
	char m_logout[10240] = { 0 };
	sprintf(m_logout, REQUEST_LOGOUT, CGI_HTTP_COOKIE);
	char *m_outbuffer = (char *)malloc(10240);
	CrawlRequest(m_logout, m_outbuffer, 10240, &m_iResult);
	free(m_outbuffer);
}

// ���ܲ�ѯ��� (/QuickQuery.cgi)
void parse_QuickQuery_Intro()
{
	bool m_need_update_cookie = false;
	process_cookie(&m_need_update_cookie);

	// ����ҳ���ļ�
	FILE *m_file_query = fopen(CGI_PATH_TRANSLATED, "rb");
	if (m_file_query == NULL)
	{
		if (m_need_update_cookie)
			cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\n";
		Error("<p>�����Ҳ������ܲ�ѯģ�塣</p>");
		return;
	}
	fseek(m_file_query, 0, SEEK_END); // �Ƶ�β
	int m_file_query_length = ftell(m_file_query); // ��ȡ�ļ�����
	fseek(m_file_query, 0, SEEK_SET); // �����ƻ���
	char *m_lpszQuery = (char *)malloc(m_file_query_length + 1);
	ZeroMemory(m_lpszQuery, m_file_query_length + 1);
	if (fread(m_lpszQuery, m_file_query_length, 1, m_file_query) != 1) // ��Ӳ�����ݿ����ڴ�
	{
		if (m_need_update_cookie)
			cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\n";
		Error("<p>�޷���ȡ���ܲ�ѯģ�����ݡ�</p>");
		fclose(m_file_query);
		free(m_lpszQuery);
		return;
	}
	fclose(m_file_query); // �ر��ļ�

	int m_ilength = strlen(m_lpszQuery) + 512;
	char *m_lpszCompleteQuery = (char *)malloc(m_ilength);
	ZeroMemory(m_lpszCompleteQuery, m_ilength);
	sprintf(m_lpszCompleteQuery, m_lpszQuery, g_QueryCount);

	if (m_need_update_cookie)
		cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\n";
	cout << GLOBAL_HEADER << m_lpszCompleteQuery;

	free(m_lpszQuery);
	free(m_lpszCompleteQuery);
}

// ���ܲ�ѯ��� (/query.cgi?act=QuickQuery)
void parse_QuickQuery_Result()
{
	bool m_need_update_cookie = false;
	process_cookie(&m_need_update_cookie);

	// �������ҳ���ļ�
	FILE *m_file_query = fopen(CGI_PATH_TRANSLATED, "rb");
	if (m_file_query == NULL)
	{
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
		Error("<p>�޷���ȡ����ҳģ�����ݡ�</p>");
		fclose(m_file_query);
		free(m_lpszQuery);
		return;
	}
	fclose(m_file_query); // �ر��ļ�

	// ��ȡ POST ���ݡ�
	int m_post_length = atoi(CGI_CONTENT_LENGTH);
	if (m_post_length <= 0)
	{
		cout << "Status: 500 Internal Server Error\n";
		if (m_need_update_cookie)
			cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\n";
		Error("<p>��������POST ���ݳ����쳣��</p>");
		WSACleanup();
		return;
	}
	char *m_post_data = (char *)malloc(m_post_length + 2);	// TORESEARCH
	if (fgets(m_post_data, m_post_length + 1, stdin) == NULL)
	{
		cout << "Status: 500 Internal Server Error\n";
		if (m_need_update_cookie)
			cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\n";
		Error("<p>��������POST ������ȡʧ�ܡ�</p>");
		WSACleanup();
		return;
	}
	
	// ��ȡѧ��
	char *pStr1 = strstr(m_post_data, "xh=");
	if (pStr1 == NULL)
	{
		WSACleanup();
		free(m_post_data);
		cout << "Status: 500 Internal Server Error\n";
		if (m_need_update_cookie)
			cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\n";
		Error("<p>�޷���ȡѧ����Ϣ��</p>");
		return;
	}
	char *pStr2 = strstr(pStr1 + 3, "&");
	char m_xuehao[128] = { 0 };
	right(m_xuehao, pStr1, strlen(pStr1) - 3);
	pStr1 = NULL;
	pStr2 = NULL;

	char m_query_param[512] = { 0 };
	sprintf(m_query_param, "LS_XH=%s", m_xuehao);
	strcat(m_query_param, "&resultPage=http%3A%2F%2F58.220.248.249%3A80%2FreportFiles%2Fcj%2Fcj_zwcjd.jsp%3F");
	char m_query_request[1024] = { 0 };
	sprintf(m_query_request, REQUEST_SET_REPORT_PARAMS, CGI_HTTP_COOKIE, strlen(m_query_param));
	strcat(m_query_request, m_query_param);
	free(m_post_data);

	int m_ilength = 0;
	char *m_lpvBuffer = (char *)malloc(4096);
	ZeroMemory(m_lpvBuffer, 4096);
	if (!CrawlRequest(m_query_request, m_lpvBuffer, 4096, &m_ilength))
	{
		cout << "Status: 500 Internal Server Error\n";
		if (m_need_update_cookie)
			cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\n";
		Error("<p>Ͷ�����ܲ�ѯ����ʧ�ܡ�</p><p>��ȷ�Ͻ���ϵͳ�ǿ��õġ�</p>");
		WSACleanup();
		free(m_lpvBuffer);
		return;
	}

	pStr1 = strstr(m_lpvBuffer, "&reportParamsId=");
	if (pStr1 == NULL)
	{
		cout << "Status: 500 Internal Server Error\n";
		if (m_need_update_cookie)
			cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\n";
		Error("<p>��ȡ����ID����(1)</p>");
		WSACleanup();
		free(m_lpvBuffer);
		return;
	}
	pStr2 = strstr(pStr1 + 16, "\n");
	if (pStr1 == NULL)
	{
		cout << "Status: 500 Internal Server Error\n";
		if (m_need_update_cookie)
			cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\n";
		Error("<p>��ȡ����ID����(2)</p>");
		WSACleanup();
		free(m_lpvBuffer);
		return;
	}
	char m_paramsID[512] = { 0 };
	mid(m_paramsID, pStr1 + 16, pStr2 - pStr1 - 16, 0);
	pStr1 = NULL;
	pStr2 = NULL;
	free(m_lpvBuffer);

	char m_query_report[512] = { 0 };
	sprintf(m_query_report, REQUEST_REPORT_FILES, m_paramsID, CGI_HTTP_COOKIE);
	m_lpvBuffer = (char *)malloc(40960);
	ZeroMemory(m_lpvBuffer, 40960);
	if (!CrawlRequest(m_query_report, m_lpvBuffer, 40960, &m_ilength))
	{
		cout << "Status: 500 Internal Server Error\n";
		if (m_need_update_cookie)
			cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\n";
		Error("<p>ͨ��ID���ܲ�ѯʧ�ܡ�</p><p>��������֪���Ĵ���</p>");
		WSACleanup();
		free(m_lpvBuffer);
		return;
	}

	pStr1 = strstr(m_lpvBuffer, "Exception: ");
	if (pStr1 != NULL)
	{
		pStr2 = strstr(pStr1, "at");
		if (pStr2 != NULL)
		{
			cout << "Status: 500 Internal Server Error\n";
			if (m_need_update_cookie)
				cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\n";
			char m_Exception[512] = { 0 };
			mid(m_Exception, pStr1 + 11, pStr2 - pStr1 - 11, 0);
			char m_ExceptionMsg[512] = "<p><b>����ϵͳ�׳������´���...</b></p><p>";
			strcat(m_ExceptionMsg, m_Exception);
			strcat(m_ExceptionMsg, "</p>");
			Error(m_ExceptionMsg);
			WSACleanup();
			free(m_lpvBuffer);
			return;
		}
	}
	pStr1 = NULL;
	pStr2 = NULL;

	pStr1 = strstr(m_lpvBuffer, "com.runqian.report.view.text.TextFileServlet");
	if (pStr1 == NULL)
	{
		cout << "Status: 500 Internal Server Error\n";
		if (m_need_update_cookie)
			cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\n";
		Error("<p>���ܲ�ѯ���ز�������(1)</p>");
		WSACleanup();
		free(m_lpvBuffer);
		return;
	}
	pStr2 = strstr(pStr1 + 46, "\";");
	if (pStr1 == NULL)
	{
		cout << "Status: 500 Internal Server Error\n";
		if (m_need_update_cookie)
			cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\n";
		Error("<p>���ܲ�ѯ���ز�������(2)</p>");
		WSACleanup();
		free(m_lpvBuffer);
		return;
	}
	char m_txt_req_path[512] = { 0 };
	mid(m_txt_req_path, pStr1 + 45, pStr2 - pStr1 - 45, 0);
	free(m_lpvBuffer);

	char m_query_score[512] = { 0 };
	sprintf(m_query_score, REQUEST_TXT_SCORES, m_txt_req_path, m_paramsID, CGI_HTTP_COOKIE);
	m_lpvBuffer = (char *)malloc(40960);
	ZeroMemory(m_lpvBuffer, 40960);
	if (!CrawlRequest(m_query_score, m_lpvBuffer, 40960, &m_ilength))
	{
		cout << "Status: 500 Internal Server Error\n";
		if (m_need_update_cookie)
			cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\n";
		Error("<p>���ܷ�����Ϣʧ�ܣ�</p>");
		WSACleanup();
		free(m_lpvBuffer);
		return;
	}

	pStr1 = strstr(m_lpvBuffer, "����\t");
	if (pStr1 == NULL)
	{
		cout << "Status: 500 Internal Server Error\n";
		if (m_need_update_cookie)
			cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\n";
		Error("<p>ѧ��������ȡʧ�ܣ�(1)</p>");
		WSACleanup();
		free(m_lpvBuffer);
		return;
	}
	pStr2 = strstr(pStr1 + 4, "\t\t");
	if (pStr2 == NULL)
	{
		cout << "Status: 500 Internal Server Error\n";
		if (m_need_update_cookie)
			cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\n";
		Error("<p>ѧ��������ȡʧ�ܣ�(2)</p>");
		WSACleanup();
		free(m_lpvBuffer);
		return;
	}
	char m_xxmz[128] = { 0 };
	mid(m_xxmz, pStr1 + 4, pStr2 - pStr1 - 5, 1);
	char m_xxmz_html[128] = { 0 };
	sprintf(m_xxmz_html, "<div class=\"content-block-title\">%s</div>", m_xxmz);

	pStr1 = NULL;
	pStr2 = NULL;
	pStr1 = strstr(m_lpvBuffer, "����ʱ��\t\t\n");
	if (pStr1 == NULL)
	{
		cout << "Status: 500 Internal Server Error\n";
		if (m_need_update_cookie)
			cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\n";
		Error("<p>���ܵ��ı���������⡣</p>");
		WSACleanup();
		free(m_lpvBuffer);
		return;
	}

	if (strlen(m_lpvBuffer) <= 800)
	{
		cout << "Status: 500 Internal Server Error\n";
		if (m_need_update_cookie)
			cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\n";
		Error("<p>�յ��ı����С�������⡣</p>");
		WSACleanup();
		free(m_lpvBuffer);
		return;
	}

	pStr1 += 11;
	pStr2 = strstr(pStr1, "\t\t\t\t");
	bool m_success = true;
	cout << GLOBAL_HEADER;
	char m_list[40960] = { 0 };
	strcat(m_list, m_xxmz_html);
	strcat(m_list, "<div class=\"list-block\"><ul>");

	while (pStr2 != NULL)
	{
		char m_kcmz[128] = { 0 };
		mid(m_kcmz, pStr1, pStr2 - pStr1, 0);
		//cout << m_kcmz;
		
		pStr1 = pStr2 + 4;
		pStr2 = strstr(pStr1, "\t");
		if (pStr2 == NULL)
		{
			m_success = false;
			break;
		}

		char m_xf[64] = { 0 };
		mid(m_xf, pStr1, pStr2 - pStr1, 0);
		pStr1 = pStr2 + 1;
		pStr2 = strstr(pStr1, "\t");
		if (pStr2 == NULL)
		{
			m_success = false;
			break;
		}

		char m_cj[64] = { 0 };
		mid(m_cj, pStr1, pStr2 - pStr1, 0);
		//cout << " - " << m_cj;
		pStr2 = strstr(pStr2 + 1, "\t");
		if (pStr2 == NULL)
		{
			m_success = false;
			break;
		}

		char m_lb[64] = { 0 };
		mid(m_lb, pStr1, pStr2 - pStr1, 0);
		if (strstr(m_lb, "����") != NULL)
		{
			char m_kcmz_cx[256] = { 0 };
			strcat(m_kcmz_cx, "[����] ");
			strcat(m_kcmz_cx, m_kcmz);
			strcpy(m_kcmz, m_kcmz_cx);
		}

		//cout << " - " << m_lb;
		pStr1 = pStr2 + 1;
		pStr2 = strstr(pStr1, "\t\t");
		if (pStr2 == NULL)
		{
			m_success = false;
			break;
		}
		char m_date[128] = { 0 };
		mid(m_date, pStr1, pStr2 - pStr1, 0);
		//cout << " - " << m_date << endl;
		pStr1 = pStr2 + 2;
		pStr2 = strstr(pStr1, "\t\t\t\t");

		if (atoi(m_xf) == 0 && atoi(m_cj) == 0)
		{
			break;
		}

		char m_temp[1024] = { 0 };
		sprintf(m_temp, QUICK_SCORE, m_kcmz, m_cj);
		strcat(m_list, m_temp);
	}

	free(m_lpvBuffer);
	if (!m_success)
	{
		cout << "Status: 500 Internal Server Error\n";
		if (m_need_update_cookie)
			cout << "Set-Cookie: JSESSIONID=" << JSESSIONID << "; path=/\n";
		Error("<p>��Ǹ�����ܲ�ѯʧ�ܣ����Ժ����ԡ�</p>");
		WSACleanup();
		return;
	}
	strcat(m_list, "</ul></div>");
	char m_outer[81920] = { 0 };
	sprintf(m_outer, m_lpszQuery, m_xxmz, m_xxmz, m_list);
	cout << m_outer;
}