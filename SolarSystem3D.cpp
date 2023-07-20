#include "Starter.hpp"
#include "Planet.hpp"
#include "Button.hpp"
#include "ScrollButton.hpp"
#include "ScrollKnob.hpp"
#include "ButtonMaker.hpp"
#include "Controller.hpp"
#include <glm/gtx/string_cast.hpp>

using namespace std::placeholders;

struct UniformBufferObject {
	alignas(16) glm::mat4 mvpMat; // model view projection matrix
	alignas(16) glm::mat4 mMat; // model matrix
	alignas(16) glm::mat4 nMat; // normal matrix
};

struct GlobalUniformBufferObject {
	alignas(16) glm::vec3 lightPos;
	alignas(16) glm::vec4 lightColor;
	alignas(16) glm::vec3 eyePos;
};

struct Vertex {
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 UV;
};

struct ColoredVertex {
	glm::vec3 pos;
	glm::vec3 color;
};

glm::vec3 vec4tovec3(glm::vec4 v)
{
	return glm::vec3(v/v.w);
}

template <typename T>
T damp(T newVar, T oldVar, float deltaT, float factor=10)
{
	return oldVar*(float)pow(M_E,-factor*deltaT)+newVar*(float)(1-pow(M_E,-factor*deltaT));
}

class SolarSystem3D;

SolarSystem3D *app;

class SolarSystem3D : public BaseProject {
	protected:

	float Ar; // current aspect ratio

	TextMaker txt;

	DescriptorSetLayout DSL; // descriptor layout
	VertexDescriptor VD; // vertex format
	Pipeline P; // pipeline

	// models, textures and descriptors
	Model<Vertex> M_Su, M_Me, M_Ve, M_Ea, M_Ma, M_Ju, M_Sa, M_Ur, M_Ne;
	DescriptorSet DS_Su, DS_Me, DS_Ve, DS_Ea, DS_Ma, DS_Ju, DS_Sa, DS_Ur, DS_Ne;
	Texture T_Su, T_Me, T_Ve, T_Ea, T_Ma, T_Ju, T_Sa, T_Ur, T_Ne;

	UniformBufferObject ubo_Su, ubo_Me, ubo_Ve, ubo_Ea, ubo_Ma, ubo_Ju, ubo_Sa, ubo_Ur, ubo_Ne;

	// solar system parameters
	const float sun_eqradius = 1.5f; // sun is not in scale with the other planets
	const float earth_eqradius = 0.4f;
	const float earth_majaxis = 7.f;
	const float earth_rottime = 2.5f;
	const float earth_revtime = 10.f;

	// planets
	Planet mercury{earth_eqradius, earth_majaxis, earth_rottime, earth_revtime, "mercury"};
	Planet venus{earth_eqradius, earth_majaxis, earth_rottime, earth_revtime, "venus"};
	Planet earth{earth_eqradius, earth_majaxis, earth_rottime, earth_revtime, "earth"};
	Planet mars{earth_eqradius, earth_majaxis, earth_rottime, earth_revtime, "mars"};
	Planet jupiter{earth_eqradius, earth_majaxis, earth_rottime, earth_revtime, "jupiter"};
	Planet saturn{earth_eqradius, earth_majaxis, earth_rottime, earth_revtime, "saturn"};
	Planet uranus{earth_eqradius, earth_majaxis, earth_rottime, earth_revtime, "uranus"};
	Planet neptune{earth_eqradius, earth_majaxis, earth_rottime, earth_revtime, "neptune"};

	std::vector<Planet *> planets{&mercury, &venus, &earth, &mars, &jupiter, &saturn, &uranus, &neptune};
	std::vector<std::reference_wrapper<Button>> buttons;

	bool exampleToggleableProperty=false;
	float exampleValue=1.0f;

	static void multiplyAppendAndUpdateText(Button& button, Button& targetButton, float *value, float factor, std::string append)
	{
		std::stringstream stream;
		std::cout << value << std::endl;
		*value*=factor;
		char* str;
		std::sprintf(str,"%.5g",*value);
		app->_replaceText(targetButton, str+append);
	}

