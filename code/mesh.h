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
		// Vertex Texture Coordinates
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

// TODO: Model struct

// Primitives

struct CubeMesh
{
	private:
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	
	GLfloat vertices[48] = 
	{
		// Positions			Normals
		-1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	// 0 (Top)
		1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	// 1
		1.0f, 1.0f, -1.0f,		0.0f, 1.0f, 0.0f,	// 2	 
		-1.0f, 1.0f, -1.0f,		0.0f, 1.0f, 0.0f,	// 3	
		-1.0f, -1.0f, 1.0f,		0.0f, -1.0f, 0.0f,	// 4 (Bottom)
		1.0f, -1.0f, 1.0f,		0.0f, -1.0f, 0.0f,	// 5
		1.0f, -1.0f, -1.0f,		0.0f, -1.0f, 0.0f,	// 6
		-1.0f, -1.0f, -1.0f,	0.0f, -1.0f, 0.0f,	// 7				
	};
	
	GLuint indices[12] = 
	{
		0, 1, 2,	// Top Triangle 1
		2, 3, 0,	// Top Triangle 2
		4, 5, 6,	// Bottom Triangle 1
		6, 7, 4,	// Bottom Triangle 2
	};

public:
	inline void Create()
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		
		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
	
		// Normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
	
		// Texture attribute
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		//glEnableVertexAttribArray(2);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
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
		glBindVertexArray(vao);
	}
	
	inline void Unuse()
	{
		glBindVertexArray(0);
	}
	
	inline void Render()
	{
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	}
};