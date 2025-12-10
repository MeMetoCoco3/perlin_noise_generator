#ifndef VSHADERS
#define VSHADERS

#include "vstd/vtypes.h"
#include <GLFW/glfw3.h>
#include "vstd/vgeneral.h"

class Shader 
{
private:
	u32 m_Program;
public:
	Shader(const std::string& VS_PATH, const std::string& FS_PATH)
	{
		u32 VertexShader = glCreateShader(GL_VERTEX_SHADER);

		std::string VertexSource = LoadFile(VS_PATH);
		const char* source = VertexSource.c_str();
		glShaderSource(VertexShader, 1, &source, NULL);
		glCompileShader(VertexShader);
		
		int  Success;
		char InfoLog[512];
		glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);

		if (!Success)
		{
			glGetShaderInfoLog(VertexShader, 512, NULL, InfoLog);
			ERROR("ERROR::SHADER::VERTEX:COMPILATION\n");
			ERROR(InfoLog);
		}
		

		u32 FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		std::string FragmentSource = LoadFile(FS_PATH);
		source = FragmentSource.c_str();
		glShaderSource(FragmentShader, 1, &source, NULL);
		glCompileShader(FragmentShader);

		glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
		if (!Success)
		{
			glGetShaderInfoLog(FragmentShader, 512, NULL, InfoLog);
			ERROR("ERROR::SHADER::FRAGMENT:COMPILATION\n");
			ERROR(InfoLog);
		}

		m_Program = glCreateProgram();
		glAttachShader(m_Program, VertexShader);
		glAttachShader(m_Program, FragmentShader);
		glLinkProgram(m_Program);

		glGetShaderiv(m_Program, GL_LINK_STATUS, &Success);
		if (!Success)
		{
			glGetShaderInfoLog(m_Program, 512, NULL, InfoLog);
			ERROR("ERROR::SHADER::LINKAGE\n");
			ERROR(InfoLog);
		}
		
		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);

	}


	inline void UseProgram(void) const 
	{
		glUseProgram(m_Program);
	}


};


#endif