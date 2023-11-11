#pragma once

class Boid
{
public:
    Vector3 position;
    Vector3 velocity;

    static void Update(vector<Boid>& boids, const AABB& aabb, float deltaTime)
    {
        auto length = boids.size();

        for (size_t i = 0; i < length; i++)
        {
            Boid& boid = boids[i];
            boid.vec_1 = Vector3Zero();
            boid.vec_2 = Vector3Zero();
            boid.vec_3 = Vector3Zero();
            boid.count_1 = 0;
            boid.count_2 = 0;
        }

        // ALL UNIQUE PAIRS
        for (size_t i = 0;   i < length; i++)
        for (size_t j = i+1; j < length; j++)
        {
            CalculatePair(boids[i],boids[j]);
        }

        for (size_t i = 0; i < length; i++)
        {
            Boid& boid = boids[i];
            auto targetVelocity = boid.TargetVelocity(aabb);
            auto newVelocity = Vector3MoveTowards(boid.velocity,targetVelocity,acc*deltaTime);
            boid.position = Vector3PositionUpdateAdvanced(boid.position,boid.velocity,boid.velocity,deltaTime);
            boid.velocity = newVelocity;
        }
    }

    Boid(const AABB& aabb, Subgen* subugen)
    {
        auto x = subugen->Range(AABBMinX(&aabb),AABBMaxX(&aabb));
        auto y = subugen->Range(AABBMinY(&aabb),AABBMaxY(&aabb));
        auto z = subugen->Range(AABBMinZ(&aabb),AABBMaxZ(&aabb));
        position = Vector3{ x,y,z };

        auto xRand = subugen->FractionSigned();
        auto yRand = subugen->FractionSigned();
        auto zRand = subugen->FractionSigned();
        auto randDirection = Vector3{ xRand,yRand,zRand };
        randDirection = Vector3Normalize(randDirection);
        auto randSpeed = subugen->Range(minSpeed,maxSpeed);
        velocity = randDirection * randSpeed;
    }

private:
    static const float minSpeed;
    static const float maxSpeed;
    static const float range_1;
    static const float range_2;
    static const float range_3;
    static const float rangeSquared_1;
    static const float rangeSquared_2;
    static const float rangeSquared_3;
    static const float power1;
    static const float power2;
    static const float power3;
    static const float acc;

    Vector3 vec_1;
    Vector3 vec_2;
    Vector3 vec_3;
    int count_1;
    int count_2;

    static void CalculatePair(Boid& boid1, Boid& boid2)
    {
        auto diff = boid1.position-boid2.position;
        auto distSquared = diff.x*diff.x + diff.y*diff.y + diff.z*diff.z;
        auto dist = sqrtf(distSquared);

        // COHESION
        if (distSquared >= rangeSquared_1) return;

        boid1.vec_1 = boid1.vec_1+boid2.position; boid1.count_1++;
        boid2.vec_1 = boid2.vec_1+boid1.position; boid2.count_1++;

        // ALIGHMENT
        if (distSquared >= rangeSquared_2) return;

        boid1.vec_2 = boid1.vec_2+boid2.velocity; boid1.count_2++;
        boid2.vec_2 = boid2.vec_2+boid1.velocity; boid2.count_2++;

        // SEPARATION
        if (distSquared >= rangeSquared_3) return;

        auto normFraction = 1.0f/dist;
        auto normDiff = Vector3{ diff.x * normFraction,diff.y * normFraction,diff.z * normFraction };
        auto dist2 = range_3 - dist;
        normDiff = normDiff*dist2;

        boid1.vec_3 = boid1.vec_3+normDiff;
        boid2.vec_3 = boid2.vec_3-normDiff;
    }

    Vector3 TargetVelocity(const AABB& aabb) const
    {
        auto l_vec_1 = vec_1;
        auto l_vec_2 = vec_2;
        auto l_vec_3 = vec_3;

        if (count_1 != 0)
        {
            l_vec_1 /= (float)count_1;
            l_vec_1 -= position;
        }
        if (count_2 != 0)
        {
            l_vec_2 /= (float)count_2;
            l_vec_2 -= velocity;
        }

        l_vec_1 *= power1;
        l_vec_2 *= power2;
        l_vec_3 *= power3;

        auto result = velocity + l_vec_1 + l_vec_2 + l_vec_3;

        result += AABBShortPathIn(&aabb,position);

        result = Vector3ClampLength(result,minSpeed,maxSpeed);
        return result;
    }
};
const float Boid::minSpeed = 9;
const float Boid::maxSpeed = 15;
const float Boid::range_1 = 5;
const float Boid::range_2 = 5;
const float Boid::range_3 = 2;
const float Boid::rangeSquared_1 = range_1*range_1;
const float Boid::rangeSquared_2 = range_2*range_2;
const float Boid::rangeSquared_3 = range_3*range_3;
const float Boid::power1 = 0.01f;
const float Boid::power2 = 0.01f;
const float Boid::power3 = 0.04f;
const float Boid::acc = 10;