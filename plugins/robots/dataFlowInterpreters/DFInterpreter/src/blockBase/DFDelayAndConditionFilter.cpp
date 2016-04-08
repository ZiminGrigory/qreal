#include "DFDelayAndConditionFilter.h"

#include <QtCore/QTimer>

using namespace dataFlowBlocks::details;


const int noCounterFilter = -1;

DFDelayAndConditionFilter::DFDelayAndConditionFilter()
	: delayTime(intProperty("dispatchTime"))
	, count(intProperty("count"))
	, condition(intProperty("condition"))
	, vars("vars = {}")
	, mTime()
{
	//	QMap<int, QVariant> valueOnPort;
	//	QMap<QString, int> portAssociatedWithProperty;
		portAssociatedWithProperty["DISPATCH_TIME"] = 0;
		portAssociatedWithProperty["FAILURE"] = 1;
		portAssociatedWithProperty["COUNT"] = 2;
		portAssociatedWithProperty["OUT"] = 3;
		portAssociatedWithProperty["DATA"] = 4;
		portAssociatedWithProperty["VARS"] = 5;
}

void DFDelayAndConditionFilter::handleData()
{
	if (hasNewProperty("DISPATCH_TIME")) {
		delayTime = propertyFromPort("DISPATCH_TIME").toInt();
	}

	if (hasNewProperty("COUNT")) {
		count = propertyFromPort("COUNT").toInt();
	}

	if (hasNewProperty("VARS")) {
		vars.replace("{}", qVariantListToLuaArrayInitializeList(propertyFromPort("VARS").value<QVariantList>()));
	}

	if (hasNewProperty("DATA")) {

		if (count == 0) {
			flushData();
			return;
		}

		if (mTime.isNull() && QTime::currentTime().msecsTo(mTime) < delayTime) {
			flushData();
			return;
		}

		QVariant rawData = propertyFromPort("DATA");
		QString data = "data = ";
		if (rawData.canConvert<QVariantList>()) {
			data += qVariantListToLuaArrayInitializeList(rawData.value<QVariantList>());
		} else {
			data += rawData.toString();
		}

		bool cond = evalCode<bool>(data + ";" + vars + ";" + condition, "condition");

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
