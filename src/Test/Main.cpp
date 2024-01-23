#include "../Base/Base.h"

void main2()
{
    Vector3 origin = { 3.0f, 0, -4 };
    Vector3 dirNorm = { 0, 0, 1 };
    Sphere sphere = {}; sphere.radius = 3;

    auto hit = RaycastFull1(origin, dirNorm, sphere);

    cout << (hit ? "hit" : "no hit") << endl;
}

int main()
{
    try
    {
        main2();
    }
    catch (const exception& e)
    {
        cout << "EXCEPTION" << endl;
        cerr << e.what() << endl;
    }

    return 0;
}