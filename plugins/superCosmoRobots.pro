TEMPLATE = subdirs

SUBDIRS = \
	robots/editor \
	robots/dataFlowEditor \
	robots/examples \
	robots/common \
	robots/interpreters \
	robots/dataFlowInterpreters \
	robots/generators \
	robots/utils \
	robots/checker \
	robots/thirdparty \

robots/utils.depends = robots/thirdparty
robots/common.depends = robots/thirdparty robots/utils
robots/interpreters.depends = robots/common robots/thirdparty robots/utils
robots/dataFlowInterpreters.depends = robots/common robots/thirdparty robots/utils robots/interpreters
robots/generators.depends = robots/common robots/utils
robots/checker.depends = robots/interpreters
