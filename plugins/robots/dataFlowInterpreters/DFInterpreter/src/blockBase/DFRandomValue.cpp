#include "DFRandomValue.h"

#include <kitBase/robotModel/robotParts/random.h>
#include <kitBase/robotModel/robotModelUtils.h>

#include <QTime>

using namespace kitBase::robotModel;

dataFlowBlocks::details::DFRandomValue::DFRandomValue(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["OUT"] = 1;
}

void dataFlowBlocks::details::DFRandomValue::handleData()
{
	int from = intProperty("fromNumber");
	int to = eval<int>("toNumber");

	const auto random = RobotModelUtils::findDevice<robotParts::Random>(mRobotModel, "RandomPort");

	const int result = random->random(from, to);
	qDebug () << "random " << result << " " << QTime::currentTime();

	emit newDataInFlow(result, portAssociatedWithProperty["OUT"]);
}
