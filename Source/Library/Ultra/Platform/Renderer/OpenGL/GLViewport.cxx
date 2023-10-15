module Ultra.Platform.Renderer.GLViewport;

import <glad/gl.h>;

namespace Ultra {

GLViewport::GLViewport(const ViewportProperties &properties): Viewport(properties) {
    glViewport(static_cast<GLint>(properties.X), static_cast<GLint>(properties.Y), static_cast<GLsizei>(properties.Width), static_cast<GLsizei>(properties.Height));
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
