//--------------------------------------------------------------------------------------
// Copyright 2015 Intel Corporation
// All Rights Reserved
//
// Permission is granted to use, copy, distribute and prepare derivative works of this
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  THIS SOFTWARE IS PROVIDED 'AS IS.'
// INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, AND ALL LIABILITY,
// INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES, FOR THE USE OF THIS SOFTWARE,
// INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY RIGHTS, AND INCLUDING THE
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.
//--------------------------------------------------------------------------------------
package com.intel.realsensesamples.javafx;

import javafx.scene.paint.Color;
import javafx.scene.paint.PhongMaterial;
import javafx.scene.shape.Box;
import javafx.scene.transform.Rotate;

/**
 * Class to represent Cube
 * @author fpedroso
 */
public class Cube extends Box {
    
    // Default Radius
    public static final double DEFAULT_RADIUS = 1.0;
    
    // Cube Rotation Consts
    private final Rotate RotationX = new Rotate(0, Rotate.X_AXIS);
    private final Rotate RotationY = new Rotate(0, Rotate.Y_AXIS);
    private final Rotate RotationZ = new Rotate(0, Rotate.Z_AXIS);
    
    /**
     * Cube constructor with specific size and color
     * @param size
     * @param color 
     */
    public Cube(double size, Color color) {
        super(size, size, size);
        this.setMaterial(new PhongMaterial(color));
        this.getTransforms().addAll(RotationX, RotationY, RotationZ);
    }
    
    /**
     * Set X cube rotation
     * @param angle 
     */
    public void setRotationX(double angle) {
        RotationX.setAngle(angle);
    }
    
    /**
     * Set Y cube rotation
     * @param angle 
     */
    public void setRotationY(double angle) {
        RotationY.setAngle(angle);
    }
    
    /**
     * Set Z cube rotation
     * @param angle 
     */
    public void setRotationZ(double angle) {
        RotationZ.setAngle(angle);
        
    }
    
    /**
     * Sets Cube color
     * @param color 
     */
    public void setColor(Color color){
        ((PhongMaterial)getMaterial()).setDiffuseColor(color);
    }
}
