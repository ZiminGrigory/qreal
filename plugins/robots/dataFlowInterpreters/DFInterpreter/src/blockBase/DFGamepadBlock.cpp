#include "DFGamepadBlock.h"

#include <kitBase/robotModel/robotModelUtils.h>

#include <QtCore/QPair>

using namespace dataFlowBlocks::details;
using namespace kitBase::robotModel;

DFGamepadBlock::DFGamepadBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
	//	QMap<int, QVariant> valueOnPort;
	//	QMap<QString, int> portAssociatedWithProperty;
		portAssociatedWithProperty["CF_IN"] = 0;
		portAssociatedWithProperty["PAD"] = 1;
		portAssociatedWithProperty["BUTTON"] = 2;
		portAssociatedWithProperty["WHEEL"] = 3;
}

void DFGamepadBlock::init()
{
	QList<QPair<QString, int>> padButtons = {
			qMakePair<QString, int>("GamepadButton1Port", 1)
			, qMakePair<QString, int>("GamepadButton2Port", 2)
			, qMakePair<QString, int>("GamepadButton3Port", 3)
			, qMakePair<QString, int>("GamepadButton4Port", 4)
			, qMakePair<QString, int>("GamepadButton5Port", 5)
	};

	for (const QPair<QString, int> &button : padButtons) {
		const PortInfo &port = RobotModelUtils::findPort(mRobotModel, button.first, input);
		robotParts::ScalarSensor *sensor = RobotModelUtils::findDevice<robotParts::ScalarSensor>(mRobotModel, port);
		connect(sensor, &robotParts::ScalarSensor::newData, this,
				[&](int data) {
					const QVariantList list = {button.second, data};
					emit newDataInFlow(list, portAssociatedWithProperty["BUTTON"]);
				}
				, Qt::QueuedConnection
		);
	}

	QList<QPair<QString, int>> padPosPorts = {
			qMakePair<QString, int>("GamepadPad1PosPort", 1)
			, qMakePair<QString, int>("GamepadPad2PosPort", 2)
	};

	for (const QPair<QString, int> &padPos : padPosPorts) {
		const PortInfo &port = RobotModelUtils::findPort(mRobotModel, padPos.first, input);
		robotParts::VectorSensor *sensor = RobotModelUtils::findDevice<robotParts::VectorSensor>(mRobotModel, port);
		connect(sensor, &robotParts::VectorSensor::newData, this,
				[&](QVector<int> data) {
					QVariantList list = {QVariant(padPos.second)};
					for (int i : data) {
						list.push_back(i);
					}

					emit newDataInFlow(list, portAssociatedWithProperty["PAD"]);
				}
				, Qt::QueuedConnection
		);
	}

	const PortInfo &gamepadWheelPort = RobotModelUtils::findPort(mRobotModel, "GamepadWheelPort", input);
	robotParts::ScalarSensor *wheel = RobotModelUtils::findDevice<robotParts::ScalarSensor>(
			mRobotModel
			, gamepadWheelPort
	);

	connect(wheel, &robotParts::ScalarSensor::newData, this,
			[&](int data) {
				emit newDataInFlow(data, portAssociatedWithProperty["WHEEL"]);
			}
			, Qt::QueuedConnection
	);

}

void DFGamepadBlock::handleData()
{

}
