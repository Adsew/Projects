using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HealthPack : Pickup {

    public float healAmount = 0.0f;

    protected override bool Effect(GameObject target)
    {
        Health health = target.GetComponent<Health>();

        if (health != null)
        {
            health.ApplyToHealth(healAmount);

            return true;
        }

        return false;
    }
}
