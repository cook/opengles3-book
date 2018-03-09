/**
 * @file   coordination.hpp
 * @author Yantao Xie <leeward.xie@gmail.com>
 * @date   Mon Aug  7 17:14:26 2017
 *
 * @brief  GLCoordination是一个具体类, 渲染3D坐标, 红色表示X, 绿色表示Y, 蓝色表示Z.
 */
#ifndef SAK_OPENGL_ES_COORDINATION_HPP__
#define SAK_OPENGL_ES_COORDINATION_HPP__

#include <cstdlib>
#include <cmath>
#include "esProgram.hpp"

namespace sak { namespace gles {

class GLCoordination : public GLProgram
{
    const int POSITION_LOC = 0;
    const int COLOR_LOC = 1;
    const int MVP_LOC = 2;

    int m_numIndices;           //!< equal to 3 * number of triangles
    // VBOs
    GLuint m_positionVBO;
    GLuint m_colorVBO;
    GLuint m_mvpVBO;
    GLuint m_indicesIBO;

  public:
    GLCoordination()
    {}
    virtual ~GLCoordination()
    {
        if (is_initialized()) {
            glDeleteBuffers(1, &m_positionVBO);
            glDeleteBuffers(1, &m_colorVBO);
            glDeleteBuffers(1, &m_mvpVBO);
            glDeleteBuffers(1, &m_indicesIBO);
        }
    }

    int init(int width, int height)
    {
        m_width = width;
        m_height = height;

        GLfloat *positions;
        GLuint *indices;
        GLfloat *colors;

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

        load_program(vShaderStr, fShaderStr);

        // Generate the vertex data
        m_numIndices = esGenCoordination(0.5f, &positions, &colors, &indices);
        const int numVertices = 30;

        // Index buffer object
        glGenBuffers(1, &m_indicesIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_numIndices, indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        free(indices);

        // Position VBO for triangle model
        glGenBuffers(1, &m_positionVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_positionVBO);
        glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(GLfloat) * 3, positions, GL_STATIC_DRAW);
        free(positions);

        // Color VBO
        glGenBuffers(1, &m_colorVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
        glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(GLfloat) * 4, colors, GL_STATIC_DRAW);
        free(colors);

        this->position(0.f, 0.f);
        this->angles(0.f, 0.f, 0.f);
        glGenBuffers(1, &m_mvpVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_mvpVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ESMatrix), NULL, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        is_initialized(true);
        return GL_TRUE;
    }

    void update(int width, int height, float deltaTime)
    {
        if (!is_initialized())
            return;

        m_width = width;
        m_height = height;

        use_program();

        // Generate a perspective matrix with a 60 degree FOV
        float aspect = (GLfloat) width /(GLfloat) height;;
        ESMatrix perspective;
        esMatrixLoadIdentity(&perspective);
        esPerspective(&perspective, 100.0f, aspect, 1.0f, 20.0f);
        // map the GPU buffer to memory
        glBindBuffer(GL_ARRAY_BUFFER, m_mvpVBO);
        ESMatrix *matrixBuf = (ESMatrix*) glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(ESMatrix), GL_MAP_WRITE_BIT);

        ESMatrix modelview;
        // Generate a model view matrix to rotate/translate the cube
        esMatrixLoadIdentity(&modelview);
        // Per-instance translation
        esTranslate(&modelview, m_x, m_y, -2.0f);
        /// Rotate
        // pitch
        esRotate(&modelview, m_pitch, 1.0, 0.0, 0.0);
        // yaw
        esRotate(&modelview, m_yaw, 0.0, 1.0, 0.0);
        // roll
        esRotate(&modelview, m_roll, 0.0, 0.0, 1.0);
        // Compute the final MVP by multiplying the
        // modevleiw and perspective matrices together
        esMatrixMultiply(matrixBuf, &modelview, &perspective);

        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    void draw()
    {
        if (!is_initialized())
            return;

        use_program();

        // Load the vertex position
        glBindBuffer(GL_ARRAY_BUFFER, m_positionVBO);
        glVertexAttribPointer(POSITION_LOC, 3, GL_FLOAT,
                              GL_FALSE, 3 * sizeof(GLfloat),(const void *) NULL);
        glEnableVertexAttribArray(POSITION_LOC);

        // Load the instance color buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
        glVertexAttribPointer(COLOR_LOC, 4, GL_FLOAT,
                              GL_FALSE, 4 * sizeof(GLfloat),(const void *) NULL);
        glEnableVertexAttribArray(COLOR_LOC);
        // 如果打开这句，整个坐标只有一个颜色。
        //glVertexAttribDivisor(COLOR_LOC, 1); // One color per instance

        // Load the instance MVP buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_mvpVBO);

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
        glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, m_indicesIBO);

        // Draw the triangle
        glLineWidth(2.0f);
        glDrawElements (GL_LINES, m_numIndices, GL_UNSIGNED_INT, NULL);
    }
};

}}

#endif
