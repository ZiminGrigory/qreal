#include "conflictsView.h"

#include "model/elementType.h"


using namespace versioning;
using namespace versioning::details;
using namespace qReal;

ConflictsView::ConflictsView(
		QWidget *parent
		, DiffModel *diffModel
		, bool isTheirs
		, Controller &controller
		, const SceneCustomizer &customizer
		, const Id &rootId
		)
	: EditorView(
		  *(!isTheirs ? diffModel->oldModel() : diffModel->newModel())
		, controller
		, customizer
		, rootId
		, parent
	)
	, mScene(mutableScene())
	, mMVIface(mutableMvIface())
	, mDiffModel(diffModel)
	, mIsTheirs(isTheirs)
	, mModel(!isTheirs ? mDiffModel->oldModel() : mDiffModel->newModel())
{
	this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	mMVIface.configure(mModel->graphicalModelAssistApi(), mModel->logicalModelAssistApi(), mModel->exploser());
	mMVIface.setModel(mModel->graphicalModel());
	mMVIface.setLogicalModel(mModel->logicalModel());
	mMVIface.setRootIndex(mModel->graphicalModelAssistApi().indexById(rootId));

	mAddedColor = SettingsManager::value(
		"diffAddedRemovedColor"
		, ui::DiffColorPreferencesDialog::defaultAddedRemovedColor()).value<QColor>();

	mModifiedColor = SettingsManager::value
		("diffModifiedColor"
		, ui::DiffColorPreferencesDialog::defaultModifiedColor()).value<QColor>();

	mHintColor = SettingsManager::value(
		"diffHintColor"
		, ui::DiffColorPreferencesDialog::defaultHintColor()).value<QColor>();

	mRemovedColor = SettingsManager::value(
		"diffRemovedColor"
		, QColor(Qt::red)).value<QColor>();

	highlightElements();
}

void ConflictsView::setDetailsWidget(DiffDetailsWidget *widget)
{
	mDetailsWidget = widget;
	connect(mDetailsWidget, SIGNAL(mouseEnteredIdWidget(qReal::Id)),
		this, SLOT(hintItem(qReal::Id)));
	connect(mDetailsWidget, SIGNAL(mouseLeavedIdWidget(qReal::Id)),
			this, SLOT(unhintItem(qReal::Id)));
}


void ConflictsView::mousePressEvent(QMouseEvent *event)
{
	EditorView::mousePressEvent(event);

	if (event->button() != Qt::LeftButton) {
		event->ignore();
		return;
	}
	event->accept();

	if (QGraphicsItem *item = itemAt(event->pos())) {
		if (Element *elem = dynamic_cast <Element *>(item)) {
			onClick(elem->id());
		}
	} else {
		onClickCancel();
	}
}

void ConflictsView::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	event->ignore();
	event->accept();
}

void ConflictsView::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
	event->ignore();
	event->accept();
}

void ConflictsView::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
	event->ignore();
	event->accept();
}

void ConflictsView::dropEvent(QGraphicsSceneDragDropEvent *event)
{
	event->ignore();
	event->accept();
}

void ConflictsView::onClick(const qReal::Id &id)
{
	if (mDetailsWidget) {
		mDetailsWidget->setId(id);
	}
}

void ConflictsView::onClickCancel()
{
	if (mDetailsWidget) {
		mDetailsWidget->reset();
	}
}

void ConflictsView::highlightElements()
{
	qReal::IdList const ids = (!mIsTheirs) ? mDiffModel->oldGraphicalElements() : mDiffModel->newGraphicalElements();
	foreach (qReal::Id const &id, ids) {
		highlight(id);
	}
}

void ConflictsView::highlight(const qReal::Id &id)
{
	Difference *difference = mDiffModel->difference(id);
	if (difference) {
		DiffState const state = (PurelyGraphical == difference->elementType())
			? difference->graphicalDifference()->state()
			: difference->logicalDifference()->state();

		if (state != DiffState::Same) {
			QColor &color =
					(state == DiffState::Added)
					? mAddedColor
					: (state == DiffState::Modified ? mModifiedColor : mRemovedColor);
			mScene.highlight(id, false, color);
		}
	}
}

void ConflictsView::hintItem(const qReal::Id &graphicalId)
{
	mScene.highlight(graphicalId, false, mHintColor);
}

void ConflictsView::unhintItem(const qReal::Id &graphicalId)
{
	highlight(graphicalId);
}
