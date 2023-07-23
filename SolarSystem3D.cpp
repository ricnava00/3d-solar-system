/*
 * TODO
 *
 * Turn off paninng when scrolling
 * Earth night texture + restore rotation code
 * Earth reflection texture
 * Transparent buttons
 * Ellipsoids https://stackoverflow.com/questions/52130939/ray-vs-ellipsoid-intersection
 * Moon
 *
 * Some custom planet handling
 * Sun becomes a planet
 * Collision detection (with tryclick and distance, push away from center)
 *
 * Animated skybox
 * Better json handling
 * Loading screen + hires textures
 */
#include "Starter.hpp"
#include "Planet.hpp"
#include "Button.hpp"
#include "ScrollButton.hpp"
#include "ScrollKnob.hpp"
#include "ButtonMaker.hpp"
#include "Controller.hpp"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>

using namespace std::placeholders;

struct UniformBufferObject {
	alignas(16) glm::mat4 mvpMat; // model view projection matrix
	alignas(16) glm::mat4 mMat; // model matrix
	alignas(16) glm::mat4 nMat; // normal matrix
};

struct GlobalUniformBufferObject {
	alignas(16) glm::vec2 selector;
	alignas(16) glm::vec3 lightPos;
	alignas(16) glm::vec4 lightColor;
	alignas(16) glm::vec3 eyePos;
};

struct Vertex {
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 UV;
};

struct RawVertex {
	glm::vec3 pos;
};

glm::vec3 vec4tovec3(glm::vec4 v)
{
	return glm::vec3(v/v.w);
}

template <typename T>
T damp(T newVar, T oldVar, float dampPerc)
{
	dampPerc = pow((1-dampPerc),0.5f);
	return oldVar*dampPerc+newVar*(1.f-dampPerc);
}

class SolarSystem3D;

SolarSystem3D *app;

class SolarSystem3D : public BaseProject {
	protected:

	float Ar; // current aspect ratio

	TextMaker txt;
	Controller *controller;

	DescriptorSetLayout DSL_Planet, DSL_Orbit, DSL_Skybox; // descriptor layout
	VertexDescriptor VD_Planet, VD_Raw; // vertex format
	Pipeline P, P_Ring, P_Sun, P_Orbit, P_Skybox; // pipeline

	// models, textures and descriptors
	Model<Vertex> M_Planet;
	std::vector<Model<Vertex>> M_Rings;
	Model<RawVertex> M_Orbit, M_Skybox;
	DescriptorSet DS_Su, DS_Skybox;
	std::vector<DescriptorSet> DS_Planets, DS_Rings, DS_Orbits;
	Texture T_Su, T_Skybox;
	std::vector<Texture> T_Planets, T_Rings;

	const float nearPlane = 0.01f;
	const float farPlane = 1000.0f;
	glm::mat4 WorldM = glm::translate(glm::mat4(1), glm::vec3(0.f, 0.f, -10.f));
	glm::mat4 ViewPrj;

	glm::vec3 CamPos=glm::vec3(0, 5, 9);
	glm::mat3 CamDir=glm::mat3({1, 0, 0}, {0, 0.973864, -0.227133}, {0, 0.227133, 0.973864});

	// solar system parameters
	const float sun_eqradius = 1.5f; // sun is not in scale with the other planets
	const float earth_eqradius = 0.4f;
	const float earth_majaxis = 7.f;
	const float earth_rottime = 2.5f;
	const float earth_revtime = 10.f;
	float earth_eqradius_real; //Real earth scale in respect to the sun set above. Still NOT in scale with distances, since it would result in showing everything at 1/100th the size
	float earth_rottime_real; //Real earth rotation time in respect to its revolution set above
	float rotRevFactor = 1.f;

	// planets
	const std::vector<std::string> planetNames = {"mercury", "venus", "earth", "mars", "jupiter", "saturn", "uranus", "neptune"};
	std::vector<Planet *> planets;
	std::vector<std::reference_wrapper<Button>> buttons;

