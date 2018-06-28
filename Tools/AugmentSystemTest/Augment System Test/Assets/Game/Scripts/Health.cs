using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Health : MonoBehaviour {

    public int hp = 100;

	public void takeDamage(int dmg) {

        hp -= dmg;
    }
}
