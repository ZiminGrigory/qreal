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
	QVector<QVariant> data;
	QString dataFromProperty = DFRobotsBlock::stringProperty(mGraphicalId, "varValue");
	if (dataFromProperty.contains('[')) {
		dataFromProperty.trimmed();
		dataFromProperty.remove('[');
		dataFromProperty.remove(']');
		QStringList localSplit = dataFromProperty.split(',');
		for (auto &s : localSplit) {
			data << QVariant(s);
		}
	} else {
		data.push_back(QVariant(dataFromProperty));
	}

	QVariant dataToSend;
	dataToSend.setValue(data);

	emit newDataInFlow(dataToSend, portAssociatedWithProperty["OUT"]);
}
