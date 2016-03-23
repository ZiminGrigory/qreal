TARGET = super-cosmo-interpreter

include(../../../../global.pri)

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

include(src/superCosmoInterpreter.pri)

