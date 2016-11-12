/*
 *  This file is part of LesJeuxDAntoine.
 *
 *  LesJeuxDAntoine is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  EasyTimeTracker is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with EasyTimeTracker.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  LesJeuxDAntoine: First creation by 2BlackCoffees: http://www.twoblackcoffees.com/
 *
 **/
#include "DynamicEntryModel.h"
#include <stdlib.h>     /* srand, rand */
#include <ctime>        // std::time

#include <QDebug>

#include <cassert>

DynamicEntryModel::DynamicEntryModel(QObject* parent, Model* model)
    : QAbstractTableModel(parent), Observer(this, model), mModel(model)
{
  setLevel(1);
}

void DynamicEntryModel::setLevel(int level) {
    assert(mModel);
    mModel->setLevel(level, this);

    clear();
    QStringList tmpList = mModel->setLevelAndGetNewList(level);
    // Store them pseudo randomly
    for(auto it = tmpList.begin(), itEnd = tmpList.end(); it != itEnd; ++it) {
        append(*it);
    }

    mPrevIndex = createIndex(-1, -1);
    mCurIndex = mPrevIndex;
}

DynamicEntryModel::~DynamicEntryModel()
{
}


void DynamicEntryModel::insert(int index, const QString & imagePath)
{

    emit beginInsertRows(QModelIndex(), index, index);
    mModel->Insert(index, imagePath);
    emit endInsertRows();
}

void DynamicEntryModel::append(const QString & imagePath)
{
    insert(mModel->NumberOfImages(), imagePath);
}

bool DynamicEntryModel::removeRows(int row, int count, const QModelIndex&)
{
    beginRemoveRows( QModelIndex(), row, row + count - 1);
    mModel->RemoveRows(row, count);
    endRemoveRows();

    return true;
}

void DynamicEntryModel::removeRow(int index, const QModelIndex&)
{
    emit beginRemoveRows(QModelIndex(), index, index);
    mModel->RemoveRow(index);
    emit endRemoveRows();

}


QVariant DynamicEntryModel::data(const QModelIndex &index, int role) const
{
    // The QML repeater works only linearly with row information
    int row = index.row();
    QVariant dbgValue;
    switch(role) {
    case DisplayRole:
        dbgValue = mModel->GetImageFromRow(row);
        return mModel->GetImageFromRow(row);

    case TextOnRectangleRole:
        dbgValue = mModel->GetTextForRectangle(row);

        return mModel->GetTextForRectangle(row);

    case WantImageRole:
        dbgValue = mModel->WantImage(row, index == mCurIndex);

        return mModel->WantImage(row, index == mCurIndex);

    case PointsLostRole:
        return mModel->HasPointsLost(index == mCurIndex);

    case PointsWonRole:
        return mModel->HasPointsWon(index == mCurIndex);

    case NumberPointsChangeToShowRole:
        return mModel->NumberPointsChangeToShow();

    case NumberPointsRole:
        return mModel->getNbPoints();

    case OpacityRole:
        return mModel->ImageAlreadyFound(row) ? 0.3 : 1.0;

    case IsAllWonRole:
        return mModel->isAllWon();

    }
    // The view asked for other data, just return an empty QVariant
    return QVariant();
}

bool DynamicEntryModel::setData(const QModelIndex & /*index*/, const QVariant & value, int role)
{
    auto row = value.toInt();
    //QModelIndex tmpIndex = mPrevIndex;
    switch(role) {
    case ClickOnImageRole:
    {
        auto tmpIndex = mCurIndex;
        mPrevIndex = createIndex(-1, -1);
        mCurIndex = createIndex(-1, -1);
        if(tmpIndex.isValid()) {
            emit dataChanged(tmpIndex, tmpIndex);
        }
    }
        break;

    case ClickOnRectangleRole:
    {
        mPrevIndex = mCurIndex;
        mCurIndex = index(row, 0, QModelIndex());

        bool pointsChanged = false;
        if(mPrevIndex.isValid() && mPrevIndex != mCurIndex &&
                mModel->HasFoundTheTwoImages(row, mPrevIndex.row())) {
            mModel->ProcessFoundTheTwoImages(row, mPrevIndex.row());
            pointsChanged = true;

        } else if(!mModel->ImageAlreadyClicked(row)) {
            mModel->SaveClickedImage(row);
        } else if(!mModel->ImageAlreadyFound(row)) {
            mModel->ProcessOpenAnAlreadyFoundImage();
            pointsChanged = true;
        }


        if(pointsChanged) {
            assert(mModel);
            mModel->increasePoints(mModel->NumberPointsChangeToShow(), this);
        } else {
            mModel->ResetPointsDifference();
        }
        if(mCurIndex.isValid()) {
            emit dataChanged(mCurIndex, mCurIndex);
        }
        if(mPrevIndex.isValid() && mCurIndex != mPrevIndex) {
            emit dataChanged(mPrevIndex, mPrevIndex);
        }
    }
        break;
    case AllIsWonRole:
        setLevel(mModel->GetNewLevel());
        break;

    case ForceRepaintRole:
        emit dataChanged(createIndex(0, 0),
                         createIndex(rowCount(QModelIndex()), 0));
        break;

    case ResetPointsValueRole:
        mModel->ResetPointsDifference();
        break;
    }
    return true;
}


