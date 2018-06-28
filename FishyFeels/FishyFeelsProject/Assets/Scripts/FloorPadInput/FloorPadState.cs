using System.Collections;
using System.Collections.Generic;

public class FloorPadState
{
	public bool[] tiles;
	public List<int> pressed;
	public List<int> released;

	public FloorPadState ()
	{
		tiles = new bool[100];
		pressed = new List<int> ();
		released = new List<int> ();
	}

	public FloorPadState Copy ()
	{
		FloorPadState to = new FloorPadState ();
		tiles.CopyTo (to.tiles, 0);
		to.pressed = new List<int> (pressed);
		to.released = new List<int> (released);
		return to;
	}
}
