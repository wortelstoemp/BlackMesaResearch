#pragma once

// Author(s): Simon

const GLdouble pi = 3.1415926535897932384626433832795;

// Replaces gluPerspective. Sets the frustum to perspective mode.
// fovY     - Field of vision in degrees in the y direction
// aspect   - Aspect ratio of the viewport
// zNear    - The near clipping distance
// zFar     - The far clipping distance
void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
	GLdouble fW, fH;
	fH = tan( fovY / 360 * pi ) * zNear;
	fW = fH * aspect;
    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}