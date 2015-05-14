#include "conflictsWindow.h"

#include <QtWidgets/QAction>


using namespace versioning;
using versioning::details::ConflictsView;

ConflictsWindow::ConflictsWindow(DiffModel *diffModel, QWidget *parent) :
	QWidget(parent)
	, mDiffModel(diffModel)
	, mMainWindow(parent)
	, mSceneCustomizer(new SceneCustomizer)
	, mController(new Controller)
	, saveButton(nullptr)
	, mDiagrams(nullptr)
	, mUndo(new QAction(this))
	, mRedo(new QAction(this))
{
	this->addAction(mUndo);
	this->addAction(mRedo);

	initBaseLayout();
	initButton();
	initViews();
	this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

ConflictsWindow::~ConflictsWindow()
{
}

versioning::details::ConflictsView *ConflictsWindow::getNewModel()
{
	return mTheirsView;
}

void ConflictsWindow::initBaseLayout()
{
	mLayout = new QGridLayout(this);
	mLayout->setContentsMargins(5, 5, 5, 5);
	mDiagrams = new QTabWidget(this);
	mLayout->addWidget(mDiagrams,0,0);

	setLayout(mLayout);
}

void ConflictsWindow::initLayout()
{
	mLayout = new QGridLayout(this);

	mLayout->setContentsMargins(5, 5, 5, 5);
	mLayout->setColumnStretch(0, 10);
	mLayout->setRowStretch(0, 10);
	mLayout->setRowStretch(1, 0);


	setLayout(mLayout);
}

void ConflictsWindow::initButton()
{
	saveButton = new QPushButton(tr("resolve ours"), this);
	connect(saveButton, SIGNAL(clicked()),SIGNAL(processWorkingCopy()));
	mLayout->addWidget(saveButton,1,0);
}


void ConflictsWindow::initViews()
{
	QSplitter *verticalSplitter = new QSplitter(Qt::Vertical, this);
	verticalSplitter->setFrameStyle(QFrame::Sunken);

	QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
	QList<int> sizes;
	sizes << 1;

	QModelIndex oursIndex = mDiffModel->oldModel()->graphicalModel()->index(0, 0);
	Id oursRootId = mDiffModel->oldModel()->graphicalModelAssistApi().idByIndex(oursIndex);
	mOursView = new ConflictsView(mMainWindow, mDiffModel, false, *mController, *mSceneCustomizer, oursRootId);
	QFrame *oursFrame = new QFrame;
	oursFrame->setLayout(initView(mOursView));
	sizes << 1;
	splitter->addWidget(oursFrame);
	mController->diagramOpened(oursRootId);

	QModelIndex theirsIndex = mDiffModel->newModel()->graphicalModel()->index(0,0);
	Id theirsRootId = mDiffModel->newModel()->graphicalModelAssistApi().idByIndex(theirsIndex);
	mTheirsView = new ConflictsView(mMainWindow, mDiffModel, true, *mController, *mSceneCustomizer, theirsRootId);
	QFrame *theirsFrame = new QFrame;
	theirsFrame->setLayout(initView(mTheirsView));
	splitter->addWidget(theirsFrame);
	mController->diagramOpened(theirsRootId);

	splitter->setSizes(sizes);

	verticalSplitter->addWidget(splitter);

	mDiffDetailsWidget = new details::DiffDetailsWidget(mDiffModel, this);
	verticalSplitter->addWidget(mDiffDetailsWidget);
	mOursView->setDetailsWidget(mDiffDetailsWidget);
	mTheirsView->setDetailsWidget(mDiffDetailsWidget);
	mDiffDetailsWidget->setVisible(true);

	auto solveModel = mDiffModel->oldActiveModel();
	QModelIndex rootSolveIndex = solveModel->graphicalModel()->index(0, 0);
	Id rootId = solveModel->graphicalModelAssistApi().idByIndex(rootSolveIndex);
	mSolveView = new EditorView(*solveModel, *mController, *mSceneCustomizer, rootId, mMainWindow);
	mController->diagramOpened(rootId);

	mSolveView->mutableMvIface().configure(
		solveModel->graphicalModelAssistApi()
		, solveModel->logicalModelAssistApi()
		, solveModel->exploser()
	);
	mSolveView->mutableMvIface().setModel(solveModel->graphicalModel());
	mSolveView->mutableMvIface().setLogicalModel(solveModel->logicalModel());
	mSolveView->mutableMvIface().setRootIndex(rootSolveIndex);
	verticalSplitter->addWidget(mSolveView);

	QWidget *tmp = new QWidget;
	QGridLayout *layoutTmp = new QGridLayout;
	layoutTmp->addWidget(verticalSplitter);
	tmp->setLayout(layoutTmp);
	mDiagrams->addTab(tmp, "conflicts");

	QList<int> sizesForCopySplitter;
	sizesForCopySplitter << 1 << 1 << 1;
	verticalSplitter->setSizes(sizesForCopySplitter);
	verticalSplitter->setStretchFactor(0, 9);
	verticalSplitter->setStretchFactor(1, 0);
	verticalSplitter->setStretchFactor(2, 9);

	mController->setActiveDiagram(rootId);

	mUndo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
	mUndo->setText(tr("Undo"));
	connect(mUndo, &QAction::triggered, mController, &Controller::undo);

	mRedo->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Z));
	mRedo->setText(tr("Redo"));
	connect(mRedo, &QAction::triggered, mController, &Controller::redo);
}

QGridLayout *ConflictsWindow::initView(ConflictsView *view)
{
	QGridLayout *result = new QGridLayout;
	result->setColumnStretch(0, 10);
	result->setColumnStretch(1, 0);
	result->addWidget(view, 0, 0);
	return result;
}
