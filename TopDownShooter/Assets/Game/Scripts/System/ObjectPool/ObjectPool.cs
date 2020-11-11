using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectPool : Singleton<ObjectPool> {

    private class _PooledInstance
    {
        public GameObject instance;
        public bool inUse;

        public _PooledInstance(GameObject go)
        {
            instance = go;
            inUse = false;
        }
    }

	private class _Pool
    {
        public Transform container;
        public List<_PooledInstance> instances;
        public int quantity;
        public int spawnPerFrame = 1;
        public bool pooling;

        public _Pool(GameObject objectPool, GameObject prefab)
        {
            GameObject tempGO = new GameObject("OP_" + prefab.name);
            container = tempGO.transform;
            container.parent = objectPool.transform;

            instances = new List<_PooledInstance>();
            quantity = 0;
            pooling = false;
        }
    }

    [System.Serializable]
    private struct PoolStartup
    {
        public GameObject prefab;   // What to spawn in pool.
        public int poolSize;        // Size of pool.
        public int spawnPerFrame;   // Increase number the more lightweight prefab is to instantiate.
    }

    [SerializeField]
    private List<PoolStartup> startupPools = new List<PoolStartup>();
    
    /// <summary>
    /// Keys should be a prefab reference to ensure pools of similar game objects are not duplicated.
    /// </summary>
    private Dictionary<GameObject, _Pool> pools = new Dictionary<GameObject, _Pool>();

    private int numPoolsPooling = 0;

    private void Awake()
    {
        // Create initial pools
        for (int i = 0; i < startupPools.Count; i++)
        {
            if (startupPools[i].prefab != null && startupPools[i].poolSize >= 0)
            {
                CreatePool(startupPools[i].prefab, startupPools[i].poolSize, startupPools[i].spawnPerFrame);
            }
        }

        startupPools.Clear();
        startupPools = null;
    }

    /// <summary>
    /// Create a new object pool of the given prefab.
    /// </summary>
    /// <param name="prefab">Prefab to make instances of.</param>
    /// <param name="amount">Number of instances initially in the pool.</param>
    /// <param name="spawnPerFrame">Higher number fills pool faster per frame. Only increase if prefab is lightweight.</param>
    /// <returns>True on successful pool creation, false if prefab is null or pool already exists.</returns>
    public bool CreatePool(GameObject prefab, int amount, int spawnPerFrame = 1)
    {
        if (!pools.ContainsKey(prefab) && prefab != null && amount >= 0)
        {
            _Pool newPool = new _Pool(gameObject, prefab);

            newPool.quantity = amount;

            if (spawnPerFrame < 1)
            {
                spawnPerFrame = 1;
            }

            newPool.spawnPerFrame = spawnPerFrame;

            pools.Add(prefab, newPool);

            StartCoroutine(FillPool(prefab, newPool));

            return true;
        }

        return false;
    }

    /// <summary>
    /// Request desired instances of prefab.
    /// </summary>
    /// <param name="prefab">The prefab identifying the game objects requested.</param>
    /// <param name="amount">Number of instances requested.</param>
    /// <param name="fillToDemand">If not enough instances are available, expand the pool to meet request next call.</param>
    /// <returns>Array size of 'amount' filled with up to 'amount' instances. Instance is still disabled when returned.</returns>
    public GameObject[] Request(GameObject prefab, int amount = 1, bool fillToDemand = false)
    {
        GameObject[] requisition = new GameObject[amount];

        if (amount > 0 && pools.ContainsKey(prefab))
        {
            _Pool pool = pools[prefab];

            if (pool != null)
            {
                int requCount = 0;

                // Find unused instances to return
                for (int i = 0; i < pool.instances.Count; i++)
                {
                    if (!pool.instances[i].inUse)
                    {
                        requisition[requCount] = pool.instances[i].instance;
                        pool.instances[i].instance.transform.parent = null;
                        pool.instances[i].inUse = true;

                        requCount++;

                        if (requCount >= amount)
                        {
                            break;
                        }
                    }
                }

                // If couldnt fill order, add more instances in prep for next call
                if (requCount < amount && fillToDemand)
                {
                    AddToPool(prefab, amount - requCount);
                }
            }
        }

        return requisition;
    }

    /// <summary>
    /// Return an instance back to the given pool.
    /// </summary>
    /// <param name="prefab">Prefab representing the pool.</param>
    /// <param name="returnInstance">The game object instance being returned.</param>
    /// <returns>True if the instance was deactivated and returned to the given pool, false otherwise.</returns>
    public bool Return(GameObject prefab, GameObject returnInstance)
    {
        _Pool pool = null;
        
        if (pools.TryGetValue(prefab, out pool))
        {
            for (int i = 0; i < pool.instances.Count; i++)
            {
                if (pool.instances[i].instance == returnInstance)
                {
                    IPoolable[] resetables = returnInstance.GetComponents<IPoolable>();

                    for (int j = 0; j < resetables.Length; j++)
                    {
                        resetables[j].ResetPoolable();
                    }

                    pool.instances[i].instance.transform.parent = pool.container;
                    pool.instances[i].instance.SetActive(false);
                    pool.instances[i].inUse = false;

                    return true;
                }
            }
        }

        return false;
    }
    
    /// <summary>
    /// Add or remove amount of instances from pool. Instances may not be available immediatly.
    /// </summary>
    /// <param name="prefab">Prefab of pool to alter.</param>
    /// <param name="amount">Amount of instances to add or remove.</param>
    /// <returns>True on successful addition or subtraction.</returns>
    public bool AddToPool(GameObject prefab, int amount)
    {
        _Pool pool = null;
        
        if (pools.TryGetValue(prefab, out pool))
        {
            pool.quantity += amount;

            if (amount > 0)
            {
                StartCoroutine(FillPool(prefab, pool));
            }
            else if (amount < 0)
            {
                // Use delete pool to fully remove a pool
                if (pool.quantity < 1)
                {
                    pool.quantity = 1;
                }

                StartCoroutine(ReducePool(pool));
            }
        }

        return false;
    }

    /// <summary>
    /// Check if pool has created instances equal to the quantity.
    /// </summary>
    /// <param name="prefab">Prefab of pool to check.</param>
    /// <returns>True if pool instances equals quantity. False if pool doesn't exist or is less than quantity.</returns>
    public bool CheckPoolFilled(GameObject prefab)
    {
        _Pool pool = null;

        if (pools.TryGetValue(prefab, out pool))
        {
            if (pool.instances.Count == pool.quantity)
            {
                return true;
            }
        }

        return false;
    }

    /// <summary>
    /// Returns maximum quantity of the given pool.
    /// </summary>
    /// <param name="prefab">Prefab representing the pool to check.</param>
    /// <returns>Quantity of the pool, or -1 if it does not exist.</returns>
    public int GetPoolQuantity(GameObject prefab)
    {
        _Pool pool = null;

        if (pools.TryGetValue(prefab, out pool))
        {
            return pool.quantity;
        }

        return -1;
    }

    /// <summary>
    /// Internal. Fills pool to the quantity of pool.
    /// </summary>
    /// <param name="prefab">Prefab to create instances from.</param>
    /// <param name="pool">Pool to be filled with instances.</param>
    /// <returns>Coroutine information.</returns>
    private IEnumerator FillPool(GameObject prefab, _Pool pool)
    {
        // This pool is already filling
        if (pool.pooling)
        {
            yield break;
        }

        GameObject newInst = null;

        pool.pooling = true;
        numPoolsPooling++;

        // Each run, add a new instance until max acheived.
        // This accounts for if more are requested while still pooling.
        // Dynamically account if there is room to spawn more or less per frame.
        while (pool.instances.Count < pool.quantity)
        {
            // (number dynamic frames per fixed frame) / pools pooling
            // Goal is to maximize spawning per dynamic frame without exceeding fixed frame limit
            float dynamicFrames = (1.0f / (Time.deltaTime / Time.fixedDeltaTime)) / (float)numPoolsPooling;

            int numToPoolInFrame = Mathf.Max(1, (int)dynamicFrames);
            numToPoolInFrame *= pool.spawnPerFrame;
            numToPoolInFrame = Mathf.Min(numToPoolInFrame, pool.quantity - pool.instances.Count);

            for (int i = 0; i < numToPoolInFrame; i++)
            {
                newInst = Instantiate(prefab, pool.container);
                newInst.SetActive(false);

                PooledObject poScript = newInst.GetComponent<PooledObject>();

                if (poScript != null)
                {
                    poScript.prefab = prefab;   // Must give reference via script
                }

                pool.instances.Add(new _PooledInstance(newInst));
            }
            
            yield return null;
        }

        pool.pooling = false;
        numPoolsPooling--;
    }

    /// <summary>
    /// Destroys instances of a pool to reduce to the stored quantity. Will only remove unused instances.
    /// </summary>
    /// <param name="pool">Pool to reduce the size of.</param>
    /// <returns>Coroutine information.</returns>
    private IEnumerator ReducePool(_Pool pool)
    {
        if (pool.instances.Count >= pool.quantity)
        {
            for (int i = pool.instances.Count; i >= 0 && pool.instances.Count > pool.quantity; i--)
            {
                if (!pool.instances[i].inUse)
                {
                    Destroy(pool.instances[i].instance);
                    pool.instances.RemoveAt(i);
                }
            }
        }

        yield break;
    }
}
