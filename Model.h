#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>

class Observer;

class Model
{
    static int mLevel;
    static int mNbPoints;
    static std::vector<Observer*> mObservers;
    static bool mReadMode;

public:
    Model();

    static void setLevel(int level, Observer* fromObserver) {
        mLevel = level;
        update(fromObserver);
    }
    static int getLevel() { return mLevel; }
    static void increasePoints(int increasePoints, Observer* fromObserver) {
        mNbPoints += increasePoints;
        update(fromObserver);
    }
    static int getNbPoints() { return mNbPoints; }
    static void registerObserver(Observer* observer) {
        mObservers.push_back(observer);
    }
    static void setReadMode(bool readMode, Observer* fromObserver) {
        mReadMode = readMode;
        update(fromObserver);
    }
    static bool getReadMode() {
        return mReadMode;
    }

    static void update(Observer* fromObserver);

};

class Observer {
public:
    Observer(Observer* registerObserver) {
        Model::registerObserver(registerObserver);
    }

    virtual void update() = 0;
};

#endif // MODEL_H
