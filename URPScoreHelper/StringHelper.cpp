#include "stdafx.h"
#include "StringHelper.h"

// ȡ�ı����
char * left(char *dst, char *src, int n)
{
	char	*p = src;
	char	*q = dst;
	int	len = strlen(src);
	if (n > len)
		n = len;
	while (n--)
		*(q++) = *(p++);
	*(q++) = '\0';                                  /*�б�Ҫ�𣿺��б�Ҫ*/
	return(dst);
}

// ȡ�ı��м�
char * mid(char *dst, char *src, int n, int m)        /*nΪ���ȣ�mΪλ��*/
{
	char	*p = src;
	char	*q = dst;
	int	len = strlen(src);
	if (n > len)
		n = len - m;                            /*�ӵ�m�������*/
	if (m < 0)
		m = 0;                                  /*�ӵ�һ����ʼ*/
	if (m > len)
		return(NULL);
	p += m;
	while (n--)
		*(q++) = *(p++);
	*(q++) = '\0';                                  /*�б�Ҫ�𣿺��б�Ҫ*/
	return(dst);
}

// ȡ�ı��ұ�
char * right(char *dst, char *src, int n)
{
	char	*p = src;
	char	*q = dst;
	int	len = strlen(src);
	if (n > len)
		n = len;
	p += (len - n); /*���ұߵ�n���ַ���ʼ����0���������ɰ�*/
	while (*(q++) = *(p++))
		;
	return(dst);
}
// ���ַ�����ָ�����ַ�����ָ���ַ������棬targ_str �Ǳ��滻�ģ�val���滻���ַ���
void replace_string(char * source_str, char * targ_str, char *val)
{
	char temp_sstr[513], result[513];
	char * p, *q;
	int len; len = 0; q = p = NULL;
	memset(result, 0, sizeof(result));
	memset(temp_sstr, 0, sizeof(temp_sstr));
	strcpy(temp_sstr, source_str);
	p = q = temp_sstr;
	len = strlen(targ_str);
	while (q != NULL)
	{
		if ((q = strstr(p, targ_str)) != NULL)
		{
			strncat(result, p, q - p);
			strcat(result, val);
			strcat(result, "\0");
			q += len;
			p = q;
		}
		else
			strcat(result, p);
	}
	strcpy(source_str, result);
}

static unsigned char hexchars[] = "0123456789ABCDEF";

static int htoi(char *s)
{
	int value;
	int c;

	c = ((unsigned char *)s)[0];
	if (isupper(c))
		c = tolower(c);
	value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

	c = ((unsigned char *)s)[1];
	if (isupper(c))
		c = tolower(c);
	value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

	return (value);
}

char * url_encode(char const *s, int len, int *new_length)
{
	register unsigned char c;
	unsigned char *to, *start;
	unsigned char const *from, *end;

	from = (unsigned char *)s;
	end = (unsigned char *)s + len;
	start = to = (unsigned char *)calloc(1, 3 * len + 1);

	while (from < end)
	{
		c = *from++;

		if (c == ' ')
		{
			*to++ = '+';
		}
		else if ((c < '0' && c != '-' && c != '.') ||
			(c < 'A' && c > '9') ||
			(c > 'Z' && c < 'a' && c != '_') ||
			(c > 'z'))
		{
			to[0] = '%';
			to[1] = hexchars[c >> 4];
			to[2] = hexchars[c & 15];
			to += 3;
		}
		else
		{
			*to++ = c;
		}
	}
	*to = 0;
	if (new_length)
	{
		*new_length = to - start;
	}
	return (char *)start;
}


int url_decode(char *str, int len)
{
	char *dest = str;
	char *data = str;

	while (len--)
	{
		if (*data == '+')
		{
			*dest = ' ';
		}
		else if (*data == '%' && len >= 2 && isxdigit((int) *(data + 1)) && isxdigit((int) *(data + 2)))
		{
			*dest = (char)htoi(data + 1);
			data += 2;
			len -= 2;
		}
		else
		{
			*dest = *data;
		}
		data++;
		dest++;
	}
	*dest = '/0';
	return dest - str;
}