#include "Qtype.h"
#include "json/json.h"
std::string strAppKey = "0b164754-fc59-4247-8308-661bfe5acafc";
std::string strAppSecret = "7e87293c0c7641c19007bb24983cd296";

QtypeManager& QtypeManager::It()
{
	static QtypeManager it;
	return it;
}

void QtypeManager::ReqToken()
{
	Json::Value root;
	root["grant_type"] = Json::Value("client_credentials");
	root["client_id"] = Json::Value(strAppKey);
	root["client_secret"] = Json::Value(strAppSecret);
	Json::FastWriter writer;
	std::string strOut = writer.write(root);
	std::cout << strOut << std::endl;

	QtypeAgent* agent = new QtypeAgent("https://api.gotye.com.cn/api/accessToken", strOut);
	agent->SetTag(HTTPTAG_QTYPE_REQTOKEN);
	CHttpClient::It().ReqAgent(agent);
}

void QtypeManager::ReqCreateRoom(unsigned int uDupID)
{
	std::string strUrl = GetUrl() + "/CreateIMRoom";
	Json::Value root;
	root["appkey"] = Json::Value(strAppKey);
	root["roomName"] = Json::Value("²âÊÔ1");
	root["head"] = Json::Value(Json::nullValue);
	root["roomType"] = Json::Value(1);
	root["scope"] = Json::Value(0);
	root["maxUserNumber"] = Json::Value(10);
	Json::FastWriter writer;
	std::string strOut = writer.write(root);
	
	QtypeAgent* agent = new QtypeAgent(strUrl, strOut);
	agent->SetTag(HTTPTAG_QTYPE_REQROOM);

	CHttpClient::It().ReqAgent(agent);
}

bool QtypeAgent::ReceiveComplete(CURLcode code)
{
	Json::Reader reader;
	Json::Value json_object;
	if (!reader.parse(m_strReceiveData, json_object))
		return false;
	int nStatus = json_object["status"].asInt();
	if (nStatus != 200 && nStatus != 0)return false;
	switch (GetTag())
	{
	case HTTPTAG_QTYPE_REQTOKEN:
		{
			std::string strUrl = json_object["api_url"].asString();
			std::string strToken = json_object["access_token"].asString();
			std::cout << strUrl << std::endl;
			std::cout << strToken << std::endl;

			QtypeManager::It().SetUrl(strUrl);
			QtypeManager::It().SetToken(strToken);
			QtypeManager::It().ReqCreateRoom(0);
		}break;
	case HTTPTAG_QTYPE_REQROOM:
		{
			Json::Value room = json_object["entity"];
			if (!room.isObject()){
				break;
			}
			std::string strRoomName = room["roomName"].asString();
			unsigned int uRoomID = room["roomId"].asUInt();
			unsigned int uMaxMember = room["maxUserNumber"].asUInt();
			std::cout << strRoomName << std::endl;
			std::cout << uRoomID << std::endl;
			std::cout << uMaxMember << std::endl;
		}break;
	default:return IAgent::ReceiveComplete(code);
		break;
	}
	return true;
}


QtypeAgent::QtypeAgent(const std::string &strURL, const std::string &strFields)
{
	SetRequestUrl(strURL, strFields);
	if (!QtypeManager::It().GetToken().empty())
	{
		AddHeaderData("Accept:application/json");
		AddHeaderData("Content-Type:application/json");
		AddHeaderData("Authorization:Bearer " + QtypeManager::It().GetToken());
	}
}
