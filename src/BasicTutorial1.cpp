/*-------------------------------------------------------------------------
This source file is a part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/


Copyright (c) 2000-2013 Torus Knot Software Ltd
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:


The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE
-------------------------------------------------------------------------*/

//! [fullsource]

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreInput.h"
#include "OgreRTShaderSystem.h"
#include "OgreApplicationContext.h"
#include <iostream>
#include <OgreCameraMan.h>
#include <OgreMeshManager.h>
#include <OgreResourceGroupManager.h>
#include <OgreAnimation.h>
#include <OgreWindowEventUtilities.h>

using namespace Ogre;
using namespace OgreBites;

class BasicTutorial1
        : public ApplicationContext
        , public InputListener
{
private:
	SceneNode* camNode;
	SceneNode* ogreNode;
	Entity* ogreEntity;
	Root* root;
	std::deque<Vector3> mWalkList;
	Real mDistance;
	Real mWalkSpd;
	Vector3 mDirection;
	Vector3 mDestination;
	

public:
    BasicTutorial1();
    virtual ~BasicTutorial1() {}

    void setup();
	//void loop();
    bool keyPressed(const KeyboardEvent& evt);
	bool mousePressed(const MouseButtonEvent& mEvt);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);
	bool nextLocation();
};


BasicTutorial1::BasicTutorial1()
    : ApplicationContext("OgreTutorialApp")
{
}


void BasicTutorial1::setup()
{
    // do not forget to call the base first
    ApplicationContext::setup();
    addInputListener(this);
	

	mDistance = 0;
	mWalkSpd = 70.;
	mDirection = Vector3::ZERO;
	mDestination = Vector3::ZERO;


    // get a pointer to the already created root
    root = getRoot();
    SceneManager* scnMgr = root->createSceneManager();

    // register our scene with the RTSS
    RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    // -- tutorial section start --
    //! [turnlights]
    scnMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
    //! [turnlights]

    //! [newlight]
    Light* light = scnMgr->createLight("MainLight");
    SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->attachObject(light);
    //! [newlight]

    //! [lightpos]
    lightNode->setPosition(0, 100, 0);
    //Creating the plane
	Plane plane(Vector3::UNIT_Y, 0);

	MeshManager::getSingleton().createPlane("ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 1500, 1500, 20, 20, true, 1, 5, 5, Vector3::UNIT_Z);
		
	Entity* planeEntity = scnMgr->createEntity("ground");

	SceneNode* planeNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	planeEntity->setMaterialName("Plane");
	planeNode->attachObject(planeEntity);
	planeNode->setPosition(0,0,0);
    //! [camera]
    camNode = scnMgr->getRootSceneNode()->createChildSceneNode();

    // create the camera
    Camera* cam = scnMgr->createCamera("myCam");
    cam->setNearClipDistance(5); // specific to this sample
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);
    

    // and tell it to render into the main window
    getRenderWindow()->addViewport(cam);

    //! [entity1]
    ogreEntity = scnMgr->createEntity("ogrehead.mesh");

    //! [entity1node]
    ogreNode = scnMgr->getRootSceneNode()->createChildSceneNode();

    //! [entity1nodeattach]
    ogreNode->attachObject(ogreEntity);
	ogreNode->setPosition(0, 20, -300);
	ogreNode->setFixedYawAxis(true, Vector3::UNIT_Y);

    //! [cameramove]
	//camNode->setPosition(0, 20, 340);
    camNode->setPosition(0, 1000, 0);
	camNode->lookAt(Vector3(0,0,0), Node::TS_WORLD, Vector3::NEGATIVE_UNIT_Z);


	//Creating the walkingpath
	mWalkList.push_back(Ogre::Vector3(-200, 20, 50));
	mWalkList.push_back(Ogre::Vector3(0, 20, 300.0));
	mWalkList.push_back(Ogre::Vector3(200, 20, 50));

}


bool BasicTutorial1::frameRenderingQueued(const Ogre::FrameEvent& fe) 
{
	if (mDirection == Ogre::Vector3::ZERO)
	{
		if (nextLocation())
		{
			//Look at next destination
			ogreNode->lookAt(Vector3(mDestination.x, 0, mDestination.z), Node::TS_WORLD, Vector3::UNIT_Z);
		}
	}
	else
	{
		Real move = mWalkSpd * fe.timeSinceLastFrame;
		mDistance -= move;

		if (mDistance <= 0)
		{
			//When we arrive at our destination, set direction to zero so we get the next destination
			ogreNode->setPosition(mDestination);
			mDirection = Ogre::Vector3::ZERO;

			if (nextLocation())
			{
				ogreNode->lookAt(Vector3(mDestination.x, 0, mDestination.z), Node::TS_WORLD, Vector3::UNIT_Z);
			}
		}
		else
		{
			//Move
			ogreNode->translate(move * mDirection);
		}
	}

	return true;
}

bool BasicTutorial1::nextLocation()
{
	if (mWalkList.empty())
		return false;

	mDestination = mWalkList.back();
	mWalkList.pop_back();
	mDirection = mDestination - ogreNode->getPosition();
	mDistance = mDirection.normalise();

	return true;
}


bool BasicTutorial1::keyPressed(const KeyboardEvent& evt)
{
    if (evt.keysym.sym == SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
    }

	if (evt.keysym.sym == SDLK_UP)
	{
		camNode->translate(0, 0, -1);
	}

	if (evt.keysym.sym == SDLK_DOWN)
	{
		camNode->translate(0, 0, 1);
	}

	if (evt.keysym.sym == SDLK_LEFT)
	{
		camNode->translate(-1, 0, 0);
	}

	if (evt.keysym.sym == SDLK_RIGHT)
	{
		camNode->translate(1, 0, 0);
	}

	

    return true;
}

bool BasicTutorial1::mousePressed(const MouseButtonEvent& mEvt) 
{
	

	if (mEvt.type == MOUSEBUTTONDOWN && mEvt.button == BUTTON_LEFT)
	{
		camNode->rotate(Vector3(0,1,0),Radian(0.2),Node::TS_LOCAL);
	}

	if (mEvt.type == MOUSEBUTTONDOWN && mEvt.button == BUTTON_RIGHT)
	{
		camNode->rotate(Vector3(0, 1, 0), Radian(-0.2), Node::TS_LOCAL);
	}


	if (mEvt.type == MOUSEBUTTONDOWN && mEvt.button == BUTTON_MIDDLE)
	{
		camNode->rotate(Vector3(1, 0, 0), Radian(-0.2), Node::TS_LOCAL);
	}


	return true;
}



bool Root::renderOneFrame(void)
{
	if (!_fireFrameStarted())
		return false;

	if (!_updateAllRenderTargets())
		return false;

	return _fireFrameEnded();
}


int main(int argc, char **argv)
{
    try
    {
    	BasicTutorial1 app;
        app.initApp();
        app.getRoot()->startRendering();

		while (true)
		{
			Ogre::WindowEventUtilities::messagePump();

			if (!app.getRoot()->renderOneFrame()) return false;
		}
		
        app.closeApp();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error occurred during execution: " << e.what() << '\n';
        return 1;
    }

    return 0;
}

//! [fullsource]
