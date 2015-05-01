#pragma once

#include <QtWidgets/QSplitter>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QWidget>

#include <qrgui/editor/sceneCustomizer.h>
#include <qrgui/controller/controller.h>

#include "model/diffModel.h"
#include "diffView.h"

namespace versioning
{

class ConflictsWindow : public QWidget
{
	Q_OBJECT

public:
	explicit ConflictsWindow(DiffModel *diffModel, QWidget *parent = 0);
	~ConflictsWindow();
	details::DiffView *getNewModel();
	bool diagramHasConflicts();

signals:
	void processWorkingCopy();

private:
	void initBaseLayout();
	void initLayout();
	void initButton();
	void initViews();
	QGridLayout *initView(details::DiffView *view);

	DiffModel *mDiffModel;
	QGridLayout *mLayout;
	details::DiffView *mOldView;
	details::DiffView *mNewView;
	QWidget *mMainWindow;
	SceneCustomizer *mSceneCustomizer;
	Controller *mController;
	QPushButton *saveButton;
	QTabWidget *mDiagrams;

	int mDiagram;
};

}
