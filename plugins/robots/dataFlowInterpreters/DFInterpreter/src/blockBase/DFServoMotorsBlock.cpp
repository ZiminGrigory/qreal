#include "DFServoMotorsBlock.h"


using namespace dataFlow::interpretation;
using namespace kitBase;
using namespace kitBase::blocksBase;
using namespace kitBase::robotModel;
using namespace dataFlowBlocks::details;

/// @todo: rewrite this: intersect with motors, add hybrid device (M1+M2 and so on)

const QStringList ports = {"S1", "S2", "S3", "S4"};

DFServoMotorsBlock::DFServoMotorsBlock(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["CF_OUT"] = 1;
	portAssociatedWithProperty["S1"] = 2;
	portAssociatedWithProperty["S2"] = 3;
	portAssociatedWithProperty["S3"] = 4;
	portAssociatedWithProperty["S4"] = 5;
}

void DFServoMotorsBlock::handleData()
{
	if (hasNewData(portAssociatedWithProperty["CF_IN"])) {
		property(portAssociatedWithProperty["CF_IN"]);
		for (const QString &port : ports) {
			motors[port]->on(intProperty(port + "Angle"));
		}
	} else {
		for (const QString &port : ports) {
			if (hasNewData(portAssociatedWithProperty[port])) {
				QVariant power = property(portAssociatedWithProperty[port]);
				motors[port]->on(power.toInt());
			}
		}
	}

	emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);

}

bool DFServoMotorsBlock::isSynchronizationNeeded()
{
	return property("synch").toBool();
}

void DFServoMotorsBlock::init()
{
	const QList<PortInfo> keys = usedDevices().keys();

	for (const PortInfo &key : keys) {
		motors.insert(key.name(), robotModel::RobotModelUtils::findDevice<robotParts::Motor>(mRobotModel, key));
	}
}

QMap<PortInfo, DeviceInfo> DFServoMotorsBlock::usedDevices()
{
	QMap<PortInfo, DeviceInfo> result;
	for (const PortInfo &port : parsePorts(ReportErrors::doNotReport)) {
		result[port] = DeviceInfo::create<robotParts::Motor>();
	}

	return result;
}

QList<PortInfo> DFServoMotorsBlock::parsePorts(ReportErrors reportErrors)
{
	Q_UNUSED(reportErrors);
	QList<PortInfo> result;

	for (const QString &port : ports) {
		PortInfo const portInfo = robotModel::RobotModelUtils::findPort(mRobotModel, port, output);
		if (portInfo.isValid()) {
			result << portInfo;
		}
	}

	return result;
}
