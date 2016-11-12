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
        return mModel->rowCount();
    }
    Q_INVOKABLE int rowToDisplay() const {
        return mModel->rowToDisplay();
    }
    Q_INVOKABLE int columnCount() const  {
        return mModel->columnCount();
    }
    Q_INVOKABLE int maxWidth() const {
        return mModel->maxWidth();
    }
    Q_INVOKABLE int maxHeight() const {
        return mModel->maxHeight();
    }
    Q_INVOKABLE QString getWinningText() const {
        return mModel->getWinningText();

    }
    Q_INVOKABLE QString startPathRed() const {
        return mModel->startPathRed();
    }
    Q_INVOKABLE QString startPathYellow() const {
        return mModel->startPathYellow();
    }
    Q_INVOKABLE bool isGoingToNextLevel() const {
        return mModel->isGoingToNextLevel();
    }
    Q_INVOKABLE bool hadErrors() const {
        return mModel->hadErrors();
    }
    Q_INVOKABLE bool isAllWon() const {
        return mModel->isAllWon();

    }
    Q_INVOKABLE QString getBackgroundImagePath() const {
        return mModel->getBackgroundImagePath();
    }
    Q_INVOKABLE QString getLocalDialogImage() const {
        return mModel->getLocalDialogImage();
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
