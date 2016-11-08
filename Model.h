#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>

class Observer;

class Model
{
    int mLevel{1};
    int mNbPoints{0};
    std::vector<Observer*> mObservers;
    bool mReadMode{true};

public:
    Model();

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

};

class Observer {
public:
    Observer(Observer* registerObserver, Model* model) {
        model->registerObserver(registerObserver);
    }

    virtual void update() = 0;
};

#endif // MODEL_H
