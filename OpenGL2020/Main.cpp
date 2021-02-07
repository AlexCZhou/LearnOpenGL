#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include "Shader.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
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
	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	
	Shader ourShader("Resource/shader/shader.vsh", "Resource/shader/shader.fsh");

	float vertices[] = {
		0.5f, 0.5f, 0.0f,   1.0f,0.0f,0.0f,// ���Ͻ�
		0.5f, -0.5f, 0.0f,  0.0f,1.0f,0.0f,// ���½�
		-0.5f, -0.5f, 0.0f, 0.0f,0.0f,1.0f,// ���½�
		-0.5f, 0.5f, 0.0f,  0.0f,1.0f,1.0f// ���Ͻ�
	};

	unsigned int indices[] = { // ע��������0��ʼ! 
		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};

	unsigned int VBO, VAO;
	unsigned int EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);
	// 1. �󶨶����������
	glBindVertexArray(VAO);
	// 2. �����ǵĶ������鸴�Ƶ�һ�����㻺���У���OpenGLʹ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. �������ǵ��������鵽һ�����������У���OpenGLʹ��
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 4. �趨��������ָ��
	//��һ������0 ����layout(location=0)������position��������λ��
	//�ڶ�������ָ���������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3
	//����������ָ�����ݵ����ͣ�������GL_FLOAT(GLSL��vec*�����ɸ�����ֵ��ɵ�)
	//���ĸ��������������Ƿ�ϣ�����ݱ���׼��(Normalize)�������������ΪGL_TRUE���������ݶ��ᱻӳ�䵽0�������з�����signed������-1����1֮�䡣���ǰ�������ΪGL_FALSE
	//�����������������(Stride)�������������������Ķ���������֮��ļ���������¸���λ��������3��float֮�����ǰѲ�������Ϊ3 * sizeof(float)��Ҫע�������������
	//	֪����������ǽ������еģ���������������֮��û�п�϶������Ҳ��������Ϊ0����OpenGL�������岽���Ƕ��٣�ֻ�е���ֵ�ǽ�������ʱ�ſ��ã���һ�������и���Ķ������ԣ�
	//	���Ǿͱ����С�ĵض���ÿ����������֮��ļ���������ں���ῴ�����������
	//���һ��������������void*��������Ҫ���ǽ��������ֵ�ǿ������ת��������ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)������λ������������Ŀ�ͷ������������0��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}