#pragma once

#include <QtCore/QHash>
#include <QtCore/QScopedPointer>

#include <qrkernel/ids.h>

#include "DFRobotBlocksTableInterface.h"
#include "DFRobotsBlockInterface.h"

namespace dataFlow {
namespace interpretation {

/// Provides mapping from block ids to objects that implement logic of block, also creates blocks when needed, so
/// clients can simply request a block by given id, and a block table will do the rest.
/// Also supports operations that shall be performed on all blocks in a system, such as setting failure or idle flags.
/// and objects implementing logic of that blocks.
class DFRobotBlocksTableBase : public DFRobotBlocksTableInterface
{
public:
	/// Constructor. (c) Captain Obvious, 2008-2015
	DFRobotBlocksTableBase();
	~DFRobotBlocksTableBase() override;

	/// Returns the object that may be used by the interpreter to invoke the block with the given id.
	/// If met for the first time it will be produced from scratch, otherwise existing instance will be returned.
	DFRobotsBlockInterface *block(const qReal::Id &element) override;

	/// Clears blocks table.
	void clear() override;


private:
	/// Must be overridded to produce the logical instance of the block itself.
	virtual DFRobotsBlockInterface *produceBlock(const qReal::Id &element) = 0;

	QHash<qReal::Id, DFRobotsBlockInterface *> mBlocks;  // Has ownership
};

}
}
