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

#include <kitBase/robotModel/robotModelManagerInterface.h>

#include "DFRobotBlocksTableBase.h"
#include "DFRobotBlocksFactoryInterface.h"

namespace dataFlow {
namespace interpretation {

/// @todo: make fatcory manager and etc.
class DFRobotBlocksTable : public DFRobotBlocksTableBase
{
public:
	/// Constructor.
	/// @param blocksFactoryManager - a factory manager that is used to create new blocks when needed.
	/// @param robotModelManager - has a reference to current robot model.
	DFRobotBlocksTable(blocksBase::DFRobotBlocksFactoryInterface &blocksFactory
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager);

private:
	interpretation::DFRobotsBlockInterface *produceBlock(const qReal::Id &element) override;

	blocksBase::DFRobotBlocksFactoryInterface &mBlocksFactory;
	const kitBase::robotModel::RobotModelManagerInterface &mRobotModelManager;
};

}
}
