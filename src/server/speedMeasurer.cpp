//
// Created by Екатерина Заболотских on 06.09.2020.
//


#include <iostream>
#include "speedMeasurer.h"

void SingleLidarSpeedMeasurer::addMeasuring(measure m) {
    filter->addData(m);
    if (filter->getData(m)) {
        switch (state) {
            case WAIT_4_WHEEL:
                if (isWheelAppear(m)) {
                    cout << "wheel appear time=" << m.timestamp << endl;
                    appearanceTime = (m.timestamp + measures.back().timestamp) / 2;
                    state = WHEEL_APPEAR;
                }
                break;
            case WHEEL_APPEAR:
                if (isWheelDisappear(m)) {
                    cout << "wheel disappear time=" << m.timestamp << endl;
                    disappearanceTime = (m.timestamp + measures.back().timestamp) / 2;
                    state = WHEEL_DISAPPEAR;
                }
                break;
            case WHEEL_DISAPPEAR:
                countSpeed();
                break;
        }
    }
    measures.push_back(m);
}

bool SingleLidarSpeedMeasurer::isWheelAppear(measure & m) {
    if (m.data > B - BRange && m.data < B + BRange) {
        return true;
    }
    return false;
}

bool SingleLidarSpeedMeasurer::isWheelDisappear(measure & m) {
    if (!(m.data > B - BRange && m.data < B + BRange)) {
        return true;
    }
    return false;
}

void SingleLidarSpeedMeasurer::countSpeed() {
    std::cout << "time appearance = " << appearanceTime << std::endl;
    std::cout << "time disappearance = " << disappearanceTime << std::endl;
    std::cout << "diameter = " << D << std::endl;
    float speed = (float)(disappearanceTime - appearanceTime) / CLOCKS_PER_SEC / D * 1000;
    float accurate = 0;
    result result = {
            SUCCESS,
            speed,
            accurate
    };
    callback(result);
}

bool measure::operator<(const measure &m) const {
    return this->data < m.data;
}

void SpeedMeasurer::setD(float D) {
    this->D = D;
}

void SpeedMeasurer::setL(float L) {
    this->L = L;
}

void SpeedMeasurer::setB(float B) {
    this->B = B;
}

void SpeedMeasurer::setBRange(float BRange) {
    this->BRange = BRange;
}

void BothLidarSpeedMeasurer::addMeasuring(measure m) {

}
