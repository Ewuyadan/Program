#include <stdio.h>
#include "Qtype.h"
#include "json/json.h"
#pragma comment(lib,"libcurl_imp.lib")

int main(void)
{
	CHttpClient::It().Init();
	QtypeManager::It().ReqToken();
	while (true)
	{
		CHttpClient::It().Update();
		//ProcessMsg();
	}
}