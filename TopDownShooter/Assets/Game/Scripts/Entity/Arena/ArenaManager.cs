using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ArenaManager : MonoBehaviour {

    [SerializeField]
    private List<Rigidbody> walls = new List<Rigidbody>();

    private List<Vector3> origins = new List<Vector3>();
    
    private void Awake()
    {
        // Create a clean list in case of errors while tracking origin points
        List<Rigidbody> tempWalls = new List<Rigidbody>();

        for (int i = 0; i < walls.Count; i++)
        {
            if (walls[i] != null)
            {
                tempWalls.Add(walls[i]);
                origins.Add(walls[i].position);
            }
        }

        walls.Clear();
        walls = tempWalls;

        GameStateSystem.Instance.OnRespawn.AddListener(this.OnRestart);
        GameStateSystem.Instance.OnContinue.AddListener(this.OnRestart);
    }

    private void Update ()
    {
        if (GameStateSystem.Instance.playing && !Player.Instance.isDead)
        {
            float percTimeRemain = GameStateSystem.Instance.levelTimer / GameStateSystem.Instance.maxLevelTime;

            if (percTimeRemain <= 0.0f)
            {
                Player.Instance.health.ApplyToHealth(-Player.Instance.health.maxHealth);
            }
            else
            {
                Vector3 newPosition = new Vector3();

                for (int i = 0; i < walls.Count; i++)
                {
                    if (walls[i] != null)
                    {
                        // Walls always head to 0,0,0 + 1 unit from center (as walls squish into a central column)
                        newPosition = (origins[i] - origins[i].normalized) * percTimeRemain + origins[i].normalized;

                        walls[i].MovePosition(newPosition);
                    }
                }
            }
        }
	}

    private void OnDestroy()
    {
        if (GameStateSystem.Instance != null)
        {
            GameStateSystem.Instance.OnRespawn.RemoveListener(this.OnRestart);
            GameStateSystem.Instance.OnContinue.RemoveListener(this.OnRestart);
        }
    }
    
    private void OnRestart()
    {
        for (int i = 0; i < walls.Count; i++)
        {
            if (walls[i] != null)
            {
                walls[i].position = origins[i];
            }
        }
    }
}
