#pragma once
#include "glm/gtx/string_cast.hpp"
#include "Button.hpp"
#include "ScrollButton.hpp"
#include "ScrollKnob.hpp"

const float UI_Scale = .8f;

struct SingleText {
	int start = 0;
	int len = 0;
};

struct CharData {
	int x;
	int y;
	int width;
	int height;
	int xoffset;
	int yoffset;
	int xadvance;
};

struct FontDef {
	int lineHeight;
	std::vector<CharData> P;
};

struct TextVertex {
	glm::vec2 pos;
	glm::vec2 texCoord;
};

struct InstanceData {
	glm::vec2 offset;
	glm::vec2 scale;
	float visible;
	float enabled;
	float hover;
	float active;
};

const std::vector<FontDef> Fonts = {{73,{{0,0,0,0,0,0,21},{116,331,18,61,4,4,21},{379,444,29,30,-1,4,26},{135,331,51,61,-4,4,41},{206,0,46,74,-2,0,41},{320,143,70,62,-2,5,65},{0,143,54,63,-1,4,49},{409,444,18,30,1,4,14},{25,0,27,77,1,3,24},{53,0,28,77,-2,3,24},{345,444,33,30,-1,4,29},{156,444,48,46,-1,19,43},{428,444,18,28,2,48,21},{26,492,29,17,-1,32,24},{56,492,18,17,2,48,21},{478,0,33,61,-4,4,21},{55,143,46,62,-2,5,41},{225,331,29,60,3,5,41},{255,331,46,60,-2,5,41},{102,143,46,62,-2,5,41},{302,331,47,60,-2,5,41},{453,143,46,61,-2,6,41},{149,143,46,62,-2,5,41},{350,331,47,59,-2,6,41},{196,143,46,62,-2,5,41},{243,143,46,62,-2,5,41},{137,444,18,47,4,18,21},{427,331,18,58,4,18,21},{344,393,52,48,-3,18,43},{247,444,48,31,-1,27,43},{397,393,52,48,-3,18,43},{391,143,43,62,1,3,41},
{116,78,68,64,4,3,74},{124,207,57,61,-3,4,49},{182,207,51,61,1,4,49},{360,0,57,64,-1,3,52},{234,207,53,61,2,4,52},{288,207,49,61,2,4,49},{338,207,47,61,2,4,44},{418,0,59,64,-1,3,57},{386,207,53,61,1,4,52},{440,207,18,61,3,4,21},{411,78,41,63,-3,4,36},{0,269,54,61,1,4,49},{55,269,45,61,1,4,41},{101,269,61,61,1,4,60},{163,269,52,61,1,4,52},{0,78,61,64,-1,3,57},{216,269,50,61,2,4,49},{253,0,61,67,-1,3,57},{267,269,54,61,2,4,52},{62,78,53,64,-1,3,49},{459,207,53,61,-3,4,44},{453,78,52,63,2,4,52},{322,269,56,61,-2,4,49},{379,269,77,61,-3,4,68},{0,331,57,61,-3,4,49},{58,331,57,61,-3,4,49},{457,269,52,61,-3,4,44},{154,0,25,76,0,4,21},{187,331,37,61,-7,4,21},{180,0,25,76,-3,4,21},{205,444,41,40,0,4,34},{75,492,51,13,-4,61,41},
{0,492,25,20,-3,3,24},{0,393,46,50,-1,17,41},{185,78,44,63,0,4,41},{184,393,43,49,-2,17,36},{230,78,44,63,-2,4,41},{47,393,46,50,-2,17,41},{290,143,29,62,-3,3,21},{315,0,44,64,-2,17,41},{0,207,42,61,0,4,41},{43,207,18,61,0,4,16},{0,0,24,77,-6,4,16},{62,207,43,61,0,4,36},{106,207,17,61,1,4,16},{446,331,62,48,0,17,60},{228,393,42,48,0,17,41},{94,393,46,50,-2,17,41},{275,78,44,63,0,17,41},{320,78,44,63,-2,17,41},{271,393,29,48,1,17,24},{141,393,42,50,-2,17,36},{398,331,28,59,-3,7,21},{301,393,42,48,0,18,41},{0,444,46,47,-4,18,36},{450,393,62,47,-4,18,52},{47,444,46,47,-4,18,36},{365,78,45,63,-3,18,36},{94,444,42,47,-2,18,36},{82,0,35,77,-6,3,25},{435,143,17,62,1,3,19},{118,0,35,77,-3,3,25},{447,444,53,24,-4,30,43},{296,444,48,31,-1,27,43}}},

{30,{{512,0,0,0,0,0,9},{740,149,11,28,0,0,9},{723,80,16,15,-2,0,11},{600,58,25,28,-3,0,17},{631,103,22,34,-2,-2,17},{512,59,32,28,-2,1,27},{542,147,26,29,-2,0,21},{740,235,11,15,-1,0,6},{723,96,15,34,-1,0,10},{723,131,15,34,-2,0,10},{599,239,17,16,-2,0,12},{600,125,24,22,-2,6,18},{740,35,12,15,-1,18,9},{700,242,16,9,-2,12,10},{740,51,12,10,-1,18,9},{700,213,17,28,-3,0,9},{655,0,22,28,-2,1,17},{655,228,16,27,0,1,17},{631,228,22,27,-2,1,17},{655,29,22,28,-2,1,17},{631,0,23,27,-2,1,17},{655,58,22,28,-2,1,17},{655,87,22,28,-2,1,17},{631,28,23,27,-2,1,17},{655,116,22,28,-2,1,17},{655,145,22,28,-2,1,17},{754,0,11,22,0,6,9},{740,207,11,27,0,6,9},{573,116,26,23,-3,6,18},{600,148,24,15,-2,10,18},{573,140,26,23,-3,6,18},{678,195,21,28,-1,0,17},
{512,29,32,29,0,0,31},{545,59,27,28,-3,0,21},{573,194,25,28,-1,0,21},{545,29,27,29,-2,0,22},{542,177,26,28,-1,0,22},{600,96,24,28,-1,0,21},{599,210,23,28,-1,0,19},{512,224,28,29,-2,0,24},{573,223,25,28,-1,0,22},{740,120,11,28,0,0,9},{678,107,21,29,-3,0,15},{542,206,26,28,-1,0,21},{655,174,22,28,-1,0,17},{512,149,29,28,-1,0,25},{605,0,25,28,-1,0,22},{512,119,29,29,-2,0,24},{600,29,25,28,-1,0,21},{512,88,29,30,-2,0,24},{578,0,26,28,-1,0,22},{542,117,26,29,-2,0,21},{573,29,26,28,-3,0,19},{573,164,25,29,-1,0,22},{573,58,26,28,-2,0,21},{512,0,36,28,-3,0,29},{542,88,27,28,-3,0,21},{549,0,28,28,-3,0,21},{573,87,26,28,-3,0,19},{740,0,13,34,-1,0,9},{700,184,18,28,-4,0,9},{723,202,14,34,-3,0,9},{700,94,20,19,-1,0,15},{600,87,25,8,-3,24,17},
{723,237,14,11,-3,0,10},{631,56,23,23,-2,6,17},{678,47,21,29,-1,0,17},{678,224,21,24,-2,5,15},{631,138,22,29,-2,0,17},{678,0,22,23,-2,6,17},{723,0,16,28,-3,0,9},{631,168,22,29,-2,6,17},{678,137,21,28,-1,0,17},{740,62,11,28,-1,0,7},{723,166,14,35,-4,0,7},{678,166,21,28,-1,0,15},{740,91,11,28,-1,0,7},{512,178,29,22,-1,6,25},{701,24,21,22,-1,6,17},{655,203,22,24,-2,5,17},{678,77,21,29,-1,6,17},{631,198,22,29,-2,6,17},{723,57,16,22,-1,6,10},{701,0,21,23,-2,6,15},{723,29,16,27,-3,2,9},{700,70,20,23,-1,6,17},{631,80,23,22,-3,6,15},{512,201,29,22,-3,6,22},{678,24,22,22,-3,6,15},{599,180,23,29,-3,6,15},{700,47,21,22,-2,6,15},{700,149,18,34,-4,0,11},{740,178,11,28,-1,0,8},{700,114,19,34,-3,0,11},{542,235,26,13,-3,11,18},{599,164,24,15,-2,10,18}}},

{16,{{768,0,0,0,0,0,5},{914,51,8,16,-1,-1,5},{902,17,11,9,-3,-1,6},{825,127,15,16,-3,-1,9},{789,135,14,19,-3,-2,9},{768,17,20,17,-3,-1,15},{768,121,17,17,-3,-1,11},{914,126,8,9,-2,-1,4},{902,27,10,20,-2,-1,6},{902,48,10,20,-3,-1,6},{825,144,12,10,-3,-1,7},{873,87,14,13,-2,2,10},{914,116,8,9,-2,9,5},{842,147,11,7,-3,5,6},{914,136,8,6,-2,9,5},{902,0,11,16,-3,-1,5},{842,21,14,17,-3,-1,9},{858,138,11,16,-2,-1,9},{858,72,14,16,-3,-1,9},{842,39,14,17,-3,-1,9},{825,42,15,16,-3,-1,9},{858,89,14,16,-3,0,9},{842,57,14,17,-3,-1,9},{768,139,15,15,-3,0,9},{842,75,14,17,-3,-1,9},{842,93,14,17,-3,-1,9},{914,102,8,13,-1,2,5},{914,68,8,16,-1,2,5},{807,119,16,14,-3,2,10},{873,101,14,10,-2,4,10},{825,0,16,14,-3,2,10},{888,34,13,16,-2,-1,9},
{768,35,19,17,-1,-1,17},{790,0,17,16,-3,-1,11},{825,59,15,16,-2,-1,11},{768,103,17,17,-3,-1,12},{808,0,16,16,-2,-1,12},{825,76,15,16,-2,-1,11},{825,93,15,16,-2,-1,10},{789,99,16,17,-2,-1,13},{807,17,16,16,-2,-1,12},{914,34,8,16,-2,-1,5},{873,123,13,17,-3,-1,8},{807,34,16,16,-2,-1,11},{858,106,14,16,-2,-1,9},{789,17,17,16,-2,-1,14},{807,51,16,16,-2,-1,12},{768,53,18,17,-3,-1,13},{825,110,15,16,-2,-1,11},{768,71,18,17,-3,-1,13},{807,68,16,16,-2,-1,12},{825,24,15,17,-2,-1,11},{807,85,16,16,-3,-1,10},{789,117,16,17,-2,-1,12},{789,34,17,16,-3,-1,11},{768,0,21,16,-3,-1,16},{789,51,17,16,-3,-1,11},{789,68,17,16,-3,-1,11},{807,102,16,16,-3,-1,10},{902,129,9,20,-2,-1,5},{888,121,12,16,-4,-1,5},{902,69,10,20,-3,-1,5},{888,66,13,12,-2,-1,8},{842,15,15,5,-3,12,9},
{902,121,10,7,-3,-1,6},{842,0,15,14,-3,2,9},{842,111,14,17,-2,-1,9},{858,123,14,14,-3,2,8},{842,129,14,17,-3,-1,9},{873,0,14,14,-3,2,9},{888,138,10,16,-3,-1,5},{858,0,14,17,-3,2,9},{888,0,13,16,-2,-1,9},{914,0,8,16,-2,-1,4},{807,134,10,20,-4,-1,4},{888,17,13,16,-2,-1,8},{914,17,8,16,-2,-1,4},{768,89,18,13,-2,2,14},{873,141,13,13,-2,2,9},{873,15,14,14,-3,2,9},{858,18,14,17,-2,2,9},{858,36,14,17,-3,2,9},{902,107,10,13,-2,2,6},{873,30,14,14,-3,2,8},{902,90,10,16,-3,0,5},{888,51,13,14,-2,2,9},{873,45,14,13,-3,2,8},{789,85,17,13,-3,2,12},{873,59,14,13,-3,2,8},{858,54,14,17,-3,2,8},{873,73,14,13,-3,2,8},{888,79,12,20,-4,-1,6},{914,85,8,16,-2,-1,5},{888,100,12,20,-3,-1,6},{825,15,16,8,-3,5,10},{873,112,14,10,-2,4,10}}}};

