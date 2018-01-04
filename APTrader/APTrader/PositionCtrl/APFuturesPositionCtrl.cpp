#include "APFuturesPositionCtrl.h"
#include "../APTradeManager.h"

APFuturesPositionCtrl::APFuturesPositionCtrl()
{
	m_trade = NULL;
}


APFuturesPositionCtrl::~APFuturesPositionCtrl()
{
	if (m_trade != NULL) {
		// trade instance will be destroyed by trade manager
		m_trade = NULL;
	}	
}

void APFuturesPositionCtrl::init(std::string positionInfo)
{
	initWithData(positionInfo);
	m_trade = dynamic_cast<APFuturesTrade*>(APTradeManager::getInstance()->getTradeInstance());
}


void APFuturesPositionCtrl::open(APTradeDirection direction, double price, long volume)
{
	open(m_instrumentID, direction, price, volume);
}

void APFuturesPositionCtrl::close(APTradeDirection direction, double price, long volume)
{
	close(m_instrumentID, direction, price, volume);
}

void APFuturesPositionCtrl::openAll(APTradeDirection direction, double price)
{
	open(direction, price, m_availablePosition);
}

void APFuturesPositionCtrl::closeAll(APTradeDirection direction, double price)
{
	close(direction, price, m_holdPosition);
}

//void APFuturesPositionCtrl::cancel(APTradeType type, double price, long volume)
//{
//}

void APFuturesPositionCtrl::cancel(APTradeType type, double price, APTradeDirection direction)
{
	if (m_trade != NULL) {
		std::list<APORDERID>::iterator it;
		APTradeOrderInfo info;
		if (type == TT_Open) {
			for (it = m_openOrderList.begin(); it != m_openOrderList.end(); ) {			
				if (m_trade->getOrderInfo(*it, info)) {
					if (info.direction == direction) {
						if (fabs(info.price - price) < DBL_EPSILON ||
							(direction == TD_Sell && info.price > price) ||  //Short Open, cancel higher offered price
							(direction == TD_Buy &&  info.price < price)) { // Long Open, cancel lower offered price
							APORDERID orderID = *it;
							std::list<APORDERID>::iterator itNext = ++it;
							m_trade->cancel(orderID, this);
							it = itNext;
							continue;
						}
					}
				}
				it++;
			}
		}
		else if (type == TT_Close) {
			for (it = m_closeOrderList.begin(); it != m_closeOrderList.end();) {
				if (m_trade->getOrderInfo(*it, info)) {
					if (info.direction == direction) {
						if (fabs(info.price - price) < DBL_EPSILON ||
							(direction == TD_Buy  && info.price > price) ||  // Long Close, cancel higher offered price
							(direction == TD_Sell && info.price < price)) { // Short Close, cancel lower offered price
							APORDERID orderID = *it;
							std::list<APORDERID>::iterator itNext = ++it;
							m_trade->cancel(orderID, this);
							it = itNext;
							continue;
						}
					}
				}
				it++;
			}
		}
	}
}

void APFuturesPositionCtrl::cancel(APTradeType type)
{
	if (m_trade != NULL) {
		//m_trade->cancel(m_instrumentID, type, this);
		std::list<APORDERID>::iterator it;
		if (type == TT_Open) {
			for (it = m_openOrderList.begin(); it != m_openOrderList.end(); it++) {
				m_trade->cancel(*it, this);
			}
		}
		else if (type == TT_Close) {
			for (it = m_closeOrderList.begin(); it != m_closeOrderList.end(); it++) {
				m_trade->cancel(*it, this);
			}
		}
	}
}

//void APFuturesPositionCtrl::cancelAll()
//{
//	if (m_trade != NULL) {
//		std::list<APORDERID>::iterator it;
//		for (it = m_openOrderList.begin(); it != m_openOrderList.end(); it++) {
//			m_trade->cancel(*it, this);
//		}
//		for (it = m_closeOrderList.begin(); it != m_closeOrderList.end(); it++) {
//			m_trade->cancel(*it, this);
//		}
//	}
//}

void APFuturesPositionCtrl::onTradeDealt(APASSETID instrumentID, APTradeType type,  double price, long deltaVolume, APORDERID orderID, APTradeDirection direction)
{
	switch (type) {
	case TT_Open:
		m_holdPosition += deltaVolume;
		m_openOrdersPosition -= deltaVolume;
		break;
	case TT_Close:
		m_availablePosition += deltaVolume;
		m_closeOrdersPosition -= deltaVolume;
		break;
	default:
		break;
	}
}

void APFuturesPositionCtrl::onTradeCanceled(APASSETID instrumentID, APTradeType type, long volume, APORDERID orderID, APTradeDirection direction)
{
	switch (type) {
	case TT_Open:
		m_availablePosition += volume;
		m_openOrdersPosition -= volume;
		m_openOrderList.remove(orderID);
		break;
	case TT_Close:
		m_holdPosition += volume;
		m_closeOrdersPosition -= volume;
		m_closeOrderList.remove(orderID);
		break;
	default:
		break;
	}
}

void APFuturesPositionCtrl::setInstrumentID(APASSETID instrumentID)
{
	m_instrumentID = instrumentID;
}

void APFuturesPositionCtrl::setContractType(APTradeDirection direction)
{
	m_directionType = direction;
}

void APFuturesPositionCtrl::open(APASSETID instrumentID, APTradeDirection direction, 
									APOrderPriceType orderPriceType, double price, 
									APOrderTimeCondition orderTimeCondition, std::string date, 
									APOrderVolumeCondition orderVolumeCondition, long volume, long minVolume, 
									APOrderContingentCondition orderContingentCondition, double stopPrice)
{
	if (m_trade != NULL) {
		APORDERID orderID = m_trade->open(instrumentID, direction,
			orderPriceType, price, this,
			orderTimeCondition, date,
			orderVolumeCondition, volume, minVolume,
			orderContingentCondition, stopPrice);
		m_openOrderList.push_back(orderID);
	}
}

void APFuturesPositionCtrl::close(APASSETID instrumentID, APTradeDirection direction, 
									APOrderPriceType orderPriceType, double price, 
									APOrderTimeCondition orderTimeCondition, std::string date, 
									APOrderVolumeCondition orderVolumeCondition, long volume, long minVolume, 
									APOrderContingentCondition orderContingentCondition, double stopPrice)
{
	if (m_trade != NULL) {
		APORDERID orderID = m_trade->close(instrumentID, direction,
			orderPriceType, price, this,
			orderTimeCondition, date,
			orderVolumeCondition, volume, minVolume,
			orderContingentCondition, stopPrice);
		m_closeOrderList.push_back(orderID);
	}
}

void APFuturesPositionCtrl::open(APASSETID instrumentID, APTradeDirection direction, double price, long volume, APOrderTimeCondition ot)
{
	if (m_trade != NULL) {
		APORDERID orderID = m_trade->open(instrumentID, direction, price, volume, this, ot);
		m_openOrderList.push_back(orderID);		
	}
}

void APFuturesPositionCtrl::close(APASSETID instrumentID, APTradeDirection direction, double price, long volume, APOrderTimeCondition ot)
{
	if (m_trade != NULL) {
		APORDERID orderID = m_trade->close(instrumentID, direction, price, volume, this, ot);
		m_closeOrderList.push_back(orderID);		
	}
}





