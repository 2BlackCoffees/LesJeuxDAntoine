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
#ifndef POINTSSTATUS_H
#define POINTSSTATUS_H

#include <Model.h>

#include <QtCore>
#include <QtGui>
#include <QtQml>

#include <string>
#include <map>
#include <cassert>

class PointsStatus : public QAbstractListModel, Observer
{
    Q_OBJECT

public:
    enum RoleSet
    {
        Points = 0,
        Level,
        Read,
        Options,
        Quit,
    };
    enum ActionRole {
        Clicked
    };

    Q_ENUMS(ActionRole)

private:
    enum RoleNames {
        DisplayRole = Qt::DisplayRole,
        CanClickRole = Qt::UserRole,
        CannotClickRole,
        CanCheckRole,
    };

    const QString DISPLAY_STR         {"display"};
    const QString CAN_CLICK_STR       {"canClick"};
    const QString CANNOT_CLICK_STR    {"cannotClick"};
    const QString CAN_CHECK_STR       {"canCheck"};

    QHash<int, QByteArray> mRoleNames {
        {DisplayRole,    DISPLAY_STR.toStdString().c_str()   },
        {CanClickRole,   CAN_CLICK_STR.toStdString().c_str()},
        {CannotClickRole,CANNOT_CLICK_STR.toStdString().c_str()},
        {CanCheckRole,   CAN_CHECK_STR.toStdString().c_str()},
    };


public:
    PointsStatus(QObject* parent = nullptr, Model* model = nullptr) :
        QAbstractListModel(parent), Observer(this, model), mModel(model) {
        assert(model);
    }

    int rowCount(const QModelIndex & /*parent = QModelIndex()*/) const override {
        return mValues.size();
    }
    int columnCount(const QModelIndex & /*parent = QModelIndex()*/) const override {
        return mValues.size();
    }
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex & /*index*/) const override
    {
        return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
    }
    QHash<int, QByteArray> roleNames() const override
    {
        return mRoleNames;
    }
    void update() override {
        emit dataChanged(createIndex(0, 0),
                         createIndex(rowCount(QModelIndex()), columnCount(QModelIndex())));
    }


    bool setData(const QModelIndex &, const QVariant &value, int role) override;

private:
    Model* mModel{nullptr};
    struct Status {
        Status(RoleSet&& valueType, RoleNames&& role = CannotClickRole, int value = -1, bool checked = false) :
            mValueType{std::move(valueType)},
            mRole{std::move(role)},
            mValue{value},
            mIsChecked{checked} {}
        RoleSet mValueType;
        RoleNames mRole{CannotClickRole};
        int mValue{-1};
        bool mIsChecked{false};
    };
    std::vector<Status> mValues{
        Status(Points),
                Status(Level),
                Status(Read,    CanClickRole, -1, true),
                Status(Options, CanClickRole),
                Status(Quit,    CanClickRole),
    };

};
#endif // POINTSSTATUS_H
