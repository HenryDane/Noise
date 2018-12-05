#include <SFML/OpenGL.hpp>
#include <cmath>

#define M_PI 3.14159265359

void gluPerspective(double fovy,double aspect, double zNear, double zFar)
{
    // Start in projection mode.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double xmin, xmax, ymin, ymax;
    ymax = zNear * tan(fovy * M_PI / 360.0);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;
    glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}

class EulerCamera {
private:
    float pitch = 0;
    float yaw = 0;
    float roll = 0;
    float fov = 90;
    float aspectRatio = 1;
    float zNear;
    float zFar;
public:
    float x;
    float y;
    float z;

    EulerCamera(float aspectRatio, float x, float y, float z) {
        if (aspectRatio <= 0) aspectRatio = 1;
        this->aspectRatio = aspectRatio;
        this->zNear = 0.3f;
        this->zFar = 100;
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void setFieldOfView(float fov) {
        this->fov = fov;
    }
    void setAspectRatio(float aspectRatio) {
        if (aspectRatio <= 0) aspectRatio = 1;
        this->aspectRatio = aspectRatio;
    }
    void setFarClippingPane(float farClippingPane) {
        if (farClippingPane <= 0) farClippingPane = 1;
        this->zFar = farClippingPane;
    }
    void setNearClippingPane(float nearClippingPane) {
        if (nearClippingPane <= 0) nearClippingPane = 100;
        this->zNear = nearClippingPane;
    }
    void setRotation(float pitch, float yaw, float roll) {
        this->pitch = pitch;
        this->yaw = yaw;
        this->roll = roll;
    }
    void setPosition(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void applyPerspectiveMatrix() {
        glPushAttrib(GL_TRANSFORM_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        //gluPerspective(fov, aspectRatio, zNear, zFar);
        gluPerspective(fov, aspectRatio, zNear, zFar);
        glPopAttrib();
    }
    void applyTranslations() {
        glPushAttrib(GL_TRANSFORM_BIT);
        glMatrixMode(GL_MODELVIEW);
        glRotatef(pitch, 1.0f, 0.0f, 0.0f);
        glRotatef(yaw, 0.0f, 1.0f, 0.0f);
        glRotatef(roll, 0.0f, 0.0f, 1.0f);
        glTranslatef(-x, -y, -z);
        glPopAttrib();
    }
    void applyOrthographicMatrix() {
        glPushAttrib(GL_TRANSFORM_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-aspectRatio, aspectRatio, -1, 1, 0, zFar);
        glPopAttrib();
    }
};
