#include "DFEncodersBlock.h"


#include <kitBase/robotModel/robotModelUtils.h>

using namespace dataFlowBlocks::details;
using namespace kitBase::robotModel;

const int dummyActivationPortNumber = -1;
const QStringList encoders = {"E1", "E2", "E3", "E4"};

DFEncodersBlock::DFEncodersBlock(kitBase::robotModel::RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
	portAssociatedWithProperty["E1_IN"] = 0;
	portAssociatedWithProperty["E2_IN"] = 1;
	portAssociatedWithProperty["E3_IN"] = 2;
	portAssociatedWithProperty["E4_IN"] = 3;
	portAssociatedWithProperty["E1_OUT"] = 4;
	portAssociatedWithProperty["E2_OUT"] = 5;
	portAssociatedWithProperty["E3_OUT"] = 6;
	portAssociatedWithProperty["E4_OUT"] = 7;

	mOwner.setValue<DFEncodersBlock *>(this);
}

int DFEncodersBlock::activationPortNumber() const
{
	return dummyActivationPortNumber;
}

void DFEncodersBlock::handleData()
{
	if (hasNewData(dummyActivationPortNumber)) {
		property(dummyActivationPortNumber);
		for (const QString &enc : encoders) {
			initEncoder(QString(enc+"_IN"));
		}
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
