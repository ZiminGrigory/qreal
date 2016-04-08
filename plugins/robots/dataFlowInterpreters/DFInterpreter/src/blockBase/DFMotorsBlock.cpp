#include "DFMotorsBlock.h"


using namespace dataFlow::interpretation;
using namespace kitBase;
using namespace kitBase::blocksBase;
using namespace kitBase::robotModel;
using namespace dataFlowBlocks::details;


//using namespace kitBase::blocksBase::common;
//using namespace kitBase::robotModel;

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
	const QList<PortInfo> keys = usedDevices().keys();

	for (const PortInfo &key : keys) {
		QVariant power = property(portAssociatedWithProperty[key.name()]);

		auto *motor = robotModel::RobotModelUtils::findDevice<robotParts::Motor>(mRobotModel, key);
		if (power != QVariant()) {
			qDebug() << key.name() << power.toInt() << "\n";
			motor->on(power.toInt());
		}
	}

	if (!mGraphicalModelApi->graphicalRepoApi().outgoingLinks(mGraphicalId).empty()) {
		emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
	}
}


QList<PortInfo> DFMotorsBlock::parsePorts(ReportErrors reportErrors)
{
	Q_UNUSED(reportErrors);
	QList<PortInfo> result;
	QList<QString> ports;
	ports << "M1" << "M2" << "M3" << "M4";

	for (const QString &port : ports) {
		PortInfo const portInfo = robotModel::RobotModelUtils::findPort(mRobotModel, port, output);
		if (portInfo.isValid()) {
			result << portInfo;
		}
	}

	return result;
}


QMap<PortInfo, DeviceInfo> DFMotorsBlock::usedDevices()
{
	QMap<PortInfo, DeviceInfo> result;
	for (const PortInfo &port : parsePorts(ReportErrors::doNotReport)) {
		result[port] = DeviceInfo::create<robotParts::Motor>();
	}

	return result;
}
