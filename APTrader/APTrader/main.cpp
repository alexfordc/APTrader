#include "APMarco.h"
#include "Strategy/APStandardGridStrategy.h"
#include "APFramework.h"
#include "APStrategyManager.h"
#include "APStrategyFactory.h"
#include <windows.h>
#include "APGlobalConfig.h"
#include "Utils/APJsonReader.h"
//#include "Simulation/APSimFramework.h"
#include "Impl/APFuturesCTPMDAgent.h"
#include "Quotation/APFuturesMarketQuotations.h"
#include "Monitor/APMonitorFramework.h"
#include <thread>

void frameworkLoop() {
	APMonitorFramework* framework = APMonitorFramework::getInstance();

	while (!framework->inited()) {
		Sleep(100);
	}

	while (!framework->finished()) {
		framework->update(0.0);
		Sleep(10);
	}
}

void main() {

	//APSimFramework* framework = new APSimFramework();
	//if (framework != NULL)
	//{
	//	framework->init();

	//	//APStandardGridStrategy* gs = (APStandardGridStrategy*)APStrategyManager::getInstance()->createStrategy("Data/grid0.stg");
	//	//gs->printGrids();
	//	
	//	while (!framework->finished()) {
	//		framework->update();
	//		Sleep(300);
	//	}
	//}
	//framework->exit();
	//delete framework;
	
	// ---- monitor ----
	APMonitorFramework* framework = APMonitorFramework::getInstance();
	std::thread frameworkThread(frameworkLoop);
	framework->init();
	
	//frameworkThread.join();
	

	while (!framework->inited()) {
		//int a = 0;
	}

	while (!framework->finished()) {
		framework->update(0.0);
	}

	framework->exit();

	printf("0");
}