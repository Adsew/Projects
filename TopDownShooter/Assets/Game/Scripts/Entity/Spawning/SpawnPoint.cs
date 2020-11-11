using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpawnPoint : MonoBehaviour {

    [HideInInspector]
    public Transform trans = null;

    private void Awake()
    {
        trans = transform;
    }

    private void Start()
    {
        // The wall will disable spawns so only those in the play field can spawn items
        // This will let spawns become active on new rounds.
        GameStateSystem.Instance.OnRespawn.AddListener(this.EnableSpawn);
        GameStateSystem.Instance.OnContinue.AddListener(this.EnableSpawn);
    }

    private void OnDestroy()
    {
        if (GameStateSystem.Exists)
        {
            GameStateSystem.Instance.OnRespawn.RemoveListener(this.EnableSpawn);
            GameStateSystem.Instance.OnContinue.RemoveListener(this.EnableSpawn);
        }
    }

    /// <summary>
    /// Enable the spawn's game object.
    /// </summary>
    public void EnableSpawn()
    {
        gameObject.SetActive(true);
    }

    /// <summary>
    /// Disable the spawn's game object.
    /// </summary>
    public void DisableSpawn()
    {
        gameObject.SetActive(false);
    }
}
