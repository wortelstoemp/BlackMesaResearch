#pragma once

struct Skybox
{
	GLuint textureID;
	GLuint vao;
	GLuint vbo;
	Shader shader;
};

void SkyboxLoadTextureFromFiles(Skybox* skybox, const char* right, const char* left, const char* up,
	const char* down, const char* back, const char* front)
{
	const char* faces[6] = { right, left, up, down, back, front};

	glGenTextures(1, &skybox->textureID);
	glActiveTexture(GL_TEXTURE0);

	DDSImage image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->textureID);
	for(GLuint i = 0; i < 6; i++)
	{
		image.LoadFromFile(faces[i]);

		unsigned int size = ((image.width + 3) / 4) * ((image.height + 3) / 4) * image.blocksize;

		glCompressedTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, image.format,
			image.width, image.height, 0, size, image.data
		);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	image.Delete();
}

void SkyboxCreate(Skybox* skybox)
{
	GLfloat skyboxVertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	skybox->shader.LoadFromFiles("../data/shaders/skyboxVert.glsl", 0, 0, 0, "../data/shaders/skyboxFrag.glsl");

	skybox->shader.AddUniform("view");
	skybox->shader.AddUniform("projection");
	skybox->shader.AddUniform("skybox");

	glGenVertexArrays(1, &skybox->vao);
	glGenBuffers(1, &skybox->vbo);
	glBindVertexArray(skybox->vao);
	glBindBuffer(GL_ARRAY_BUFFER, skybox->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
}

void SkyboxRender(Skybox* skybox, const Camera& camera)
{
	glDepthFunc(GL_LEQUAL);
	skybox->shader.Use();


	skybox->shader.SetUniform(skybox->shader.uniforms[0], ViewMatrix4x4(camera.transform.position, camera.transform.orientation));
	skybox->shader.SetUniform(skybox->shader.uniforms[1], Perspective(camera.fovy, camera.aspect, camera.zNear, camera.zFar));
	skybox->shader.SetUniform(skybox->shader.uniforms[2], 0);

	glBindVertexArray(skybox->vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	skybox->shader.Unuse();
	glDepthFunc(GL_LESS);
}
