#pragma once

struct Skybox
{
    GLuint textureID;
    GLuint VAO;
    GLuint VBO;
};

void DrawSkybox(Skybox* skybox, Shader* shader, Camera* camera)
{
	glDepthFunc(GL_LEQUAL);

	Matrix4x4 view = ViewMatrix4x4(camera->transform.position, camera->transform.orientation);
	Matrix4x4 projection = Perspective(camera->fovy, camera->aspect, camera->zNear, camera->zFar);

	shader->Use();
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "view"), 1, GL_FALSE, view.values);
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "projection"), 1, GL_FALSE, projection.values);

	glBindVertexArray(skybox->VAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader->program, "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	shader->Unuse();

	glDepthFunc(GL_LESS);
}

void CreateSkybox(Skybox* skybox, GLuint skyboxTextureID)
{
	skybox->textureID = skyboxTextureID;

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

    glGenVertexArrays(1, &skybox->VAO);
    glGenBuffers(1, &skybox->VBO);
    glBindVertexArray(skybox->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, skybox->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
}

GLuint LoadSkyboxTexture(char** faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	DDSImage image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
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

	return textureID;
}
