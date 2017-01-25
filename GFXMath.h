/*
 * Copyright (c) 2015 Michael Shafae
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * $Id: GFXMath.h 5509 2015-02-10 10:04:54Z mshafae $
 *
 */

#pragma clang diagnostic ignored "-Wunused-function"

#ifndef _GFXMATH_H_
#define _GFXMATH_H_
#ifdef WIN32
#pragma once
#define _USE_MATH_DEFINES 1
#endif

#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>

#ifdef __linux__
#ifdef minor
#undef minor
#endif
#ifdef major
#undef major
#endif
#endif

#ifndef IOS_FP_PRECISION
#define IOS_FP_PRECISION 5
#endif

#ifndef M_PI
#define M_PI        3.14159265358979323846264338327950288
#endif

#ifndef PI_OVER_ONE_EIGHTY
#define PI_OVER_ONE_EIGHTY 0.01745329251994329547437168059786927188
#endif

#ifndef ONE_EIGHTY_OVER_PI
#define ONE_EIGHTY_OVER_PI 57.29577951308232286464772187173366546631
#endif

template <typename T>
static inline T sqr(T a)
{
  return a * a;
}

template <typename T>
static inline T _pow(T a, unsigned int exponent)
{
  T product = T(1);
  for(int i = 0; i < exponent; i++){
    product *= a;
  }
  return product;
}



#ifndef MSGFX_FP
#define MSGFX_FP

/*
 * One option is to make the Vector and Matrix classes
 * aware of how much precision is really needed. Barring
 * that, let's have a few templated functions that help
 * in determining if two floating point values are equal.
 */

/*
 * These two defines are useful if you want to remember
 * how much precision you'll have with your float or double
 * types. However, 7 decimal places may be overkill for our
 * work. Consider defining your own macro or static variable
 * that defines how much precision you want and need.
 *
 * Single precision floating point numbers have a
 * significand of 24 bits which is about 7 decimal places.
 * Double precision floating point numbers have a
 * significand of 53 bits which is about 16 decimal places.
 */
#define FP_SP_EPSILON 1e-6
#define FP_DP_EPSILON 1e-15

template <typename T>
static bool fpEqual(T a, T b, double epsilon)
{
  return (a == b) || ((a - epsilon) < b && (a + epsilon) > b);
}

template <typename T>
static bool fpNotEqual(T a, T b, double epsilon)
{
  return (a != b) && ((a - epsilon) > b || (a + epsilon) < b);
}

#endif

template <typename T>
static T degreesToRadians(T degrees)
{
  return degrees * static_cast<T>(PI_OVER_ONE_EIGHTY);
}

template <typename T>
static T radiansToDegrees(T radians)
{
  return radians * static_cast<T>(ONE_EIGHTY_OVER_PI);
}

/*
 * See Park & Miller's paper Random Number Generators: Good Ones are 
 * Hard to Find, Communications of the ACM, October 1988, Vol. 31, no. 10
 *
 * In a nutshell, this is Lehmer's parametric multiplicative
 * linear congruential algorithm to generate a series of psuedo random
 * unsigned integers. MS Window's standard library has a broken
 * implementation so it's included here in case someone needs to use
 * it.
 */
// This should be a Singleton but it's not.
class LPMRandom{
public:
  LPMRandom( unsigned int seed = 1 ){
    if(seed < 1 || seed >= m){
      seed = 1234557890;
    }
    this->seed = seed;
  };
  void reseed(unsigned int s){
    seed = s;
  }
  unsigned int operator ()(void){
    return _rand( );
  }
  operator int( ){
    return static_cast<int>(_rand( ));
  }
  operator float( ){
    // invM = 1 / m
    float invM = 0.0000000004656612875245797;
    return _rand( ) * invM;
  }
  operator double( ){
    // invM = 1 / m
    double invM = 0.0000000004656612875245797;
    return _rand( ) * invM;
  }
  float frand( ){
    return static_cast<float>(*this);
  }
  double drand( ){
    return static_cast<double>(*this);
  }
  int irand( ){
    return static_cast<int>(*this);
  }
private:
  unsigned int seed;
  // The multiplier -- 7**5 = 16807
  const static unsigned int a = 16807;
  // The modulo -- a large prime number
  const static unsigned int m = 2147483647;
  // quotient -- m div a
  const static unsigned int q = 127773;
  // remainder -- m mod a
  const static unsigned int r = 2836;
  unsigned int _rand( ){
    unsigned int lo, hi;
    int test;
    hi = seed / q;
    lo = seed % q;
    test = a * lo - r * hi;
    if( test < 0 ){
      test += m;
    }
    seed = test % m;
    return seed;
  }
}; // end class LPMRandom

template <typename T, const int length>
class VecN{
public:
  typedef class VecN<T,length> this_t;
  typedef T component_t;

  VecN( ){};

  VecN(const VecN& rhs){
    assign(rhs);
  }
  
  VecN(const VecN* rhs){
    assign(rhs);
  }
  
  VecN(const T* array){
    assign(array);
  }
  
  VecN(T const & s){
    for( int i = 0; i < length; i++ ){
      data[i] = s;
    }
  }
  
  VecN& operator =(const VecN& rhs){
    assign(rhs);
    return *this;
  }

  VecN& operator =(const VecN* rhs){
    assign(rhs);
    return *this;
  }
  
  VecN& operator =(const T& s){
    assign(s);
    return *this;
  }
  
  //Overloaded addition operator to work for adding two vectors together
  VecN operator +(const VecN& rhs) const{
    VecN sum;
    // Fill me in!

    //Hopefully working version
    for(int i = 0; i < length; i++)
      {
	sum.data[i] = data[i] + rhs.data[i];
      }

    return sum;
  }
  
  VecN& operator +=(const VecN& rhs){
    return *this = *this + rhs;
  }

  //Unary operator, converts the vector's components to a negative sign,
  //and vice-versa
  VecN operator -( ) const{
    VecN rv;
    // Fill me in!

    //Hopefully working version
    for(int i = 0; i < length; i++)
      {
	rv.data[i] = (-1)*(data[i]);
      }
      
    return rv;
  }
  
