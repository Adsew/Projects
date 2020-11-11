using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public abstract class Equipment : MonoBehaviour {

    [Tooltip("The slot this equipment goes into.")]
    public string slot = "";

    [Tooltip("If true, inventories will automatically try to store/equip this.")]
    public bool autoPickup = false;

    [Tooltip("Perform any external events that should happen before being added to an inventory.")]
    public UnityEvent OnEquipEvent = new UnityEvent();

    [Tooltip("Perform any events that should happen before being placed back in the world.")]
    public UnityEvent OnUnequipEvent = new UnityEvent();

    /// <summary>
    /// Called by inventory when equipment successfully enters an inventory slot.
    /// </summary>
    /// <param name="inventory">The inventory being equiped to.</param>
    public abstract void OnEquip(Inventory inventory);

    /// <summary>
    /// Called by inventory when the equipment is unequipped by the inventory. Not called if unequiped externally.
    /// </summary>
    /// <param name="inventory">The inventory being unequipped from.</param>
    public abstract void OnUnequip(Inventory inventory);
}
