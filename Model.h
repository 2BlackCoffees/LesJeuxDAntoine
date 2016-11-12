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
#ifndef MODEL_H
#define MODEL_H

#include <QStringList>
#include <QVariant>

#include <vector>
#include <string>
#include <cassert>

class Observer;

class Model
{
    int mLevel;
    int mNbPoints = 0;
    std::vector<Observer*> mObservers;
    bool mReadMode;

public:
    Model();

    QStringList setLevelAndGetNewList(int level);

    int NumberOfImages() {
        return mImagePathsToShow.size();
    }

    void Clear() {
        mImagePathsToShow.clear();
        mImagesFound.clear();
        mImagesClicked.clear();
    }
    void Insert(int index, const QString & imagePath)
    {
        mImagePathsToShow.insert(index, imagePath);
    }

    void RemoveRows(int row, int count)
    {
        for (int index = 0; index < count; ++index) {
            mImagePathsToShow.removeAt(row);
        }
    }

    void RemoveRow(int index)
    {
        mImagePathsToShow.removeAt(index);
    }

    bool ImageAlreadyFound(int row) const {
        return std::find(mImagesFound.begin(),
                         mImagesFound.end(), row) != mImagesFound.end();
    }

    bool ImageAlreadyClicked(int row) const {
        return std::find(mImagesClicked.begin(),
                         mImagesClicked.end(), row) != mImagesClicked.end();
    }

    QString GetImageFromRow(int row) {
        return mImagePathsToShow.value(row);
    }

    QString GetTextForRectangle(int row) {
        if(getReadMode()) {
            return mImagePathsToShow.value(row).remove( QRegExp( "^.*\\/" )).remove(QRegExp( "\\..*$" ));
        } else {
            return "Clique moi :-)";
        }
    }

    bool WantImage(int row, bool clickedOnSameImage) {
        return ImageAlreadyFound(row) || clickedOnSameImage;
    }

    bool HasPointsLost(bool clickedOnSameImage) {
        return clickedOnSameImage && mPointsDifference < 0;
    }

    bool HasPointsWon(bool clickedOnSameImage) {
        return clickedOnSameImage && mPointsDifference > 0;
    }

    int NumberPointsChangeToShow() {
        return mPointsDifference;
    }

    bool HasFoundTheTwoImages(int curRow, int prevRow) {
        assert(curRow != prevRow);
        if(curRow == prevRow) return false;
        return mImagePathsToShow[curRow] == mImagePathsToShow[prevRow];
    }

    void ProcessFoundTheTwoImages(int curRow, int prevRow) {
        mImagesFound.push_back(curRow);
        mImagesFound.push_back(prevRow);
        mPointsDifference = 5;
    }

    void ProcessOpenAnAlreadyFoundImage() {
        mPointsDifference = -1;
        mErrorMade = true;
    }

    void ResetPointsDifference() {
        mPointsDifference = 0;
    }

    int GetNewLevel() {
        int level = getLevel();
        if(mNextLevelRequired) ++level;
        mNextLevelRequired = false;
        return level;
    }

    void SaveClickedImage(int row) {
        mImagesClicked.push_back(row);
    }

    int rowCount() const {
        // Provide information to the QML repeater
        return mImagePathsToShow.count();
    }
    int rowToDisplay() const {
        // Provide information to the Grid: Nb of rows
        return mImagePathsToShow.count() / mMaxCols;
    }
    int columnCount() const  {
        // Provide information to the grid: Nb of columns
        return mMaxCols;
    }
    int maxWidth() const {
        return mMaxWidth;
    }
    int maxHeight() const {
        return mMaxHeight;
    }
    QString getWinningText() const {
        QString returnString = mWinningText;
        mWinningText = "";
        return returnString;
    }
    QString startPathRed() const {
        return mStarPathRed;
    }
    QString startPathYellow() const {
        return mStarPathYellow;
    }
    bool isGoingToNextLevel() const {
        return mNextLevelRequired;
    }
    bool hadErrors() const {
        return mNbTimesLevelFullySuceeded == 0;
    }
    bool isAllWon() const {
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
    QString getBackgroundImagePath() const {
        return mAllBkGnds[mCurBkGnd];
    }
    QString getLocalDialogImage() const {
        return "Misc/TowTruckSawyer.png";
    }


    void setLevel(int level, Observer* fromObserver) {
        mLevel = level;
        update(fromObserver);
    }
    int getLevel() { return mLevel; }
    void increasePoints(int increasePoints, Observer* fromObserver) {
        mNbPoints += increasePoints;
        update(fromObserver);
    }
    int getNbPoints() { return mNbPoints; }
    void registerObserver(Observer* observer) {
        mObservers.push_back(observer);
    }
    void setReadMode(bool readMode, Observer* fromObserver) {
        mReadMode = readMode;
        update(fromObserver);
    }
    bool getReadMode() {
        return mReadMode;
    }

    void update(Observer* fromObserver);
private:
    const QString mStarPathYellow{"Misc/SmallStarYellow.png"};
    const QString mStarPathRed{"Misc/SmallStarRed.png"};
    int mMaxWidth{0};
    int mMaxHeight{0};
    QStringList mAllImagePaths;
    QStringList mAllBkGnds;
    QStringList mImagePathsToShow;
    std::vector<int> mImagesFound;
    std::vector<int> mImagesClicked;
    int mMaxCols{2};
    int mPointsDifference{0};
    unsigned int mCurBkGnd{0};
    mutable unsigned int mNbTimesLevelFullySuceeded{0};
    mutable unsigned int mErrorMade{false};
    mutable QString mWinningText;
    mutable bool mNextLevelRequired{false};

};

class Observer {
public:
    Observer(Observer* registerObserver, Model* model) {
        model->registerObserver(registerObserver);
    }

    virtual void update() = 0;
};

#endif // MODEL_H