  //Substraction operator to subtract two vectors
  VecN operator -(const VecN& rhs) const{
    VecN difference;
    // Fill me in!

    //Hopefully working version
    for(int i = 0; i < length; i++){
      difference.data[i] = data[i] - rhs.data[i];
    }

    return difference;
  }
  
  VecN& operator -=(const VecN& rhs){
    return *this = *this - rhs;
  }
  
  //Scalar vector multiplication
  VecN operator *(const T& s) const{
    VecN rv;
    // Fill me in!

    //Hopefully working version
    for(int i = 0; i < length; i++){
      rv.data[i] = s * data[i];
    }

    return rv;
  }
  
  VecN operator *=(const T& s){
    assign(*this * s);
    return *this;
  }
  
  //Scalar vector division
  VecN operator /(const T& s) const{
    VecN rv;
    // Fill me in!

    //Hopefully working version

    if(s != T(0)){
      for(int i = 0; i < length; i++){
	rv.data[i] = data[i]/s;
      }
    }
    else{
      throw("Cannot divide by zero");
    }

    return rv;
  }
  
  VecN operator /=(const T& s){
    assign(*this / s);
    return *this;
  }
  
  T& operator [](unsigned int i){
    if( i < length ){
      return data[i];
    }else{
      throw( "Index out of range" );
    }
  }
  
  const T& operator [](unsigned int i) const{
    if( i < length ){
      return data[i];
    }else{
      throw( "Index out of range" );
    }
  }
  
  //Overloaded boolean equals operator in order to check if 
  //two vectors are equal
  bool operator ==(const VecN& rhs) const{
    bool rv = false;
    // Fill me in!

    //Hopefully working version
    int n = 0;
    float precision = FP_SP_EPSILON;
    
    do{

      if(fpEqual(data[n], rhs.data[n], precision)){
	rv = true;
      }
      else{
	rv = false;
      }

      n++;

    }while(n < length && rv == true);

    return rv;
  }
  
  //Boolean not equals operator that will check of two vectors are
  //not equal to each other.
  bool operator !=(const VecN& rhs) const{
    bool rv = false;
    // Fill me in!

    //Hopefully working version
    int n = 0;
    float precision = FP_SP_EPSILON;

    do{
      
      if(fpNotEqual(data[n], rhs.data[n], precision)){
	rv = true;
      }
      else{
	rv = false;
      }

      n++;
      
    }while(n < length && rv == true);

    return rv;
  }
  
  static int size( ){
    return length;
  }
  
  operator T*( ){
    return &data[0];
  }

  operator const T*( ) const{
    return &data[0];
  }
  
  std::ostream& write( std::ostream &out ) const{
    out.setf( std::ios::fixed );
    out << std::setprecision(IOS_FP_PRECISION)
      << "(";
    for( int i = 0; i < length; i++ ){
      if( i < (length - 1) ){
        out << " " << data[i] << std::endl;
      }else{
        out << " " << data[i] << " )" << std::endl;
      }
    }
    out.unsetf( std::ios::fixed );
    return( out );
  }
  
  std::ostream& write_row( std::ostream &out) const{
    out.setf( std::ios::fixed );
    out << std::setprecision(IOS_FP_PRECISION)
      << "(";
    for( int i = 0; i < length; i++ ){
      if( i < (length - 1) ){
        out << data[i] << ", ";
      }else{
        out << data[i] << ")" << std::endl;
      }
    }
    out.unsetf( std::ios::fixed );
    return( out );
  }
  
  std::ostream& description( std::ostream &out = std::cerr ) const{
    out.setf( std::ios::fixed );
    out << std::setprecision(IOS_FP_PRECISION) <<
      "<Vec"<< length << " " << this << "> ";
    out << "(";
    for( int i = 0; i < length; i++ ){
      if( i < (length - 1) ){
        out << data[i] << ", ";
      }else{
        out << data[i] << ")" << std::endl;
      }
    }
    out.unsetf( std::ios::fixed );
    return( out );
  }
  
  
protected:
  T data[length];

  inline void assign(const VecN& rhs){
    for( int i = 0; i < length; i++ ){
      data[i] = rhs.data[i];
    }
  }

  inline void assign(const T& s){
    for( int i = 0; i < length; i++ ){
      data[i] = s;
    }
  }
  
  inline void assign(const T* array){
    for( int i = 0; i < length; i++ ){
      data[i] = array[i];
    }
  }
  
  
  inline void assign(const VecN* rhs){
    for( int i = 0; i < length; i++ ){
      data[i] = rhs->data[i];
    }
  }

}; // end class VecN

template <typename T>
class TVec1 : public VecN<T, 1>{
public:
  typedef VecN<T, 1> base;
  TVec1( ) : base( ){ }
  TVec1(const base& v) : base(v){ }
  explicit TVec1(const base* v) : base(v){ }
  explicit TVec1(T x){
    base::data[0] = x;
  }
}; // end class TVec1

template <typename T>
class TVec2 : public VecN<T, 2>{
public:
  typedef VecN<T, 2> base;
  TVec2( ) : base( ){ }
  TVec2(const base& v) : base(v){ }
  explicit TVec2(const base* v) : base(v){ }
  explicit TVec2(T x, T y){
    base::data[0] = x;
    base::data[1] = y;
  }
  base perp( ){
    // Fill me in!
    return base(0, 0);
  }
}; // end class TVec2

template <typename T>
class TVec3 : public VecN<T, 3>{
public:
  typedef VecN<T, 3> base;
  TVec3( ) : base( ){ }
  TVec3(const base& v) : base(v){ }
  explicit TVec3(const base* v) : base(v){ }
  explicit TVec3(T x, T y, T z){
    base::data[0] = x;
    base::data[1] = y;
    base::data[2] = z;
  }
  
  explicit TVec3(const TVec2<T>& v, T z){
    base::data[0] = v[0];
    base::data[1] = v[1];
    base::data[2] = z;
  }
}; // end class TVec3

template <typename T>
class TVec4 : public VecN<T, 4>{
public:
  typedef VecN<T, 4> base;
  TVec4( ) : base( ){ }
  TVec4(const base& v) : base(v){ }
  explicit TVec4(const base* v) : base(v){ }
  explicit TVec4(T x, T y, T z, T w){
    base::data[0] = x;
    base::data[1] = y;
    base::data[2] = z;
    base::data[3] = w;
  }
  
