#pragma once
char *CGI_REQUEST_METHOD; // ���󷽷�
char *CGI_CONTENT_LENGTH; // ���ݳ���
char *CGI_SCRIPT_NAME; // �ű�����
char *CGI_QUERY_STRING; // ��ѯ����
char *CGI_PATH_TRANSLATED; // �ű�λ��
char *CGI_HTTP_COOKIE; // Cookie

FILE *g_fQueryCount;
INT64 g_QueryCount;

int parse_index();
int parse_query();
void parse_friendly_score(char *p_lpszScore);
void get_student_name(char *p_lpszBuffer);
int system_registration();