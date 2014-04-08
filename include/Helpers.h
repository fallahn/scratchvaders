/*********************************************************************
Matt Marchant 2013 - 2014
http://trederia.blogspot.com

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

//header file containing static utility methods//

#ifndef HELPERS_H_
#define HELPERS_H_

#include <math.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>
#include <sstream>
#include <cassert>
#include <algorithm>

//helper functions
namespace Helpers
{
	namespace Random
	{
		const float Float(float begin, float end);

		const int Int(int begin, int end);
	};
	
	namespace Vectors
	{
		//calculates dot product of 2 vectors
		static float Dot(const sf::Vector2f& lv, const sf::Vector2f& rv)
		{
			return lv.x * rv.x + lv.y * rv.y;
		}		
		
		//Returns a given vector with its length normalized to 1
		static sf::Vector2f Normalize(sf::Vector2f source)
		{
			float length = std::sqrt(Dot(source, source));
			if (length != 0) source /= length;

			return source;
		}

		//Returns angle in degrees of a given vector where 0 is horizontal
		static float GetAngle(const sf::Vector2f& source)
		{
			return atan2(source.y , source.x) * 180.f / 3.14159265f;
		}

		//returns length squared
		static float GetLengthSquared(const sf::Vector2f& source)
		{
			return Dot(source, source);
		}

		//Returns length of a given vector
		static float GetLength(const sf::Vector2f& source)
		{
			return std::sqrt(GetLengthSquared(source));
		}

		//Returns Normalized vector from given angle in degrees where 0 is horizontal
		static sf::Vector2f GetVectorFromAngle(float angle)
		{
			if(angle < 0) angle += 360; //make sure angle is always positive value
			switch((int)angle)
			{
			case 0:
			case 360:
				return sf::Vector2f(1.f, 0.f);
			case 90:
			//case -270:
				return sf::Vector2f(0.f, 1.f);
			case 180:
			//case -180:
				return sf::Vector2f(-1.f, 0.f);
			case 270:
			//case -90:
				return sf::Vector2f(0.f, -1.f);
				break;
			default:
				angle = (float)(angle * (3.14159265 / 180));
				return sf::Vector2f(cos(angle), sin(angle));
			}
		}

		//calculates cross product of 2 vectors
		static float Cross(const sf::Vector2f& lv, const sf::Vector2f& rv)
		{
			return lv.x * rv.y - lv.y * rv.x;
		}
		static float Cross(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c)
		{
			sf::Vector2f BA = a - b;
			sf::Vector2f BC = c - b;
			return (BA.x * BC.y - BA.y * BC.x);
		}

		//project onto dest
		static sf::Vector2f Project(sf::Vector2f source, sf::Vector2f dest)
		{
			return dest * Dot(source, dest);
		}
		//project onto dest with signed magnitude
		static sf::Vector2f Project(sf::Vector2f source, sf::Vector2f dest, float& mag)
		{
			float sourceDotDest = Dot(source, dest);
			mag = sourceDotDest;
			return dest * sourceDotDest;
		}

		//perpendicular vector of given length
		static sf::Vector2f GetPerpendicular(const sf::Vector2f& source, float length = 1.f)
		{			
			return Normalize(sf::Vector2f(source.y, -source.x)) * length + source;
		}

		//reflection of a vector about a Normal
		static sf::Vector2f Reflect(const sf::Vector2f& v, const sf::Vector2f& n)
		{
			return -2 * Dot(v, n) * n + v;
		}
	};

	namespace Math
	{
		static float Clamp(float x, float a, float b)
		{
			return x < a ? a : (x > b ? b : x);
		}

		static float Round(float val)
		{
			return floor(val + 0.5f);
		}

		static float ToDegrees(float rads)
		{
			return rads * 57.2957795f;
		}

		static float ToRadians(float degrees)
		{
			return degrees * 0.0174533f;
		}
	};
};

#endif