using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Shotgun : Weapon {

    [Tooltip("Number of bullets fired per shot.")]
    [Range(1, 20)]
    public int bulletsPerShot = 5;

    [Tooltip("Angle of fire the bullets will come out.")]
    [Range(0.0f, 180.0f)]
    public float spread = 80.0f;

    public override void Fire()
    {
        if (bullet != null && !onCooldown && ammo > 0)
        {
            GameObject[] tempBullet = ObjectPool.Instance.Request(bullet, bulletsPerShot, true);
            Vector3 barrelDir = barrel.forward;
            Transform bulletTrans = null;

            barrelDir.y = 0.0f;
            barrelDir = barrelDir.normalized;

            barrel.forward = barrelDir;
            barrel.Rotate(Vector3.up, -spread / 2.0f);

            for (int i = 0; i < tempBullet.Length && tempBullet[i] != null; i++)
            {
                bulletTrans = tempBullet[i].transform;

                bulletTrans.forward = barrel.forward;
                bulletTrans.position = barrel.position;

                tempBullet[i].SetActive(true);

                barrel.Rotate(Vector3.up, spread / bulletsPerShot);
            }

            barrel.forward = barrelDir;

            ammo--;

            StartCoroutine(Cooldown(1.0f / fireRate));
        }
    }
}
