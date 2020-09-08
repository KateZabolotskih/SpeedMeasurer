//
// Created by Екатерина Заболотских on 06.09.2020.
//

#ifndef SPEEDMEASURER_SPEEDMEASURERMANAGER_H
#define SPEEDMEASURER_SPEEDMEASURERMANAGER_H

#include <vector>
#include "speedMeasurer.h"

using namespace std;


class SpeedMeasurerManager {
protected:
    virtual void resultCallback(SpeedMeasurer * speedMeasurer, result result) = 0;
public:
    virtual void addSpeedMeasurer(SpeedMeasurer * speedMeasurer) = 0;
    virtual void addMeasuring(measure measure) = 0;
};

class DefaultSpeedMeasurerManager : SpeedMeasurerManager {
    vector<SpeedMeasurer *> measurers;
    unsigned int numOfFinished;
    void (*callback)(result result);
    void resultCallback(SpeedMeasurer * speedMeasurer, result result) override;
public:
    explicit DefaultSpeedMeasurerManager(void (*callback)(result result)) : numOfFinished(0), callback(callback) {}
    void addSpeedMeasurer(SpeedMeasurer * speedMeasurer) override;
    void addMeasuring(measure measure) override;
    ~DefaultSpeedMeasurerManager();
};



#endif //SPEEDMEASURER_SPEEDMEASURERMANAGER_H
