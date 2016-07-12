#include "DFRandomValue.h"

#include <kitBase/robotModel/robotModelUtils.h>

#include <QtCore/QTime>
#include <QApplication>


using namespace kitBase::robotModel;

static bool isDestoyed = false;

dataFlowBlocks::details::DFRandomValue::DFRandomValue(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
	isDestoyed = false;
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["OUT"] = 1;
}

dataFlowBlocks::details::DFRandomValue::~DFRandomValue()
{
	isDestoyed = true;
}

void dataFlowBlocks::details::DFRandomValue::init()
{
	repeatedly = boolProperty("repeatedly");
	sendingTime  = intProperty("time");
	if (sendingTime <= 0) {
		error("Dispatch time for Random generator must be greater than zero");
	}

	mRandom = RobotModelUtils::findDevice<robotParts::Random>(mRobotModel, "RandomPort");
}

void dataFlowBlocks::details::DFRandomValue::handleData()
{
	int from = intProperty("fromNumber");
	int to = eval<int>("toNumber");

	if (repeatedly) {
		while (!isDestoyed) {
			QTime pendingTime = QTime::currentTime().addMSecs(sendingTime);
			while (!isDestoyed && QTime::currentTime() < pendingTime) {
				QCoreApplication::processEvents(QEventLoop::AllEvents, sendingTime / 3);
			}

			if (!isDestoyed) {
				const int result = mRandom->random(from, to);
				emit newDataInFlow(result, portAssociatedWithProperty["OUT"]);
			}
		}
	} else {
		const int result = mRandom->random(from, to);
		emit newDataInFlow(result, portAssociatedWithProperty["OUT"]);
	}
}
