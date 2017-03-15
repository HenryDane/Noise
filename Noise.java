/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package noise;

import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.Random;
import java.util.Timer;
import java.util.TimerTask;
import noise.util.BufferTools;
import noise.util.EulerCamera;
import org.lwjgl.BufferUtils;
import org.lwjgl.LWJGLException;
import org.lwjgl.input.Keyboard;
import org.lwjgl.input.Mouse;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.DisplayMode;
import static org.lwjgl.opengl.GL11.*;

/**
 *
 * @author Henry Dane
 */
public class Noise {
    DisplayMode DISPLAYMODE;
    private static FloatBuffer lightPosition = BufferTools.asFlippedFloatBuffer(50, 10, 100, 1);
    private static EulerCamera camera;
    float[][] values = new float[100][100];
    float[][] ref = new float[100][100];
    boolean[][] ud = new boolean[100][100];
    int i = 23;
    ArrayList<Point> points = new ArrayList<Point>();
    ArrayList<Point> queue = new ArrayList<Point>();
    
    public Noise (){
        // make a display
        setupdisplay();
        setupstates();
        setupcamera();
        
        // create random
        Random rand = new Random(System.currentTimeMillis());
        
        // set pointsiz
        glPointSize(3f);
        
        // init values
        for (int i = 0; i < 100; i++){
            for (int j = 0; j < 100; j++){
                values[i][j] = rand.nextFloat();
                values[i][j] = values[i][j] / 2;
                ref[i][j] = values[i][j];
                ref[i][j] = ref[i][j] + 0.1f;
                if(rand.nextBoolean()){
                    values[i][j] = - values[i][j];
                }
            }
        }
        
        while (!Display.isCloseRequested()) {
            glLoadIdentity();
            camera.applyTranslations();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            checkInput();
            
            glPushMatrix();
            glScalef(2f, 2f, 2f);

            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // go wireframe
            glColor3f(0.1f,0.1f,0.1f);
            glBegin(GL_QUADS);
            for(int i = 0; i < 99; i++){
                for (int j = 0; j < 99; j++){
                    glVertex3f(i, values[i][j] + .001f, j);
                    glVertex3f(i + 1, values[i + 1][j] + .001f, j);
                    glVertex3f(i + 1, values[i + 1][j+1] + .001f, j + 1);
                    glVertex3f(i, values[i][j + 1] + .001f, j + 1);
                }
            }
            glEnd();
                       
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); // go wireframe
            glColor3f(0.5f,0.5f,1.0f);
            glBegin(GL_QUADS);
            for(int i = 0; i < 99; i++){
                for (int j = 0; j < 99; j++){
                    glVertex3f(i, values[i][j], j);
                    glVertex3f(i + 1, values[i + 1][j], j);
                    glVertex3f(i + 1, values[i + 1][j+1], j + 1);
                    glVertex3f(i, values[i][j + 1], j + 1);
                }
            }
            glEnd();
            
//            glBegin(GL_POINTS);
//            for(Point p : points){
//                float t = p.lifetime / p.lmax;
//                //glPointSize(2f * t);
//                //glColor3f(t,0,0);
//                glVertex3f(p.x, p.y, p.z);
//                p.lifetime--;
//                if (p.lifetime <= 0){
//                    queue.add(p);
//                }
//            }
//            glEnd();
            glPopMatrix();
            
            for(int i = 0; i < 99; i++){
                for (int j = 0; j < 99; j++){
                    if (ud[i][j]){
                        values[i][j] = values[i][j] + 0.01f;
                    } else {
                        values[i][j] = values[i][j] - 0.01f;
                    }
                    
                    if (Math.abs(values[i][j]) > Math.abs(ref[i][j])){
                        ud[i][j] = !ud[i][j];
                        points.add(new Point(i, 1.3f, j, 10));
                    }
                }
            }
                
            points.removeAll(queue);
            Display.update();
            Display.sync(60);
        }
    }
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        Noise noise = new Noise();
    }
    
    private void setupdisplay(){
        try {
            DISPLAYMODE = new DisplayMode(800, 600);
            Display.setDisplayMode(DISPLAYMODE);
            Display.setVSyncEnabled(true);
            Display.setTitle("Noise");
            Display.create();
        } catch (LWJGLException e) {
            System.err.println("Couldn't set up the display");
            Display.destroy();
            System.exit(1);
        }
    }
    
    private void setupstates(){
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_NORMALIZE);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_DEPTH_TEST);
        glLight(GL_LIGHT0, GL_POSITION, lightPosition);
        glLightModel(GL_LIGHT_MODEL_AMBIENT, BufferTools.asFlippedFloatBuffer(.01f, 0.01f, 0.01f, 1));
        glLight(GL_LIGHT0, GL_AMBIENT, BufferTools.asFlippedFloatBuffer(.5f, .5f, .5f, 1));
        glLight(GL_LIGHT0, GL_DIFFUSE, BufferTools.asFlippedFloatBuffer(.7f, .7f, .7f, 1));
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glClearColor(.91f, .91f, .91f, 0.001f);
        glFogf(GL_FOG_START, 5f);
        glFogf(GL_FOG_END, 60.0f);
        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogf(GL_FOG_DENSITY, .05f);
        
        FloatBuffer fogColor = BufferUtils.createFloatBuffer(4);
        fogColor.put(1f).put(1f).put(1f).put(1f).flip();
        glFog(GL_FOG_COLOR, fogColor);    
    } 
    
    private void setupcamera(){
        camera = new EulerCamera.Builder().setAspectRatio((float) DISPLAYMODE.getWidth() / DISPLAYMODE.getHeight()).setPosition(23, 34, 87).setRotation(22, 341,
            0).setNearClippingPane(2).setFarClippingPane(60).setFieldOfView(60).build();
        camera.applyOptimalStates();
        camera.applyPerspectiveMatrix();
        camera.setPosition(50, 8, 100);
        camera.setRotation(2, 90, 0);
    }
    
    private void checkInput() {
//        camera.processKeyboard(2, 50);
//        camera.processMouse(1, 80, -80);
        if (Keyboard.isKeyDown(Keyboard.KEY_ESCAPE)) {
            Display.destroy();
            System.exit(0);
        } else if (Keyboard.isKeyDown(Keyboard.KEY_X)) {
            System.out.println("X:" + camera.x() + " Y:" + camera.y() + " Z:" + camera.z());
            System.out.println("Pitch:" + camera.pitch() + "Yaw:" + camera.yaw() + "Roll:" + camera.roll());
        } 
//        if (Mouse.isButtonDown(0)) {
//            Mouse.setGrabbed(true);
//        } else if (Mouse.isButtonDown(1)) {
//            Mouse.setGrabbed(false);
//        }
    }
    
    class Timed extends TimerTask{
        public void run (){
            i++;
            System.out.println("redraw");
            Random r = new Random((long) Math.random() + i);
            for (int i = 0; i < 100; i++){
                for (int j = 0; j < 100; j++){
                    values[i][j] = r.nextFloat();
                }
            }
        }
    }
}
