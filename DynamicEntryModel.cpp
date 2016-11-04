#include "DynamicEntryModel.h"
#include <stdlib.h>     /* srand, rand */
#include <ctime>        // std::time

#include <QDebug>

DynamicEntryModel::DynamicEntryModel(QObject *parent)
    : QAbstractTableModel(parent), Observer(this)
{

    QDirIterator resourceCars(":/CarLogos", QDirIterator::Subdirectories);
    while (resourceCars.hasNext()) {
        resourceCars.next();
        mAllImagePaths.append(resourceCars.filePath().remove( QRegExp( "^:\\/" ) ));
    }

    // Process max width and max height
    foreach(const QString& imagePath, mAllImagePaths) {
        auto image = QImage("://" + imagePath);
        mMaxWidth = std::max(mMaxWidth, image.width());
        mMaxHeight = std::max(mMaxHeight, image.height());
    }

    QDirIterator resourceBkGnd(":/BkGnds", QDirIterator::Subdirectories);
    while (resourceBkGnd.hasNext()) {
        resourceBkGnd.next();
        mAllBkGnds.append(resourceBkGnd.filePath().remove( QRegExp( "^:\\/" ) ));
    }
    mCurBkGnd = 0;

    setLevel(1);
    std::srand ( unsigned ( std::time(0) ) );
}

void DynamicEntryModel::setLevel(int level) {
    Model::setLevel(level, this);

    clear();
    std::vector<int> tmpList;
    mCurBkGnd = (level - 1) % mAllBkGnds.count();

    // Create pseudo randomized index
    auto size = mAllImagePaths.size();
    for(int index = 0; index < level + 1; ++index) {
        int curIndex = std::rand() % size;
        while(std::find(tmpList.begin(), tmpList.end(), curIndex) != tmpList.end()) {
            curIndex = (curIndex + 1) % size;
        }
        tmpList.push_back(curIndex);
        tmpList.push_back(curIndex);
    }
    std::random_shuffle ( tmpList.begin(), tmpList.end() );

    // Store them pseudo randomly
    for(auto it = tmpList.begin(), itEnd = tmpList.end(); it != itEnd; ++it) {
        //mImagePathsToShow.append(mAllImagePaths[*it]);
        append(mAllImagePaths[*it]);
    }
    mMaxCols = 2;
    if(level > 1) {
        for(int maxCol = 4; maxCol > 2; --maxCol) {
            if(((level + 1) * 2) % maxCol == 0) {
                mMaxCols = maxCol;
            }
        }
    }
    mPrevIndex = createIndex(-1, -1);
    mCurIndex = mPrevIndex;
}

DynamicEntryModel::~DynamicEntryModel()
{
}


void DynamicEntryModel::clear() {
    if(mImagePathsToShow.size() > 0) {
        removeRows(0, mImagePathsToShow.size(), QModelIndex());
    }
    mImagePathsToShow.clear();
    mImagesFound.clear();
    mImagesClicked.clear();
}

void DynamicEntryModel::insert(int index, const QString & imagePath)
{

    // view protocol (begin => manipulate => end]
    emit beginInsertRows(QModelIndex(), index, index);
    //m_data.insert(index, color);
    mImagePathsToShow.insert(index, imagePath);
    emit endInsertRows();
    // update our count property
    //emit countChanged(m_data.count());
}

void DynamicEntryModel::append(const QString & imagePath)
{
    insert(mImagePathsToShow.count(), imagePath);
}

bool DynamicEntryModel::removeRows(int row, int count, const QModelIndex&)
{
    beginRemoveRows( QModelIndex(), row, row + count - 1);

    for (int index = 0; index < count; ++index) {
        mImagePathsToShow.removeAt(row);
    }

    endRemoveRows();

    return true;
}

void DynamicEntryModel::removeRow(int index, const QModelIndex&)
{
    if(index < 0 || index >= mImagePathsToShow.count()) {
        return;
    }
    emit beginRemoveRows(QModelIndex(), index, index);
    mImagePathsToShow.removeAt(index);
    emit endRemoveRows();
    // do not forget to update our count property
    //emit countChanged(m_data.count());
}

bool DynamicEntryModel::ImageAlreadyFound(int row) const {
    return std::find(mImagesFound.begin(),
                     mImagesFound.end(), row) != mImagesFound.end();
}

bool DynamicEntryModel::ImageAlreadyClicked(int row) const {
    return std::find(mImagesClicked.begin(),
                     mImagesClicked.end(), row) != mImagesClicked.end();
}

QVariant DynamicEntryModel::data(const QModelIndex &index, int role) const
{
    // The repeater works only linearly with row information
    int row = index.row();

    // boundary check for the row
    /*    if(row < 0 || row > rowCount()) {
        return QVariant();
    }*/

    // A model can return data for different roles.
    // The default role is the display role.
    // it can be accesses in QML with "model.display"
    switch(role) {
    case DisplayRole:
        // Return the color name for the particular row
        // Qt automatically converts it to the QVariant type
        return mImagePathsToShow.value(row);

    case TextOnRectangleRole:
        if(Model::getReadMode()) {
            return mImagePathsToShow.value(row).remove( QRegExp( "^.*\\/" )).remove(QRegExp( "\\..*$" ));
        } else {
            return "Clique moi :-)";
        }
    case WantImageRole:

        return ImageAlreadyFound(row) || index == mCurIndex;

    case PointsLostRole:
        return index == mCurIndex && mPointsDifference < 0;

    case PointsWonRole:
        return index == mCurIndex && mPointsDifference > 0;

    case NumberPointsChangeToShowRole:
        return mPointsDifference;

    case NumberPointsRole:
        return Model::getNbPoints();

    case OpacityRole:
        return ImageAlreadyFound(row) ? 0.3 : 1.0;

    case IsAllWonRole:
        return isAllWon();

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
        bool pointsChanged = false;
        mPrevIndex = mCurIndex;
        mCurIndex = index(row, 0, QModelIndex());

        if(mPrevIndex.isValid() && mPrevIndex != mCurIndex &&
                mImagePathsToShow[row] == mImagePathsToShow[mPrevIndex.row()]) {
            mImagesFound.push_back(row);
            mImagesFound.push_back(mPrevIndex.row());
            mPointsDifference = 5;
            pointsChanged = true;
        } else if(!ImageAlreadyClicked(row)) {
            mImagesClicked.push_back(row);
        } else if(!ImageAlreadyFound(row)){
            mPointsDifference = -1;
            mErrorMade = true;
            pointsChanged = true;
        }


        if(pointsChanged) {
            Model::increasePoints(mPointsDifference, this);
        } else {
            mPointsDifference = 0;
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
        if(mNextLevelRequired) {
            setLevel(Model::getLevel() + 1);
        } else {
            setLevel(Model::getLevel());
        }
        mNextLevelRequired = false;
        break;

    case ForceRepaintRole:
        emit dataChanged(createIndex(0, 0),
                         createIndex(rowCount(QModelIndex()), 0));
        break;

    case ResetPointsValueRole:
        mPointsDifference = 0;
        break;
    }
    return true;
}

QColor DynamicEntryModel::get(int index)
{
    if(index < 0 || index >= mImagePathsToShow.count()) {
        return QColor();
    }
    return mImagePathsToShow.at(index);
}
