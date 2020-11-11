using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpeedShoulder : Equipment
{
    [SerializeField]
    private Movement entityMovement = null;

    private float speedGranted = 0.0f;

    [Tooltip("Grants this amount times extra speed in addition to normal max.")]
    public float timesBonusSpeed = 2.0f;

    public override void OnEquip(Inventory inventory)
    {
        entityMovement = inventory.GetComponent<Movement>();

        if (entityMovement != null)
        {
            speedGranted = entityMovement.MaxSpeed * timesBonusSpeed;
            entityMovement.bonusMaxSpeed += speedGranted;
        }
    }

    public override void OnUnequip(Inventory inventory)
    {
        if (entityMovement != null)
        {
            entityMovement.bonusMaxSpeed -= speedGranted;
            speedGranted = 0.0f;
        }
    }
}
