/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package noise;

/**
 *
 * @author Henry Dane
 */
public class Point {
    public float x, y, z;
    public float lmax;
    public float lifetime;
    
    public Point(float x, float y, float z, float lifetime){
        this.x = x;
        this.y = y;
        this.z = z;
        this.lifetime = lifetime;
        this.lmax = lifetime;
    }
}
