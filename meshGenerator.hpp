// Creates a sphere of radius 1 centered in the origin

void SolarSystem3D::createPlanetMesh(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx) {

    auto addVertex = [&vDef](float x, float y, float z, float u_old) {

        // UNIT NORMAL VECTOR

        // the gradient of the sphere is (2x, 2y, 2z)

        float normal_x = 2 * x;
        float normal_y = 2 * y;
        float normal_z = 2 * z;

        float nor_magnitude = sqrt(pow(normal_x, 2) + pow(normal_y, 2) + pow(normal_z, 2));

        //normalize
        float un_nor_x = normal_x / nor_magnitude;
        float un_nor_y = normal_y / nor_magnitude;
        float un_nor_z = normal_z / nor_magnitude;

        // UV COORDINATES

        float point_magnitude = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); // size of the vector corresponding to the vertex

        // normalize
        float unit_point_x = x / point_magnitude;
        float unit_point_y = y / point_magnitude;
        float unit_point_z = z / point_magnitude;

        float u = atan2(unit_point_x, unit_point_z) / (2 * M_PI) + 0.5f;
        float v = unit_point_y * 0.5f + 0.5f;
        v = 1 - v;

        if ((u - u_old) > 0.5f) {
            vDef.push_back({ {x, y, z}, {un_nor_x, un_nor_y, un_nor_z}, {0.f, v} });
            vDef.push_back({ {x, y, z}, {un_nor_x, un_nor_y, un_nor_z}, {1.f, v} });
        }

        vDef.push_back({ {x, y, z}, {un_nor_x, un_nor_y, un_nor_z}, {u, v} });

        return u;

    };

    auto divideCircumference = [&addVertex](int n, float y, float r) {
        float x, z;
        float u_old = 2.f;

        for (int i = 0; i < n; i++) {
            x = r * cos(2 * M_PI * i / n);
            z = r * sin(2 * M_PI * i / n);
            u_old = addVertex(x, y, z, u_old);
        }
    };

    auto connectPole = [&vIdx](int pole_idx, int start_idx, int n_sectors) {
        int i;

        for (i = start_idx; i < start_idx + n_sectors - 1; i++) {
            vIdx.push_back(pole_idx); vIdx.push_back(i); vIdx.push_back(i + 1);
        }
        vIdx.push_back(pole_idx); vIdx.push_back(start_idx); vIdx.push_back(i);
    };

    auto createSector = [&vIdx](int v1, int v2, int v3, int v4) {
        vIdx.push_back(v1); vIdx.push_back(v2); vIdx.push_back(v3);
        vIdx.push_back(v2); vIdx.push_back(v3); vIdx.push_back(v4);
    };

    /*---------------------------------------*/
    ////////////// ADDS VERTICES //////////////
    /*---------------------------------------*/

    int parallels = 599, meridians = 30; // the number of parallels must be ODD
    float radius_precision = 1.f / ceil(parallels / 2);

    float x, y, z; // sphere equation: x^2 + y^2 + z^2 = 1

    // UPPER PART OF THE SPHERE

    addVertex(0.f, 1.f, 0.f, 2.f);

    for (float r = radius_precision; r <= 1.f; r += radius_precision) {
        y = sqrt(1 - pow(r, 2)); // circumference equation: x^2 + z^2 = 1 - y^2 = r^2
        divideCircumference(meridians, y, r);
    }

    // BOTTOM PART OF THE SPHERE

    for (float r = 1 - radius_precision; r >= radius_precision; r -= radius_precision) {
        y = sqrt(1 - pow(r, 2)) * (-1); // circumference equation: x^2 + z^2 = 1 - y^2 = r^2
        divideCircumference(meridians, y, r);
    }

    addVertex(0.f, -1.f, 0.f, 2.f);

    /*---------------------------------------*/
    //////////// CONNECTS VERTICES ////////////
    /*---------------------------------------*/

    int idx, start_idx;
    int n_sectors = meridians + 2;

    connectPole(0, 1, n_sectors);

    for (int i = 0; i < parallels - 1; i++) {
        start_idx = n_sectors * i + 1;
        for (idx = start_idx; idx < start_idx + n_sectors - 1; idx++) {
            createSector(idx, idx + 1, idx + n_sectors, idx + n_sectors + 1);
        }
        createSector(idx, start_idx, idx + n_sectors, start_idx + n_sectors);
    }

    connectPole(n_sectors * parallels + 1, start_idx + n_sectors, n_sectors);

}