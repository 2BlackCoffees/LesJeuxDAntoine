#ifndef QMLINTERFACE_H
#define QMLINTERFACE_H

#include "DynamicEntryModel.h"
#include "PointsStatus.h"
#include "Model.h"

#include <QObject>
#include <QQuickView>

#include <memory>

class QMLInterface : public QObject
{
    Q_OBJECT
public:
    explicit QMLInterface(QObject *parent = nullptr) :
        mModel{new Model()},
        mDynamicEntryModel{new DynamicEntryModel(parent, mModel.get())},
        mPointsStatus{new PointsStatus(parent, mModel.get())} {
    }
    virtual ~QMLInterface();

    Q_INVOKABLE int rowCount() const {
        // Provide information to the QML repeater
        return mDynamicEntryModel->rowCount();
    }
    Q_INVOKABLE int rowToDisplay() const {
        return mDynamicEntryModel->rowToDisplay();
    }
    Q_INVOKABLE int columnCount() const  {
        return mDynamicEntryModel->columnCount();
    }
    Q_INVOKABLE int maxWidth() const {
        return mDynamicEntryModel->maxWidth();
    }
    Q_INVOKABLE int maxHeight() const {
        return mDynamicEntryModel->maxHeight();
    }
    Q_INVOKABLE QString getWinningText() const {
        return mDynamicEntryModel->getWinningText();

    }
    Q_INVOKABLE QString startPathRed() const {
        return mDynamicEntryModel->startPathRed();
    }
    Q_INVOKABLE QString startPathYellow() const {
        return mDynamicEntryModel->startPathYellow();
    }
    Q_INVOKABLE bool isGoingToNextLevel() const {
        return mDynamicEntryModel->isGoingToNextLevel();
    }
    Q_INVOKABLE bool hadErrors() const {
        return mDynamicEntryModel->hadErrors();
    }
    Q_INVOKABLE bool isAllWon() const {
        return mDynamicEntryModel->isAllWon();

    }
    Q_INVOKABLE QString getBackgroundImagePath() const {
        return mDynamicEntryModel->getBackgroundImagePath();
    }
    Q_INVOKABLE QString getLocalDialogImage() const {
        return mDynamicEntryModel->getLocalDialogImage();
    }

    Q_INVOKABLE DynamicEntryModel* getDynamicEntryModel() {
        return mDynamicEntryModel.get();
    }

    Q_INVOKABLE PointsStatus* getPointsStatus() {
        return mPointsStatus.get();
    }


private:
    std::unique_ptr<Model> mModel;
    std::unique_ptr<DynamicEntryModel> mDynamicEntryModel;
    std::unique_ptr<PointsStatus> mPointsStatus;

};

#endif // QMLINTERFACE_H
