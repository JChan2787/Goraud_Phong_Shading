// 
// Michael Shafae
// mshafae at fullerton.edu
// 
// Procedural module that implements transformations used in
// the homework assignment.
//
// $Id: transformations.cpp 5554 2015-02-19 06:59:50Z mshafae $
//
// STUDENTS _MUST_ ADD THEIR CODE INTO THIS FILE
//

/*
  Modified By:  Jose Vicente D. Chan Jr.
  CLASS      :  CPSC 484
  TIME       :  TTh @ 1:00PM
 */

#include "transformations.h"

#ifndef __SOLUTION__

//Implementaion to implement rotateCameraLeft
//Will rotate camera sideways
void rotateCameraLeft(float degrees, Vec3& eyePosition, Vec3& centerPosition, Vec3& upVector){
  // Please implement this function.

  //Hopefully working version

  //Calling the rotate function and will give us a 
  //rotation matrix based on the Rodrigues rotation formula
  //Since we are rotating sideways, we will be using the
  //upVector as our rotation axis
  Mat4 rot = rotate(degrees, upVector);

  //Putting the eyePosition into a Vec4 object
  Vec4 newEye(eyePosition, 0.0);

  //Get the new eyeposition by multiplying it with our 
  //rotation matrix
  newEye = rot * newEye; 

  //Update eyeposition
  eyePosition = Vec3(newEye[0], newEye[1], newEye[2]);
}

//Implementation to implement rotateCameraUp
//Will rotate camera up and down
void rotateCameraUp(float degrees, Vec3& eyePosition, Vec3& centerPosition, Vec3& upVector){
  // Please implement this function.

  //Hopefully working version

  //Get the gaze vector
  Vec3 gazeVec = centerPosition - eyePosition;
  
  gazeVec = normalize(gazeVec);


  //The right vector is determined by taking the cross product 
  //between the gaze and the up vectors
  Vec3 rightVec = cross(gazeVec, upVector);

  rightVec = normalize(rightVec);

  //Normalize the right vector
  Vec3 normRight = normalize(rightVec);

  //Calling rotate function to get rotation matrix based on 
  //our right normalized vector
  Mat4 rot = rotate(degrees, normRight);
  
  //Putting the eyePosition into a Vec4 object
  Vec4 newEye(eyePosition, 0.0);
  
  //Get the new eyeposition by multiplying it with our 
  //rotation matrix
  newEye = rot * newEye;
  
  //Update eyePosition
  eyePosition = Vec3(newEye[0], newEye[1], newEye[2]);

  //With our new eyePosition, get the gaze vector,
  //we will use this to get the new position for
  //our upVector
  gazeVec = centerPosition - eyePosition;

  gazeVec = normalize(gazeVec);
  
  //Update our upVector by taking the cross product 
  //of rightVector and gazeVector, as well as 
  //normalizing it.
  upVector = normalize(cross(rightVec, gazeVec));
  
}

#else
#include "transformations_solution.cpp"
#endif
