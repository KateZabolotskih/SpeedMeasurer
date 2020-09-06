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

enum {
    coming,
    weel_1,
    weel_2,
    gap,
    gone,
    stop
}train;




#endif //SPEEDMEASURER_CLIENT_H
