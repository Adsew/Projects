using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpeedUp : PowerUp
{
    protected float speedIncrease = 0.0f;

    [Tooltip("How many times max speed to add to current max speed.")]
    [Range(0.0f, 10.0f)]
    public float speedMultiplier = 1.0f;

    protected override void PowerUpEntity(GameObject entity)
    {
        Movement movement = entity.GetComponent<Movement>();

        if (movement != null)
        {
            speedIncrease = movement.MaxSpeed * speedMultiplier;
            movement.bonusMaxSpeed += speedIncrease;
        }
    }

    protected override void PowerDownEntity(GameObject entity)
    {
        Movement movement = entity.GetComponent<Movement>();

        if (movement != null)
        {
            movement.bonusMaxSpeed -= speedIncrease;
        }
    }
}
