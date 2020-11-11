using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyController : MonoBehaviour {
    
    [SerializeField]
    private List<GameObject> attackColliders = new List<GameObject>();

    private Inventory.Slot weaponSlot = null;

    public string fsmDeathTrigger = "Die";
    public string weaponSlotName = "Weapon";

    public Transform trans = null;
    public Animator anim = null;
    public EnemyMovement movement = null;
    public Health health = null;
    public Inventory inventory = null;
    public Spawnable spawnable = null;

    [Tooltip("How much stats will increase based on difficulty.")]
    public float difficultyScale = 0.3f;

    private void Awake()
    {
        // First remove self hits from attack colliders
        Collider[] colliders = GetComponentsInChildren<Collider>();

        for (int i = 0; i < colliders.Length; i++)
        {
            for (int j = i + 1; j < colliders.Length; j++)
            {
                Physics.IgnoreCollision(colliders[i], colliders[j]);
            }
        }

        // Default all attack colliders to off
        for (int i = 0; i < attackColliders.Count; i++)
        {
            attackColliders[i].SetActive(false);
        }

        // Get components
        if (trans == null)
        {
            trans = transform;
        }

        if (anim == null)
        {
            anim = GetComponent<Animator>();
        }

        if (movement == null)
        {
            movement = GetComponent<EnemyMovement>();
        }

        if (health == null)
        {
            health = GetComponent<Health>();
        }

        if (inventory == null)
        {
            inventory = GetComponent<Inventory>();

            for (int i = 0; i < inventory.slots.Count; i++)
            {
                if (inventory.slots[i].name == weaponSlotName)
                {
                    weaponSlot = inventory.slots[i];
                    break;
                }
            }
        }

        if (spawnable == null)
        {
            spawnable = GetComponent<Spawnable>();
        }
    }

    private void OnEnable()
    {
        // Exponentially increase health with difficulty
        if (health != null)
        {
            health.maxHealth += health.maxHealth * difficultyScale * (GameStateSystem.Instance.difficulty - 1);
        }
    }

    /// <summary>
    /// Toggle an attack collider active or inactive.
    /// </summary>
    /// <param name="num">The id of the attack collider.</param>
    /// <param name="active">Set collider to active.</param>
    /// <returns>True if collider id existed. False otherwise.</returns>
    public bool ToggleAttackCollider(int num, bool active)
    {
        if (num >= 0 && num < attackColliders.Count)
        {
            attackColliders[num].SetActive(active);

            return true;
        }

        return false;
    }

    /// <summary>
    /// Set collider active to true.
    /// </summary>
    /// <param name="num">The id of the attack collider.</param>
    /// <returns>True if collider id existed. False otherwise.</returns>
    public bool ActivateAttackCollider(int num)
    {
        if (num >= 0 && num < attackColliders.Count)
        {
            attackColliders[num].SetActive(true);

            return true;
        }

        return false;
    }

    /// <summary>
    /// Set collider active to false.
    /// </summary>
    /// <param name="num">The id of the attack collider.</param>
    /// <returns>True if collider id existed. False otherwise.</returns>
    public bool DeactivateAttackCollider(int num)
    {
        if (num >= 0 && num < attackColliders.Count)
        {
            attackColliders[num].SetActive(false);

            return true;
        }

        return false;
    }

    /// <summary>
    /// Fire the equipped weapon in the weaponSlotName slot.
    /// </summary>
    public void UseWeapon()
    {
        if (weaponSlot != null && weaponSlot.equipment != null)
        {
            Weapon weapon = weaponSlot.equipment as Weapon;

            if (weapon != null)
            {
                weapon.Fire();
            }
        }
    }

    /// <summary>
    /// Trigger the enemy brain to start a death.
    /// </summary>
    public void Die()
    {
        if (anim != null)
        {
            anim.SetTrigger(fsmDeathTrigger);
        }
    }
}
