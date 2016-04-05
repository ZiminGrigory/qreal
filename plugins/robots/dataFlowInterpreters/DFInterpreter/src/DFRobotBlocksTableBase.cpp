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

#include "DFRobotBlocksTableBase.h"

#include <qrkernel/exception/exception.h>

using namespace dataFlow::interpretation;
using namespace qReal;

DFRobotBlocksTableBase::DFRobotBlocksTableBase()
{
}

DFRobotBlocksTableBase::~DFRobotBlocksTableBase()
{
	qDeleteAll(mBlocks);
}

DFRobotsBlockInterface *DFRobotBlocksTableBase::block(const Id &element)
{
	if (mBlocks.contains(element)) {
		return mBlocks[element];
	}

	DFRobotsBlockInterface * const newBlock = produceBlock(element);
	if (newBlock) {
		mBlocks.insert(element, newBlock);
	}

	return newBlock;
}

void DFRobotBlocksTableBase::clear()
{
	qDeleteAll(mBlocks);
	mBlocks.clear();
}

