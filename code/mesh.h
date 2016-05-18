#pragma once

// Author(s): Tom

struct Vertex
{
	Vec3 position;
	Vec3 normal;
	Vec2 uv;
};

struct Mesh
{
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	
public:
	inline void Create(std::vector<Vertex> vertices, std::vector<GLuint> indices)
	{
		this->vertices = vertices;
    	this->indices = indices;
		
		glGenVertexArrays(1, &this->vao);
		glBindVertexArray(this->vao);
		
		glGenBuffers(1, &this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		
		glGenBuffers(1, &this->ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
		
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);  
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
		
		// Vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		// Vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		// Vertex UV's
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
		
		glBindVertexArray(0);
	}
	
	inline void Delete()
	{
		glDeleteBuffers(1, &ebo);
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}
	
	inline void Use()
	{
		glBindVertexArray(this->vao);
	}
	
	inline void Unuse()
	{
		glBindVertexArray(0);
	}
	
	inline void Render()
	{
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	}
};

struct Mesh2
{
	std::vector<Vec3> positions;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvs;
	GLuint vao;
	GLuint positionVBO;
	GLuint uvVBO;
	GLuint normalVBO;		
	//GLuint ebo;
};

void Mesh_Create(Mesh2* mesh)
{
	glGenVertexArrays(1, &mesh->vao);
	glBindVertexArray(mesh->vao);
	
	glGenBuffers(1, &mesh->positionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->positionVBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->positions.size() * sizeof(Vec3), &mesh->positions[0], GL_STATIC_DRAW);

	glGenBuffers(1, &mesh->normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->normalVBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->normals.size() * sizeof(Vec3), &mesh->normals[0], GL_STATIC_DRAW);
		
	glGenBuffers(1, &mesh->uvVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->uvVBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->uvs.size() * sizeof(Vec2), &mesh->uvs[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

bool Mesh_LoadOBJ(Mesh2* mesh, const char* fileName)
{
	std::vector<unsigned int> positionIndices;
	std::vector<unsigned int> normalIndices;
	std::vector<unsigned int> uvIndices;
	std::vector<Vec3> positions;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvs;
	FILE* file = fopen(fileName, "r");
	if (file == NULL )
	{
    	printf("Can't open file '%s'!\n", fileName);
    	return false;
	}
	
	
	while (true)
	{
		char line[128];
		if (fscanf(file, "%s", line) == EOF)
			break;
		
		if (strcmp(line, "v") == 0)
		{
			Vec3 position;
			fscanf(file, "%f %f %f\n", &position.x, &position.y, &position.z);
			positions.push_back(position);
		}
		else if (strcmp(line, "vt") == 0)
		{
			Vec2 uv;
			fscanf(file, "%f %f\n", &uv.u, &uv.v);
			uv.v = -uv.v; // For DDS textures
			uvs.push_back(uv);
		}
		else if (strcmp(line, "vn") == 0)
		{
			Vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);	
		}
		else if (strcmp(line, "f") == 0)
		{
			unsigned int positionIndex[3];
			unsigned int normalIndex[3];
			unsigned int uvIndex[3];
			const int numValues = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &positionIndex[0], &uvIndex[0], &normalIndex[0], &positionIndex[1], &uvIndex[1], &normalIndex[1], &positionIndex[2], &uvIndex[2], &normalIndex[2]);
			if (numValues != 9)
			{
				printf("File not readable. Use Assimp or other OBJ options.\n");
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
			char throwaway[1000];
			fgets(throwaway, 1000, file);
		}
	}
	
	const unsigned int numPositions = positionIndices.size();
	for (unsigned int i = 0; i < positionIndices.size(); i++)
	{
		unsigned int positionIndex = positionIndices[i];
		unsigned int normalIndex = normalIndices[i];
		unsigned int uvIndex = uvIndices[i];
		
		Vec3 position = positions[positionIndex - 1];
		Vec3 normal = normals[normalIndex - 1];
		Vec2 uv = uvs[uvIndex - 1];
		
		mesh->positions.push_back(position);
		mesh->uvs.push_back(uv);
		mesh->normals.push_back(normal);
	}
	
	return true;
}

// TODO: Load own format
bool Mesh_LoadOwnFormat(Mesh2* mesh, const char* fileName)
{
	printf("Own mesh file format not implemented yet!\n");
	return false;
}

bool Mesh_LoadFromFile(Mesh2* mesh, const char* fileName)
{
	const int length = strlen(fileName);
	const char* extension = fileName + length - 4;

	if (strcmp(extension, ".obj") == 0)
	{
		return Mesh_LoadOBJ(mesh, fileName);
	}
	else if (strcmp(extension, ".own") == 0)
	{
		return Mesh_LoadOwnFormat(mesh, fileName);
	}
	
	printf("Mesh file format not supported!\n");
	return false;
}

inline void Mesh_Use(Mesh2* mesh)
{
	glBindVertexArray(mesh->vao);
}
	
inline void Mesh_Unuse()
{
	glBindVertexArray(0);
}

inline void Mesh_Render(Mesh2* mesh)
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->positionVBO);	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->normalVBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->uvVBO);	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glDrawArrays(GL_TRIANGLES, 0, mesh->positions.size());
	
	glDisableVertexAttribArray(0);		
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

// TODO: Model struct

// Primitives

struct CubeMesh
{
private:
	GLuint vao;
	GLuint vbo;
	
	GLfloat vertices[288] = 
	{
		// Positions          // Normals           // Texture Coords
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f				
	};

public:
	inline void Create()
	{
		glGenVertexArrays(1, &vao);
    	glGenBuffers(1, &vbo);

    	glBindBuffer(GL_ARRAY_BUFFER, vbo);
    	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    	glBindVertexArray(vao);
    	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    	glEnableVertexAttribArray(0);
    	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    	glEnableVertexAttribArray(1);
    	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    	glEnableVertexAttribArray(2);
    	glBindVertexArray(0);
	}
	
	inline void Delete()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}
	
	inline void Use()
	{
		glBindVertexArray(vao);
	}
	
	inline void Unuse()
	{
		glBindVertexArray(0);
	}
	
	inline void Render()
	{
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
};