using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CloudSpawner : MonoBehaviour {

    private float timer = 0;
    private float timeBetweenClouds = 3.0f;

    public List<GameObject> objectsToSpawn = new List<GameObject>();
    public List<GameObject> spawnLocations = new List<GameObject>();

    public float timeBetweenCloudsMin = 3.0f;
    public float timeBetweenCloudsMax = 5.0f;

    // Use this for initialization
    void Start () {

        
	}
	
	// Update is called once per frame
	void Update () {

        timer += Time.deltaTime;

        if (timer >= timeBetweenClouds)
        {
            timer = 0.0f;
            timeBetweenClouds = Random.Range(timeBetweenCloudsMin, timeBetweenCloudsMax);

            if (spawnLocations.Count > 0 && objectsToSpawn.Count > 0)
            {
                GameObject spawnedObj = Instantiate(objectsToSpawn[Random.Range(0, objectsToSpawn.Count)]);

                if (spawnedObj != null)
                {
                    spawnedObj.transform.position = spawnLocations[Random.Range(0, spawnLocations.Count)].transform.position;
                }
            }
        }
	}
}
