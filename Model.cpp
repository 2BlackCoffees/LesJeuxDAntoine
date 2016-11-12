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
#include "Model.h"

#include <QDirIterator>
#include <QString>
#include <QImage>

#include <stdlib.h>     /* srand, rand */
#include <ctime>        // std::time

Model::Model() {
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

    std::srand ( unsigned ( std::time(0) ) );

}

QStringList Model::setLevelAndGetNewList(int level) {
    // TODO: This is to be done in client !
    //setLevel(level, this);

    Clear();
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

    mMaxCols = 2;
    if(level > 1) {
        for(int maxCol = 4; maxCol > 2; --maxCol) {
            if(((level + 1) * 2) % maxCol == 0) {
                mMaxCols = maxCol;
            }
        }
    }

    QStringList returnList;
    for(auto it = tmpList.begin(), itEnd = tmpList.end(); it != itEnd; ++it) {
        returnList.append(mAllImagePaths[*it]);
    }

    return returnList;
}

void Model::update(Observer *fromObserver) {
    for(Observer* observer: mObservers) {
        if(observer != fromObserver) observer->update();
    }
}
