module;

#include <glad/GL.h>

module Ultra.Platform.Engine.Renderer.GLViewport;

namespace Ultra {

GLViewport::GLViewport(const ViewportProperties &properties): Viewport(properties) {
    glViewport(properties.X, properties.Y, properties.Width, properties.Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // OpenGL's window coordinates and texture coordinates have opposite vertical  orientation. Automatically compensate via the projection matrix.
    if (properties.Window) {
        glTranslatef(-1.0,  1.0,  0.0);
        glScalef(2.0f / properties.Width, -2.0f / properties.Height, 1.0f);
    } else {
        glTranslatef(-1.0, -1.0, 0.0);
        glScalef(2.0f / properties.Width,  2.0f / properties.Height, 1.0f);
    }
}

}
