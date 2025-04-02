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

// ---- Point ----
Point::Point():x(0), y(0), z(0) {}
Point::Point( const float in_x, const float in_y, const float in_z ):
    x(in_x), y(in_y), z(in_z) {}

float Point::mag() const
{
    return sqrt( x*x + y*y + z*z );
}

Point Point::unit() const
{
    float m = mag();
    return Point( x/m, y/m, z/m );
}

float Point::absDist( const Point inp ) const
{
    Point delta = *this - inp;
    return delta.mag();
}

float Point::heading() const
{
    // start by making a unit vector
    Point unit_vec(x,y,0);
    unit_vec = unit_vec.unit();

    float rad = 0;

    // this whole thing is a shit-show.
    // avert your eyes for your own good.
    // you have been warned.

    // on axis (and possible divide by zero) checks
    if( unit_vec.x == 0 )
    {
        if( unit_vec.y > 0 )
            return 0;   // north 000
        else
            return PI;  // south 180
    }
    if( unit_vec.y == 0 )
    {
        if( unit_vec. x > 0 )
            return PI/2.f;  // east 090
        else
            return (3.f/2.f) * PI; // west 270
    }
    // an edge case where the input vector had only z component (which we discarded)
    // or no components at all
    if( unit_vec.x == 0 && unit_vec.y == 0 ) return 0;

    // use arcsin when closer to x axis
    bool use_arcsin = false;
    if( abs(unit_vec.x) > abs(unit_vec.y) )
        use_arcsin = true;

    // here be demons, turn back
    if( unit_vec.y > 0 )
    {
        if( unit_vec.x > 0 )
        {
            // sector I
            if( use_arcsin )
                rad = asin( unit_vec.y );
            else
                rad = (PI/2.f) - acos( unit_vec.x );
            rad = PI/2.f - rad;
        }else{
            // sector II
            if( use_arcsin )
                rad = asin( unit_vec.y );
            else
                rad = (PI/2.f) - acos( -unit_vec.x );
            rad = (3.f*PI)/2.f + rad;
        }
    }else{
        if( unit_vec.x > 0 )
        {
            // sector IV
            if( use_arcsin )
                rad = asin( -unit_vec.y );
            else
                rad = (PI/2.f) - acos( unit_vec.x );
            rad = PI/2.f + rad;
        }else{
            // sector III
            if( use_arcsin )
                rad = asin( -unit_vec.y );
            else
                rad = (PI/2.f) - acos( -unit_vec.x );
            rad = PI + rad;
        }
    }
    // fucking finally
    return rad;
}

float Point::sqErrSep( const Point p1, float seperation )
{
    Point delta = *this - p1;
    float current_sep = abs( delta.mag() );
    return ( (seperation-current_sep) * (seperation-current_sep) );
}

Point& Point::operator+=( const Point &rhs )
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

Point& Point::operator-=( const Point &rhs )
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

Point& Point::operator*=( const Point &rhs )
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
}

Point& Point::operator*=( const float & f )
{
    x*=f;
    y*=f;
    z*=f;
    return *this;
}

Point operator+( const Point &p1, const Point &p2 )
{
    return Point( p1.x+p2.x, p1.y+p2.y, p1.z+p2.z );
}

Point operator-( const Point &p1, const Point &p2 )
{
    return Point( p1.x-p2.x, p1.y-p2.y, p1.z-p2.z );
}

Point operator*( const Point &p1, const float f )
{
    return Point( p1.x*f, p1.y*f, p1.z*f );
}

// ---- End of Point ----

Quaternion &Quaternion::operator=(const Quaternion &q)
{
	w = q.w;
	x = q.x;
	y = q.y;
	z = q.z;
	return *this;
}

Quaternion &Quaternion::operator*=(const Quaternion &q)
{
	float w1 = w;
	float x1 = x;
	float y1 = y;
	float z1 = z;

	float w2 = q.w;
	float x2 = q.x;
	float y2 = q.y;
	float z2 = q.z;

	w = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
	x = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2;
	y = w1 * y2 + y1 * w2 + z1 * x2 - x1 * z2;
	z = w1 * z2 + z1 * w2 + x1 * y2 - y1 * x2;
	return *this;
}

float Quaternion::dot(const Quaternion &q) const
{
	return w * q.w + x * q.x + y * q.y + z * q.z;
}

float Quaternion::norm() const
{
	return sqrtf(w * w + x * x + y * y + z * z);
}

Quaternion &Quaternion::normalize()
{
	float iLen = 1 / norm();
	w *= iLen;
	x *= iLen;
	y *= iLen;
	z *= iLen;
	return *this;
}

const Quaternion Quaternion::conjugate() const
{
	return Quaternion(w, -x, -y, -z);
}

void Quaternion::rotateVector( sf::Vector3f &vec ) const
{
	// t = 2q x v
	float tx = 2. * (y * vec.z - z * vec.y);
	float ty = 2. * (z * vec.x - x * vec.z);
	float tz = 2. * (x * vec.y - y * vec.x);

	// v + w t + q x t
	vec.x = vec.x + w * tx + y * tz - z * ty;
	vec.y = vec.y + w * ty + z * tx - x * tz;
	vec.z = vec.z + w * tz + x * ty - y * tx;
}

