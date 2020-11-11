using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : Singleton<Player> {

    public Transform trans = null;
    public Health health = null;
    public PlayerController pControl = null;
    
    [HideInInspector]
    public bool isDead = false;
    
    private void Start()
    {
        if (Camera.main != null)
        {
            PositionTracker tracker = Camera.main.GetComponent<PositionTracker>();

            if (tracker != null)
            {
                tracker.trackedObject = transform;
            }
        }
#if DEBUG
        else
        {
            Debug.LogError(gameObject + " Player: No main camera exists. Can not capture camera.");
        }
#endif
    }

    public void OnDeath()
    {
        isDead = true;
    }
}
