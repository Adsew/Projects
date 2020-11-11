using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class PeriodicSpawner : Spawner {

    protected List<GameObject> liveRequiredEntities = new List<GameObject>();

    protected float nextSpawnTime = 0.0f;

    [Tooltip("There must be at least one item from this list live at any time. If not, spawner will prioritize these items.")]
    public List<EntitySpawnParameter> requiredEntities = new List<EntitySpawnParameter>();

    [Tooltip("The interval new items will spawn into the field.")]
    public float spawnInterval = 5.0f;

    [Tooltip("This overrides the individual spawn between field.")]
    public int numberToSpawn = 1;
    [Tooltip("Max amount of items allowed on the field.")]
    public int spawnedCap = 10;

    protected void Start()
    {
        GameStateSystem.Instance.OnRespawn.AddListener(ResetSpawner);
        GameStateSystem.Instance.OnContinue.AddListener(ResetSpawner);
        
        // Clean list of bad items
        for (int i = entities.Count - 1; i >= 0; i--)
        {
            if (entities[i].entity == null)
            {
                entities.RemoveAt(i);

                continue;
            }

#if DEBUG
            Spawnable spawnable = entities[i].entity.GetComponent<Spawnable>();

            if (spawnable == null)
            {
                Debug.LogWarning(
                    gameObject.name + " EntitySpawner: Item " + entities[i].entity.name
                    + " is missing Spawnable script. Can not spawn it."
                    );
                entities.RemoveAt(i);
            }
#endif
        }
    }

    private void Update()
    {
        if (GameStateSystem.Instance.playing)
        {
            if (nextSpawnTime >= GameStateSystem.Instance.levelTimer)
            {
                if (liveEntities.Count + liveRequiredEntities.Count < spawnedCap
                    || liveRequiredEntities.Count <= 0)
                {
                    Spawn();
                }

                nextSpawnTime -= spawnInterval;
            }
        }
    }

    public override void Spawn()
    {
        GameObject newSpawn = null;
        int entitiesToSpawn = numberToSpawn;
        int randEntity = 0;
        int randSpawnPoint = 0;

        // Ensure that, if needed, at least 1 required entity on the field will be spawned
        if (liveRequiredEntities.Count <= 0)
        {
            randEntity = Random.Range(0, requiredEntities.Count);
            randSpawnPoint = Random.Range(0, _spawnPoints.Length);

            int i = 0;

            // Verify spawn is still active, find one that is
            while (!_spawnPoints[randSpawnPoint].gameObject.activeSelf && i < _spawnPoints.Length)
            {
                randSpawnPoint = (randSpawnPoint + 1) % _spawnPoints.Length;
                i++;
            }

            // No spawns available
            if (i >= _spawnPoints.Length)
            {
                return;
            }

            newSpawn = Instantiate(
                requiredEntities[randEntity].entity,
                _spawnPoints[randSpawnPoint].trans.position,
                _spawnPoints[randSpawnPoint].trans.rotation);

            Spawnable spawnable = newSpawn.GetComponent<Spawnable>();

            if (spawnable != null)
            {
                spawnable.spawnedMe = this;
            }

            liveRequiredEntities.Add(newSpawn);
            newSpawn.SetActive(true);

            entitiesToSpawn--;
        }
        
        // Now randomly spawn rest
        while (entitiesToSpawn > 0)
        {
            // Gives more equal distribution treating lists as one
            randEntity = Random.Range(0, entities.Count + requiredEntities.Count);
            randSpawnPoint = Random.Range(0, _spawnPoints.Length);

            int i = 0;

            // Verify spawn is still active, find one that is
            while (!_spawnPoints[randSpawnPoint].gameObject.activeSelf && i < _spawnPoints.Length)
            {
                randSpawnPoint = (randSpawnPoint + 1) % _spawnPoints.Length;
                i++;
            }

            // No spawns available
            if (i >= _spawnPoints.Length)
            {
                return;
            }
            
            if (randEntity >= entities.Count)
            {
                newSpawn = Instantiate(
                    requiredEntities[randEntity - entities.Count].entity,
                    _spawnPoints[randSpawnPoint].trans.position,
                    _spawnPoints[randSpawnPoint].trans.rotation);

                liveRequiredEntities.Add(newSpawn);
            }
            else
            {
                newSpawn = Instantiate(
                    entities[randEntity].entity,
                    _spawnPoints[randSpawnPoint].trans.position,
                    _spawnPoints[randSpawnPoint].trans.rotation);

                liveEntities.Add(newSpawn);
            }

            Spawnable spawnable = newSpawn.GetComponent<Spawnable>();

            if (spawnable != null)
            {
                spawnable.spawnedMe = this;
            }

            newSpawn.SetActive(true);

            entitiesToSpawn--;
        }
    }
    
    public override void Despawn(GameObject e, bool delete)
    {
        if (liveEntities.Contains(e))
        {
            liveEntities.Remove(e);

            if (delete)
            {
                Destroy(e);
            }
        }
        else if (liveRequiredEntities.Contains(e))
        {
            liveRequiredEntities.Remove(e);

            if (delete)
            {
                Destroy(e);
            }
        }
    }
    
    public override void ResetSpawner()
    {
        for (int i = 0; i < liveEntities.Count; i++)
        {
            Destroy(liveEntities[i]);
        }

        liveEntities.Clear();

        for (int i = 0; i < liveRequiredEntities.Count; i++)
        {
            Destroy(liveRequiredEntities[i]);
        }

        liveRequiredEntities.Clear();

        if (GameStateSystem.Exists)
        {
            nextSpawnTime = GameStateSystem.Instance.maxLevelTime;
        }
    }

    protected void OnDestroy()
    {
        ResetSpawner();

        if (GameStateSystem.Instance != null)
        {
            GameStateSystem.Instance.OnRespawn.RemoveListener(ResetSpawner);
            GameStateSystem.Instance.OnContinue.RemoveListener(ResetSpawner);
        }
    }
}
