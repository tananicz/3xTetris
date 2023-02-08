#include "../headers/Point.h"

namespace Console2D
{
    void Point::moveBy(Point offset)
    {
        x += offset.x;
        y += offset.y;
    }
}
