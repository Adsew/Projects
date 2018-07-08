#include "Core.h"
#include "Shader.h"

#include "FileManager.h"

IMPLEMENT_DYNAMIC_CLASS(Shader);

Shader::Shader()
	: m_id(0),
	m_type(0),
	m_loaded(false),
	m_compiled(false)
{
}

Shader::~Shader()
{
	if (m_compiled)
	{
		glDeleteShader(m_id);
	}
}

void Shader::load(XMLElement * element)
{
	Asset::load(element);
	XMLElement* path = element->FirstChildElement("path");
	if (path != nullptr)
	{
		if (FileManager::loadFile(&m_source, path->GetText()))
		{

		}
		else {
			std::cout << "Shader was unable to load file" << std::endl;
		}
	}
	XMLElement* type = element->FirstChildElement("type");
	if (type != nullptr)
	{
		// TODO: implement the rest of the types of shaders
		std::string t = type->GetText();
		if (t == "GL_VERTEX_SHADER")
		{
			m_type = GL_VERTEX_SHADER;
		}
		else if (t == "GL_FRAGMENT_SHADER")
		{
			m_type = GL_FRAGMENT_SHADER;
		}
		else {
			std::cout << "Unable to find the corresponding shader type from XML file" << std::endl;
		}
	}

	m_id = glCreateShader(m_type);
}

void Shader::initialize()
{
	Asset::initialize();
	compile();
}

bool Shader::isCompiled() const
{
	return m_compiled;
}

bool Shader::isLoaded() const
{
	return m_loaded;
}

void Shader::compile()
{
	const char* s = m_source.c_str();


	glShaderSource(m_id, 1, &s, nullptr);

	glCompileShader(m_id);

	int32_t status;
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &status);

	switch (m_type)
	{
	case GL_VERTEX_SHADER:
		std::cout << "GL_VERTEX_SHADER ";
		break;
	case GL_FRAGMENT_SHADER:
		std::cout << "GL_FRAGMENT_SHADER ";
		break;
	default:
		std::cout << "GL_SHADER ";
		break;
	}

	if (status == false)
	{
		int32_t logSize;
		glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &logSize);

		char* log = new char[logSize + 1];
		glGetShaderInfoLog(m_id, logSize, nullptr, log);

		std::cout << " failed to compile: " << log << std::endl;

		delete log;
	}
	else {
		std::cout << " compiled successfully" << std::endl;
		m_compiled = true;
	}

	m_loaded = true;
}

unsigned int Shader::getID() const
{
	return m_id;
}

unsigned int Shader::getType() const
{
	return m_type;
}
