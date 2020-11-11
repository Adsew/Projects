using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class BulletMovement : Movement, IPoolable
{
    protected bool stopped = false;

    public override void Move(Vector3 direction, Vector3 rotation)
    {
        if (!stopped)
        {
            if (speed < maxSpeed)
            {
                speed += acceleration * Time.fixedDeltaTime;

                if (speed > maxSpeed)
                {
                    speed = maxSpeed;
                }
            }

            trans.forward = direction;

            rb.MovePosition(trans.position + trans.forward * speed * Time.fixedDeltaTime);
        }
    }

    /// <summary>
    /// While set true, movement wont move or accelerate. Does not reset values.
    /// </summary>
    /// <param name="stop">Stops the movement.</param>
    public void SetStopped(bool stop)
    {
        stopped = stop;
    }

    public void ResetPoolable()
    {
        speed = 0.0f;
        stopped = false;
    }

    private void FixedUpdate()
    {
        Move(trans.forward, trans.forward);
    }
}