  explicit TVec4(const TVec2<T>& v, T z, T w){
    base::data[0] = v[0];
    base::data[1] = v[1];
    base::data[2] = z;
    base::data[3] = w;
  }
  
  explicit TVec4(const TVec3<T>& v, T w){
    base::data[0] = v[0];
    base::data[1] = v[1];
    base::data[2] = v[2];
    base::data[3] = w;
  }
}; // end class TVec4

template <typename T, int length>
static const VecN<T, length> operator *(T lhs, const VecN<T, length>& rhs){
  return rhs * lhs;
}

template <typename T>
static const TVec1<T> operator /(T lhs, const TVec1<T>& rhs){
  return TVec1<T>(lhs / rhs[0]);
}

template <typename T>
static const TVec2<T> operator /(T lhs, const TVec2<T>& rhs){
  return TVec2<T>(lhs / rhs[0], lhs / rhs[1]);
}

template <typename T>
static const TVec3<T> operator /(T lhs, const TVec3<T>& rhs){
  return TVec3<T>(lhs / rhs[0], lhs / rhs[1], lhs / rhs[2]);
}

template <typename T>
static const TVec4<T> operator /(T lhs, const TVec4<T>& rhs){
  return TVec4<T>(lhs / rhs[0], lhs / rhs[1], lhs / rhs[2], lhs / rhs[3]);
}

//Calculates the dot product between two vectors of the same size
//MUST have the same size or this function will throw an error
//message out
template <typename T, int length>
static T dot(const VecN<T, length>& a, const VecN<T, length>& b){
  T rv = static_cast<T>(0);
  // Fill me in!

  //Hopefully working version
  if(a.size() == b.size()){

    for(int i = 0; i < a.size(); i++){
      rv = rv + (a[i] * b[i]);
    }
  }
  else{
    throw("Vectors must be the same size");
  }
  return rv;
}

//Calculates the squared length of a vector. Meaning:
// V = x^2 + y^2 + z^2 + ... + _z^2
template <typename T, int length>
static T squaredLength(const VecN<T, length>& v){
  T rv(0);
  // Fill me in!

  //Hopefully working version
  for(int i = 0; i < v.size(); i++){
    rv = rv + sqr(v[i]);
  }
  
  return rv;
}

template <typename T, int length>
static T length(const VecN<T, length>& v){
  return sqrt(squaredLength(v));
}

//Normalizes the vector into unit length
template <typename T, int _length>
static VecN<T, _length> normalize(const VecN<T, _length>& v){
  VecN<T, _length> rv;
  // Fill me in!

  //Hopefully working version
  T tv(0);

  tv = sqrt(squaredLength(v));

  for(int i = 0; i < v.size(); i++){

    rv[i] = v[i] / tv;
    
  }
  return rv;
}

//Calculates the distance between two vectors
template <typename T, int length>
static T distance(const VecN<T, length>& a, const VecN<T, length>& b){
  T rv;
  // Fill me in!

  VecN<T, length> tmp; 

  //Hopefully working version
  if(a.size() == b.size()){
    
    for(int i = 0; i < a.size(); i++){
      
      //Add the result to rv
      rv = rv + sqr(a[i] - b[i]);

    }
    
  }
  else{
    throw("Vectors must be the same size");
  }

  rv = sqrt(rv);

  return rv;
}

//Calculates the angle in radians between two Vector 2's
template <typename T>
static T angleInRadians(const TVec2<T>& a, const TVec2<T>& b){
  T rv(0);
  // Fill me in!

  //Hopefully working version
  rv = (length(a) * length(b));
  rv = (dot(a,b) / rv);
  rv = acos(rv);

  return rv;
}

//Calculates the angle in radians between two Vector 3's
template <typename T>
static T angleInRadians(const TVec3<T>& a, const TVec3<T>& b){
  T rv(0);
  // Fill me in!

  //Hopefully working version
  rv = (length(a) * length(b));
  rv = (dot(a,b) / rv);
  rv = (acos(rv));

  return rv;
}

template <typename T>
static T angle(const TVec2<T>& a, const TVec2<T>& b){
  // In degrees!
  return radiansToDegrees(angleInRadians(a, b));
}

template <typename T>
static T angle(const TVec3<T>& a, const TVec3<T>& b){
  // In degrees!
  return radiansToDegrees(angleInRadians(a, b));
}

//Cross product
template <typename T>
static VecN<T, 3> cross(const VecN<T, 3>& a, const VecN<T, 3>& b){
  // Fill me in!

  return  TVec3<T>(
		   ((a[1] * b[2]) - (a[2] * b[1])),
		   ((-1) * ((a[0] * b[2]) - (a[2] * b[0]))),
		   ((a[0] * b[1]) - (a[1] * b[0]))
		  );
}

template <typename T, int length>
std::ostream& operator <<( std::ostream &out, const VecN<T, length> &v ){
  return(v.write_row( out ));
}

typedef TVec1<float> Vec1;
typedef TVec1<double> Vec1d;
typedef TVec1<int> Vec1i;

typedef TVec2<float> Vec2;
typedef TVec2<double> Vec2d;
typedef TVec2<int> Vec2i;

typedef TVec3<float> Vec3;
typedef TVec3<double> Vec3d;
typedef TVec3<int> Vec3i;

typedef TVec4<float> Vec4;
typedef TVec4<double> Vec4d;
typedef TVec4<int> Vec4i;


template <typename T, const int w, const int h>
class MatNM{
public:
  typedef class MatNM<T, w, h> this_t;
  typedef class VecN<T, h> vec_t;
  
  MatNM( ) {};
  
  MatNM(const MatNM& rhs){
    assign(rhs);
  }
  
  explicit MatNM(const T s){
    assign(s);
  }
  
  explicit MatNM(const vec_t& v){
    assign(v);
  }
  
  explicit MatNM(const T* array){
    assign(array);
  }  
  
  MatNM& operator =(const this_t& rhs){
    assign(rhs);
    return *this;
  }
  
