#pragma once

// Author(s): Simon, Tom

// API Usage code:
// DefaultShader shader;
// shader.CreateFromFiles("../data/shaders/default_vs.glsl", 0, 0, 0, "../data/shaders/default_fs.glsl");
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
	
	virtual void Init() = 0;
	//virtual void Update(const Transform& transform, const Camera& camera, float deltaTime) = 0;

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
		
		Init();
	}
	
	void CreateFromFiles(
		char* vertexFile,
		char* tessControlFile,
		char* tessEvalFile,
		char* geometryFile,
		char* fragmentFile)
	{
		char* vertexSource = vertexFile ? ReadFile(vertexFile) : 0;
		char* tessControlSource = tessControlFile ? ReadFile(tessControlFile) : 0;
		char* tessEvalSource = tessEvalFile ? ReadFile(tessEvalFile) : 0;
		char* geometrySource = geometryFile ? ReadFile(geometryFile) : 0;
		char* fragmentSource = fragmentFile ? ReadFile(fragmentFile) : 0;
		
		Create(vertexSource, tessControlSource, tessEvalSource, geometrySource, fragmentSource);		
	
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
			printf("Problem adding uniform: \"%s\" or OpenGL might have optimized it out!\n", uniform->name);
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
		glUniform2f(uniform.id, (GLfloat)value.x, (GLfloat)value.y);
	}
	
	inline void SetUniform(const ShaderUniform& uniform, const Vec3& value)
	{
		glUniform3f(uniform.id, (GLfloat)value.x, (GLfloat)value.y, (GLfloat)value.z);
	}
	
	inline void SetUniform(const ShaderUniform& uniform, const Vec4& value)
	{
		glUniform4f(uniform.id, (GLfloat)value.x, (GLfloat)value.y, (GLfloat)value.z, (GLfloat)value.w);
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
	ShaderUniform mvp;
	
	inline void Init()
	{
		mvp.name = "mvp";
		AddUniform(&mvp);
	}
	
	inline void Update(const Transform& transform, const Camera& camera, float deltaTime)
	{
		SetUniform(mvp, CalculateMVP(transform, camera));
	}
};

// TODO(Tom): Implement Phong shader (ambient, diffuse, specular)
struct PhongShader : public Shader
{
	ShaderUniform mvp;
	ShaderUniform model;
	ShaderUniform cameraPosition;
	ShaderUniform ambientLightColor;
	ShaderUniform ambientLightIntensity;
	ShaderUniform dirLightDirection;
	ShaderUniform dirLightDiffuseIntensity;
	ShaderUniform materialSpecularIntensity;
	ShaderUniform materialSpecularPower;			
	
	inline void Init()
	{
		mvp.name = "mvp";
		AddUniform(&mvp);
		model.name = "model";
		AddUniform(&model);
		cameraPosition.name = "cameraPosition";
		AddUniform(&cameraPosition);
		ambientLightColor.name = "ambientLight.color";
		AddUniform(&ambientLightColor);
		ambientLightIntensity.name = "ambientLight.intensity";
		AddUniform(&ambientLightIntensity);
		dirLightDirection.name = "dirLight.direction";
		AddUniform(&dirLightDirection);
		dirLightDiffuseIntensity.name = "dirLight.diffuseIntensity";
		AddUniform(&dirLightDiffuseIntensity);
		materialSpecularIntensity.name = "material.specularIntensity";
		AddUniform(&materialSpecularIntensity);
		materialSpecularPower.name = "material.specularPower";
		AddUniform(&materialSpecularPower);
	}
	
	inline void Update(const Transform& transform, const Camera& camera, float deltaTime)
	{
		SetUniform(this->mvp, CalculateMVP(transform, camera));
		SetUniform(this->model, CalculateMVP(transform, camera));
		SetUniform(this->cameraPosition, camera.transform.position);						
	}
	
	inline void UpdateMaterial(const Material& material)
	{
		SetUniform(this->materialSpecularIntensity, material.specularIntensity);
		SetUniform(this->materialSpecularPower, material.specularPower);		
	}
	
	inline void UpdateLight(const AmbientLight& ambientLight)
	{
		SetUniform(this->ambientLightColor, ambientLight.color);
		SetUniform(this->ambientLightIntensity, ambientLight.intensity);		
	}
	
	inline void UpdateLight(const DirectionalLight& dirLight)
	{
		SetUniform(this->dirLightDirection, dirLight.direction);
		SetUniform(this->dirLightDiffuseIntensity, dirLight.diffuseIntensity);		
	}
};


// TODO(Tom): Trying new Shader API
struct ShaderNew
{
	GLuint shaderProgram;
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
		this->shaderProgram = glCreateProgram();
		this->numUniforms = 0;
		
		CompileAndAttachShader(this->shaderProgram, &vertexSource, GL_VERTEX_SHADER);
		CompileAndAttachShader(this->shaderProgram, &tessControlSource, GL_TESS_CONTROL_SHADER);
		CompileAndAttachShader(this->shaderProgram, &tessEvalSource, GL_TESS_EVALUATION_SHADER);
		CompileAndAttachShader(this->shaderProgram, &geometrySource, GL_GEOMETRY_SHADER);
		CompileAndAttachShader(this->shaderProgram, &fragmentSource, GL_FRAGMENT_SHADER);

		glLinkProgram(this->shaderProgram);

