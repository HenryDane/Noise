#ifndef CAMERA_H
#define CAMERA_H 1

#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <GL/glu.h>

#define M_PI 3.14159265359

/*void gluPerspective(double fovy,double aspect, double zNear, double zFar)
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
}*/

class EulerCamera {
public:
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
        if (farClippingPane <= 0) farClippingPane = 100;
        this->zFar = farClippingPane;
    }
    void setNearClippingPane(float nearClippingPane) {
        if (nearClippingPane <= 0) nearClippingPane = 0.001;
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
    void applyNTranslations() {
        glPushAttrib(GL_TRANSFORM_BIT);
        glMatrixMode(GL_MODELVIEW);
        glRotatef(pitch, 1.0f, 0.0f, 0.0f);
        glRotatef(yaw, 0.0f, 1.0f, 0.0f);
        glRotatef(roll, 0.0f, 0.0f, 1.0f);
        //glTranslatef(-x, -y, -z);
        glPopAttrib();
    }
    void applyOrthographicMatrix() {
        glPushAttrib(GL_TRANSFORM_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-aspectRatio, aspectRatio, -1, 1, 0, zFar);
        glPopAttrib();
    }

    float toRadians(float degrees) {
        return (degrees / 180 * M_PI);
    }

    void moveFromLook(float dx, float dy, float dz) {
        this->z += dx * (float) cos(toRadians(yaw - 90)) + dz * cos(toRadians(yaw));
        this->x -= dx * (float) sin(toRadians(yaw - 90)) + dz * sin(toRadians(yaw));
        this->y += dy * (float) sin(toRadians(pitch - 90)) + dz * sin(toRadians(pitch));
    }

        /** Processes mouse input and converts it in to camera movement. */
/*
    void processMouse(vec2f_t &mousedxdy) {
        float MAX_LOOK_UP = 90;
        float MAX_LOOK_DOWN = -90;
        float mouseDX = mousedxdy.x * 0.16f;
        float mouseDY = mousedxdy.y * 0.16f;
        if (yaw + mouseDX >= 360) {
            yaw = yaw + mouseDX - 360;
        } else if (yaw + mouseDX < 0) {
            yaw = 360 - yaw + mouseDX;
        } else {
            yaw += mouseDX;
        }
        if (pitch - mouseDY >= MAX_LOOK_DOWN && pitch - mouseDY <= MAX_LOOK_UP) {
            pitch += -mouseDY;
        } else if (pitch - mouseDY < MAX_LOOK_DOWN) {
            pitch = MAX_LOOK_DOWN;
        } else if (pitch - mouseDY > MAX_LOOK_UP) {
            pitch = MAX_LOOK_UP;
        }
    }
*/

    /**
     * Processes mouse input and converts it in to camera movement.
     *
     * @param mouseSpeed the speed (sensitivity) of the mouse, 1.0 should suffice
     */
/*
    void processMouse(float mouseSpeed, vec2f_t &mousedxdy) {
        float MAX_LOOK_UP = 90;
        float MAX_LOOK_DOWN = -90;
        float mouseDX = mousedxdy.x * mouseSpeed * 0.16f;
        float mouseDY = mousedxdy.y * mouseSpeed * 0.16f;
        if (yaw + mouseDX >= 360) {
            yaw = yaw + mouseDX - 360;
        } else if (yaw + mouseDX < 0) {
            yaw = 360 - yaw + mouseDX;
        } else {
            yaw += mouseDX;
        }
        if (pitch - mouseDY >= MAX_LOOK_DOWN && pitch - mouseDY <= MAX_LOOK_UP) {
            pitch += -mouseDY;
        } else if (pitch - mouseDY < MAX_LOOK_DOWN) {
            pitch = MAX_LOOK_DOWN;
        } else if (pitch - mouseDY > MAX_LOOK_UP) {
            pitch = MAX_LOOK_UP;
        }
    }
*/

    /**
     * Processes mouse input and converts it into camera movement.
     *
     * @param mouseSpeed the speed (sensitivity) of the mouse, 1.0 should suffice
     * @param maxLookUp the maximum angle in degrees at which you can look up
     * @param maxLookDown the maximum angle in degrees at which you can look down
     */
/*
    void processMouse(float mouseSpeed, float maxLookUp, float maxLookDown, vec2f_t &mousedxdy) {
        float mouseDX = mousedxdy.x * mouseSpeed * 0.16f;
        float mouseDY = mousedxdy.y * mouseSpeed * 0.16f;
        if (yaw + mouseDX >= 360) {
            yaw = yaw + mouseDX - 360;
        } else if (yaw + mouseDX < 0) {
            yaw = 360 - yaw + mouseDX;
        } else {
            yaw += mouseDX;
        }
        if (pitch - mouseDY >= maxLookDown && pitch - mouseDY <= maxLookUp) {
            pitch += -mouseDY;
        } else if (pitch - mouseDY < maxLookDown) {
            pitch = maxLookDown;
        } else if (pitch - mouseDY > maxLookUp) {
            pitch = maxLookUp;
        }
    }
*/

    /**
     * Processes keyboard input and converts into camera movement.
     *
     * @param delta the elapsed time since the last frame update in milliseconds
     *
     * @throws IllegalArgumentException if delta is 0 or delta is smaller than 0
     */

    void processKeyboard(float delta, sf::Event &key_event) {
        if (delta <= 0) return;
        if (key_event.type != sf::Event::KeyPressed) return;

        bool keyUp = (key_event.key.code == sf::Keyboard::Up) || (key_event.key.code == sf::Keyboard::W);
        bool keyDown = (key_event.key.code == sf::Keyboard::Down) || (key_event.key.code == sf::Keyboard::S);
        bool keyLeft = (key_event.key.code == sf::Keyboard::Left) || (key_event.key.code == sf::Keyboard::A);
        bool keyRight = (key_event.key.code == sf::Keyboard::Right) || (key_event.key.code == sf::Keyboard::D);
        bool flyUp = (key_event.key.code == sf::Keyboard::Space);
        bool flyDown = (key_event.key.code == sf::Keyboard::LShift);

        if (keyUp && keyRight && !keyLeft && !keyDown) {
            moveFromLook(delta * 0.003f, 0, -delta * 0.003f);
        }
        if (keyUp && keyLeft && !keyRight && !keyDown) {
            moveFromLook(-delta * 0.003f, 0, -delta * 0.003f);
        }
        if (keyUp && !keyLeft && !keyRight && !keyDown) {
            moveFromLook(0, 0, -delta * 0.003f);
        }
        if (keyDown && keyLeft && !keyRight && !keyUp) {
            moveFromLook(-delta * 0.003f, 0, delta * 0.003f);
        }
        if (keyDown && keyRight && !keyLeft && !keyUp) {
            moveFromLook(delta * 0.003f, 0, delta * 0.003f);
        }
        if (keyDown && !keyUp && !keyLeft && !keyRight) {
            moveFromLook(0, 0, delta * 0.003f);
        }
        if (keyLeft && !keyRight && !keyUp && !keyDown) {
            moveFromLook(-delta * 0.003f, 0, 0);
        }
        if (keyRight && !keyLeft && !keyUp && !keyDown) {
            moveFromLook(delta * 0.003f, 0, 0);
        }
        if (flyUp && !flyDown) {
            y += delta * 0.003f;
        }
        if (flyDown && !flyUp) {
            y -= delta * 0.003f;
        }
    }

    /**
     * Processes keyboard input and converts into camera movement.
     *
     * @param delta the elapsed time since the last frame update in milliseconds
     * @param speed the speed of the movement (normal = 1.0)
     *
     * @throws IllegalArgumentException if delta is 0 or delta is smaller than 0
     */
    void processKeyboard(float delta, float speed) {
        if (delta <= 0) return;

        bool keyUp = (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::W));
        bool keyDown = (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::S));
        bool keyLeft = (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A));
        bool keyRight = (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::D));
        bool flyUp = (sf::Keyboard::isKeyPressed(sf::Keyboard::Space));
        bool flyDown = (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift));

        if (keyUp && keyRight && !keyLeft && !keyDown) {
            moveFromLook(speed * delta * 0.003f, 0, -speed * delta * 0.003f);
        }
        if (keyUp && keyLeft && !keyRight && !keyDown) {
            moveFromLook(-speed * delta * 0.003f, 0, -speed * delta * 0.003f);
        }
        if (keyUp && !keyLeft && !keyRight && !keyDown) {
            moveFromLook(0, 0, -speed * delta * 0.003f);
        }
        if (keyDown && keyLeft && !keyRight && !keyUp) {
            moveFromLook(-speed * delta * 0.003f, 0, speed * delta * 0.003f);
        }
        if (keyDown && keyRight && !keyLeft && !keyUp) {
            moveFromLook(speed * delta * 0.003f, 0, speed * delta * 0.003f);
        }
        if (keyDown && !keyUp && !keyLeft && !keyRight) {
            moveFromLook(0, 0, speed * delta * 0.003f);
        }
        if (keyLeft && !keyRight && !keyUp && !keyDown) {
            moveFromLook(-speed * delta * 0.003f, 0, 0);
        }
        if (keyRight && !keyLeft && !keyUp && !keyDown) {
            moveFromLook(speed * delta * 0.003f, 0, 0);
        }
        if (flyUp && !flyDown) {
            y += speed * delta * 0.003f;
        }
        if (flyDown && !flyUp) {
            y -= speed * delta * 0.003f;
        }
    }

    /**
     * Processes keyboard input and converts into camera movement.
     *
     * @param delta the elapsed time since the last frame update in milliseconds
     * @param speedX the speed of the movement on the x-axis (normal = 1.0)
     * @param speedY the speed of the movement on the y-axis (normal = 1.0)
     * @param speedZ the speed of the movement on the z-axis (normal = 1.0)
     *
     * @throws IllegalArgumentException if delta is 0 or delta is smaller than 0
     */
    void processKeyboard(float delta, float speedX, float speedY, float speedZ, sf::Event &key_event) {
        if (delta <= 0) return;
        if (key_event.type != sf::Event::KeyPressed) return;

        bool keyUp = (key_event.key.code == sf::Keyboard::Up) || (key_event.key.code == sf::Keyboard::W);
        bool keyDown = (key_event.key.code == sf::Keyboard::Down) || (key_event.key.code == sf::Keyboard::S);
        bool keyLeft = (key_event.key.code == sf::Keyboard::Left) || (key_event.key.code == sf::Keyboard::A);
        bool keyRight = (key_event.key.code == sf::Keyboard::Right) || (key_event.key.code == sf::Keyboard::D);
        bool flyUp = (key_event.key.code == sf::Keyboard::Space);
        bool flyDown = (key_event.key.code == sf::Keyboard::LShift);

        if (keyUp && keyRight && !keyLeft && !keyDown) {
            moveFromLook(speedX * delta * 0.003f, 0, -speedZ * delta * 0.003f);
        }
        if (keyUp && keyLeft && !keyRight && !keyDown) {
            moveFromLook(-speedX * delta * 0.003f, 0, -speedZ * delta * 0.003f);
        }
        if (keyUp && !keyLeft && !keyRight && !keyDown) {
            moveFromLook(0, 0, -speedZ * delta * 0.003f);
        }
        if (keyDown && keyLeft && !keyRight && !keyUp) {
            moveFromLook(-speedX * delta * 0.003f, 0, speedZ * delta * 0.003f);
        }
        if (keyDown && keyRight && !keyLeft && !keyUp) {
            moveFromLook(speedX * delta * 0.003f, 0, speedZ * delta * 0.003f);
        }
        if (keyDown && !keyUp && !keyLeft && !keyRight) {
            moveFromLook(0, 0, speedZ * delta * 0.003f);
        }
        if (keyLeft && !keyRight && !keyUp && !keyDown) {
            moveFromLook(-speedX * delta * 0.003f, 0, 0);
        }
        if (keyRight && !keyLeft && !keyUp && !keyDown) {
            moveFromLook(speedX * delta * 0.003f, 0, 0);
        }
        if (flyUp && !flyDown) {
            y += speedY * delta * 0.003f;
        }
        if (flyDown && !flyUp) {
            y -= speedY * delta * 0.003f;
        }
    }
};

#endif // CAMERA_H