  //Overloaded '+' operator to add Matrices. 
  //Has to be matrices of the same dimensions.
  //Or else an error statement will be printed out.
  //The addition process is column by row
  MatNM operator +(const this_t& rhs) const{
    this_t sum;
    // Fill me in!

    //Hopefully working version
    
    //Check if the Matrices' dimensions match
    if(w == rhs.width() && h == rhs.height()){
      for(int i = 0; i < w; i++){
	sum.cols[i] = cols[i] + rhs.cols[i];
      }
    }
    else{
      throw("Matrices has to be the same dimensions");
    }

    return sum;
  }
  
  this_t& operator +=(const this_t& rhs){
    return (*this = *this + rhs);
  }
  
  //Overloaded unary operator to turn all values in the 
  //matrix to their opposite signs
  this_t operator -( ) const{
    this_t rv;
    // Fill me in!

    //Hopefully working version
    for(int i = 0; i < w; i++){
      rv.cols[i] = -cols[i];
    }
    return rv;
  }
  
  //Overloaded '-' operator for matrix subtraction
  //Has to be the same size for both matrices 
  //or this will spit out an error
  this_t operator -(const this_t& rhs) const{
    this_t difference;
    // Fill me in!
    
    //Hopefully working version
    if(w == rhs.width() && h == rhs.height()){
      for(int i = 0; i < w; i++){
	difference.cols[i] = cols[i] - rhs.cols[i];
      }
    }
    else{
      throw("Matrix dimensions do not match");
    }
    return difference;
  }
  
  this_t& operator -=(const this_t& rhs){
    return (*this = *this - rhs);
  }
      
  //Overloaded operator for scalar matrix 
  //multiplication
  this_t operator *(const T& rhs) const{
    this_t product;
    // Fill me in!

    //Hopefully working version
    for(int i = 0; i < w; i++){
      product.cols[i] = rhs * cols[i];
    }
    
    return product;
  }
  
  this_t& operator *=(const T& rhs){
    return (*this = *this * rhs);
  }
  
  //Overloaded operator for matrix multiplication
  //Dimensions have to match or else this would not
  //work
  this_t operator *(const this_t& rhs){
    this_t product;
    // Fill me in!

    //Hopefully working version
    if(w == rhs.width() && h == rhs.height()){
      for(int _row = 0; _row < h; _row++){
	for(int _col = 0; _col < w; _col++){
	  product(_col, _row) = dot(row(_row), rhs.cols[_col]);
	}
      }
    }
    else{
      throw("Matrix dimensions do not match");
    }

    return product;
  }
  
  
  this_t& operator *=(const this_t& rhs){
    return (*this = *this * rhs);
  }
  
  //Scalar matrix division
  this_t operator /(const T& rhs) const{
    this_t quotient;
    // Fill me in!

    //Hopefully working version
    if(rhs != T(0)){
      for(int i = 0; i < w; i++){
	quotient.cols[i] = cols[i] / rhs;
      }
    }
    else{
      throw("Cannot divide by zero.");
    }

    return quotient;
  }
  
  this_t& operator /=(const T& rhs){
    return (*this = *this / rhs);
  }
  
  //Overloaded boolean equals operator
  //Checks if two matrices are equal or not equal
  bool operator ==(const this_t& rhs) const{
    bool rv = false;
    // Fill me in!

    //Hopefully working version
    if(w == rhs.width() && h == rhs.height()){
      for(int i = 0; i < w; i++){
	rv = cols[i] == rhs.cols[i];
      }
    }
    else{
      rv = false;
    }
    return rv;
  }
  
  bool operator !=(const this_t& rhs) const{
    return( ! (*this == rhs));
  }
  
  const T operator ()(const size_t& col_i, const size_t& row_j) const{
    return cols[col_i][row_j];
  }
  
  T& operator ()(const size_t& col_i, const size_t& row_j){
    return cols[col_i][row_j];
  }
  
  vec_t& column(const size_t& col_i){
    if( col_i < w ){
      return cols[col_i];
    }else{
      throw( "Index out of range" );
    }
  }
  
  vec_t& operator [](const size_t& col_i){
    return column(col_i);
  }
  
  const vec_t& column(const size_t& col_i) const{
    if( col_i < w ){
      return cols[col_i];
    }else{
      throw( "Index out of range" );
    }
  }
  
  const vec_t& operator[](const size_t& col_i) const{
    return column(col_i);
  }
    
  VecN<T, w> row(const size_t& row_j) const{
    T vals[w];
    for(int i = 0; i < w; i++){
      vals[i] = cols[i][row_j];
    }
    return VecN<T, h>(vals);
  }
  
  operator T*( ){
    return &cols[0][0];
  }
  
  operator const T*( ) const{
    return &cols[0][0];
  }
    
  //Method to get the Transpose of a matrix
  MatNM<T, h, w> transpose( ) const{
    MatNM<T, h, w> rv;
    // Fill me in!

    //Hopefully working version
    for(int i = 0; i < w; i++){
      for(int j = 0; j < w; j++){
	rv(j, i) = T(cols[i][j]);
      }
    }

    return rv;
  }
    
  static int width(void){
    return w;
  }
  
  static int height(void){
    return h;
  }
  
  std::ostream& write(std::ostream& out) const{
    out.setf( std::ios::fixed );
    out << std::setprecision(IOS_FP_PRECISION);
    for(int i = 0; i < h; i++){
      out << row(i); 
    }
    out.unsetf( std::ios::fixed );
    return(out);
  }
  
  std::ostream& description(std::ostream &out = std::cerr) const{
    out << "<Mat" << w << "x" << h << " " << this << ">" << std::endl;
    for( int i = 0; i < h; i++ ){
      out << row(i) << std::endl; 
    }
    return(out);
  }
protected:
  VecN<T, h> cols[w];
  
  void assign(const MatNM& rhs){
    for(int i = 0; i < w; i++){
      cols[i] = rhs.cols[i]; 
    }
  }
  
  void assign(const T s){
    for(int i = 0; i < w; i++){
      cols[i] = vec_t(s); 
    }
  }
  
  void assign(const vec_t& v){
    for(int i = 0; i < w; i++){
      cols[i] = v; 
    }
  }
  
  void assign(const T* a){
    for(int i = 0; i < w; i++){
      cols[i] = vec_t(a+(i*h)); 
    }
  }  
}; // end class MatNM

template <typename T, const int w>
class MatN : public MatNM<T, w, w>{
public:
  typedef MatNM<T, w, w> base;
  typedef MatN<T, w> this_t;
  
