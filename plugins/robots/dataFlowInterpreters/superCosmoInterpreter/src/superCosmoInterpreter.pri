includes(plugins/robots/common/kitBase \
		plugins/robots/common/twoDModel \
		plugins/robots/common/ev3Kit \
		plugins/robots/utils \
		qrtext \
		plugins/robots/interpeters/ev3KitInterpreter/src \
		plugins/robots/thirdparty/libusb-1.0.19/include/libusb-1.0 \
)

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
		robots-utils robots-kit-base robots-ev3-kit robots-2d-model qextserialport qslog \
)

win32 {
	LIBS += -llibusb-1.0
}

unix:!macx {
	LIBS += -L /usr/bin -lusb-1.0
}

macx {
	LIBS += -lusb-1.0
}

HEADERS += \

SOURCES += \
