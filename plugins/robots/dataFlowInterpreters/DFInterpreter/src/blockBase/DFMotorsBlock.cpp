#include "DFMotorsBlock.h"

using namespace interpreterCore;
using namespace interpreter;
using namespace dataFlowInterpretation;
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
	portAssociatedWithProperty["M1"] = 1;
	portAssociatedWithProperty["M2"] = 2;
	portAssociatedWithProperty["M3"] = 4;
	portAssociatedWithProperty["M4"] = 5;
	portAssociatedWithProperty["CF_OUT"] = 3;

	if (Block::property("synch").toBool()) {
		qReal::IdList localIncomingLinks = mGraphicalModelApi->graphicalRepoApi().incomingLinks(mGraphicalId);
		for (auto &id : localIncomingLinks) {
			synchronisedPorts.insert(qRound64(mGraphicalModelApi->toPort(id)));
		}
	}
}

void DFMotorsBlock::run()
{
	auto motors = usedDevices();
	auto keys = motors.keys();

	for (auto &key : keys) {
		QVariant power = valueOnPort[portAssociatedWithProperty[key.name()]];
		auto *motor = robotModel::RobotModelUtils::findDevice<robotParts::Motor>(mRobotModel, key);
		if (power != QVariant()) {
			motor->on(power.toInt());
		}
	}

	if (!mGraphicalModelApi->graphicalRepoApi().outgoingLinks(mGraphicalId).empty()) {
		emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
	}
}


QList<PortInfo> DFMotorsBlock::parsePorts(qReal::interpretation::Block::ReportErrors reportErrors)
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
