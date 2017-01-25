#version 120
/*
 * Michael Shafae
 * mshafae at fullerton.edu
 * 
 * A simple Gouraud shader with two light sources.
 * This file is the vertex shader which calculates
 * the the vector to the light, the vector to the viewer,
 * and the reflection vector. The color is calculated
 * using the Phong illumination equation.
 *
 * For more information see:
 *     <http://en.wikipedia.org/wiki/Gouraud_shading>
 *
 * $Id: gouraud.vert.glsl 5717 2015-04-15 17:31:49Z mshafae $
 *
 * Be aware that for this course, we are limiting ourselves to
 * GLSL v.1.2. This is not at all the contemporary shading
 * programming environment, but it offers the greatest degree
 * of compatability.
 *
 * Please do not use syntax from GLSL > 1.2 for any homework
 * submission.
 *
 */

//Submitted By	: Jay Chan Jr.
//CWID	    	: 892945154
//Class		: CPSC 484 TTh @ 1:00PM

// These are passed in from the CPU program, camera_control_*.cpp
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;
uniform vec4 light0_position;
uniform vec4 light0_color;
uniform vec4 light1_position;
uniform vec4 light1_color;

// out
varying vec4 myColor;

void main( ){
  vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);
  vec4 diffuse = vec4(0.5, 0.5, 0.5, 1.0);
  vec4 specular = vec4(1.0, 1.0, 1.0, 1.0);
  float shininess = 100;
  
  // Transforming The Vertex
  gl_Position = projectionMatrix * modelViewMatrix * gl_Vertex;

  // Fill me in!

  //Hopefully working version
  
  vec3 view = vec3(modelViewMatrix * gl_Vertex);
  vec3 norm = gl_Normal;
  
  //Getting the direction vector for both light-sources
  vec3 dirLight0 = vec3(gl_Position - light0_position);
  vec3 dirLight1 = vec3(gl_Position - light1_position);

  //Normalizing the direction vectors
  vec3 _dirLight0 = normalize(-dirLight0);
  vec3 _dirLight1 = normalize(-dirLight1);
  
  //Normalizing the viewing vector
  vec3 _view = normalize(view);  

  //Getting the dot product of N & L
  float nDotL_l0 = dot(norm, _dirLight0);
  float nDotL_l1 = dot(norm, _dirLight1); 

  //Eye is always at (0, 0, 0) looking down -z axis
  const vec3 eye = vec3(0, 0, 0);
  vec3 eyeNorm = normalize(-_view);

  //Calculating the dot product of the reflection and view
  //vectors
  vec3 twoLight0 = _dirLight0 * 2; 
  vec3 twoLight1 = _dirLight1 * 2;
  vec3 twoLightTimesnDotL0 = normalize(twoLight0 * nDotL_l0); 
  vec3 twoLightTimesnDotL1 = normalize(twoLight1 * nDotL_l1);  
  vec3 subtractWithL0 = normalize(twoLightTimesnDotL0 - _dirLight0);
  vec3 subtractWithL1 = normalize(twoLightTimesnDotL1 - _dirLight1);
  float rDotV0 = dot(subtractWithL0, -_view);
  float rDotV1 = dot(subtractWithL1, -_view);

  // The next two lines are place holders.
  vec4 color0 = vec4(0.0, 0.0, 0.0, 1.0);
  vec4 color1 = vec4(0.0, 0.0, 0.0, 1.0);

  //Diffuse:
  vec4 _diffLight0 = clamp(diffuse * light0_color * max(nDotL_l0, 0.0), 0.0, 1.0);
  vec4 _diffLight1 = clamp(diffuse * light1_color * max(nDotL_l1, 0.0), 0.0, 1.0);
  color0 = _diffLight0 + _diffLight1;
     
  //Specular: 
  vec4 _specLight0 = clamp(specular * light0_color * pow(max(rDotV0, 0.0), shininess), 0.0, 1.0);
 
  vec4 _specLight1 = clamp(specular * light1_color * pow(max(rDotV1, 0.0), shininess), 0.0, 1.0); 

  color1 = _specLight0 + _specLight1;
  
  // Sum the colors and pass it along to the fragment shader.
  myColor = ambient + color0 + color1;
}
