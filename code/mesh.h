#pragma once

// Author(s): Tom
struct SimpleSpriteMesh
{
private:
	// TODO(Tom): Vec3 color member?
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	
	GLfloat vertices[32] = {
		// Positions		Colors				Texture Coordinates
		1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 1.0f,	// Top Right
		-1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 1.0f,	0.0f, 1.0f,	// Top Left
		-1.0f, -1.0f, 0.0f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f,	// Bottom Left
		1.0f, -1.0f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 0.0f	// Bottom Right	
	};
	
	GLuint indices[6] = {
		0, 1, 2,	// First Triangle
		2, 3, 0		// Second Triangle
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
	
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
	
		// Texture attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		
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
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
};