#include "renderareawidget.h"
#include "bezier.h"

#include <QMouseEvent>

const char* vertexShaderSource = R"(
    #version 330 core

    layout( location = 0 ) in vec3 vertexPos;
    uniform mat4 transformMatrix;

    void main()
    {
        gl_Position = transformMatrix * vec4( vertexPos, 1 );
    }
)";


const char* fragmentShaderSource = R"(
    #version 330 core

    uniform vec3 color;
    out vec3 finalColor;

    void main()
    {
        finalColor = color;
    }
)";


RenderAreaWidget::RenderAreaWidget(QWidget* parent)
    : QOpenGLWidget(parent),
      program(nullptr)
{

}


RenderAreaWidget::~RenderAreaWidget()
{
    makeCurrent();
    pointsBuffer.destroy();
    curveBuffer.destroy();
    doneCurrent();
    delete program;
}


void RenderAreaWidget::initializeGL()
{
    initializeOpenGLFunctions();

    makeCurrent();

    glViewport(0,0,width(),height());

    //Layout de ponto e linha:
    glEnable(GL_POINT_SMOOTH);
    glEnable( GL_LINE_SMOOTH );
    glLineWidth(1.0f);
    glPointSize(8.0f);

    program = new QOpenGLShaderProgram();
    program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    program->link();

    if (!program->isLinked())
    {
        //TODO: Exibir erro de link e fechar o programa
    }

    program->bind();

    pointsBuffer.create();

    curveBuffer.create();

    proj.ortho(-10.f,10.f,-10.f,10.f,-1.f,1.0f);

    program->setUniformValue("transformMatrix", proj*view);
}


void RenderAreaWidget::paintGL()
{
    program->bind();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    if(!points.empty() && pointsBuffer.isCreated())
    {
        std::vector<QVector3D> preview;
        for(int i=0;i<points.size();i++){
            preview.push_back(points[i]);
        }
        if(points.size() < 4){
            preview.push_back(current_point);
        }
        pointsBuffer.bind();
        curveBuffer.bind();
        pointsBuffer.allocate( &preview[0], (int)preview.size()*sizeof(QVector3D) );
        program->enableAttributeArray(0);
        program->setAttributeBuffer(0,GL_FLOAT,0,3,sizeof(QVector3D));

        //Desenha o poligono
        program->setUniformValue("color", QVector3D(1,0,0)); //Vermelho
        glDrawArrays(GL_LINE_STRIP, 0, (int)preview.size());

        //Desenha os pontos
        program->setUniformValue("color", QVector3D(1,1,0)); //Amarelo
        glDrawArrays(GL_POINTS, 0, (int)preview.size());


        //if(points.size() == 4){
        program->setUniformValue("color", QVector3D(1,1,1)); //Branco
        Bezier b(preview);
        b.calculateAllPoints();
        //curveBuffer.bind();
        curveBuffer.allocate(&(b.curve_points[0]),(int)b.curve_points.size()*sizeof(QVector3D) );

        glDrawArrays(GL_LINE_STRIP, 0, (int)b.curve_points.size());
        //}
    }
}


void RenderAreaWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}


void RenderAreaWidget::mousePressEvent(QMouseEvent *event)
{

}


void RenderAreaWidget::mouseMoveEvent(QMouseEvent *event)
{
    QVector3D point( event->x(), height()-event->y(), 0 );
    point = point.unproject( view, proj, QRect(0,0,width(),height()));
    point.setZ(0.f);

    current_point.setX(point.x());
    current_point.setY(point.y());

    if(selectedPoint != -1){
        points[selectedPoint].setX(point.x());
        points[selectedPoint].setY(point.y());
    }

    update();

}


void RenderAreaWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QVector3D point( event->x(), height()-event->y(), 0 );
    point = point.unproject( view, proj, QRect(0,0,width(),height()));
    point.setZ(0.f);
    if(points.size()==4){
       if(selectedPoint != -1){
            points[selectedPoint].setX(point.x());
            points[selectedPoint].setY(point.y());
            selectedPoint = -1;
        } else {
            for(int i = 0; i<4; i++){
                if(points[i].distanceToPoint(point) < 1){
                    selectedPoint = i;
                }
            }
        }
    } else {
        points.push_back( point );
    }

    update();
}

