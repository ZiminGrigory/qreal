#include "DFLoopBlock.h"

#include <QApplication>


using namespace dataFlowBlocks::details;

DFLoopBlock::DFLoopBlock()
{
	portAssociatedWithProperty["FROM"] = 0;
	portAssociatedWithProperty["TO"] = 1;
	portAssociatedWithProperty["STEP"] = 2;
	portAssociatedWithProperty["CF_IN"] = 3;
	portAssociatedWithProperty["CF_OUT"] = 4;
	portAssociatedWithProperty["OUT"] = 5;
}

void DFLoopBlock::init()
{
	from = intProperty("start_border");
	to = intProperty("end_border");
	step = intProperty("increment");
}

void DFLoopBlock::handleData()
{
	if (hasNewData("FROM")) {
		from = propertyFromPort("FROM").toInt();
	}

	if (hasNewData("TO")) {
		to = propertyFromPort("TO").toInt();
	}

	if (hasNewData("STEP")) {
		step = propertyFromPort("STEP").toInt();
	}

	if (hasNewData("CF_IN")) {
		currentCounter = from;
		while (currentCounter <= to) {
			emit newDataInFlow(currentCounter, portAssociatedWithProperty["OUT"]);
			QCoreApplication::processEvents();
			currentCounter += step;
		}

		emit newDataInFlow(QVariant(), portAssociatedWithProperty["CF_OUT"]);
	}
}
