#include <reactphysics3d/reactphysics3d.h>

namespace rp3d = reactphysics3d;

Vector3 ToEuler(rp3d::Quaternion rot)
{
    auto quat = Vector4 { rot.x, rot.y, rot.z, rot.w };
    auto euler = ToEuler(quat);
    return Vector3 { -euler.x, -euler.y, -euler.z };
}
rp3d::Quaternion FromEuler(Vector3 euler)
{
    euler.x = -euler.x;
    euler.y = -euler.y;
    euler.z = -euler.z;

    auto quat = ToQuaternion(euler);
    auto quat2 = rp3d::Quaternion { quat.x, quat.y, quat.z, quat.w };

    return quat2;

    // auto quat = rp3d::Quaternion::fromEulerAngles(-euler.x, -euler.y, -euler.z);
    // return rp3d::Quaternion { quat.x, quat.y, quat.z, quat.w };
}

Vector3 Rp3dGetPosition(const rp3d::RigidBody& rb)
{
    auto transform = rb.getTransform();
    auto position = transform.getPosition();
    return { position.x, position.y, position.z };
}
Vector3 Rp3dGetRotation(const rp3d::RigidBody& rb)
{
    auto transform = rb.getTransform();
    auto quaternion = transform.getOrientation();
    auto rotation = ToEuler(quaternion);
    return rotation;
}
Pose Rp3dGetPose(const rp3d::RigidBody& rb)
{
    Pose pose;
    pose.position = Rp3dGetPosition(rb);
    pose.rotation = Rp3dGetRotation(rb);
    return pose;
}
