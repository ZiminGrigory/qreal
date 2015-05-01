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

class PullDialog : public QDialog
{
	Q_OBJECT

public:
	PullDialog(QWidget *parent = 0);
	QString url() const;
	QString branch() const;


private:
	QLineEdit *mUrl;
	QLabel *mUrlLabel;
	QLineEdit *mBranch;
	QLabel *mBranchLabel;
	QDir mCurrentDir;

	QPushButton *createButton(const QString &text, const char *member);
};

}
}
