//
// Created by Екатерина Заболотских on 06.09.2020.
//

#ifndef SPEEDMEASURER_SPEEDMEASURERMANAGER_H
#define SPEEDMEASURER_SPEEDMEASURERMANAGER_H

#include "speedMeasurer.h"

class speedMeasurerManager {
public:
    virtual void addSpeedMeasurer(speedMeasurer & speedMeasurer);
    virtual void run();
    virtual void resultCallback(void (*callback)(float));
};

class SimpleThreadSpeedMeasurerManager : speedMeasurerManager {
public:
    void addSpeedMeasurer(speedMeasurer & speedMeasurer) override;
    void run() override;
    void resultCallback(void (*callback)(float)) override;
};



#endif //SPEEDMEASURER_SPEEDMEASURERMANAGER_H