const Quaternion Quaternion::fromEuler( sf::Vector3f euler ) 
{
	float x = euler.x * 0.5;
	float y = euler.y * 0.5;
	float z = euler.z * 0.5;

	float cX = cosf(x);
	float cY = cosf(y);
	float cZ = cosf(z);

	float sX = sinf(x);
	float sY = sinf(y);
	float sZ = sinf(z);

    // YXZ
	return Quaternion(
		sX * sY * sZ + cX * cY * cZ,
		sX * sZ * cY + sY * cX * cZ,
		sX * cY * cZ - sY * sZ * cX,
		sZ * cX * cY - sX * sY * cZ);
}

const Quaternion Quaternion::fromAxisAngle( sf::Vector3f axis, float radian ) 
{
	Quaternion ret;

	float halfAngle = radian * 0.5;

	float sin_2 = sinf(halfAngle);
	float cos_2 = cosf(halfAngle);

	float sin_norm = sin_2 / sqrtf( axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);

	ret.w = cos_2;
	ret.x = axis.x * sin_norm;
	ret.y = axis.y * sin_norm;
	ret.z = axis.z * sin_norm;

	return ret;
}


BB3D::BB3D()
{
    m_pos = sf::Vector3f(0,0,0);
    m_size = sf::Vector3f(1,1,1);
}

BB3D::BB3D( const sf::Vector3f pos,
            const Quaternion quat,
            const sf::Vector3f size ):
    m_pos(pos),m_quat(quat),m_size(size)
{
}

void BB3D::yawLeft( float radian )
{
    sf::Vector3f axis = getLocalVecZ();
    // create a quat that rotates us in local csys
    Quaternion q = Quaternion::fromAxisAngle( axis, radian );

    // apply quat to our member quat
    m_quat *= q;
}

void BB3D::pitchUp( float radian )
{
    sf::Vector3f axis = getLocalVecX();
    // create a quat that rotates us in local csys
    Quaternion q = Quaternion::fromAxisAngle( axis, radian );

    // apply quat to our member quat
    m_quat *= q;
}

void BB3D::rollRight( float radian )
{
    sf::Vector3f axis = getLocalVecY();
    // create a quat that rotates us in local csys
    Quaternion q = Quaternion::fromAxisAngle( axis, radian );

    // apply quat to our member quat
    m_quat *= q;
}

void BB3D::translateLocal( const sf::Vector3f t )
{
    m_pos += getLocalVecX() * t.x +
             getLocalVecY() * t.y +
             getLocalVecZ() * t.z;
}
void BB3D::translateFWD( const float meter )   { translateLocal(sf::Vector3f(meter,0,0)); }
void BB3D::translateRight( const float meter ) { translateLocal(sf::Vector3f(0,meter,0)); }
void BB3D::translateUp( const float meter )    { translateLocal(sf::Vector3f(0,0,meter)); }

// -- Setters --
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

void BB3D::setAng( const sf::Vector3f euler )
{
    m_quat = Quaternion::fromEuler( euler );
}

void BB3D::setAng( const float x, const float y, const float z )
{
    setAng( sf::Vector3f(x,y,z) );
}

// -- Getters --
sf::Vector3f BB3D::getSize() const { return m_size; }

sf::Vector3f BB3D::getPos() const { return m_pos; }

sf::Vector3f BB3D::getAngEuler() const
{
    // conversion yanked from, then modified:
    // https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/indexLocal.htm

    // TODO axis naming convention must be checked
    Quaternion q1 = m_quat;
	double sqw = q1.w*q1.w;
    double sqx = q1.x*q1.x;
    double sqy = q1.y*q1.y;
    double sqz = q1.z*q1.z;
    float heading = std::atan2(2.0 * (q1.x*q1.y + q1.z*q1.w),(sqx - sqy - sqz + sqw));
    float bank = std::atan2(2.0 * (q1.y*q1.z + q1.x*q1.w),(-sqx - sqy + sqz + sqw));
    float attitude = std::asin(-2.0 * (q1.x*q1.z - q1.y*q1.w));

    return sf::Vector3f( attitude, bank, heading );
}

Quaternion BB3D::getQuaternion() const { return m_quat; }

sf::Vector3f BB3D::getLocalVecX() const
{
    sf::Vector3f ret(1,0,0);
    m_quat.rotateVector( ret );
    return ret;
}

sf::Vector3f BB3D::getLocalVecY() const 
{
    sf::Vector3f ret(0,1,0);
    m_quat.rotateVector( ret );
    return ret;
}

sf::Vector3f BB3D::getLocalVecZ() const
{
    sf::Vector3f ret(0,0,1);
    m_quat.rotateVector( ret );
    return ret;
}


// adds positions and angles, uses left operands size
BB3D operator +( BB3D left, BB3D right )
{
    BB3D ret = left;
    ret.m_pos += right.m_pos;
    ret.m_quat *= right.m_quat;

    return ret;
}

// adds positions and angles, uses left operands size
BB3D& operator +=( BB3D& left, BB3D right )
{
    BB3D ret = left;
    ret.m_pos += right.m_pos;
    ret.m_quat *= right.m_quat;

    left = ret;
    return left;
}
