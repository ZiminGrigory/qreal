#include "DFGamepadBlock.h"

#include <kitBase/robotModel/robotModelUtils.h>
#include <trikKitInterpreterCommon/robotModel/real/parts/gamepadPadPressSensor.h>

#include <QtCore/QPair>
#include <QDebug>

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

void DFGamepadBlock::handleData()
{
	if (!isActivated) {
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
					[=](int data) {
						const QVariantList list = {button.second, data};
						if (data) {
							emit newDataInFlow(list, portAssociatedWithProperty["BUTTON"]);
						}
					}
					, Qt::QueuedConnection
			);
		}

		struct Pad{
			QString pad;
			QString padPressed;
			int padNumber;
		};

		QList<Pad> padPosPorts = {
				{"GamepadPad1PosPort", "GamepadPad1PressedPort", 1}
				, {"GamepadPad2PosPort", "GamepadPad2PressedPort", 2}
		};

		for (const Pad &pad : padPosPorts) {
			const PortInfo &portVec = RobotModelUtils::findPort(mRobotModel, pad.pad, input);
			auto sensorVec = RobotModelUtils::findDevice<robotParts::VectorSensor>(mRobotModel, portVec);
			const PortInfo &portSc = RobotModelUtils::findPort(mRobotModel, pad.padPressed, input);
			auto sensorSc = RobotModelUtils::findDevice<trik::robotModel::real::parts::GamepadPadPressSensor>(
					mRobotModel
					, portSc
			);

			connect(sensorVec, &robotParts::VectorSensor::newData, this,
					[=](QVector<int> data) {
						if (sensorSc->oldValue()) {
							QVariantList list = {QVariant(pad.padNumber)};
							for (int i : data) {
								list.push_back(i);
							}

							qDebug() << "sends data" << list;
							emit newDataInFlow(list, portAssociatedWithProperty["PAD"]);
						}
					}
					, Qt::QueuedConnection
			);

			connect(sensorSc, &trik::robotModel::real::parts::GamepadPadPressSensor::stateChanged, this,
					[=](bool down) {
						handlePadUpOrDown(pad.padNumber, down);
					}
					, Qt::QueuedConnection
			);
		}


//		const PortInfo &gamepadWheelPort = RobotModelUtils::findPort(mRobotModel, "GamepadWheelPort", input);
//		robotParts::ScalarSensor *wheel = RobotModelUtils::findDevice<robotParts::ScalarSensor>(
//				mRobotModel
//				, gamepadWheelPort
//		);

//		connect(wheel, &robotParts::ScalarSensor::newData, this,
//				[=](int data) {
//					if (data != dummyData) {
//						emit newDataInFlow(data, portAssociatedWithProperty["WHEEL"]);
//					}
//				}
//				, Qt::QueuedConnection
//		);

		isActivated = true;
	}
}

void DFGamepadBlock::handlePadUpOrDown(int padNumber, int isPressed)
{
	if (!isPressed) {
		QVariantList list = {QVariant(padNumber), QVariant(0), QVariant(0)};
		qDebug() << "pressed change";
		newDataInFlow(list, portAssociatedWithProperty["PAD"]);
	}
}
