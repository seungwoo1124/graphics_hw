#include "Camera.h"

// TODO: fill up the following function properly 
void Camera::set_rotation(const glm::quat& _q)
{
  glm::mat4 rotMat = glm::mat4_cast(_q);

  right_dir_.x =  rotMat[0][0]; right_dir_.y =  rotMat[0][1]; right_dir_.z =  rotMat[0][2];
  up_dir_.x    =  rotMat[1][0]; up_dir_.y    =  rotMat[1][1]; up_dir_.z    =  rotMat[1][2];
  front_dir_.x = -rotMat[2][0]; front_dir_.y = -rotMat[2][1]; front_dir_.z = -rotMat[2][2];
  
  right_dir_ = glm::normalize(right_dir_);
  up_dir_ = glm::normalize(up_dir_);
  front_dir_ = glm::normalize(front_dir_);
}

// TODO: re-write the following function properly 
const glm::quat Camera::get_rotation() const
{
  glm::vec4 X(right_dir_.x, right_dir_.y, right_dir_.z, 0.0f);
  glm::vec4 Y(up_dir_.x, up_dir_.y, up_dir_.z, 0.0f);
  glm::vec4 Z(-front_dir_.x, -front_dir_.y, -front_dir_.z, 0.0f);
  glm::mat4 rotMat(X, Y, Z, glm::vec4(0, 0, 0, 1.0f));

  glm::quat q = glm::quat_cast(rotMat);

  return q;
}

// TODO: fill up the following function properly 
void Camera::set_pose(const glm::quat& _q, const glm::vec3& _t)
{
  set_rotation(_q);
  position_ = _t;
}

// TODO: fill up the following function properly 
void Camera::get_pose(glm::quat& _q, glm::vec3& _t) const
{
  _q = get_rotation();
  _t = position();
}

// TODO: rewrite the following function properly 
const glm::mat4 Camera::get_pose() const
{
  glm::vec4 X(right_dir_.x, right_dir_.y, right_dir_.z, 0.0f);
  glm::vec4 Y(up_dir_.x, up_dir_.y, up_dir_.z, 0.0f);
  glm::vec4 Z(-front_dir_.x, -front_dir_.y, -front_dir_.z, 0.0f);
  glm::vec4 pos(position_.x, position_.y, position_.z, 1.0f);

  glm::mat4 pose(X, Y, Z, pos);

  return pose;
}

// TODO: fill up the following function properly 
void Camera::set_pose(const glm::mat4& _frame)
{
  // +x_cam: right direction of the camera    (it should be a unit vector whose length is 1)
  // right_dir_ = ..... ; // +x
  right_dir_ = _frame[0];
  right_dir_ = glm::normalize(right_dir_);
  // +y_cam: up direction of the camera       (it should be a unit vector whose length is 1)   
  // up_dir_    = ..... ;    // +y
  up_dir_ = _frame[1];
  up_dir_ = glm::normalize(up_dir_);
  // -z_cam: front direction of the camera    (it should be a unit vector whose length is 1)
  // front_dir_ = ..... ;    // -z
  front_dir_ = -_frame[2];
  front_dir_ = glm::normalize(front_dir_);
  // pos_cam: position of the camera
  // position_  = ..... ;    // pos
  position_ = _frame[3];
}

// TODO: fill up the following function properly 
void Camera::set_pose(const glm::vec3& _pos, const glm::vec3& _at, const glm::vec3& _up_dir)
{
  // up_dir_    = ..... ;
  // front_dir_ = ..... ;    // -z_cam direction
  front_dir_ = glm::normalize(_at - _pos);
  // right_dir_ = ..... ;    // +x_cam direction
  right_dir_ = glm::normalize(glm::cross(front_dir_, _up_dir));
  // up_dir_    = ..... ;    // +y_cam direction
  up_dir_ = glm::normalize(glm::cross(right_dir_, front_dir_));

  // position_  = ..... ;      // pos
  position_ = _pos;
}

// TODO: rewrite the following function properly 
const glm::mat4 Camera::get_view_matrix() const
{
  // glm::mat4 view = glm::lookAt(position_, position_ + front_dir_, up_dir_);
  // return view;
  glm::vec4 X(right_dir_.x, right_dir_.y, right_dir_.z, 0.0f);
  glm::vec4 Y(up_dir_.x, up_dir_.y, up_dir_.z, 0.0f);
  glm::vec4 Z(-front_dir_.x, -front_dir_.y, -front_dir_.z, 0.0f);

  glm::mat4 rot_inv(X, Y, Z, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  rot_inv = glm::transpose(rot_inv);

  glm::mat4 trans_inv(1.0f);
  trans_inv[3] = glm::vec4(-position().x, -position().y, -position().z, 1.0f);

  glm::mat4 V_inv = rot_inv * trans_inv;

  return V_inv;
}

// TODO: rewrite the following function properly 
const glm::mat4 Camera::get_projection_matrix() const
{
  // TODO: Considering the followings,
  //       You must return a proper projection matrix
  //       i) camera mode: it can be either kOrtho or kPerspective
  //       ii) zoom-in/-out: if the camera mode is kOrtho, 
  //                         utilize ortho_scale_ for zoom-in/-out
  //                         if the camera mode is kPerspective,
  //                         utilize fovy_ for zoom-in/-out
  //       iii) aspect ratio: utilize aspect_ in the both camera modes
  //       iv) near/far clipping planes: utilize near_, far_

  glm::mat4 ret(1.0f);

  if (mode_ != kOrtho)
  {
    ret = glm::perspective(glm::radians(fovy()), aspect_, near_, far_);
  }
  else
  {
    ret = glm::ortho(-ortho_scale_ * aspect_, ortho_scale_ * aspect_, -ortho_scale_, ortho_scale_, near_, far_);
  }

  return ret;
}

// TODO: fill up the following functions properly 
void Camera::move_forward(float delta)
{
  //Use set_pose()

  //position_ += delta * front_direction();
  set_pose(get_rotation(), position_ + delta * front_direction());
}

// TODO: fill up the following functions properly 
void Camera::move_backward(float delta)
{
  //Use set_pose() of first version with get_pose()

  //position_ -= delta * front_direction();
  glm::quat q;
  glm::vec3 pos;
  get_pose(q, pos);
  set_pose(q, pos - delta * front_direction());
}

// TODO: fill up the following functions properly 
void Camera::move_left(float delta)
{
  //Use set_pose() of second version with get_pose()

  //position_ -= glm::normalize(glm::cross(front_direction(), up_direction())) * delta;
  glm::mat4 pose = get_pose();

  glm::vec3 newPos = pose[3];
  newPos -= glm::normalize(glm::cross(front_direction(), up_direction())) * delta;

  pose[3].x = newPos.x; pose[3].y = newPos.y; pose[3].z = newPos.z;

  set_pose(pose);
}

// TODO: fill up the following functions properly 
void Camera::move_right(float delta)
{
  // Use set_pose() of third version
  //position_ += glm::normalize(glm::cross(front_direction(), up_direction())) * delta;
  glm::vec3 newPos = position() + glm::normalize(glm::cross(front_direction(), up_direction())) * delta;

  set_pose(newPos, newPos + front_direction(), up_direction());
}

// TODO: fill up the following functions properly 
void Camera::move_up(float delta)
{
  // Naive
  position_ += delta * up_direction();
}

// TODO: fill up the following functions properly 
void Camera::move_down(float delta)
{
  // Naive
  position_ -= delta * up_direction();
}
