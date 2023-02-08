#ifndef POINT_H
#define POINT_H

#include "../../Interfaces/IPosition.h"

namespace Console2D
{
    struct Point : IPosition
    {
    public:
        int x = 0;
        int y = 0;
        void moveBy(Point offset);
    };
}

#endif // POINT_H