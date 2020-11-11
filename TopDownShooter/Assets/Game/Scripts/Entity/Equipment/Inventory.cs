using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Inventory : MonoBehaviour {

    private List<Equipment> nearbyEquipment = new List<Equipment>();

    [System.Serializable]
	public class Slot
    {
        public string name = "";
        public Equipment equipment = null;
        public Transform attachPoint = null;
    }

    public List<Slot> slots = new List<Slot>();

    private void Start()
    {
        // Setup any preequipped items
        for (int i = 0; i < slots.Count; i++)
        {
            if (slots[i].equipment != null)
            {
                // Attach it to entity's attach point and center it.
                Transform toEquipTrans = slots[i].equipment.transform;

                if (slots[i].attachPoint != null)
                {
                    toEquipTrans.parent = slots[i].attachPoint;
                }
                else
                {
                    toEquipTrans.parent = transform;
                }

                toEquipTrans.localPosition = Vector3.zero;
                toEquipTrans.localRotation = Quaternion.identity;

                slots[i].equipment.OnEquipEvent.Invoke();
                slots[i].equipment.OnEquip(this);
            }
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        Equipment equipable = other.gameObject.GetComponent<Equipment>();

        if (equipable != null )
        {
            if (equipable.autoPickup)
            {
                Equip(equipable, true);
            }
            else
            {
                nearbyEquipment.Add(equipable);
            }
        }
    }

    private void OnTriggerExit(Collider other)
    {
        Equipment equipable = other.gameObject.GetComponent<Equipment>();

        if (equipable != null)
        {
            nearbyEquipment.Remove(equipable);
        }
    }

    /// <summary>
    /// Attempt to equip equipment into a slot in the inventory. Prioritizes empty slots first.
    /// </summary>
    /// <param name="toEquip">Equipment to equip. Can be null to unequip something.</param>
    /// <param name="replaceCurrent">If required slot is occupied, will unequip current equipment first.</param>
    /// <returns>True if equip was successfuly.</returns>
    public bool Equip(Equipment toEquip, bool replaceCurrent = false)
    {
        Slot slotToUse = null;

        // Find a slot
        for (int i = 0; i < slots.Count; i++)
        {
            if (slots[i].name == toEquip.slot)
            {
                if (slots[i].equipment == null)
                {
                    slotToUse = slots[i];

                    break;
                }
                // Hold as desired slot unless a free one exists
                else if (replaceCurrent)
                {
                    slotToUse = slots[i];
                }
            }
        }

        // Slot found, actually equip
        if (slotToUse != null)
        {
            if (slotToUse.equipment != null)
            {
                slotToUse.equipment.transform.parent = null;
                slotToUse.equipment.OnUnequipEvent.Invoke();
                slotToUse.equipment.OnUnequip(this);
            }

            slotToUse.equipment = toEquip;

            if (toEquip != null)
            {
                // Attach it to entity's attach point and center it.
                Transform toEquipTrans = toEquip.transform;

                if (slotToUse.attachPoint != null)
                {
                    toEquipTrans.parent = slotToUse.attachPoint;
                }
                else
                {
                    toEquipTrans.parent = transform;
                }

                toEquipTrans.localPosition = Vector3.zero;
                toEquipTrans.localRotation = Quaternion.identity;

                toEquip.OnEquipEvent.Invoke();
                toEquip.OnEquip(this);
            }

            return true;
        }

        return false;
    }

    public void Unequip(string slot)
    {
        if (!string.IsNullOrEmpty(slot))
        {
            for (int i = 0; i < slots.Count; i++)
            {
                if (slots[i].name == slot && slots[i].equipment != null)
                {
                    slots[i].equipment.transform.parent = null;
                    slots[i].equipment.OnUnequipEvent.Invoke();
                    slots[i].equipment.OnUnequip(this);

                    break;
                }
            }
        }
    }

    /// <summary>
    /// Equip the equipment closest to the player.
    /// </summary>
    /// <returns>True on successful equip, false if nothing detected.</returns>
    public bool PickUpNearest()
    {
        if (nearbyEquipment.Count > 0)
        {
            Transform trans = transform;
            Equipment nearest = null;
            float nearestDist = -1.0f;
            float tempDist = 0.0f;
            int i = nearbyEquipment.Count - 1;

            // Find nearest while also clearing list of bad elements
            do
            {
                if (nearbyEquipment[i] == null)
                {
                    nearbyEquipment.RemoveAt(i);
                }
                else if (nearest == null)
                {
                    nearest = nearbyEquipment[i];
                    nearestDist = Vector3.Magnitude(nearest.transform.position - trans.position);
                }
                else
                {
                    tempDist = Vector3.Magnitude(nearbyEquipment[i].transform.position - trans.position);

                    if (tempDist < nearestDist)
                    {
                        nearestDist = tempDist;
                        nearest = nearbyEquipment[i];
                    }
                }

                i--;

            } while (i >= 0);
            
            if (nearest != null)
            {
                nearbyEquipment.Remove(nearest);
                Equip(nearest, true);

                return true;
            }
        }

        return false;
    }
}
