#ifndef POINT_3D_H
#define POINT_3D_H

#include "../../../Interfaces/IPosition.h"

namespace Win3D
{
	struct Point3D : public IPosition
	{
	public:
		int x;
		int y;
		int z;
		Point3D();
		Point3D(int x, int y, int z);
		Point3D(const Point3D &other);
		void moveBy(const Point3D &offset);
		Point3D operator + (const Point3D& other);
	};
}

#endif
