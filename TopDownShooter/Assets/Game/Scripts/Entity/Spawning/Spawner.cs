using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class Spawner : MonoBehaviour {

    [System.Serializable]
    public class EntitySpawnParameter
    {
        public GameObject entity = null;
        public Vector2Int spawnBetween = new Vector2Int();
    }

    protected List<GameObject> liveEntities = new List<GameObject>();

    protected SpawnPoint[] _spawnPoints;    // For faster lookup with frequent use

    public List<SpawnPoint> spawnPoints = new List<SpawnPoint>();

    public bool fillSpawnsFromChildren = false;

    [Tooltip("Entities added here should have a Spawnable and PooledObject script attached.")]
    public List<EntitySpawnParameter> entities = new List<EntitySpawnParameter>();

    protected void Awake()
    {
        VerifySpawnPoints();
    }

    /// <summary>
    /// Spawn a random amount of enemies based on the list provided.
    /// </summary>
    public abstract void Spawn();

    /// <summary>
    /// Remove an enemy from the live entities list.
    /// </summary>
    /// <param name="e">Entity to remove.</param>
    /// <param name="delete">Dispose of entity when removing from spawner.</param>
    public abstract void Despawn(GameObject e, bool delete);
    
    /// <summary>
    /// Dispose of all live entities and return spawner to before first spawn.
    /// </summary>
    public abstract void ResetSpawner();

    /// <summary>
    /// Return entity to the spawner to cancel despawn.
    /// </summary>
    /// <param name="spawn"></param>
    public void Reaquire(Spawnable spawn)
    {
        if (spawn.spawnedMe == this && !liveEntities.Contains(spawn.gameObject))
        {
            liveEntities.Add(spawn.gameObject);
        }
    }

    /// <summary>
    /// Seek through premade spawns or generate spawns to create spawn point array.
    /// </summary>
    protected void VerifySpawnPoints()
    {
        if (fillSpawnsFromChildren)
        {
            _spawnPoints = GetComponentsInChildren<SpawnPoint>();
        }
        else
        {
            int i;

            for (i = spawnPoints.Count - 1; i >= 0; i--)
            {
                if (spawnPoints[i] == null)
                {
                    spawnPoints.RemoveAt(i);
                }
            }

            _spawnPoints = new SpawnPoint[spawnPoints.Count];

            for (i = 0; i < spawnPoints.Count; i++)
            {
                _spawnPoints[i] = spawnPoints[i];
            }
        }

        spawnPoints.Clear();
        spawnPoints = null;
    }
}
