using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityOSC;
using System;

public static class FloorPadInput
{
		private static FloorPadState live, current, previous;
		private static int gridSize = 10;
		// Third party OSC library used to receive OSC packets
		private static OSCServer _OSCServer;

		public enum TileColor
		{
				briteGreen, // yellowy green 
				calypso, // magenta
				marina, // blue 
				portabella, // light brown 
				undefined
		}

		private static List<int> tileColors = new List<int>{1,2,0,1,0,2,3,0,2,0,2,3,3,2,1,3,1,3,0,2,1,0,2,0,3,0,2,1,2,1,3,2,3,1,0,1,0,2,3,0,0,1,0,2,1,2,1,2,0,1,0,2,1,3,2,3,0,1,2,3,1,0,3,1,1,2,3,2,3,0,3,1,2,0,3,0,2,0,1,1,0,3,2,1,0,1,0,1,3,2,2,1,0,3,2,2,1,3,0,1};

		public static void Initialize ()
		{
				live = new FloorPadState ();
				current = new FloorPadState ();
				previous = new FloorPadState ();

				// ScreenPlay floor sends OSC packets on port 5000
				if (_OSCServer == null) {
						_OSCServer = new OSCServer (5000);
						_OSCServer.PacketReceivedEvent += OnPacketReceived;
				}
		}

		public static bool GetTile (int x, int y)
		{
				int index = GetIndex (x, y);
				return current.tiles [index];
		}

		public static TileColor GetTileColor (int x, int y)
		{
				int index = GetIndex (x, y);
				return (TileColor)tileColors [index];
		}

		public static TileColor GetTileColor (Vector2 coords)
		{
				int index = GetIndex ((int)coords.x, (int)coords.y);
				return (TileColor)tileColors [index];
		}

		public static bool GetTileDown (int x, int y)
		{
				int index = GetIndex (x, y);
				return current.tiles [index] && !previous.tiles [index];
		}

		public static bool GetTileUp (int x, int y)
		{
				int index = GetIndex (x, y);
				return !current.tiles [index] && previous.tiles [index];
		}

		public static List<Vector2> GetPressedCoordinates ()
		{
				List<Vector2> coords = new List<Vector2> ();
				foreach (int index in current.pressed) {
						coords.Add (GetCoordinates (index));
				}
				return coords;
		}

		public static List<Vector2> GetReleasedCoordinates ()
		{
				List<Vector2> coords = new List<Vector2> ();
				for (int y = 0; y < gridSize; y++) {
						for (int x = 0; x < gridSize; x++) {
								if (!GetTile (x, y)) { 
										coords.Add (new Vector2 (x, y));
								}
						}
				}
				return coords;
		}

		public static List<Vector2> GetPressedCoordinates (TileColor tileColor)
		{
        //We will never use this
				List<Vector2> coords = new List<Vector2> ();
				for (int y = 0; y < gridSize; y++) {
						for (int x = 0; x < gridSize; x++) {
								if (GetTile (x, y) && GetTileColor (x, y) == tileColor) { 
										coords.Add (new Vector2 (x, y));
								}
						}
				}
				return coords;
		}
	
		public static List<Vector2> GetReleasedCoordinates (TileColor tileColor)
		{
				List<Vector2> coords = new List<Vector2> ();
				for (int y = 0; y < gridSize; y++) {
						for (int x = 0; x < gridSize; x++) {
								if (!GetTile (x, y) && GetTileColor (x, y) == tileColor) { 
										coords.Add (new Vector2 (x, y));
								}
						}
				}
				return coords;
		}
	
		public static int GetPressedCount ()
		{
				return current.pressed.Count;
		}

		public static int GetReleasedCount ()
		{
				return gridSize * gridSize - current.pressed.Count;
		}

		private static Vector2 GetCoordinates (int index)
		{
				int x = (gridSize - 1) - (index / (gridSize));
				int y = index % (gridSize);
				return new Vector2 (x, y);
		}
	
		private static int GetIndex (int x, int y)
		{
				return  (gridSize - 1 - x) * gridSize + y;
		}

		private static bool GetTileDown (int index)
		{
				return current.tiles [index] && !previous.tiles [index];
		}
	
		private static bool GetTileUp (int index)
		{
				return !current.tiles [index] && previous.tiles [index];
		}
	
		public static void Update ()
		{
				lock (live) {
						// clean up live released if in both current and live
						for (int i=live.released.Count-1; i>=0; i--) {
								if (current.released.Contains (live.released [i])) {
										live.released.Remove (live.released [i]);
								}
						}

						previous = current.Copy ();
						current = live.Copy ();
				}
		}

		public static void GetEvents (GameObject receiver)
		{
				foreach (int index in current.pressed) {
						if (GetTileDown (index)) { 
								receiver.SendMessage ("OnTileDown", GetCoordinates (index), SendMessageOptions.DontRequireReceiver);
						}
				}

				foreach (int index in current.released) {
						if (GetTileUp (index)) {
								receiver.SendMessage ("OnTileUp", GetCoordinates (index), SendMessageOptions.DontRequireReceiver);
						}
				}
		}
	
		public static void OnQuit ()
		{
				_OSCServer.Close ();
		}
	
		static void OnPacketReceived (OSCServer server, OSCPacket packet)
		{
				lock (live) {
						// OSC address is of the format /1/pushXXX, where XXX indicates the tile whose
						// state is changing, from 1-100
						string address = packet.Address;
						string push = "push";
						int pushIndex = address.IndexOf (push);
						int pushLength = push.Length;
						int tileNumber = Convert.ToInt32 (address.Substring (pushIndex + pushLength));
						float sensorState = (float)packet.Data [0];
						//Debug.Log ("OSC RX: " + tileNumber + " State: " + sensorState);
						bool state = System.Convert.ToBoolean (sensorState);
						int index = tileNumber - 1;

						live.tiles [index] = state;

						if (state) {
								if (!live.pressed.Contains (index)) {
										live.pressed.Add (index);
										if (live.released.Contains (index)) {
												live.released.Remove (index);
										}
								}
						} else {
								if (!live.released.Contains (index)) {
										live.released.Add (index);
										if (live.pressed.Contains (index)) {
												live.pressed.Remove (index);
										}
								}
						}

				}
		}
}