	static void updateScrollText(Button &button, float perc) {
		app->_replaceText(button, std::to_string(static_cast<int>(perc * 100)) + "%");
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

	static void toggleWhenClicked(Button& button) {
		app->_toggleWhenClicked(button);
	}

	void _toggleWhenClicked(Button& button) {
		std::cout << "_toggleWhenClicked " << &button << std::endl;
		exampleToggleableProperty=!exampleToggleableProperty;
		button.setActive(exampleToggleableProperty);
	}

	void setWindowParameters() {
		windowWidth = 1280;
		windowHeight = 720;
		windowTitle = "Solar System 3D";
		windowResizable = GLFW_TRUE;
		initialBackgroundColor = { 0.0f, 0.005f, 0.01f, 1.0f };

		// Descriptor pool sizes
		uniformBlocksInPool = 1 + 1 + 3 * buttonInfo.size() + 2 * (planets.size()+1); //orbit, text
		texturesInPool = 1 + 3 * buttonInfo.size() + (planets.size()+1); //text
		setsInPool = 1 + 1 + 3 * buttonInfo.size() + (planets.size()+1); //orbit, text

		Ar = (float)windowWidth / (float)windowHeight;
	}

	void onWindowResize(int w, int h) {
		Ar = (float)w / (float)h;
		windowWidth = w;
		windowHeight = h;
		txt.setAr(Ar);
	}

	void localInit() {
		DSL.init(this, {
			{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
			{1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
			{2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
		});

		VD.init(this, {
			{0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX} },
			{ {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos), sizeof(glm::vec3), POSITION},
			{0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, norm), sizeof(glm::vec3), NORMAL},
			{0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, UV), sizeof(glm::vec2), UV}
		});

		P.init(this, &VD, "shaders/PlanetVert.spv", "shaders/PlanetFrag.spv", { &DSL });
		P.setAdvancedFeatures(VK_COMPARE_OP_LESS, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, false, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

		createPlanetMesh(M_Su.vertices, M_Su.indices);
		M_Su.initMesh(this, &VD);

		createPlanetMesh(M_Me.vertices, M_Me.indices);
		M_Me.initMesh(this, &VD);

		createPlanetMesh(M_Ve.vertices, M_Ve.indices);
		M_Ve.initMesh(this, &VD);

		createPlanetMesh(M_Ea.vertices, M_Ea.indices);
		M_Ea.initMesh(this, &VD);

		createPlanetMesh(M_Ma.vertices, M_Ma.indices);
		M_Ma.initMesh(this, &VD);

		createPlanetMesh(M_Ju.vertices, M_Ju.indices);
		M_Ju.initMesh(this, &VD);

		createPlanetMesh(M_Sa.vertices, M_Sa.indices);
		M_Sa.initMesh(this, &VD);

		createPlanetMesh(M_Ur.vertices, M_Ur.indices);
		M_Ur.initMesh(this, &VD);

		createPlanetMesh(M_Ne.vertices, M_Ne.indices);
		M_Ne.initMesh(this, &VD);
		
		T_Su.init(this, "textures/sun.jpg");
		T_Me.init(this, "textures/planets/mercury.jpg");
		T_Ve.init(this, "textures/planets/venus.jpg");
		T_Ea.init(this, "textures/planets/earth.jpg");
		T_Ma.init(this, "textures/planets/mars.jpg");
		T_Ju.init(this, "textures/planets/jupiter.jpg");
		T_Sa.init(this, "textures/planets/saturn.jpg");
		T_Ur.init(this, "textures/planets/uranus.jpg");
		T_Ne.init(this, "textures/planets/neptune.jpg");

		Button *testButton = new Button(toggleWhenClicked, "Test toggle", {-1,-1}, false, {2,0});
		buttons.push_back(*testButton);
		Button *testButton2 = new Button(toggleWhenClicked, "Test toggle2", {0,0}, false, {0,0}, testButton);
		buttons.push_back(*testButton2);
		Button *scrollButton = new ScrollButton("Test scroll", {0.1,0}, false, {1,0});
		buttons.push_back(*scrollButton);
		Button *scrollKnob = new ScrollKnob(std::bind(updateScrollText,std::reference_wrapper<Button>(*scrollButton),_1), 10, 5, 0.82f, {0,0}, false, {(scrollButton->getFullWidth()-scrollButton->getInfo().marginHorizontal)/UI_Scale,0});
		scrollKnob->setOffset({0.1,(362.f/800/2-scrollKnob->getInfo().height/2)*UI_Scale});
		buttons.push_back(*scrollKnob);
		txt.init(this, buttons, Ar);
		txt.setButtonsActive({exampleToggleableProperty,true,true,true});
	}

	void pipelinesAndDescriptorSetsInit() {
		P.create();

		DS_Su.init(this, &DSL, {
			{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
			{1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
			{2, TEXTURE, 0, &T_Su}
		});

		
		DS_Me.init(this, &DSL, {
			{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
			{1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
			{2, TEXTURE, 0, &T_Me}
		});

		DS_Ve.init(this, &DSL, {
			{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
			{1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
			{2, TEXTURE, 0, &T_Ve}
		});

		DS_Ea.init(this, &DSL, {
			{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
			{1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
			{2, TEXTURE, 0, &T_Ea}
		});

		DS_Ma.init(this, &DSL, {
			{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
			{1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
			{2, TEXTURE, 0, &T_Ma}
		});

		DS_Ju.init(this, &DSL, {
			{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
			{1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
			{2, TEXTURE, 0, &T_Ju}
		});

		DS_Sa.init(this, &DSL, {
			{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
			{1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
			{2, TEXTURE, 0, &T_Sa}
		});

		DS_Ur.init(this, &DSL, {
			{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
			{1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
			{2, TEXTURE, 0, &T_Ur}
		});

		DS_Ne.init(this, &DSL, {
			{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
			{1, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr},
			{2, TEXTURE, 0, &T_Ne}
		});

		txt.pipelinesAndDescriptorSetsInit();
	}

	void pipelinesAndDescriptorSetsCleanup() {
		P.cleanup();

		DS_Su.cleanup();
		DS_Me.cleanup();
		DS_Ve.cleanup();
		DS_Ea.cleanup();
		DS_Ma.cleanup();
		DS_Ju.cleanup();
		DS_Sa.cleanup();
		DS_Ur.cleanup();
		DS_Ne.cleanup();
		txt.pipelinesAndDescriptorSetsCleanup();
	}

	void localCleanup() {
		T_Su.cleanup();
		T_Me.cleanup();
		T_Ve.cleanup();
		T_Ea.cleanup();
		T_Ma.cleanup();
		T_Ju.cleanup();
		T_Sa.cleanup();
		T_Ur.cleanup();
		T_Ne.cleanup();

		M_Su.cleanup();
		M_Me.cleanup();
		M_Ve.cleanup();
		M_Ea.cleanup();
		M_Ma.cleanup();
		M_Ju.cleanup();
		M_Sa.cleanup();
		M_Ur.cleanup();
		M_Ne.cleanup();

		DSL.cleanup();

		P.destroy();
		txt.localCleanup();
	}

	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
		P.bind(commandBuffer);

		DS_Su.bind(commandBuffer, P, 0, currentImage);
		M_Su.bind(commandBuffer);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Su.indices.size()), 1, 0, 0, 0);

		DS_Me.bind(commandBuffer, P, 0, currentImage);
		M_Me.bind(commandBuffer);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Me.indices.size()), 1, 0, 0, 0);
		
		DS_Ve.bind(commandBuffer, P, 0, currentImage);
		M_Ve.bind(commandBuffer);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Ve.indices.size()), 1, 0, 0, 0);

		DS_Ea.bind(commandBuffer, P, 0, currentImage);
		M_Ea.bind(commandBuffer);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Ea.indices.size()), 1, 0, 0, 0);

		DS_Ma.bind(commandBuffer, P, 0, currentImage);
		M_Ma.bind(commandBuffer);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Ma.indices.size()), 1, 0, 0, 0);

		DS_Ju.bind(commandBuffer, P, 0, currentImage);
		M_Ju.bind(commandBuffer);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Ju.indices.size()), 1, 0, 0, 0);

		DS_Sa.bind(commandBuffer, P, 0, currentImage);
		M_Sa.bind(commandBuffer);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Sa.indices.size()), 1, 0, 0, 0);

		DS_Ur.bind(commandBuffer, P, 0, currentImage);
		M_Ur.bind(commandBuffer);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Ur.indices.size()), 1, 0, 0, 0);

		DS_Ne.bind(commandBuffer, P, 0, currentImage);
		M_Ne.bind(commandBuffer);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(M_Ne.indices.size()), 1, 0, 0, 0);

		txt.populateCommandBuffer(commandBuffer, currentImage);
	}

	void updateUniformBuffer(uint32_t currentImage) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GL_TRUE);

