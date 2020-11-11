using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Spawnable : MonoBehaviour {

    [HideInInspector]
    public Spawner spawnedMe = null;

    private void OnDestroy()
    {
        RemoveFromSpawner(false);
    }

    /// <summary>
    /// Remove entity from management by the spawner.
    /// </summary>
    /// <param name="delete">Tell spawner to delete this when removed.</param>
    public void RemoveFromSpawner(bool delete)
    {
        if (spawnedMe != null)
        {
            spawnedMe.Despawn(gameObject, delete);
        }
    }

    /// <summary>
    /// Return this item to management by the spawner.
    /// </summary>
    public void ReturnToSpawner()
    {
        if (spawnedMe != null)
        {
            spawnedMe.Reaquire(this);
        }
    }
}
