Graphics and Animations Final Assignment
Hidden Object Game

Members: James Brooks, Matthew Warner

HOW TO PLAY
----------------------------
WASD to move
Mouse to rotate camera
F to turn the flashlight on and off
Left click to pick up a hidden object within distance
TAB to bring up the high scores
ESC to exit at any time

GOAL
----------------------------
Find all 5 hidden toruses around the map.
Your time will be recorded when you win.
Objects spawn randomly each time your run the game.


Submission Notes
----------------------------
Everything is implemented fro mthe assignment document.

We have boxes that have two textures, wood, and a shiny metal border.

We have 5 different lights:
	- Spotlight which is the flashlight: low ambience, full specular, smooth transition using theta,phi, lambda
	- Directional light: low ambiance, angled to the ground and slightly forward, low specular, low diffuse
	- Light 1: No diffuse, not ambiance, full specular
	- Light 2: Full diffuse, low ambience, low specular
	- Light 3: No diffuse, high ambience, no specular
All lights use atinuation to cut off at a certain distance (except directional, which acts as the sun)
