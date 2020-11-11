using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class Health : MonoBehaviour {

    private ParticleSystem lowHPps = null;
    
    [SerializeField]
    [Tooltip("Automatically sets itself to max on start.")]
    protected float health;
    
    public float maxHealth = 100.0f;
    public float HP { get { return health; } private set { health = value; } }

    public bool hasDied { get; private set; }

    public bool hitRecently { get; private set; }
    public float hitRecentlyTime = 5.0f;
    private float hitRecTimer = 0.0f;

    public bool invincible = false;

    public GameObject lowHPParticle = null;
    [Range(0.0f, 100.0f)]
    public float lowHPPercent = 25.0f;

    public UnityEvent onHealthChange = new UnityEvent();
    public UnityEvent onHit = new UnityEvent();
    public UnityEvent onDeath = new UnityEvent();
    
	protected void Start ()
    {
        if (lowHPParticle != null)
        {
            GameObject ps = Instantiate(lowHPParticle, transform);

            lowHPps = ps.GetComponent<ParticleSystem>();

            if (lowHPps != null)
            {
                lowHPps.Stop();
            }
        }

        health = maxHealth;
        hasDied = false;
	}

    /// <summary>
    /// Apply some value to health. Negative values reduce health while positive ones heal.
    /// </summary>
    /// <param name="amount">Amount to add to the health.</param>
    public virtual void ApplyToHealth(float amount)
    {
        if (!hasDied)
        {
            if (!invincible || (invincible && amount > 0.0f))
            {
                health += amount;

                onHealthChange.Invoke();

                if (amount <= 0.0f)
                {
                    if (!hasDied)
                    {
                        onHit.Invoke();
                    }

                    StartCoroutine(RecentlyHitTimer());
                }

                if (health <= 0.0f && onDeath != null)
                {
                    health = 0.0f;
                    onDeath.Invoke();

                    hasDied = true;
                }
                else if (health > maxHealth)
                {
                    health = maxHealth;
                }

                LowHPParticleCheck();
            }
        }
    }

    /// <summary>
    /// Bring health back to full and reset death. Only works if hasDied is true;
    /// </summary>
    public void Revive()
    {
        hasDied = false;
        health = maxHealth;
        hitRecently = false;

        LowHPParticleCheck();
        onHealthChange.Invoke();
    }

    private void LowHPParticleCheck()
    {
        if (lowHPps != null)
        {
            if ((health / maxHealth) * 100.0f > lowHPPercent)
            {
                lowHPps.Stop();
            }
            else
            {
                lowHPps.Play();
            }
        }
    }

    public void SetInvinicible(bool i)
    {
        invincible = i;
    }

    public void ToggleInvinicible()
    {
        invincible = !invincible;
    }

    private IEnumerator RecentlyHitTimer()
    {
        hitRecTimer = 0.0f;

        if (!hitRecently)
        {
            hitRecently = true;

            while (hitRecTimer < hitRecentlyTime)
            {
                hitRecTimer += Time.deltaTime;

                yield return null;
            }

            hitRecently = false;
        }
    }
}
