using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WallCleaner : MonoBehaviour {

    [Tooltip("The wall will disable game objects of these types when hit.")]
    public List<string> objectsToDisable = new List<string>();

    [Tooltip("The wall will destroy game objects of these types when hit.")]
    public List<string> objectsToDestroy = new List<string>();

    private void OnTriggerEnter(Collider other)
    {
        // Walls will disable/destroy certain elements of the game as they go out of bounds
        // such as item spawns, pickups, and equipment
        for (int i = 0; i < objectsToDisable.Count; i++)
        {
            if (!string.IsNullOrEmpty(objectsToDisable[i]) && other.GetComponent(objectsToDisable[i]) != null)
            {
                other.gameObject.SetActive(false);

                return;
            }
        }

        for (int i = 0; i < objectsToDestroy.Count; i++)
        {
            if (!string.IsNullOrEmpty(objectsToDestroy[i]) && other.GetComponent(objectsToDestroy[i]) != null)
            {
                Destroy(other.gameObject);

                return;
            }
        }
    }
}
