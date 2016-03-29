include (qreal.pri)

plugins.file = $$PWD/plugins/DFRobots.pro

RESOURCES += \
	qrgui/mainwindow/qrgui.qrc

include (installer/platform/install-trik-studio.pri)
