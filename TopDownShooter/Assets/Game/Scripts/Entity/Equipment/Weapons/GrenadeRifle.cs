using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GrenadeRifle : Weapon
{
    public override void Fire()
    {
        if (bullet != null && !onCooldown && ammo > 0)
        {
            GameObject[] tempBullet = ObjectPool.Instance.Request(bullet, 1, true);

            if (tempBullet[0] != null)
            {
                Transform bulletTrans = tempBullet[0].transform;
                Vector3 newForward = barrel.forward;

                newForward.y = 0.05f;       // Give it a slight arc

                bulletTrans.forward = newForward.normalized;
                bulletTrans.position = barrel.position;

                tempBullet[0].SetActive(true);

                ammo--;

                StartCoroutine(Cooldown(1.0f / fireRate));
            }
        }
    }
}
