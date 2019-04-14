/* ******************************************************************************** */
/* GameEngine.h                                                                     */
/*                                                                                  */
/* Game is driven using the physics module and applying pool game rules             */
/*                                                                                  */
/* ******************************************************************************** */
#include "stdafx.h"

#pragma once

#include "Physics.h"


class CGameEngine
{
private:
	CPhysics		cPoolPhysics;		// instance of the physics class, into which all of the game entities are linked
	void			InitPoolGeometry(); // initialize static math objects
	void			InitPoolEntities(); // initialize math objects
	void			InitForces();		// initialize external forces which act upon pool objects
	void			MoveCuestick(float offset);  // move cue stick in "stroke" mode with accordance to last mouse movement
	void			ResetCuestick();	// reset cue stick position to cue ball and camera eye point position
	void			ResetDynamics();	// reset all object velocities
	CForce*			pGravity;			// external force, globally active throughout the simulation
	enum GameMode
	{
		Aim,		// default mode, allowing rotation while the table is at rest
		Stroke,		// no rotation is allowed, cue stick is moved back and forth to apply stroke
		Running,	// simulation in progress, cue stick can not be moved
	} eGameMode;
	D3DXVECTOR3		vImpactCueStickPosition; // position of the cue stick at the time of impact

	// pool table geometry which is active during simulation
	CMathObjectLine*	pRail[18];
	int				iNumOfBalls;		// total number of balls in the game

public:
	CGameEngine();
	bool				bActive;		// whether the game is running
	CMathObjectPlane*	pPoolTable;		// pool game entity (pool table surface)
	CMathObject*		pPoolTableCenter; // object corresponding to graphical table (referenced by graphics engine only)
	CMathObjectSphere*	pPoolBall[16];	// pool game entities (pool balls)
	CMathObjectLine*	pCueStick;		// pool game entity (cue stick)

	void				SetupTable();	// starting transformation
	void				RotateView(LONG ofsX, LONG ofsY, LONG ofsZ, BYTE buttons[8]); // rotate camera view according to mouse movement

	// methods which handle game states
	void				SwitchStrokeMode();

	void				StepSimulation(float dtime); // step physics simulator for a given time interval

	void				Cleanup();		// release all memory reserved by the module
};
