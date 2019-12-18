# 1v1 Demolition

1v1 Demolition is a simple 3D Racing Arena Shooter Local Multiplayer Game developed by Ángel González and Gerard Romeu, two 2nd year students of the Bachelor Degree in Video Game Design and Development at UPC CITM/TTC, Barcelona. It has been made from scratch in C++ with Visual Studio using the OpenGL and Bullet libraries.



# Goal of the Game

The goal of the game is, by the use of projectiles, destroy the other player's vehicle.  

* The first player to destroy the opponent's vehicle 3 times wins a round.
* The first player to win 3 rounds wins a game.
* The first player to win 3 games gets the win.



# Game Mechanics

**<u>Basic Movement:</u>**

* **Throttle** (W/KP 8) makes the vehicle move slowly but helps in aiming shots and making sharp turns.

* **Turbo** (L SHIFT/ KP Plus) propels the vehicle forward, which helps in moving around easily and avoiding the opponent's projectiles.

  

**<u>Lives:</u>**

* Each player starts with 3 lives. A life is subtracted when a player is hit by the opponent's projectiles. 
* When the amount of lives reach 0, the player loses and is reset back to the starting position. This grants the other player a round win. At 3 round wins the game ends and a new one starts.



**<u>Shooting:</u>**

* Each player starts with 4 projectiles and, once shot, they need to be recharged by reaching the nearest "ammo pack".  
* Ammo packs are white spheres spread out through the Arena. Once a player goes through one, it will change to red or blue (depends on the player).

* Player 1's projectiles are blue in colour and Player 2's are red. These colors change when the projectiles hit the opposing player (to red and blue respectively). 

* Projectiles that have changed colours are harmless to the players. Those who have not, however, can still damage them.



**<u>Other Mechanics:</u>**

* If for whatever reason a vehicle is flipped over, each player has a specific input key (E/KP 7) assigned to flip their vehicle back to its original state.

* Players' ammo, lives, round Wins and game wins can be checked out in the window title.
* Player's Round and Game wins are displayed in game, represented by coloured cubes.
  * Both displays will be located top-left for Player 1 and top-right for Player 2.
  * Player 1's Round Win cubes will be Blue while Game Win cubes will be Cyan.
  * Player 2's Round Win cubes will be Red while Game Win cubes will be Magenta.



# Controls

## Player 1

- **Basic Movement Keys:** 

  - <u>W Key:</u> Throttle / Accelerate.

  - <u>S Key:</u> Brake.

  - <u>A Key:</u> Turn Left. (The vehicle's Left)

  - <u>D Key:</u> Turn Right. (The vehicle's Right)

  - <u>Left Shift Key</u>: Turbo.

    

- **Special Actions:**

  - <u>Space Bar:</u> Use Item. (If Player 1 has any)

  - <u>E Key:</u> Flip over the vehicle. (Only in case it was turned upside down.)
  
    

## Player 2

- **Basic Movement Keys:** 

  - <u>8 Key (Keypad):</u> Throttle / Accelerate.

  - <u>5 Key (Keypad):</u> Brake.

  - <u>4 Key (Keypad):</u> Turn Left. (The vehicle's Left)

  - <u>6 Key (Keypad):</u> Turn Right. (The vehicle's Right)

  - <u>Plus Key (Keypad):</u> Turbo.

    

- **Special Actions:**

  - <u>0 Key (Keypad):</u> Use Item. (If Player 2 has any)

  - <u>7 Key (Keypad):</u> Flip over the vehicle. (Only in case it was turned upside down.)
  
    

# Debug Keys

* **F1**: Debug Mode (Spawn PhysBodies and Free Camera Movement)

  * <u>Free Camera Inputs:</u>
    
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

    

* **F2**: Enable/Disable Primitives Render.

* **F3**: Enable/Disable Respawn Players at Round End mode. It is enabled by default.

  * This Game Mode makes both players respawn  when a player is killed in a round. 

  * This mode prevents players from camping the other player at his/her respawn point.

    

* **R Key**: Resets everything back to the starting point (Vehicles, Round and Game wins...)

* **O Key**: Reset Player 1.

* **P Key**: Reset Player 2.

  

# Team Members

- **Ángel González:** https://github.com/BarcinoLechiguino
- **Gerard Romeu:** https://github.com/Gromeu2000



# Work Distribution

- **Ángel González:**  

   - Camera Implementation.

   - Camera Debug  and Camera Debug Key Implementation.

   - Game Loop & Win Lose Condition Implementation.

   - Constraints Implementation.

   - Sensor Implementation (Ammo Packs & Out Of Bounds Sensor).

   - Player 1 & Player 2 Implementation.

   - Arena / Circuit Design and Implementation.

   - Round Wins and Game Wins Display Implementation.

   - Respawn at Round's End mode Implementation.

     

- **Gerard Romeu:**

   - Song & FX implementation.
   
   - Vehicle Design.
   
   - Player 1 & Player 2 Vehicle Basic Movement Implementation.
   
     


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