		///////// TIMER AND CONTROLLER ////////
		
		float deltaT; // time since the last call to the procedure
		glm::vec3 m = glm::vec3(0.0f); // motion in [-1,1] range (left stick of the gamepad, or ASWD + RF keys on the keyboard)
		glm::vec3 r = glm::vec3(0.0f); // rotation in [-1,1] range (right stick of the gamepad, or Arrow keys + QE keys on the keyboard, or mouse)
		bool fire = false; // true if fired has been pressed (SPACE on the keyboard, A or B button on the Gamepad, Right mouse button)
		getSixAxis(deltaT, m, r, fire);

		// instanciating controller
		static Controller controller{window, planets, buttons};
		ControllerActions currAction = controller.listenEvent();

		/////////// WORLD MATRICES ///////////
		
		mercury.updatePosition(deltaT);
		mercury.updateRotation(deltaT);

		venus.updatePosition(deltaT);
		venus.updateRotation(deltaT);

		earth.updatePosition(deltaT);
		earth.updateRotation(deltaT);

		mars.updatePosition(deltaT);
		mars.updateRotation(deltaT);

		jupiter.updatePosition(deltaT);
		jupiter.updateRotation(deltaT);

		saturn.updatePosition(deltaT);
		saturn.updateRotation(deltaT);

