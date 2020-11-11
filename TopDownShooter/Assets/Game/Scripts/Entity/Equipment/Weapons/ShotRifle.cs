using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShotRifle : Weapon
{
    [SerializeField]
    private Vector3 recallPosition = new Vector3();

    public override void Fire()
    {
        if (bullet != null && !onCooldown)
        {
            GameObject[] tempBullet = ObjectPool.Instance.Request(bullet, 1, true);

            if (tempBullet[0] != null)
            {
                Transform bulletTrans = tempBullet[0].transform;
                Vector3 newForward = barrel.forward;

                newForward.y = 0.0f;

                bulletTrans.forward = newForward.normalized;
                bulletTrans.position = barrel.position;

                tempBullet[0].SetActive(true);

                StartCoroutine(Cooldown(1.0f / fireRate));
            }
        }
    }

    public override void OnUnequip(Inventory inventory)
    {
        transform.position = recallPosition;    // Special gun is always available
    }
}