	//buttons
	Button *toggleScalesButton;
	Button *planetButton;
	bool realScales = false;
	bool realPeriodsRatio = false;
	bool showOrbits = false;

	static void updatePlanetButton(bool isWatching, std::string planetName) {
		app->_updatePlanetButton(isWatching, planetName);
	}

	void _updatePlanetButton(bool isWatching, std::string planetName) {
		planetButton->setActive(isWatching);
		_replaceText(std::reference_wrapper<Button>(*planetButton), planetName);
	}

	static void setZoomedPlanetIndexRelative(Button &button, int offset) {
		app->_setZoomedPlanetIndexRelative(button, offset);
	}

	void _setZoomedPlanetIndexRelative(Button &button, int offset) {
		controller->setZoomedPlanetIndexRelative(offset);
	}

	static void updatePeriodsRatioAndScrollText(Button &button, float perc) {
		app->_updatePeriodsRatioAndScrollText(button, perc);
	}

	void _updatePeriodsRatioAndScrollText(Button &button, float perc) {
		rotRevFactor = pow(10,perc*4-2);
		char* str;
		asprintf(&str,"%gx",rotRevFactor);
		_replaceText(button, str);
		_updatePlanetScales();
	}

	static void replaceText(Button& button, std::string str) {
		app->_replaceText(button, str);
	}

	void _replaceText(Button& button, std::string str) {
		std::cout << "_replaceText " << &button << std::endl;
		txt.setButtonText(button, str);
		createCommandBuffers();
	}

	static void hideWhenClicked(Button& button) {
		app->_hideWhenClicked(button);
	}

	void _hideWhenClicked(Button& button) {
		std::cout << "_hideWhenClicked " << &button << std::endl;
		button.setVisible(false);
	}

	static void toggleWhenClicked(Button& button, bool &toggle) {
		app->_toggleWhenClicked(button, toggle);
	}

	void _toggleWhenClicked(Button& button, bool &toggle) {
		std::cout << "_toggleWhenClicked " << &button << std::endl;
		toggle=!toggle;
		button.setActive(toggle);
	}

	void _updatePlanetScales() {
		float rottime = realPeriodsRatio ? earth_rottime_real : earth_rottime;
		float revtime = earth_revtime;
		rottime /= rotRevFactor;
		revtime /= rotRevFactor;
		Planet::setScales(realScales ? earth_eqradius_real : earth_eqradius, earth_majaxis, rottime, revtime);
	}

	void setWindowParameters() {
		windowWidth = 1280;
		windowHeight = 720;
		windowTitle = "Solar System 3D";
		windowResizable = GLFW_TRUE;
		initialBackgroundColor = { 0.0f, 0.005f, 0.01f, 1.0f };
		Ar = (float) windowWidth / (float) windowHeight;

		initPlanets();
		// Descriptor pool sizes
		int numPlanets = planets.size() + 1;
		int numOrbits = planets.size();
		int numRings = 0;
		for (Planet *p : planets) {
			numRings += p->hasRing();
		}
		int numButtons = buttonInfo.size();
		uniformBlocksInPool = 1 + 1 + 3 * numButtons + 2 * numPlanets + 2 * numRings + 2 * numOrbits; //constants are text and skybox
		texturesInPool = 1 + 6 + 3 * numButtons + numPlanets + numRings + numOrbits;
		setsInPool = 1 + 1 + 3 * numButtons + numPlanets + numRings + numOrbits;
	}

	void onWindowResize(int w, int h) {
		Ar = (float)w / (float)h;
		windowWidth = w;
		windowHeight = h;
		txt.setAr(Ar);
		controller->setWindowSize(w, h);
	}

