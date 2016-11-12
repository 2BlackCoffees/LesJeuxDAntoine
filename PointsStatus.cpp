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
#include "PointsStatus.h"

#include <QApplication>

QVariant PointsStatus::data(const QModelIndex &index, int role) const {
    int row = index.row();
    switch(role) {
    case DisplayRole:
    {
        // Return the color name for the particular row
        // Qt automatically converts it to the QVariant type
        Status tmpValue = mValues[row];
        assert(mModel);
        if(tmpValue.mValueType == Points) {
            return "Points:\n" + QString::number(mModel->getNbPoints());
        } else if(tmpValue.mValueType == Level) {
            return "Level:\n" + QString::number(mModel->getLevel());
        } else if(tmpValue.mValueType == Read) {
            if(!tmpValue.mIsChecked) return "Not\nreading";
            else return "Reading";
        } else if(tmpValue.mValueType == Quit) {
            return "Quit";
        } else if(tmpValue.mValueType == Options) {
            return "Options";
        }
    }
        break;
    case CanClickRole:
        return mValues[row].mRole == CanClickRole;
        break;

    }
    return QVariant();
}

bool PointsStatus::setData(const QModelIndex & /*index*/, const QVariant & value, int role)
{
    auto row = value.toInt();
    switch(role) {
    case Clicked:
        if(row == Quit) {
            QApplication::quit();
        } else if(row == Read) {
            assert(mModel);
            mValues[row].mIsChecked ^= true;
            mModel->setReadMode(mValues[row].mIsChecked, this);
            emit dataChanged(createIndex(row, 0), createIndex(row, 0));
        }
    }
    return true;
}
