#include "DFFactoryBase.h"

#include "DFRobotsBlock.h"
#include "blockBase/DFMotorsBlock.h"
#include "blockBase/DFValueEmitter.h"
#include "blockBase/DFFunctionBlock.h"
#include "blockBase/DFSensorVariableEmitter.h"
#include "blockBase/DFSupressorBlock.h"
#include "blockBase/DFInhibitionBlock.h"
#include "blockBase/DFInterflowingBlock.h"
#include "blockBase/DFExitExecutionBlock.h"
#include "blockBase/DFDelayAndConditionFilter.h"
#include "blockBase/DFGamepadBlock.h"
#include "blockBase/DFInputSubProgramPort.h"
#include "blockBase/DFOutputSubProgramPort.h"
#include "blockBase/DFSubprogramCall.h"
#include "blockBase/DFVariable.h"
#include "blockBase/DFWaitNode.h"
#include "blockBase/DFRandomValue.h"
#include "blockBase/DFLoopBlock.h"
#include "blockBase/DFConditionBlock.h"
#include "blockBase/DFSwitchBlock.h"
#include "blockBase/DFSeparatorBlock.h"
#include "blockBase/DFServoMotorsBlock.h"
#include "blockBase/DFConcreteReadPort.h"
#include "blockBase/DFConcreteWritablePort.h"
#include "blockBase/DFEncodersBlock.h"
#include "blockBase/DFForkBlock.h"
#include "blockBase/deviceBlocks/DFSmileBlock.h"
#include "blockBase/deviceBlocks/DFClearScreenBlock.h"
#include "blockBase/deviceBlocks/DFSetPenBlock.h"
#include "blockBase/deviceBlocks/DFPaintSettingsBlock.h"
#include "blockBase/deviceBlocks/DFTextOnScreenBlock.h"
#include "blockBase/deviceBlocks/DFShapeOnScreen.h"
#include "blockBase/deviceBlocks/DFLedBlock.h"
#include "blockBase/deviceBlocks/DFSayBlock.h"
#include "blockBase/deviceBlocks/DFWriteToFileBlock.h"
#include "blockBase/deviceBlocks/DFRemoveFileBlock.h"
#include "blockBase/deviceBlocks/DFReadFileBlock.h"
#include "blockBase/deviceBlocks/DFSystemCommandBlock.h"
#include "blockBase/deviceBlocks/DFVideoStreamBlock.h"
#include "blockBase/deviceBlocks/DFInitCameraBlock.h"
#include "blockBase/deviceBlocks/DFLineDetectorEmitterBlock.h"
#include "blockBase/deviceBlocks/DFSendMessageToRobotBlock.h"
#include "blockBase/deviceBlocks/DFReceiveMessageBlock.h"
#include "blockBase/DFDrawPixelBlock.h"
#include "blockBase/DFDrawLineBlock.h"



///@todo: split to factories (ex common, trik, etc)

using namespace dataFlow::blocksBase;
using namespace dataFlowBlocks;


void DFFactoryBase::configure(const qReal::GraphicalModelAssistInterface &graphicalModelApi
		, const qReal::LogicalModelAssistInterface &logicalModelApi
		, kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qReal::ErrorReporterInterface &errorReporter
		, qrtext::LanguageToolboxInterface &textLanguageToolbox)
{
	mRobotModelManager = &robotModelManager;
	mGraphicalModelApi = &graphicalModelApi;
	mLogicalModelApi = &logicalModelApi;
	mErrorReporter = &errorReporter;
	mParser = &textLanguageToolbox;
}

