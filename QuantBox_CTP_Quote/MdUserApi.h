#pragma once

#include "../include/CTP/ThostFtdcMdApi.h"
#include "../include/ApiStruct.h"

#include <set>
#include <string>
#include <atomic>
#include <mutex>

using namespace std;

class CMdUserApi :
	public CThostFtdcMdSpi
{
public:
	CMdUserApi(void);
	virtual ~CMdUserApi(void);

	void Register(void* pMsgQueue);
	ConfigInfoField* Config(ConfigInfoField* pConfigInfo);

	void Connect(const string& szPath,
		ServerInfoField* pServerInfo,
		UserInfoField* pUserInfo);
	void Disconnect();

	void Subscribe(const string& szInstrumentIDs, const string& szExchageID);
	void Unsubscribe(const string& szInstrumentIDs, const string& szExchageID);

	void SubscribeQuote(const string& szInstrumentIDs, const string& szExchageID);
	void UnsubscribeQuote(const string& szInstrumentIDs, const string& szExchageID);

private:
	//订阅行情
	void Subscribe(const set<string>& instrumentIDs, const string& szExchageID);
	void SubscribeQuote(const set<string>& instrumentIDs, const string& szExchageID);
	//登录请求
	void ReqUserLogin();

	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

	virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);

	//检查是否出错
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);//将出错消息送到消息队列
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);//不送出错消息

private:
	mutex						m_csMapInstrumentIDs;
	mutex						m_csMapQuoteInstrumentIDs;

	atomic<int>					m_nRequestID;			//请求ID，每次请求前自增

	set<string>					m_setInstrumentIDs;		//正在订阅的合约
	set<string>					m_setQuoteInstrumentIDs;		//正在订阅的合约
	CThostFtdcMdApi*			m_pApi;					//行情API
	void*						m_msgQueue;				//消息队列指针

	string						m_szPath;				//生成配置文件的路径
	set<string>					m_arrAddresses;			//服务器地址
	string						m_szBrokerId;			//期商ID
	string						m_szInvestorId;			//投资者ID
	string						m_szPassword;			//密码
	string						m_szAddresses;
};

