#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>

namespace git
{
struct Tag
{
	Tag()
	{
	}

	Tag(
			const QString &oper
			, const QString &strTagOption1 = QString()
			, const QString &strTagOption2 = QString()
			, bool bTag = false)
		: operation(oper)
		, stringTagOption1(strTagOption1)
		, stringTagOption2(strTagOption2)
		, boolTag(bTag)
	{
	}

	QString operation;
	QString stringTagOption1;
	QString stringTagOption2;
	bool boolTag;
};
}

Q_DECLARE_METATYPE(git::Tag)
