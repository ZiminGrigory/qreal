#include "DFWaitNode.h"
#include "QtCore/QTimer"


using namespace dataFlowBlocks::details;


DFWaitNode::DFWaitNode()
{
		portAssociatedWithProperty["CF_IN"] = 0;
		portAssociatedWithProperty["CF_OUT"] = 1;
		portAssociatedWithProperty["DATA"] = 2;
		portAssociatedWithProperty["OUT"] = 3;
		portAssociatedWithProperty["TIME"] = 4;
}

void DFWaitNode::init()
{
	int time = intProperty("time");
	if (time) {
		timeToWait = time;
	}
}

void DFWaitNode::handleData()
{
	if (!wait) {
		if (hasNewProperty("TIME")) {
			timeToWait = propertyFromPort("TIME").toInt();
		}

		if (hasNewProperty("CF_IN") || hasNewProperty("DATA")) {
			wait = true;
			startWaitingTime = QTime::currentTime();
			if (hasNewProperty("CF_IN")) {
				QTimer::singleShot(timeToWait, this, [&](){
								newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
								wait = false;
						});
			} else {

				mData = propertyFromPort("DATA");

				QTimer::singleShot(timeToWait, this, [&](){
								newDataInFlow(mData, portAssociatedWithProperty["OUT"]);
								newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
								wait = false;
						});
			}

			return;
		}

	} else {
		flushData();
	}
}

void DFWaitNode::flushData()
{
	propertyFromPort("DATA");
	propertyFromPort("TIME");
	propertyFromPort("CF_IN");
}
