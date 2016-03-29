#include "DFRobotsBlock.h"


using namespace kitBase;
using namespace blocksBase;
using namespace qReal;
using namespace interpreterCore;
using namespace interpreterCore::interpreter;
using namespace interpreterCore::interpreter::dataFlowInterpretation;


void DFRobotsBlock::interpretThis()
{
	run();
}

void DFRobotsBlock::handleDataFromFlow(const QVariant& data, int port)
{
	valueOnPort[port] = data;
	synchronisedPorts.remove(port);
	if (synchronisedPorts.empty()) {
		run();
	}
}

DFRobotsBlock::DFRobotsBlock()
	: RobotsBlock()
{
}

QVariant DFRobotsBlock::property(int portNumber)
{
	return valueOnPort.value(portNumber);
}

QVariant DFRobotsBlock::propertyFromPort(const QString& name)
{
	return property(portAssociatedWithProperty.value(name));
}

