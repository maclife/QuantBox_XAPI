#pragma once

#include "../include/XSpeed_Stock/DFITCSECTraderApi.h"
#include "../include/ApiStruct.h"

#ifdef _WIN64
#pragma comment(lib, "../include/XSpeed/win64/DFITCTraderApi.lib")
#pragma comment(lib, "../lib/QuantBox_Queue_x64.lib")
#else
#pragma comment(lib, "../include/XSpeed_Stock/win32/DFITCSECTraderApi.lib")
#pragma comment(lib, "../lib/QuantBox_Queue_x86.lib")
#endif

#include <set>
#include <list>
#include <map>
#include <string>
#include <mutex>
#include <atomic>
#include <thread>
#include <unordered_map>

using namespace std;

class CMsgQueue;

class CTraderApi :
	public DFITCSECTraderSpi
{
	//请求数据包类型
	enum RequestType
	{
		E_Init,
		E_StockUserLoginField,
		E_SOPUserLoginField,
		E_FASLUserLoginField,

		E_AbiInstrumentField,
		E_CancelOrderField,
		E_CapitalField,
		E_ExchangeInstrumentField,
		
		E_InsertOrderField,
		E_MatchField,
		E_OrderField,
		E_PositionField,
		E_PositionDetailField,
		E_SpecificInstrumentField,
		
		E_QuoteSubscribeField,
		E_QuoteUnSubscribeField,
		E_QuoteInsertField,
		E_QuoteCancelField,
	};

public:
	CTraderApi(void);
	virtual ~CTraderApi(void);

	void Register(void* pCallback, void* pClass);

	void Connect(const string& szPath,
		ServerInfoField* pServerInfo,
		UserInfoField* pUserInfo);
	void Disconnect();

	int ReqOrderInsert(
		OrderField* pOrder,
		int count,
		OrderIDType* pInOut);

	//int ReqParkedOrderInsert(int OrderRef,
	//	OrderField* pOrder1,
	//	OrderField* pOrder2);

	/*int ReqOrderAction(const string& szId);
	void ReqCancelOrder(
		const string& szInstrumentId,
		DFITCLocalOrderIDType localOrderID,
		DFITCSPDOrderIDType spdOrderID);

	int ReqQuoteInsert(
		int QuoteRef,
		OrderField* pOrderAsk,
		OrderField* pOrderBid);

	int ReqQuoteAction(const string& szId);
	void ReqQuoteCancelOrder(
		const string& szInstrumentId,
		DFITCLocalOrderIDType localOrderID,
		DFITCSPDOrderIDType spdOrderID);*/


	//void ReqQryMatchInfo(DFITCInstrumentTypeType instrumentType);
	//void ReqQryOrderInfo(DFITCInstrumentTypeType instrumentType);
	//void ReqQryCustomerCapital();
	//void ReqQryPosition(const string& szInstrumentId);
	//void ReqQryPositionDetail(const string& szInstrumentId);
	//void ReqQryExchangeInstrument(const string& szExchangeId, DFITCInstrumentTypeType instrumentType);
	//void ReqQryArbitrageInstrument(const string& szExchangeId);
	//void ReqQrySpecifyInstrument(const string& szInstrumentId, const string& szExchangeId, DFITCInstrumentTypeType instrumentType);
	//void ReqQuoteSubscribe(const string& szExchangeId, DFITCInstrumentTypeType instrumentType);
	//void ReqQuoteUnSubscribe(const string& szExchangeId, DFITCInstrumentTypeType instrumentType);

private:
	friend void* __stdcall Query(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);
	void QueryInThread(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);

	void Clear();

	int _Init();

	void ReqStockUserLogin();
	int _ReqStockUserLogin(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);
	void ReqSOPUserLogin();
	int _ReqSOPUserLogin(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);
	void ReqFASLUserLogin();
	int _ReqFASLUserLogin(char type, void* pApi1, void* pApi2, double double1, double double2, void* ptr1, int size1, void* ptr2, int size2, void* ptr3, int size3);


	int ReqInit();

	//检查是否出错
	bool IsErrorRspInfo_Output(struct DFITCSECRspInfoField *pRspInfo);//将出错消息送到消息队列
	bool IsErrorRspInfo(struct DFITCSECRspInfoField *pRspInfo); //不输出信息

	//连接
	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);

	//认证
	virtual void OnRspStockUserLogin(DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo);
	virtual void OnRspSOPUserLogin(DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo);
	virtual void OnRspFASLUserLogin(DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo);


	//virtual void OnRspUserLogin(struct DFITCUserLoginInfoRtnField *pRspUserLogin, struct DFITCErrorRtnField *pRspInfo);
	//virtual void OnRspUserLogout(struct DFITCUserLogoutInfoRtnField * pUserLogoutInfoRtn, struct DFITCErrorRtnField * pErrorInfo){};
	//下单
	//virtual void OnRspInsertOrder(struct DFITCOrderRspDataRtnField * pOrderRtn, struct DFITCErrorRtnField * pErrorInfo);
	//virtual void OnRtnOrder(struct DFITCOrderRtnField * pRtnOrderData);

	//virtual void OnRspQryOrderInfo(struct DFITCOrderCommRtnField * pRtnOrderData, struct DFITCErrorRtnField * pErrorInfo, bool bIsLast){};

	////撤单
	//virtual void OnRspCancelOrder(struct DFITCOrderRspDataRtnField *pOrderCanceledRtn, struct DFITCErrorRtnField *pErrorInfo);
	//virtual void OnRtnCancelOrder(struct DFITCOrderCanceledRtnField * pCancelOrderData){};

	////成交回报
	//virtual void OnRspQryMatchInfo(struct DFITCMatchedRtnField * pRtnMatchData, struct DFITCErrorRtnField * pErrorInfo, bool bIsLast){};
	//virtual void OnRtnMatchedInfo(struct DFITCMatchRtnField * pRtnMatchData);

	////仓位
	//virtual void OnRspQryPosition(struct DFITCPositionInfoRtnField * pPositionInfoRtn, struct DFITCErrorRtnField * pErrorInfo, bool bIsLast){};
	//virtual void OnRspQryPositionDetail(struct DFITCPositionDetailRtnField * pPositionDetailRtn, struct DFITCErrorRtnField * pErrorInfo, bool bIsLast){};

	////资金
	//virtual void OnRspCustomerCapital(struct DFITCCapitalInfoRtnField * pCapitalInfoRtn, struct DFITCErrorRtnField * pErrorInfo, bool bIsLast);

	////合约、手续费
	//virtual void OnRspQryExchangeInstrument(struct DFITCExchangeInstrumentRtnField * pInstrumentData, struct DFITCErrorRtnField * pErrorInfo, bool bIsLast);
	//virtual void OnRspArbitrageInstrument(struct DFITCAbiInstrumentRtnField * pAbiInstrumentData, struct DFITCErrorRtnField * pErrorInfo, bool bIsLast);
	//virtual void OnRspQrySpecifyInstrument(struct DFITCInstrumentRtnField * pInstrument, struct DFITCErrorRtnField * pErrorInfo, bool bIsLast){};

	//////其它
	//virtual void OnRtnErrorMsg(struct DFITCErrorRtnField *pErrorInfo){};
	//virtual void OnRtnExchangeStatus(struct DFITCExchangeStatusRtnField * pRtnExchangeStatusData){};

	//virtual void OnRspResetPassword(struct DFITCResetPwdRspField * pResetPassword, struct DFITCErrorRtnField * pErrorInfo){};
	//virtual void OnRspQryTradeCode(struct DFITCQryTradeCodeRtnField * pTradeCode, struct DFITCErrorRtnField * pErrorInfo, bool bIsLast){};
	//virtual void OnRspBillConfirm(struct DFITCBillConfirmRspField * pBillConfirm, struct DFITCErrorRtnField * pErrorInfo){};
	//virtual void OnRspEquityComputMode(struct DFITCEquityComputModeRtnField * pEquityComputMode){};
	//virtual void OnRspQryBill(struct DFITCQryBillRtnField *pQryBill, struct DFITCErrorRtnField * pErrorInfo, bool bIsLast){};
	//virtual void OnRspConfirmProductInfo(struct DFITCProductRtnField * pProductRtnData){};
	//virtual void OnRspTradingDay(struct DFITCTradingDayRtnField * pTradingDayRtnData){};

	//// 做市商
	//virtual void OnRspQuoteSubscribe(struct DFITCQuoteSubscribeRspField * pRspQuoteSubscribeData);
	//virtual void OnRtnQuoteSubscribe(struct DFITCQuoteSubscribeRtnField * pRtnQuoteSubscribeData);
	//virtual void OnRspQuoteInsert(struct DFITCQuoteRspField * pRspQuoteData, struct DFITCErrorRtnField * pErrorInfo){};
	//virtual void OnRtnQuote(struct DFITCQuoteRtnField * pRtnQuoteData){};
	//virtual void OnRspQuoteCancel(struct DFITCQuoteRspField * pRspQuoteCanceledData, struct DFITCErrorRtnField * pErrorInfo){};
	//virtual void OnRtnQuoteCancel(struct DFITCQuoteCanceledRtnField * pRtnQuoteCanceledData){};

private:
	atomic<long>				m_lRequestID;			//请求ID,得保持自增

	DFITCSECRspUserLoginField	m_RspUserLogin;			//返回的登录成功响应，目前利用此内成员进行报单所属区分
	OrderIDType					m_orderInsert_Id;

	mutex						m_csOrderRef;
	int							m_nMaxOrderRef;			//报单引用，用于区分报单，保持自增

	DFITCSECTraderApi*			m_pApi;					//交易API
	
	string						m_szPath;				//生成配置文件的路径
	ServerInfoField				m_ServerInfo;
	UserInfoField				m_UserInfo;

	int							m_nSleep;
	

	unordered_map<string, OrderField*>				m_id_platform_order;
	//unordered_map<string, DFITCOrderRtnField*>		m_id_api_order;
	unordered_map<string, string>					m_sysId_orderId;

	//unordered_map<string, QuoteField*>				m_id_platform_quote;
	//unordered_map<string, DFITCQuoteRtnField*>		m_id_api_quote;
	unordered_map<string, string>					m_sysId_quoteId;

	CMsgQueue*					m_msgQueue;				//消息队列指针
	CMsgQueue*					m_msgQueue_Query;
	void*						m_pClass;
};

