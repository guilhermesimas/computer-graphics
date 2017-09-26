#include "bezier.h"

Bezier::Bezier(std::vector<QVector3D> points)
{
    if(points.size() >= 4){
        for(int i=0;i<4;i++){
          control_points.push_back(points[i]);
        }
    } else if(points.size() == 3){
        control_points.push_back(points[0]);
        control_points.push_back(points[1]);
        control_points.push_back(points[1]);
        control_points.push_back(points[2]);
    } else if(points.size() == 2){
        control_points.push_back(points[0]);
        control_points.push_back(points[1]);
        control_points.push_back(points[0]);
        control_points.push_back(points[1]);
    }

}

void Bezier::calculatePoint(float t){
    if(t<0) t = 0.0;
    if(t>1) t = 1;
    float x = std::pow(1-t,3)*control_points[0].x() + 3*std::pow(1-t,2)*t*control_points[1].x() + 3*(1-t)*std::pow(t,2)*control_points[2].x() + std::pow(t,3)*control_points[3].x();
    float y = std::pow(1-t,3)*control_points[0].y() + 3*std::pow(1-t,2)*t*control_points[1].y() + 3*(1-t)*std::pow(t,2)*control_points[2].y() + std::pow(t,3)*control_points[3].y();
    QVector3D v(x,y,0);
    curve_points.push_back(v);
}

void Bezier::calculateAllPoints(void){
    float dt = (float)1/500;
    for(int i=0;i<=500;i++){
        calculatePoint(i*dt);
    }
}
