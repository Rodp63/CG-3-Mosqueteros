#ifndef CUBITO_CAMERA_H
#define CUBITO_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum class CameraMovement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
 private:
 
 public:
  void updateCameraVectors();
  // camera Attributes
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  // euler Angles
  float Yaw;
  float Pitch;
  // camera options
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
         float yaw = YAW,
         float pitch = PITCH);
  Camera(float, float, float, float , float, float, float, float);
  const glm::mat4 GetViewMatrix();
  const glm::mat4 GetViewAirPlane();
  void ProcessKeyboard(CameraMovement, float);
  void ProcessMouseMovement(float, float, GLboolean);
  void Automatic(float, double);
  void ProcessMouseScroll(float yoffset);
};

// -----------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) 
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
  Position = position;
  WorldUp = up;
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
  Position = glm::vec3(posX, posY, posZ);
  WorldUp = glm::vec3(upX, upY, upZ);
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
}

const glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(Position, Position + Front, Up);
}

const glm::mat4 Camera::GetViewAirPlane() {
  return glm::lookAt(Position, Front, Up);
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime) {
  float velocity = MovementSpeed * deltaTime;
  if (direction == CameraMovement::FORWARD)
    Position += Front * velocity;
  if (direction == CameraMovement::BACKWARD)
    Position -= Front * velocity;
  if (direction == CameraMovement::LEFT)
    Position -= Right * velocity;
  if (direction == CameraMovement::RIGHT)
    Position += Right * velocity;
}

void Camera::Automatic(float distance_f_cube, double time) {

  float cam_x = static_cast<float>(sin(time) * distance_f_cube);
  float cam_y = static_cast<float>(sin(time) * distance_f_cube);
  float cam_z = static_cast<float>(cos(time) * distance_f_cube);

  float target_x = static_cast<float>(sin(time) * (distance_f_cube-1.0f));
  float target_y = static_cast<float>(sin(time) * (distance_f_cube-1.0f));
  float target_z = static_cast<float>(cos(time) * (distance_f_cube-1.0f));

  this->Position = glm::vec3(cam_x, cam_y, cam_z);
  glm::vec3 target = glm::vec3(target_x, target_y, target_z);
  this->Front = target - this->Position;

  // saving last position Pitch and Yaw
  this->Pitch = glm::degrees(asin(this->Front.y));
  this->Yaw = glm::degrees( asin( this->Front.x / (cos(glm::radians(this->Pitch))) ) );
  this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
  this->Up = glm::normalize(glm::cross(this->Right, this->Front));

}

  void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
  xoffset *= this->MouseSensitivity;
  yoffset *= this->MouseSensitivity;

  this->Yaw += xoffset;
  this->Pitch += yoffset;

  if (constrainPitch)  {
    if (this->Pitch > 89.0f)
      this->Pitch = 89.0f;
    if (this->Pitch < -89.0f)
      this->Pitch = -89.0f;
  }

  updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
  Zoom -= (float)yoffset;
  if (Zoom < 1.0f)
    Zoom = 1.0f;
  if (Zoom > 45.0f)
    Zoom = 45.0f;
}

void Camera::updateCameraVectors() {
  glm::vec3 front;
  front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
  front.y = sin(glm::radians(this->Pitch));
  front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
  this->Front = glm::normalize(front);
  this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
  this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}

#endif // CUBITO_CAMERA_H