struct TextMaker {
	const float textScale=1.0/800.0*UI_Scale;

	float Ar;

	VertexDescriptor VD;
	
	BaseProject *BP;

	DescriptorSetLayout DSL;
	Pipeline PText;
	Pipeline PButtons;
	Model<TextVertex> MText;
	std::vector<Model<TextVertex>> MButtons;
	Texture TText;
	std::vector<Texture> TButtons;
	DescriptorSet DSText;
	std::vector<DescriptorSet> DSButtons;
	
	std::vector<SingleText> Texts;
	std::vector<std::reference_wrapper<Button>> Buttons;
	VkBuffer textBuffer;
	std::vector<VkBuffer> instanceBuffers;
	VkDeviceMemory textBufferMemory;
	std::vector<VkDeviceMemory> instanceBufferMemories;

	void init(BaseProject *_BP, std::vector<std::reference_wrapper<Button>> &_Buttons, float _Ar) {
		BP = _BP;
		Buttons = _Buttons;
		Ar = _Ar;
		for(Button &b: Buttons)
		{
			b.setUIScale(UI_Scale);
			b.setAr(_Ar);
		}
		createTextDescriptorSetAndVertexLayout();
		createTextPipeline();
		createTextModelAndTexture();
		updateInstanceBuffers(true);
	}

