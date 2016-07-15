#include "DFDelayAndConditionFilter.h"

#include <QtCore/QTimer>

using namespace dataFlowBlocks::details;


const int noCounterFilter = -1;

DFDelayAndConditionFilter::DFDelayAndConditionFilter()
	: condition()
{
	//	QMap<int, QVariant> valueOnPort;
	//	QMap<QString, int> portAssociatedWithProperty;
		portAssociatedWithProperty["DISPATCH_TIME"] = 0;
		portAssociatedWithProperty["FAILURE"] = 1;
		portAssociatedWithProperty["COUNT"] = 2;
		portAssociatedWithProperty["OUT"] = 3;
		portAssociatedWithProperty["DATA"] = 4;
}

void DFDelayAndConditionFilter::init()
{
	delayTime = intProperty("dispatchTime");
	count = intProperty("count");
	condition = stringProperty("condition");
}

void DFDelayAndConditionFilter::handleData()
{
	if (hasNewData("DISPATCH_TIME")) {
		delayTime = propertyFromPort("DISPATCH_TIME").toInt();
	}

	if (hasNewData("COUNT")) {
		count = propertyFromPort("COUNT").toInt();
	}

	if (hasNewData("DATA")) {

		if (count == 0) {
			flushData();
			emit newDataInFlow(QVariant(), portAssociatedWithProperty["FAILURE"]);
			return;
		}

		if (!mTime.isNull() && mTime.msecsTo(QTime::currentTime()) < delayTime) {
			flushData();
			return;
		}

		QVariant rawData = propertyFromPort("DATA");
		QString data = "data";
		setVariable(data, rawData);

		bool cond = evalCode<bool>(condition, "condition");

		if (cond) {
			count = count != noCounterFilter ? count - 1 : count;
			mTime = QTime::currentTime();
			emit newDataInFlow(rawData, portAssociatedWithProperty["OUT"]);
		} else {
			emit newDataInFlow(QVariant(), portAssociatedWithProperty["FAILURE"]);
		}

	}
}

void DFDelayAndConditionFilter::flushData()
{
	propertyFromPort("DATA");
}
