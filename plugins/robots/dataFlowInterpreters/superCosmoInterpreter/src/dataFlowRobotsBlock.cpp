#include "dataFlowRobotsBlock.h"


using namespace kitBase;
using namespace blocksBase;
using namespace qReal;
using namespace interpreterCore;
using namespace interpreterCore::interpreter;
using namespace interpreterCore::interpreter::dataFlowInterpretation;


void DataFlowRobotsBlock::interpretThis()
{
	run();
}

void DataFlowRobotsBlock::handleDataFromFlow(const QVariant& data, int port)
{
	valueOnPort[port] = data;
	synchronisedPorts.remove(port);
	if (synchronisedPorts.empty()) {
		run();
	}
}

DataFlowRobotsBlock::DataFlowRobotsBlock()
	: RobotsBlock()
{
}

QVariant DataFlowRobotsBlock::property(int portNumber)
{
	return valueOnPort.value(portNumber);
}

QVariant DataFlowRobotsBlock::propertyFromPort(const QString& name)
{
	return property(portAssociatedWithProperty.value(name));
}

