/*
 Copyright (C) 2010-2012 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Camera.h"

#include <algorithm>

namespace TrenchBroom {
    namespace Renderer {
        Camera::Camera(float fieldOfVision, float nearPlane, float farPlane, const Vec3f& position, const Vec3f& direction) : m_fieldOfVision(fieldOfVision), m_nearPlane(nearPlane), m_farPlane(farPlane), m_position(position), m_direction(direction) {
            if (m_direction.equals(Vec3f::PosZ)) {
                m_right = Vec3f::NegY;
                m_up = Vec3f::NegX;
            } else if (m_direction.equals(Vec3f::NegZ)) {
                m_right = Vec3f::NegY;
                m_up = Vec3f::PosX;
            } else {
                m_right = m_direction % Vec3f::PosZ;
                m_up = m_right % m_direction;
            }
        }
        
        const Vec3f Camera::defaultPoint() const {
            return defaultPoint(m_direction);
        }
        
        const Vec3f Camera::defaultPoint(const Vec3f& direction) const {
            return m_position + direction * 256.0f;
        }

        const Vec3f Camera::defaultPoint(float x, float y) const {
            const Vec3f point = unproject(x, y, 0.5f);
            return defaultPoint((point - m_position).normalized());
        }

        const Vec3f Camera::project(const Vec3f& point) const {
            GLdouble objX, objY, objZ, winX, winY, winZ;
            objX = static_cast<GLdouble>(point.x);
            objY = static_cast<GLdouble>(point.y);
            objZ = static_cast<GLdouble>(point.z);
            
            gluProject(objX, objY, objZ, m_modelview, m_projection, m_viewport, &winX, &winY, &winZ);
            return Vec3f(static_cast<float>(winX), static_cast<float>(winY), static_cast<float>(winZ));
        }

        const Vec3f Camera::unproject(float x, float y, float depth) const {
            GLdouble rx, ry, rz;
            gluUnProject(x, y, depth, m_modelview, m_projection, m_viewport, &rx, &ry, &rz);
            
            return Vec3f(static_cast<float>(rx), static_cast<float>(ry), static_cast<float>(rz));
        }

        const Ray Camera::pickRay(float x, float y) const {
            Vec3f direction = (unproject(x, y, 0.5f) - m_position).normalized();
            return Ray(m_position, direction);
        }

        void Camera::update(float x, float y, float width, float height) {
            glMatrixMode(GL_PROJECTION);
            float vfrustum = static_cast<float>(tan(m_fieldOfVision * Math::Pi / 360)) * 0.75f * m_nearPlane;
            float hfrustum = vfrustum * width / height;
            glFrustum(-hfrustum, hfrustum, -vfrustum, vfrustum, m_nearPlane, m_farPlane);
            
            const Vec3f& pos = m_position;
            const Vec3f& at = m_position + m_direction;
            const Vec3f& up = m_up;
            
            glMatrixMode(GL_MODELVIEW);
            glViewport(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height));
            gluLookAt(pos.x, pos.y, pos.z, at.x, at.y, at.z, up.x, up.y, up.z);

            glGetIntegerv(GL_VIEWPORT, m_viewport);
            glGetDoublev(GL_MODELVIEW_MATRIX, m_modelview);
            glGetDoublev(GL_PROJECTION_MATRIX, m_projection);
        }
        
        void Camera::setBillboard() {
            Vec3f bbLook = m_direction * -1;
            Vec3f bbUp = m_up;
            Vec3f bbRight = bbUp % bbLook;
            
            float matrix[] = {bbRight.x, bbRight.y, bbRight.z, 0.0f, bbUp.x, bbUp.y, bbUp.z, 0.0f, bbLook.x, bbLook.y, bbLook.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
            glMultMatrixf(matrix);
        }

        float Camera::distanceTo(const Vec3f& point) {
            return sqrt(squaredDistanceTo(point));
        }
        
        float Camera::squaredDistanceTo(const Vec3f& point) {
            return (point - m_position).lengthSquared();
        }

        void Camera::moveTo(Vec3f position) {
            m_position = position;
        }
        
        void Camera::moveBy(float forward, float right, float up) {
            m_position += m_direction * forward;
            m_position += m_right * right;
            m_position += m_up * up;
        }
        
        void Camera::lookAt(Vec3f point, Vec3f up) {
            setDirection((point - m_position).normalized(), up);
        }
        
        void Camera::setDirection(Vec3f direction, Vec3f up) {
            m_direction = direction;
            m_right = (m_direction % up).normalized();
            m_up = m_right % m_direction;
        }
        
        void Camera::rotate(float yawAngle, float pitchAngle) {
            if (yawAngle == 0.0f && pitchAngle == 0.0f) return;
            
            Quat rotation = Quat(yawAngle, Vec3f::PosZ) * Quat(pitchAngle, m_right);
            Vec3f newDirection = rotation * m_direction;
            Vec3f newUp = rotation * m_up;

            if (newUp.z < 0.0f) {
                newUp.z = 0.0f;
                newDirection.x = 0.0f;
                newDirection.y = 0.0f;
            }
            
            setDirection(newDirection, newUp);
        }
        
        void Camera::orbit(Vec3f center, float hAngle, float vAngle) {
            if (hAngle == 0.0f && vAngle == 0.0f) return;
            
            Quat rotation = Quat(hAngle, Vec3f::PosZ) * Quat(vAngle, m_right);
            Vec3f newDirection = rotation * m_direction;
            Vec3f newUp = rotation * m_up;
            Vec3f offset = m_position - center;
            
            if (newUp.z < 0.0f) {
                newUp = m_up;
                newDirection.x = 0.0f;
                newDirection.y = 0.0f;
                newDirection.normalize();
                
                // correct rounding errors
                float cos = (std::max)(-1.0f, (std::min)(1.0f, m_direction | newDirection));
                float angle = acosf(cos);
                if (!Math::zero(angle)) {
                    Vec3f axis = (m_direction % newDirection).normalized();
                    rotation = Quat(angle, axis);
                    offset = rotation * offset;
                    newUp = rotation * newUp;
                }
            } else {
                offset = rotation * offset;
            }
            
            setDirection(newDirection, newUp);
            moveTo(offset + center);
        }
    }
}