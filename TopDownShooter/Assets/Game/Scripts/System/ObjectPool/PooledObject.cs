using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class PooledObject : MonoBehaviour {

    [HideInInspector]
    public GameObject prefab = null;

    [Tooltip("Add events to reset prefab when returned to pool using this script.")]
    public UnityEvent OnReturnToPool = new UnityEvent();

    /// <summary>
    /// Reclaim this game object in its object pool.
    /// </summary>
	public void ReturnToPool()
    {
        OnReturnToPool.Invoke();

        if (ObjectPool.Exists)
        {
            ObjectPool.Instance.Return(prefab, gameObject);
        }
    }
}
