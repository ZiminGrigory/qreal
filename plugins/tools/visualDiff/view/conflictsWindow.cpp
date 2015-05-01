#include "conflictsWindow.h"


using namespace versioning;

ConflictsWindow::ConflictsWindow(DiffModel *diffModel, QWidget *parent) :
	QWidget(parent)
	, mDiffModel(diffModel)
	, mMainWindow(parent)
	, mSceneCustomizer(new SceneCustomizer)
	, mController(new Controller)
{
	this->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	initLayout();
	initButton();
	initViews();
	QList<int> sizes;
	sizes << 5 << 2;
	mSplitter->setSizes(sizes);
	mSplitter->setStretchFactor(0, 5);
	mSplitter->setStretchFactor(1, 2);

}

ConflictsWindow::~ConflictsWindow()
{
}

versioning::details::DiffView *ConflictsWindow::getNewModel()
{
	return mNewView;
}

void ConflictsWindow::initLayout()
{
	mLayout = new QGridLayout(this);

	mLayout->setContentsMargins(5, 5, 5, 5);
	mLayout->setColumnStretch(0, 10);
	mLayout->setRowStretch(0, 10);
	mLayout->setRowStretch(1, 0);

	mSplitter = new QSplitter(Qt::Vertical, this);
	mSplitter->setFrameStyle(QFrame::Sunken);
	mLayout->addWidget(mSplitter);
	setLayout(mLayout);
}

void ConflictsWindow::initButton()
{
	///@to do: make it, add save button, on clicked -> emit signal to wrapper for process and reopen project
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
	mSplitter->addWidget(splitter);
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
