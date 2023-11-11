#pragma once

class BoidWorld
{
public:
    AABB aabb;
    vector<Boid> boids;

    BoidWorld()
    {
        Subgen subgen = Subgen(0);
        aabb.p0 = Vector3Backward() + Vector3Left();
        aabb.p1 = Vector3Forward() + Vector3Right();
        auto size = 50.0f;
        aabb.p0 *= size;
        aabb.p1 *= size;
        aabb.p1 += Vector3Up()*size*2;
        for (int i = 0; i < boidCount; i++)
            boids.push_back(Boid(aabb,&subgen));
    }

    void Update(float deltaTime)
    {
        Boid::Update(boids,aabb,deltaTime);
    }

private:
    int boidCount = 400;
};