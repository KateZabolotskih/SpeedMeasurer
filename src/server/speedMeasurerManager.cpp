//
// Created by Екатерина Заболотских on 06.09.2020.
//

#include "speedMeasurerManager.h"

void DefaultSpeedMeasurerManager::addMeasuring(measure measure) {
    for(auto sm : measurers) {
        sm->addMeasuring(measure);
    }
}

void DefaultSpeedMeasurerManager::addSpeedMeasurer(SpeedMeasurer * speedMeasurer) {
    measurers.push_back(speedMeasurer);
}

DefaultSpeedMeasurerManager::~DefaultSpeedMeasurerManager() {
    for(auto sm : measurers) {
        delete sm;
    }
}

void DefaultSpeedMeasurerManager::resultCallback(SpeedMeasurer *speedMeasurer, result result) {
    numOfFinished++;
    if (numOfFinished == measurers.size()) {
        //TODO build result
        callback(result);
    }
}
