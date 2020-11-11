using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class PowerUp : Equipment
{
    protected GameObject poweredEntity = null;

    public ParticleSystem particles = null;

    [Tooltip("Duration the powerup lasts when picked up in seconds.")]
    [Range(0.0f, 1000.0f)]
    public float duration = 1.0f;

    public override void OnEquip(Inventory inventory)
    {
        poweredEntity = inventory.gameObject;

        PowerUpEntity(poweredEntity);

        StartCoroutine(DurationTimer());

        if (particles == null)
        {
            particles = GetComponent<ParticleSystem>();
        }

        if (particles != null)
        {
            particles.Play();
        }
    }

    public override void OnUnequip(Inventory inventory)
    {
        PowerDownEntity(poweredEntity);
        
        Destroy(gameObject);
    }

    /// <summary>
    /// Applies any effects the power up provides.
    /// </summary>
    /// <param name="entity">The entity to power up.</param>
    protected abstract void PowerUpEntity(GameObject entity);

    /// <summary>
    /// Removes any effects, usually those applied by this power up.
    /// </summary>
    /// <param name="entity">The entity to power down.</param>
    protected abstract void PowerDownEntity(GameObject entity);

    /// <summary>
    /// Counts down the lifespan of this powerup when equipped to the entity.
    /// </summary>
    /// <returns>Coroutine information.</returns>
    private IEnumerator DurationTimer()
    {
        float timer = duration;

        while (timer > 0.0f)
        {
            yield return null;

            timer -= Time.deltaTime;
        }

        PowerDownEntity(poweredEntity);

        Destroy(gameObject);
    }
}
