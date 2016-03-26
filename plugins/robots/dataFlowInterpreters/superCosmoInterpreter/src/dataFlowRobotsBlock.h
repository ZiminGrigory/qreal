/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once


#include <QtCore/QMap>
#include <QtCore/QSet>

#include <kitBase/blocksBase/blocksFactoryInterface.h>
#include <kitBase/robotModel/robotModelManagerInterface.h>
#include <kitBase/blocksBase/robotsBlock.h>


namespace interpreterCore {
namespace interpreter {
namespace dataFlowInterpretation {

/// Base class for all blocks implementations used in robots interpreter.
class DataFlowRobotsBlock : public kitBase::blocksBase::RobotsBlock
{
	Q_OBJECT

public:
	virtual void interpretThis();
	void handleDataFromFlow(const QVariant &data, int port);

signals:
	void newDataInFlow(const QVariant &data, int port);

protected:
	DataFlowRobotsBlock();

	QVariant property(int portNumber);
	QVariant propertyFromPort(const QString &name);

	QMap<int, QVariant> valueOnPort;
	QMap<QString, int> portAssociatedWithProperty;
	QSet<int> synchronisedPorts;

private:
	/// Shall be reimplemented to provide semantics of block execution.
	virtual void run(){}

};

}
}
}
