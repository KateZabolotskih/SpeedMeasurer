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
                state = WHEEL_APPEAR;
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
    filter->addData(m);
    if (filter->getData(m)) {
        switch (state) {
            case WAIT_4_WEELSET:
                if ((isWheelSetAppear_1(m))) {
                    cout << "wheelset catch first lidar =" << m.timestamp << endl;
                    firstLidarAppearanceTime = (m.timestamp + measures.back().timestamp) / 2;
                    state = FIRST_LIDAR_CATCH;
                    weelset = CUR;
                }
                break;
            case FIRST_LIDAR_CATCH:
                if (isWheelSetAppear_2(m)) {
                    cout << "wheelset catch second lidar =" << m.timestamp << endl;
                    secondLidarAppearanceTime = (m.timestamp + measures.back().timestamp) / 2;
                    state = SECOND_LIDAR_CATCH;
                }
                break;
            case SECOND_LIDAR_CATCH:
                if (weelset == CUR) {
                    countSpeed();
                }
                weelset = NEXT;
                if (isWheelSetDisappear_2(m)) {
                    state = WAIT_4_WEELSET;
                }
        }
    }
}

bool BothLidarSpeedMeasurer::isWheelSetAppear_1(measure & m) {
    if (m.data > B - BRange && m.data < B + BRange) {
        firstLidarNumber = m.lidarNumber;
        return true;
    }
    return false;
}

bool BothLidarSpeedMeasurer::isWheelSetAppear_2(measure & m) {
    if ((m.data > B - BRange && m.data < B + BRange) && m.lidarNumber != firstLidarNumber) {
        secondLidarNumber = m.lidarNumber;
        return true;
    }
    return false;
}

bool BothLidarSpeedMeasurer::isWheelSetDisappear_2(measure & m) {
    if (!(m.data > B - BRange && m.data < B + BRange) && m.lidarNumber != firstLidarNumber) {
        secondLidarNumber = m.lidarNumber;
        return true;
    }
    return false;
}

void BothLidarSpeedMeasurer::countSpeed() {
    std::cout << "first lidar catch time = " << firstLidarAppearanceTime << std::endl;
    std::cout << "second lidar catch time = " << secondLidarAppearanceTime << std::endl;
    std::cout << "interval = " << L << std::endl;
    float speed = (float)(secondLidarAppearanceTime - firstLidarAppearanceTime) / CLOCKS_PER_SEC / L * 1000;
    float accurate = 0;
    result result = {
            SUCCESS,
            speed,
            accurate
    };
    callback(result);
}