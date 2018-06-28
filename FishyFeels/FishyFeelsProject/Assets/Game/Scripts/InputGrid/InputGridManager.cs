using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

[RequireComponent(typeof(FloorPadInputManager))]
public class InputGridManager : MonoBehaviour {

    private bool[,] activeTiles = null;

    public static InputGridManager This = null;

    public static readonly int gridSize = 10;

    public Tilemap testingGrid = null;
    public Sprite activeTileImage = null;

    public bool debugColours = false;   // Only used when tilemap clicking mode is active

    void Start() {
        
        // Make singleton instance accessable anywhere
        if (This == null) {

            This = this;

            activeTiles = new bool[gridSize, gridSize];
        }
    }

    // Update is called once per frame
    void Update () {
        
        if (testingGrid == null) {

            tilesPressedUpdate();
        }
        else {

            testingPressedUpdate();
        }
    }

    // Clear grid and mark the new currently pressed grids
    private void tilesPressedUpdate() {
        
        activeTiles = new bool[gridSize, gridSize];
        List<Vector2> tilesPressed = FloorPadInput.GetPressedCoordinates();
        
        foreach (Vector2 tile in tilesPressed) {

            activeTiles[(int)tile.x, (int)tile.y] = true;
        }
    }

    // Use a tilemap grid to detect tiles being pressed and released
    private void testingPressedUpdate() {

        if (Input.GetMouseButtonDown(0)) {

            Vector3 worldClickPos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
            Vector3Int pos = new Vector3Int();
            
            pos.x = (int)worldClickPos.x;
            pos.y = (int)worldClickPos.y;

            // Only take input from within bounds of real input grid
            if (pos.x < gridSize && pos.x >= 0 && pos.y >= 0 && pos.y < gridSize) {

                if (!testingGrid.GetTile(pos)) {

                    Tile debugTile = ScriptableObject.CreateInstance<Tile>();

                    if (debugColours) {
                        
                        debugTile.sprite = activeTileImage;
                        debugTile.color = new Color(255, 0, 0, 50);
                    }

                    testingGrid.SetTile(pos, debugTile);

                    activeTiles[pos.x, pos.y] = true;
                }
                else {

                    Destroy(testingGrid.GetTile(pos));
                    testingGrid.SetTile(pos, null);

                    activeTiles[pos.x, pos.y] = false;
                }
            }
        }
    }

    public bool[,] getActiveTiles() {

        return activeTiles;
    }
}
