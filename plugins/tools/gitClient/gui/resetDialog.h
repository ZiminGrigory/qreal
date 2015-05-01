#pragma once

#include <QtCore/QDir>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

namespace git
{
namespace ui
{

class ResetDialog : public QDialog
{
	Q_OBJECT

public:
	ResetDialog(QWidget *parent = 0);
	QString hashCommit() const;

private:
	QLineEdit *mHashCommitLineEdit;
	QLabel *mHashCommitLabel;
	QDir mCurrentDir;

	QPushButton *createButton(const QString &text, const char *member);
};

}
}
