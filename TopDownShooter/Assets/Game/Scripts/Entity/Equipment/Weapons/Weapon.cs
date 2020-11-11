using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class Weapon : Equipment {

    [SerializeField]
    [Tooltip("Spawn point for the bullets.")]
    protected Transform barrel = null;

    [SerializeField]
    [Tooltip("Bullet that will be fired.")]
    protected GameObject bullet = null;

    protected bool onCooldown = false;

    [Tooltip("A circular sprite representing the weapon.")]
    public Sprite icon = null;

    [Range(0, 10000)]
    public int maxAmmo;
    public int ammo;

    [Tooltip("Shots per second.")]
    public float fireRate = 2.0f;

    protected virtual void Awake()
    {
        if (barrel == null)
        {
            barrel = transform;
        }

        ammo = maxAmmo;
    }

    public override void OnEquip(Inventory inventory)
    {
        
    }

    public override void OnUnequip(Inventory inventory)
    {
        if (ammo <= 0)
        {
            Destroy(gameObject);
        }
    }

    /// <summary>
    /// Weapon specific fire function to fire bullets, enact cooldowns, etc.
    /// </summary>
    public abstract void Fire();

    /// <summary>
    /// Enact a cooldown (onCooldown is true during cooldown) for fire rate or reload purposes.
    /// </summary>
    /// <param name="time">Time till cooldown finishes.</param>
    /// <returns>Coroutine information.</returns>
    protected IEnumerator Cooldown(float time)
    {
        float timer = 0.0f;

        onCooldown = true;

        while (timer < time)
        {
            yield return null;

            timer += Time.deltaTime;
        }

        onCooldown = false;
    }
}
