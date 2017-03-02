#include "stdafx.h"
#include "General.h"

using namespace std;

SOCKET g_so = INVALID_SOCKET;
char JSESSIONID[256] = {0};

const char* GLOBAL_HEADER = "Cache-Control: no-cache\r\n\
Pragma: no-cache\r\n\
Expires: Thu, 16 Oct 1997 06:00:00 GMT\r\n\
X-Powered-By: GuanglingScoreHelper\r\n\
Content-Type: text/html; charset=gb2312\r\n\r\n";

// ��ҳ HTTP ����
const char* REQUEST_HOME_PAGE = "\
GET / HTTP/1.0\r\n\
Host: 58.220.248.249\r\n\
User-Agent: iEdon-GuanglingScoreHelper\r\n\
Accept: */*\r\n\
Connection: close\r\n\r\n";

// ��ҳ HTTP ���󣨴� COOKIE ��
const char* REQUEST_HOME_PAGE_WITH_COOKIE = "\
GET / HTTP/1.0\r\n\
Host: 58.220.248.249\r\n\
User-Agent: iEdon-GuanglingScoreHelper\r\n\
Accept: */*\r\n\
Cookie: %s\r\n\
Connection: close\r\n\r\n";

// ��֤�� HTTP ����
const char* REQUEST_CAPTCHA = "\
GET /validateCodeAction.do?random=0.%d HTTP/1.0\r\n\
Host: 58.220.248.249\r\n\
User-Agent: iEdon-GuanglingScoreHelper\r\n\
Accept: */*\r\n\
Referer: http://58.220.248.249/\r\n\
Cookie: JSESSIONID=%s\r\n\
Connection: close\r\n\r\n";

// �����½
const char * REQUEST_LOGIN = "\
POST /loginAction.do HTTP/1.0\r\n\
Host: 58.220.248.249\r\n\
User-Agent: iEdon-GuanglingScoreHelper\r\n\
Content-Length: %d\r\n\
Cache-Control: max-age=0\r\n\
Origin: http://58.220.248.249\r\n\
Content-Type: application/x-www-form-urlencoded\r\n\
Accept: */*\r\n\
Referer: http://58.220.248.249/\r\n\
Cookie: %s\r\n\
Connection: close\r\n\r\n\
zjh1=&tips=&lx=&evalue=&eflag=&fs=&dzslh=&zjh=%s&mm=%s&v_yzm=%s\
";

// ������
const char * REQUEST_QUERY_SCORE = "\
GET /bxqcjcxAction.do HTTP/1.0\r\n\
Host: 58.220.248.249\r\n\
User-Agent: iEdon-GuanglingScoreHelper\r\n\
Accept: */*\r\n\
Cookie: %s\r\n\
Connection: close\r\n\r\n\
";

// ��ȫ�ǳ�����ϵͳ
const char * REQUEST_LOGOUT = "\
GET /logout.do HTTP/1.0\r\n\
Host: 58.220.248.249\r\n\
User-Agent: iEdon-GuanglingScoreHelper\r\n\
Accept: */*\r\n\
Cookie: %s\r\n\
Connection: close\r\n\r\n\
";

// ��ȡ����ע��ҳ��
const char * REQUEST_GET_REGISTER_INTERFACE = "\
GET /dzzcAction.do HTTP/1.0\r\n\
Host: 58.220.248.249\r\n\
User-Agent: iEdon-GuanglingScoreHelper\r\n\
Accept: */*\r\n\
Cookie: %s\r\n\
Connection: close\r\n\r\n\
";

// �ύ����ע������
const char * REQUEST_POST_REGISTER_INTERFACE = "\
POST /dzzcAction.do?zc=zc&zxjxjhh=%s HTTP/1.0\r\n\
Host: 58.220.248.249\r\n\
User-Agent: iEdon-GuanglingScoreHelper\r\n\
Accept: */*\r\n\
Cookie: %s\r\n\
Content-Length: %d\r\n\
Connection: close\r\n\r\n\
%s\
";

// ������ʾ��
const char *SCORE_TEMPLATE = "<div class=\"content-block-title\">%s</div><div class=\"list-block\"><ul><li class=\"item-content\"><div class=\"item-media\"><i class=\"icon icon-f7\"></i></div><div class=\"item-inner\"><div class=\"item-title\">�ɼ�</div><div class=\"item-after\">%s</div></div></li><li class=\"item-content\"><div class=\"item-media\"><i class=\"icon icon-f7\"></i></div><div class=\"item-inner\"><div class=\"item-title\">ƽ����</div><div class=\"item-after\">%s</div></div></li><li class=\"item-content\"><div class=\"item-media\"><i class=\"icon icon-f7\"></i></div><div class=\"item-inner\"><div class=\"item-title\">��߷�</div><div class=\"item-after\">%s</div></div></li><li class=\"item-content\"><div class=\"item-media\"><i class=\"icon icon-f7\"></i></div><div class=\"item-inner\"><div class=\"item-title\">��ͷ�</div><div class=\"item-after\">%s</div></div></li><li class=\"item-content\"><div class=\"item-media\"><i class=\"icon icon-f7\"></i></div><div class=\"item-inner\"><div class=\"item-title\">����</div><div class=\"item-after\">%s</div></div></li><li class=\"item-content\"><div class=\"item-media\"><i class=\"icon icon-f7\"></i></div><div class=\"item-inner\"><div class=\"item-title\">ѧ��</div><div class=\"item-after\">%s</div></div></li></ul></div>";

// ����ҳ���ʼ��
char *ERROR_HTML = NULL;

void Error( char *p_ErrMsg )
{
	char m_ErrPage[4096] = { 0 };
	sprintf(m_ErrPage, ERROR_HTML, p_ErrMsg);
	char m_ErrMsg[4096] = {0};
	strcat( m_ErrMsg, GLOBAL_HEADER );
	strcat( m_ErrMsg, "\n"); // ��ȫ CGI_HEADER ���з�
	strcat( m_ErrMsg, m_ErrPage );
	cout << m_ErrMsg;
}

const char * base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char * base64_encode(const unsigned char * bindata, char * base64, int binlength)
{
	int i, j;
	unsigned char current;

	for (i = 0, j = 0; i < binlength; i += 3)
	{
		current = (bindata[i] >> 2);
		current &= (unsigned char)0x3F;
		base64[j++] = base64char[(int)current];

		current = ((unsigned char)(bindata[i] << 4)) & ((unsigned char)0x30);
		if (i + 1 >= binlength)
		{
			base64[j++] = base64char[(int)current];
			base64[j++] = '=';
			base64[j++] = '=';
			break;
		}
		current |= ((unsigned char)(bindata[i + 1] >> 4)) & ((unsigned char)0x0F);
		base64[j++] = base64char[(int)current];

		current = ((unsigned char)(bindata[i + 1] << 2)) & ((unsigned char)0x3C);
		if (i + 2 >= binlength)
		{
			base64[j++] = base64char[(int)current];
			base64[j++] = '=';
			break;
		}
		current |= ((unsigned char)(bindata[i + 2] >> 6)) & ((unsigned char)0x03);
		base64[j++] = base64char[(int)current];

		current = ((unsigned char)bindata[i + 2]) & ((unsigned char)0x3F);
		base64[j++] = base64char[(int)current];
	}
	base64[j] = '\0';
	return base64;
}
