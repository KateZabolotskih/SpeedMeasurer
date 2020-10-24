//
// Created by Екатерина Заболотских on 07.09.2020.
//

#ifndef SPEEDMEASURER_FILTER_H
#define SPEEDMEASURER_FILTER_H

#include <vector>
#include "models/models.h"

using namespace std;

class StreamFilter {
public:
    virtual void addData(measure data) = 0;
    virtual bool getData(measure & data) = 0;
};

class StreamFloatMedianFilter : StreamFilter{
    unsigned int r;
    vector<measure> buffer;
public:
    explicit StreamFloatMedianFilter(unsigned int r) : r(r) {}
    void addData(measure data) override;
    bool getData(measure & data) override;
};


#endif //SPEEDMEASURER_FILTER_H
