#include <stdlib.h>
#include <math.h>
#include "esUtil.h"

#ifdef _WIN32
#define srandom srand
#define random rand
#endif

#define POSITION_LOC    0
#define COLOR_LOC       1
#define MVP_LOC         2

typedef struct
{
    // Handle to a program object
    GLuint programObject;

    // VBOs
    GLuint positionVBO;
    GLuint colorVBO;
    GLuint mvpVBO;
    GLuint indicesIBO;

    // Number of indices
    int       numIndices;

    // Rotation angle
    GLfloat   angle;

} UserData;

///
// Initialize the shader and program object
//
int Init(ESContext *esContext)
{
    GLfloat *positions;
    GLuint *indices;
    GLfloat *colors;

    UserData *userData = esContext->userData;
    const char vShaderStr[] =
        "#version 300 es                             \n"
        "layout(location = 0) in vec4 a_position;    \n"
        "layout(location = 1) in vec4 a_color;       \n"
        "layout(location = 2) in mat4 a_mvpMatrix;   \n"
        "out vec4 v_color;                           \n"
        "void main()                                 \n"
        "{                                           \n"
        "   v_color = a_color;                       \n"
        "   gl_Position = a_mvpMatrix * a_position;  \n"
        "}                                           \n";

    const char fShaderStr[] =
        "#version 300 es                                \n"
        "precision mediump float;                       \n"
        "in vec4 v_color;                               \n"
        "layout(location = 0) out vec4 outColor;        \n"
        "void main()                                    \n"
        "{                                              \n"
        "  outColor = v_color;                          \n"
        "}                                              \n";

    // Load the shaders and get a linked program object
    userData->programObject = esLoadProgram(vShaderStr, fShaderStr);

    // Generate the vertex data
    userData->numIndices = esGenCoordination(0.5f, &positions, &colors, &indices);
    const int numVertices = 30;

    // Index buffer object
    glGenBuffers(1, &userData->indicesIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userData->indicesIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * userData->numIndices, indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    free(indices);

    // Position VBO for triangle model
    glGenBuffers(1, &userData->positionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, userData->positionVBO);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(GLfloat) * 3, positions, GL_STATIC_DRAW);
    free(positions);

    // Color VBO
    glGenBuffers(1, &userData->colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, userData->colorVBO);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(GLfloat) * 4, colors, GL_STATIC_DRAW);
    free(colors);

    userData->angle = 0.f;
    glGenBuffers(1, &userData->mvpVBO);
    glBindBuffer(GL_ARRAY_BUFFER, userData->mvpVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ESMatrix), NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    return GL_TRUE;
}

