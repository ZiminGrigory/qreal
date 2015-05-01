#include "resetDialog.h"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFileDialog>

#include <qrkernel/settingsManager.h>


using namespace git::ui;

ResetDialog::ResetDialog(QWidget *parent)
	: QDialog(parent)
{
	mHashCommitLabel = new QLabel(tr("Enter hash of commit: "));
	mHashCommitLineEdit = new QLineEdit(this);
	QPushButton *ok = new QPushButton(tr("OK"), this);
	QPushButton *cancel = new QPushButton(tr("Cancel"), this);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(ok);
	buttonsLayout->addWidget(cancel);

	connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(mHashCommitLabel, 0, 0);
	mainLayout->addWidget(mHashCommitLineEdit, 0, 1, 1, 2);
	mainLayout->addLayout(buttonsLayout,2, 0, 1, 3);
	setLayout(mainLayout);

	setWindowTitle(tr("Reset options"));
	setMinimumSize(500,100);
}

QPushButton *ResetDialog::createButton(const QString &text, const char *member)
{
	QPushButton *button = new QPushButton(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}

QString ResetDialog::hashCommit() const
{
	return mHashCommitLineEdit->text();
}


