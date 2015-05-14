#pragma once

#include <qrgui/editor/editorView.h>
#include <qrgui/models/models.h>
#include <qrgui/editor/sceneCustomizer.h>
#include <qrgui/controller/controller.h>

#include "model/diffModel.h"
#include "view/diffColorPreferencesDialog.h"
#include "view/detailsWidget/diffDetailsWidget.h"


namespace versioning
{
namespace details
{

class ConflictsView : public EditorView
{
	Q_OBJECT

public:
	ConflictsView(
		QWidget *parent
		, versioning::DiffModel *diffModel
		, bool isTheirs
		, Controller &controller
		, const SceneCustomizer &customizer
		, const Id &rootId
	);

	void highlightElements();
	void setDetailsWidget(DiffDetailsWidget *widget);

protected:
	virtual void mousePressEvent(QMouseEvent *event);

	void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
	void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
	void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
	void dropEvent(QGraphicsSceneDragDropEvent *event);

	virtual void onClick(qReal::Id const &id);
	virtual void onClickCancel();

private slots:
	void hintItem(qReal::Id const &graphicalId);
	void unhintItem(qReal::Id const &graphicalId);

private:
	EditorViewScene &mScene;
	EditorViewMViface &mMVIface;
	void highlight(qReal::Id const &id);

	DiffModel *mDiffModel;
	bool mIsTheirs;
	models::Models *mModel;
	DiffDetailsWidget *mDetailsWidget;

	QColor mAddedColor;
	QColor mRemovedColor;
	QColor mModifiedColor;
	QColor mHintColor;
};

}
}