///
// Update MVP matrix based on time
//
void Update(ESContext *esContext, float deltaTime)
{
    UserData *userData =(UserData *) esContext->userData;
    ESMatrix *matrixBuf;
    ESMatrix perspective;
    float    aspect;

    // Compute the window aspect ratio
    aspect =(GLfloat) esContext->width /(GLfloat) esContext->height;

    // Generate a perspective matrix with a 60 degree FOV
    esMatrixLoadIdentity(&perspective);
    esPerspective (&perspective, 100.0f, aspect, 1.0f, 20.0f);

    glBindBuffer(GL_ARRAY_BUFFER, userData->mvpVBO);
    matrixBuf = (ESMatrix*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(ESMatrix), GL_MAP_WRITE_BIT);

    {
        ESMatrix modelview;
        float translateX = 0.0f;
        float translateY = 0.0f;

        // Generate a model view matrix to rotate/translate the cube
        esMatrixLoadIdentity(&modelview);

        // Per-instance translation
        esTranslate(&modelview, translateX, translateY, -2.0f);

        // Compute a rotation angle based on time to rotate the cube
        userData->angle +=(deltaTime * 40.0f);

        if (userData->angle >= 360.0f)
            userData->angle -= 360.0f;

        // Rotate the cube
        esRotate(&modelview, userData->angle, 1.0, 0.0, 1.0);

        // Compute the final MVP by multiplying the
        // modevleiw and perspective matrices together
        esMatrixMultiply(matrixBuf, &modelview, &perspective);
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw(ESContext *esContext)
{
    UserData *userData = esContext->userData;

    // Set the viewport
    glViewport(0, 0, esContext->width, esContext->height);

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use the program object
    glUseProgram(userData->programObject);

    // Load the vertex position
    glBindBuffer(GL_ARRAY_BUFFER, userData->positionVBO);
    glVertexAttribPointer(POSITION_LOC, 3, GL_FLOAT,
                            GL_FALSE, 3 * sizeof(GLfloat),(const void *) NULL);
    glEnableVertexAttribArray(POSITION_LOC);

    // Load the instance color buffer
    glBindBuffer(GL_ARRAY_BUFFER, userData->colorVBO);
    glVertexAttribPointer(COLOR_LOC, 4, GL_FLOAT,
                          GL_FALSE, 4 * sizeof(GLfloat),(const void *) NULL);
    glEnableVertexAttribArray(COLOR_LOC);
    // 如果打开这句，整个坐标只有一个颜色。
    //glVertexAttribDivisor(COLOR_LOC, 1); // One color per instance


    // Load the instance MVP buffer
    glBindBuffer(GL_ARRAY_BUFFER, userData->mvpVBO);

    // Load each matrix row of the MVP.  Each row gets an increasing attribute location.
    glVertexAttribPointer(MVP_LOC + 0, 4, GL_FLOAT, GL_FALSE, sizeof(ESMatrix),(const void *) NULL);
    glVertexAttribPointer(MVP_LOC + 1, 4, GL_FLOAT, GL_FALSE, sizeof(ESMatrix),(const void *)(sizeof(GLfloat) * 4));
    glVertexAttribPointer(MVP_LOC + 2, 4, GL_FLOAT, GL_FALSE, sizeof(ESMatrix),(const void *)(sizeof(GLfloat) * 8));
    glVertexAttribPointer(MVP_LOC + 3, 4, GL_FLOAT, GL_FALSE, sizeof(ESMatrix),(const void *)(sizeof(GLfloat) * 12));
    glEnableVertexAttribArray(MVP_LOC + 0);
    glEnableVertexAttribArray(MVP_LOC + 1);
    glEnableVertexAttribArray(MVP_LOC + 2);
    glEnableVertexAttribArray(MVP_LOC + 3);

    // One MVP per instance
    glVertexAttribDivisor(MVP_LOC + 0, 1);
    glVertexAttribDivisor(MVP_LOC + 1, 1);
    glVertexAttribDivisor(MVP_LOC + 2, 1);
    glVertexAttribDivisor(MVP_LOC + 3, 1);

    // Bind the index buffer
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, userData->indicesIBO);

    // Draw the triangle
    glLineWidth(2.0f);
    glDrawElements (GL_LINES, userData->numIndices, GL_UNSIGNED_INT, NULL);
}

///
// Cleanup
//
void Shutdown(ESContext *esContext)
{
    UserData *userData = esContext->userData;

    glDeleteBuffers(1, &userData->positionVBO);
    glDeleteBuffers(1, &userData->colorVBO);
    glDeleteBuffers(1, &userData->mvpVBO);
    glDeleteBuffers(1, &userData->indicesIBO);

    // Delete program object
    glDeleteProgram(userData->programObject);
}


int esMain(ESContext *esContext)
{
    esContext->userData = malloc(sizeof(UserData));

    esCreateWindow(esContext, "Instancing", 640, 480, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

    if (!Init(esContext))
        return GL_FALSE;

    esRegisterShutdownFunc(esContext, Shutdown);
    esRegisterUpdateFunc(esContext, Update);
    esRegisterDrawFunc(esContext, Draw);

    return GL_TRUE;
}

