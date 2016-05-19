#pragma once

// Author(s): Tom

struct Mesh
{
	std::vector<Vec3> positions;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvs;
	std::vector<unsigned int> indices;
	GLuint vao;
	GLuint positionVBO;
	GLuint uvVBO;
	GLuint normalVBO;		
	GLuint ebo;
};

static bool Mesh_LoadOBJ(Mesh* mesh, const char* fileName)
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
	std::vector<unsigned int> positionIndices;
	std::vector<unsigned int> normalIndices;
	std::vector<unsigned int> uvIndices;
	
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
			unsigned int positionIndex[3];
			unsigned int normalIndex[3];
			unsigned int uvIndex[3];
			const int numValues = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
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
	
	std::vector<Vec3> positions;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvs;
	
	const unsigned int numPositions = positionIndices.size();
	for (unsigned int i = 0; i < numPositions; i++)
	{
		unsigned int positionIndex = positionIndices[i];
		unsigned int normalIndex = normalIndices[i];
		unsigned int uvIndex = uvIndices[i];
		
		Vec3 position = tmpPositions[positionIndex - 1];
		Vec3 normal = tmpNormals[normalIndex - 1];
		Vec2 uv = tmpUVs[uvIndex - 1];
		
		positions.push_back(position);
		uvs.push_back(uv);
		normals.push_back(normal);
	}
	
	const unsigned int size = positions.size();
	for (unsigned int i = 0; i < size; i++)
	{
		mesh->positions.push_back(positions[i]);
		mesh->normals.push_back(normals[i]);
		mesh->uvs.push_back(uvs[i]);
		mesh->indices.push_back(mesh->positions.size() - 1);
	}	
	
	return true;
}

static bool Mesh_LoadQVM(Mesh* mesh, const char* fileName)
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
	std::vector<unsigned int> positionIndices;
	std::vector<unsigned int> normalIndices;
	std::vector<unsigned int> uvIndices;
	
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
			unsigned int positionIndex[3];
			unsigned int uvIndex[3];
			unsigned int normalIndex;
			const int numValues = sscanf(line, "p(v(%d,%d,%d), t(%d,%d,%d), n(%d))\n",
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
	 
	std::vector<Vec3> positions;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvs;
	
	const unsigned int numPositions = positionIndices.size();
	for (unsigned int i = 0; i < numPositions; i++)
	{
		unsigned int positionIndex = positionIndices[i];
		unsigned int normalIndex = normalIndices[i];
		unsigned int uvIndex = uvIndices[i];
		
		Vec3 position = tmpPositions[positionIndex];
		Vec3 normal = tmpNormals[normalIndex];
		Vec2 uv = tmpUVs[uvIndex];
		
		positions.push_back(position);
		uvs.push_back(uv);
		normals.push_back(normal);
	}
	
	const unsigned int size = positions.size();
	for (unsigned int i = 0; i < size; i++)
	{
		mesh->positions.push_back(positions[i]);
		mesh->normals.push_back(normals[i]);
		mesh->uvs.push_back(uvs[i]);
		mesh->indices.push_back(mesh->positions.size() - 1);
	}
	  
	return true;
}

Mesh Mesh_CreateFromFile(const char* fileName)
{
	const int length = strlen(fileName);
	const char* extension = fileName + length - 4;
	Mesh mesh;

	if (strcmp(extension, ".obj") == 0)
	{
		Mesh_LoadOBJ(&mesh, fileName);
	}
	else if (strcmp(extension, ".qvm") == 0)
	{
		Mesh_LoadQVM(&mesh, fileName);
	}
	else
	{
		printf("Mesh file format '%s' not supported!\n", extension);
	}
	
	glGenVertexArrays(1, &mesh.vao);
	glBindVertexArray(mesh.vao);
	
	glGenBuffers(1, &mesh.positionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.positionVBO);
	glBufferData(GL_ARRAY_BUFFER, mesh.positions.size() * sizeof(Vec3), &mesh.positions[0], GL_STATIC_DRAW);

	glGenBuffers(1, &mesh.normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.normalVBO);
	glBufferData(GL_ARRAY_BUFFER, mesh.normals.size() * sizeof(Vec3), &mesh.normals[0], GL_STATIC_DRAW);
		
	glGenBuffers(1, &mesh.uvVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.uvVBO);
	glBufferData(GL_ARRAY_BUFFER, mesh.uvs.size() * sizeof(Vec2), &mesh.uvs[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &mesh.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0] , GL_STATIC_DRAW);

	glBindVertexArray(0);
	
	return mesh;
}

inline void Mesh_Render(Mesh* mesh)
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

	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, (void*) 0);
	
	glDisableVertexAttribArray(0);		
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}