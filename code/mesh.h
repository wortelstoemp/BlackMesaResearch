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