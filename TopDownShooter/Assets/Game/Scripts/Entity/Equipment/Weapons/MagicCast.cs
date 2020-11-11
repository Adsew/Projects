using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MagicCast : Weapon
{
    public override void Fire()
    {
        if (bullet != null && barrel != null)
        {
            GameObject[] magic = ObjectPool.Instance.Request(bullet, 1, true);
            
            if (magic[0] != null)
            {
                magic[0].transform.SetPositionAndRotation(barrel.position, barrel.rotation);
                magic[0].SetActive(true);
            }
        }
    }
}
