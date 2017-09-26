#ifndef RENDERAREAWIDGET_H
#define RENDERAREAWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include <vector>

class RenderAreaWidget
    : public QOpenGLWidget
    , protected QOpenGLFunctions
{
Q_OBJECT

public:
    explicit RenderAreaWidget(QWidget* parent = 0);
    ~RenderAreaWidget();

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    std::vector< QVector3D > points;

    QVector3D current_point;

    QOpenGLShaderProgram* program;
    QOpenGLBuffer pointsBuffer;
    QOpenGLBuffer curveBuffer;

    QMatrix4x4 view;
    QMatrix4x4 proj;

    int selectedPoint = -1;
};

#endif // RENDERAREAWIDGET_H
