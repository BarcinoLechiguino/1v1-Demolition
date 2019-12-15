# 1v1 Demolition

1v1 Demolition is a simple 3D Racing Brawling Game developed by Ángel González and Gerard Romeu, two 2nd year students of the Bachelor Degree in Video Game Design and Development at UPC CITM/TTC, Barcelona. It has been made from scratch in C++ with Visual Studio using the OpenGL and Bullet libraries.



# Goal of the Game

The goal of the game is, by the use of projectiles, destroy the other player's car. 



# Game Mechanics

Each player starts with 3 lives. A life is subtracted when a player is hitted by the opponent's projectiles. When the amount of lives reach 0, the player loses and is reset back to the starting position.

Each player starts with 4 projectiles and, once shot, they need to be recharged by reaching the nearest "ammo pack". 

Player 1's projectiles are blue in colour and Player 2's are red. These colors change when the projectiles hit the opposing player (to red and blue respectively). 

Projectiles that have changed colours are harmless to the players. Those who have not, however, can still damage them.

If for whatever reason a vehicle is flipped over, each player has a specific input key assigned to flip their vehicle back to its original state.

Normal acceleration (W/8) makes the vehicle move slowly, and turbo (LSHIFT/ KP Plus) propels the vehicle forward,



# Controls

## Player 1

- **Basic Movement Keys:** 

  - W Key: Throttle / Accelerate.
  - S Key: Brake.
  - A Key: Turn Left. (The vehicle's Left)
  - D Key: Turn Right. (The vehicle's Right)
  - Left Shift Key: Turbo.

- **Special Actions:**

  - Space Bar: Use Item. (If Player 1 has any)

  - E Key: Flip over the vehicle. (In case it was upside down.)
  
    

## Player 2

- **Basic Movement Keys:** 

  - 8 Key (Keypad): Throttle / Accelerate.
  - 5 Key (Keypad): Brake.
  - 4 Key (Keypad): Turn Left. (The vehicle's Left)
  - 6 Key (Keypad): Turn Right. (The vehicle's Right)
  - + Key (Keypad): Turbo.

- **Special Actions:**

  - 0 Key (Keypad): Use Item. (If Player 2 has any)

  - 7 Key: Flip over the vehicle. (In case it was upside down.)
  
    

# Debug Keys

* **F1**: Debug Mode (Show PhysBodies, spawn PhysBodies and Free Camera Movement)

  * Free Camera Inputs:
    * **S Key**: Increase the camera's position in the Z Axis.
    * **W Key**: Decrease the camera's position in the Z Axis.
    * **D Key**: Increase the camera's position in the X Axis.
    * **A Key**: Decrease the camera's position in the X Axis
    * **R Key**: Increase the camera's position in the Y Axis.
    * **F Key**: Decrease the camera's position in the Y Axis.
  * **1 Key**: Spawn Sphere.
  * **2 Key**: Spawn Cube.
  * **3 Key**: Spawn Cylinder.
  * **4 Key**: Randomize all free bodies' positions.
  * **5 Key**: Add a random force too all free bodies.

* **R Key**: Reset the game/match.

* **O Key**: Reset Player 1.

* **P Key**: Reset Player 2.

  

# Team Members

- **Ángel González:** https://github.com/BarcinoLechiguino
- **Gerard Romeu:** https://github.com/Gromeu2000



# Work Distribution





# Link to Repository

<https://github.com/BarcinoLechiguino/3D-Racing-Game>


# License

MIT License

Copyright (c) [2019] [Ángel González, Gerard Romeu]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.