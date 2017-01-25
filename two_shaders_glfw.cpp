// 
// Michael Shafae
// mshafae at fullerton.edu
// 
// The two Utah teapot floating in space. Camera can be rotated up/down
// and left right. The camera's transformations are defined/implemented
// in transformations.h/cpp.
//
// Each teapot has it's own shading program but share the two light sources.
// The lights are defined as objects named SpinningLight.
//
// This demonstration program requires that you provide your own GFXMath.h.
//
// $Id: two_shaders_glfw.cpp 5723 2015-04-15 22:25:35Z mshafae $
//

//Submitted By  :   Jay Chan Jr.
//CWID          :   892984154
//Class         :   CPSC484 TTh @ 1:00PM

#include <cstdlib>
#include <cstdio>
#include <sys/time.h>

#include "GLFWApp.h"
#include "GFXMath.h"

#include "GLSLShader.h"
#include "transformations.h"
#include "glut_teapot.h"

void msglVersion(void){
  fprintf(stderr, "OpenGL Version Information:\n");
  fprintf(stderr, "\tVendor: %s\n", glGetString(GL_VENDOR));
  fprintf(stderr, "\tRenderer: %s\n", glGetString(GL_RENDERER));
  fprintf(stderr, "\tOpenGL Version: %s\n", glGetString(GL_VERSION));
  fprintf(stderr, "\tGLSL Version: %s\n",
          glGetString(GL_SHADING_LANGUAGE_VERSION));
}

class SpinningLight{
public:
  SpinningLight( ){ }
  SpinningLight(Vec3& color, Vec3& position, Vec3& center) :
    _color(color), _position(position), _center(center){
    // Fill me in!
    
    //Hopefully working version
   
    _tangent = Vec3(0.0, 1.0, 0.0);
  }

  Vec4 color( ){return Vec4(_color, 1.0);}
  Vec4 position( ){return Vec4(_position, 1.0);}
  
  //Rotate the lights up and down
  void rotateUp(float degrees){
    // Fill me in!
    
    //Hopefully working version
    
    //Calculating the third axis 
    Vec3 normalVec = normalize(cross(direction(), _tangent));
    
    //Calling the Rodrigues rotation matrix, 
    //since we are rotating by the normal,
    //pass that vector in
    Mat4 rot = rotate(degrees, normalVec);
    
    //Store the position to a vector 4
    Vec4 tmpPos(_position, 0.0);
    
    //Getting the new position
    tmpPos = rot * tmpPos;

    //Store the new position, along with the new tangent vector
    _position = Vec3(tmpPos[0], tmpPos[1], tmpPos[2]);
    _tangent = normalize(cross(normalVec, direction()));

  }

  //Rotate the lights left and right 
  void rotateLeft(float degrees){
    // Fill me in!

    //Hopefuly working version

    //Call Rodrigues rotation matrix, to rotate left and right
    //the axis of rotation will be the tangent vector
    Mat4 rot = rotate(degrees, _tangent);

    //Store the position to a Vec4
    Vec4 tmpPos(_position, 0.0);
    
    //Get and store new position
    tmpPos = rot * tmpPos;
    _position = Vec3(tmpPos[0], tmpPos[1], tmpPos[2]);
  }
  
  //Roll the camera around its direction vector
  void roll(float degrees){
    // Fill me in!

    //Hopefully working version

    //Call Rodrigues rotation matrix, to rotate roll around
    //the axis of rotation will be the direction vector
    Mat4 rot = rotate(degrees, direction());
    
    //Store the tangent to a vector 4
    Vec4 tmpTan(_tangent, 0.0);

    //Only thing should change is the tangent
    tmpTan = rot * tmpTan;
    _tangent = Vec3(tmpTan[0], tmpTan[1], tmpTan[2]);
  }
  
private:
  Vec3 _color;
  Vec3 _position;
  Vec3 _center;
  Vec3 _tangent;
  
  Vec3 direction( ){
    Vec3 d;
    // Fill me in!
    
    //Hopefully working version
    d = _center - _position;
 
    return d;
  }
};

class TwoShadersApp : public GLFWApp{
private:
  float rotationDelta;

  Vec3 centerPosition;
  Vec3 eyePosition;
  Vec3 upVector;

  Vec3 teapotTranslation_A;
  Vec3 teapotTranslation_B;

  Mat4 modelViewMatrix;
  Mat4 projectionMatrix;
  Mat4 normalMatrix;
  
  GLSLProgram shaderProgram_A;
  GLSLProgram shaderProgram_B;

  SpinningLight light0;
  SpinningLight light1; 

  // Variables to set uniform params for lighting fragment shader 
  unsigned int uModelViewMatrix_A;
  unsigned int uProjectionMatrix_A;
  unsigned int uNormalMatrix_A;
  unsigned int uLight0_position_A;
  unsigned int uLight0_color_A;
  unsigned int uLight1_position_A;
  unsigned int uLight1_color_A;
  
