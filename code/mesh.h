#pragma once

// Author(s): Tom

struct Mesh
{
	GLuint vao;
	GLuint positionVBO;
	GLuint uvVBO;
	GLuint normalVBO;
	GLuint ebo;
	int32 vertexIndexCount;
};

struct Meshdata
{
	std::vector<Vec3> positions;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvs;
	std::vector<uint32> indices;
};

static bool MeshLoadOBJ(Mesh* mesh, Meshdata* meshdata, const char* fileName)
{
	FILE* file = fopen(fileName, "r");
	if (file == NULL )
	{
    	printf("Can't open file '%s'!\n", fileName);
    	return false;
	}

	std::vector<Vec3> tmpPositions;
	std::vector<Vec3> tmpNormals;
	std::vector<Vec2> tmpUVs;
	std::vector<uint32> positionIndices;
	std::vector<uint32> normalIndices;
	std::vector<uint32> uvIndices;

	while (true)
	{
		char line[1024];
		if (fscanf(file, "%s", line) == EOF)
			break;

		if (strcmp(line, "v") == 0)
		{
			Vec3 position;
			fscanf(file, "%f %f %f\n", &position.x, &position.y, &position.z);
			tmpPositions.push_back(position);
		}
		else if (strcmp(line, "vt") == 0)
		{
			Vec2 uv;
			fscanf(file, "%f %f\n", &uv.u, &uv.v);
			uv.v = 1 - uv.v; // For DDS textures
			tmpUVs.push_back(uv);
		}
		else if (strcmp(line, "vn") == 0)
		{
			Vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			tmpNormals.push_back(normal);
		}
		else if (strcmp(line, "f") == 0)
		{
			uint32 positionIndex[3];
			uint32 normalIndex[3];
			uint32 uvIndex[3];
			const int32 numValues = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&positionIndex[0], &uvIndex[0], &normalIndex[0],
				&positionIndex[1], &uvIndex[1], &normalIndex[1],
				&positionIndex[2], &uvIndex[2], &normalIndex[2]);

			if (numValues != 9)
			{
				printf("File not readable. Use Assimp or other OBJ options.\n");
				fclose(file);
				return false;
			}

			positionIndices.push_back(positionIndex[0]);
			positionIndices.push_back(positionIndex[1]);
			positionIndices.push_back(positionIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
		}
		else
		{
			// Comment or other nonsense
			char throwaway[1024];
			fgets(throwaway, 1024, file);
		}
	}
	fclose(file);

	const uint32 numPositions = positionIndices.size();
	for (uint32 i = 0; i < numPositions; i++)
	{
		uint32 positionIndex = positionIndices[i];
		uint32 normalIndex = normalIndices[i];
		uint32 uvIndex = uvIndices[i];

		meshdata->positions.push_back(tmpPositions[positionIndex - 1]);
		meshdata->normals.push_back(tmpNormals[normalIndex - 1]);
		meshdata->uvs.push_back(tmpUVs[uvIndex - 1]);
		meshdata->indices.push_back(i);
	}

	mesh->vertexIndexCount = meshdata->positions.size();

	return true;
}

