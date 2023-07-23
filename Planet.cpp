#include "Planet.hpp"
#include <glm/gtx/transform.hpp>

Planet::Planet(std::string planet_name, json &planet_data, json &earth_data) {

    this->planet_name = planet_name;
    this->setShape(planet_data["shape"], earth_data["shape"]);
    this->setOrbit(planet_data["orbit"], earth_data["orbit"]);
    this->setSpeeds(planet_data["times"], earth_data["times"]);
    this->ring = false;
    if (planet_data.contains("ring")) {
		this->ring = true;
		setRing(planet_data["ring"], earth_data["shape"], earth_data["times"]);
	}
    this->rev_active = true;

}

void Planet::setScales(float radius_scale, float orbit_scale, float rotation_scale, float revolution_scale) {
	Planet::radius_scale = radius_scale;
	Planet::orbit_scale = orbit_scale;
	Planet::rotation_scale = 1.f / rotation_scale; //time to speed
	Planet::revolution_scale = 1.f / revolution_scale;
}

void Planet::setShape(json planet_data, json earth_data){
    
    this->x_radius = (float) planet_data["equatorial_radius"] / (float) earth_data["equatorial_radius"];
    this->y_radius = this->x_radius;
    this->z_radius = this->x_radius;

    this->tilt = glm::radians(- (float) planet_data["axial_tilt"]);
    this->theta = 0.f;
}

void Planet::setOrbit(json planet_data, json earth_data){
    
    // real planet data
    float pl_semi_major = planet_data["semi-major_axis"];
    float ea_semi_major = earth_data["semi-major_axis"];
    float pl_e = planet_data["eccentricity"];
    float pl_aphelion = planet_data["aphelion"];
    float pl_inclination = planet_data["inclination"];
    std::string pl_aphelion_dir = planet_data["aphelion_dir"];
    float pl_radius_diff = pl_aphelion - pl_semi_major;
    float c_shift = pl_radius_diff / ea_semi_major;

    // simulation data
    this->a = pl_semi_major / ea_semi_major;
    this->b = this->a * sqrt(1 - pow(pl_e, 2));
	this->c = glm::vec3(c_shift * ((pl_aphelion_dir == "r") ? -1 : 1), 0.f, 0.f);

    this->inclination = glm::radians(pl_inclination);
    this->phi = 0.f;

}

void Planet::setSpeeds(json planet_data, json earth_data){

    float rot_time = (float) planet_data["synodic_rotation_period"] / (float) earth_data["synodic_rotation_period"];
    float rev_time = (float) planet_data["sideral_orbital_period"] / (float) earth_data["sideral_orbital_period"];

    this->rot_speed = 2 * (float) M_PI / rot_time;
    this->rev_speed = 2 * (float) M_PI / rev_time;

}

void Planet::setRing(json ring_data, json earth_shape_data, json earth_times_data) {
	this->ring_inner_radius = (float) ring_data["inner_radius"] / (float) earth_shape_data["equatorial_radius"];
	this->ring_outer_radius = (float) ring_data["outer_radius"] / (float) earth_shape_data["equatorial_radius"];
	float rot_time = (float) ring_data["rotation_period"] / (float) earth_times_data["synodic_rotation_period"];
	this->ring_rot_speed = 2 * (float) M_PI / rot_time;
}

void Planet::updatePosition(float deltaT){

    this->phi += deltaT * this->rev_speed * revolution_scale * rev_active;
    this->phi = fmod(this->phi, 2 * (float) M_PI);

}

void Planet::updateRotation(float deltaT){

    this->theta += deltaT * this->rot_speed * rotation_scale;
    this->theta = fmod(this->theta, 2 * (float) M_PI);
	if (this->ring) {
		this->ring_theta += deltaT * this->ring_rot_speed * rotation_scale;
		this->ring_theta = fmod(this->ring_theta, 2 * (float) M_PI);
	}

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

	return glm::vec3(this->x_radius, this->y_radius, this->z_radius) * radius_scale;

}

glm::vec3 _vec4tovec3(glm::vec4 v)
{
	return glm::vec3(v/v.w);
}

glm::vec3 Planet::getPosition(){

    // Flat orbit position
    float x = this->a * cos(this->phi) - this->c.x;
    float y = 0.f - this->c.y;
    float z = this->b * sin(this->phi) - this->c.z;

    // Projection of the point on the inclined orbit
	return glm::vec3(x * cos(this->inclination), x * sin(this->inclination), z) * orbit_scale;

}

float Planet::getTilt(){

    return this->tilt;

}

float Planet::getRotation(){

    return this->theta;

}

float Planet::getOrbitInclination(){

    return this->inclination;

}

glm::vec2 Planet::getOrbitSize(){

	return glm::vec2(this->a, this->b) * orbit_scale;

}

glm::vec3 Planet::getOrbitFocus(){

	return this->c * orbit_scale;

}

bool Planet::hasRing() {
	return this->ring;
}

glm::vec2 Planet::getRingRadii() {
	return glm::vec2(this->ring_inner_radius, this->ring_outer_radius) * radius_scale;
}

float Planet::getRingRotation() {
	return this->ring_theta;
}

float Planet::getZoomDistance(){

    float coeff = this->x_radius / this->a;
    return -2.272727f * coeff + 0.97977273f;

}

glm::vec3 Planet::getNameSize(){

    float size = std::max(std::min(1.75f * this->x_radius, 5.f), 0.7f);
    return glm::vec3(size);

}

glm::vec3 Planet::getNamePosition(){

    glm::vec3 distance = glm::vec3(
        0,
        this->getNameSize().y + this->y_radius,
        0
    );
    return this->getPosition() + distance;

}
