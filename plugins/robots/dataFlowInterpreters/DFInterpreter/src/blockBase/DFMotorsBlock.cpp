#include "DFMotorsBlock.h"


using namespace dataFlow::interpretation;
using namespace kitBase;
using namespace kitBase::blocksBase;
using namespace kitBase::robotModel;
using namespace dataFlowBlocks::details;

/// @todo: rewrite this SH~

const QStringList ports = {"M1", "M2", "M3", "M4"};

DFMotorsBlock::DFMotorsBlock(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
//	QMap<int, QVariant> valueOnPort;
//	QMap<QString, int> portAssociatedWithProperty;
//	QSet<int> synchronisedPorts;
	// associate port number and port name
	portAssociatedWithProperty["M1"] = 0;
	portAssociatedWithProperty["M2"] = 1;
	portAssociatedWithProperty["M3"] = 3;
	portAssociatedWithProperty["M4"] = 4;
	portAssociatedWithProperty["CF_OUT"] = 2;
}

void DFMotorsBlock::handleData()
{
	for (const QString &port : ports) {
		if (hasNewProperty(portAssociatedWithProperty[port])) {
			QVariant power = property(portAssociatedWithProperty[port]);
			motors[port]->on(power.toInt());
		}
	}

	emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);

}

bool DFMotorsBlock::isSynchronizationNeeded()
{
	return property("synch").toBool();
}

void DFMotorsBlock::init()
{
	const QList<PortInfo> keys = usedDevices().keys();

	for (const PortInfo &key : keys) {
		motors.insert(key.name(), robotModel::RobotModelUtils::findDevice<robotParts::Motor>(mRobotModel, key));
	}
}

QMap<PortInfo, DeviceInfo> DFMotorsBlock::usedDevices()
{
	QMap<PortInfo, DeviceInfo> result;
	for (const PortInfo &port : parsePorts(ReportErrors::doNotReport)) {
		result[port] = DeviceInfo::create<robotParts::Motor>();
	}

	return result;
}

QList<PortInfo> DFMotorsBlock::parsePorts(ReportErrors reportErrors)
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
