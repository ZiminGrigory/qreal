TEMPLATE = subdirs

SUBDIRS = \
	editor \
	dataFlowEditor \
	examples \
	common \
	interpreters \
	dataFlowInterpreters \
	generators \
	utils \
	checker \
	thirdparty \

editor.subdir = $$PWD/robots/editor
dataFlowEditor.subdir = $$PWD/robots/dataFlowEditor
examples.subdir = $$PWD/robots/examples
common.subdir = $$PWD/robots/common
interpreters.subdir = $$PWD/robots/interpreters
dataFlowInterpreters.subdir = $$PWD/robots/dataFlowInterpreters
generators.subdir = $$PWD/robots/generators
utils.subdir = $$PWD/robots/utils
checker.subdir = $$PWD/robots/checker
thirdparty.subdir = $$PWD/robots/thirdparty

utils.depends = thirdparty
common.depends = thirdparty utils
interpreters.depends = common thirdparty utils
dataFlowInterpreters.depends = common thirdparty utils interpreters
generators.depends = common utils
checker.depends = interpreters

