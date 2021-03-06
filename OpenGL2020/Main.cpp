﻿#include<iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "stb_image.h"


const float screenWidth = 1366;
const float screenHeight = 768;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
bool cursorDisable = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
//Imgui
//当前被选中的点光源
int currentLight = 0;
const char* pointLights[] = { "pointLight 1","pointLight 2","pointLight 3","pointLight 4" };
glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
};
void loadImgui();

void console(GLFWwindow* win, int key, int code, int action, int mods);
int main() {

    //init windows
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //init windows obj
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, console);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, screenWidth, screenHeight);

    //创建Z缓冲
    glEnable(GL_DEPTH_TEST);
    
    Shader ourShader("Resource/shader/shader.vsh", "Resource/shader/shader.fsh");
    Shader lightShader("Resource/shader/shader.vsh", "Resource/shader/light.fsh");
    //模型加载
    Model ourModel("Resource/model/nanosuit.obj");

    //创建IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //ImFont* font = io.Fonts->AddFontFromFileTTF("Resource/font/SourceHanSerifCN-Medium-6.otf", 20.0f,NULL,io.Fonts->GetGlyphRangesChineseSimplifiedCommon());


    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    glm::vec3 cubePositions[] = {
        //glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    

    float vertices[] = {
    //顶点                纹理          法向量
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
    };

    

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // 1. 绑定顶点数组对象
    glBindVertexArray(VAO);
    // 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 4. 设定顶点属性指针
    //第一个参数0 代表layout(location=0)定义了position顶点属性位置
    //第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3
    //第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)
    //第四个参数定义我们是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE
    //第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)。要注意的是由于我们
    //	知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。一旦我们有更多的顶点属性，
    //	我们就必须更小心地定义每个顶点属性之间的间隔，我们在后面会看到更多的例子
    //最后一个参数的类型是void*，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0。

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //设置颜色顶点属性
    /*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);*/

    //设置纹理顶点属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //设置法向量顶点属性
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(3);

    //纹理
    //让载入图片的头文件反转y轴
    stbi_set_flip_vertically_on_load(true);
    unsigned int textureBox;
    glGenTextures(1, &textureBox);
    glBindTexture(GL_TEXTURE_2D, textureBox);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    int width, height, nrChannels;
    unsigned char* data = stbi_load("Resource/texture/container2.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    

    unsigned int textureFace;
    glGenTextures(1, &textureFace);
    glBindTexture(GL_TEXTURE_2D, textureFace);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    data = stbi_load("Resource/texture/container2_specular.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    
    
    
    
    ourShader.use();
    ourShader.setInt("material.diffuse", 0);
    ourShader.setInt("material.specular", 1);

    ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    
    // Our state for Imgui
    ImVec4 imObjColor = ImVec4(1.0f, 0.5f, 0.31f, 1.00f);
    
    float modelAngelX = 0.0f;
    float modelAngelY = 0.0f;
    float modelAngelZ = 0.0f;
    
    glm::vec3 ambient(0.0215f, 0.1745f, 0.0215f);
    glm::vec3 diffuse(0.07568f, 0.61424f, 0.07568f);
    glm::vec3 specular(0.633f, 0.727811f, 0.633f);
    float shininess = 0.6f*128.0f;

    glm::vec3 tempColor = glm::vec3(1.0f, 1.0f, 1.0f);
    float dur = 2.5f;
    float cnt = 0.5f;
    int lastLight = -1;
    while (!glfwWindowShouldClose(window)) {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        loadImgui();

        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //因为我们使用了深度测试，我们也想要在每次渲染迭代之前清除深度缓冲（否则前一帧的深度信息仍然保存在缓冲中）
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // input
        processInput(window);
        // -----
        ourShader.use();
        

        ourShader.setVec3("viewPos", camera.Position);

        //定义材质
        ourShader.setVec3("material.specular", specular);
        ourShader.setFloat("material.shininess", shininess);

        //定义平行光
        ourShader.setVec3("dirLight.direction", -0.2f,-1.0f,-0.3f);

        ourShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
        ourShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
        ourShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);


        //定义聚光
        ourShader.setVec3("spotLight.position", camera.Position);
        ourShader.setVec3("spotLight.direction", camera.Front);
        ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

        ourShader.setVec3("spotLight.ambient", 1.0f, 1.0f, 1.0f);
        ourShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);

        ourShader.setFloat("spotLight.constant", 1.0f);
        ourShader.setFloat("spotLight.linear", 0.09f);
        ourShader.setFloat("spotLight.quadratic", 0.032f);

        

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);
        

        //绑定纹理单元
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureBox);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureFace);

        glBindVertexArray(VAO);
        
        
        ourShader.use();
        //物体变换矩阵
        glm::mat4 model;
        model = glm::rotate(model, modelAngelX, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, modelAngelY, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, modelAngelZ, glm::vec3(0.0f, 0.0f, 1.0f));

        ourShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //渲染模型
        ourModel.Draw(ourShader);

        
        //箱子来
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureBox);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureFace);
        for (unsigned int i = 0; i < 9; i++) {
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        

        //定义点光源
        for (int i = 0; i < 4; i++) {
            
            //光源变换矩阵
            lightShader.use();
            lightShader.setMat4("projection", projection);
            lightShader.setMat4("view", view);
            glm::mat4 light;
            light = glm::translate(light, pointLightPositions[i]);
            light = glm::scale(light, glm::vec3(0.5f, 0.5f, 0.5f));
            lightShader.setMat4("model", light);
            if (i == currentLight) {
                if (lastLight != currentLight) {
                    if(dur > 0.0f){ 
                        dur -= deltaTime;
                        if (cnt > 0.0f) {
                            cnt -= deltaTime;
                        }
                        else {
                            tempColor = glm::vec3(tempColor.x, abs(tempColor.y - 1.0f), abs(tempColor.z - 1.0f));
                            cnt = 0.5f;
                        }
                    }
                    else {
                        dur = 2.5f;
                        lastLight = currentLight;
                    }
                }
                else {
                    tempColor = glm::vec3(1.0f, 1.0f, 1.0f);
                }
                lightShader.setVec3("color", tempColor);
            }
            else {
                lightShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
            }
            
            glDrawArrays(GL_TRIANGLES, 0, 36);

            ourShader.use();
            char buffer[100];
            sprintf_s(buffer, "light[%d].position", i);
            ourShader.setVec3(buffer, pointLightPositions[i]);
            sprintf_s(buffer, "light[%d].ambient", i);
            ourShader.setVec3(buffer, 0.2f, 0.2f, 0.2f);
            sprintf_s(buffer, "light[%d].diffuse", i);
            ourShader.setVec3(buffer, 0.5f, 0.5f, 0.5f);
            sprintf_s(buffer, "light[%d].specular", i);
            ourShader.setVec3(buffer, 1.0f, 1.0f, 1.0f);
            sprintf_s(buffer, "light[%d].constant", i);
            ourShader.setFloat(buffer, 1.0f);
            sprintf_s(buffer, "light[%d].linear", i);
            ourShader.setFloat(buffer, 0.09f);
            sprintf_s(buffer, "light[%d].quadratic", i);
            ourShader.setFloat(buffer, 0.032f);
        }


        glBindVertexArray(0);

        //GUI
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //===
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

    //if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS) {
    //    if(cursorDisable){
    //    // tell GLFW to capture our mouse
    //        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //        cursorDisable = false;
    //    }
    //    else {
    //        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //        cursorDisable = true;
    //    }
    //}
    
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (cursorDisable) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
    else {
        firstMouse = true;
    }
}

