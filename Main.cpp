#include "config.h"	

unsigned int make_shader(const std::string& vertexPath, const std::string& fragmentPath);

unsigned int make_module(const std::string& filePath, unsigned int moduleType);

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
	}

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	unsigned int shader = make_shader("shaders/vertex.h", "shaders/fragment.h");

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader); //Do not need to call this every frame, but it is something I might need when switiching shaders out
		glfwSwapBuffers(window);
	}

	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}

unsigned int make_shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::vector<unsigned int> shaderModules;

	shaderModules.push_back(make_module(vertexPath, GL_VERTEX_SHADER));
	shaderModules.push_back(make_module(fragmentPath, GL_FRAGMENT_SHADER));

	unsigned int shader = glCreateProgram();

	//Creating a single shader program from multiple shader modules, which are compiled independently
	for (unsigned int shaderModule : shaderModules)
	{
		glAttachShader(shader, shaderModule); //Attaching shader modules to the shader program is the process of associating compiled shader modules with a shader program, allowing them to be linked together into a complete shader program that can be used for rendering
	}
	glLinkProgram(shader); //Linking is the process of creating a shader program from multiple shader modules, which are compiled independently

	int success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success); //Querying for shader program linking success.

	if (!success)
	{
		char infoLog[1024];
		glGetProgramInfoLog(shader, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//Shader modules are independent of the shader program, so we can delete them after linking
	for (unsigned int shaderModule : shaderModules)
	{
		glDeleteShader(shaderModule);
	}

	return shader;
}

unsigned int make_module(const std::string& filePath, unsigned int moduleType)
{
	std::ifstream file;
	std::stringstream bufferedLines;
	std::string line;

	file.open(filePath);

	//Writing the shader source code to a stringstream, which is a stream class to operate on strings. 
	while(std::getline(file, line))
	{
		bufferedLines << line << "\n";
	}

	std::string shaderSource = bufferedLines.str();
	const char* shaderSourcePtr = shaderSource.c_str();
	bufferedLines.str("");
	file.close();

	//Creating shader module of the specified type, then using the loaded shader source to compile the shader module.
	unsigned int shaderModule = glCreateShader(moduleType);
	glShaderSource(shaderModule, 1, &shaderSourcePtr, NULL); //Attaching the shader source code loaded as a string to the shader module.
	glCompileShader(shaderModule); //Compiling the newly created shader module with the attached shader source code.

	int success;
	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success); //Querying for shader compilation success.

	if (!success)
	{
		char infoLog[1024];
		glGetShaderInfoLog(shaderModule, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return shaderModule;
}

