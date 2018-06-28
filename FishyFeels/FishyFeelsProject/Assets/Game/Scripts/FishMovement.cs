using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FishMovement : MonoBehaviour {

    public void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.GetComponentInChildren<CatController>())
        {
            Vector3 nextPos = new Vector3();

            nextPos.x = Random.Range(0.0f, (float)InputGridManager.gridSize);
            nextPos.y = Random.Range(1.0f, (float)InputGridManager.gridSize - 1.0f);
            nextPos.y = (int)nextPos.y + 0.5f;

            transform.position = nextPos;
        }
    }
}
