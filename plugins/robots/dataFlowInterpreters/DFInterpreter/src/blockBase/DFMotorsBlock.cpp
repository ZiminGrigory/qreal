#include "DFMotorsBlock.h"

#include <QtCore/QPair>


using namespace dataFlow::interpretation;
using namespace kitBase;
using namespace kitBase::blocksBase;
using namespace kitBase::robotModel;
using namespace dataFlowBlocks::details;

/// @todo: rewrite this SH~

const QStringList trikPorts = {"M1", "M2", "M3", "M4"};

DFMotorsBlock::DFMotorsBlock(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["CF_OUT"] = 1;
	portAssociatedWithProperty["M1"] = 2;
	portAssociatedWithProperty["M2"] = 3;
	portAssociatedWithProperty["M3"] = 4;
	portAssociatedWithProperty["M4"] = 5;
}

void DFMotorsBlock::handleData()
{
	auto aggregator = robotModel::RobotModelUtils::findDevice<robotParts::MotorsAggregator>(mRobotModel, "MAll");

	bool hasData = true;

	if (hasNewData(portAssociatedWithProperty["CF_IN"])) {
		property(portAssociatedWithProperty["CF_IN"]);
		hasData = false;
	}

	if (!aggregator) {
		auto setPower = [&](const QString &port, int power){
			if (power != mPreviousPower.value(port, 0)) {
				motors[port]->on(power);
				mPreviousPower.insert(port, power);
			}
		};

		for (const QString &port : trikPorts) {
			int power = 0;
			if (hasData && hasNewData(portAssociatedWithProperty[port])) {
				power = property(portAssociatedWithProperty[port]).toInt();
				setPower(port, power);
			} else if (!hasData) {
				power = intProperty(port + "Power");
				setPower(port, power);
			}
		}
	} else {
		QList<QPair<QString, int>> powersForMotors;
		auto setPower = [&](const QString &port, int power){
			if (power != mPreviousPower.value(port, 0)) {
				powersForMotors.append(qMakePair(port, power));
				mPreviousPower.insert(port, power);
			}
		};

		for (const QString &port : trikPorts) {
			int power = 0;
			if (hasData && hasNewData(portAssociatedWithProperty[port])) {
				power = property(portAssociatedWithProperty[port]).toInt();
				setPower(port, power);
			} else if (!hasData) {
				power = intProperty(port + "Power");
				setPower(port, power);
			}
		}

		if (!powersForMotors.isEmpty()) {
			aggregator->on(powersForMotors);
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

	for (const QString &port : trikPorts) {
		PortInfo const portInfo = robotModel::RobotModelUtils::findPort(mRobotModel, port, output);
		if (portInfo.isValid()) {
			result << portInfo;
		}
	}

	return result;
}
