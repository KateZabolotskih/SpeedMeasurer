//
// Created by Екатерина Заболотских on 07.09.2020.
//

#include <iostream>
#include "models.h"
#include "filter.h"
#include "speedMeasurer.h"


bool StreamFloatMedianFilter::getData(measure & data) {
    if (buffer.size() < (2 * r + 1))
        return false;
    vector<measure> copy = buffer;
    sort(copy.begin(), copy.end());
    data = copy[copy.size() / 2];
    return true;
}

void StreamFloatMedianFilter::addData(measure data) {
    buffer.push_back(data);
    if (buffer.size() > 2 * r + 1) {
        buffer.erase(buffer.begin());
    }
    for(auto data : buffer) {
        if (data.data < 50) {
            std::cout << "wheel(" << data.timestamp << ", " << data.data << ") ";
        } else {
            std::cout << "nothing(" << data.timestamp << ", " << data.data << ") ";
        }
    } std::cout << std::endl;
}