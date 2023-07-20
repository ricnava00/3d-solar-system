#include "Planet.hpp"

void Planet::setShape(float earth_eqradius, json planet_data, json earth_data){
    
    this->x_radius = earth_eqradius * (float) planet_data["equatorial_radius"] / (float) earth_data["equatorial_radius"];
    this->y_radius = this->x_radius;
    this->z_radius = this->x_radius;

    this->tilt = glm::radians(- (float) planet_data["axial_tilt"]);
    this->theta = 0.f;
}

void Planet::setOrbit(float earth_majaxis, json planet_data, json earth_data){
    
    // real planet data
    float pl_semi_major = planet_data["semi-major_axis"];
    float ea_semi_major = earth_data["semi-major_axis"];
    float pl_e = planet_data["eccentricity"];
    float pl_aphelion = planet_data["aphelion"];
    float pl_inclination = planet_data["inclination"];
    std::string pl_aphelion_dir = planet_data["aphelion_dir"];
    float pl_radius_diff = pl_aphelion - pl_semi_major;
    float c_shift = earth_majaxis * pl_radius_diff / ea_semi_major;

    // simulation data
    this->a = earth_majaxis * pl_semi_major / ea_semi_major;
    this->b = this->a * sqrt(1 - pow(pl_e, 2));
    if (pl_aphelion_dir == "r") this->c = glm::vec3(- c_shift, 0.f, 0.f);
    else this->c = glm::vec3(c_shift, 0.f, 0.f);

    this->inclination = glm::radians(pl_inclination);
    this->phi = 0.f;

}

void Planet::setSpeeds(float earth_rottime, float earth_revtime, json planet_data, json earth_data){

    float rot_time = earth_rottime * (float) planet_data["synodic_rotation_period"] / (float) earth_data["synodic_rotation_period"];
    float rev_time = earth_revtime * (float) planet_data["sideral_orbital_period"] / (float) earth_data["sideral_orbital_period"];

    this->rot_speed = 2 * (float) M_PI / rot_time;
    this->rev_speed = 2 * (float) M_PI / rev_time;

}

Planet::Planet(float earth_eqradius, float earth_majaxis, float earth_rottime, float earth_revtime, std::string planet_name){

    std::ifstream f("solar-system-data.json");
    json ss_data = json::parse(f);

    json planet_data = ss_data[planet_name];
    json earth_data = ss_data["earth"];

    this->planet_name = planet_name;
    this->setShape(earth_eqradius, planet_data["shape"], earth_data["shape"]);
    this->setOrbit(earth_majaxis, planet_data["orbit"], earth_data["orbit"]);
    this->setSpeeds(earth_rottime, earth_revtime, planet_data["times"], earth_data["times"]);
    this->rev_active = true;

}

void Planet::updatePosition(float deltaT){

    this->phi += deltaT * this->rev_speed * rev_active;
    this->phi = fmod(this->phi, 2 * (float) M_PI);

}

void Planet::updateRotation(float deltaT){

    this->theta += deltaT * this->rot_speed;
    this->theta = fmod(this->theta, 2 * (float) M_PI);

}

void Planet::enableRevolution(){

    this->rev_active = true;

}

void Planet::disableRevolution(){

    this->rev_active = false;

}

std::string Planet::getName() {
	return planet_name;
}

glm::vec3 Planet::getSize(){

    return glm::vec3(this->x_radius, this->y_radius, this->z_radius);

}

glm::vec3 Planet::getPosition(){

    // Flat orbit position
    float x = this->a * cos(this->phi) - this->c.x;
    float y = 0.f - this->c.y;
    float z = this->b * sin(this->phi) - this->c.z;

    // Projection of the point on the inclined orbit
    x = x * cos(this->inclination) - y * sin(this->inclination);
    y = x * sin(this->inclination) + y * cos(this->inclination);

    return glm::vec3(x, y, z);

}

float Planet::getTilt(){

    return this->tilt;

}

float Planet::getRotation(){

    return this->theta;

}
