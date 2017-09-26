#ifndef BEZIER_H
#define BEZIER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include <vector>

class Bezier
{
public:
    Bezier(std::vector<QVector3D> points);
    std::vector< QVector3D > control_points;
    std::vector< QVector3D > curve_points;
    void calculateAllPoints(void);
private:
    void calculatePoint(float t);
};

#endif // BEZIER_H
