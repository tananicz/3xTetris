#include "../headers/Point3D.h"

namespace Win3D
{
	Point3D::Point3D()
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}
	
	Point3D::Point3D(int x, int y, int z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Point3D::Point3D(const Point3D& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
	}

	void Point3D::MoveBy(const Point3D &offset)
	{
		this->x += offset.x;
		this->y += offset.y;
		this->z += offset.z;
	}

	Point3D Point3D::operator + (const Point3D& other)
	{
		Point3D result(*this);
		result.MoveBy(other);
		return result;
	}
}
