#pragma once
#include "../APTrade.h"
#include <vector>

class APSharesTrade : public APTrade
{
public:
	APSharesTrade();
	~APSharesTrade();

public:
	virtual void open(APORDERID orderID, APASSETID instrumentID, APTrendType trend, double price, long volume, APOrderPriceType orderPriceType = OPT_LimitPrice, APOrderTimeCondition ot = OTC_GoodForDay);
	virtual void close(APORDERID orderID, APASSETID instrumentID, APTrendType trend, double price, long volume, APOrderPriceType orderPriceType = OPT_LimitPrice, APOrderTimeCondition ot = OTC_GoodForDay);

	virtual void cancel(APSYSTEMID sysID);

//protected:
//	long buy(APASSETID instrumentID, double price, long volume);
//	long sell(APASSETID instrumentID, double price, long volume);
//
//	void buyOrder(APASSETID instrumentID, double price, long volume);
//	void sellOrder(APASSETID instrumentID, double price, long volume);
//	void cancelOrder(long orderID);
//
//	void onBought(UINT orderID, double price, long volume);
//	void onSold(UINT orderID, double price, long volume);
//	void onCancel(UINT orderID);
//
//	void cancelBuyOrderBeyondPrice(double price);
//	void cancelSellOrderUnderPrice(double price);

protected:
	std::vector<APTradeOrderInfo> m_buyOrders;
	std::vector<APTradeOrderInfo> m_sellOrders;
};

