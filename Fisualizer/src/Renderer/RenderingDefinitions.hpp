#pragma once

inline const float WIDTH = 800;
inline const float HEIGHT = 800;

inline glm::vec3 globalPosition(0.0f, 0.0f, 50.0f);
inline glm::vec3 globalCameraTarget(0);

inline glm::vec3 cameraPosition = globalPosition;
inline glm::vec3 cameraTarget(0, 0, 0);
inline glm::vec3 cameraUp(0, 1, 0);