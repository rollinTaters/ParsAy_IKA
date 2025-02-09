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


#include "utility.hpp"

BB3D::BB3D()
{
    m_pos = sf::Vector3f(0,0,0);
    m_angles = sf::Vector3f(0,0,0);
    m_size = sf::Vector3f(1,1,1);
}

BB3D::BB3D( const sf::Vector3f pos,
            const sf::Vector3f angles,
            const sf::Vector3f size ):
    m_pos(pos),m_angles(angles),m_size(size)
{
}

void BB3D::setSize( const sf::Vector3f size )
{
    m_size = size;
}

void BB3D::setSize( const float x, const float y, const float z )
{
    m_size = sf::Vector3f(x,y,z);
}

void BB3D::setPos( const sf::Vector3f pos )
{
    m_pos = pos;
}

void BB3D::setPos( const float x, const float y, const float z )
{
    m_pos = sf::Vector3f(x,y,z);
}

void BB3D::setAng( const sf::Vector3f ang )
{
    m_angles = ang;
}

void BB3D::setAng( const float x, const float y, const float z )
{
    m_angles = sf::Vector3f(x,y,z);
}


// adds positions and angles, uses left operands size
BB3D operator +( BB3D left, BB3D right )
{
    BB3D ret = left;
    ret.m_pos += right.m_pos;

    // FIXME use quaternions for angular stuff
    // im gonna just keep going for now with only yaw angles, and ignore the rest
    ret.m_angles.z += right.m_angles.z;
    if( ret.m_angles.z >= 360.f )
        ret.m_angles.z -= 360.f;
    else if( ret.m_angles.z < 0.f )
        ret.m_angles.z += 360.f;

    return ret;
}

// adds positions and angles, uses left operands size
BB3D& operator +=( BB3D& left, BB3D right )
{
    BB3D ret = left;
    ret.m_pos += right.m_pos;

    // FIXME use quaternions for angular stuff
    // im gonna just keep going for now with only yaw angles, and ignore the rest
    ret.m_angles.z += right.m_angles.z;
    if( ret.m_angles.z >= 360.f )
        ret.m_angles.z -= 360.f;
    else if( ret.m_angles.z < 0.f )
        ret.m_angles.z += 360.f;

    left = ret;
    return left;
}
