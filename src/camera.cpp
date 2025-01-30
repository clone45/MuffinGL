// camera.cpp
#include "muffinGL/camera.hpp"

Camera::Camera()
    : m_x(0.0f)
    , m_y(0.0f)
    , m_zoom(1.0f)
{
}

Camera::Camera(float x, float y, float zoom)
    : m_x(x)
    , m_y(y)
    , m_zoom(zoom)
{
}

void Camera::pan(float dx, float dy) {
    m_x += dx;
    m_y += dy;
}

void Camera::zoomAtPoint(float factor, int screenX, int screenY) {
    // Convert screen point to world space
    float worldX, worldY;
    screenToWorld(screenX, screenY, worldX, worldY);
    
    // Apply zoom
    float oldZoom = m_zoom;
    m_zoom *= factor;
    
    // Adjust camera position to keep worldX,worldY at the same screen position
    float newWorldX, newWorldY;
    screenToWorld(screenX, screenY, newWorldX, newWorldY);
    
    m_x += worldX - newWorldX;
    m_y += worldY - newWorldY;
}

void Camera::worldToScreen(float worldX, float worldY, int& screenX, int& screenY) const {
    screenX = static_cast<int>((worldX - m_x) * m_zoom);
    screenY = static_cast<int>((worldY - m_y) * m_zoom);
}

void Camera::screenToWorld(int screenX, int screenY, float& worldX, float& worldY) const {
    worldX = (screenX / m_zoom) + m_x;
    worldY = (screenY / m_zoom) + m_y;
}