  unsigned int uModelViewMatrix_B;
  unsigned int uProjectionMatrix_B;
  unsigned int uNormalMatrix_B;
  unsigned int uLight0_position_B;
  unsigned int uLight0_color_B;
  unsigned int uLight1_position_B;
  unsigned int uLight1_color_B;
  
public:
  TwoShadersApp(int argc, char* argv[]) :
    GLFWApp(argc, argv, std::string("Camera Control").c_str( ), 
            500, 500){ }
  
  void initCenterPosition( ){
    centerPosition = Vec3(0.0, 0.0, 0.0);
  }
  
  void initEyePosition( ){
    eyePosition = Vec3(0.0, 0.0, 8.0);
  }

  void initUpVector( ){
    upVector = Vec3(0.0, 1.0, 0.0);
  }

  void initRotationDelta( ){
    rotationDelta = 5.0;
  }
   
  void initLights( ){
    Vec3 color0(0.6, 0.3, 0.0);
    Vec3 position0(0.0, 5.0, 10.0);
    Vec3 color1(0, 0.3, 0.6);
    Vec3 position1(0.0, 5.0, -10.0);
    light0 = SpinningLight(color0, position0, centerPosition);
    light1 = SpinningLight(color1, position1, centerPosition);
  }
  
  void initTeapotTransforms( ){
    teapotTranslation_A = Vec3(3.0, 0.0, 0.0);
    teapotTranslation_B = Vec3(-3.0, 0.0, 0.0);
  }
  
  bool begin( ){
    msglError( );
    initCenterPosition( );
    initEyePosition( );
    initUpVector( );
    initRotationDelta( );
    initLights( );
    initTeapotTransforms( );
    
    // Load shader program A
    const char* vertexShaderSource_A = "gouraud.vert.glsl";
    const char* fragmentShaderSource_A = "gouraud.frag.glsl";
    FragmentShader fragmentShader_A(fragmentShaderSource_A);
    VertexShader vertexShader_A(vertexShaderSource_A);
    shaderProgram_A.attach(vertexShader_A);
    shaderProgram_A.attach(fragmentShader_A);
    shaderProgram_A.link( );
    shaderProgram_A.activate( );
    
    printf("Shader program A built from %s and %s.\n",
           vertexShaderSource_A, fragmentShaderSource_A);
    if( shaderProgram_A.isActive( ) ){
      printf("Shader program is loaded and active with id %d.\n", shaderProgram_A.id( ) );
    }else{
      printf("Shader program did not load and activate correctly. Exiting.");
      exit(1);
    }
    
    // Load shader program B
    const char* vertexShaderSource_B = "blinn_phong.vert.glsl";
    const char* fragmentShaderSource_B = "blinn_phong.frag.glsl";
    FragmentShader fragmentShader_B(fragmentShaderSource_B);
    VertexShader vertexShader_B(vertexShaderSource_B);
    shaderProgram_B.attach(vertexShader_B);
    shaderProgram_B.attach(fragmentShader_B);
    shaderProgram_B.link( );
    shaderProgram_B.activate( );
    
    printf("Shader program B built from %s and %s.\n",
           vertexShaderSource_B, fragmentShaderSource_B);
    if( shaderProgram_B.isActive( ) ){
      printf("Shader program is loaded and active with id %d.\n", shaderProgram_B.id( ) );
    }else{
      printf("Shader program did not load and activate correctly. Exiting.");
      exit(1);
    }
    
    // Set up uniform variables for shader program A
    uModelViewMatrix_A = glGetUniformLocation(shaderProgram_A.id( ), "modelViewMatrix");
    uProjectionMatrix_A = glGetUniformLocation(shaderProgram_A.id( ), "projectionMatrix");
    uNormalMatrix_A = glGetUniformLocation(shaderProgram_A.id( ), "normalMatrix");
    uLight0_position_A = glGetUniformLocation(shaderProgram_A.id( ), "light0_position");
    uLight0_color_A = glGetUniformLocation(shaderProgram_A.id( ), "light0_color");
    uLight1_position_A = glGetUniformLocation(shaderProgram_A.id( ), "light1_position");
    uLight1_color_A = glGetUniformLocation(shaderProgram_A.id( ), "light1_color");
    
    // Set up uniform variables for shader program B
    uModelViewMatrix_B = glGetUniformLocation(shaderProgram_B.id( ), "modelViewMatrix");
    uProjectionMatrix_B = glGetUniformLocation(shaderProgram_B.id( ), "projectionMatrix");
    uNormalMatrix_B = glGetUniformLocation(shaderProgram_B.id( ), "normalMatrix");
    uLight0_position_B = glGetUniformLocation(shaderProgram_B.id( ), "light0_position");
    uLight0_color_B = glGetUniformLocation(shaderProgram_B.id( ), "light0_color");
    uLight1_position_B = glGetUniformLocation(shaderProgram_B.id( ), "light1_position");
    uLight1_color_B = glGetUniformLocation(shaderProgram_B.id( ), "light1_color");
    
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    msglVersion( );
    
    return !msglError( );
  }
  