	void setAr(float _Ar)
	{
		Ar = _Ar;
		for(Button &b: Buttons)
		{
			b.setAr(_Ar);
		}
	}

	void setButtonText(Button &button, std::string text) {
		if (button.getText() != text) {
			button.setText(text);
			createTextMesh();
			MText.cleanup();
			MText.initMesh(BP, &VD);
		}
	}

	void setButtonsHover(std::vector<bool> hover) {
		int n = 0;
		for (Button &b : Buttons) {
			b.setHover(hover[n]);
			n++;
		}
	}

	void setButtonsActive(std::vector<bool> active) {
		int n = 0;
		for (Button &b : Buttons) {
			b.setActive(active[n]);
			n++;
		}
	}

	void setButtonsVisible(std::vector<bool> visible) {
		int n = 0;
		for (Button &b : Buttons) {
			b.setVisible(visible[n]);
			n++;
		}
	}

	void createTextDescriptorSetAndVertexLayout() {
		VD.init(BP, {
			{0, sizeof(TextVertex), VK_VERTEX_INPUT_RATE_VERTEX},
			{1, sizeof(InstanceData), VK_VERTEX_INPUT_RATE_INSTANCE}
		}, {
			{0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(TextVertex, pos), sizeof(glm::vec2), OTHER},
			{0, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(TextVertex, texCoord), sizeof(glm::vec2), UV},
			{1, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(InstanceData, offset), sizeof(glm::vec2), OTHER},
			{1, 3, VK_FORMAT_R32G32_SFLOAT, offsetof(InstanceData, scale), sizeof(glm::vec2), OTHER},
			{1, 4, VK_FORMAT_R32_SFLOAT, offsetof(InstanceData, visible), sizeof(float), OTHER},
			{1, 5, VK_FORMAT_R32_SFLOAT, offsetof(InstanceData, enabled), sizeof(float), OTHER},
			{1, 6, VK_FORMAT_R32_SFLOAT, offsetof(InstanceData, hover), sizeof(float), OTHER},
			{1, 7, VK_FORMAT_R32_SFLOAT, offsetof(InstanceData, active), sizeof(float), OTHER}
		});
		DSL.init(BP, {
			{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
			{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
		});
	}

 	void createTextPipeline() {
		PText.init(BP, &VD, "shaders/ButtonVert.spv", "shaders/TextFrag.spv", {&DSL});
		PText.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, true, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
		PButtons.init(BP, &VD, "shaders/ButtonVert.spv", "shaders/ButtonFrag.spv", {&DSL});
		PButtons.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, true, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
 	}
	
	void createTextModelAndTexture() {
		createTextMesh();
		createButtons();
		MText.initMesh(BP, &VD);
		for (auto &M : MButtons) {
			M.initMesh(BP, &VD);
		}
		TText.init(BP, "textures/gui/Fonts.png");
		TButtons.resize(buttonInfo.size() * 3);
		int n = 0;
		for (auto const& [type, info] : buttonInfo) {
			TButtons[n].init(BP, (std::string("textures/gui/") + info.textureName + std::string("-L.png")).c_str(), VK_FORMAT_R8G8B8A8_SRGB, false);
			TButtons[n].createTextureSampler(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE); //Removes weird artifact where the texture wraps even when using 0-1 (I guess it's due to the linear interpolation looking for negative pixels)
			TButtons[n + 1].init(BP, (std::string("textures/gui/") + info.textureName + std::string("-C.png")).c_str());
			TButtons[n + 2].init(BP, (std::string("textures/gui/") + info.textureName + std::string("-R.png")).c_str(), VK_FORMAT_R8G8B8A8_SRGB, false);
			TButtons[n + 2].createTextureSampler(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
			n += 3;
		}
	}

	void updateInstanceBuffers(bool init = false) {
		std::vector<InstanceData> instanceData;
		alignButtons();
		updateBoundingBoxes();
		instanceData = getTextInstanceData();
		if (init) {
			createBuffer(&textBuffer, &textBufferMemory, instanceData);
			instanceBuffers.resize(buttonInfo.size() * 3);
			instanceBufferMemories.resize(buttonInfo.size() * 3);
		}
		updateBuffer(&textBufferMemory, instanceData);
		int n = 0;
		for (auto const& [type, info] : buttonInfo) {
			instanceData = getButtonInstanceData(type, 0);
			if (!instanceData.size()) { //No buttons of this type
				continue;
			}
			if (init) {
				createBuffer(&instanceBuffers[n], &instanceBufferMemories[n], instanceData);
			}
			updateBuffer(&instanceBufferMemories[n], instanceData);
			instanceData = getButtonInstanceData(type, 1);
			if (init) {
				createBuffer(&instanceBuffers[n + 1], &instanceBufferMemories[n + 1], instanceData);
			}
			updateBuffer(&instanceBufferMemories[n + 1], instanceData);
			instanceData = getButtonInstanceData(type, 2);
			if (init) {
				createBuffer(&instanceBuffers[n + 2], &instanceBufferMemories[n + 2], instanceData);
			}
			updateBuffer(&instanceBufferMemories[n + 2], instanceData);
			n += 3;
		}
	}

	void createButtons() {
		MButtons.resize(buttonInfo.size()*3);
		int n = 0;
		for (auto const& [type, info] : buttonInfo) {
			createRectMesh(0, 0, info.widthLeft * UI_Scale, info.height * UI_Scale, MButtons[n]);
			createRectMesh(0, 0, info.widthCenter * UI_Scale, info.height * UI_Scale, MButtons[n + 1]);
			createRectMesh(0, 0, info.widthRight * UI_Scale, info.height * UI_Scale, MButtons[n + 2]);
			n += 3;
		}
	}

	template <class Vert>
	void createRectMesh(float x, float y, float width, float height, Model<Vert> &M) {
		TextVertex vertex{};

		vertex.pos = {
			x,
			y
		};
		vertex.texCoord = {
			0.0F,
			0.0F
		};			
		M.vertices.push_back(vertex);
		vertex.pos = {
			x+width,
			y
		};
		vertex.texCoord = {
			1.0F,
			0.0F
		};			
		M.vertices.push_back(vertex);
		vertex.pos = {
			x+width,
			y+height
		};
		vertex.texCoord = {
			1.0F,
			1.0F
		};			
		M.vertices.push_back(vertex);
		vertex.pos = {
			x,
			y+height
		};
		vertex.texCoord = {
			0.0F,
			1.0F
		};			
		M.vertices.push_back(vertex);
		M.indices = {0, 1, 2, 2, 3, 0};
	}

	void createTextMesh() {
		int totLen = 0;
		for(Button &button: Buttons) {
			totLen += button.getText().length();
		}
		std::cout << "Total characters: " << totLen << "\n";

		MText.indices.resize(6 * totLen);
		MText.vertices.clear();
		Texts.clear();

		int FontId = 0;

		int minChar = 32;
		int maxChar = 127;
		int texW = 1024;
		int texH = 512;

		int tpx = 0;

		int ib = 0, k = 0, n = 0;
		for(Button &button: Buttons) {
			SingleText Txt;
			Txt.start = ib;
			for(int j = 0; j < button.getText().length(); j++) {
				int c = ((int)button.getText()[j]) - minChar;
				if((c >= 0) && (c <= maxChar)) {
					//std::cout << k << " " << j << " " << i << " " << ib << " " << c << "\n";
					CharData d = Fonts[FontId].P[c];
					
					TextVertex vertex{};
		
					vertex.pos = {
						(float)(tpx + d.xoffset) * textScale,
						(float)d.yoffset * textScale
					};
					vertex.texCoord = {
						(float)d.x / texW,
						(float)d.y / texH 
					};			
					MText.vertices.push_back(vertex);
			
					vertex.pos = {
						(float)(tpx + d.xoffset + d.width) * textScale,
						(float)d.yoffset * textScale
					};						
					vertex.texCoord = {
						(float)(d.x + d.width) / texW,
						(float)d.y / texH 
					};
					MText.vertices.push_back(vertex);
		
					vertex.pos = {
						(float)(tpx + d.xoffset) * textScale,
						(float)(d.yoffset + d.height) * textScale
					};						
					vertex.texCoord = {
						(float)d.x / texW,
						(float)(d.y + d.height) / texH 
					};
					MText.vertices.push_back(vertex);
		
					vertex.pos = {
						(float)(tpx + d.xoffset + d.width) * textScale,
						(float)(d.yoffset + d.height) * textScale
					};						
					vertex.texCoord = {
						(float)(d.x + d.width) / texW,
						(float)(d.y + d.height) / texH 
					};
					MText.vertices.push_back(vertex);
								
					MText.indices[ib + 0] = 4 * k + 0;
					MText.indices[ib + 1] = 4 * k + 1;
					MText.indices[ib + 2] = 4 * k + 2;
					MText.indices[ib + 3] = 4 * k + 1;
					MText.indices[ib + 4] = 4 * k + 2;
					MText.indices[ib + 5] = 4 * k + 3;

					ib += 6;
					tpx += d.xadvance;
					k++;
				}
			}
			Txt.len = ib - Txt.start;
			const glm::vec2 textSize(tpx*textScale,textScale*Fonts[FontId].lineHeight);
			button.setTextSize(textSize);
			button.setMinSize({std::max({button.getMinSize().x,textSize.x}),std::max(button.getMinSize().y,textSize.y)});
			Texts.push_back(Txt);
			n++;
			tpx = 0;
		}		
		std::cout << "Text: " << MText.vertices.size()
					<< ", I: " << MText.indices.size() << "\n";
	}

	void createTextDescriptorSets() {
		DSText.init(BP, &DSL, {
					{0, UNIFORM, sizeof(float), nullptr},
					{1, TEXTURE, 0, &TText}
				});
		DSButtons.resize(buttonInfo.size() * 3);
		int n = 0;
		for (auto &DSB : DSButtons) {
			DSB.init(BP, &DSL, {
					{0, UNIFORM, sizeof(float), nullptr},
					{1, TEXTURE, 0, &TButtons[n]}
				});
			n++;
		}
	}

	void pipelinesAndDescriptorSetsInit() {
		PText.create();
		PButtons.create();
		createTextDescriptorSets();
	}
	
	void pipelinesAndDescriptorSetsCleanup() {
		PText.cleanup();
		PButtons.cleanup();
		DSText.cleanup();
		for (auto &DSB : DSButtons) {
			DSB.cleanup();
		}
	}

	void localCleanup() {
		TText.cleanup();
		for (auto &T : TButtons) {
			T.cleanup();
		}
		MText.cleanup();
		for (auto &M : MButtons) {
			M.cleanup();
		}
		DSL.cleanup();
		PText.destroy();
		PButtons.destroy();
		vkDestroyBuffer(BP->device, textBuffer, nullptr);
		for (auto &B : instanceBuffers) {
			vkDestroyBuffer(BP->device, B, nullptr);
		}
		vkFreeMemory(BP->device, textBufferMemory, nullptr);
		for (auto &M : instanceBufferMemories) {
			vkFreeMemory(BP->device, M, nullptr);
		}
	}

	void updateUniformBuffer(int currentImage) {
		bool update = false;
		for (Button &b : Buttons) {
			update |= b.needsUpdate();
		}
		if (update) {
			updateInstanceBuffers();
		}
		DSText.map(currentImage, &Ar, sizeof(Ar), 0);
		for (auto &DSB : DSButtons) {
			DSB.map(currentImage, &Ar, sizeof(Ar), 0);
		}
	}

	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage, int curText = 0) {
		VkDeviceSize offsets[1] = { 0 };
		//TODO remove when sure everything works
		if(std::min({MButtons.size(),instanceBuffers.size(),instanceBufferMemories.size(),DSButtons.size(),TButtons.size(),buttonInfo.size()*3})!=std::max({MButtons.size(),instanceBuffers.size(),instanceBufferMemories.size(),DSButtons.size(),TButtons.size(),buttonInfo.size()*3}))
		{
			std::vector<size_t> f={MButtons.size(),instanceBuffers.size(),instanceBufferMemories.size(),DSButtons.size(),TButtons.size(),buttonInfo.size()*3};
			for(size_t fl: f)
			{
				std::cout << fl << " ";
			}
			std::cout << std::endl;
			exit(1);
		}
		PButtons.bind(commandBuffer);
		for (int n = 0; n < buttonInfo.size() * 3; n++) {
			if (instanceBufferMemories[n] == nullptr) { //No buttons of this type
				continue;
			}
			MButtons[n].bind(commandBuffer);
			vkCmdBindVertexBuffers(commandBuffer, 1, 1, &instanceBuffers[n], offsets);
			DSButtons[n].bind(commandBuffer, PText, 0, currentImage);
			vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(MButtons[n].indices.size()), Buttons.size(), 0, 0, 0);
		}
		PText.bind(commandBuffer);
		MText.bind(commandBuffer);
		DSText.bind(commandBuffer, PText, 0, currentImage);
		vkCmdBindVertexBuffers(commandBuffer, 1, 1, &textBuffer, offsets);
		int n=0;
		for(SingleText t: Texts)
		{
			vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(t.len), 1, static_cast<uint32_t>(t.start), 0, n);
			n++;
		}
	}

