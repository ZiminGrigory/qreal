#include "DFFactoryBase.h"

#include "blockBase/DFMotorsBlock.h"
#include "blockBase/DFValueEmitter.h"

using namespace kitBase::blocksBase;
using namespace dataFlowBlocks;


qReal::interpretation::Block *dataFlowBlocks::DFFactoryBase::produceBlock(const qReal::Id &element)
{
	if (elementDFMetatypeIs(element, "Motors")) {
		return new details::DFMotorsBlock(mRobotModelManager->model());
	} else if (elementDFMetatypeIs(element, "ValueEmitter")) {
		return new details::DFValueEmitter();
	}

	return nullptr;
}

qReal::IdList dataFlowBlocks::DFFactoryBase::providedBlocks() const
{
	qReal::IdList result;

	result << dataFlowId("Motors")
			<< dataFlowId("ValueEmitter");

	return result;
}

qReal::IdList dataFlowBlocks::DFFactoryBase::blocksToDisable() const
{
	qReal::IdList result;

	if (mRobotModelManager->model().name().contains("TwoD")) {
		result << dataFlowId("SayNode");
	}

	return result;
}

qReal::Id dataFlowBlocks::DFFactoryBase::dataFlowId(const QString &metatype) const
{
	return qReal::Id("RobotsDataFlowMetamodel", "RobotsDataFlowDiagram", metatype);
}

bool DFFactoryBase::elementDFMetatypeIs(const qReal::Id &element, const QString &metatype) const
{
	return element.type() == dataFlowId(metatype);
}