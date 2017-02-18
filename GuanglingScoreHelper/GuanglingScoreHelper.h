#pragma once
const char *CGI_REQUEST_METHOD; // ���󷽷�
const char *CGI_CONTENT_LENGTH; // ���ݳ���
const char *CGI_SCRIPT_NAME; // �ű�����
const char *CGI_QUERY_STRING; // ��ѯ����
const char *CGI_PATH_TRANSLATED; // �ű�λ��
const char *CGI_HTTP_COOKIE; // Cookie
const char *CGI_SERVER_SOFTWARE; // ���������

FILE *g_fQueryCount;
INT64 g_QueryCount;

int parse_index();
int parse_query();
void parse_friendly_score(char *p_lpszScore);
void get_student_name(char *p_lpszBuffer);
int system_registration();
void student_logout();
bool student_login(char *p_xuehao, char *p_password, char *p_captcha);