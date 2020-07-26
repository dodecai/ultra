#include "Renderer.h"
#include "Renderer2D.h"

#include "Engine/Platform/OpenGL/GLShader.h"

namespace Ultra {

Renderer::SceneData *Renderer::mSceneData = new Renderer::SceneData;

void Renderer::Load() {
	RenderCommand::Load();
	Renderer2D::Load();
}

void Renderer::BeginScene(Camera &camera) {
	mSceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::BeginScene(OrthographicCamera &camera) {
	mSceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}


void Renderer::EndScene() {
}

void Renderer::Resize(const uint32_t width, const uint32_t height) {
	RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::Submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &vertexArray, const glm::mat4 &transform) {
	shader->Bind();
	shader->SetMat4("u_ViewProjection", mSceneData->ViewProjectionMatrix);
	shader->SetMat4("u_Transform", transform);

	vertexArray->Bind();
	RenderCommand::DrawIndexed(vertexArray);
}

}


#if PROTOTYPE_EXAMPLES_3D

inline static float cubeVertices[] = {
    // positions          // normals
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
inline static float skyboxVertices[] = {
    // positions          
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

void Renderer::Init() {
    WindowWidth = 800;
    WindowHeight = 600;

    // MSAA
    glEnable(GL_MULTISAMPLE); //glfwWindowHint(GLFW_SAMPLES, 16);
                              // Eneble Z-Buffer
    glEnable(GL_DEPTH_TEST);

    // Draw in wireframe polgons
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // VSync (Lock to vertical sync)
    //glfwSwapInterval(0);
    // Capture Mouse
    //glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    TextureShaders.Load("Assets/Shaders/Texture.vs", "Assets/Shaders/Texture.fs", "");
    TextureShaders.Activate();
    TextureShaders.SetInt("texture", 0);

    // Model
    //BasicModel.loadModel("Assets/Models/Aventador/Avent.obj");
    //BasicModel.loadModel("Assets/Models/Nanosuit/nanosuit.obj");
    //BasicModel.loadModel("Assets/Models/Planet/planet.obj");
    BasicModel.loadModel("Assets/Objects/Cube.obj");
    //BasicModel.loadModel("Assets/Objects/Sphere.obj");
}

void Renderer::Draw(double alpha) {
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // don't forget to enable shader before setting uniforms
    TextureShaders.Activate();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)WindowWidth / (float)WindowWidth, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    TextureShaders.SetMat4("projection", projection);
    TextureShaders.SetMat4("view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));  // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));		   // it's a bit too big for our scene, so scale it down
    TextureShaders.SetMat4("model", model);
    BasicModel.Draw(TextureShaders);
}

void Renderer::DrawGrid(double alpha) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    BasicShaders.Activate();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);


    BasicShaders.SetMat4("Model", model);
    BasicShaders.SetMat4("View", view);
    BasicShaders.SetMat4("Projection", projection);

    float blueValue = cos(deltaTime) / 2.0f + 0.5f;
    int vertexColorLocation = glGetUniformLocation(BasicShaders.ID, "gColor");
    glUniform4f(vertexColorLocation, 0.0f, 0.0f, blueValue, 1.0f);

    // render the triangle
    int size = (sizeof(vertices) / sizeof(*vertices) / 3);
    glDrawArrays(GL_TRIANGLES, 0, size);

    auto numberOfLines = 50;
    for (int x = 0; x < numberOfLines; x++) {
        glBegin(GL_LINES);
        glVertex2f(x, 0);
        glVertex2f(0, x);
        glEnd();
    }
    for (int y = 0; y < numberOfLines; y++) {
        glBegin(GL_LINES);
        glVertex2f(y, 0);
        glVertex2f(0, y);
        glEnd();
    };

}

void Renderer::DrawSkybox() {
    //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Skybox
    CubemapShader.Activate();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)WindowWidth / (float)WindowHeight, 0.1f, 100.0f);
    CubemapShader.SetMat4("model", model);
    CubemapShader.SetMat4("view", view);
    CubemapShader.SetMat4("projection", projection);
    CubemapShader.SetVec3("cameraPos", camera.GetPosition());
    //// cubes
    //glBindVertexArray(cubeVAO);
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    //glBindVertexArray(0);

    // draw skybox as last
    glDepthFunc(GL_LEQUAL);	 // change depth function so depth test passes when values are equal to depth buffer's content
    SkyboxShader.Activate();
    view = glm::mat4(glm::mat3(camera.GetViewMatrix()));  // remove translation from the view matrix
    SkyboxShader.SetMat4("view", view);
    SkyboxShader.SetMat4("projection", projection);
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);  // set depth function back to default
}
unsigned int Renderer::LoadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

unsigned int Renderer::LoadTexture(char const *path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
// Test
void Renderer::Test() {
    glEnable(GL_DEPTH_TEST);

    // Skybox
    CubemapShader.Load("Assets/Shaders/Cubemaps.vs", "Assets/Shaders/Cubemaps.fs");
    SkyboxShader.Load("Assets/Shaders/Skybox.vs", "Assets/Shaders/Skybox.fs");

    // cube VAO
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    // skybox VAO
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    vector<std::string> faces {
        "Assets/Textures/Skybox/Right.bmp",
        "Assets/Textures/Skybox/Left.bmp",
        "Assets/Textures/Skybox/Top.bmp",
        "Assets/Textures/Skybox/Bottom.bmp",
        "Assets/Textures/Skybox/Front.bmp",
        "Assets/Textures/Skybox/Back.bmp",
    };
    cubemapTexture = LoadCubemap(faces);

    CubemapShader.Activate();
    CubemapShader.SetInt("skybox", 0);

    SkyboxShader.Activate();
    SkyboxShader.SetInt("skybox", 0);
}

#endif
