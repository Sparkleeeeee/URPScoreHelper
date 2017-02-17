// GuanglingScoreHelper.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "General.h"
#include "StringHelper.h"
#include "CrawlFactory.h"
#include "GuanglingScoreHelper.h"
#include "resource.h"

// ��ں���
int main()
{
	CGI_REQUEST_METHOD = getenv("REQUEST_METHOD"); // ���󷽷�
	CGI_CONTENT_LENGTH = getenv("CONTENT_LENGTH"); // ���ݳ���
	CGI_SCRIPT_NAME = getenv("SCRIPT_NAME"); // �ű�����
	CGI_QUERY_STRING = getenv("QUERY_STRING"); // ��ѯ����
	CGI_PATH_TRANSLATED = getenv("PATH_TRANSLATED"); // �ű�λ��
	CGI_HTTP_COOKIE = getenv("HTTP_COOKIE"); // Cookie

	// ��ȡ�����û�ʹ�������ǵķ��� :)
	g_fQueryCount = fopen("QueryCount.log","r+");
	g_QueryCount = 0;
	if (g_fQueryCount != NULL) 
	{
		fscanf(g_fQueryCount, "%ld", &g_QueryCount);
	}
	else
	{
		g_fQueryCount = fopen("QueryCount.log", "w+");
	}
	if (g_fQueryCount == NULL)
	{
		puts("Status: 500 Internal Server Error\n");
		puts("<p>fopen() ʧ�ܣ�</p>");
		return -1;
	}
	fseek(g_fQueryCount, 0, SEEK_SET);

	// ������Դ
	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(IDR_HTML1), MAKEINTRESOURCE(RT_HTML));
	if (NULL == hRsrc)
	{
		puts("Status: 500 Internal Server Error\n");
		puts("<p>FindResoure() ʧ�ܣ�</p>");
		return -1;
	}

	// ��ȡ��Դ�Ĵ�С
	DWORD dwSize = SizeofResource(NULL, hRsrc);
	if (0 == dwSize)
	{
		puts("Status: 500 Internal Server Error\n");
		puts("<p>SizeofResource() ʧ�ܣ�</p>");
		return -1;
	}

	// ������Դ
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	if (NULL == hGlobal)
	{
		puts("Status: 500 Internal Server Error\n");
		puts("<p>LoadResoure() ʧ�ܣ�</p>");
		return -1;
	}

	// ������Դ
	ERROR_HTML = (char *)LockResource(hGlobal);
	if (NULL == ERROR_HTML)
	{
		puts("Status: 500 Internal Server Error\n");
		puts("<p>LockResoure() ʧ�ܣ�</p>");
		return -1;
	}

	if (CGI_REQUEST_METHOD == NULL || CGI_CONTENT_LENGTH == NULL || CGI_SCRIPT_NAME == NULL || CGI_QUERY_STRING == NULL ||
		CGI_PATH_TRANSLATED == NULL || CGI_HTTP_COOKIE == NULL)
	{
		puts("Status: 500 Internal Server Error\n");
		puts("<p>CGI �ӿ��쳣���������ã�</p>");
		return -1;
	}

	// ��ʼ�� Socket �⡣
	if (!InitSocketLibrary())
	{
		return -1;
	}

	if ( strcmp( CGI_REQUEST_METHOD, "GET" ) == 0 ) // ����� GET ����
	{
		if (strcmp(CGI_SCRIPT_NAME, "/") == 0 || strcmp(CGI_SCRIPT_NAME, "/index.cgi") == 0)
		{
			return parse_index();
		}
		if (strcmp(CGI_SCRIPT_NAME, "/query.cgi") == 0)
		{
			if (strcmp(CGI_QUERY_STRING, "act=system_registration") == 0)
			{
				return system_registration();
			}
			else 
			{
				puts("Status: 405 Method Not Allowed");
				Error("<p>��ҳ�治���� GET ����</p>");
				WSACleanup();
				return 1;
			}
		}
		puts("Status: 404 No Such CGI Page");
		Error("<p>�Ҳ�����ҳ�档</p>");
		WSACleanup();
		return 1;
	}

	if ( strcmp( CGI_REQUEST_METHOD, "POST" ) == 0 ) // ����� POST ����
	{
		if ( strcmp( CGI_SCRIPT_NAME, "/query.cgi" ) == 0 )
		{
			return parse_query();
		}
	}

	puts("Status: 500 Internal Server Error");
	Error("<p>��������δ��������쳣��</p>");
	WSACleanup();
    return 1;
}

