#include "DFExitExecutionBlock.h"

using namespace dataFlowBlocks::details;

DFExitExecutionBlock::DFExitExecutionBlock()
{
}

void dataFlowBlocks::details::DFExitExecutionBlock::handleData()
{
	emit stopExecution();
}
