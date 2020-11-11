using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Rotate : MonoBehaviour {

    private Transform trans = null;

    public Vector3 speed = new Vector3();

    public bool active = true;

    // Use this for initialization
    void Awake()
    {
        trans = transform;
    }

    // Update is called once per frame
    void Update()
    {
        if (active)
        {
            if (speed.x != 0.0f)
            {
                trans.Rotate(Vector3.right, speed.x * Time.deltaTime);
            }
            if (speed.y != 0.0f)
            {
                trans.Rotate(Vector3.up, speed.y * Time.deltaTime);
            }
            if (speed.z != 0.0f)
            {
                trans.Rotate(Vector3.forward, speed.z * Time.deltaTime);
            }
        }
    }

    public void SetSpeed(Vector3 s)
    {
        speed = s;
    }

    public void AddToSpeed(Vector3 add)
    {
        speed += add;
    }

    public void Activate()
    {
        active = true;
    }

    public void Deactivate()
    {
        active = false;
    }

    public void ToggleActive()
    {
        active = !active;
    }
}
