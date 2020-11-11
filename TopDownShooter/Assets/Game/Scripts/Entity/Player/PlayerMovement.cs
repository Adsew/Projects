using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerMovement : Movement {

    [Header("Aim")]

    [Tooltip("Time in seconds to rotate towards desired direction.")]
    [Range(0.01f, 1.0f)]
    public float timeToRotate = 0.05f;

    [Header("Animation")]

    private Animator anim = null;

    public string animForwardMove;
    public string animRightMove;

    protected void Start()
    {
        anim = GetComponent<Animator>();

#if DEBUG
        if (anim == null)
        {
            Debug.LogWarning(gameObject.name + " PlayerMovement: GameObject has no animator. No animation will be played.");
        }

        if (string.IsNullOrEmpty(animForwardMove) || string.IsNullOrEmpty(animRightMove))
        {
            Debug.LogWarning(gameObject + " PlayerMovement: Missing names for animation control variables.");
        }
#endif
    }

    public override void Move(Vector3 direction, Vector3 rotation)
    {
        if (rb != null && trans != null)
        {
            // Lower threshhold
            if (direction.magnitude < 0.05f && speed <= 0.01f)
            {
                speed = 0.0f;
            }
            else
            {
                // Allow finer movement by checking joystick is only slightly moved
                float mockMaxSpeed = (maxSpeed + Mathf.Max(0.0f, bonusMaxSpeed)) * direction.magnitude;

                if (speed < mockMaxSpeed)
                {
                    speed += acceleration * Time.deltaTime;

                    if (speed > mockMaxSpeed)
                    {
                        speed = mockMaxSpeed;
                    }
                }
                else if (speed > mockMaxSpeed)
                {
                    speed -= acceleration * Time.deltaTime;

                    if (speed < 0.0f)
                    {
                        speed = 0.0f;
                    }
                }
            }

            rb.MovePosition(trans.position + direction.normalized * speed * Time.deltaTime);

            // Keep rotation same if no input
            if (rotation.magnitude == 0.0f)
            {
                rotation = trans.forward;
            }

            trans.forward = Vector3.Slerp(trans.forward, rotation.normalized, Time.deltaTime / timeToRotate);

            // Implement animation
            if (anim != null)
            {
                direction = direction.normalized;

                Vector3 relativeDirection = new Vector3(
                    direction.x * trans.right.x - direction.z * trans.forward.x,
                    0.0f,
                    direction.z * trans.forward.z - direction.x * trans.right.z
                    );

                anim.SetFloat(animForwardMove, relativeDirection.z * (speed / maxSpeed));
                anim.SetFloat(animRightMove, relativeDirection.x * (speed / maxSpeed));
            }
        }
    }
}
