#pragma once

#include <QtCore/QObject>

#include <qrkernel/ids.h>

namespace dataFlow {
namespace interpretation {

/// An interface of the dataflow block`s implementation used in the interprer for executing it.
class DFRobotsBlockInterface : public QObject
{
	Q_OBJECT

public:
	~DFRobotsBlockInterface() override {}

	/// Starts block execution when data received.
	/// @param data - data received  for this block.
	/// @param port - port from data sends.
	virtual void handleNewDataFromFlow(const QVariant &data, int port) = 0;

	/// Returns the diagram`s id of the block whitch is impemented by this object.
	virtual const qReal::Id id() const = 0;

	/// Returns port number for activation with dummy data. Needs to DFThreads to starting interpretation.
	virtual int activationPortNumber() const = 0;

signals:
	/// Emitted each time when block sends data to others.
	/// @param data - any data produced by block.
	/// @param port - port from data sends.
	void newDataInFlow(const QVariant &data, int port);

	/// Emitted each time when the block stops its execution due to some error.
	void failure();

	/// Emitted when we need stop the execution
	void stopExecution();

};

}
}