  MatN( ) : base( ) {}
  MatN(const base& m) : base(m) {}
  MatN(const T* array) : base(array) {}
  MatN(const VecN<T, w>& v) : base(v) {}
  
  void identity( ){
    // Fill me in!
    
    //Hopefully working version
    for(int i = 0; i < w; i++){
      for(int j = 0; j < w; j++){
	if(j == i){
	  (*this)(i, j) = T(1); 
	}
	else{
	  (*this)(i, j) = T(0);
	}
      }
    }
  }
    
}; // end class MatN

template <typename T>
class TMat2 : public MatN<T, 2>{
public:
  typedef MatN<T, 2> base;
  typedef TMat2<T> this_t;
  TMat2( ) : base( ) {}
  TMat2(const this_t& m) : base(m) {}
  TMat2(const base& m) : base(m) {}
  TMat2(const MatNM<T, 2, 2>& m) : base(m) {}
  TMat2(const T* array) : base(array) {}
  TMat2(const VecN<T, 2>& v) : base(v) {}
  TMat2(const VecN<T, 2>& a, const VecN<T, 2>& b){
    base::cols[0] = a;
    base::cols[1] = b;
  }
  TMat2(const T& a, const T& b, const T& c, const T& d){
    base::cols[0] = TVec2<T>(a, b);
    base::cols[1] = TVec2<T>(c, d);
  }

  //Implementation to get the determinant of a 2x2 matrix
  T determinant( ){
    // Fill me in!

    //Hopefully working version
    return T(
	     (*this)[0][0]*(*this)[1][1] - (*this)[0][1] * (*this)[1][0]
	     );
  }
  
  //Implementation to get the minor of a 2x2 matrix
  //Minor is specified by ith column and jth row
  T minor(unsigned int col_i, unsigned int row_j){
    // Fill me in!
    
    //Hopefully working version

    //The minor is the determinant of the sub-matrix
    //crossed out by the column and row specified in
    //its parameters. However, since this is a 2x2 
    //Matrix, the determinant is also the sub-matrix.
    
    //Due the simplistic nature of a 2x2 matrix, 
    //simply re-assigning both column and row to 1 when
    //both are zero works.
    if(col_i == 0 && row_j == 0){
      col_i = 1;
      row_j = 1;
    }
    
    //...and vice versa.
    else if(col_i == 1 && row_j == 1){
      col_i = 0;
      row_j = 0;
    }

    return T((*this)[row_j][col_i]);
  }
  
  //Implementation to get the cofactors of a 2x2 matrix
  //Cofactors are determined by the ith column and jth row
  T cofactor(unsigned int col_i, unsigned int row_j){
    // Fill me in!
    
    //Hopefully working version
    return minor(col_i, row_j) * _pow(T(-1), (row_j) + (col_i));
  }
  
  //Implementation to get the adguate matrix 
  this_t adjugate( ){
    this_t m;
    // Fill me in!

    //Hopefully working version
    for(int i = 0; i < (*this).width(); i++){
      for(int j = 0; j < (*this).width(); j++){
	m(j, i) = cofactor(i, j);
      }
    }

    return m;
  }
  
  //Implementation to calculate the invers of a 2x2 matrix
  //using determinant and the adjugate matrix
  this_t inverse( ){
    this_t m;
    // Fill me in!

    //Hopefully working version
    
    //Check if Det(A) is not zero
    assert(determinant() > T(0));
    
    m = T(1)/determinant() * adjugate();

    return m;
  }
  
}; // end class TMat2


template <typename T>
class TMat3 : public MatN<T, 3>{
public:
  typedef MatN<T, 3> base;
  typedef TMat3<T> this_t;
  TMat3( ) : base( ) {}
  TMat3(const this_t& m) : base(m) {}
  TMat3(const base& m) : base(m) {}
  TMat3(const MatNM<T, 3, 3>& m) : base(m) {}
  TMat3(const T* array) : base(array) {}
  TMat3(const MatN<T, 2> m){
    base::cols[0] = m[0];
    base::cols[1] = m[1];
    base::cols[2] = base::vec_t(0, 0, 0);
  }
  TMat3(const VecN<T, 3>& v) : base(v) {}
  TMat3(const VecN<T, 3>& a, const VecN<T, 3>& b, const VecN<T, 3>& c){
    base::cols[0] = a;
    base::cols[1] = b;
    base::cols[2] = c;
  }
  TMat3(const T& a, const T& b, const T& c, const T& d, const T& e, const T& f, const T& g, const T& h, const T& i){
    base::cols[0] = TVec3<T>(a, b, c);
    base::cols[1] = TVec3<T>(d, e, f);
    base::cols[2] = TVec3<T>(g, h, i);
  }
  
  //Method to calculate the determinant of a 3x3 matrix
  T determinant( ){
    // Fill me in!

    //Hopefully working version
    
    //The determinant of a 3x3 matrix is actually the crossed out row and columns, multiplied by
    //by the determinants of their respective submatrices, as well as their cofactors
    return(
	   T((*this)[0][0] * (((*this)[1][1] * (*this)[2][2]) - ((*this)[1][2] * (*this)[2][1])) -
	   (*this)[1][0] * (((*this)[0][1] * (*this)[2][2]) - ((*this)[0][2] * (*this)[2][1])) + 
	   (*this)[2][0] * (((*this)[0][1] * (*this)[1][2]) - ((*this)[0][2] * (*this)[1][1])))
	   );
  }
  
  //This method calculates this matrix's minor based on the column and row
  //in its parameters. The minor is the determinant of the sub-matrix crossed
  //out by the column and row specified in the method's parameters
  T minor(unsigned int col_i, unsigned int row_j){
    // Fill me in!

     //Hopefully working version
   
    //Check if this matrix is actually a square matrix
    //before we do the funky stuff
    // assert(col_i != row);
   
    //Declaring a local variable 2x2 matrix to temporarily
    //hold the sub-matrix
    TMat2<T> m;
    
    int cc = 0; //Column counter
    int rc = 0; //Row counter
    
    for(int i = 0; i < (*this).width(); i++){
    
      //Checks if the column, pointed by i, is not equal to the specifed column to
      //cross out. If true, we will then look at that column's members and start
      //inserting them to the sub-matrix
      if(i != col_i){

	for(int j = 0; j < (*this).width(); j++){
	  
	  //Checks if the row, pointed by j, is not equal to the specified
	  //row to cross out. Don't want any shenanigans here
	  if(j != row_j){
	    
	    //Inserting the matrix's member pointed by i & j into the sub-matrix
	    m(rc, cc) = T((*this)[i][j]);
	    
	    //Increment the row counter to jump to the next member in the column
	    rc++;
	  }
	    
	}
	
	//Reset the row counter to zero, we are going to another column
	rc = 0;
	
	//The column counter is incremented before the loop iterates
	cc++;
      }

    }
    
    //The minor is actually the determinant of the submatrix crossed out by
    //the column and row. 
    return T(m.determinant());
  }
  
