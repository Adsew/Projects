using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

[RequireComponent(typeof(Animator))]
[RequireComponent(typeof(NavMeshAgent))]
public class EnemyMovement : Movement {
    
    [HideInInspector]
    public Animator anim = null;
    [HideInInspector]
    public NavMeshAgent agent = null;

    public string animSpeedName = "";

    protected override void Awake()
    {
        base.Awake();

        agent = GetComponent<NavMeshAgent>();
        anim = GetComponent<Animator>();

        if (agent != null)
        {
            agent.speed = maxSpeed + bonusMaxSpeed;
            agent.acceleration = acceleration;
        }

#if DEBUG
        if (agent == null)
        {
            Debug.LogError(gameObject.name + " EnemyMovement: Missing NavMeshAgent.");
        }
        if (anim == null)
        {
            Debug.LogError(gameObject.name + " EnemyMovement: Missing Animator.");
        }
#endif
    }

    private void Update()
    {
        if (agent != null && anim != null)
        {
            agent.speed = Mathf.Max(0.0f, maxSpeed + bonusMaxSpeed);
            anim.SetFloat(animSpeedName, agent.velocity.magnitude / agent.speed);
        }
    }

    public override void Move(Vector3 position, Vector3 offset)
    {
        if (agent != null)
        {
            if (position + offset != agent.destination)
            {
                rb.velocity = Vector3.zero;
                agent.isStopped = false;
                agent.SetDestination(position + offset);
            }
        }
    }

    public void Stop()
    {
        if (agent != null)
        {
            agent.isStopped = true;
            agent.velocity = Vector3.zero;
        }
    }

    public void Resume()
    {
        if (agent != null)
        {
            agent.isStopped = false;
        }
    }
}