void loadImgui() {

    ImGui::Begin("LearnOpenGL");         

    ImGui::Text("WASD to move camera");
    ImGui::Text("SPACE go up");
    ImGui::Text("ctrl go down");
    ImGui::Text("Press `(grave accent) key to enable/disable cursor.");

    ImGui::Combo("PointLights", &currentLight, pointLights, IM_ARRAYSIZE(pointLights));
    float tempLightPos[] = { pointLightPositions[currentLight].x,pointLightPositions[currentLight].y, pointLightPositions[currentLight].z };
    ImGui::DragFloat3("currentLightPos", tempLightPos, 0.01f);
    pointLightPositions[currentLight] = glm::make_vec3(tempLightPos);

    //ImGui::ColorEdit3("Object color", (float*)&imObjColor); // Edit 3 floats representing a color

    //ImGui::SliderAngle("modelAngel X", &modelAngelX, -90.0f, 90.0f);
    //ImGui::SliderAngle("modelAngel Y", &modelAngelY, -90.0f, 90.0f);
    //ImGui::SliderAngle("modelAngel Z", &modelAngelZ, -90.0f, 90.0f);

    //ImGui::DragFloat("lightPos x", &lightPos.x, 0.005f);
    //ImGui::DragFloat("lightPos y", &lightPos.y, 0.005f);
    //ImGui::DragFloat("lightPos z", &lightPos.z, 0.005f);
    //float ambientTemp[3] = { ambient.x,ambient.y,ambient.z };
    //ImGui::DragFloat3("ambient", ambientTemp, 0.005f);
    //float specularTemp[3] = { specular.x,specular.y,specular.z };
    //ImGui::DragFloat3("specular", specularTemp, 0.005f);
    //float diffuseTemp[3] = { diffuse.x,diffuse.y,diffuse.z };
    //ImGui::DragFloat3("diffuse", diffuseTemp, 0.005f);

    //ImGui::DragFloat("shininess", &shininess, 0.005f);

    /*ambient = glm::make_vec3(ambientTemp);
    specular = glm::make_vec3(specularTemp);
    diffuse = glm::make_vec3(diffuseTemp);*/

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

void console(GLFWwindow* window, int key, int code, int action, int mods) {
    if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_RELEASE) {
        
        if (cursorDisable) {
            // tell GLFW to capture our mouse
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursorDisable = false;
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cursorDisable = true;
        }
    }
}