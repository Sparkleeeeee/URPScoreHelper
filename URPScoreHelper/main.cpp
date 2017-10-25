/*
******************************************
********** iEdon URPScoreHelper **********
**********  C MVC - Controller  **********
**********  Copyright (C) iEdon **********
******************************************
****  Project encoding must be UTF-8  ****
******************************************
*/

#include "stdafx.h"
#include "main.h"
#include "General.h"
#include "gbkutf8.h"

int main(int argc, const char *argv[])
{
	LoadConfig();
	str_normalize_init();
	FCGX_Init();
	curl_global_init(CURL_GLOBAL_ALL);
	isPageSrcLoadSuccess = false;

	int FCGX_SocketId = 0;
	if (argc == 3)
	{
		if (strcmp(argv[1], "-p") == 0)
		{
			FCGX_SocketId = FCGX_OpenSocket(argv[2], 5);
			if (FCGX_SocketId == -1)
				FCGX_SocketId = 0;
		}
	}

	int ret = 0;
	bool isFirst = true;
	while (FCGX_InitRequest(&request, FCGX_SocketId, 0) == 0)
	{
		ret = app_intro();
		if (isFirst && ret != 0)
		{
			break;
		}
		isFirst = false;
		FCGX_Finish_r(&request);
		memset(&request, 0, sizeof(FCGX_Request));
	}

	printf("%s\n%s\n\n%s\n", SOFTWARE_NAME, SOFTWARE_COPYRIGHT, "\tOptions: [-p (localhost):port_number]");
	
	free(HEADER_TEMPLATE_LOCATION);
	free(FOOTER_TEMPLATE_LOCATION);
	free(SERVER_URL);
	free(USER_AGENT);
	free(OAUTH2_APPID);
	free(OAUTH2_SECRET);
	free(CURL_PROXY_URL);
	free(APP_NAME);
	free(CARD_AD_BANNER_1_IMG);
	free(CARD_AD_BANNER_2_IMG);
	free(CARD_AD_BANNER_1_URL);
	free(CARD_AD_BANNER_2_URL);
	free(ADMIN_USER_NAME);
	free(ADMIN_PASSWORD);
	free(SECONDARY_TITLE);
	free(APP_KEYWORDS);
	free(APP_DESCRIPTION);
	free(FOOTER_TEXT);
	free(ANALYSIS_CODE);
	sqlite3_close(db);
	curl_global_cleanup();
	return 0;
}