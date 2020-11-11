using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class DealsDamage : MonoBehaviour {

    private Transform trans;

    public float damage;

    public enum Application
    {
        OnHit,
        Constant,
        OnFinish,
        OnFunctionCall,
        Never
    }
    public Application hitType;

    public enum ForceType
    {
        Directional,
        ExplosionRelative
    }
    public ForceType forceType = ForceType.Directional;

    public Vector3 force = new Vector3();

    public bool destroyOnHit = true;
    public bool disableOnHit = false;

    public UnityEvent OnHitSuccess = new UnityEvent();

    private void Start()
    {
        trans = transform;
    }

    public void OnTriggerEnter(Collider other)
    {
        if (hitType == Application.OnHit)
        {
            DealDamage(other.gameObject);
        }
    }

    public void OnTriggerStay(Collider other)
    {
        if (hitType == Application.Constant)
        {
            DealDamage(other.gameObject);
        }
    }

    public void OnTriggerExit(Collider other)
    {
        if (hitType == Application.OnFinish)
        {
            DealDamage(other.gameObject);
        }
    }
    
    public void OnCollisionEnter(Collision collision)
    {
        if (hitType == Application.OnHit)
        {
            DealDamage(collision.gameObject);
        }
    }

    public void OnCollisionStay(Collision collision)
    {
        if (hitType == Application.Constant)
        {
            DealDamage(collision.gameObject);
        }
    }

    public void OnCollisionExit(Collision collision)
    {
        if (hitType == Application.OnFinish)
        {
            DealDamage(collision.gameObject);
        }
    }

    public void DealDamage(GameObject target)
    {
        Health otherHealth = target.GetComponent<Health>();

        if (otherHealth != null)
        {
            Rigidbody otherRB = target.GetComponent<Rigidbody>();

            // Ignore force if no rigidbody
            if (otherRB != null && force.magnitude > 0.0f)
            {
                if (forceType == ForceType.Directional)
                {
                    // Original
                    //Vector3 relativeForce = new Vector3(
                    //force.x,
                    //force.y * trans.up.y - force.z * trans.forward.y,
                    //force.z * trans.forward.z - force.y * trans.up.z
                    //);

                    Vector3 relativeForce = new Vector3(
                    force.x * trans.right.x + force.z * trans.forward.x,
                    force.y,
                    force.z * trans.forward.z - force.x * trans.right.z
                    );
                    
                    //Vector3 relativeDirection = new Vector3(
                    //direction.x * trans.right.x - direction.z * trans.forward.x,
                    //0.0f,
                    //direction.z * trans.forward.z - direction.x * trans.right.z
                    //);

                    otherRB.velocity = relativeForce / otherRB.mass;
                }
                else if (forceType == ForceType.ExplosionRelative)
                {
                    Vector3 dirToRBForward = (otherRB.position - trans.position).normalized;
                    Vector3 dirToRBRight = new Vector3(dirToRBForward.z, dirToRBForward.y, dirToRBForward.x).normalized;

                    Vector3 relativeForce = new Vector3(
                    force.x * dirToRBForward.z + force.z * dirToRBForward.x,
                    force.y,
                    force.z * dirToRBForward.z - force.x * dirToRBForward.x
                    );
                    
                    otherRB.velocity = relativeForce / otherRB.mass;
                }
            }

            if (hitType == Application.Constant)
            {
                otherHealth.ApplyToHealth(-damage * Time.deltaTime);
            }
            else
            {
                otherHealth.ApplyToHealth(-damage);
            }

            OnHitSuccess.Invoke();

            if (destroyOnHit)
            {
                Destroy(gameObject);
            }

            if (disableOnHit)
            {
                gameObject.SetActive(false);
            }
        }
    }
}
