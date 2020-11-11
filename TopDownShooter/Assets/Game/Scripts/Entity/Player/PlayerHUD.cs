using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlayerHUD : MonoBehaviour {

    [Header("UI Elements")]

    [SerializeField]
    private Text timerText = null;

    [SerializeField]
    private Text scoreText = null;

    [SerializeField]
    private Image healthBar = null;

    [SerializeField]
    private Image ammoBar = null;

    [SerializeField]
    private Text ammoText = null;

    [SerializeField]
    private Image equipIcon = null;
    [SerializeField]
    private Sprite defaultEquipIcon = null;

    private Weapon currentWeapon = null;

    private void Update()
    {
        if (Player.Instance != null)
        {
            // Timer
            if (timerText != null && GameStateSystem.Exists)
            {
                int minutes = (int)GameStateSystem.Instance.levelTimer / 60;
                int seconds = (int)GameStateSystem.Instance.levelTimer % 60;

                if (minutes <= 0)
                {
                    timerText.text = seconds + "s";
                }
                else
                {
                    timerText.text = minutes + "m" + seconds + "s";
                }
            }
            
            // Score
            if (scoreText != null && ScoreKeeper.Exists)
            {
                scoreText.text = "SCORE\n" + (ScoreKeeper.Instance.score + ScoreKeeper.Instance.roundScore);
            }

            // Health
            if (healthBar != null)
            {
                healthBar.fillAmount = Player.Instance.health.HP / Player.Instance.health.maxHealth;
            }

            // Weapon
            if (ammoBar != null && ammoText != null && equipIcon != null)
            {
                Weapon tempWeapon = Player.Instance.pControl.weaponSlot.equipment as Weapon;

                if (tempWeapon != currentWeapon)
                {
                    currentWeapon = tempWeapon;

                    if (currentWeapon == null)
                    {
                        ammoBar.fillAmount = 0;
                        ammoText.text = "0\n/0";
                        equipIcon.sprite = defaultEquipIcon;
                    }
                    else
                    {
                        equipIcon.sprite = currentWeapon.icon;
                    }
                }

                if (currentWeapon != null)
                {
                    ammoBar.fillAmount = (float)currentWeapon.ammo / (float)currentWeapon.maxAmmo;
                    ammoText.text = currentWeapon.ammo + "\n/" + currentWeapon.maxAmmo;
                }
            }
        }
    }
}
