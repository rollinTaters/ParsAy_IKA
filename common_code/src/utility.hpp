/*
	MIT License

	Copyright (c) 2025 rollinTaters

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/


/*
   Some utility functions to make life easier

*/

#pragma once

#include <cmath>
#include "SFML/System/Vector3.hpp"

#define PI 3.141592

template<typename T>
float mag( const sf::Vector3<T> v )
{
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

struct Point
{
    float x;
    float y;
    float z;

    Point();
    Point( const float, const float, const float );

    float mag() const;  // returns magnitude of this point (threats it as a vector)
    Point unit() const; // returns unit vector of this point (threats it as a vector)
    float absDist( const Point ) const; // returns absolute distance between this and given
    float heading() const;  // north is +Y, Z (up-down) is ignored, return value is in radians

    // returns squared error of desired seperation between two points
    float sqErrSep( const Point p1, float seperation );

    Point& operator+=( const Point & );
    Point& operator-=( const Point & );
    Point& operator*=( const Point & );
    Point& operator*=( const float & );

};

Point operator+( const Point &, const Point & );
Point operator-( const Point &, const Point & );
Point operator*( const Point &, const float );

class Quaternion
{
  public:
    float w;
    float x;
    float y;
    float z;

    Quaternion() : w(1), x(0), y(0), z(0) {}
    Quaternion( const Quaternion &q ) : w(q.w), x(q.x), y(q.y), z(q.z) {}
    Quaternion( float _x, float _y, float _z ) : w(0), x(_x), y(_y), z(_z) {}
    Quaternion( float _w, float _x, float _y, float _z ) : w(_w), x(_x), y(_y), z(_z) {}

    Quaternion &operator=( const Quaternion &rhs );
    Quaternion &operator*=( const Quaternion &q );
    const Quaternion operator*( const Quaternion &q ) const { return Quaternion(*this) *= q; }

    float dot( const Quaternion &q ) const;
    float norm() const;
    Quaternion &normalize();

    const Quaternion conjugate() const;
    void rotateVector( sf::Vector3f &vec ) const;

    // these create a new quaternion and return it
    static const Quaternion fromEuler( sf::Vector3f euler );
    static const Quaternion fromAxisAngle( sf::Vector3f axis, float radian );
};

// 3 dimensional bounding box geometry
struct BB3D
{
  public:
    BB3D();
    BB3D( const sf::Vector3f pos,
          const Quaternion quat,
          const sf::Vector3f size );

    // these rotate around the local object axes
    void yawLeft( float radian );
    void pitchUp( float radian );
    void rollRight( float radian );

    // these translate on the local object axes
    void translateLocal( const sf::Vector3f );
    void translateFWD( const float meter );
    void translateRight( const float meter );
    void translateUp( const float meter );


    // -- setters --
    void setSize( const sf::Vector3f size );
    void setSize( const float x, const float y, const float z );

    void setPos( const sf::Vector3f pos );
    void setPos( const float x, const float y, const float z );

    void setAng( const sf::Vector3f euler );    // (radian)
    void setAng( const float x, const float y, const float z ); // (radian)

    // -- getters --
    sf::Vector3f getSize() const;

    sf::Vector3f getPos() const;

    sf::Vector3f getAngEuler() const;   // euler angles (in order: z_yaw, x_pitch, y_roll) (radian)
    Quaternion getQuaternion() const;   // returns quaternion describing the rotation of local csys

    sf::Vector3f getLocalVecX() const;
    sf::Vector3f getLocalVecY() const;
    sf::Vector3f getLocalVecZ() const;

  private:
    // unless otherwise specified, all 3d vectors are:
    // +X:starboard, +Y:bow, +Z:above, (or width, len, height) (metre)
    // +X:pitch up, +Y:roll right, +Z:yaw left      (radian)

    // positional variables
    sf::Vector3f m_pos;      // (metre)
    Quaternion m_quat;       // a quaternion describing our rotation relative to global

    // size variable
    sf::Vector3f m_size;     // (metre)

    friend BB3D operator +( BB3D left, BB3D right );
    friend BB3D& operator +=( BB3D& left, BB3D right );
};

// adds positions and angles, uses left operands size
BB3D operator +( BB3D left, BB3D right );

// adds positions and angles, uses left operands size
BB3D& operator +=( BB3D& left, BB3D right );
