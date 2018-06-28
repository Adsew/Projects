using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SplineObject : MonoBehaviour {

    public Spline spline = null;

    public float t;         // Position on the spline
    public float oldT;      // Temp t of previous frame used to calculate distance
    public float lastT;     // Value of t on previous frame

    [HideInInspector]
    public float pingpongSign = 1.0f;

    [Tooltip("Prevents the spline object from updating on the spline.")]
    public bool disabled = false;

    [Tooltip("If true, object will go to other end of the spline when passing the ends. (Does not effect loop splines)")]
    public bool loopAtEnds = true;

    public bool allowCollision = false;
    public bool useRigidbodyMovement = false;

    [Header("Spline Effects")]
    public bool x = true;
    public bool y = true;
    public bool z = true;

    public void SetSpline(Spline s) {

        RemoveFromSpline();

        spline = s;

        spline.AddHead(this);
    }

    public void SetSplineToClosestPoint(Spline s) {

        RemoveFromSpline();

        spline = s;

        spline.AddHeadAtPoint(this, spline.GetClosestPoint(this.transform.position));
    }

    public void SetSplineToClosestLocation(Spline s) {

        RemoveFromSpline();

        spline = s;

        spline.AddHeadAtClosestLocation(this);
    }

    public void RemoveFromSpline() {

        if (spline != null) {

            spline.RemoveHead(this);
            spline = null;
        }
    }
    
    public void OnCollisionEnter(Collision collision) {

        if (allowCollision) {
            
            RaycastHit hit;

            Vector3 origin = transform.position;
            float distance = 2.0f;

            //Determine collider being used
            BoxCollider box = GetComponent<BoxCollider>();

            if (box != null) {

                origin += box.center;
                distance = box.size.z;
            }
            else {

                SphereCollider sphere = GetComponent<SphereCollider>();

                if (sphere != null) {

                    origin += sphere.center;
                    distance = sphere.radius;
                }
            }

            //Check for collision in front of spline object to determin if it needs to stop
            if (Physics.Raycast(new Ray(origin, transform.forward), out hit, distance)) {

                //t += (lastT - t) * 23.0f;

                t = lastT;
                oldT = lastT;
            }
        }
    }
    
    public void OnCollisionEnter2D(Collision2D collision) {

        if (allowCollision) {

            t += (lastT - t) * 23.0f;
        }
    }
}
