using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class Collidable : MonoBehaviour {

    private Collider[] myColliders = null;

    public enum Mode
    {
        Collide,
        Trigger
    }
    public Mode collisionType = Mode.Collide;

    public enum Occur
    {
        Enter,
        Stay,
        Exit
    }
    public Occur when = Occur.Enter;

    public enum Cleanup
    {
        None,
        DisableGameObject,
        DisableCollider,
        Destroy
    }
    public Cleanup cleanupMethod = Cleanup.None;

    [HideInInspector]
    public GameObject recentCollision = null;

    public LayerMask layers;
    
    public UnityEvent OnTrigger = new UnityEvent();

    private void Awake()
    {
        myColliders = GetComponents<Collider>();
    }
    
    /// <summary>
    /// Called when conditions are met to trigger collision events.
    /// </summary>
    /// <param name="other">Game object colling with this object.</param>
    private void OnSuccess(GameObject other)
    {
        if ((layers | (1 << other.gameObject.layer)) == layers)
        {
            recentCollision = other;

            OnTrigger.Invoke();

            recentCollision = null;

            if (cleanupMethod == Cleanup.DisableGameObject)
            {
                gameObject.SetActive(false);
            }
            if (cleanupMethod == Cleanup.DisableCollider)
            {
                for (int i = 0; i < myColliders.Length; i++)
                {
                    myColliders[i].enabled = false;
                }
            }
            else if (cleanupMethod == Cleanup.Destroy)
            {
                Destroy(gameObject);
            }
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        if (collisionType == Mode.Trigger && when == Occur.Enter)
        {
            OnSuccess(other.gameObject);
        }
    }

    private void OnTriggerStay(Collider other)
    {
        if (collisionType == Mode.Trigger && when == Occur.Stay)
        {
            OnSuccess(other.gameObject);
        }
    }

    private void OnTriggerExit(Collider other)
    {
        if (collisionType == Mode.Trigger && when == Occur.Exit)
        {
            OnSuccess(other.gameObject);
        }
    }

    private void OnCollisionEnter(Collision collision)
    {
        if (collisionType == Mode.Collide && when == Occur.Enter)
        {
            OnSuccess(collision.gameObject);
        }
    }

    private void OnCollisionStay(Collision collision)
    {
        if (collisionType == Mode.Collide && when == Occur.Stay)
        {
            OnSuccess(collision.gameObject);
        }
    }

    private void OnCollisionExit(Collision collision)
    {
        if (collisionType == Mode.Collide && when == Occur.Exit)
        {
            OnSuccess(collision.gameObject);
        }
    }
}
