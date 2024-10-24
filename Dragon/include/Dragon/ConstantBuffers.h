#pragma once

#include <Dragon\Math.h>

namespace Dragon
{
    struct CBCamera
    {
        Matrix view;
        Matrix projection;
    };
    struct CBObject
    {
        Matrix model;
        Matrix normal;
    };
}
