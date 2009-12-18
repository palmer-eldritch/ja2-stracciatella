#include "Event_Manager.h"
#include "HImage.h"
#include "Init.h"
#include "Local.h"
#include "Screens.h"
#include "Strategic_Movement_Costs.h"
#include "Sys_Globals.h"
#include "VObject.h"
#include "RenderWorld.h"
#include "MouseSystem.h"
#include "WorldDef.h"
#include "Animation_Data.h"
#include "Overhead.h"
#include "Font_Control.h"
#include "Timer_Control.h"
#include "Radar_Screen.h"
#include "Render_Dirty.h"
#include "Sound_Control.h"
#include "Lighting.h"
#include "Cursor_Control.h"
#include "Video.h"
#include "Interface_Items.h"
#include "Dialogue_Control.h"
#include "Text.h"
#include "Laptop.h"
#include "NPC.h"
#include "MercTextBox.h"
#include "Tile_Cache.h"
#include "StrategicMap.h"
#include "Map_Information.h"
#include "Exit_Grids.h"
#include "Summary_Info.h"
#include "GameSettings.h"
#include "Game_Init.h"
#include "Strategic_Movement.h"
#include "Vehicles.h"
#include "Shading.h"
#include "VSurface.h"

#if defined JA2BETAVERSION && defined JA2EDITOR
#	include "EditScreen.h"
#	include "JAScreens.h"
#endif


GameMode g_game_mode = GAME_MODE_GAME;


// The InitializeGame function is responsible for setting up all data and Gaming Engine
// tasks which will run the game


ScreenID InitializeJA2(void)
try
{
	gfWorldLoaded = FALSE;

	// Load external text
	LoadAllExternalText();

	gsRenderCenterX = 805;
	gsRenderCenterY = 805;

	// Init animation system
	InitAnimationSystem();

	// Init lighting system
	InitLightingSystem();

	// Init dialog queue system
	InitalizeDialogueControl();

	InitStrategicEngine();

	//needs to be called here to init the SectorInfo struct
	InitStrategicMovementCosts( );

	// Init tactical engine
	InitTacticalEngine();

	// Init timer system
	//Moved to the splash screen code.
	//InitializeJA2Clock( );

	// INit shade tables
	BuildShadeTable( );

	// INit intensity tables
	BuildIntensityTable( );

	// Init Event Manager
	InitializeEventManager();

	// Initailize World
	InitializeWorld();

	InitTileCache( );

	InitMercPopupBox( );

#ifdef JA2BETAVERSION
	#ifdef JA2EDITOR

	//UNCOMMENT NEXT LINE TO ALLOW FORCE UPDATES...
	//LoadGlobalSummary();
	if( gfMustForceUpdateAllMaps )
	{
		ApologizeOverrideAndForceUpdateEverything();
	}
	#endif
#endif

	switch (g_game_mode)
	{
#if defined JA2BETAVERSION
		case GAME_MODE_MAP_UTILITY: return MAPUTILITY_SCREEN;

#	if defined JA2EDITOR
		case GAME_MODE_EDITOR:
			DebugMsg(TOPIC_JA2EDITOR, DBG_LEVEL_1, "Beginning JA2 using -editor commandline argument...");
			gfAutoLoadA9 = FALSE;
			goto editor;

		case GAME_MODE_EDITOR_AUTO:
			DebugMsg(TOPIC_JA2EDITOR, DBG_LEVEL_1, "Beginning JA2 using -editorauto commandline argument...");
			gfAutoLoadA9 = TRUE;
editor:
			//For editor purposes, need to know the default map file.
			strcpy(g_filename, "none");
			//also set the sector
			SetWorldSectorInvalid();
			gfIntendOnEnteringEditor = TRUE;
			gGameOptions.fGunNut     = TRUE;
			return GAME_SCREEN;
#	endif
#endif

		default: return INIT_SCREEN;
	}
}
catch (...) { return ERROR_SCREEN; }


void ShutdownJA2(void)
{
  UINT32 uiIndex;

	FRAME_BUFFER->Fill(Get16BPPColor(FROMRGB(0, 0, 0)));
	InvalidateScreen( );
	// Remove cursor....
	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );

	RefreshScreen();

	ShutdownStrategicLayer();

	// remove temp files built by laptop
	ClearOutTempLaptopFiles( );

	// Shutdown queue system
	ShutdownDialogueControl();

  // Shutdown Screens
  for (uiIndex = 0; uiIndex < MAX_SCREENS; uiIndex++)
  {
		void (*const shutdown)(void) = GameScreens[uiIndex].ShutdownScreen;
		if (shutdown != NULL) shutdown();
  }


	ShutdownLightingSystem();

	CursorDatabaseClear();

	ShutdownTacticalEngine( );

	// Shutdown Overhead
	ShutdownOverhead( );

	DeInitAnimationSystem();

	DeinitializeWorld( );

	DeleteTileCache( );

	ShutdownJA2Clock( );

	ShutdownFonts();

	ShutdownJA2Sound( );

	ShutdownEventManager( );

	ClearOutVehicleList();
}
