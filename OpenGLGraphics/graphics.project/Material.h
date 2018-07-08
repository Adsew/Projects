#pragma once

class TextureAsset;

// holds the textures used for a material. i.e. the diffuse and specular textures used in the lighting shader
class Material : public Object
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Material, Object);
private:
	std::weak_ptr<TextureAsset> m_diffuseTexture;
	std::weak_ptr<TextureAsset> m_specularTexture;
	float m_shineness;

public:
	Material();
	void load(XMLElement* element);
	void use();
	void disable();
	unsigned int getDiffuseID() const;
	unsigned int getSpecularID() const;
	unsigned int getDiffuseUnit() const;
	unsigned int getSpecularUnit() const;
	float getShineness() const;
};