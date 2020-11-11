using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour {

    private PlayerMovement movement = null;
    private Inventory inventory = null;
    public Inventory.Slot weaponSlot { get; private set; }

    private bool mouseMode = true;

    public string weaponSlotName = "";

    [Header("Inputs")]

    public string horizontalMove = "";
    public string verticalMove = "";
    public string horizontalAim = "";
    public string verticalAim = "";
    public string pickupButton = "";
    public string fireButton = "";
    public string pauseButton = "";

    protected void Awake()
    {
        movement = GetComponent<PlayerMovement>();
        inventory = GetComponent<Inventory>();

        if (inventory != null)
        {
            // Keep track of this slot to always see equipped weapon
            for (int i = 0; i < inventory.slots.Count; i++)
            {
                if (inventory.slots[i].name == weaponSlotName)
                {
                    weaponSlot = inventory.slots[i];

                    break;
                }
            }
        }

#if DEBUG
        if (movement == null)
        {
            Debug.LogError(gameObject.name + " PlayerController: Player is missing PlayerMovement script.");
        }
        if (inventory == null)
        {
            Debug.LogError(gameObject.name + " PlayerController: Player is missing Inventory script.");
        }
#endif
    }

    private void Update()
    {
        if (!Player.Instance.isDead && GameStateSystem.Instance.timerRunning)
        {
            if (movement != null)
            {
                Vector3 direction = new Vector3(
                    Input.GetAxis(horizontalMove),
                    0.0f,
                    Input.GetAxis(verticalMove)
                    );

                if (direction.magnitude > 1.0f)
                {
                    direction = direction.normalized;
                }

                // Aiming input depends on controller
                Vector3 rotation = new Vector3();

                if (mouseMode)
                {
                    rotation.x = Input.mousePosition.x - Screen.width / 2.0f;
                    rotation.z = Input.mousePosition.y - Screen.height / 2.0f;

                    // Detected controller input, switch to controller mode
                    if (Input.GetAxis(horizontalAim) != 0.0f || Input.GetAxis(verticalAim) != 0.0f)
                    {
                        mouseMode = false;
                    }
                }
                else
                {
                    rotation.x = Input.GetAxis(horizontalAim);
                    rotation.z = Input.GetAxis(verticalAim);

                    if (Input.GetMouseButtonDown(0))
                    {
                        mouseMode = true;
                    }
                }

                movement.Move(direction, rotation);
            }

            if (inventory != null && Input.GetButtonDown(pickupButton))
            {
                inventory.PickUpNearest();
            }

            if (Input.GetButton(fireButton) && weaponSlot != null && weaponSlot.equipment != null)
            {
                Weapon weapon = weaponSlot.equipment as Weapon;

                if (weapon != null)
                {
                    weapon.Fire();
                }
            }

            if (Input.GetButtonDown(pauseButton))
            {
                GameStateSystem.Instance.PauseGame();
            }
        }
    }
}
