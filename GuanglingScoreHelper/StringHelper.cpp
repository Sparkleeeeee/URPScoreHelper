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
