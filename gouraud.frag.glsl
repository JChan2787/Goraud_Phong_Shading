#version 120
/*
 * Michael Shafae
 * mshafae at fullerton.edu
 * 
 * A simple Gouraud shader with two light sources.
 * This file is the fragment shader which assigns
 * the color calculated in the vertex shader to the
 * fragment.
 *
 * For more information see:
 *     <http://en.wikipedia.org/wiki/Gouraud_shading>
 *
 * $Id: gouraud.frag.glsl 5717 2015-04-15 17:31:49Z mshafae $
 *
 * Be aware that for this course, we are limiting ourselves to
 * GLSL v.1.2. This is not at all the contemporary shading
 * programming environment, but it offers the greatest degree
 * of compatibility.
 *
 * Please do not use syntax from GLSL > 1.2 for any homework
 * submission.
 *
 */

varying vec4 myColor;

void main( ){
  gl_FragColor = myColor;
}