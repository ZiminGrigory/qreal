#pragma once

#include <QtCore/QScopedPointer>

#include "interpreterBase/robotModel/robotModelInterface.h"
#include "interpreterBase/robotModel/configuration.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {

class ROBOTS_INTERPRETER_BASE_EXPORT CommonRobotModel : public RobotModelInterface
{
	Q_OBJECT

public:
	CommonRobotModel();
	~CommonRobotModel() override;

	void init() final;

	void connectToRobot() override;

	/// Shall be implemented in descendants to stop all robot devices. Default implementation does nothing.
	void stopRobot() override;

	void disconnectFromRobot() override;

	ConnectionState connectionState() const final;

	ConfigurationInterface const &configuration() const final;

	QList<PortInfo> availablePorts() const override;
	QList<PortInfo> configurablePorts() const override;
	QList<DeviceInfo> allowedDevices(PortInfo const &port) const override;

	void configureDevice(PortInfo const &port, DeviceInfo const &deviceInfo) final;

	void applyConfiguration() final;

	QList<DeviceInfo> convertibleBases() const override;

public slots:
	virtual void rereadSettings();

protected:
	void addAllowedConnection(PortInfo const &port, QList<DeviceInfo> const &devices);

	ConfigurationInterface &mutableConfiguration();

private slots:
	void onConnected(bool success);
	void onDisconnected();

private:
	virtual robotParts::Device * createDevice(PortInfo const &port, DeviceInfo const &deviceInfo);

	QHash<PortInfo, QList<DeviceInfo>> mAllowedConnections;

	/// Devices configuration.
	Configuration mConfiguration;

	/// Model connection state.
	ConnectionState mState;

	bool mConfigurationPostponed;
};

}
}
