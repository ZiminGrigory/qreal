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
#include "conflictsView.h"
#include "view/detailsWidget/diffDetailsWidget.h"

namespace versioning
{

class ConflictsWindow : public QWidget
{
	Q_OBJECT

public:
	explicit ConflictsWindow(DiffModel *diffModel, QWidget *parent = 0);
	~ConflictsWindow();
	details::ConflictsView *getNewModel();
	bool diagramHasConflicts();

signals:
	void processWorkingCopy();

private:
	void initBaseLayout();
	void initLayout();
	void initButton();
	void initViews();
	QGridLayout *initView(details::ConflictsView *view);

	DiffModel *mDiffModel;
	QGridLayout *mLayout;
	details::ConflictsView *mOursView;
	details::ConflictsView *mTheirsView;
	EditorView *mSolveView;
	details::DiffDetailsWidget *mDiffDetailsWidget;
	QWidget *mMainWindow;
	SceneCustomizer *mSceneCustomizer;
	Controller *mController;
	QPushButton *saveButton;
	QTabWidget *mDiagrams;

	QAction *mUndo;
	QAction *mRedo;

	int mDiagram;
};

}
