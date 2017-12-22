#include "APMonitorFramework.h"
#include "../APMarco.h"
#include "../Utils/APLog.h"

#ifdef USE_CTP
#include "../Impl/APFuturesCTPMDAgent.h"
#endif

#include "../APMarketDataManager.h"
#include "APQuotationMonitorManager.h"

void onRemoteSystemInited() {
	APMonitorFramework::getInstance()->onRemoteInited();
}

APMonitorFramework::APMonitorFramework()
{
}


APMonitorFramework::~APMonitorFramework()
{
}

void APMonitorFramework::init()
{
	m_inited = false;

	initRemoteSystem();
}

void APMonitorFramework::update(float deltaTime)
{
	APMarketDataMgr->update(deltaTime);
	APQuotationMonitorManager::getInstance()->update();
}

void APMonitorFramework::exit()
{
#ifdef USE_CTP
	//APFuturesCTPMDAgent::getInstance()->
#endif // USE_CTP

	APQuotationMonitorManager::getInstance()->destroy();
}

bool APMonitorFramework::inited()
{
	return m_inited;
}

void APMonitorFramework::initRemoteSystem()
{
#ifdef USE_CTP
	APFuturesCTPMDAgent::getInstance()->registerInitCallback(onRemoteSystemInited);
	APFuturesCTPMDAgent::getInstance()->init();
#endif // USE_CTP

}

void APMonitorFramework::onRemoteInited()
{
#ifdef USE_CTP
	APLogger->log("CTP inited ----");
#endif
	initLocalSystem();
}

void APMonitorFramework::initLocalSystem()
{
	APMarketDataMgr->init();
	APQuotationMonitorManager::getInstance()->init();
}

void APMonitorFramework::onLocalInited()
{
	m_inited = true;
}