// ������ҳ������ (GET / /index.cgi)
int parse_index()
{
	int m_iResult = 0;

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
		puts("Status: 500 Internal Server Error");
		Error("<p>�޷���ȡ Servlet Session ID��</p><p>Cookie ��ͷʧ�ܡ�</p>");
		return -1;
	}
	char *pStr2 = strstr(pStr1 + 11, ";");
	if (pStr2 == NULL)
	{
		closesocket(g_so);
		WSACleanup();
		puts("Status: 500 Internal Server Error");
		Error("<p>�޷���ȡ Servlet Session ID��</p><p>Cookie ��βʧ�ܡ�</p>");
		return -1;
	}
	mid(JSESSIONID, pStr1, pStr2 - pStr1 - 11, 11); // �ɹ���� Session ID��
	//printf(JSESSIONID);
	//printf("\n");
	free(m_rep_header);

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
	pStr1 = strstr(m_rep_body, "\r\n\r\n");
	if (pStr1 == NULL)
	{
		WSACleanup();
		puts("Status: 500 Internal Server Error");
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
		puts("Status: 500 Internal Server Error");
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
		puts("Status: 500 Internal Server Error");
		Error("<p>�޷���ȡ��ҳģ�����ݡ�</p>");
		fclose(m_file_homepage);
		free(m_lpszHomepage);
		WSACleanup();
		return -1;
	}
	fclose(m_file_homepage); // �ر��ļ�

	 // �����ҳģ��
	int m_iBufferSize = m_file_homepage_length + strlen(m_DataURL); // ��û���������
	char *m_lpszCompleteHomepage = (char *)malloc(m_iBufferSize);
	ZeroMemory(m_lpszCompleteHomepage, m_iBufferSize);
	sprintf(m_lpszCompleteHomepage, m_lpszHomepage, g_QueryCount, m_DataURL);
	free(m_lpszHomepage);

	// �����ҳ
	printf("Set-Cookie: JSESSIONID=%s; path=/\n", JSESSIONID);
	puts(GLOBAL_HEADER);
	puts(m_lpszCompleteHomepage);

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
		puts("Status: 500 Internal Server Error");
		Error("<p>��������POST ���ݳ����쳣��</p>");
		WSACleanup();
		return -1;
	}
	char *m_post_data = (char *)malloc(m_post_length + 2);	// TODO: Ϊʲô������+2������+1��ò��CONTENTLENGTH֮����1����������
	if (fgets(m_post_data, m_post_length + 1 , stdin) == NULL) // TODO: CONTENTLENGTH֮����1����������
	{
		puts("Status: 500 Internal Server Error");
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
		puts("Status: 500 Internal Server Error");
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
		puts("Status: 500 Internal Server Error");
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
		puts("Status: 500 Internal Server Error");
		Error("<p>�޷���ȡ��֤����Ϣ��</p>");
		return -1;
	}
	char m_captcha[128] = { 0 };
	right(m_captcha, pStr1 + 4, 4);

	// ���͵�½����
	int m_iResult = 0;
	char * m_rep_body = (char *)malloc(40960);
	ZeroMemory(m_rep_body, 40960);
	char POST_LOGIN[1024] = { 0 };
	char *m_origin = "zjh1=&tips=&lx=&evalue=&eflag=&fs=&dzslh=&zjh=%s&mm=%s&v_yzm=%s";
	char m_padding[1024] = { 0 };
	sprintf(m_padding, m_origin, m_xuehao, m_password, m_captcha);
	int m_ContentLength = strlen(m_padding); // TODO: ���ﲻ�ü�Ī������Ľ�������
	sprintf( POST_LOGIN, REQUEST_LOGIN, m_ContentLength, CGI_HTTP_COOKIE, m_xuehao, m_password, m_captcha );
	if (!CrawlRequest( POST_LOGIN, m_rep_body, 40960, &m_iResult ))
	{
		free(m_rep_body);
		return -1;
	}

	// ��ȡ��¼�����
	char *m_result = strstr(m_rep_body, "\r\n\r\n");
	if (m_result == NULL)
	{
		WSACleanup();
		free(m_post_data);
		free(m_rep_body);
		puts("Status: 500 Internal Server Error");
		Error("<p>�ӷ�������ȡ��¼���ʧ�ܡ�</p>");
		return -1;
	}

	// �����¼�����
	char *m_login_not_auth = strstr(m_result, "֤����");
	if (m_login_not_auth != NULL)
	{
		WSACleanup();
		free(m_post_data);
		free(m_rep_body);
		puts("Status: 403 Forbidden");
		Error("<p>ѧ�Ż����벻�԰������С� TAT��</p><p>Ĭ�������Ǿ���ѧ��Ŷ��������޸Ĺ����룬�㶮��~</p>");
		return -1;
	}
	m_login_not_auth = strstr(m_result, "��֤��");
	if (m_login_not_auth != NULL)
	{
		WSACleanup();
		free(m_post_data);
		free(m_rep_body);
		puts("Status: 403 Forbidden");
		Error("<p>��֤�벻�԰������С� TAT��</p>");
		return -1;
	}
	m_login_not_auth = strstr(m_result, "���ݿ�");
	if (m_login_not_auth != NULL)
	{
		WSACleanup();
		free(m_post_data);
		free(m_rep_body);
		puts("Status: 403 Forbidden");
		Error("<p>����ϵͳ��˵���ݿⷱæ :P</p><p>����<b>���ݿ���·</b>���⣬�Ǿ͵ȵ��ȿ�~</p>");
		return -1;
	}
	char *m_login_success = strstr(m_result, "ѧ�����ۺϽ���");
	if (m_login_success == NULL)
	{
		WSACleanup();
		free(m_post_data);
		free(m_rep_body);
		puts("Status: 403 Forbidden");
		Error("<p>���š���������һ������⣡����ϵͳ������ 0.0</p>");
		return -1;
	}
	
	// ���ˣ�ѧ����¼�ɹ����ͷ���Դ��
	free(m_post_data);
	free(m_rep_body);

	// ��ʼ��֡�
	char QUERY_SCORE[512] = { 0 };
	m_rep_body = (char *)malloc(81920);
	sprintf( QUERY_SCORE, REQUEST_QUERY_SCORE, CGI_HTTP_COOKIE );
	if (!CrawlRequest(QUERY_SCORE, m_rep_body, 81920, &m_iResult))
	{
		free(m_rep_body);
		return -1;
	}
	m_result = strstr(m_rep_body, "\r\n\r\n");
	if (m_result == NULL)
	{
		WSACleanup();
		free(m_post_data);
		free(m_rep_body);
		puts("Status: 500 Internal Server Error");
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

	char *m_query_not_reg = strstr(p_lpszScore, "û��ע��");
	if (m_query_not_reg != NULL)
	{
		free(m_lpszQuery);
		puts("Status: 403 Forbidden");
		char *m_original_str = "<p><b>�װ���%s������ϵͳ��˵�㱾ѧ�ڻ�û�е���ע�� 0.0</b></p><p>�ҿ���ʩչ������һ�������ڽ���ϵͳע��Ŷ~</p>\
				<div class=\"col-100\"><a href=\"query.cgi?act=system_registration\" class=\"button button-big but\
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
		free(m_lpszQuery);
		puts(GLOBAL_HEADER);
		puts(p_lpszScore);
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

		// ������xѧ�֣�ȫ��������/��ѧ�� = ��Ȩ��
		float m_xuefen = atof(m_subXuefen);
		if (m_xuefen != 0)
		{
			m_Total_xuefen += m_xuefen;
		}
		float m_chengji = atof(m_subchengji);
		if (m_chengji != 0)
		{
			double m_pointsxxuefen = m_xuefen * m_chengji;
			if (m_pointsxxuefen != 0)
			{
				m_Total_pointsxxuefen += m_pointsxxuefen;
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

	puts(GLOBAL_HEADER);

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
	
	free(m_lpszQuery);

	fprintf(g_fQueryCount, "%ld", ++g_QueryCount);
	fclose(g_fQueryCount);

	puts(m_lpszCompleteQuery);

	// ��ȫ�ǳ�����ϵͳ��
	int m_iResult = 0;
	char m_logout[1024] = { 0 };
	sprintf(m_logout, REQUEST_LOGOUT, CGI_HTTP_COOKIE);
	char *m_outbuffer = (char *)malloc(1024);
	if (!CrawlRequest(m_logout, m_outbuffer, 40960, &m_iResult))
	{
		free(m_outbuffer);
		return;
	}
	free(m_outbuffer);

}

// ��ȡѧ������
void get_student_name(char *p_lpszBuffer)
{
	if (strlen(CGI_HTTP_COOKIE) == 0)
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
	if (strlen(CGI_HTTP_COOKIE) == 0)
	{
		puts("Status: 302 Found\nLocation: index.cgi\n");
		puts(GLOBAL_HEADER);
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
		return -1;
	}

	// ������Ҫ����ע���ѧ����Ϣ��
	char *pStr1 = strstr(m_rep_header, "selected>");
	if (pStr1 == NULL)
	{
		free(m_rep_header);
		Error("<p>���ݴ��󡣲�����˼���Զ�ע��ʧ�ܣ��������ȥ����ϵͳ������~ (1)</p>");
		return -1;
	}
	pStr1 -= 70;
	char *pStr2 = strstr(pStr1, "<option value=\"");
	if (pStr2 == NULL)
	{
		free(m_rep_header);
		Error("<p>���ݴ��󡣲�����˼���Զ�ע��ʧ�ܣ��������ȥ����ϵͳ������~ (2)</p>");
		return -1;
	}
	pStr1 = pStr2;
	pStr2 = strstr(pStr1 + 16, "\"");
	if (pStr2 == NULL)
	{
		free(m_rep_header);
		Error("<p>���ݴ��󡣲�����˼���Զ�ע��ʧ�ܣ��������ȥ����ϵͳ������~ (3)</p>");
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
		free(m_rep_header);
		return -1;
	}

	// ����Ƿ�ע��ɹ���
	pStr1 = strstr(m_rep_header, "ע��ɹ�");
	if (pStr1 == NULL)
	{
		free(m_rep_header);
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
		return -1;
	}
	char *m_result = strstr(m_rep_header, "\r\n\r\n");
	if (m_result == NULL)
	{
		WSACleanup();
		free(m_rep_header);
		puts("Status: 500 Internal Server Error");
		Error("<p>�ӷ�������ȡ����ʧ�ܡ�</p>");
		return -1;
	}

	// �Ż����ܽ������ʾ��ѯҳ��
	parse_friendly_score(m_result);
	free(m_rep_header);

	// ����~��
	WSACleanup();
	return 0;
}
