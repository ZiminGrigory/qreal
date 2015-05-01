#include "conflictsWindow.h"


using namespace versioning;

ConflictsWindow::ConflictsWindow(DiffModel *diffModel, QWidget *parent) :
	QWidget(parent)
	, mDiffModel(diffModel)
	, mMainWindow(parent)
	, mSceneCustomizer(new SceneCustomizer)
	, mController(new Controller)
{
	initBaseLayout();
	initButton();
	initViews();
//	QList<int> sizes;
//	sizes << 5 << 5;
//	mSplitter->setSizes(sizes);
//	mSplitter->setStretchFactor(0, 5);
//	mSplitter->setStretchFactor(1, 5);
	this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

}

ConflictsWindow::~ConflictsWindow()
{
}

versioning::details::DiffView *ConflictsWindow::getNewModel()
{
	return mNewView;
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
	saveButton = new QPushButton(tr("save"), this);
	connect(saveButton, SIGNAL(clicked()),SIGNAL(processWorkingCopy()));
	mLayout->addWidget(saveButton,1,0);
}


void ConflictsWindow::initViews()
{
	QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
	QList<int> sizes;
	sizes << 1;

	QModelIndex indexForOld = mDiffModel->oldModel()->graphicalModel()->index(0, 0);
	Id rootIdForOld = mDiffModel->oldModel()->graphicalModelAssistApi().idByIndex(indexForOld);
	mOldView = new details::DiffView(mMainWindow, mDiffModel, true, *mController, *mSceneCustomizer, rootIdForOld);
	QFrame *oldFrame = new QFrame;
	oldFrame->setLayout(initView(mOldView));
	sizes << 1;
	splitter->addWidget(oldFrame);


	QModelIndex indexForNew = mDiffModel->newModel()->graphicalModel()->index(0,0);
	Id rootIdForNew = mDiffModel->newModel()->graphicalModelAssistApi().idByIndex(indexForNew);
	mNewView = new details::DiffView(mMainWindow, mDiffModel, false, *mController, *mSceneCustomizer, rootIdForNew);
	QFrame *newFrame = new QFrame;
	newFrame->setLayout(initView(mNewView));
	splitter->addWidget(newFrame);

	splitter->setSizes(sizes);
	QWidget *tmp = new QWidget;
	QGridLayout *layoutTmp = new QGridLayout;
	layoutTmp->addWidget(splitter);
	tmp->setLayout(layoutTmp);
	mDiagrams->addTab(tmp, "ololo1");
}

QGridLayout *ConflictsWindow::initView(details::DiffView *view)
{
	QGridLayout *result = new QGridLayout;
	result->setColumnStretch(0, 10);
	result->setColumnStretch(1, 0);
	result->addWidget(view, 0, 0);
	QSlider *zoomSlider = new QSlider(Qt::Horizontal, this);
	zoomSlider->setMinimum(0);
	zoomSlider->setMaximum(100);
	zoomSlider->setValue(50);
	connect(zoomSlider, SIGNAL(valueChanged(int)), view, SLOT(adjustZoom(int)));
	view->adjustZoom(zoomSlider->value());
	result->addWidget(zoomSlider, 1, 0);
	return result;
}
