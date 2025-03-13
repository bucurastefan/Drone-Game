#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include <cmath>

namespace cam
{
    class Camera
    {
    public:
        Camera()
        {
            position = glm::vec3(0, 2, 5);
            forward = glm::vec3(0, 0, -1);
            up = glm::vec3(0, 1, 0);
            right = glm::vec3(1, 0, 0);
            distanceToTarget = 2;

            // Default orthographic projection parameters
            left = -50.0f; rightP = 50.0f; bottom = -50.0f; top = 50.0f;
            zNear = 0.1f; zFar = 300.0f;
            useOrthographic = true; // We'll use orthographic for the minimap
        }

        Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            Set(position, center, up);
            // Default projection parameters
            left = -50.0f; rightP = 50.0f; bottom = -50.0f; top = 50.0f;
            zNear = 0.1f; zFar = 300.0f;
            useOrthographic = true;
        }

        ~Camera()
        {
        }

        void Set(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            this->position = position;
            forward = glm::normalize(center - position);
            right = glm::cross(forward, up);
            this->up = glm::cross(right, forward);
        }

        void MoveForward(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }

        void TranslateForward(float distance)
        {
            position += glm::normalize(forward) * distance;
        }

        void TranslateUpward(float distance)
        {
            position += distance * glm::normalize(up);
        }

        void TranslateRight(float distance)
        {
            position += distance * glm::normalize(right);
        }

        void RotateFirstPerson_OX(float angle)
        {
            forward = glm::normalize(glm::rotate(glm::mat4(1.f), angle, right) * glm::vec4(forward, 0));
            up = glm::normalize(glm::cross(right, forward));
        }

        void RotateFirstPerson_OY(float angle)
        {
            forward = glm::normalize(glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 0));
            right = glm::normalize(glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 0));
            up = glm::normalize(glm::cross(right, forward));
        }

        void RotateFirstPerson_OZ(float angle)
        {
            right = glm::normalize(glm::rotate(glm::mat4(1.f), angle, forward) * glm::vec4(right, 0));
            up = glm::normalize(glm::rotate(glm::mat4(1.f), angle, forward) * glm::vec4(up, 0));
            forward = glm::cross(up, right);
        }

        void RotateThirdPerson_OX(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OX(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OY(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OY(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OZ(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-distanceToTarget);
        }

        glm::mat4 GetViewMatrix()
        {
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }

        // ------------------- New methods -------------------

        // Sets camera position
        void SetPosition(const glm::vec3& pos) {
            position = pos;
        }

        // Sets camera rotation from Euler angles (pitch, yaw, roll)
        // angles.x = pitch (rotation around OX)
        // angles.y = yaw   (rotation around OY)
        // angles.z = roll  (rotation around OZ)
        void SetRotation(const glm::vec3& angles) {
            // Start from default orientation
            forward = glm::vec3(0, 0, -1);
            up = glm::vec3(0, 1, 0);
            right = glm::vec3(1, 0, 0);

            // Apply yaw (rotate around Y)
            RotateFirstPerson_OY(angles.y);
            // Apply pitch (rotate around X)
            RotateFirstPerson_OX(angles.x);
            // Apply roll (rotate around Z)
            RotateFirstPerson_OZ(angles.z);
        }

        // Sets both position and rotation
        void SetPositionAndRotation(const glm::vec3& pos, const glm::vec3& angles) {
            SetPosition(pos);
            SetRotation(angles);
        }

        // Projection matrix getter
        glm::mat4 GetProjectionMatrix() const {
            if (useOrthographic) {
                return glm::ortho(left, rightP, bottom, top, zNear, zFar);
            }
            else {
                // If needed, implement perspective fields and return perspective matrix
                // For now, only orthographic is implemented
                return glm::ortho(left, rightP, bottom, top, zNear, zFar);
            }
        }

        // Set orthographic projection parameters
        void SetOrthographic(float left, float right, float bottom, float top, float zNear, float zFar) {
            this->left = left;
            this->rightP = right;
            this->bottom = bottom;
            this->top = top;
            this->zNear = zNear;
            this->zFar = zFar;
            this->useOrthographic = true;
        }

    public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;

    private:
        // Projection parameters for orthographic
        float left, rightP, bottom, top, zNear, zFar;
        bool useOrthographic;
    };
}   // namespace implemented
