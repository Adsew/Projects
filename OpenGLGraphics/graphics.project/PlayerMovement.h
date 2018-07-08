#pragma once

class IsPickUpable;
class InventoryRenderer;

// move camera around script
class PlayerMovement : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(PlayerMovement, Component);
private:
	bool m_pickUpPressed;
	bool checkCollisions();
	void checkForPickUp(std::shared_ptr<IsPickUpable> pickupable);
	std::weak_ptr<InventoryRenderer> m_inventory;

public:
	bool locked;

	void initialize() override;
	void start() override;
	void update(float _deltaTime) override;
	// callback that is registered on initialization for the cursor position changing
	virtual void onCursorPosChange(Window* window, double xPos, double yPos);
};