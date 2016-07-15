#include "DFEncodersBlock.h"


#include <kitBase/robotModel/robotModelUtils.h>

using namespace dataFlowBlocks::details;
using namespace kitBase::robotModel;

const QStringList encoders = {"E1", "E2", "E3", "E4"};

DFEncodersBlock::DFEncodersBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["CF_OUT"] = 1;
	portAssociatedWithProperty["E1_IN"] = 2;
	portAssociatedWithProperty["E2_IN"] = 3;
	portAssociatedWithProperty["E3_IN"] = 4;
	portAssociatedWithProperty["E4_IN"] = 5;
	portAssociatedWithProperty["E1_OUT"] = 6;
	portAssociatedWithProperty["E2_OUT"] = 7;
	portAssociatedWithProperty["E3_OUT"] = 8;
	portAssociatedWithProperty["E4_OUT"] = 9;

	mOwner.setValue<DFEncodersBlock *>(this);
}

void DFEncodersBlock::handleData()
{
	if (hasNewData(portAssociatedWithProperty["CF_IN"])) {
		property(portAssociatedWithProperty["CF_IN"]);
		for (const QString &enc : encoders) {
			if (boolProperty(enc + "Clear")) {
				initEncoder(QString(enc + "_IN"));
			}
		}

		emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
	} else {
		handleEncoder("E1_IN");
		handleEncoder("E2_IN");
		handleEncoder("E3_IN");
		handleEncoder("E4_IN");
	}
}

void DFEncodersBlock::init()
{
	for (const QString &enc : encoders) {
		auto encoder = RobotModelUtils::findDevice<robotParts::EncoderSensor>(mRobotModel, enc);
		mEncodersValue.insert(QString(enc+"_IN"), 0);
		mEncoders.insert(QString(enc+"_IN"), encoder);
		mEncodersToPorts.insert(encoder, QString(enc+"_OUT"));
	}
}

void DFEncodersBlock::newIntDataReceived(int data)
{
	if (sender()->property("owner").value<DFEncodersBlock *>() == this) {
		auto encoder = dynamic_cast<robotParts::EncoderSensor *>(sender());
		QString encoderOutPortName = mEncodersToPorts.value(encoder);
		QString encoderINPortName = encoderOutPortName;
		encoderINPortName.replace("OUT", "IN");
		emit newDataInFlow(QVariant(data + mEncodersValue[encoderINPortName])
				, portAssociatedWithProperty[encoderOutPortName]);
	}
}

void DFEncodersBlock::handleEncoder(const QString &portName)
{
	if (mEncoders[portName] && hasNewData(portAssociatedWithProperty[portName])) {
		initEncoder(portName);
	}
}

void DFEncodersBlock::initEncoder(const QString &portName)
{
	mEncodersValue[portName] = propertyFromPort(portName).toInt();
	mEncoders[portName]->setProperty("owner", mOwner);
	mEncoders[portName]->nullify();
	disconnect(mEncoders[portName], &robotParts::EncoderSensor::newData, this
			, &DFEncodersBlock::newIntDataReceived);
	connect(mEncoders[portName], &robotParts::EncoderSensor::newData, this
			, &DFEncodersBlock::newIntDataReceived, Qt::QueuedConnection);
}
