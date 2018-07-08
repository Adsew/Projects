#pragma once

#include "Asset.h"

// shader class for each type of shader
// used later for swapping shaders in/out of a shader program
class Shader : public Asset
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Shader, Asset);
private:
	unsigned int m_id;
	unsigned int m_type;
	bool m_loaded;
	bool m_compiled;
	std::string m_source;

public:
	Shader();
	~Shader();
	void load(XMLElement* element) override;
	void initialize() override;
	bool isCompiled() const;
	bool isLoaded() const;
	void compile();
	unsigned int getID() const;
	unsigned int getType() const;
};