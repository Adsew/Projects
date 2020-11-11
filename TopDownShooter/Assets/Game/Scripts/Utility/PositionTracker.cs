using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PositionTracker : MonoBehaviour {

    private Transform trans = null;

    public Transform trackedObject = null;

    public Vector3 offset = new Vector3();

    public enum TrackMode
    {
        Position,
        LookAt,
        Off
    }
    public TrackMode mode = TrackMode.Position;

    [Tooltip("Higher speed sticks closer to the target.")]
    public float trackSpeed = 10.0f;

    private void Awake()
    {
        trans = transform;
    }

    void FixedUpdate ()
    {
		if (trackedObject != null)
        {
            switch (mode)
            {
                case TrackMode.Position:
                    
                    trans.position += (trackedObject.position + offset - trans.position) * trackSpeed * Time.fixedDeltaTime;
                    break;

                case TrackMode.LookAt:

                    trans.LookAt(trackedObject.position + offset);
                    break;
            }
        }
	}
}
