using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlatformMaker : Singleton <PlatformMaker>
{

    private Dictionary<Vector2Int, GameObject> platforms = new Dictionary<Vector2Int, GameObject>();

    public List<GameObject> platformTypes = new List<GameObject>();

    public GameObject[] popInOutEffects;

    public GameObject bird;

    // Update is called once per frame
    void Update()
    {

        growPlatformUpdate();
        createPlatformUpdate();
    }

    // Check through tile input and grow or shrink platforms appropriately
    private void growPlatformUpdate()
    {

        bool[,] tiles = InputGridManager.This.getActiveTiles();
        IsPlatform platform = null;
        List<KeyValuePair<Vector2Int, GameObject>> toDelete = new List<KeyValuePair<Vector2Int, GameObject>>();

        foreach (KeyValuePair<Vector2Int, GameObject> p in platforms)
        {

            platform = p.Value.GetComponent<IsPlatform>();

            if (platform != null)
            {

                // Grow the platform if still pushed, shrink otherwise
                if (tiles[p.Key.x, p.Key.y])
                {

                    platform.grow();
                }
                // If shrink fails, platform is too small thus delete
                else if (!platform.shrink())
                {

                    toDelete.Add(p);
                }
            }
            else
            {

                toDelete.Add(p);
            }
        }

        // Now remove from dictionary (avoids errors when deleting while iterating)
        foreach (KeyValuePair<Vector2Int, GameObject> p in toDelete)
        {

            //if (popInOutEffect != null)
            //{

            //    GameObject pop = Instantiate(popInOutEffect);
            //    pop.transform.position = p.Value.transform.position;
            //}

            platforms.Remove(p.Key);

            Destroy(p.Value);
        }
    }

    // Check through pressed tile input and create missing platforms
    private void createPlatformUpdate()
    {

        if (platformTypes.Count > 0)
        {

            bool[,] tiles = InputGridManager.This.getActiveTiles();

            for (int i = 0; i < InputGridManager.gridSize; i++)
            {
                for (int j = 0; j < InputGridManager.gridSize; j++)
                {

                    // If platform does not exist at this point create it
                    if (tiles[i, j] && !platforms.ContainsKey(new Vector2Int(i, j)))
                    {                 /// ACCOUNT FOR MULTIPLE TILES IN ON AREA PRESSED
                        int type = (int)(Random.value * platformTypes.Count);
                        GameObject newPlat = Instantiate(platformTypes[type]);

                        // Set the position data
                        if (newPlat != null)
                        {

                            IsPlatform platScript = newPlat.GetComponent<IsPlatform>();
                            Vector3 worldPos = new Vector3();

                            worldPos.x = i + 0.5f;
                            worldPos.y = j + 0.5f;

                            newPlat.transform.position = worldPos;

                            if (platScript == null)
                            {

                                platScript = newPlat.AddComponent<IsPlatform>();
                            }

                            platScript.tilePosition = new Vector2Int(i, j);

                            platforms.Add(platScript.tilePosition, newPlat);
                        }

                        if (popInOutEffects != null)
                        {

                            GameObject pop = Instantiate(popInOutEffects[type]);
                            pop.transform.position = newPlat.transform.position;
                        }
                    }
                }
            }
        }


    }
    public void GiveFish(GameObject p)
    {
        Debug.Log("ah.");
        Dictionary<GameObject, Vector2Int> InversePlatforms = new Dictionary<GameObject, Vector2Int>();
        foreach (KeyValuePair<Vector2Int, GameObject> entry in platforms)
        {
            InversePlatforms.Add(entry.Value, entry.Key);
        }

        Vector2Int targetPosition = InversePlatforms[p];
        Vector2 fishSpawnPos = new Vector2(targetPosition.x + 0.5f, targetPosition.y + 0.8f);


        bird.SendMessage("GiveFish", fishSpawnPos);

    }
}
