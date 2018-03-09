#include "coordination.hpp"

using sak::gles::GLProgram;

int Init(ESContext *esContext)
{
    GLProgram *userData = (GLProgram*)esContext->userData;
    userData->init(esContext->width, esContext->height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    return GL_TRUE;
}

void Update(ESContext *esContext, float deltaTime)
{
    GLProgram *userData = (GLProgram*)esContext->userData;
    userData->update(esContext->width, esContext->height, deltaTime);
}

void Draw(ESContext *esContext)
{
    GLProgram *userData = (GLProgram*)esContext->userData;

    glViewport(0, 0, esContext->width, esContext->height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    userData->draw();
}

void Shutdown(ESContext *esContext)
{
    GLProgram *userData = (GLProgram*)esContext->userData;
    delete userData;
    esContext->userData = NULL;
}

extern "C"
int esMain(ESContext *esContext)
{
    esContext->userData = (void*) new sak::gles::GLCoordination();

    esCreateWindow(esContext, "GLCoordination", 640, 480, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

    if (!Init(esContext))
        return GL_FALSE;

    esRegisterShutdownFunc(esContext, Shutdown);
    esRegisterUpdateFunc(esContext, Update);
    esRegisterDrawFunc(esContext, Draw);

    return GL_TRUE;
}
