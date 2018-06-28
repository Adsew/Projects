using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName = "Fire Augment", menuName = "Augments/Fire")]
public class FireAug : Augment {

    private Health targetHealth = null;

    public int damagePerTick = 1;

    public int tickRate = 3;

    public override void OnInteraction(Augment other) {

        // If already on fire, esentially reset the fire and take the stronger one
        // Stronger damage takes presendence.
        if (other.GetType() == GetType()) {

            FireAug fireOther = other as FireAug;
            
            if (fireOther.damagePerTick < damagePerTick) {

                fireOther.damagePerTick = damagePerTick;
                fireOther.duration = duration;
            }

            fireOther.timer -= duration; // Add this fires duration to the time remaining
            if (fireOther.timer < 0.0f) { fireOther.timer = 0.0f; }

            needsToBeAdded = false;
        }
    }
    
    public override void OnApply() {

        targetHealth = board.GetComponent<Health>();
    }
    
    public override void OnUpdate() {

        if (targetHealth != null) {

            if ((int)timer % tickRate == 0) {

                targetHealth.takeDamage(damagePerTick);
            }
        }
    }
    
    //public override void OnFinish() {


    //}
}