  bool end( ){
    windowShouldClose( );
    return true;
  }
  
  void activateUniforms_A(Vec4& _light0, Vec4& _light1){
    glUniformMatrix4fv(uModelViewMatrix_A, 1, false, modelViewMatrix);    
    glUniformMatrix4fv(uProjectionMatrix_A, 1, false, projectionMatrix);
    glUniformMatrix4fv(uNormalMatrix_A, 1, false, normalMatrix);
    glUniform4fv(uLight0_position_A, 1, _light0);
    glUniform4fv(uLight0_color_A, 1, light0.color( ));
    glUniform4fv(uLight1_position_A, 1, _light1);
    glUniform4fv(uLight1_color_A, 1, light1.color( ));
  }
  
  void activateUniforms_B(Vec4& _light0, Vec4& _light1){
    glUniformMatrix4fv(uModelViewMatrix_B, 1, false, modelViewMatrix);
    glUniformMatrix4fv(uProjectionMatrix_B, 1, false, projectionMatrix);
    glUniformMatrix4fv(uNormalMatrix_B, 1, false, normalMatrix);
    glUniform4fv(uLight0_position_B, 1, _light0);
    glUniform4fv(uLight0_color_B, 1, light0.color( ));
    glUniform4fv(uLight1_position_B, 1, _light1);
    glUniform4fv(uLight1_color_B, 1, light1.color( ));
  }
  
  bool render( ){
    Vec4 _light0;
    Vec4 _light1;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Vec2i w = windowSize( );
    double ratio = double(w[0]) / double(w[1]);

    projectionMatrix = perspective(90.0, ratio, 1.0, 25.0);

    Mat4 lookAtMatrix = lookat(eyePosition, centerPosition, upVector);

    // Set light & material properties for the teapot;
    // lights are transformed by current modelview matrix
    // such that they are positioned correctly in the scene.
    _light0 = lookAtMatrix * light0.position( );
    _light1 = lookAtMatrix * light1.position( );
    
    modelViewMatrix = lookAtMatrix * translate(teapotTranslation_A);
    normalMatrix = modelViewMatrix.inverse( ).transpose( );
    shaderProgram_A.activate( );
    activateUniforms_A(_light0, _light1);
    _glutSolidTeapot(1.0);
    
    // Let's reuse _light0 and _light1
    modelViewMatrix = lookAtMatrix * translate(teapotTranslation_B);
    normalMatrix = modelViewMatrix.inverse( ).transpose( );
    shaderProgram_B.activate( );
    activateUniforms_B(_light0, _light1);
    _glutSolidTeapot(1.0);
    
    if(isKeyPressed('Q')){
      end( );      
    }else if(isKeyPressed(GLFW_KEY_EQUAL)){
      rotationDelta += 1.0;
      printf( "Rotation delta set to %g\n", rotationDelta );
    }else if(isKeyPressed(GLFW_KEY_MINUS)){
      rotationDelta -= 1.0;
      printf( "Rotation delta set to %g\n", rotationDelta );
    }else if(isKeyPressed('R')){
      initEyePosition( );
      initUpVector( );
      initRotationDelta( );
      initLights( );  
      printf("Eye position, up vector and rotation delta reset.\n");
    }else if(isKeyPressed(GLFW_KEY_LEFT)){
      rotateCameraLeft(-rotationDelta, eyePosition,
                       centerPosition, upVector);
    }else if(isKeyPressed(GLFW_KEY_RIGHT)){
      rotateCameraLeft(rotationDelta, eyePosition,
                       centerPosition, upVector);
    }else if(isKeyPressed(GLFW_KEY_UP)){
      rotateCameraUp(-rotationDelta, eyePosition,
                     centerPosition, upVector);
    }else if(isKeyPressed(GLFW_KEY_DOWN)){
      rotateCameraUp(rotationDelta, eyePosition,
                     centerPosition, upVector);
    }else if(isKeyPressed('W')){
      light0.rotateUp(rotationDelta);
    }else if(isKeyPressed('S')){
      light0.rotateUp(-rotationDelta);
    }else if(isKeyPressed('A')){
      light0.rotateLeft(rotationDelta);
    }else if(isKeyPressed('D')){
      light0.rotateLeft(-rotationDelta);
    }else if(isKeyPressed('X')){
      light0.roll(rotationDelta);
    }else if(isKeyPressed('Y')){
      light1.rotateUp(rotationDelta);
    }else if(isKeyPressed('H')){
      light1.rotateUp(-rotationDelta);
    }else if(isKeyPressed('G')){
      light1.rotateLeft(rotationDelta);
    }else if(isKeyPressed('J')){
      light1.rotateLeft(-rotationDelta);
    }else if(isKeyPressed('N')){
      light1.roll(rotationDelta);
    }
    return !msglError( );
  }
    
};


int main(int argc, char* argv[]){
  TwoShadersApp app(argc, argv);
  return app();
}

