// Creates a sphere of radius 1 centered in the origin

void SolarSystem3D::createPlanetMesh(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx) {

    auto addVertex = [&vDef](float x, float y, float z, float u2 = -1) {

    	float u;
    	if(u2==-1)
    	{
    		u=atan2(z, -x) / (2 * M_PI) + 0.5f;
    	}
    	else
    	{
    		u=1-u2;
    	}
        float v = acos(y) / M_PI - 1.f;

        vDef.push_back({ {x, y, z}, {x, y, z}, {u, v} });

        return u;

    };

    auto divideCircumference = [&addVertex](int n, float y, float r) {
        float x, z;

        for (int i = 0; i <= n; i++) {
            x = r * cos(2 * M_PI * i / n);
            z = r * sin(2 * M_PI * i / n);
            addVertex(x, y, z, (float)i/n);
        }
    };

    auto connectPole = [&vIdx](int pole_idx, int start_idx, int n_sectors, bool swap) {
        int i;

        for (i = 0; i < n_sectors - 1; i++) {
            vIdx.push_back(pole_idx+i); vIdx.push_back(start_idx+i+1*(1-swap)); vIdx.push_back(start_idx+i+1*swap);
        }
        vIdx.push_back(pole_idx+i); vIdx.push_back(start_idx+i*swap); vIdx.push_back(start_idx+i*(1-swap));
    };

    auto createSector = [&vIdx](int v1, int v2, int v3, int v4) {
        vIdx.push_back(v1); vIdx.push_back(v2); vIdx.push_back(v3);
        vIdx.push_back(v3); vIdx.push_back(v2); vIdx.push_back(v4);
    };

    /*---------------------------------------*/
    ////////////// ADDS VERTICES //////////////
    /*---------------------------------------*/

    int parallels = 499, meridians = 30; // the number of parallels must be ODD

    float x, y, z, r; // sphere equation: x^2 + y^2 + z^2 = 1

    // UPPER PART OF THE SPHERE

    for(int n=0;n<=meridians;n++)
    {
    	addVertex(0.f, 1.f, 0.f, (float)n/meridians);
    }

    for (int n = 1; n <= ceil(parallels/2); n += 1) {
        r = n / ceil(parallels / 2);
    	y = sqrt(1 - pow(r, 2)); // circumference equation: x^2 + z^2 = 1 - y^2 = r^2
        divideCircumference(meridians, y, r);
    }

    // BOTTOM PART OF THE SPHERE

    for (int n = ceil(parallels/2)-1; n > 0; n -= 1) {
        r = n / ceil(parallels / 2);
        y = sqrt(1 - pow(r, 2)) * (-1); // circumference equation: x^2 + z^2 = 1 - y^2 = r^2
        divideCircumference(meridians, y, r);
    }

    for(int n=0;n<=meridians;n++)
    {
    	addVertex(0.f, -1.f, 0.f, (float)n/meridians);
    }

    /*---------------------------------------*/
    //////////// CONNECTS VERTICES ////////////
    /*---------------------------------------*/

    int idx, start_idx;
    int n_sectors = meridians + 1;

    connectPole(0, n_sectors, n_sectors,0);

    for (int i = 1; i < parallels - 2; i++) {
        start_idx = n_sectors * i;
        for (idx = start_idx; idx < start_idx + n_sectors - 1; idx++) {
            createSector(idx, idx + 1, idx + n_sectors, idx + n_sectors + 1);
        }
        createSector(idx, start_idx, idx + n_sectors, start_idx + n_sectors);
    }

    connectPole(start_idx + 2*n_sectors, start_idx + n_sectors, n_sectors,1);

}

/** Creates a ring mesh, with the specified inner radius and outer radius 1
 * The texture represents a horizontal section of the ring, to be repeated on the y axis
 * */
void SolarSystem3D::createRingMesh(float innerRadius, float textureRatio, std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	int len = 1000;
	float angle, v;
	float finalV = 2.f * M_PI * textureRatio;
	textureRatio *= std::roundf(finalV) / finalV; //Fix texture repetition: if last V is 9.3, make it 9
	for (int n = 0; n <= len; n++) {
		angle = (float) n / len * 2 * M_PI;
		v = angle * textureRatio;
		vDef.push_back({{cos(angle), 0, sin(angle)}, {0, 1, 0}, {1, v}});
		vDef.push_back({{innerRadius * cos(angle), 0, innerRadius * sin(angle)}, {0, 1, 0}, {0, v}});
		vIdx.push_back(n * 2);
		vIdx.push_back(n * 2 + 1);
		vIdx.push_back((n * 2 + 3) % ((len+1) * 2));
		vIdx.push_back((n * 2 + 3) % ((len+1) * 2));
		vIdx.push_back((n * 2 + 2) % ((len+1) * 2));
		vIdx.push_back(n * 2);
	}
}

/** Creates a circumference mesh with radius 1 */
void SolarSystem3D::createOrbitMesh(std::vector<RawVertex> &vDef, std::vector<uint32_t> &vIdx) {
	int len=200;
	for(int n=0;n<len;n++)
	{
		vDef.push_back({{cos((float)n/len*2*M_PI),0,sin((float)n/len*2*M_PI)}});
		vIdx.push_back(n);
	}
	vIdx.push_back(0);
}

/** Creates a cube mesh with side 1 */
void SolarSystem3D::createSkyboxMesh(std::vector<RawVertex> &vDef, std::vector<uint32_t> &vIdx) {
	vDef = {
			{{1, 1, 1}},
			{{-1, 1, 1}},
			{{1, -1, 1}},
			{{-1, -1, 1}},
			{{1, 1, -1}},
			{{-1, 1, -1}},
			{{-1, -1, -1}},
			{{1, -1, -1}},
	};
	vIdx = {3, 2, 6, 7, 4, 2, 0, 3, 1, 6, 5, 4, 1, 0};
}


void SolarSystem3D::createTextMesh(std::vector<TexturedVertex>& vDef, std::vector<uint32_t>& vIdx) {

    int texture_width = 1280;
    int texture_height = 720;

    float mesh_width = 2.f;
    float mesh_height = mesh_width * texture_height / texture_width;

    vDef.push_back({ {- mesh_width / 2, mesh_height / 2, 0}, {0, 0} });
    vDef.push_back({ {mesh_width / 2, mesh_height / 2, 0}, {1, 0} });
    vDef.push_back({ {- mesh_width / 2, - mesh_height / 2, 0}, {0, 1} });
    vDef.push_back({ {mesh_width / 2, - mesh_height / 2, 0}, {1, 1} });

    vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);
    vIdx.push_back(1); vIdx.push_back(2); vIdx.push_back(3);

}
