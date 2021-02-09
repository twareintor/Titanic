
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define VER_NUM		"000.A00"
#define VER_AUT		"Claudiu \"Twareintor\" Ciutacu <mailto:ciutacu@hotmail.com>"
#define VER_DES		"Emergency case behaviour simulation: Titanic demo project in Irrlicht"


/*		


*/		

#include <irrlicht.h>
#include <windows.h>	// for screen size
#include <ctime>
#include "driverChoice.h"
#include "ScreenShotFactory.h"
#include "GetThisPath.h"
#include "EventReceiver.h"

// for realistic water:
#include "RealisticWater/RealisticWater.h"

// for clouds:
#include "CloudSceneNode/CloudSceneNode.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

int main()
{
	// ask user for driver
	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
	if (driverType==video::EDT_COUNT)
		return 1;

	// create device with full flexibility over creation parameters
	// you can add more parameters if desired, check irr::SIrrlichtCreationParameters
	irr::SIrrlichtCreationParameters params;
	params.DriverType=driverType;

	const u16 nWidth = ::GetSystemMetrics(0);
	const u16 nHeight = ::GetSystemMetrics(1);
	params.WindowSize=core::dimension2d<u32>(nWidth*2/3, nHeight*2/3);
	// params.Fullscreen = false;
	params.Fullscreen = false;															/** Params full screen */
	params.Bits = 128;
	IrrlichtDevice* device = createDeviceEx(params);

	if (device == 0)
		return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* env = device->getGUIEnvironment();

	smgr->setAmbientLight(video::SColorf(.1f, 0.1f, 0.1f, 1.f));

	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
	SetWorkingDirectory2AppPath();			// needed for the start from "DevEnv"
	
	// fog density variable: 
	// f32 fFogDensity = 0.000003f; // starting fog density

	//set other font
	gui::IGUIFont* fntMain = 0;
	fntMain =  env->getFont("../../../../media/fontlucida.png");
	if(fntMain)
		env->getSkin()->setFont(fntMain);
	else
		std::cout<<"font not loaded\n"<<std::endl;

	// create skybox and skydome
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false); // remember to disable mipmaps for skybox and skydome
	scene::ISceneNode* skydome=smgr->addSkyDomeSceneNode(
		driver->getTexture("../../../../media/skydome_starlight.jpg"),16,8,0.95f,2.0f);
	skydome->setMaterialFlag(video::EMF_FOG_ENABLE, false);
	skydome->setMaterialFlag(video::EMF_LIGHTING, false);

	// create event receiver
	MyEventReceiver receiver;
	device->setEventReceiver(&receiver);

	// Add a light, so that the unselected nodes aren't completely dark: light from 'up'
	scene::ILightSceneNode* lgtAmb1 = 0;
	lgtAmb1 = smgr->addLightSceneNode(0, core::vector3df(1000.f, 10000.f, 1000.f), video::SColorf(0.4f, 0.4f, 0.4f, 1.0f), 480000.0f);
	lgtAmb1->setID(-1); // Make it an invalid target for selection.
	lgtAmb1->enableCastShadow(false);
	// lgtAmb1->setVisible(false);

	// another light: light from 'down'
	scene::ILightSceneNode* lgtAmb2 = 0;
	lgtAmb2 = smgr->addLightSceneNode(0, core::vector3df(-1000.f, -10000.f, -1000.f), video::SColorf(0.2f, 0.1f, 0.0f, 1.0f), 480000.0f);
	lgtAmb2 ->setID(-1); // Make it an invalid target for selection.
	lgtAmb1->enableCastShadow(false);
	// lgtAmb2->setVisible(false);

	/*********** triangle selector, meta triangle selector// collision manager system etc. ******/
	scene::IMetaTriangleSelector* met = 0;
	met = smgr->createMetaTriangleSelector();
	scene::ITriangleSelector* tri = 0;

	/***************************** REALISTIC WATER SCENE NODE  *************************************
	// add realistic water scene node:
	RealisticWaterSceneNode* rwsWater = new RealisticWaterSceneNode(smgr, 2048.f, 2048.f, 
		core::stringc("../../RealisticWater"), core::dimension2du(2048, 2048), 0, -1);
	
	// then set its parameters:
	// then make a node of it:
	smgr->getRootSceneNode()->addChild(rwsWater);
	rwsWater->setScale(core::vector3df(100.f, 100.f, 100.f));
	// rwsWater->setPosition(posCenter+core::vector3df(2000.f, 150.f, 2000.f)*0.f); // raw positioning
	rwsWater->setPosition(core::vector3df(0.f, 0.f, 0.f));
	rwsWater->setRotation(core::vector3df(0.f, 0.f, 0.f));
	rwsWater->setWaterColor(video::SColorf(0.01f, 0.01f, 0.01f, 0.1f));
	rwsWater->setColorBlendFactor(0.5f);
	bool bUnderWater[2] = {false, false};  // checks if the camera is under water (true, if under)
	*********************************************************************/
	gui::IGUIStaticText* txtDeb = env->addStaticText(TEXT("Emercency Simulation: "), core::rect<s32>(10, 10, 1200, 64), false, true, 0, -1, true);
	txtDeb->setOverrideColor(video::SColor(255, 20, 120, 0));
	txtDeb->setText(TEXT(""));
	int lastFPS = -1;
	// txtDeb->setVisible(false); // for debug...

	/*************************************************************************
	float fth = 0.0f;	// wind direction
	float fwf = 6.0f;	// wind force
	float fwh = .025f;	// wave height 

	rwsWater->setWindForce(fwf);
	rwsWater->setWaveHeight(fwh);
	rwsWater->setWindDirection(core::vector2df(cosf(fth), sinf(fth)));
	**************************************************************************/

	/******************************************** CLOUDS *************************************************/
	scene::CCloudSceneNode* cloudLayers[3] ;
	for(u32 i=0; i<3; i++)
	{
		cloudLayers[i] = new scene::CCloudSceneNode(smgr->getRootSceneNode(), smgr, -1);
		cloudLayers[i]->setTranslation(core::vector2d<f32>(0.006f+0.002f*(0==i), 0.f+0.003f*(0!=i)));
		core::stringc strCloudPath("../../CloudSceneNode/clouds/cloud0");
		strCloudPath+=(i+1);
		cloudLayers[i]->getMaterial(0).setTexture(0, driver->getTexture((strCloudPath+".png").c_str()));
		cloudLayers[i]->setCloudHeight(0.5f-0.1*i, 0.1f-0.05f*i, -0.05f+0.05f*i);
		cloudLayers[i]->setTextureScale(1-0.5/i);
		// cloudLayers[i]->setCloudColor(video::SColor(255, 128, 128, 32+30*i), video::SColor(128, 204, 204, 204+10*i), video::SColor(104, 224, 224, 224));
		cloudLayers[i]->setMaterialFlag(video::EMF_FOG_ENABLE, false);
		smgr->getRootSceneNode()->addChild(cloudLayers[i]);
		if(i) cloudLayers[i]->setVisible(false); /** we are still developing */
	}

	/*************** add the ship: *********************/
	// Ship: 
	scene::IAnimatedMesh* mshDeck = smgr->getMesh("../../../../media/Titanic1/DeckByDeck4Games/DeckB/Part03/model.obj");
	scene::IMeshBuffer* mb = mshDeck->getMeshBuffer(0);
	scene::CDynamicMeshBuffer* dynMeshBuf = new scene::CDynamicMeshBuffer(video::EVT_STANDARD, video::EIT_32BIT);
	dynMeshBuf->append(mb); // why is this???
	scene::ISceneNode* nodDeck = 0;
	if (mshDeck)
	{
		nodDeck = smgr->addOctreeSceneNode(mshDeck, 0, -1, 256);
		nodDeck->setScale(core::vector3df(100.f, 100.f, 100.f));
		nodDeck->setPosition(core::vector3df(0.f, 0.f, 0.f));
		nodDeck->setRotation(core::vector3df(2.f, 0.f, 0.f));
		nodDeck->setMaterialFlag(video::EMF_LIGHTING, true);
		nodDeck->setMaterialFlag(video::EMF_FOG_ENABLE, false);
		nodDeck->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
		nodDeck->setDebugDataVisible(true);
		tri = smgr->createTriangleSelector(mshDeck, nodDeck);
		if(!nodDeck->getTriangleSelector()) nodDeck->setTriangleSelector(tri);
		met->addTriangleSelector(tri);			// don't drop it, we are not ready wit it
		nodDeck->setDebugDataVisible(true);		// DEBUG ONLY - DON'T FORGET TO DELETE IT BEFORE SHIPPING!!!!!!!!!!!!!!!
	}

	// must appear illuminated from inside...
	scene::ILightSceneNode* lgtShip = 0;
	lgtShip = smgr->addLightSceneNode(nodDeck, core::vector3df(0.f, 0.f, 0.f), video::SColorf(0.7f, 0.6f, 0.4f, 1.0f), 15200.f);
	lgtShip->setID(-1); // Make it an invalid target for selection.
	lgtShip->enableCastShadow(false);
	lgtShip->setVisible(false);

	// add camera			// first keymap:
	SKeyMap keyMap[] = 
	{
		{EKA_MOVE_FORWARD,		KEY_UP		}, 		{EKA_MOVE_FORWARD,		KEY_KEY_W	}, 
		{EKA_MOVE_BACKWARD,		KEY_DOWN	}, 		{EKA_MOVE_BACKWARD,		KEY_KEY_S	}, 
		{EKA_STRAFE_LEFT,		KEY_LEFT	}, 		{EKA_STRAFE_LEFT,		KEY_KEY_A	}, 
		{EKA_STRAFE_RIGHT,		KEY_RIGHT	}, 		{EKA_STRAFE_RIGHT,		KEY_KEY_D	},
		{EKA_JUMP_UP,			KEY_SPACE	}, 		{EKA_JUMP_UP,			KEY_INSERT	},
	};
	scene::ICameraSceneNode* camMain =smgr->addCameraSceneNodeFPS(0, 50.0f, .2f, -1, keyMap, sizeof(keyMap), false, 3, false, true);
	camMain->setPosition(core::vector3df(-4633.f, 703.f, 1175.f));
	// camMain->setPosition(nodDeck->getPosition()+core::vector3df(0.f, 80.f, 0.f));
	camMain->setTarget(nodDeck->getPosition());
	camMain->setFarValue(90000.0f);
	// create collision response animator and attach it to the camera
	scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
		met, camMain, core::vector3df(10.f, 55.5f, 10.f),
		core::vector3df(0.f, -0.f, 0.f),  // Gravity per second...
		core::vector3df(0.f, 45.f, 0.f));
	
	// we don't need the triangle selector anymore...
	if(tri) tri->drop();		
	tri = 0;
	// camMain->addAnimator(anim); 
	anim->drop();			
	anim = 0 ;
	// add this for the lake scene node for the simulation when we are under the water level:B
	/**********************************************************************
	bUnderWater[0] = camMain->getPosition().Y<rwsWater->getPosition().Y;
	***********************************************************************/
	// create an event receiver for making screenshots
	char* szPre = new char[0xff];
	time_t t = time(0);
	struct tm* now = std::localtime(&t);
	sprintf(szPre, "%02d%02d%02d%02d%02d%02d", now->tm_year+1900, now->tm_mon, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
	char* szPrf = new char[0xff]; strcat(szPrf, "Lake_"); strcat(szPrf, szPre);
	CScreenShotFactory* scrFactory = new CScreenShotFactory(device, (const c8*)szPrf, (scene::ISceneNode*)0);
	delete[] szPrf;
	delete[] szPre;

	// disable mouse cursor
	device->getCursorControl()->setVisible(false);
	core::stringw str = L"";		// this caused the crash at the end of the program!!!!

	int nMaterial = 0;

	while(device->run())
		if (device->isWindowActive())
		{
			driver->beginScene(true, true, video::SColor(255, 128, 128, 255));

			// display frames per second in window title
			// water, wind: to set:
			/*********************************************************************************************
			if(receiver.IsKeyDown(KEY_NUMPAD6))	// remember that the priority is FPS instead of less code!
			{
				rwsWater->setWindForce(fwf*=1.001f);
			}
			if(receiver.IsKeyDown(KEY_NUMPAD4))
			{
				rwsWater->setWindForce(fwf*=0.999f);
			}
			if(receiver.IsKeyDown(KEY_NUMPAD9))
			{
				rwsWater->setWaveHeight(fwh*1.1f);
			}
			if(receiver.IsKeyDown(KEY_NUMPAD3))
			{
				rwsWater->setWaveHeight(fwh*0.9f);
			}
			if(receiver.IsKeyDown(KEY_NUMPAD8))
			{
				rwsWater->setWindDirection(core::vector2df(cosf(fth+=0.0001f), sinf(fth)));
			}
			if(receiver.IsKeyDown(KEY_NUMPAD2))
			{
				rwsWater->setWindDirection(core::vector2df(cosf(fth-=0.0001f), sinf(fth)));
			}
			*************************************************************************************/
			if(receiver.IsKeyDown(KEY_F6))
			{
				scrFactory->Shoot();
			}

			if(receiver.IsKeyDown(KEY_KEY_C))
			{
				skydome->setRotation(skydome->getRotation()+core::vector3df(1.f, 0.f, 0.f));
			}
			/***** DEBUG REALISTIC WATER NODE 
			if(receiver.IsKeyDown(KEY_KEY_I))	rwsWater->setPosition(rwsWater->getPosition()+core::vector3df(1.f, 0.f, 0.f));
			if(receiver.IsKeyDown(KEY_KEY_K))	rwsWater->setPosition(rwsWater->getPosition()+core::vector3df(-1.f, 0.f, 0.f));
			if(receiver.IsKeyDown(KEY_KEY_J))	rwsWater->setPosition(rwsWater->getPosition()+core::vector3df(0.f, 0.f, 1.f));
			if(receiver.IsKeyDown(KEY_KEY_L))	rwsWater->setPosition(rwsWater->getPosition()+core::vector3df(0.f, 0.f, -1.f));
			if(receiver.IsKeyDown(KEY_KEY_Y))	rwsWater->setPosition(rwsWater->getPosition()+core::vector3df(0.f, 1.f, 0.f));
			if(receiver.IsKeyDown(KEY_KEY_H))	rwsWater->setPosition(rwsWater->getPosition()+core::vector3df(0.f, -1.f, 0.f));
			*****/
			if(receiver.IsKeyDown(KEY_KEY_Y))	lgtShip->setPosition(lgtShip->getPosition()+core::vector3df(0.f, 200.f, 0.f));
			if(receiver.IsKeyDown(KEY_KEY_H))	lgtShip->setPosition(lgtShip->getPosition()+core::vector3df(0.f, -200.f, 0.f));
			if(receiver.IsKeyDown(KEY_KEY_P))
			{
				nodDeck->getMaterial(nMaterial++).Lighting = false;
				std::cout<<nMaterial<<std::endl;
				Sleep(400);
			}
			// rwsWater->OnAnimate(lastFPS*2);	// update
			// ~~ eo water, wind, //	//	//	//	//	//
			/**************************
			in order to simulate the view from under water, we need just to rotate the whole water surface with 180 degrees
			around X or Z axis, when the camera position is under the water (the y of cam is < as the y of water surface)
			This can be done if we check the position of the camera two frames one after another. It must be rotated again
			when we go up higher than the water level. it must be, otherwise, we will have a permanent set of the rotation
			of a complex node every frame and that's bad for FPS!
			*********************/
			/***************************************************************************************
			bUnderWater[1] = camMain->getPosition().Y<rwsWater->getPosition().Y; // this frame
			if((bUnderWater[1] && bUnderWater[0]))
			{
				// under water: more fog to simulate the water density...
				rwsWater->setRotation(core::vector3df(180.f, 0.f, 0.f));
				driver->setFog(video::SColor(0, 128, 128, 140), video::EFT_FOG_EXP, 250.f, 2000.f, fFogDensity, false, false);  
			}
			if((!bUnderWater[1] && !bUnderWater[0]))
			{
				rwsWater->setRotation(core::vector3df(0.f, 0.f, 0.f));
				driver->setFog(video::SColor(0, 128, 128, 140), video::EFT_FOG_LINEAR, 80000.f, 180000.f, 0.0003f, false, false);  
			}
			bUnderWater[0] = camMain->getPosition().Y<rwsWater->getPosition().Y; // this (becomes) last frame
			****************************************************************************************/
			int fps = driver->getFPS();
			L"Terrain Renderer - Irrlicht Engine ["; str+=driver->getDriverType(); str += "] FPS: "; str += fps;
			// Also print terrain height of current camera position
			// We can use camera position because terrain is located at coordinate origin
			str += "\tHeight: ";
			/***************************************************************************************************
			str+="\nWind force: "; str+=fwf; str+="\tWind direction: "; str+=fth; str+="\tWave height: "; str+=fwh;
			****************************/
			str+="\nCamera Position: ("; str+=camMain->getPosition().X; str+=", "; str+=camMain->getPosition().Y; 
			str+=", "; str+=camMain->getPosition().Z; str+=")";
			/*********************************************************
			str+="\nWater position: ("; str+=rwsWater->getPosition().X; str+=", "; str+=rwsWater->getPosition().Y; 
			str+=", "; str+=rwsWater->getPosition().Z; str+=")";
			// device->setWindowCaption(str.c_str());
			*************************************************************************************************/
			txtDeb->setText(str.c_str());
			txtDeb->draw();
			lastFPS = fps;
			str = core::stringw(L"");
			str.empty();

			// draw all then end scene:
			smgr->drawAll();
			env->drawAll();
			driver->endScene();
		}
	// free resources:
	
	for(u32 i=0; i<3; i++)
	{
		smgr->getRootSceneNode()->removeChild(cloudLayers[i]);
		cloudLayers[i]->drop();
	}
	/**************************************************************
	smgr->getRootSceneNode()->removeChild(rwsWater);
	delete rwsWater;
	**************************************************************/
	device->drop();
	std::wcout<<L"Program terminated\n"<<std::endl;
	return 0;
}

