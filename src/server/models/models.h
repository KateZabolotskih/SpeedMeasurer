//
// Created by Екатерина Заболотских on 08.09.2020.
//

#ifndef SPEEDMEASURER_MODELS_H
#define SPEEDMEASURER_MODELS_H

#include <cstdint>

typedef enum {SUCCESS, FAIL} status;

struct result {
    status status;
    float speed;
    float accurate;
};

struct measure {
    uint32_t timestamp;
    int lidarNumber;
    float data;
    bool operator<(const measure & m) const;
};

#endif //SPEEDMEASURER_MODELS_H
