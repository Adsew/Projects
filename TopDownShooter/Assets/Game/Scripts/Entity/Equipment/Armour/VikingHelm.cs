using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class VikingHelm : Equipment
{
    [SerializeField]
    private Health entityHealth = null;

    public float healOnHit = 2.0f;

    public override void OnEquip(Inventory inventory)
    {
        entityHealth = inventory.GetComponent<Health>();

        if (entityHealth != null)
        {
            entityHealth.onHit.AddListener(HealWearer);
        }
    }

    public override void OnUnequip(Inventory inventory)
    {
        if (entityHealth != null)
        {
            entityHealth.onHit.RemoveListener(HealWearer);
        }
    }

    public void HealWearer()
    {
        entityHealth.ApplyToHealth(healOnHit);
    }
}
