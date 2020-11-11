using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerSpawn : MonoBehaviour {

	// Use this for initialization
	void Awake ()
    {
        if (GameStateSystem.Instance != null)
        {
            GameStateSystem.Instance.SetStartPosition(transform);
        }
    }

    private void Start()
    {
        GameStateSystem.Instance.SetStartPosition(transform);
    }
}
