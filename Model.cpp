#include "Model.h"

Model::Model() {
}

void Model::update(Observer *fromObserver) {
    for(Observer* observer: mObservers) {
        if(observer != fromObserver) observer->update();
    }
}
