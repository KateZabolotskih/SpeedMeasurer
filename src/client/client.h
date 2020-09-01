//
// Created by Екатерина Заболотских on 01.09.2020.
//

#ifndef SPEEDMEASURER_CLIENT_H
#define SPEEDMEASURER_CLIENT_H

struct Measure {
    uint32_t timestamp;
    float lidar1_data;
    float lidar2_data;
};

#endif //SPEEDMEASURER_CLIENT_H