  //Method to calculate this 3x3 matrix's cofactor specified by the
  //column and row
  T cofactor(unsigned int col_i, unsigned int row_j){
    // Fill me in!
    
     //Hopefully working version
    return minor(col_i, row_j) * _pow(T(-1), (row_j) + (col_i));
  }
  
  //Method to calculate the adjoint of this 3x3 matrix
  this_t adjugate( ){
    this_t m;
    // Fill me in!

    //Hopefully working version
    for(int i = 0; i < (*this).width(); i++){
      for(int j = 0; j < (*this).width(); j++){
	
	//The adjoint is the transpose matrix of the this matrix's cofactors 
	m(j, i) = cofactor(i, j);
      }
    }

    return m;
  }
  
  //Method to calculate the inverse of a 3x3 matrix using
  //determinants and adjoint
  this_t inverse( ){
    this_t m;
    // Fill me in!

    //Hopefully working version
    
    //Checking if the determinant is nonzero
    //Spooky stuff will happen otherwise
    assert(determinant() > T(0));
    
    m = T(1)/determinant() * adjugate();

    return m;
  }

}; // end class TMat3

template <typename T>
class TMat4 : public MatN<T, 4>{
public:
  typedef MatN<T, 4> base;
  typedef TMat4<T> this_t;
  TMat4( ) : base( ) {}
  TMat4(const this_t& m) : base(m) {}
  TMat4(const base& m) : base(m) {}
  TMat4(const MatNM<T, 4, 4>& m) : base(m) {}
  TMat4(const T* array) : base(array) {}
  TMat4(const MatN<T, 3> m){
    base::cols[0] = m[0];
    base::cols[1] = m[1];
    base::cols[2] = m[2];
    base::cols[3] = base::vec_t(0, 0, 0, 0);
  }
  TMat4(const VecN<T, 4>& v) : base(v) {}
  TMat4(const VecN<T, 4>& a, const VecN<T, 4>& b, const VecN<T, 4>& c, const VecN<T, 4>& d){
    base::cols[0] = a;
    base::cols[1] = b;
    base::cols[2] = c;
    base::cols[3] = d;
  }
  TMat4(const T& a, const T& b, const T& c, const T& d, const T& e, const T& f, const T& g, const T& h, const T& i, const T& j, const T& k, const T& l, const T& m, const T& n, const T& o, const T& p){
    base::cols[0] = TVec4<T>(a, b, c, d);
    base::cols[1] = TVec4<T>(e, f, g, h);
    base::cols[2] = TVec4<T>(i, j, k, l);
    base::cols[3] = TVec4<T>(m, n, o, p);
  }
  
  this_t transpose( ) const{
    return base::transpose( );
  }
  
  //Method to calculate the determinant of a 4x4 matrix
  T determinant( ){
    // Fill me in!

    //Hopefully working version

    //Making 4 3x3 sub-matrices in order to make calculating this 4x4 matrix's determinant
    //less of a chore. 

    //This will be the sub-matrix crossed out by column 0 and row 0
    TMat3<T> m1 = TMat3<T>( T((*this)[1][1]), T((*this)[1][2]), T((*this)[1][3]),
			    T((*this)[2][1]), T((*this)[2][2]), T((*this)[2][3]),
			    T((*this)[3][1]), T((*this)[3][2]), T((*this)[3][3]) );

    //This will be the sub-matrix crossed out by column 1 and row 0
    TMat3<T> m2 = TMat3<T>( T((*this)[0][1]), T((*this)[0][2]), T((*this)[0][3]),
			    T((*this)[2][1]), T((*this)[2][2]), T((*this)[2][3]),
			    T((*this)[3][1]), T((*this)[3][2]), T((*this)[3][3]) );

    //This will be the sub-matrix crossed out by column 2 and row 0
    TMat3<T> m3 = TMat3<T>( T((*this)[0][1]), T((*this)[0][2]), T((*this)[0][3]),
			    T((*this)[1][1]), T((*this)[1][2]), T((*this)[1][3]),
			    T((*this)[3][1]), T((*this)[3][2]), T((*this)[3][3]) );
    
    //This will be the sub-matrix crossed out by column 3 and row 0
    TMat3<T> m4 = TMat3<T>( T((*this)[0][1]), T((*this)[0][2]), T((*this)[0][3]),
			    T((*this)[1][1]), T((*this)[1][2]), T((*this)[1][3]),
			    T((*this)[2][1]), T((*this)[2][2]), T((*this)[2][3]) );

    return(
	   
	   //The determinant of this 4x4 matrix  will then be values crossed out by the column and row,
	   //multiplied by the determinants of their respective submatrices, as well as their cofactors
	   T( (*this)[0][0]) * T(m1.determinant()) - ( T((*this)[1][0]) * T(m2.determinant()) ) + ( T((*this)[2][0]) * T(m3.determinant()) ) -
	    (T((*this)[3][0]) * T(m4.determinant()) )

	   );
  }
  
