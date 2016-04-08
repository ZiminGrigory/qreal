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

	if (elementDFMetatypeIs(element, "Motors")) {
		res = new details::DFMotorsBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "ValueEmitter")) {
		res = new details::DFValueEmitter();
	} else if (elementDFMetatypeIs(element, "Function")) {
		res = new details::DFFunctionBlock();
	} else if (elementDFMetatypeIs(element, "SensorVar")) {
		res = new details::DFSensorVariableEmitter(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "Inhibitor")) {
		res = new details::DFInhibitionBlock();
	} else if (elementDFMetatypeIs(element, "Supressor")) {
		res = new details::DFSupressorBlock();
	} else if (elementDFMetatypeIs(element, "Interflowing")) {
		res = new details::DFInterflowingBlock();
	} else if (elementDFMetatypeIs(element, "FinalNode")) {
		res = new details::DFExitExecutionBlock();
	} else if (elementDFMetatypeIs(element, "DelayFilterNode")) {
		res = new details::DFDelayAndConditionFilter();
	} else if (elementDFMetatypeIs(element, "Gamepad")) {
		res = new details::DFGamepadBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "InPort")) {
		res = new details::DFInputSubProgramPort();
	} else if (elementDFMetatypeIs(element, "OutPort")) {
		res = new details::DFOutputSubProgramPort();
	} else if (elementDFMetatypeIs(element, "SubprogramCall")) {
		res = new details::DFSubprogramCall();
	}


	if (res) {
		res->init(element
				, *mGraphicalModelApi
				, *mLogicalModelApi
				, mErrorReporter
				, *mRobotModelManager
				, *mParser
				);
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
			<< dataFlowId("SubprogramCall");

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
