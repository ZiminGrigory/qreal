#include "DFValueEmitter.h"

dataFlowBlocks::details::DFValueEmitter::DFValueEmitter()
{
//	QMap<int, QVariant> valueOnPort;
//	QMap<QString, int> portAssociatedWithProperty;
	portAssociatedWithProperty["CF_IN"] = 0;
	portAssociatedWithProperty["OUT"] = 1;
}

void dataFlowBlocks::details::DFValueEmitter::handleData()
{
	QString dataFromProperty = DFRobotsBlock::stringProperty(mGraphicalId, "varValue");
	if (dataFromProperty.contains('[')) {
		QVariantList data;
		dataFromProperty.trimmed();
		dataFromProperty.remove('[');
		dataFromProperty.remove(']');
		QStringList localSplit = dataFromProperty.split(',');
		for (auto &s : localSplit) {
			data << QVariant(s);
		}


		emit newDataInFlow(data, portAssociatedWithProperty["OUT"]);

	} else {
		emit newDataInFlow(dataFromProperty, portAssociatedWithProperty["OUT"]);
	}
}
