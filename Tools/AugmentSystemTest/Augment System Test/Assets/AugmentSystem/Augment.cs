using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class Augment : ScriptableObject {

    [HideInInspector]
    public AugmentBoard board = null;
    [HideInInspector]
    public bool needsToBeAdded = true;

    [TextArea]
    public string comment;

    [HideInInspector]
    public float timer = 0.0f;
    public float duration = 0.0f;

    /// <summary>
    /// Override this to allow interaction with curently applied augments.
    /// </summary>
    public virtual void OnInteraction(Augment other) { }

    /// <summary>
    /// Override this to perform an action when applied. 
    /// Occurs after interacting with currently applied augments.
    /// </summary>
    public virtual void OnApply() { }

    /// <summary>
    /// Override this to perform some action every frame.
    /// </summary>
    public virtual void OnUpdate() { }

    /// <summary>
    /// Override to perform an action when this augment naturally expires
    /// </summary>
    public virtual void OnFinish() { }
}
