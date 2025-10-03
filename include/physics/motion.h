#ifndef MOTION_H
#define MOTION_H

#include <Eigen/Dense>

class Motion {
public:
  Motion()
      : position(Eigen::Vector3f::Zero()), velocity(Eigen::Vector3f::Zero()) {}
  Motion(const Eigen::Vector3f &pos, const Eigen::Vector3f &vel)
      : position(pos), velocity(vel) {}
  Eigen::Vector3f position;
  Eigen::Vector3f velocity;
};

#endif // MOTION_H