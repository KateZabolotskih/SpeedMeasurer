//
// Created by Екатерина Заболотских on 06.09.2020.
//




#ifndef SPEEDMEASURER_SPEEDMEASURER_H
#define SPEEDMEASURER_SPEEDMEASURER_H

#include "filter.h"
#include <cstdint>

class SpeedMeasurer {
protected:
    float D;
    float S;
    float L;
    float B;
    float BRange;
    StreamFilter * filter;
public:
    SpeedMeasurer(StreamFilter * filter) : filter(filter) {};
    virtual void addMeasuring(measure m) = 0;
    void setD(float D);
    void setL(float L);
    void setB(float B);
    void setBRange(float BRange);
};

class SingleLidarSpeedMeasurer : SpeedMeasurer {
    enum {WAIT_4_WHEEL, WHEEL_APPEAR, WHEEL_DISAPPEAR} state;

    uint32_t appearanceTime;
    uint32_t disappearanceTime;
    void (*callback) (result result);
    vector<measure> measures;

    bool isWheelAppear(measure & m);
    bool isWheelDisappear(measure & m);
    void countSpeed();
public:
    SingleLidarSpeedMeasurer(StreamFilter* filter, void (*callback)(result result))
                            : SpeedMeasurer(filter), callback(callback), state(WAIT_4_WHEEL) {}
    void addMeasuring(measure m) override;

};

class BothLidarSpeedMeasurer : SpeedMeasurer {
    unsigned long firstLidarAppearanceTime;
    unsigned long secondLidarAppearanceTime;
public:
    BothLidarSpeedMeasurer(float D, float S, float L, StreamFilter * filter) : SpeedMeasurer(filter) {}
    void addMeasuring(measure m) override;
};


#endif //SPEEDMEASURER_SPEEDMEASURER_H
