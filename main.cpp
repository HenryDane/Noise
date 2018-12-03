#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <iostream>

#include "camera.h"

#define MATRIX_WIDTH 100
#define MATRIX_HEIGHT 100

void init_gl();

float width = 800;
float height = 600;

float randf(float minv, float maxv) {
    return ((float(rand()) / float(RAND_MAX)) * (maxv - minv)) + minv;
}

int main() {
    srand(1826); // init

    // create the window
    sf::Window window(sf::VideoMode(width, height), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);

    // load resources, initialize the OpenGL states, ...
    glEnable(GL_TEXTURE_2D);
    glPointSize(3);
    init_gl();

    EulerCamera camera(width / height, 23, 34, 87);
    camera.setNearClippingPane(2);
    camera.setFarClippingPane(60);
    camera.setFieldOfView(60);
//    camera.applyOptimalStates();
    camera.applyPerspectiveMatrix();
    camera.setPosition(0, 8, 100);
    camera.setRotation(2, 90, 0);

    // initalize stuff
    float * values = new float[MATRIX_WIDTH * MATRIX_HEIGHT];
    float * refs = new float[MATRIX_WIDTH * MATRIX_HEIGHT];
    bool * ud = new bool[MATRIX_WIDTH * MATRIX_HEIGHT];
    for (int i = 0; i < MATRIX_HEIGHT * MATRIX_WIDTH; i++) {
        float h = randf(-1, 1);
        values[i] = h;
        refs[i] = values[i] + 0.01;
        ud[i] = rand() % 2;
    }

    std::cout << "test";

    // run the main loop
    while (window.isOpen()) {
        // handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                break;
            }
            else if (event.type == sf::Event::Resized) {
                width = event.size.width;
                height = event.size.height;
                glViewport(0, 0, width, height); // adjust the viewport when the window is resized
            }
        }

        glLoadIdentity();
        camera.applyTranslations();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the buffers

        // draw lines
        glPushMatrix();
        glScalef(2.0, 1, 2.0);
        glTranslatef(0.0, 0.1, 0.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor3f(0.1, 0.1, 0.1);
        glBegin(GL_TRIANGLES); // looks better
        if ( 1){ // lines
            for(int i = 0; i < MATRIX_WIDTH; i++){
                for (int j = 0; j < MATRIX_HEIGHT - 1; j++){
                    glVertex3f(i, values[j * MATRIX_WIDTH + (i % 99)] + 0.001, j);
                    glVertex3f(i + 1, values[j * MATRIX_WIDTH + ((i + 1) % 99)] + 0.001, j);
                    glVertex3f(i, values[(j + 1) * MATRIX_WIDTH + (i % 99)] + 0.001, j + 1);

                    glVertex3f(i + 1, values[j * MATRIX_WIDTH + ((i + 1) % 99)] + 0.001, j);
                    glVertex3f(i + 1, values[(j + 1) * MATRIX_WIDTH + (i + 1 % 99)] + 0.001, j + 1);
                    glVertex3f(i, values[(j + 1) * MATRIX_WIDTH + (i % 99)] + 0.001, j + 1);
                }
            }
        }
        glEnd();
        glPopMatrix();

        glPushMatrix();
        glScalef(2.0, 1.0, 2.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3f(0.5, 0.5, 1.0);
        glBegin(GL_TRIANGLES); // looks better
        if ( 1){ // lines
            for(int i = 0; i < MATRIX_WIDTH; i++){
                for (int j = 0; j < MATRIX_HEIGHT - 1; j++){
                    glVertex3f(i, values[j * MATRIX_WIDTH + (i % 99)] + 0.001, j);
                    glVertex3f(i + 1, values[j * MATRIX_WIDTH + ((i + 1) % 99)] + 0.001, j);
                    glVertex3f(i, values[(j + 1) * MATRIX_WIDTH + (i % 99)] + 0.001, j + 1);

                    glVertex3f(i + 1, values[j * MATRIX_WIDTH + ((i + 1) % 99)] + 0.001, j);
                    glVertex3f(i + 1, values[(j + 1) * MATRIX_WIDTH + (i + 1 % 99)] + 0.001, j + 1);
                    glVertex3f(i, values[(j + 1) * MATRIX_WIDTH + (i % 99)] + 0.001, j + 1);
                }
            }
        }
        glEnd();
        glPopMatrix();

        // process mesh
        for(int i = 0; i < MATRIX_WIDTH; i++){
            for (int j = 0; j < MATRIX_HEIGHT; j++){
                if (ud[i + j * MATRIX_WIDTH]){
                    values[i + j * MATRIX_WIDTH] = values[i + j * MATRIX_WIDTH] + 0.01;
                } else {
                    values[i + j * MATRIX_WIDTH] = values[i + j * MATRIX_WIDTH] - 0.01;
                }
                if (abs(values[i + j * MATRIX_WIDTH]) > abs(refs[i + j * MATRIX_WIDTH])){
                    ud[i + j * MATRIX_WIDTH] = !ud[i + j * MATRIX_WIDTH];
                }
            }
        }

        window.display(); // end the current frame (internally swaps the front and back buffers)
    }

    // release resources...
    delete[] values;
    delete[] refs;

    return 0;
}

void init_gl() {
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    float a[4] = {50, 10, 100, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, a);
    float b[4] = {0.1, 0.1, 0.1, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, b);
    float c[4] = {0.5, 0.5, 0.5, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, c);
    float d[4] = {0.7, 0.7, 0.7, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, d);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.91, 0.91, 0.91, 0.001);
    glFogf(GL_FOG_START, 5.0);
    glFogf(GL_FOG_END, 60.0);
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_DENSITY, .05);

    glCullFace(GL_BACK); // halves amount of processing necessary

    float e[4] = {1.0, 1.0, 1.0, 1.0};
    glFogfv(GL_FOG_COLOR, e);
}
