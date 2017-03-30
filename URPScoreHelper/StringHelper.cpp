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