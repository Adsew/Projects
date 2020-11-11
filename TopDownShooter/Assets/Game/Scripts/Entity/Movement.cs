using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class Movement : MonoBehaviour {

    public Transform trans { get; private set; }
    public Rigidbody rb { get; private set; }

    [Header("Movement")]

    [SerializeField]
    [Tooltip("Units per second.")]
    protected float maxSpeed = 1.0f;
    public float MaxSpeed { get { return maxSpeed; } private set { } }

    protected float speed = 0.0f;

    public float acceleration = 2.0f;

    [HideInInspector]
    public float bonusMaxSpeed = 0.0f;     // External sources add and remove speed here.

    protected virtual void Awake()
    {
        trans = transform;
        rb = GetComponent<Rigidbody>();
        
#if DEBUG
        if (rb == null)
        {
            Debug.LogWarning(gameObject.name + " Movement: GameObject is missing rigidbody.");
        }
#endif
    }

    /// <summary>
    /// Each update, move character towards desired direction and face the desired rotation
    /// </summary>
    /// <param name="direction"></param>
    /// <param name="rotation"></param>
	public abstract void Move(Vector3 direction, Vector3 rotation);
}