	void initPlanets() {
		std::ifstream f("solar-system-data.json");
	    json ss_data = json::parse(f);
	    json sun_data = ss_data["sun"];
	    json earth_data = ss_data["earth"];
		Planet::setScales(earth_eqradius, earth_majaxis, earth_rottime, earth_revtime);
		for (std::string name : planetNames) {
			Planet *p = new Planet(name, ss_data[name], earth_data);
			planets.push_back(p);
		}
		earth_eqradius_real = sun_eqradius * (float) earth_data["shape"]["equatorial_radius"] / (float) sun_data["shape"]["equatorial_radius"];
		earth_rottime_real = earth_revtime * (float) earth_data["times"]["synodic_rotation_period"] / (float) earth_data["times"]["sideral_orbital_period"];
	}

	void initButtons() {
		Button *toggleOrbitsButton = new Button([](Button &b) {
			app->_toggleWhenClicked(b, app->showOrbits);
		}, "Toggle orbits", {-0.98, -0.96}, ALIGN_LEFT, {1.2, 0});
		toggleOrbitsButton->setActive(showOrbits);
		toggleScalesButton = new Button([](Button &b) {
			app->_toggleWhenClicked(b, app->realScales);
			app->_updatePlanetScales();
		}, "Real scales", {0.98, -0.96}, ALIGN_RIGHT, {1.2, 0});
		toggleScalesButton->setActive(realScales);
		Button *togglePeriodsButton = new Button([](Button &b) {
			app->_toggleWhenClicked(b, app->realPeriodsRatio);
			app->_updatePlanetScales();
		}, "Real periods ratios", {0.98, -0.76}, ALIGN_RIGHT, {1.2, 0});
		togglePeriodsButton->setActive(realPeriodsRatio);
		planetButton = new Button([](Button&) {
		}, "Earth", {0, 0.82}, ALIGN_CENTER, {1, 0});
		planetButton->setActive(false);
		Button *prevPlanetButton = new Button(std::bind(setZoomedPlanetIndexRelative, _1, -1), "<", {0, 0}, ALIGN_RIGHT, {0, 0}, planetButton);
		Button *nextPlanetButton = new Button(std::bind(setZoomedPlanetIndexRelative, _1, 1), ">", {0, 0}, ALIGN_LEFT, {0, 0}, planetButton);
		Button *scrollPeriodsButton = new ScrollButton("", {0.98, -0.56}, ALIGN_RIGHT, {1.2, 0});
		scrollPeriodsButton->setAr(Ar);
		scrollPeriodsButton->setUIScale(UI_Scale);
		scrollPeriodsButton->align(true);
		ButtonInfo i = buttonInfo.at(SCROLL_KNOB);
		float width = scrollPeriodsButton->getFullWidth() * Ar / UI_Scale;
		width += (i.widthLeft + i.widthCenter + i.widthRight) * 2;
		Button *scrollPeriodsKnob = new ScrollKnob(std::bind(updatePeriodsRatioAndScrollText, std::reference_wrapper<Button>(*scrollPeriodsButton), _1), 4, 2, .85f, {0, 0}, ALIGN_RIGHT, {width, 0});
		scrollPeriodsKnob->setAr(Ar);
		scrollPeriodsKnob->setUIScale(UI_Scale);
		scrollPeriodsKnob->setOffset(scrollPeriodsButton->getSideGeometry(0).offset + glm::vec2(0, (362.f / 800 / 2 - scrollPeriodsKnob->getInfo().height / 2) * UI_Scale));
		buttons.push_back(*toggleOrbitsButton);
		buttons.push_back(*toggleScalesButton);
		buttons.push_back(*togglePeriodsButton);
		buttons.push_back(*planetButton);
		buttons.push_back(*prevPlanetButton);
		buttons.push_back(*nextPlanetButton);
		buttons.push_back(*scrollPeriodsButton);
		buttons.push_back(*scrollPeriodsKnob);
	}