		uranus.updatePosition(deltaT);
		uranus.updateRotation(deltaT);

		neptune.updatePosition(deltaT);
		neptune.updateRotation(deltaT);

		glm::mat4 WorldM = glm::translate(glm::mat4(1), glm::vec3(0.f, 0.f, -10.f));

		glm::mat4 WorldM_Su =	WorldM *
								glm::scale(glm::mat4(1), glm::vec3(sun_eqradius));

		glm::mat4 WorldM_Me =	glm::translate(glm::mat4(1), mercury.getPosition()) * WorldM *
								glm::rotate(glm::mat4(1), mercury.getRotation(), glm::vec3(-sin(mercury.getTilt()), cos(mercury.getTilt()), 0)) *
								glm::rotate(glm::mat4(1), mercury.getTilt(), glm::vec3(0, 0, 1)) *
								glm::scale(glm::mat4(1), mercury.getSize());
		
		glm::mat4 WorldM_Ve =	glm::translate(glm::mat4(1), venus.getPosition()) * WorldM *
								glm::rotate(glm::mat4(1), venus.getRotation(), glm::vec3(-sin(venus.getTilt()), cos(venus.getTilt()), 0)) *
								glm::rotate(glm::mat4(1), venus.getTilt(), glm::vec3(0, 0, 1)) *
								glm::scale(glm::mat4(1), venus.getSize());
		
		glm::mat4 WorldM_Ea =	glm::translate(glm::mat4(1), earth.getPosition()) * WorldM *
								glm::rotate(glm::mat4(1), earth.getRotation(), glm::vec3(-sin(earth.getTilt()), cos(earth.getTilt()), 0)) *
								glm::rotate(glm::mat4(1), earth.getTilt(), glm::vec3(0, 0, 1)) *
								glm::scale(glm::mat4(1), earth.getSize());
		
		glm::mat4 WorldM_Ma =	glm::translate(glm::mat4(1), mars.getPosition()) * WorldM *
								glm::rotate(glm::mat4(1), mars.getRotation(), glm::vec3(-sin(mars.getTilt()), cos(mars.getTilt()), 0)) *
								glm::rotate(glm::mat4(1), mars.getTilt(), glm::vec3(0, 0, 1)) *
								glm::scale(glm::mat4(1), mars.getSize());
		
		glm::mat4 WorldM_Ju =	glm::translate(glm::mat4(1), jupiter.getPosition()) * WorldM *
								glm::rotate(glm::mat4(1), jupiter.getRotation(), glm::vec3(-sin(jupiter.getTilt()), cos(jupiter.getTilt()), 0)) *
								glm::rotate(glm::mat4(1), jupiter.getTilt(), glm::vec3(0, 0, 1)) *
								glm::scale(glm::mat4(1), jupiter.getSize());
		
		glm::mat4 WorldM_Sa =	glm::translate(glm::mat4(1), saturn.getPosition()) * WorldM *
								glm::rotate(glm::mat4(1), saturn.getRotation(), glm::vec3(-sin(saturn.getTilt()), cos(saturn.getTilt()), 0)) *
								glm::rotate(glm::mat4(1), saturn.getTilt(), glm::vec3(0, 0, 1)) *
								glm::scale(glm::mat4(1), saturn.getSize());
		
		glm::mat4 WorldM_Ur =	glm::translate(glm::mat4(1), uranus.getPosition()) * WorldM *
								glm::rotate(glm::mat4(1), uranus.getRotation(), glm::vec3(-sin(uranus.getTilt()), cos(uranus.getTilt()), 0)) *
								glm::rotate(glm::mat4(1), uranus.getTilt(), glm::vec3(0, 0, 1)) *
								glm::scale(glm::mat4(1), uranus.getSize());
		
