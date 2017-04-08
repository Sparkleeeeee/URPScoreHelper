#pragma once
char * left(char *dst, char *src, int n);
char * mid(char *dst, char *src, int n, int m);
char * right(char *dst, char *src, int n);
void replace_string(char * source_str, char * targ_str, char *val);
static int htoi(char *s);
char * url_encode(char const *s, int len, int *new_length);
int url_decode(char *str, int len);