  //This method calculates this matrix's minor based on the column and row
  //in its parameters
  T minor(unsigned int col_i, unsigned int row_j){
    // Fill me in!

    
    //Hopefully working version
    TMat3<T> m;

    int cc = 0; //Column counter
    int rc = 0; //Row counter
    
    //What will happen here is that the nested for loops will put all the non
    //crossed out members of the 4x4 matrix into a 3x3 matrix.
    for(int i = 0; i < (*this).width(); i++){

      //If the column, pointed by i, doesn't match the column to cross out
      //then do the following:
      if(i != col_i){

	//Will now traverse the column's components, pointing by the row, j
	for(int j = 0; j < (*this).width(); j++){
	  
	  //If the row, pointed by j, doesn't match the row to cross out
	  //then do the following
	  if(j != row_j){

	    //Insert the matrix member in the 4x4 matrix inside the 3x3 matrix
	    m(rc, cc) = (*this)[i][j];
	    
	    //Increment rc
	    rc++;
	  }
	}

	//Reset rc back to zero, so when i goes to the next column, we can
	//access and modify the members in the column
	rc = 0;
	
	//Increment cc, going to the next column
	cc++;
      }
    }

    //The minor is the determinant calculated from the sub-matrix we 
    //just made based on the crossed out column and row
    return T(m.determinant());
  }
  
  //Calculates the cofactor based on the column and row
  T cofactor(unsigned int col_i, unsigned int row_j){
    // Fill me in!

    //Hopefully working version  
    return minor(col_i, row_j) * _pow(T(-1), (row_j) + (col_i));
  }
  
  //This method gets this Matrix's adjoint  
  this_t adjugate( ){
    this_t m;
    // Fill me in!

    //Hopefully working version

    for(int i = 0; i < (*this).width(); i++){
      for(int j = 0; j < (*this).width(); j++){
	
	//The adjugate is the Transpose of the matrix of cofactors
	//so we're flipping i & j and inserting it to the matrix, m
	m(j, i) = cofactor(i, j);
      }
    }

    return m;
  }
  
  //This method calculates the inverse for a 4x4 Matrix using adjoints
  //cofactors
  this_t inverse( ){
    this_t m;
    // Fill me in!

    //Hopefully working version
    
    //Checking if the determinant is non zero
    //Spooky things will happen otherwise
    assert(determinant() > T(0));
    
    //The inverse of a matrix is 1 divided by the determinant, multiplied by its adjoint
    m = T(1)/determinant() * adjugate();

    return m;
  }

private:
  TVec4<T> _cross4(const TVec4<T>& u, const TVec4<T>& v, const TVec4<T>& w){
    T a, b, c, d, e, f;
    a = (v[0] * w[1]) - (v[1] * w[0]);
    b = (v[0] * w[2]) - (v[2] * w[0]);
    c = (v[0] * w[3]) - (v[3] * w[0]);
    d = (v[1] * w[2]) - (v[2] * w[1]);
    e = (v[1] * w[3]) - (v[3] * w[1]);
    f = (v[2] * w[3]) - (v[3] * w[2]);

    TVec4<T> n = TVec4<T>(
      (u[1] * f) - (u[2] * e) + (u[3] * d), 
    - (u[0] * f) + (u[2] * c) - (u[3] * b), 
      (u[0] * e) - (u[1] * c) + (u[3] * a), 
    - (u[0] * d) + (u[1] * b) - (u[2] * a)
    );
    return n;
  }
  
}; // end class TMat4

typedef TMat2<float> Mat2;
typedef TMat2<int> iMat2;
typedef TMat2<unsigned int> uMat2;
typedef TMat2<double> dMat2;

typedef TMat3<float> Mat3;
typedef TMat3<int> iMat3;
typedef TMat3<unsigned int> uMat3;
typedef TMat3<double> dMat3;

typedef TMat4<float> Mat4;
typedef TMat4<int> iMat4;
typedef TMat4<unsigned int> uMat4;
typedef TMat4<double> dMat4;

template <typename T, const int w, const int h>
std::ostream& operator <<( std::ostream &out, const MatNM<T, w, h> &m ){
  return(m.write( out ));
}

//Overloaded '*' operator in order to multiply two matrices
template <typename T, const int w1, const int h1, const int w2, const int h2>
MatNM<T, w1, h2> operator *(const MatNM<T, w1, h1>& lhs, const MatNM<T, w2, h2>& rhs){
  MatNM<T, w1, h2> product;
  // Fill me in!

  //Hopefully working version
  
  //Check if the height of Matrix A matches the width of Matrix B
  if(lhs.height() == rhs.width()){
    for(int row = 0; row < lhs.height(); row++){
      for(int col = 0; col < rhs.width(); col++){
	product(row, col) = dot(lhs.row(row), rhs[col]);
      }
    }
  }
  //Otherwise, throw out a spooky error message
  else{
    throw("Invalid matrix dimensions");
  }
  
  return product;
}

//Overloaded '*' operator for multiplying a matrix and a vector
template <typename T, const int w>
static const VecN<T, w> operator *(const MatNM<T, w, w>& lhs, VecN<T, w> rhs){
  VecN<T, w> product;
  // Fill me in!
  
  //Hopefully working version

  //Check if the height of the matrix object is the same as the size of the vector object
  if(lhs.height() == rhs.size()){
    for(int i = 0; i < lhs.height(); i++){
      product[i] = dot(lhs.row(i), rhs);
    }
  }
  //Otherwise, throw out a spooky error message
  else{
    throw("Invalid dimensions between the matrix object and vector object");
  }
  
  
  return product;
}

template <typename T, const int w, const int h>
static const MatNM<T, w, h> operator *(T lhs, const MatNM<T, w, h>& rhs){
  return rhs * lhs;
}


class ViewPort{
  ViewPort( unsigned int width = 0, unsigned int height = 0 ) : _width(width), _height(height) {}
  unsigned int width( ){
    return _width;
  }
  unsigned int height( ){
    return _height;
  }
protected:
  unsigned int _width;
  unsigned int _height;
};

static Mat4 frustum(float left, float right, float bottom, float top, float near, float far){
  Mat4 m;
  // Fill me in!  
  return m;
}

//Method to set up a pespective projection matrix
//The following URL was referenced in order to implement this function:
//--https://www.opengl.org/sdk/docs/man2/xhtml/gluPerspective.xml
static Mat4 perspective(float fovy_in_Y_direction, float aspect, float near, float far){
  Mat4 m;
  // Fill me in! 
  
  //Hopefully working version

  //Convert fovy to radians, since it's passed in as degrees. And then divide by 2.0 to get 
  //actual value of 'f' as defined in the URL.
  float locFovY = (fovy_in_Y_direction * PI_OVER_ONE_EIGHTY)/2.0;
  float fVar = 1/tan(locFovY);
  
  m = Mat4( fVar/aspect, 0.0, 0.0, 0.0,
	    0.0, fVar, 0.0, 0.0,
	    0.0, 0.0, (far+near)/(near-far), -1.0,
	    0.0, 0.0, 2 * ((far*near)/(near-far)), 0.0 );

  return m;
}

