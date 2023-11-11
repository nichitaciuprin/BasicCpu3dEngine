// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html

#pragma once

struct AABB
{
    Vector3 p0, p1;
};
struct Sphere
{
    Vector3 position;
    float radius;
};
float AABBMinX(const AABB* aabb)
{
    return MathMin(aabb->p0.x,aabb->p1.x);
}
float AABBMinY(const AABB* aabb)
{
    return MathMin(aabb->p0.y,aabb->p1.y);
}
float AABBMinZ(const AABB* aabb)
{
    return MathMin(aabb->p0.z,aabb->p1.z);
}
float AABBMaxX(const AABB* aabb)
{
    return MathMax(aabb->p0.x,aabb->p1.x);
}
float AABBMaxY(const AABB* aabb)
{
    return MathMax(aabb->p0.y,aabb->p1.y);
}
float AABBMaxZ(const AABB* aabb)
{
    return MathMax(aabb->p0.z,aabb->p1.z);
}
Vector3 AABBSize(const AABB* aabb)
{
    Vector3 result = aabb->p0-aabb->p1;
    result.x = MathAbs(result.x);
    result.y = MathAbs(result.y);
    result.z = MathAbs(result.z);
    return result;
}
Vector3 AABBWrapAround(const AABB* aabb, Vector3 point)
{
    auto size = AABBSize(aabb);
    if      (point.x < AABBMinX(aabb)) point.x += size.x;
    else if (point.x > AABBMaxX(aabb)) point.x -= size.x;
    if      (point.y < AABBMinY(aabb)) point.y += size.y;
    else if (point.y > AABBMaxY(aabb)) point.y -= size.y;
    if      (point.z < AABBMinZ(aabb)) point.z += size.z;
    else if (point.z > AABBMaxZ(aabb)) point.z -= size.z;
    return point;
}
Vector3 AABBShortPathIn(const AABB* aabb, Vector3 point)
{
    auto result = Vector3Zero();
    if      (point.x < AABBMinX(aabb)) result.x = AABBMinX(aabb) - point.x;
    else if (point.x > AABBMaxX(aabb)) result.x = AABBMaxX(aabb) - point.x;
    if      (point.y < AABBMinY(aabb)) result.y = AABBMinY(aabb) - point.y;
    else if (point.y > AABBMaxY(aabb)) result.y = AABBMaxY(aabb) - point.y;
    if      (point.z < AABBMinZ(aabb)) result.z = AABBMinZ(aabb) - point.z;
    else if (point.z > AABBMaxZ(aabb)) result.z = AABBMaxZ(aabb) - point.z;
    return result;
}
bool InsideSphere(Vector3 point, Sphere sphere)
{
    auto diff = point - sphere.position;
    return Vector3LengthSquared(diff) <= sphere.radius * sphere.radius;
}
bool RaycastFull1(Vector3 origin, Vector3 dirNorm, Sphere sphere)
{
    // TODO must be tested

    auto v1 = sphere.position - origin;
    auto v2Length = Vector3Dot(dirNorm, v1);
    auto v2 = dirNorm * v2Length;
    auto v3 = v2 - v1;
    auto v3LengthSquared = Vector3LengthSquared(v3);
    auto radiusSquared = sphere.radius * sphere.radius;

    // no intersection
    if (v3LengthSquared > radiusSquared) return false;

    auto offset = MathSqrt(radiusSquared - v3LengthSquared);

    auto dist1 = v2Length - offset;
    auto dist2 = v2Length + offset;

    auto point1 = origin + dirNorm * dist1;
    auto point2 = origin + dirNorm * dist2;

    auto normal1 = point1 - sphere.position;
    auto normal2 = point2 - sphere.position;

    normal1 = Vector3Normalize(normal1);
    normal2 = Vector3Normalize(normal2);

    return true;
}
bool RaycastFull2(Vector3 origin, Vector3 dirNorm, Sphere sphere)
{
    // TODO must be tested

    auto diff = origin - sphere.position;
    auto b = Vector3Dot(dirNorm, diff) * 2;
    auto c = Vector3LengthSquared(diff) - (sphere.radius * sphere.radius);
    auto deltaSquared = b * b - 4 * c;

    // no intersection
    if (deltaSquared < 0) return false;

    auto delta = MathSqrt(deltaSquared);

    auto dist1 = (-b - delta) / 2;
    auto dist2 = (-b + delta) / 2;

    auto point1 = origin + dirNorm * dist1;
    auto point2 = origin + dirNorm * dist2;

    auto normal1 = point1 - sphere.position;
    auto normal2 = point2 - sphere.position;

    normal1 = Vector3Normalize(normal1);
    normal2 = Vector3Normalize(normal2);

    return true;
}
bool Raycast(Vector3 origin, Vector3 dirNorm, Sphere sphere, float* outDistance, Vector3* outPoint, Vector3* outNormal)
{
    // TODO must be tested
    // Assuming the origin is outside the sphere

    auto v1 = sphere.position - origin;
    auto v2Length = Vector3Dot(dirNorm, v1);

    // sphere is behind
    if (v2Length < 0) return false;

    auto v2 = dirNorm * v2Length;
    auto v3 = v2 - v1;
    auto v3LengthSquared = Vector3LengthSquared(v3);
    auto radiusSquared = sphere.radius * sphere.radius;

    // no intersection
    if (v3LengthSquared > radiusSquared) return false;

    auto offset = MathSqrt(radiusSquared - v3LengthSquared);
    auto dist = v2Length - offset;
    auto point = origin + dirNorm * dist;
    auto normal = point - sphere.position;
    normal = Vector3Normalize(normal);

    *outDistance = dist;
    *outPoint = point;
    *outNormal = normal;

    return true;
}
bool LineSegmentIntersection(Vector3 start, Vector3 end, Sphere sphere)
{
    // TODO must be tested

    auto origin = start;
    auto dir = end - start;
    auto dirNorm = Vector3Normalize(dir);

    auto v1 = sphere.position - origin;
    auto v2Length = Vector3Dot(dirNorm, v1);
    auto v2 = dirNorm * v2Length;
    auto v3 = v2 - v1;
    auto v3LengthSquared = Vector3LengthSquared(v3);
    auto radiusSquared = sphere.radius * sphere.radius;

    // no intersection
    if (v3LengthSquared > radiusSquared) return false;

    auto offset = MathSqrt(radiusSquared - v3LengthSquared);
    auto dist1 = v2Length - offset;
    auto dist2 = v2Length + offset;
    auto maxDist = MathMax(dist1, dist2);

    // all points behind
    if (maxDist < 0) return false;

    // segment is short
    if (Vector3LengthSquared(dir) < maxDist * maxDist) return false;

    return true;
}
