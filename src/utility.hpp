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

// 3 dimensional bounding box geometry
struct BB3D
{
  public:
    BB3D();
    BB3D( const sf::Vector3f pos,
          const sf::Vector3f angles,
          const sf::Vector3f size );

    void setSize( const sf::Vector3f size );
    void setSize( const float x, const float y, const float z );

    void setPos( const sf::Vector3f pos );
    void setPos( const float x, const float y, const float z );

    void setAng( const sf::Vector3f ang );
    void setAng( const float x, const float y, const float z );

  private:
    // unless otherwise specified, all 3d vectors are:
    // +X:starboard, +Y:bow, +Z:above, (or width, len, height) (metre)
    // +X:pitch up, +Y:roll right, +Z:yaw left      (degree)

    // positional variables
    sf::Vector3f m_pos;      // (metre)
    sf::Vector3f m_angles;   // (degree)

    // size variable
    sf::Vector3f m_size;     // (metre)

    friend BB3D operator +( BB3D left, BB3D right );
    friend BB3D& operator +=( BB3D& left, BB3D right );
};

// adds positions and angles, uses left operands size
BB3D operator +( BB3D left, BB3D right );

// adds positions and angles, uses left operands size
BB3D& operator +=( BB3D& left, BB3D right );