		glm::mat4 WorldM_Ne =	glm::translate(glm::mat4(1), neptune.getPosition()) * WorldM *
								glm::rotate(glm::mat4(1), neptune.getRotation(), glm::vec3(-sin(neptune.getTilt()), cos(neptune.getTilt()), 0)) *
								glm::rotate(glm::mat4(1), neptune.getTilt(), glm::vec3(0, 0, 1)) *
								glm::scale(glm::mat4(1), neptune.getSize());

		// std::cout << "rotation = " + std::to_string(earth.getRotation()) << std::endl;

		///////// VIEW MATRIX /////////
		
		glm::mat4 ViewM;
		Planet *zoomedPlanet = controller.getZoomedPlanet();

		if (currAction == ZoomPlanet){

			zoomedPlanet->disableRevolution();

			// The camera is placed between the sun and the planet.
			// The variable dist_perc contains the distance of the camera from the planet in percentage wrt the line connecting the sun with the planet
			
			float dist_perc = 0.9f;

			glm::vec3 cam_a = glm::vec3(			// camera target
				WorldM *
				glm::vec4(zoomedPlanet->getPosition(), 1)
			); 
			glm::vec3 cam_u = glm::vec3(0, 1, 0);	// up vector
			glm::vec3 cam_c = glm::vec3(			// camera center
				WorldM *
				glm::vec4(zoomedPlanet->getPosition() * dist_perc, 1)
			);

			glm::mat4 ViewM_Zoom = glm::lookAt(cam_c, cam_a, cam_u);
			ViewM = ViewM_Zoom;

		}

		else{

			zoomedPlanet->enableRevolution();

			glm::vec3 cam_c = glm::vec3(0.f, 0.f, 0.f); // center of the camera
			float cam_yaw = 0.f;
			float cam_pitch = 0.f;
			float cam_roll = 0.f;
			
			const float CAM_ANG_SPEED = glm::radians(120.0f);
			const float CAM_LIN_SPEED = 4.0f;

			static glm::mat4 ViewM_Reg =	glm::rotate(glm::mat4(1.0f), -cam_roll, glm::vec3(0, 0, 1)) *
					glm::rotate(glm::mat4(1.0f), -cam_pitch, glm::vec3(1, 0, 0)) *
					glm::rotate(glm::mat4(1.0f), -cam_yaw, glm::vec3(0, 1, 0)) *
					glm::translate(glm::mat4(1), -cam_c);

			ViewM_Reg = glm::rotate(glm::mat4(1), CAM_ANG_SPEED * r.x * deltaT, glm::vec3(1, 0, 0)) * ViewM_Reg;
			ViewM_Reg = glm::rotate(glm::mat4(1), CAM_ANG_SPEED * r.y * deltaT, glm::vec3(0, 1, 0)) * ViewM_Reg;
			ViewM_Reg = glm::rotate(glm::mat4(1), CAM_ANG_SPEED * r.z * deltaT, glm::vec3(0, 0, 1)) * ViewM_Reg;
			ViewM_Reg = glm::translate(glm::mat4(1), glm::vec3(
				CAM_LIN_SPEED * -m.x * deltaT,
				CAM_LIN_SPEED * -m.y * deltaT,
				CAM_LIN_SPEED * m.z * deltaT)
				) * ViewM_Reg;

			ViewM = ViewM_Reg;

		}

		///////// PROJECTION MATRIX /////////
	
		const float FOVy = glm::radians(90.0f);
		const float nearPlane = 0.1f;
		const float farPlane = 1000.0f;

		glm::mat4 ProjectionM = glm::perspective(FOVy, Ar, nearPlane, farPlane);
		ProjectionM[1][1] *= -1;

		/*
		const float w = 10, nearPlane = 0.1f, farPlane = 100.f;
		glm::mat4 ParallelM = glm::mat4(
			1.0f / w, 0, 0, 0,
			0, -Ar / w, 0, 0,
			0, 0, 1.0f / (nearPlane - farPlane), 0,
			0, 0, nearPlane / (nearPlane - farPlane), 1
		);
		glm::mat4 T1 = glm::rotate(glm::mat4(1), glm::radians(-45.0f), glm::vec3(0, 1, 0));
		glm::mat4 T2 = glm::rotate(glm::mat4(1), glm::radians(30.0f), glm::vec3(1, 0, 0));
		// glm::mat4 ProjectionM = ParallelM * T2 * T1;
		glm::mat4 ProjectionM = ParallelM;
		*/

