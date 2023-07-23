#pragma once

#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <json.hpp>

using json = nlohmann::json;

class Planet{

    private:
		std::string planet_name;

        // SHAPE
        float x_radius;
        float y_radius;
        float z_radius;
        float tilt;
        float theta; // angle of rotation of the planet around its axis

        // ORBIT
        float a; // semi-major axis
        float b; // semi-minor axis
        glm::vec3 c; // center
        float inclination;
        float phi; // angle defined by the position of the planet in the orbit

        // SPEEDS
        float rot_speed;
        float rev_speed;

        // RING
        bool ring;
        float ring_theta;
        float ring_inner_radius;
        float ring_outer_radius;
        float ring_rot_speed;

        // SCALES
        bool rev_active; // true if the revolution of the planet is active, false otherwise //TODO remove
        inline static float radius_scale;
        inline static float orbit_scale;
        inline static float rotation_scale;
        inline static float revolution_scale;

        void setShape(json planet_data, json earth_data);
        void setOrbit(json planet_data, json earth_data);
        void setSpeeds(json planet_data, json earth_data);
        void setRing(json ring_data, json earth_shape_data, json earth_times_data);
    public:

        Planet(std::string planet_name, json &planet_data, json &earth_data);

        static void setScales(float radius_scale, float orbit_scale, float rotation_scale, float revolution_scale);
        void updatePosition(float deltaT);
        void updateRotation(float deltaT);
        void enableRevolution();
        void disableRevolution();

        std::string getName();
        glm::vec3 getSize();
        glm::vec3 getPosition();
        float getTilt();
        float getRotation();
        glm::vec2 getOrbitSize();
        float getOrbitInclination();
        glm::vec3 getOrbitFocus();
        bool hasRing();
        glm::vec2 getRingRadii();
        float getRingRotation();

        // controller features
        float getZoomDistance();
        glm::vec3 getNameSize();
        glm::vec3 getNamePosition();

};
