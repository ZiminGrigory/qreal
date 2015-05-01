#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFileDialog>

#include "pullDialog.h"

using namespace git::ui;

PullDialog::PullDialog(QWidget *parent)
	: QDialog(parent)
{
	mUrlLabel = new QLabel(tr("remote url: "));
	mUrl = new QLineEdit(this);
	mBranchLabel = new QLabel(tr("branch: "));
	mBranch = new QLineEdit(this);
	QPushButton *ok = new QPushButton(tr("OK"), this);
	QPushButton *cancel = new QPushButton(tr("Cancel"), this);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(ok);
	buttonsLayout->addWidget(cancel);

	connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(mUrlLabel, 0, 0);
	mainLayout->addWidget(mUrl, 0, 1, 1, 2);
	mainLayout->addWidget(mBranchLabel, 1, 0);
	mainLayout->addWidget(mBranch, 1, 1, 1, 2);
	mainLayout->addLayout(buttonsLayout, 2, 0, 1, 3);
	setLayout(mainLayout);

	setWindowTitle(tr("Pull options"));
	setMinimumSize(500,100);
}

QPushButton *PullDialog::createButton(const QString &text, const char *member)
{
	QPushButton *button = new QPushButton(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}

QString PullDialog::url() const
{
	return mUrl->text();
}

QString PullDialog::branch() const
{
	return mBranch->text();
}