static bool MeshLoadQVM(Mesh* mesh, Meshdata* meshdata, const char* fileName)
{
	FILE* file = fopen(fileName, "r");
	if (file == NULL )
	{
    	printf("Can't open file '%s'!\n", fileName);
    	return false;
	}

	std::vector<Vec3> tmpPositions;
	std::vector<Vec3> tmpNormals;
	std::vector<Vec2> tmpUVs;
	std::vector<uint32> positionIndices;
	std::vector<uint32> normalIndices;
	std::vector<uint32> uvIndices;

	char line[1024];
  	while (fgets(line, sizeof(line), file))
	{
		if (strncmp(line, "v", 1) == 0)
		{
			Vec3 position;
			sscanf(line, "v(%f,%f,%f)\n", &position.x, &position.y, &position.z);
			tmpPositions.push_back(position);
		}
		else if (strncmp(line, "t", 1) == 0)
		{
			Vec2 uv;
			sscanf(line, "t(%f,%f)\n", &uv.u, &uv.v);
			uv.v = 1 - uv.v; // For DDS textures
			tmpUVs.push_back(uv);
		}
		else if (strncmp(line, "n", 1) == 0)
		{
			Vec3 normal;
			sscanf(line, "n(%f,%f,%f)\n", &normal.x, &normal.y, &normal.z);
			tmpNormals.push_back(normal);
		}
		else if (strncmp(line, "p", 1) == 0)
		{
			uint32 positionIndex[3];
			uint32 uvIndex[3];
			uint32 normalIndex;
			const int32 numValues = sscanf(line, "p(v(%d,%d,%d), t(%d,%d,%d), n(%d))\n",
				&positionIndex[0], &positionIndex[1], &positionIndex[2],
				&uvIndex[0], &uvIndex[1], &uvIndex[2],
				&normalIndex);

			if (numValues != 7)
			{
				printf("File '%s' not readable.\n", fileName);
				fclose(file);
				return false;
			}

			positionIndices.push_back(positionIndex[0]);
			positionIndices.push_back(positionIndex[1]);
			positionIndices.push_back(positionIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex);
			normalIndices.push_back(normalIndex);
			normalIndices.push_back(normalIndex);
		}
  	}
	fclose(file);

	const uint32 numPositions = positionIndices.size();
	for (uint32 i = 0; i < numPositions; i++)
	{
		uint32 positionIndex = positionIndices[i];
		uint32 normalIndex = normalIndices[i];
		uint32 uvIndex = uvIndices[i];

		meshdata->positions.push_back(tmpPositions[positionIndex]);
		meshdata->normals.push_back(tmpNormals[normalIndex]);
		meshdata->uvs.push_back(tmpUVs[uvIndex]);
		meshdata->indices.push_back(i);
	}
	mesh->vertexIndexCount = meshdata->positions.size();

	return true;
}

Mesh MeshCreateFromFile(const char* fileName, Vec3 offset)
{
	const int32 length = strlen(fileName);
	const char* extension = fileName + length - 4;
	Mesh mesh;
	Meshdata meshdata;

	if (strcmp(extension, ".obj") == 0)
	{
		MeshLoadOBJ(&mesh, &meshdata, fileName);
	}
	else if (strcmp(extension, ".qvm") == 0)
	{
		MeshLoadQVM(&mesh, &meshdata, fileName);
	}
	else
	{
		printf("Mesh file format '%s' not supported!\n", extension);
	}

	if (offset.x != 0 || offset.y != 0 || offset.z != 0)
	{
		int32 vertCount = meshdata.positions.size();
		for (int32 i = 0; i < vertCount; i++)
		{
			meshdata.positions[i] = meshdata.positions[i] + offset;
		}
	}

	glGenVertexArrays(1, &mesh.vao);
	glBindVertexArray(mesh.vao);

	glGenBuffers(1, &mesh.positionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.positionVBO);
	glBufferData(GL_ARRAY_BUFFER, meshdata.positions.size() * sizeof(Vec3), &meshdata.positions[0], GL_STATIC_DRAW);

	glGenBuffers(1, &mesh.normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.normalVBO);
	glBufferData(GL_ARRAY_BUFFER, meshdata.normals.size() * sizeof(Vec3), &meshdata.normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &mesh.uvVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.uvVBO);
	glBufferData(GL_ARRAY_BUFFER, meshdata.uvs.size() * sizeof(Vec2), &meshdata.uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &mesh.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshdata.indices.size() * sizeof(uint32), &meshdata.indices[0] , GL_STATIC_DRAW);

	glBindVertexArray(0);

	return mesh;
}

Mesh MeshCreateFromFile(const char* fileName)
{
	return MeshCreateFromFile(fileName, {0, 0, 0});
}

inline void MeshRender(Mesh* mesh)
{
	glBindVertexArray(mesh->vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->positionVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->normalVBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->uvVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);

	glDrawElements(GL_TRIANGLES, mesh->vertexIndexCount, GL_UNSIGNED_INT, (void*) 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}
