#ifndef DISTANCE_H
#define DISTANCE_H
#include "car.h"
class Line {
public:
    double x0;
    double y0;
    double z0;
    double x1;
    double y1;
    double z1;
    Line(){}
    Line(Point a, Point b){
        x0=a[0];
        y0=a[1];
        z0=a[2];
        x1=b[0];
        y1=b[1];
        z1=b[2];
    }
};
//la puta norma!
inline double norm(double x,double y, double z){
	return sqrt(pow(x,2)+ pow(y,2)+pow(z,2));
}

inline double distance(Point p1,Point p2){
	return norm(p1[0]-p2[0],p1[1]-p2[1],0);
}
double colide(Line line1, Line line2);
#endif  //DISTANCE_H