	template<class Any>
	void createBuffer(VkBuffer *instanceBuffer, VkDeviceMemory *instanceBufferMemory, std::vector<Any> instanceData)
	{
		VkDeviceSize bufferSize = sizeof(instanceData[0]) * instanceData.size();
		BP->createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
							VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
							VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
							*instanceBuffer, *instanceBufferMemory);

	}

	template<class Any>
	void updateBuffer(VkDeviceMemory *instanceBufferMemory, std::vector<Any> instanceData)
	{
		void* data;
		VkDeviceSize bufferSize = sizeof(instanceData[0]) * instanceData.size();
		vkMapMemory(BP->device, *instanceBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, instanceData.data(), (size_t) bufferSize);
		vkUnmapMemory(BP->device, *instanceBufferMemory);			
	}

	std::vector<InstanceData> getTextInstanceData() {
		std::vector<InstanceData> instanceData;
		Geometry geometry;
		for (Button &b : Buttons) {
			geometry = b.getTextGeometry();
			instanceData.push_back( { geometry.offset, geometry.scale, b.isVisible() ? 1.0f : 0.0f, b.isEnabled() ? 1.0f : 0.0f, b.isHover() ? 1.0f : 0.0f, b.isActive() ? 1.0f : 0.0f });
		}
		return instanceData;
	}

	std::vector<InstanceData> getButtonInstanceData(ButtonType type, int side) {
		std::vector<InstanceData> instanceData;
		Geometry geometry;
		for (Button &b : Buttons) {
			if (b.getType() == type) {
				geometry = b.getSideGeometry(side);
				instanceData.push_back( {geometry.offset, geometry.scale, b.isVisible() ? 1.0f : 0.0f, b.isEnabled() ? 1.0f : 0.0f, b.isHover() ? 1.0f : 0.0f, b.isActive() ? 1.0f : 0.0f});
			}
		}
		return instanceData;
	}
	
	void alignButtons() {
		for (Button &b : Buttons) {
			b.align(true);
		}
	}

	void updateBoundingBoxes() {
		for (Button &b : Buttons) {
			b.updateBoundingBox();
		}
	}
};
