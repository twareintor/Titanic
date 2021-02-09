////////////////////////////////////////////////////////////////////////////////////////

#include "ScreenShotFactory.h"


CScreenShotFactory::CScreenShotFactory(IrrlichtDevice *device, const c8 * templateName, scene::ISceneNode* node )
	: m_device(device), m_number(0), m_strFilenameTemplate(templateName), m_node(node)
{
	m_strFilenameTemplate.replace ( '/', '_' );
	m_strFilenameTemplate.replace ( '\\', '_' );
}

void CScreenShotFactory::Shoot()
{
	video::IImage* image = m_device->getVideoDriver()->createScreenShot();
	if (image)
	{
		
		c8 buf[256];
		snprintf(buf, 256, "%s_shot%04d.jpg", m_strFilenameTemplate.c_str(), ++m_number);
		m_device->getVideoDriver()->writeImageToFile(image, buf, 85);
		image->drop();
	}
}


