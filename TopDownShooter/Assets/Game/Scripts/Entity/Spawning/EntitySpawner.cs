using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class EntitySpawner : Spawner {
    
    [Tooltip("With each spawn, increase the base and max spawnable of each entity by this amount.")]
    [Range(0, 10)]
    public int incMaxSpawnRate = 1;

    public UnityEvent OnEntitiesCleared = new UnityEvent();
    
    protected void Start()
    {
        GameStateSystem.Instance.OnRespawn.AddListener(ResetSpawner);
        GameStateSystem.Instance.OnRespawn.AddListener(Spawn);
        GameStateSystem.Instance.OnContinue.AddListener(Spawn);

        // Create base pools for listed entities and remove bad items
        for (int i = entities.Count - 1; i >= 0; i--)
        {
            if (entities[i].entity == null)
            {
                entities.RemoveAt(i);

                continue;
            }

            Spawnable spawnable = entities[i].entity.GetComponent<Spawnable>();
            PooledObject pooledObj = entities[i].entity.GetComponent<PooledObject>();

            if (spawnable != null && pooledObj != null)
            {
                int amountToPool = entities[i].spawnBetween.y;

                if (amountToPool < 0) { amountToPool = 0; }

                ObjectPool.Instance.CreatePool(entities[i].entity, amountToPool);
            }
            else
            {
#if DEBUG
                Debug.LogWarning(
                    gameObject.name + " EntitySpawner: Item " + entities[i].entity.name 
                    + " is missing either Spawnable or PooledObject scripts. Can not spawn."
                    );
#endif
                entities.RemoveAt(i);

                continue;
            }
        }
    }
    
    public override void Spawn()
    {
        for (int i = 0; i < entities.Count; i++)
        {
            if (entities[i].entity != null)
            {
                int numToSpawn = Random.Range(
                    entities[i].spawnBetween.x + (GameStateSystem.Instance.difficulty - 1) * incMaxSpawnRate,
                    entities[i].spawnBetween.y + (GameStateSystem.Instance.difficulty - 1) * incMaxSpawnRate
                    );

                if (numToSpawn <= 0)
                {
                    continue;
                }

                if (numToSpawn > _spawnPoints.Length) { numToSpawn = _spawnPoints.Length; }
                
                // Request and spawn entities
                GameObject[] newEntities = ObjectPool.Instance.Request(entities[i].entity, numToSpawn, true);
                Spawnable spawned = null;
                int randSpawner = 0;

                for (int j = 0; j < newEntities.Length && newEntities[j] != null; j++)
                {
                    spawned = newEntities[j].GetComponent<Spawnable>();
                    
                    if (spawned != null)
                    {
                        spawned.spawnedMe = this;
                    }

                    randSpawner = Random.Range(0, _spawnPoints.Length);

                    newEntities[j].transform.SetPositionAndRotation(
                        _spawnPoints[randSpawner].trans.position,
                        _spawnPoints[randSpawner].trans.rotation
                        );
                    
                    liveEntities.Add(newEntities[j]);

                    newEntities[j].SetActive(true);
                }

                // Prepare pool for next level spawn
                int desiredPoolIncrease =
                    entities[i].spawnBetween.y
                    + GameStateSystem.Instance.difficulty * incMaxSpawnRate
                    - ObjectPool.Instance.GetPoolQuantity(entities[i].entity);
                
                if (desiredPoolIncrease > 0)
                {
                    ObjectPool.Instance.AddToPool(entities[i].entity, desiredPoolIncrease);
                }
            }
        }
    }

    public override void Despawn(GameObject e, bool delete)
    {
        if (liveEntities.Contains(e))
        {
            if (delete)
            {
                PooledObject pooledObj = e.GetComponent<PooledObject>();

                if (pooledObj != null)
                {
                    pooledObj.ReturnToPool();
                }
            }

            liveEntities.Remove(e);

            if (liveEntities.Count <= 0)
            {
                OnEntitiesCleared.Invoke();
            }
        }
    }
    
    public override void ResetSpawner()
    {
        PooledObject pooledObj = null;

        for (int i = 0; i < liveEntities.Count; i++)
        {
            pooledObj = liveEntities[i].GetComponent<PooledObject>();

            if (pooledObj != null)
            {
                pooledObj.ReturnToPool();
            }
        }

        liveEntities.Clear();
    }
    
    protected void OnDestroy()
    {
        ResetSpawner();

        if (GameStateSystem.Instance != null)
        {
            GameStateSystem.Instance.OnRespawn.RemoveListener(ResetSpawner);
            GameStateSystem.Instance.OnRespawn.RemoveListener(Spawn);
            GameStateSystem.Instance.OnContinue.RemoveListener(Spawn);
        }
    }
}
