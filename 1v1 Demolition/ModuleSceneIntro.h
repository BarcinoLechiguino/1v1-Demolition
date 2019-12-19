#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

class Primitive;
class PhysBody3D;
class PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2) override;

public:
	void CameraMovement();																			//Method that is responsible of moving the camera around. Done for readability
	float GetZoom() const;																			//Gets the amount of zoom required taking into account the distance between players (ratio).

	void AddPrimitive(Primitive* p);																//Adds a primitive to the primitives dynArray.
	void DeletePrimitive(Primitive* p);																//Deletes a primitive from the primitives dynArray.
	void DeleteBody(Primitive* p);																	//Deletes a body. It just sends it really far and sets its mass to not 0 and as not an environment. Sorry for this dirty fix.
	void AddTorque(Primitive* p, vec3 torque);														//Adds torque to a primitive's body. Done to increase readability.

	Cube* SetCube(const vec3& position, const vec3& size, float mass, float angle, const vec3& axis, bool is_sensor = false, bool is_environment = false);						//Creates and sets a cube on the world. Used to set Arena elements.
	Sphere* SetSphere(const vec3& position, float radius, float mass, bool is_sensor = false, bool is_environment = false);														//Creates and sets a sphere on the world. Used to set Arena elements.
	Cylinder* SetCylinder(const vec3& position, float radius, float height, float mass, float angle, const vec3& axis, bool is_sensor = false, bool is_environment = false);	//Creates and sets a sphere on the world. Used to set Arena elements.

	void InitGame();																				//Initializes all arena and game related variables.
	void LoadArena();																				//Loads all Arena elements. Done for readability.
	void ApplyTorqueToConstrainedElements();														//Applies torque to all free constrained elements.
	void ClearConstraintElements();																	//Clears all constrain elements from the Arena (Columns and rotors).
	
	void CheckProjectileCount();																	//Safety measure to prevent projectile overpopulation in the arena.
	void ClearProjectiles();																		//When the amount of projectiles spawned reach a certain point they are all deleted.

	void CheckWins();																			//Checks the amount of rounds each player has won.
	void RoundWinsDisplay();																		//Depending on the amount of round wins each player has, it will display more or less elements.
	void GameWinsDisplay();																			//Depending on the amount of wins each player has, it will display more or less elements.
	void ClearDisplayElements();																	//Clears all currently existing display elements
	void RestartGame();																				//Resets everyithing except Arena elements and game wins.
	void RestartEverything();																		//Resets everyithing back to the starting point.

public:
	Cube* centerLeft_Rotor;																			//Pointer of the element constrained to the Center-Left column. Done so torque can be added in the update.
	Cube* centerRight_Rotor;																		//Pointer of the element constrained to the Center-Right column. Done so torque can be added in the update.
	Cube* northWest_Rotor;																			//Pointer of the element constrained to the North-West column. Done so torque can be added in the update.
	Cube* southEast_Rotor;																			//Pointer of the element constrained to the South-East column. Done so torque can be added in the update.

	Cube* roundWinCube1_P1;																			//Pointer of the element assigned to display that P1 has won 1 round. 
	Cube* roundWinCube2_P1;																			//Pointer of the element assigned to display that P1 has won 2 rounds. 
	Cube* roundWinCube3_P1;																			//Pointer of the element assigned to display that P1 has won 3 rounds. 
	Cube* gameWinCube1_P1;																			//Pointer of the element assigned to display that P1 has won 1 game. 
	Cube* gameWinCube2_P1;																			//Pointer of the element assigned to display that P1 has won 2 games. 
	Cube* gameWinCube3_P1;																			//Pointer of the element assigned to display that P1 has won 3 games. 

	Cube* roundWinCube1_P2;																			//Pointer of the element assigned to display that P2 has won 1 round.
	Cube* roundWinCube2_P2;																			//Pointer of the element assigned to display that P2 has won 2 rounds.
	Cube* roundWinCube3_P2;																			//Pointer of the element assigned to display that P2 has won 3 rounds.
	Cube* gameWinCube1_P2;																			//Pointer of the element assigned to display that P2 has won 1 game.
	Cube* gameWinCube2_P2;																			//Pointer of the element assigned to display that P2 has won 2 games.
	Cube* gameWinCube3_P2;																			//Pointer of the element assigned to display that P2 has won 3 games.

	Cube* outOfBoundsSensor;

	Primitive* furniture;																			//Multiuse pointer that is used in the SetCube/Sphere/Cylinder methods.	

	int projectileCount;																			//Amount of projectiles spawned in the arena.
	bool RespawnOnRoundEnd;																			//Keeps track whether or not the car's positions are reset back to their spawn positions.

private:
	void HandleDebugInput();																		//Method that holds all the inputs that are activated when in debug mode.
	void DebugSpawnPrimitive(Primitive* p);															//Spawns an element on the world. Used in debug mode.

	p2DynArray<Primitive*> primitives;																//Array containing all elements that are currently existing in the world.
};