	void localInit() {
		DS_Planets.resize(planets.size());
		int numRings=0;
		for (Planet *p : planets) {
			numRings += p->hasRing();
		}
		DS_Rings.resize(numRings);
		DS_Orbits.resize(planets.size());

		DSL_Planet.init(this, {
			{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
			{1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
			{2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
		});

		DSL_Orbit.init(this, {
			{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
			{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
		});

		DSL_Skybox.init(this, {
			{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
			{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
		});

		VD_Planet.init(this, {
			{0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX} },
			{ {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos), sizeof(glm::vec3), POSITION},
			{0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, norm), sizeof(glm::vec3), NORMAL},
			{0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, UV), sizeof(glm::vec2), UV}
		});

		VD_Raw.init(this,
			{{0, sizeof(RawVertex), VK_VERTEX_INPUT_RATE_VERTEX}},
			{{0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(RawVertex, pos), sizeof(glm::vec3), POSITION}}
		);

		P.init(this, &VD_Planet, "shaders/PlanetVert.spv", "shaders/PlanetFrag.spv", { &DSL_Planet });
		P_Sun.init(this, &VD_Planet, "shaders/PlanetVert.spv", "shaders/SunFrag.spv", { &DSL_Planet });
		P_Ring.init(this, &VD_Planet, "shaders/PlanetVert.spv", "shaders/RingFrag.spv", { &DSL_Planet });
		P_Ring.setAdvancedFeatures(VK_COMPARE_OP_LESS, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, true, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
		P_Orbit.init(this, &VD_Raw, "shaders/OrbitVert.spv", "shaders/OrbitFrag.spv", { &DSL_Orbit });
		P_Orbit.setAdvancedFeatures(VK_COMPARE_OP_LESS, VK_POLYGON_MODE_LINE, VK_CULL_MODE_BACK_BIT, true, VK_PRIMITIVE_TOPOLOGY_LINE_STRIP);
		P_Skybox.init(this, &VD_Raw, "shaders/SkyboxVert.spv", "shaders/SkyboxFrag.spv", { &DSL_Skybox });
		P_Skybox.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, false, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);

		T_Su.init(this, "textures/sun.jpg");
		Texture T;
		for (Planet *p: planets) {
			T.init(this, (std::string("") + "textures/planets/" + p->getName() + ".jpg").c_str());
			T_Planets.push_back(T);
			if(p->hasRing())
			{
				T.init(this, (std::string("") + "textures/rings/" + p->getName() + ".png").c_str());
				T_Rings.push_back(T);
			}
		}
		const char *SkyboxTextures[] = {"textures/sky/px.png", "textures/sky/nx.png", "textures/sky/py.png", "textures/sky/ny.png", "textures/sky/pz.png", "textures/sky/nz.png"};
		T_Skybox.initCubic(this, SkyboxTextures);


		createPlanetMesh(M_Planet.vertices, M_Planet.indices);
		M_Planet.initMesh(this, &VD_Planet);

		int n = 0;
		for (Planet *p : planets) {
			if (p->hasRing()) {
				Model<Vertex> M;
				glm::vec2 radii = p->getRingRadii();
				float innerRadiusScaled = radii.x / radii.y;
				Texture &T = T_Rings.at(n);
				float textureRatio = (float) T.texWidth / T.texHeight;
				createRingMesh(innerRadiusScaled, textureRatio, M.vertices, M.indices);
				M.initMesh(this, &VD_Planet);
				M_Rings.push_back(M);
			}
		}

		createOrbitMesh(M_Orbit.vertices, M_Orbit.indices);
		M_Orbit.initMesh(this, &VD_Raw);

		createSkyboxMesh(M_Skybox.vertices, M_Skybox.indices);
		M_Skybox.initMesh(this, &VD_Raw);

		initButtons();
		txt.init(this, buttons, Ar);

		controller = new Controller(window, planets, buttons, windowWidth, windowHeight, tryClickPlanet, updatePlanetButton);
	}

	void pipelinesAndDescriptorSetsInit() {
		P.create();
		P_Ring.create();
		P_Sun.create();
		P_Orbit.create();
		P_Skybox.create();

		DS_Su.init(this, &DSL_Planet, {
			{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
			{1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
			{2, TEXTURE, 0, &T_Su}
		});

		int n=0;
		int r=0;
		for (Planet *p : planets) {
			DS_Planets.at(n).init(this, &DSL_Planet, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
				{2, TEXTURE, 0, &T_Planets.at(n)}
			});
			if(p->hasRing())
			{
				DS_Rings.at(r).init(this, &DSL_Planet, {
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
					{2, TEXTURE, 0, &T_Rings.at(r)}
				});
				r++;
			}
			n++;
		}

		n=0;
		for (DescriptorSet &DS : DS_Orbits) {
			DS.init(this, &DSL_Orbit, {
				{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
				{1, TEXTURE, 0, &T_Planets.at(n)}
			});
			n++;
		}

		DS_Skybox.init(this, &DSL_Skybox, {
			{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
			{1, TEXTURE, 0, &T_Skybox}
		});

		txt.pipelinesAndDescriptorSetsInit();
	}

	void pipelinesAndDescriptorSetsCleanup() {
		P.cleanup();
		P_Ring.cleanup();
		P_Sun.cleanup();
		P_Orbit.cleanup();
		P_Skybox.cleanup();

		DS_Su.cleanup();
		for (DescriptorSet &DS : DS_Planets) {
			DS.cleanup();
		}
		for (DescriptorSet &DS : DS_Rings) {
			DS.cleanup();
		}
		for (DescriptorSet &DS : DS_Orbits) {
			DS.cleanup();
		}
		DS_Skybox.cleanup();

		txt.pipelinesAndDescriptorSetsCleanup();
	}

	void localCleanup() {
		T_Su.cleanup();
		for (Texture &T : T_Planets) {
			T.cleanup();
		}
		for (Texture &T : T_Rings) {
			T.cleanup();
		}
		T_Skybox.cleanup();

		M_Planet.cleanup();
		for (auto &M : M_Rings) {
			M.cleanup();
		}
		M_Orbit.cleanup();
		M_Skybox.cleanup();

		DSL_Planet.cleanup();
		DSL_Orbit.cleanup();
		DSL_Skybox.cleanup();

		P.destroy();
		P_Ring.destroy();
		P_Sun.destroy();
		P_Orbit.destroy();
		P_Skybox.destroy();

		txt.localCleanup();
	}

	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
		P_Skybox.bind(commandBuffer);
		M_Skybox.bind(commandBuffer);
		DS_Skybox.bind(commandBuffer, P_Skybox, 0, currentImage);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Skybox.indices.size()), 1, 0, 0, 0);

		P_Sun.bind(commandBuffer);

		M_Planet.bind(commandBuffer);

		DS_Su.bind(commandBuffer, P_Sun, 0, currentImage);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Planet.indices.size()), 1, 0, 0, 0);

		P.bind(commandBuffer);
		for (DescriptorSet &DS : DS_Planets) {
			DS.bind(commandBuffer, P, 0, currentImage);
			vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Planet.indices.size()), 1, 0, 0, 0);
		}

		vkCmdSetLineWidth(commandBuffer, 3);
		M_Orbit.bind(commandBuffer);
		P_Orbit.bind(commandBuffer);
		for (DescriptorSet &DS : DS_Orbits) {
			DS.bind(commandBuffer, P_Orbit, 0, currentImage);
			vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Orbit.indices.size()), 1, 0, 0, 0);
		}

		//Rings are transparent, draw last!
		P_Ring.bind(commandBuffer);
		int r=0;
		for (DescriptorSet &DS : DS_Rings) {
			M_Rings.at(r).bind(commandBuffer);
			DS.bind(commandBuffer, P_Ring, 0, currentImage);
			vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Rings.at(r).indices.size()), 1, 0, 0, 0);
			r++;
		}

		txt.populateCommandBuffer(commandBuffer, currentImage);
	}

	void updateOrbitsUniformBuffer(uint32_t currentImage) {
		glm::mat4 WorldM_P;
		UniformBufferObject ubo{};

		int n=0;
		for(Planet *p: planets)
		{
			if(showOrbits)
			{
				glm::vec2 orbitSize = p->getOrbitSize();
				WorldM_P =	WorldM * glm::rotate(glm::mat4(1), p->getOrbitInclination(), glm::vec3(0, 0, 1)) *
							glm::translate(glm::mat4(1), -p->getOrbitFocus()) *
							glm::scale(glm::mat4(1), glm::vec3(orbitSize.x, 0, orbitSize.y));

				ubo.mMat = WorldM_P;
				ubo.mvpMat = ViewPrj * ubo.mMat;
				ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
			}
			else
			{
				ubo.mMat = glm::mat4(0);
				ubo.mvpMat = glm::mat4(0);
				ubo.nMat = glm::mat4(0);
			}

			DS_Orbits.at(n).map(currentImage, &ubo, sizeof(ubo), 0);
			n++;
		}

		txt.updateUniformBuffer(currentImage);
	}

	void updateUniformBuffer(uint32_t currentImage) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GL_TRUE);

		///////// TIMER AND CONTROLLER ////////
		
		float deltaT; // time since the last call to the procedure
		glm::vec3 m = glm::vec3(0.0f); // motion in [-1,1] range (left stick of the gamepad, or ASWD + RF keys on the keyboard)
		glm::vec3 r = glm::vec3(0.0f); // rotation in [-1,1] range (right stick of the gamepad, or Arrow keys + QE keys on the keyboard, or mouse)
		bool fire = false; // true if fired has been pressed (SPACE on the keyboard, A or B button on the Gamepad, Right mouse button)
		getSixAxis(deltaT, m, r, fire, false);

		// instantiating controller

		/////////// WORLD MATRICES ///////////
		
		for (Planet *p : planets) {
			p->updatePosition(deltaT);
			p->updateRotation(deltaT);
		}

		///////// VIEW MATRIX /////////
		
		glm::mat4 ViewM = createView();

		///////// PROJECTION MATRIX /////////
		const float FOVy = glm::radians(45.0f);

		glm::mat4 ProjectionM = glm::perspective(FOVy, Ar, nearPlane, farPlane);
		ProjectionM[1][1] *= -1;
		ViewPrj = ProjectionM * ViewM;

		/////// TRANSFORMATIONS APPLICATION ///////
		glm::mat4 WorldM_P =	WorldM *
								glm::scale(glm::mat4(1), glm::vec3(sun_eqradius));

		GlobalUniformBufferObject gubo{};
		gubo.selector = glm::vec2(0);
		gubo.selector[static_cast<int>(controller->getRenderType())] = 1;
		gubo.lightPos = vec4tovec3(WorldM_P * glm::vec4(0, 0, 0, 1));
		gubo.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		gubo.eyePos = vec4tovec3(glm::inverse(ViewM) * glm::vec4(0, 0, 0, 1));

		UniformBufferObject ubo{};
		ubo.mMat = WorldM_P;
		ubo.mvpMat = ViewPrj * ubo.mMat;
		ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
		DS_Su.map(currentImage, &ubo, sizeof(ubo), 0);
		DS_Su.map(currentImage, &gubo, sizeof(gubo), 1);

		int n=0;
		int rn=0;
		for(Planet *p: planets)
		{
			WorldM_P =	WorldM * glm::translate(glm::mat4(1), p->getPosition()) *
						glm::rotate(glm::mat4(1), p->getTilt(), glm::vec3(0, 0, 1)) *
						glm::rotate(glm::mat4(1), p->getRotation(), glm::vec3(0, 1, 0)) *
						glm::scale(glm::mat4(1), p->getSize());

			ubo.mMat = WorldM_P;
			ubo.mvpMat = ViewPrj * ubo.mMat;
			ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
			DS_Planets.at(n).map(currentImage, &ubo, sizeof(ubo), 0);
			DS_Planets.at(n).map(currentImage, &gubo, sizeof(gubo), 1);
			if(p->hasRing())
			{
				WorldM_P =	WorldM * glm::translate(glm::mat4(1), p->getPosition()) *
							glm::rotate(glm::mat4(1), p->getTilt(), glm::vec3(0, 0, 1)) *
							glm::rotate(glm::mat4(1), p->getRingRotation(), glm::vec3(0, 1, 0)) *
							glm::scale(glm::mat4(1), glm::vec3(p->getRingRadii().y));

				ubo.mMat = WorldM_P;
				ubo.mvpMat = ViewPrj * ubo.mMat;
				ubo.nMat = glm::inverse(glm::transpose(ubo.mMat));
				DS_Rings.at(rn).map(currentImage, &ubo, sizeof(ubo), 0);
				DS_Rings.at(rn).map(currentImage, &gubo, sizeof(gubo), 1);
				rn++;
			}
			n++;
		}

		ubo.mMat = glm::mat4(1);
		ubo.mvpMat = ProjectionM * glm::transpose(glm::mat4(CamDir));
		ubo.nMat = glm::mat4(1);
		DS_Skybox.map(currentImage, &ubo, sizeof(ubo), 0);

		updateOrbitsUniformBuffer(currentImage);

		txt.updateUniformBuffer(currentImage);
	}

	glm::mat4 createView() {
		static glm::vec3 targetPos;
		static glm::vec3 targetDir;
		static glm::vec3 savedPos;
		static glm::vec3 savedDir;
		const float dampTime = 2.0f;

		static float camDist;
		static float damped = dampTime;

		const float ROT_SPEED = glm::radians(120.0f);
		const float MOVE_SPEED = 2.0f;
		float deltaT;
		glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
		bool fire;
		bool update = false;

		getSixAxis(deltaT, m, r, fire);

		ControllerActions currAction = controller->listenEvent();
		static ControllerActions lastAction = currAction;
		Planet *zoomedPlanet = controller->getZoomedPlanet();
		static Planet *lastZoomedPlanet = zoomedPlanet;
		if (lastZoomedPlanet != zoomedPlanet) {
			lastZoomedPlanet->enableRevolution();
		}
		if (lastAction != currAction || (currAction == ZoomPlanet && lastZoomedPlanet != zoomedPlanet)) {
			if (lastAction == NoAction)
			{
				savedPos = CamPos;
				savedDir = CamDir[2];
			}
			if (currAction == ZoomPlanet) {
				toggleScalesButton->setEnabled(false);
				zoomedPlanet->disableRevolution();
				targetPos = zoomedPlanet->getPosition(); //Watch planet
				targetPos -= glm::normalize(zoomedPlanet->getPosition()) * glm::vec3(4*zoomedPlanet->getSize().y); //Go back towards sun by factor proportional to planet radius
				targetPos = vec4tovec3(WorldM * glm::vec4(targetPos, 1)); //Transform by world matrix
				targetDir = -zoomedPlanet->getPosition();
				damped = 0;
			} else {
				toggleScalesButton->setEnabled(true);
				zoomedPlanet->enableRevolution();
				targetPos = savedPos;
				targetDir = savedDir;
				damped = 0;
			}
			lastAction = currAction;
			lastZoomedPlanet = zoomedPlanet;
		}
		if (damped < dampTime) {
			CamPos = damp(targetPos, CamPos, damped / dampTime);
			float angle = glm::orientedAngle(CamDir[2], glm::normalize(targetDir), glm::cross(CamDir[2], targetDir));
			if (std::abs(glm::degrees(angle)) > 2) {
				angle = damp(angle, 0.f, damped / dampTime);
				CamDir = glm::rotate(angle, glm::cross(CamDir[2], targetDir)) * glm::mat4(CamDir);
			}
			if (CamDir[0].y != 0) {
				//Fix z rotation
				glm::vec3 targetAxis;
				if (CamDir[2].y == 1) { //z axis is vertical, set directly to correct target
					targetAxis = glm::vec3(1, 0, 0);
				} else {
					targetAxis = glm::normalize(glm::cross(glm::vec3(0, 1, 0), CamDir[2]));
				}
				angle = glm::orientedAngle(CamDir[0], targetAxis, CamDir[2]);
				if (std::abs(glm::degrees(angle)) > 2) {
					angle = damp(angle, 0.f, damped / dampTime);
					CamDir = glm::rotate(angle, CamDir[2]) * glm::mat4(CamDir);
				}
			}
			damped += deltaT;
		} else {
			if (currAction == NoAction) {
				float speed = 1 + 9 * (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT));
				CamDir = glm::mat3(glm::rotate(glm::mat4(1.0f), -r.x * deltaT * ROT_SPEED, glm::vec3(CamDir[0])) * glm::mat4(CamDir));
				CamDir = glm::mat3(glm::rotate(glm::mat4(1.0f), -r.y * deltaT * ROT_SPEED, glm::vec3(CamDir[1])) * glm::mat4(CamDir));
				CamDir = glm::mat3(glm::rotate(glm::mat4(1.0f), r.z * deltaT * ROT_SPEED, glm::vec3(CamDir[2])) * glm::mat4(CamDir));
				CamPos += m.x * MOVE_SPEED * speed * glm::vec3(CamDir[0]) * deltaT;
				CamPos += m.y * MOVE_SPEED * speed * glm::vec3(CamDir[1]) * deltaT;
				CamPos -= m.z * MOVE_SPEED * speed * glm::vec3(CamDir[2]) * deltaT;
			} else {
				CamPos = targetPos;
			}
		}
		return glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos);
	}

	static int tryClickPlanet(float mouseX, float mouseY) {
		return app->_tryClickPlanet(mouseX, mouseY);
	}

	//http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-a-physics-library/
	/** Finds the closest clicked planet (if any) and zooms in on it */
	int _tryClickPlanet(float mouseX, float mouseY) {
		glm::vec4 rayStart_NDC(mouseX, mouseY, nearPlane, 1.0f);
		glm::vec4 rayEnd_NDC(mouseX, mouseY, farPlane, 1.0f);

		glm::mat4 InverseViewProj = glm::inverse(ViewPrj);

		glm::vec3 rayStart_world = vec4tovec3(InverseViewProj * rayStart_NDC);
		glm::vec3 rayEnd_world = vec4tovec3(InverseViewProj * rayEnd_NDC);
		glm::vec3 rayDir_world = glm::normalize(rayEnd_world - rayStart_world);

		int found = -1;
		float minDistance = FLT_MAX;
		float distance;
		int n=0;
		for (Planet *p : planets) {
			float planetScale = 1; //TODO slider scale
			distance = raySphereIntersect(rayStart_world, rayDir_world, glm::vec3(
					WorldM *
					glm::translate(glm::mat4(1), p->getPosition()) *
					glm::scale(glm::mat4(1), glm::vec3(planetScale)) * glm::vec4(0, 0, 0, 1)),
					p->getSize().y * planetScale);
			if (distance >= 0 && distance < minDistance) {
				found = n;
				minDistance = distance;
			}
			n++;
		}
		return found;
	}

	//https://iquilezles.org/articles/intersectors/
	/** If the ray intersects the sphere, returns the distance from the ray origin to the closest intersection point. Otherwise returns -1 */
	static float raySphereIntersect(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 sphereCenter, float sphereRadius) {
		glm::vec3 oc = rayOrigin - sphereCenter;
		float b = dot(oc, rayDirection);
		glm::vec3 qc = oc - b * rayDirection;
		float h = sphereRadius * sphereRadius - dot(qc, qc);
		if (h < 0.0)
			return -1.0; // no intersection
		h = sqrt(h);
		return b - h;
	}

	void createPlanetMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx);
	void createRingMesh(float innerRadius, float textureRatio, std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx);
	void createOrbitMesh(std::vector<RawVertex> &vDef, std::vector<uint32_t> &vIdx);
	void createSkyboxMesh(std::vector<RawVertex> &vDef, std::vector<uint32_t> &vIdx);

};

#include "meshGenerator.hpp"

int main() {
	app = new SolarSystem3D();
	try {
		app->run();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
