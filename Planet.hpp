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

        // CONTROLS
        bool rev_active; // true if the revolution of the planet is active, false otherwise

        void setShape(float earth_eqradius, json planet_data, json earth_data);
        void setOrbit(float earth_majaxis, json planet_data, json earth_data);
        void setSpeeds(float earth_rottime, float earth_revtime, json planet_data, json earth_data);
    
    public:

        Planet(float earth_eqradius, float earth_majaxis, float earth_rottime, float earth_revtime, std::string planet_name);

        void updatePosition(float deltaT);
        void updateRotation(float deltaT);
        void enableRevolution();
        void disableRevolution();

        std::string getName();
        glm::vec3 getSize();
        glm::vec3 getPosition();
        float getTilt();
        float getRotation();

        // controller features
        float getZoomDistance();
        glm::vec3 getNameSize();
        glm::vec3 getNamePosition();

};
