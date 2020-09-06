//
// Created by Екатерина Заболотских on 06.09.2020.
//

#ifndef SPEEDMEASURER_SPEEDMEASURER_H
#define SPEEDMEASURER_SPEEDMEASURER_H



#ifndef RAILWAYS_SPEADMEASURER_H
#define RAILWAYS_SPEADMEASURER_H


struct measure {

};

class speedMeasurer {
    float D;
    float S;
    float B;
public:
    speedMeasurer(float D, float S, float B) : D(D), S(S), B(B) {};
    virtual void addMeasuring(measure m);
    virtual void resultCallback(void (*callback)(float));
};

class SimpleLidarSpeedMeasurer : speedMeasurer {
    unsigned long firstAppearanceTime;
    unsigned long lastAppearanceTime;
public:
    SimpleLidarSpeedMeasurer(float D, float S, float B, int lidarNumber);
    void addMeasuring(measure m) override;
    void resultCallback(void (*callback)(float)) override;
};

class BothLidarSpeedMeasurer : speedMeasurer {
    unsigned long firstLidarAppearanceTime;
    unsigned long secondLidarAppearanceTime;
public:
    BothLidarSpeedMeasurer(float D, float S, float B);
    void addMeasuring(measure m) override;
    void resultCallback(void (*callback)(float)) override;
};



#endif //RAILWAYS_SPEADMEASURER_H


#endif //SPEEDMEASURER_SPEEDMEASURER_H
