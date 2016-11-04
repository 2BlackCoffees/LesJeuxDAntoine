#include "Model.h"

int Model::mLevel{2};
int Model::mNbPoints{0};
std::vector<Observer*> Model::mObservers;
bool Model::mReadMode{true};


Model::Model() {
}

void Model::update(Observer *fromObserver) {
    for(Observer* observer: mObservers) {
        if(observer != fromObserver) observer->update();
    }
}
