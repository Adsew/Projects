using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AugmentBoard : MonoBehaviour {

    private List<Augment> augments = new List<Augment>();
    private List<Augment> finishingAugments = new List<Augment>();

    /// <summary>
    /// Attach a new augment to this board.
    /// </summary>
    /// <param name="aug">The augment to apply</param>
    public void ApplyAugment(Augment aug) {

        aug.board = this;

        for (int i = 0; i < augments.Count; i++) {

            aug.OnInteraction(augments[i]);
        }

        // It is possible augments interacting with their own type
        // prevent the need to apply twice, and rather combine,
        // so check if it still needs to be applied.
        if (aug.needsToBeAdded) {

            aug.OnApply();
            augments.Add(aug);
        }
    }

    // Use this for initialization
    //void Start () {
		
	//}
	
	// Update is called once per frame
	void Update () {

        int i = 0;

        // Enact effect for applied augments
        for (i = 0; i < augments.Count; i++) {

            augments[i].timer += Time.deltaTime;

            augments[i].OnUpdate();

            if (augments[i].timer > augments[i].duration) {

                finishingAugments.Add(augments[i]);
                augments.RemoveAt(i);
            }
        }

        // Complete any augments that have reached their end
        for (i = 0; i < finishingAugments.Count; i++) {

            finishingAugments[i].OnFinish();
        }

        finishingAugments.Clear();
	}
}
