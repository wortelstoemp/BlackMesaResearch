#pragma once

// Author(s): Simon, Tom

// TODO(Tom): Make simpeler API usage code?
// Usage
// DefaultShader shader;
// char* vertexSource = ReadFile("../data/shaders/default_vs.glsl");
// char* fragmentSource = ReadFile("../data/shaders/default_fs.glsl");
// shader.Create(vertexSource, 0, 0, 0, fragmentSource);
// FreeFile(vertexSource);
// FreeFile(fragmentSource);
// shader.Init();
// ...
// shader.Use();
// shader.Update(transform, camera, deltaTime);
// ...
// shader.Unuse();
// ...
// shader.Delete();

struct ShaderUniform
{
	const char* name;	
	GLint id;
};

struct Shader
{
	GLuint shaderProgram;

	void CompileAndAttachShader(int programID, char** source, GLenum shaderType) const
	{
		if (*source == 0)
		{
			return;
		}
	
		int size = strlen(*source);
		int shaderID = glCreateShader(shaderType);
		glShaderSource(shaderID, 1, source, &size);
		glCompileShader(shaderID);

		int compiled;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
		if (compiled != GL_TRUE)
		{
			GLsizei log_length = 0;
			GLchar message[1024];
			glGetShaderInfoLog(shaderID, 1024, &log_length, message);
			printf("%s\n", message);
		}

		glAttachShader(programID, shaderID);
		glDeleteShader(shaderID);
	}
	
	virtual void Init() = 0;
	virtual void Update(const Transform& transform, const Camera& camera, float deltaTime) = 0;

	void Create(
		char* vertexSource,
		char* tessControlSource,
		char* tessEvalSource,
		char* geometrySource,
		char* fragmentSource)
	{
		shaderProgram = glCreateProgram();
	
		CompileAndAttachShader(shaderProgram, &vertexSource, GL_VERTEX_SHADER);
		CompileAndAttachShader(shaderProgram, &tessControlSource, GL_TESS_CONTROL_SHADER);
		CompileAndAttachShader(shaderProgram, &tessEvalSource, GL_TESS_EVALUATION_SHADER);
		CompileAndAttachShader(shaderProgram, &geometrySource, GL_GEOMETRY_SHADER);
		CompileAndAttachShader(shaderProgram, &fragmentSource, GL_FRAGMENT_SHADER);

		glLinkProgram(shaderProgram);

		int linked;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);
		if (linked != GL_TRUE)
		{
			GLsizei log_length = 0;
			GLchar message[1024];
			glGetProgramInfoLog(shaderProgram, 1024, &log_length, message);
			printf("%s\n", message);
		}
	}

	inline void Delete()
	{
		glDeleteProgram(shaderProgram);
	}

	inline void Use()
	{
		glUseProgram(shaderProgram);
	}
	
	inline void Unuse()
	{
		glUseProgram(0);		
	}
	
	inline void AddUniform(ShaderUniform* uniform)
	{
		uniform->id = glGetUniformLocation(shaderProgram, (const GLchar*)uniform->name);
		if (uniform->id == -1)
		{
			printf("Problem adding uniform: %s!\n", uniform->name);
		}
	}
	
	inline void SetUniform(const ShaderUniform& uniform, const bool value)
	{
		glUniform1i(uniform.id, (GLint)(value ? 1 : 0));
	}
	
	inline void SetUniform(const ShaderUniform& uniform, const int value)
	{
		glUniform1i(uniform.id, (GLint)value);
	}
	
	inline void SetUniform(const ShaderUniform& uniform, const unsigned int value)
	{
		glUniform1ui(uniform.id, (GLuint)value);
	}
	
	inline void SetUniform(const ShaderUniform& uniform, const float value)
	{
		glUniform1f(uniform.id, (GLfloat)value);
	}
	
	inline void SetUniform(const ShaderUniform& uniform, const Vec2& value)
	{
		glUniform2f(uniform.id, (GLfloat)value.X, (GLfloat)value.Y);
	}
	
	inline void SetUniform(const ShaderUniform& uniform, const Vec3& value)
	{
		glUniform3f(uniform.id, (GLfloat)value.X, (GLfloat)value.Y, (GLfloat)value.Z);
	}
	
	inline void SetUniform(const ShaderUniform& uniform, const Vec4& value)
	{
		glUniform4f(uniform.id, (GLfloat)value.X, (GLfloat)value.Y, (GLfloat)value.Z, (GLfloat)value.W);
	}
	
	inline void SetUniform(const ShaderUniform& uniform, const Matrix4x4& value)
	{
		glUniformMatrix4fv(uniform.id, 1, GL_TRUE, (GLfloat*)value.values);
	}
};

// Basic way to create new shaders.
// This is the default shader.
struct DefaultShader : public Shader
{
	ShaderUniform transformUniform;
	
	void Init()
	{
		transformUniform.name = "transform";
		AddUniform(&transformUniform);
	}
	
	void Update(const Transform& transform, const Camera& camera, float deltaTime)
	{
		SetUniform(transformUniform, CalculateMVP(transform, camera));
	}
};