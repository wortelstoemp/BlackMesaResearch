#pragma once

// Author(s): Simon, Tom

// API Usage code:
// Shader shader;
// shader.LoadFromFiles("../data/shaders/default_vs.glsl", 0, 0, 0, "../data/shaders/default_fs.glsl");
// DefaultShader_Init(&shader);
// ...
// shader.Use();
// DefaultShader_Update(&shader, transform, camera);
// ...
// shader.Unuse();
// ...
// shader.Delete();

// Look at DefaultShader_ and PhongShader_ functions to create your own shaders

struct Shader
{
	GLuint program;
	std::vector<GLint> uniforms;
	std::vector<const char*> uniformNames;
	int numUniforms;

	void CompileAndAttachShader(int programID, char** source, GLenum shaderType) const
	{
		if (*source == 0)
			return;

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

	void Load(
		char* vertexSource,
		char* tessControlSource,
		char* tessEvalSource,
		char* geometrySource,
		char* fragmentSource)
	{
		this->program = glCreateProgram();
		this->numUniforms = 0;

		CompileAndAttachShader(this->program, &vertexSource, GL_VERTEX_SHADER);
		CompileAndAttachShader(this->program, &tessControlSource, GL_TESS_CONTROL_SHADER);
		CompileAndAttachShader(this->program, &tessEvalSource, GL_TESS_EVALUATION_SHADER);
		CompileAndAttachShader(this->program, &geometrySource, GL_GEOMETRY_SHADER);
		CompileAndAttachShader(this->program, &fragmentSource, GL_FRAGMENT_SHADER);

		glLinkProgram(this->program);

		int linked;
		glGetProgramiv(this->program, GL_LINK_STATUS, &linked);
		if (linked != GL_TRUE)
		{
			GLsizei log_length = 0;
			GLchar message[1024];
			glGetProgramInfoLog(this->program, 1024, &log_length, message);
			printf("%s\n", message);
		}
	}

	void LoadFromFiles(
		char* vertexFile,
		char* tessControlFile,
		char* tessEvalFile,
		char* geometryFile,
		char* fragmentFile)
	{
		char* vertexSource = 0;
		char* tessControlSource = 0;
		char* tessEvalSource = 0;
		char* geometrySource = 0;
		char* fragmentSource = 0;

		if (vertexFile)
		{
			vertexSource = ReadFile(vertexFile);
			Assert(vertexSource);
		}
		if (tessControlFile)
		{
			tessControlSource = ReadFile(tessControlFile);
			Assert(tessControlSource);
		}
		if (tessEvalFile)
		{
			tessEvalSource = ReadFile(tessEvalFile);
			Assert(tessEvalSource);
		}
		if (geometryFile)
		{
			geometrySource = ReadFile(geometryFile);
			Assert(geometrySource);
		}
		if (fragmentFile)
		{
			fragmentSource = ReadFile(fragmentFile);
			Assert(fragmentSource);
		}

		Load(vertexSource, tessControlSource, tessEvalSource, geometrySource, fragmentSource);

		if (vertexSource)
			FreeFile(vertexSource);
		if (tessControlSource)
			FreeFile(tessControlSource);
		if (tessEvalSource)
			FreeFile(tessEvalSource);
		if (geometrySource)
			FreeFile(geometrySource);
		if (fragmentSource)
			FreeFile(fragmentSource);
	}

	inline void Delete()
	{
		glDeleteProgram(program);
	}

	inline void Use()
	{
		glUseProgram(program);
	}

	inline void Unuse()
	{
		glUseProgram(0);
	}

	inline bool AddUniform(const char* uniformName)
	{
		GLint uniform = glGetUniformLocation(this->program, (const GLchar*)uniformName);

		if (uniform == -1)
		{
			printf("Problem adding uniform: \"%s\" or OpenGL might have optimized it out!\n", uniformName);
			return false;
		}

		this->uniforms.push_back(uniform);
		this->uniformNames.push_back(uniformName);
		this->numUniforms++;
		return true;
	}

	// For binary search when having a lot of uniforms
	inline bool AddUniformSorted(const char* uniformName)
	{
		return false;
	}

	inline GLint FindShaderUniform(const char* uniformName)
	{
		// Linear search because not much uniforms
		// NOTE(Tom): Binary search if more than x (see tests) uniforms
		for(int i = 0; i < numUniforms; i++)
		{
			if (!strcmp(uniformNames[i], uniformName))
			{
				return uniforms[i];
			}
		}

		return -1;
	}

	inline void SetUniform(const GLint uniform, const bool value)
	{
		if (uniform != -1)
			glUniform1i(uniform, (GLint)(value ? 1 : 0));
	}

	inline void SetUniform(const GLint uniform, const int value)
	{
		if (uniform != -1)
			glUniform1i(uniform, (GLint)value);
	}

	inline void SetUniform(const GLint uniform, const unsigned int value)
	{
		if (uniform != -1)
			glUniform1ui(uniform, (GLuint)value);
	}

	inline void SetUniform(const GLint uniform, const float value)
	{
		if (uniform != -1)
			glUniform1f(uniform, (GLfloat)value);
	}

	inline void SetUniform(const GLint uniform, const Vec2& value)
	{
		if (uniform != -1)
			glUniform2f(uniform, (GLfloat)value.x, (GLfloat)value.y);
	}

	inline void SetUniform(const GLint uniform, const Vec3& value)
	{
		if (uniform != -1)
			glUniform3f(uniform, (GLfloat)value.x, (GLfloat)value.y, (GLfloat)value.z);
	}

	inline void SetUniform(const GLint uniform, const Vec4& value)
	{
		if (uniform != -1)
			glUniform4f(uniform, (GLfloat)value.x, (GLfloat)value.y, (GLfloat)value.z, (GLfloat)value.w);
	}

	inline void SetUniform(const GLint uniform, const Matrix4x4& value)
	{
		if (uniform != -1)
			glUniformMatrix4fv(uniform, 1, GL_TRUE, (GLfloat*)value.values);
	}
};

// Default shader
inline void DefaultShader_Init(Shader* shader)
{
	shader->AddUniform("mvp");
}

inline void DefaultShader_Update(Shader* shader, const Transform& transform, const Camera& camera)
{
	GLint mvp = shader->uniforms[0];
	shader->SetUniform(mvp, CalculateMVP(transform, camera));
}

// Phong shader
inline void PhongShader_Init(Shader* shader)
{
	shader->AddUniform("mvp");
	shader->AddUniform("model");
	shader->AddUniform("cameraPosition");
	shader->AddUniform("material.specular");
	shader->AddUniform("material.shine");
	shader->AddUniform("dirLight.direction");
	shader->AddUniform("dirLight.ambient");
	shader->AddUniform("dirLight.diffuse");
	shader->AddUniform("dirLight.specular");
}

inline void PhongShader_Update(Shader* shader, const Transform& transform, const Camera& camera)
{
	GLint mvp = shader->uniforms[0];
	shader->SetUniform(mvp, CalculateMVP(transform, camera));

	GLint model = shader->uniforms[1];
	shader->SetUniform(model, transform.CalculateModel());

	GLint cameraPosition = shader->uniforms[2];
	shader->SetUniform(cameraPosition, camera.transform.position);
}

inline void PhongShader_UpdateMaterial(Shader* shader, const Material& material)
{
	GLint specular = shader->uniforms[3];
	shader->SetUniform(specular, material.specular);

	GLint shine = shader->uniforms[4];
	shader->SetUniform(shine, material.shine);
}

inline void PhongShader_UpdateLight(Shader* shader, const DirectionalLight& dirLight)
{
	GLint direction = shader->uniforms[5];
	shader->SetUniform(direction, dirLight.direction);

	GLint ambient = shader->uniforms[6];
	shader->SetUniform(ambient, dirLight.ambient);

	GLint diffuse = shader->uniforms[7];
	shader->SetUniform(diffuse, dirLight.diffuse);

	GLint specular = shader->uniforms[8];
	shader->SetUniform(specular, dirLight.specular);
}