		int linked;
		glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &linked);
		if (linked != GL_TRUE)
		{
			GLsizei log_length = 0;
			GLchar message[1024];
			glGetProgramInfoLog(this->shaderProgram, 1024, &log_length, message);
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
		char* vertexSource = vertexFile ? ReadFile(vertexFile) : 0;
		char* tessControlSource = tessControlFile ? ReadFile(tessControlFile) : 0;
		char* tessEvalSource = tessEvalFile ? ReadFile(tessEvalFile) : 0;
		char* geometrySource = geometryFile ? ReadFile(geometryFile) : 0;
		char* fragmentSource = fragmentFile ? ReadFile(fragmentFile) : 0;
		
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
	
	inline bool AddUniform(const char* uniformName)
	{
		GLint uniform = glGetUniformLocation(shaderProgram, (const GLchar*)uniformName);
		
		if (uniform == -1) {
			printf("Problem adding uniform: \"%s\" or OpenGL might have optimized it out!\n", uniformName);
			return false;
		}
		
		this->uniforms.push_back(uniform);
		this->uniformNames.push_back(uniformName);
		this->numUniforms++;		
		return true;
	}
	
	inline GLint FindShaderUniform(const char* uniformName)
	{
		// Linear search because not much uniforms
		// TODO(Tom): Binary search if more than x (see tests) uniforms
		for(int i = 0; i < numUniforms; i++) {
			if (!strcmp(uniformNames[i], uniformName)) {
				return uniforms[i];
			}
		}
		
		return -1;
	}
	
	inline void SetUniform(const GLint uniform, const bool value)
	{
		if (uniform != -1) {
			glUniform1i(uniform, (GLint)(value ? 1 : 0));
		}
	}
	
	inline void SetUniform(const GLint uniform, const int value)
	{
		if (uniform != -1) {
			glUniform1i(uniform, (GLint)value);
		}
	}
	
	inline void SetUniform(const GLint uniform, const unsigned int value)
	{
		if (uniform != -1) {
			glUniform1ui(uniform, (GLuint)value);
		}
	}
	
	inline void SetUniform(const GLint uniform, const float value)
	{
		if (uniform != -1) {
			glUniform1f(uniform, (GLfloat)value);
		}
	}
	
	inline void SetUniform(const GLint uniform, const Vec2& value)
	{
		if (uniform != -1) {
			glUniform2f(uniform, (GLfloat)value.x, (GLfloat)value.y);
		}
	}
	
	inline void SetUniform(const GLint uniform, const Vec3& value)
	{
		if (uniform != -1) {
			glUniform3f(uniform, (GLfloat)value.x, (GLfloat)value.y, (GLfloat)value.z);
		}
	}
	
	inline void SetUniform(const GLint uniform, const Vec4& value)
	{
		if (uniform != -1) {
			glUniform4f(uniform, (GLfloat)value.x, (GLfloat)value.y, (GLfloat)value.z, (GLfloat)value.w);
		}
	}
	
	inline void SetUniform(const GLint uniform, const Matrix4x4& value)
	{
		if (uniform != -1) {
			glUniformMatrix4fv(uniform, 1, GL_TRUE, (GLfloat*)value.values);
		}
	}
};

// Default shader
inline ShaderNew CreateDefaultShader()
{
	ShaderNew shader;
	shader.AddUniform("mvp");
	
	return shader;
}

inline void DefaultShader_Update(ShaderNew* shader, const Transform& transform, const Camera& camera)
{
	GLint mvp = shader->FindShaderUniform("mvp");
	if (mvp != -1) {
		shader->SetUniform(mvp, CalculateMVP(transform, camera));
	}
}

// Phong shader
inline ShaderNew CreatePhongShader()
{
	ShaderNew shader;
	shader.AddUniform("mvp");
	shader.AddUniform("model");
	shader.AddUniform("cameraPosition");
	shader.AddUniform("ambientLight.color");
	shader.AddUniform("ambientLight.intensity");
	shader.AddUniform("dirLight.direction");
	shader.AddUniform("dirLight.diffuseIntensity");
	shader.AddUniform("material.specularIntensity");
	shader.AddUniform("material.specularPower");
}

inline void PhongShader_Update(ShaderNew* shader, const Transform& transform, const Camera& camera)
{
	GLint mvp = shader->FindShaderUniform("mvp");
	shader->SetUniform(mvp, CalculateMVP(transform, camera));
	
	GLint model = shader->FindShaderUniform("model");
	// TODO(Tom): use transform.CalculateModel() instead !!!
	shader->SetUniform(model, CalculateMVP(transform, camera));
	
	GLint cameraPosition = shader->FindShaderUniform("cameraPosition");
	shader->SetUniform(cameraPosition, camera.transform.position);				
}

inline void PhongShader_UpdateMaterial(ShaderNew* shader, const Material& material)
{
	GLint materialSpecularIntensity = shader->FindShaderUniform("material.specularIntensity");
	shader->SetUniform(materialSpecularIntensity, material.specularIntensity);
	
	GLint materialSpecularPower = shader->FindShaderUniform("material.specularPower");
	shader->SetUniform(materialSpecularPower, material.specularPower);		
}

inline void PhongShader_UpdateLight(ShaderNew* shader, const AmbientLight& ambientLight)
{
	GLint ambientLightColor = shader->FindShaderUniform("ambientLight.color");
	shader->SetUniform(ambientLightColor, ambientLight.color);
	
	GLint ambientLightIntensity = shader->FindShaderUniform("ambientLight.intensity");	
	shader->SetUniform(ambientLightIntensity, ambientLight.intensity);		
}

inline void PhongShader_UpdateLight(ShaderNew* shader, const DirectionalLight& dirLight)
{
	GLint dirLightDirection = shader->FindShaderUniform("dirLight.direction");	
	shader->SetUniform(dirLightDirection, dirLight.direction);
	
	GLint dirLightDiffuseIntensity = shader->FindShaderUniform("dirLight.diffuseIntensity");	
	shader->SetUniform(dirLightDiffuseIntensity, dirLight.diffuseIntensity);		
}