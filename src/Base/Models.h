#pragma once

// float h = 0.5f;

namespace Models
{
    namespace Cube
    {
        Vector3 vertices[] =
        {
            Vector3{ -0.5f, -0.5f, -0.5f },
            Vector3{ -0.5f, -0.5f,  0.5f },
            Vector3{ -0.5f,  0.5f, -0.5f },
            Vector3{ -0.5f,  0.5f,  0.5f },
            Vector3{  0.5f, -0.5f, -0.5f },
            Vector3{  0.5f, -0.5f,  0.5f },
            Vector3{  0.5f,  0.5f, -0.5f },
            Vector3{  0.5f,  0.5f,  0.5f }
        };

        // int indicesLines[12][2] =
        // {
        //     0,1,
        //     1,5,
        //     5,4,
        //     4,0,
        //     2,3,
        //     3,7,
        //     7,6,
        //     6,2,
        //     0,2,
        //     1,3,
        //     5,7,
        //     4,6
        // };
    }
}