		/////// TRANSFORMATIONS APPLICATION ///////
		GlobalUniformBufferObject gubo{};

		gubo.lightPos = vec4tovec3(WorldM_Su*glm::vec4(0, 0, 0, 1));
		gubo.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		gubo.eyePos = vec4tovec3(glm::inverse(ViewM)*glm::vec4(0, 0, 0, 1));

		ubo_Su.mMat = WorldM_Su;
		ubo_Su.mvpMat = ProjectionM * ViewM * ubo_Su.mMat;
		ubo_Su.nMat = glm::inverse(glm::transpose(ubo_Su.mMat));
		DS_Su.map(currentImage, &ubo_Su, sizeof(ubo_Su), 0);
		DS_Su.map(currentImage, &gubo, sizeof(gubo), 1);

		ubo_Me.mMat = WorldM_Me;
		ubo_Me.mvpMat = ProjectionM * ViewM * ubo_Me.mMat;
		ubo_Me.nMat = glm::inverse(glm::transpose(ubo_Me.mMat));
		DS_Me.map(currentImage, &ubo_Me, sizeof(ubo_Me), 0);
		DS_Me.map(currentImage, &gubo, sizeof(gubo), 1);

		ubo_Ve.mMat = WorldM_Ve;
		ubo_Ve.mvpMat = ProjectionM * ViewM * ubo_Ve.mMat;
		ubo_Ve.nMat = glm::inverse(glm::transpose(ubo_Ve.mMat));
		DS_Ve.map(currentImage, &ubo_Ve, sizeof(ubo_Ve), 0);
		DS_Ve.map(currentImage, &gubo, sizeof(gubo), 1);

		ubo_Ea.mMat = WorldM_Ea;
		ubo_Ea.mvpMat = ProjectionM * ViewM * ubo_Ea.mMat;
		ubo_Ea.nMat = glm::inverse(glm::transpose(ubo_Ea.mMat));
		DS_Ea.map(currentImage, &ubo_Ea, sizeof(ubo_Ea), 0);
		DS_Ea.map(currentImage, &gubo, sizeof(gubo), 1);

		ubo_Ma.mMat = WorldM_Ma;
		ubo_Ma.mvpMat = ProjectionM * ViewM * ubo_Ma.mMat;
		ubo_Ma.nMat = glm::inverse(glm::transpose(ubo_Ma.mMat));
		DS_Ma.map(currentImage, &ubo_Ma, sizeof(ubo_Ma), 0);
		DS_Ma.map(currentImage, &gubo, sizeof(gubo), 1);

		ubo_Ju.mMat = WorldM_Ju;
		ubo_Ju.mvpMat = ProjectionM * ViewM * ubo_Ju.mMat;
		ubo_Ju.nMat = glm::inverse(glm::transpose(ubo_Ju.mMat));
		DS_Ju.map(currentImage, &ubo_Ju, sizeof(ubo_Ju), 0);
		DS_Ju.map(currentImage, &gubo, sizeof(gubo), 1);

		ubo_Sa.mMat = WorldM_Sa;
		ubo_Sa.mvpMat = ProjectionM * ViewM * ubo_Sa.mMat;
		ubo_Sa.nMat = glm::inverse(glm::transpose(ubo_Sa.mMat));
		DS_Sa.map(currentImage, &ubo_Sa, sizeof(ubo_Sa), 0);
		DS_Sa.map(currentImage, &gubo, sizeof(gubo), 1);

		ubo_Ur.mMat = WorldM_Ur;
		ubo_Ur.mvpMat = ProjectionM * ViewM * ubo_Ur.mMat;
		ubo_Ur.nMat = glm::inverse(glm::transpose(ubo_Ur.mMat));
		DS_Ur.map(currentImage, &ubo_Ur, sizeof(ubo_Ur), 0);
		DS_Ur.map(currentImage, &gubo, sizeof(gubo), 1);

		ubo_Ne.mMat = WorldM_Ne;
		ubo_Ne.mvpMat = ProjectionM * ViewM * ubo_Ne.mMat;
		ubo_Ne.nMat = glm::inverse(glm::transpose(ubo_Ne.mMat));
		DS_Ne.map(currentImage, &ubo_Ne, sizeof(ubo_Ne), 0);
		DS_Ne.map(currentImage, &gubo, sizeof(gubo), 1);

		txt.updateUniformBuffer(currentImage);
	}

	void createPlanetMesh(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx);

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
