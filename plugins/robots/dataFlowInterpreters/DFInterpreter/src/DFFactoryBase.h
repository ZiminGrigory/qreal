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

#include "DFRobotsBlock.h"
#include "DFRobotBlocksFactoryInterface.h"

namespace dataFlow {
namespace blocksBase {

class DFFactoryBase : public DFRobotBlocksFactoryInterface
{
public:
	void configure(const qReal::GraphicalModelAssistInterface &graphicalModelApi
			, const qReal::LogicalModelAssistInterface &logicalModelApi
			, kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qReal::ErrorReporterInterface &errorReporter
			, qrtext::LanguageToolboxInterface &textLanguageToolbox
			) final;

	interpretation::DFRobotsBlockInterface *block(const qReal::Id &element) override;
	qReal::IdList providedBlocks() const override;
	qReal::IdList blocksToDisable() const override;

protected:
	qReal::Id dataFlowId(const QString &metatype) const;
	bool elementDFMetatypeIs(const qReal::Id &element, const QString &metatype) const;

	kitBase::robotModel::RobotModelManagerInterface *mRobotModelManager = nullptr;  // Does not have ownership.
	const qReal::GraphicalModelAssistInterface *mGraphicalModelApi = nullptr;  // Does not have ownership.
	const qReal::LogicalModelAssistInterface *mLogicalModelApi = nullptr;  // Does not have ownership.
	qReal::ErrorReporterInterface *mErrorReporter = nullptr;  // Does not have ownership.
	qrtext::LanguageToolboxInterface *mParser = nullptr;  // Does not have ownership.
};

}
}
