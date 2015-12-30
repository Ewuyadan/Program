#pragma once

#include "HttpClient.h"


class QtypeAgent :public IAgent
{
public:
	QtypeAgent(const std::string &strURL, const std::string &strFields);

	virtual bool ReceiveComplete(CURLcode);
};

class QtypeManager
{
public:
	static QtypeManager& It();

	void ReqToken();
	void ReqCreateRoom(unsigned int uDupID);
	
	std::string GetToken(){ return m_strToken; }
	void SetToken(std::string strToken){ m_strToken = strToken; }

	std::string GetUrl(){ return m_strUrl; }
	void SetUrl(std::string strUrl){ m_strUrl = strUrl; }


private:
	std::string			m_strToken;
	std::string			m_strUrl;
};