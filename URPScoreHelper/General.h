#pragma once

#define SOFTWARE_NAME "Ψ��С����"

// �������ⲿʹ�õı���
extern SOCKET g_so;

extern const char* GLOBAL_HEADER;
extern const char* SCORE_TEMPLATE;
extern const char* QUICK_SCORE;

extern const char* REQUEST_HOME_PAGE;
extern const char* REQUEST_HOME_PAGE_WITH_COOKIE;
extern const char* REQUEST_CAPTCHA;
extern const char* REQUEST_LOGIN;
extern const char* REQUEST_QUERY_SCORE;
extern const char* REQUEST_LOGOUT;
extern const char* REQUEST_GET_REGISTER_INTERFACE;
extern const char* REQUEST_POST_REGISTER_INTERFACE;
extern const char* REQUEST_SET_REPORT_PARAMS;
extern const char* REQUEST_REPORT_FILES;
extern const char* REQUEST_TXT_SCORES;
extern const char* REQUEST_PHOTO;
extern const char* REQUEST_TOP;
extern const char* GET_GRADE_BY_QBINFO;
extern const char* GET_GRADE_BY_PLAN;
extern const char* GET_GRADE_BY_FAILED;

extern char *SERVER;
extern char *OAUTH2_APPID;
extern char *OAUTH2_SECRET;

extern char* ERROR_HTML;
extern char JSESSIONID[256];
extern void Error(char *p_ErrMsg);
extern char * base64_encode(const unsigned char * bindata, char * base64, int binlength);