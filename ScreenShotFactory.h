#pragma once

#include <irrlicht.h>

using namespace irr;

/*
A class to produce a series of screenshots // modified
*/

class CScreenShotFactory
{
public:
	CScreenShotFactory( IrrlichtDevice *device, const c8 * templateName, scene::ISceneNode* node = 0);
	void Shoot();

private:
	IrrlichtDevice* m_device;
	u32 m_number;
	core::stringc m_strFilenameTemplate;
	scene::ISceneNode* m_node;
};

