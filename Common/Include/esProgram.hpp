/**
 * @file   esProgram.hpp
 * @author Yantao Xie <leeward.xie@gmail.com>
 * @date   Mon Aug  7 17:16:20 2017
 *
 * @brief  GLProgram是一个抽象类, 是用来和esUtil.h中的GL窗口系统结合起来使用的.
 */
#ifndef SAK_OPENGL_ES_PROGRAM_HPP__
#define SAK_OPENGL_ES_PROGRAM_HPP__

#include "esUtil.h"

namespace sak { namespace gles {

class GLProgram
{
    GLuint m_program;
    // Status
    bool m_isInitial;
  protected:
    // frame buffer size
    int m_width;
    int m_height;
    // xy position
    float   m_x;
    float   m_y;
    // euler angle
    GLfloat m_yaw;              //!< x
    GLfloat m_pitch;            //!< y
    GLfloat m_roll;             //!< z

  public:
    GLProgram()
        :m_isInitial(false),
         m_width(0), m_height(0), m_x(0.), m_y(0.),
         m_yaw(0.), m_pitch(0.), m_roll(0.)
    {}
    virtual ~GLProgram()
    {
        // Delete program object
        glDeleteProgram(m_program);
    }

    bool is_initialized() const { return m_isInitial; }
    void load_program(const char* vShaderStr, const char* fShaderStr)
    {
        m_program = esLoadProgram(vShaderStr, fShaderStr);
    }
    void use_program() const
    {
        glUseProgram(m_program);
    }
    void position(float x, float y)
    {
        m_x = x, m_y = y;
    }
    void angles(GLfloat yaw, GLfloat pitch, GLfloat roll)
    {
        m_yaw = yaw;
        m_pitch = pitch;
        m_roll = roll;
    }

    virtual int init(int width, int height) = 0;
    virtual void update(int width, int height, float deltaTime) = 0;
    virtual void draw() = 0;

  protected:
    void is_initialized(bool v) { m_isInitial = v; }
};

}}

#endif
