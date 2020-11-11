using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public abstract class Pickup : MonoBehaviour {

    public UnityEvent OnPickup = new UnityEvent();

    private void OnTriggerEnter(Collider other)
    {
        if (Effect(other.gameObject))
        {
            Destroy(gameObject);
        }
    }

    /// <summary>
    /// Determine if the target can be affected by the pickup. If so, enact pickup effect.
    /// </summary>
    /// <param name="target">Target being affected by pickup.</param>
    /// <returns>True if target can be affected, false otherwise. Pickup self-destroys if true is returned.</returns>
    protected abstract bool Effect(GameObject target);
}
