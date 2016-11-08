#ifndef DATAENTRYMODEL_H
#define DATAENTRYMODEL_H

#include "Model.h"

#include <QtCore>
#include <QtGui>
#include <QtQml>

#include <vector>

class DynamicEntryModel : public QAbstractTableModel, Observer
{
    Q_OBJECT
public:
    enum RoleNames {
        DisplayRole = Qt::DisplayRole,
        ClickRole = Qt::UserRole,
        WantImageRole = Qt::UserRole + 1,
        PointsLostRole,
        PointsWonRole,
        NumberPointsChangeToShowRole,
        NumberPointsRole,
        OpacityRole,
        IsAllWonRole,
        TextOnRectangleRole,
    };

    enum RoleSet
    {
        ClickOnImageRole,
        ClickOnRectangleRole,
        AllIsWonRole,
        ForceRepaintRole,
        ResetPointsValueRole
    };
    Q_ENUMS(RoleSet)

    explicit DynamicEntryModel(QObject *parent, Model* model);
    ~DynamicEntryModel();
    // inserts a color at the index (0 at begining, count-1 at end)
    Q_INVOKABLE void insert(int index, const QString & imagePath);
    // uses insert to insert a color at the end
    Q_INVOKABLE void append(const QString & imagePath);
    // removes a color from the index
    Q_INVOKABLE void removeRow(int index, const QModelIndex&);
    // clear the whole model (e.g. reset)
    Q_INVOKABLE void clear();
    // gets a color at the index
    Q_INVOKABLE QColor get(int index);

    virtual QVariant data(const QModelIndex & index, int role) const;

    Q_INVOKABLE int rowCount() const {
        // Provide information to the QML repeater
        return mImagePathsToShow.count();
    }
    Q_INVOKABLE int rowCount(const QModelIndex & /*parent = QModelIndex()*/) const override {
        // Provide information to the QML repeater
        return rowCount();
    }
    Q_INVOKABLE int rowToDisplay() const {
        // Provide information to the Grid: Nb of rows
        return mImagePathsToShow.count() / mMaxCols;
    }
    Q_INVOKABLE int columnCount() const  {
        // Provide information to the grid: Nb of columns
        return mMaxCols;
    }
    Q_INVOKABLE int columnCount(const QModelIndex & /*parent = QModelIndex()*/) const override {
        // Provide information to the grid: Nb of columns
        return columnCount();
    }
    Q_INVOKABLE int maxWidth() const {
        return mMaxWidth;
    }
    Q_INVOKABLE int maxHeight() const {
        return mMaxHeight;
    }
    Q_INVOKABLE QString getWinningText() const {
        QString returnString = mWinningText;
        mWinningText = "";
        return returnString;
    }
    Q_INVOKABLE QString startPathRed() const {
        return mStarPathRed;
    }
    Q_INVOKABLE QString startPathYellow() const {
        return mStarPathYellow;
    }
    Q_INVOKABLE bool isGoingToNextLevel() const {
        return mNextLevelRequired;
    }
    Q_INVOKABLE bool hadErrors() const {
        return mNbTimesLevelFullySuceeded == 0;
    }
    Q_INVOKABLE bool isAllWon() const {
        if(static_cast<unsigned int>(mImagePathsToShow.size()) ==
                mImagesFound.size()) {
            if(mWinningText.isEmpty()) {
                if(!mErrorMade) {
                    mNbTimesLevelFullySuceeded++;
                    if(mNbTimesLevelFullySuceeded >= 2) {
                        mNbTimesLevelFullySuceeded = 0;
                        mNextLevelRequired = true;
                        mWinningText = "Encore un sans fautes !!!\n Prepare toi\nau prochain niveau !!";
                    } else {
                        mWinningText = "Bravo c'est un sans fautes !!\nEncore une fois un sans fautes et ... \nc'est le prochain niveau !!";
                    }
                } else {
                    mNbTimesLevelFullySuceeded = 0;
                    mWinningText = "Quelques petites erreurs,\nil faut faire deux fois de suite\nun sans fautes pour passer\nau niveau suivant ...";
                }
                mErrorMade = false;
            }
            return true;
        }
        return false;
    }
    Q_INVOKABLE QString getBackgroundImagePath() const {
        return mAllBkGnds[mCurBkGnd];
    }
    Q_INVOKABLE QString getLocalDialogImage() const {
        return "Misc/TowTruckSawyer.png";
    }

    QHash<int, QByteArray> roleNames() const override
    {
        return mRoleNames;
    }

    bool setData(const QModelIndex &/*index*/, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex & /*index*/) const
    {
        return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
    }
    QModelIndex index(int row,int column,QModelIndex parent) const {
        Q_UNUSED(parent);
        return createIndex(row, column, nullptr);
    }
    void update() override {
        emit dataChanged(createIndex(0, 0),
                         createIndex(rowCount(QModelIndex()), 0));
    }

    bool removeRows(int row, int count, const QModelIndex &);
private:
    const QString CLICK_STR                 {"click"};
    const QString DISPLAY_STR               {"display"};
    const QString WANT_IMAGE_STR            {"wantImage"};
    const QString POINTS_LOST_STR           {"pointsLost"};
    const QString POINTS_WON_STR            {"pointsWon"};
    const QString POINTS_CHANGED_TO_SHOW_STR{"pointsChangeToShow"};
    const QString POINTS_NUMBER_STR         {"pointsNumber"};
    const QString OPACITY_STR               {"opacity"};
    const QString ALL_IS_WON_STR            {"allIsWon"};
    const QString TEXT_ON_RECTANGLE_STR     {"textOnRectangle"};

    QHash<int, QByteArray> mRoleNames{
        {ClickRole,                    CLICK_STR.toStdString().c_str()     },
        {DisplayRole,                  DISPLAY_STR.toStdString().c_str()   },
        {WantImageRole,                WANT_IMAGE_STR.toStdString().c_str()},
        {PointsLostRole,               POINTS_LOST_STR.toStdString().c_str()},
        {PointsWonRole,                POINTS_WON_STR.toStdString().c_str()},
        {NumberPointsChangeToShowRole, POINTS_CHANGED_TO_SHOW_STR.toStdString().c_str()},
        {NumberPointsRole,             POINTS_NUMBER_STR.toStdString().c_str()},
        {OpacityRole,                  OPACITY_STR.toStdString().c_str()},
        {IsAllWonRole,                 ALL_IS_WON_STR.toStdString().c_str()},
        {TextOnRectangleRole,          TEXT_ON_RECTANGLE_STR.toStdString().c_str()},
    };
    QModelIndex mPrevIndex;
    QModelIndex mCurIndex;
    const QString mStarPathYellow{"Misc/SmallStarYellow.png"};
    const QString mStarPathRed{"Misc/SmallStarRed.png"};
    int mMaxWidth{0};
    int mMaxHeight{0};
    QStringList mAllImagePaths;
    QStringList mAllBkGnds;
    QStringList mImagePathsToShow;
    std::vector<int> mImagesFound;
    std::vector<int> mImagesClicked;
    bool ImageAlreadyFound(int row) const;
    int mMaxCols{2};
    void setLevel(int level);
    bool ImageAlreadyClicked(int row) const;
    int mPointsDifference{0};
    unsigned int mCurBkGnd{0};
    Model* mModel{nullptr};
    mutable unsigned int mNbTimesLevelFullySuceeded{0};
    mutable unsigned int mErrorMade{false};
    mutable QString mWinningText;
    mutable bool mNextLevelRequired{false};
};

#endif // DATAENTRYMODEL_H
