#include "Core.h"
#include "MiniMap.h"

#include "AssetManager.h"
#include "TextureAsset.h"
#include "ShaderProgram.h"
#include "RenderManager.h"

IMPLEMENT_DYNAMIC_CLASS(MiniMap);

void MiniMap::load(XMLElement * element)
{
	InventoryRenderer::load(element);

	if (auto strName = element->Attribute("texture-name"))
	{
		auto asset = AssetManager::instance().getAsset(strName);
		if (asset != nullptr && asset->isA(TextureAsset::getClassHashCode()))
		{
			m_screenWidth = RenderManager::instance().getWidth();
			m_screenHeight = RenderManager::instance().getHeight();
			addInventoryIcon(getUUIDHashCode(), std::static_pointer_cast<TextureAsset>(asset));
		}
	}
}