static Mat4 ortho(float left, float right, float bottom, float top, float near, float far){
  Mat4 m;
  // Fill me in!  
  return m;
}

//Rotate function using Rodrigues' rotation formula
//The following URL was referenced to help implementing this function:
//--http://electroncastle.com/wp/?p=39
static Mat4 rotate(float angleInDegrees, float axis_x, float axis_y, float axis_z){
  Mat4 m;
  // Fill me in!  
  
  Mat3 m2;

  //According to the referenced URL, the Rodrigues Rotation Formula is as follows:
  //R = Identity Matrix + Skew-Symmetric Matrix * sine(alpha) + (Skew-Symmetric Matrix)^2 * (1 - cosine(alpha))
  //This is also the same as the notes derived during lecture.

  //Hopefully working version
  float cosAlpha = cos((angleInDegrees * PI_OVER_ONE_EIGHTY));
  float sinAlpha = sin((angleInDegrees * PI_OVER_ONE_EIGHTY));
  
  //Creating the skew-symmetric matrix:
  Mat3 skew_mat = Mat3( 0.0, axis_z, -1.0 * axis_y,
		    -1.0 * axis_z, 0.0, axis_x,
		    axis_y, -1.0 * axis_x, 0.0 );
  
  //Creating the skew_symmetric matrix - squared 
  Mat3 skew_squared = Mat3( (-1.0 * (axis_z * axis_z)) + (-1.0 * (axis_y * axis_y)), axis_x * axis_y, axis_x * axis_z,
			    axis_x * axis_y, (-1.0 * (axis_z * axis_z)) + (-1.0 * (axis_x * axis_x)), axis_y * axis_z,
			    axis_x * axis_z, axis_y * axis_z, (-1.0 * (axis_y * axis_y)) + (-1.0 * (axis_x * axis_x)) );

  //Creating an identity matrix on R3
  Mat3 id_matrix(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);


  //Getting the Rodrigues Rotation Matrix
  Mat3 rodriguez_rot = id_matrix +  (skew_mat * sinAlpha) + (skew_squared * (1 - cosAlpha));
  
  //Let's put everything together! Weee~
  m = Mat4( rodriguez_rot(0,0), rodriguez_rot(0,1), rodriguez_rot(0,2), 0.0,
	    rodriguez_rot(1,0), rodriguez_rot(1,1), rodriguez_rot(1,2), 0.0,
	    rodriguez_rot(2,0), rodriguez_rot(2,1), rodriguez_rot(2,2), 0.0,
	    0.0, 0.0, 0.0, 1.0 );

  return m;
}

static Mat4 rotate(float angleInDegrees, const Vec3& axis){
  return rotate(angleInDegrees, axis[0], axis[1], axis[2]);
}

static Mat4 scale(float s){
  Mat4 m;
  // Fill me in!  
  return m;
}

static Mat4 scale(float x, float y, float z){
  Mat4 m;
  // Fill me in!  
  return m;
}

static Mat4 scale(const Vec3& v){
  float x = v[0];
  float y = v[1];
  float z = v[2];
  return scale(x, y, z);
}

static Mat4 translate(float x, float y, float z){
  Mat4 m;
  // Fill me in! 
  
  //Hopefully working version
  m = Mat4(1.0, 0.0, 0.0, 0.0, 
	   0.0, 1.0, 0.0, 0.0,
	   0.0, 0.0, 1.0, 0.0,
	   x, y, z, 1.0);

  return m;
}

static Mat4 translate(const Vec3& v){
  float x = v[0];
  float y = v[1];
  float z = v[2];
  return translate(x, y, z);
}

//Lookat function for camera use
//The following URL was referenced to help implementing this function:
//--https://www.opengl.org/sdk/docs/man2/xhtml/gluLookAt.xml
static Mat4 lookat(const Vec3& eye, const Vec3& center, const Vec3& up){
  Mat4 m;
  // Fill me in!  

  //Hopefully working version

  //Matrix to translate
  Mat4 trans = Mat4( 1.0, 0.0, 0.0, 0.0,
		     0.0, 1.0, 0.0, 0.0,
		     0.0, 0.0, 1.0, 0.0,
		    -eye[0], -eye[1], -eye[2], 1.0 );
  
  
  //forward vector
  Vec3 f = Vec3(center[0] - eye[0], center[1] - eye[1], center[2] - eye[2]);

  //Normalized forward vector
  Vec3 f_normalized = normalize(f);

  //Normalized up vector
  Vec3 up_normalized = normalize(up);
  
  //Creating vectors u & s
  Vec3 s = cross(f_normalized, up_normalized);
  Vec3 u = cross(s, f_normalized);

  //Creating the viewing matrix
  m = Mat4( s[0], u[0], -f_normalized[0], 0.0,
	    s[1], u[1], -f_normalized[1], 0.0,
	    s[2], u[2], -f_normalized[2], 0.0,
	    0.0, 0.0, 0.0, 1.0 );

  //Last step is to multiply the translation matrix with the our viewing matrix
  return m * trans;
}

static bool project(const Vec3& objCoord, const Mat4& projection, const Mat4& modeling, ViewPort& vp, Vec3* winCoord){
  // Fill me in!
  return false;
}

static bool unproject(const Vec3& winCoord, const Mat4& projection, const Mat4& modeling, ViewPort& vp, Vec3* objCoord){
  // Fill me in!
  return false;
}

static bool unproject4(const Vec3& winCoord, float clipw, const Mat4& projection, const Mat4& modeling, ViewPort& vp, float near, float far, Vec4* objCoord){
  // Fill me in!
  return false;
}

template <typename T>
TVec3<T> reflect(TVec3<T>& direction, TVec3<T>& normal){
  // Fill me in!
  return TVec3<T>(0, 0);
}

template <typename T>
TVec4<T> reflect(TVec4<T>& direction, TVec4<T>& normal){
  // Fill me in!
  return TVec4<T>(0, 0);
}

#endif // End GFXMath.h
