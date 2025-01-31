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
