#include "Core.h"
#include "IsPickUpable.h"

#include "AssetManager.h"
#include "Asset.h"
#include "TextureAsset.h"

IMPLEMENT_DYNAMIC_CLASS(IsPickUpable);

void IsPickUpable::load(XMLElement * element)
{
	Component::load(element);

	if (auto strName = element->Attribute("icon-name"))
	{
		auto asset = AssetManager::instance().getAsset(strName);
		if (asset != nullptr && asset->isA(TextureAsset::getClassHashCode()))
		{
			iconTexture = std::static_pointer_cast<TextureAsset>(asset);
		}
	}
}