dataFlow::interpretation::DFRobotsBlockInterface *DFFactoryBase::block(const qReal::Id &element)
{
	interpretation::DFRobotsBlock *res = nullptr;

	if (elementDFMetatypeIs(element, "DFMotorsBlock")) {
		res = new details::DFMotorsBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFServoBlock")) {
		res = new details::DFServoMotorsBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFValueEmitterBlock")) {
		res = new details::DFValueEmitter();
	} else if (elementDFMetatypeIs(element, "DFFunctionBlock")) {
		res = new details::DFFunctionBlock();
	}  else if (elementDFMetatypeIs(element, "DFLoopBlock")) {
		res = new details::DFLoopBlock();
	} else if (elementDFMetatypeIs(element, "DFSensorVarBlock")) {
		res = new details::DFSensorVariableEmitter(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFInhibitorBlock")) {
		res = new details::DFInhibitionBlock();
	} else if (elementDFMetatypeIs(element, "DFSupressorBlock")) {
		res = new details::DFSupressorBlock();
	} else if (elementDFMetatypeIs(element, "DFInterflowingBlock")) {
		res = new details::DFInterflowingBlock();
	} else if (elementDFMetatypeIs(element, "DFFinalBlock")) {
		res = new details::DFExitExecutionBlock();
	} else if (elementDFMetatypeIs(element, "DFDelayFilterBlock")) {
		res = new details::DFDelayAndConditionFilter();
	} else if (elementDFMetatypeIs(element, "DFGamepadBlock")) {
		res = new details::DFGamepadBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFInPortBlock")) {
		res = new details::DFInputSubProgramPort();
	} else if (elementDFMetatypeIs(element, "DFOutPortBlock")) {
		res = new details::DFOutputSubProgramPort();
	} else if (elementDFMetatypeIs(element, "DFUserBlock")) {
		res = new details::DFSubprogramCall();
	} else if (elementDFMetatypeIs(element, "DFGetSetVariableBlock")) {
		res = new details::DFVariable();
	} else if (elementDFMetatypeIs(element, "DFWaitBlock")) {
		res = new details::DFWaitNode();
	} else if (elementDFMetatypeIs(element, "DFRandomValueBlock")) {
		res = new details::DFRandomValue(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFIfBlock")) {
		res = new details::DFConditionBlock();
	} else if (elementDFMetatypeIs(element, "DFSwitchBlock")) {
		res = new details::DFSwitchBlock();
	} else if (elementDFMetatypeIs(element, "DFSeparatorBlock")) {
		res = new details::DFSeparatorBlock();
	} else if (elementDFMetatypeIs(element, "DFSmilePainterBlock")) {
		res = new details::DFSmileBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFClearScreenBlock")) {
		res = new details::DFClearScreenBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFPenBlock")) {
		res = new details::DFSetPenBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFPaintSettingsBlock")) {
		res = new details::DFPaintSettingsBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFTextOnScreenBlock")) {
		res = new details::DFTextOnScreenBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "ShapePainter")) {
		res = new details::DFShapeOnScreen(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFLedBlock")) {
		res = new details::DFLedBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFSayBlock")) {
		res = new details::DFSayBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFWriteToFileBlock")) {
		res = new details::DFWriteToFileBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFRemoveFileBlock")) {
		res = new details::DFRemoveFileBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFReadFileBlock")) {
		res = new details::DFReadFileBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFSystemCallBlock")) {
		res = new details::DFSystemCommandBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFStreamingBlock")) {
		res = new details::DFVideoStreamBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFInitCameraBlock")) {
		res = new details::DFInitCameraBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFDetectByVideoBlock")) {
		res = new details::DFLineDetectorEmitterBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFConcreteInputPortBlock")) {
		res = new details::DFConcreteWritablePort(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFConcreteOutputPortBlock")) {
		res = new details::DFConcreteReadPort(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFEncodersBlock")) {
		res = new details::DFEncodersBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "ForkNode")) {
		res = new details::DFForkBlock();
	} else if (elementDFMetatypeIs(element, "DFReceiveMsgBlock")) {
		res = new details::DFReceiveMessageBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFMessageToRobotBlock")) {
		res = new details::DFSendMessageToRobotBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFDrawPixelBlock")) {
		res = new details::DFDrawPixelBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "DFDrawLineBlock")) {
		res = new details::DFDrawLineBlock(mRobotModelManager->model());
	}

	if (res) {
		res->init(element
				, *mGraphicalModelApi
				, *mLogicalModelApi
				, mErrorReporter
				, *mRobotModelManager
				, *mParser
				);

		res->configureSynchronizedPorts();
	}


	return res ? res : nullptr;
}

qReal::IdList DFFactoryBase::providedBlocks() const
{
	qReal::IdList result;

	result << dataFlowId("Motors")
			<< dataFlowId("ValueEmitter")
			<< dataFlowId("Function")
			<< dataFlowId("SensorVar")
			<< dataFlowId("Supressor")
			<< dataFlowId("Inhibitor")
			<< dataFlowId("Interflowing")
			<< dataFlowId("FinalNode")
			<< dataFlowId("DelayFilterNode")
			<< dataFlowId("Gamepad")
			<< dataFlowId("InPort")
			<< dataFlowId("OutPort")
			<< dataFlowId("SubprogramCall")
			<< dataFlowId("GetSetVariable")
			<< dataFlowId("WaitNode")
			<< dataFlowId("RandomValue")
			<< dataFlowId("IfNode")
			<< dataFlowId("Switch")
			<< dataFlowId("Separator")
			<< dataFlowId("Servo");
	return result;
}

qReal::IdList DFFactoryBase::blocksToDisable() const
{
	qReal::IdList result;

	if (mRobotModelManager->model().name().contains("TwoD")) {
		result << dataFlowId("SayNode");
	}

	return result;
}


qReal::Id DFFactoryBase::dataFlowId(const QString &metatype) const
{
	return qReal::Id("RobotsDataFlowMetamodel", "RobotsDataFlowDiagram", metatype);
}

bool DFFactoryBase::elementDFMetatypeIs(const qReal::Id &element, const QString &metatype) const
{
	return element.type() == dataFlowId(metatype);
}
