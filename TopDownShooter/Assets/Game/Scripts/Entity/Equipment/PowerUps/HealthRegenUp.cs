using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HealthRegenUp : PowerUp
{
    private Health health = null;

    public float healthPerSec = 1.0f;

    protected override void PowerUpEntity(GameObject entity)
    {
        health = entity.GetComponent<Health>();

        StartCoroutine(HealthRegen());
    }

    protected override void PowerDownEntity(GameObject entity)
    {
        health = null;
    }

    private IEnumerator HealthRegen()
    {
        while (health != null)
        {
            health.ApplyToHealth(healthPerSec * Time.deltaTime);

            yield return null;
        }
    }
}
