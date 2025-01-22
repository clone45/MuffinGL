// camera.hpp
#pragma once

class Camera {
public:
    Camera();
    Camera(float x, float y, float zoom);

    // Core operations
    void pan(float dx, float dy);
    void zoomAtPoint(float factor, int screenX, int screenY);
    
    // Coordinate conversion
    void worldToScreen(float worldX, float worldY, int& screenX, int& screenY) const;
    void screenToWorld(int screenX, int screenY, float& worldX, float& worldY) const;

    // Accessors
    float getX() const { return m_x; }
    float getY() const { return m_y; }
    float getZoom() const { return m_zoom; }

private:
    float m_x;      // Camera position in world space
    float m_y;
    float m_zoom;   // Zoom factor (1.0 = no zoom)
};