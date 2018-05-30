//
// Copyright (c) 2008-2015 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/VertexBuffer.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>

#include <iostream>
#include <fstream>

#include "StaticScene.h"

#include <Urho3D/DebugNew.h>

URHO3D_DEFINE_APPLICATION_MAIN(StaticScene)

StaticScene::StaticScene(Context* context) :
    Sample(context)
{
        const Vector<String>& arguments = GetArguments();

	if (arguments.Size()>0)
        	std::cout << arguments[0].CString () << std::endl;

	indexNode=0;
}

void StaticScene::Start()
{
    // Execute base class startup
    Sample::Start();

	SetLogoVisible(false);

    // Create the scene content
    CreateScene();

    // Create the UI content
    CreateInstructions();

    // Setup the viewport for displaying the scene
    SetupViewport();

    // Hook up to the frame update events
    SubscribeToEvents();
}

void StaticScene::CreateScene()
{
	int index=0;
    ResourceCache* cache = GetSubsystem<ResourceCache>();

    scene_ = new Scene(context_);

    // Create the Octree component to the scene. This is required before adding any drawable components, or else nothing will
    // show up. The default octree volume will be from (-1000, -1000, -1000) to (1000, 1000, 1000) in world coordinates; it
    // is also legal to place objects outside the volume but their visibility can then not be checked in a hierarchically
    // optimizing manner
    scene_->CreateComponent<Octree>();

    // Create a child scene node (at world origin) and a StaticModel component into it. Set the StaticModel to show a simple
    // plane mesh with a "stone" material. Note that naming the scene nodes is optional. Scale the scene node larger
    // (100 x 100 world units)

    Node* planeNode = scene_->CreateChild("Plane");
    planeNode->SetPosition(Vector3(0.0f, -10.0f, 0.0f));
    planeNode->SetScale(Vector3(10.0f, 1.0f, 10.0f));
    StaticModel* planeObject = planeNode->CreateComponent<StaticModel>();
    planeObject->SetModel(cache->GetResource<Model>("Models/Plane.mdl"));
    planeObject->SetMaterial(cache->GetResource<Material>("Materials/Fatalex/fatalex_Plane.xml"));
	matrixNode[index++]=planeNode;

    // Create a directional light to the world so that we can see something. The light scene node's orientation controls the
    // light direction; we will use the SetDirection() function which calculates the orientation from a forward direction vector.
    // The light will use default settings (white light, no shadows)

    Node* lightNode = scene_->CreateChild("DirectionalLight");
    //lightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f)); // The direction vector does not need to be normalized
    lightNode->SetDirection(Vector3(0.0f, -1.0f, 0.0f)); // The direction vector does not need to be normalized
    Light* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
        light->SetBrightness(1.0);

/*
    lightNode = scene_->CreateChild("DirectionalLight");
    lightNode->SetDirection(Vector3(-0.6f, -1.0f, -0.8f)); // The direction vector does not need to be normalized
    light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
        light->SetBrightness(1.0);

    lightNode = scene_->CreateChild("DirectionalLight");
    lightNode->SetDirection(Vector3(0.8f, -1.0f, -0.6f)); // The direction vector does not need to be normalized
    light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
        light->SetBrightness(0.5);

    lightNode = scene_->CreateChild("DirectionalLight");
    lightNode->SetDirection(Vector3(0.8f, -1.0f, -0.6f)); // The direction vector does not need to be normalized
    light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
        light->SetBrightness(0.5);
*/

    // Create more StaticModel objects to the scene, randomly positioned, rotated and scaled. For rotation, we construct a
    // Create more StaticModel objects to the scene, randomly positioned, rotated and scaled. For rotation, we construct a
    // Create more StaticModel objects to the scene, randomly positioned, rotated and scaled. For rotation, we construct a
    // quaternion from Euler angles where the Y angle (rotation about the Y axis) is randomized. The mushroom model contains
    // LOD levels, so the StaticModel component will automatically select the LOD level according to the view distance (you'll
    // see the model get simpler as it moves further away). Finally, rendering a large number of the same object with the
    // same material allows instancing to be used, if the GPU supports it. This reduces the amount of CPU work in rendering the
    // scene.
	int y=0;
	//for (int y=-2;y<=2;y++)
		for (int z=-2;z<=2;z++)
			for (int x=-2;x<=2;x++)
			{
				char uniqname[50];

// Création cube vert
			//	int x=0, y=0, z=0;
				sprintf(uniqname,"Wall%2.2d%2.2d",x,z);
        			Node* wallNode = scene_->CreateChild(uniqname);
				float px= x*4.0f;
				//float py= y*4.0f;
				float py= 0.0f;
				float pz= -z*4.0f;
				std::cout << "Cube at " << px << "," << py << "," << pz << std::endl;

        			wallNode->SetPosition(Vector3(px, py, pz));
        			//wallNode->SetRotation(Quaternion(0.0f, 15.0f, 0.0f));
        			wallNode->SetScale(2.0f);
        			StaticModel* wallObject = wallNode->CreateComponent<StaticModel>();
        			Model *model=cache->GetResource<Model>("Models/Plane.mdl");
        			wallObject->SetModel(model);
                    sprintf(uniqname,"Materials/Fatalex/fatalex_Wall%2.2d%2.2d.xml",x,z);
                    wallObject->SetMaterial(cache->GetResource<Material>(uniqname));

/*
    VertexBuffer* buffer = model->GetGeometry(0, 0)->GetVertexBuffer(0);
    const unsigned char* vertexData = (const unsigned char*)buffer->Lock(0, buffer->GetVertexCount());
	if (vertexData)
	{
        	unsigned numVertices = buffer->GetVertexCount();
        	unsigned vertexSize = buffer->GetVertexSize();

		std::cout << "numVertices=" << numVertices << std::endl;
		std::cout << "vertexSize=" << vertexSize << std::endl;
        
		for (unsigned i = 0; i < numVertices; ++i)
        	{
            		const Vector3& src = *reinterpret_cast<const Vector3*>(vertexData + i * vertexSize);
			std::cout << "i=" << i << " x=" << src.x_ << ", y=" << src.y_ << ", z=" << src.z_ << std::endl;
			Vector3 lsrc=wallNode->LocalToWorld(src);
			std::cout << "i=" << i << " x=" << lsrc.x_ << ", y=" << lsrc.y_ << ", z=" << lsrc.z_ << std::endl;
        	}
        	buffer->Unlock();
	}
*/
// création cube vert
        			//wallObject->SetMaterial(cache->GetResource<Material>("Materials/cube_noir.xml"));

				/*if (y==-2)
        				wallObject->SetMaterial(cache->GetResource<Material>("Materials/RedPawn.xml"));
				else if (y==-1)
        				wallObject->SetMaterial(cache->GetResource<Material>("Materials/YellowPawn.xml"));
				else if (y==0)
        				wallObject->SetMaterial(cache->GetResource<Material>("Materials/GreenPawn.xml"));
				else if (y==1)
        				wallObject->SetMaterial(cache->GetResource<Material>("Materials/BluePawn.xml"));
				else if (y==2)
        				wallObject->SetMaterial(cache->GetResource<Material>("Materials/BlackPawn.xml"));
        			wallObject->SetMaterial(cache->GetResource<Material>("Materials/AHC01_59.xml"));
        			wallObject->SetMaterial(cache->GetResource<Material>("Materials/outwarped.xml"));*/
				matrixNode[index++]=wallNode;

			}

/*
    for (unsigned i = 0; i < NUM_OBJECTS; ++i)
    {
        Node* wallNode = scene_->CreateChild("Wall");
        wallNode->SetPosition(Vector3(Random(90.0f) - 45.0f, 1.0f, Random(90.0f) - 45.0f));
        //wallNode->SetRotation(Quaternion(0.0f, Random(360.0f), 0.0f));
        //wallNode->SetScale(0.5f + Random(2.0f));
        StaticModel* wallObject = wallNode->CreateComponent<StaticModel>();
        //wallObject->SetModel(cache->GetResource<Model>("Models/Cube.mdl"));
        wallObject->SetModel(cache->GetResource<Model>("Models/cubeuv.mdl"));
        //wallObject->SetMaterial(cache->GetResource<Material>("Materials/BlackPawn.xml"));
        wallObject->SetMaterial(cache->GetResource<Material>("Materials/cube_noir.xml"));
    }
*/

    // Create a scene node for the camera, which we will move around
    // The camera will use default settings (1000 far clip distance, 45 degrees FOV, set aspect ratio automatically)
    cameraNode_ = scene_->CreateChild("Camera");
    cameraNode_->CreateComponent<Camera>();

    // Set an initial position for the camera scene node above the plane
    cameraNode_->SetPosition(Vector3(0.0f, 14.0f, 0.0f));

	Camera* camera = cameraNode_->GetComponent<Camera>();

	camera->SetFov(90.0f);
	camera->SetAspectRatio(1.0f);
}

void StaticScene::CreateInstructions()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Graphics* graphics = GetSubsystem<Graphics>();
    UI* ui = GetSubsystem<UI>();

    // Get rendering window size as floats
    float width = (float)graphics->GetWidth();
    float height = (float)graphics->GetHeight();

        uiRoot_ = ui->GetRoot();

        XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
        SharedPtr<Cursor> cursor(new Cursor(context_));
        cursor->SetStyleAuto(style);
        ui->SetCursor(cursor);

        uiRoot_->SetDefaultStyle(style);

/*
    std::cout << "width=" << width << ", height=" << height << std::endl;

    Texture2D* anamorphTex = cache->GetResource<Texture2D>("Textures/espion.png");
    //SharedPtr<Sprite> sprite(new Sprite(context_));
    sprite=new Sprite(context_);
    sprite->SetTexture(anamorphTex);

    sprite->SetPosition(Vector2(1024.0f/2.0, 768.0f/2.0f));

    // Set sprite size & hotspot in its center
    sprite->SetSize(IntVector2(1024, 768));
    sprite->SetHotSpot(IntVector2(1024/2, 768/2));

    // Set random color and additive blending mode
    //sprite->SetColor(Color(Random(0.5f) + 0.5f, Random(0.5f) + 0.5f, Random(0.5f) + 0.5f));
    sprite->SetBlendMode(BLEND_ADD);

    // Add as a child of the root UI element
    ui->GetRoot()->AddChild(sprite);
*/


/*
    // Construct new Text object, set string to display and font to use
    Text* instructionText = ui->GetRoot()->CreateChild<Text>();
    instructionText->SetText("Use WASD keys and mouse/touch to move");
    instructionText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 15);

    // Position the text relative to the screen center
    instructionText->SetHorizontalAlignment(HA_CENTER);
    instructionText->SetVerticalAlignment(VA_CENTER);
    instructionText->SetPosition(0, ui->GetRoot()->GetHeight() / 4);
*/
}

void StaticScene::SetupViewport()
{
    Renderer* renderer = GetSubsystem<Renderer>();

    // Set up a viewport to the Renderer subsystem so that the 3D scene can be seen. We need to define the scene and the camera
    // at minimum. Additionally we could configure the viewport screen size and the rendering path (eg. forward / deferred) to
    // use, but now we just use full screen and default render path configured in the engine command line options
    SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
    renderer->SetViewport(0, viewport);
}

void StaticScene::MoveCamera(float timeStep)
{
    // Do not move if the UI has a focused element (the console)
    if (GetSubsystem<UI>()->GetFocusElement())
        return;

    Input* input = GetSubsystem<Input>();

        UI* ui = GetSubsystem<UI>();
        ui->GetCursor()->SetVisible(true);

    // Movement speed as world units per second
    const float MOVE_SPEED = 20.0f;
    // Mouse sensitivity as degrees per pixel
    const float MOUSE_SENSITIVITY = 0.1f;

    // Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
    IntVector2 mouseMove = input->GetMouseMove();
    yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
    pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
    pitch_ = Clamp(pitch_, -90.0f, 90.0f);

    // Construct new orientation for the camera scene node from yaw and pitch. Roll is fixed to zero
    //cameraNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));
    //cameraNode_->SetRotation(Quaternion(0.0f, 0.0f, 0.0f));

        if (input->GetMouseButtonPress(MOUSEB_LEFT))
        {
                Vector3 hitPos;
                Vector3 newPos;
                Drawable* hitDrawable;

                if (Raycast(250.0f, hitPos, hitDrawable))
                {
                }
        }
    // Read WASD keys and move the camera scene node to the corresponding direction if they are pressed
    // Use the Translate() function (default local space) to move relative to the node's orientation.
    if (input->GetKeyDown('W'))
        cameraNode_->Translate(Vector3::FORWARD * MOVE_SPEED * timeStep);
    if (input->GetKeyDown('S'))
        cameraNode_->Translate(Vector3::BACK * MOVE_SPEED * timeStep);
    if (input->GetKeyDown('A'))
        cameraNode_->Translate(Vector3::LEFT * MOVE_SPEED * timeStep);
    if (input->GetKeyDown('D'))
        cameraNode_->Translate(Vector3::RIGHT * MOVE_SPEED * timeStep);


	// deplacement camera

    if (input->GetKeyPress('H'))
	{
 		Vector3 cpos=cameraNode_->GetPosition();
                cpos.x_=cpos.x_-2.0f;
                cameraNode_->SetPosition(cpos);
		std::cout << "Camera pos " << cpos.x_ << "," << cpos.y_ << "," << cpos.z_ << std::endl;
	}
    if (input->GetKeyPress('J'))
	{
 		Vector3 cpos=cameraNode_->GetPosition();
                cpos.x_=cpos.x_+2.0f;
                cameraNode_->SetPosition(cpos);
		std::cout << "Camera pos " << cpos.x_ << "," << cpos.y_ << "," << cpos.z_ << std::endl;
	}
    if (input->GetKeyPress('U'))
	{
 		Vector3 cpos=cameraNode_->GetPosition();
                cpos.z_=cpos.z_+2.0f;
                cameraNode_->SetPosition(cpos);
		std::cout << "Camera pos " << cpos.x_ << "," << cpos.y_ << "," << cpos.z_ << std::endl;
	}
    if (input->GetKeyPress('N'))
	{
 		Vector3 cpos=cameraNode_->GetPosition();
                cpos.z_=cpos.z_-2.0f;
                cameraNode_->SetPosition(cpos);
		std::cout << "Camera pos " << cpos.x_ << "," << cpos.y_ << "," << cpos.z_ << std::endl;
	}
    if (input->GetKeyPress('I'))
	{
 		Vector3 cpos=cameraNode_->GetPosition();
                cpos.y_=cpos.y_+2.0f;
                cameraNode_->SetPosition(cpos);
		std::cout << "Camera pos " << cpos.x_ << "," << cpos.y_ << "," << cpos.z_ << std::endl;
	}
    if (input->GetKeyPress('K'))
	{
 		Vector3 cpos=cameraNode_->GetPosition();
                cpos.y_=cpos.y_-2.0f;
                cameraNode_->SetPosition(cpos);
		std::cout << "Camera pos " << cpos.x_ << "," << cpos.y_ << "," << cpos.z_ << std::endl;
	}

    if (input->GetKeyPress('Y'))
	{
		Camera* camera = cameraNode_->GetComponent<Camera>();

		std::ofstream myfile;
		myfile.open("planes.txt");

		for (int k=0;k<26;k++)
		{
			Node *n=matrixNode[k];
        		StaticModel* wallObject = n->GetComponent<StaticModel>();
			Model *model=wallObject->GetModel();
    
			VertexBuffer* buffer = model->GetGeometry(0, 0)->GetVertexBuffer(0);
    			const unsigned char* vertexData = (const unsigned char*)
				buffer->Lock(0, buffer->GetVertexCount());
			if (vertexData)
			{
        			unsigned numVertices = buffer->GetVertexCount();
        			unsigned vertexSize = buffer->GetVertexSize();

				myfile << n->GetName().CString() << std::endl;
				//std::cout << n->GetName().CString() << std::endl;
				//std::cout << numVertices << std::endl;
				//std::cout << vertexSize << std::endl;
        
				for (unsigned i = 0; i < numVertices; ++i)
        			{
            				const Vector3& src = *reinterpret_cast<const Vector3*>(vertexData + i * vertexSize);
					//std::cout << "i=" << i << " x=" << src.x_ << ", y=" << src.y_ << ", z=" << src.z_ << std::endl;
					Vector3 wsrc=n->LocalToWorld(src);
					//std::cout << "i=" << i << " x=" << wsrc.x_ << ", y=" << wsrc.y_ << ", z=" << wsrc.z_ << std::endl;
					Vector3 pos = camera->WorldToScreenPoint(wsrc);
					//std::cout << i << " " << round(pos.x_*1024.0) << "," << round(pos.y_*768.0) << std::endl;	
					myfile << i << "," << round(pos.x_*1024.0) << "," << round(pos.y_*768.0) << std::endl;	
        			}
        			buffer->Unlock();
			}
		}

		//Vector3 pos = camera->WorldToScreenPoint(Vector3(0, 0, 0));
		//std::cout << "WorldToScreenPoint" << pos.x_ << "," << pos.y_ << std::endl;	
	}

    if (input->GetKeyPress('R'))
	{
/*
		for (int i=0;i<125;i++)
			matrixNode[i]->SetEnabled(true);
		std::cout << "Enabled true" << std::endl;
*/
		//sprite->SetEnabled(true);
    		sprite->SetPosition(Vector2(1024.0f/2.0, 768.0f/2.0f));
		std::cout << "Enabled true" << std::endl;
	}

    if (input->GetKeyPress('T'))
	{
/*
		for (int i=0;i<125;i++)
			matrixNode[i]->SetEnabled(false);
		std::cout << "Enabled false" << std::endl;
*/
    		sprite->SetPosition(Vector2(10240.0f, 7680.0f));
		std::cout << "Enabled false" << std::endl;
	}

    if (input->GetKeyPress('F'))
	{
		for (int i=0;i<125;i++)
			matrixNode[i]->SetEnabled(false);
		if (indexNode<125)
		{
			indexNode++;
			matrixNode[indexNode]->SetEnabled(true);
			std::cout << "indexNode=" << indexNode << std::endl;
		}
		
	}
    if (input->GetKeyPress('G'))
	{
		for (int i=0;i<125;i++)
			matrixNode[i]->SetEnabled(false);
		if (indexNode>0)
		{
			indexNode--;
			matrixNode[indexNode]->SetEnabled(true);
			std::cout << "indexNode=" << indexNode << std::endl;
		}
	}

    if (input->GetKeyPress('P'))
	{
		Image screenshot(context_); 
		GetSubsystem<Graphics>()->TakeScreenShot(screenshot); 
		screenshot.SavePNG("res.png");
		std::cout << "Take screenshot" << std::endl;
	}

    if (input->GetKeyPress(','))
	{
		std::cout << "," << std::endl;
		RaycastMap(250.0f);
	}

	Vector3 tpos(0.0f, 0.0f, 0.0f);
	cameraNode_->LookAt(tpos, Vector3::UP);
}

void StaticScene::SubscribeToEvents()
{
    // Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(StaticScene, HandleUpdate));
}

void StaticScene::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    // Move the camera, scale movement with time step
    MoveCamera(timeStep);
}

bool StaticScene::Raycast(float maxDistance, Vector3& hitPos, Drawable*& hitDrawable)
{
        hitDrawable = 0;

        UI* ui = GetSubsystem<UI>();
        IntVector2 pos = ui->GetCursorPosition();

        // Check the cursor is visible and there is no UI element in front of the cursor

        if (!ui->GetCursor()->IsVisible() || ui->GetElementAt(pos, true))
                return false;

        Graphics* graphics = GetSubsystem<Graphics>();
        Camera* camera = cameraNode_->GetComponent<Camera>();

	std::cout << "pos.x=" << pos.x_ << " , pos.y=" << pos.y_ << std::endl;

        Ray cameraRay = camera->GetScreenRay((float)pos.x_ / graphics->GetWidth(),
                (float)pos.y_ / graphics->GetHeight());

        // Pick only geometry objects, not eg. zones or lights, only get the first (closest) hit

        PODVector<RayQueryResult> results;
        RayOctreeQuery query(results, cameraRay, RAY_TRIANGLE, maxDistance, DRAWABLE_GEOMETRY);

        // This is much better to get all the objects at the cursor location

        scene_->GetComponent<Octree>()->Raycast(query);

        //scene_->GetComponent<Octree>()->RaycastSingle(query);

        if (results.Size())
        {
        	std::cout << "-- result.size()=" << results.Size() << std::endl;
                for (int i=0;i<results.Size();i++)
                {
                        RayQueryResult& result = results[i];
                        Node *n=result.drawable_->GetNode();

                        std::cout << n->GetName().CString() << std::endl;
                        if ((strncmp(n->GetName().CString(),"Wall",4)==0))
                        {
				std::cout << "mur" << std::endl;
                        }
                }
                RayQueryResult& result = results[0];
                hitPos = result.position_;
		std::cout << hitPos.x_ << "," << hitPos.y_ << "," << hitPos.z_ << " distance=" << result.distance_ << std::endl;
                hitDrawable = result.drawable_;
                return true;
        }

        return false;
}

void StaticScene::RaycastMap(float maxDistance)
{
        Graphics* graphics = GetSubsystem<Graphics>();
        Camera* camera = cameraNode_->GetComponent<Camera>();

	int gx,px;
	int gy,py;

	gx=graphics->GetWidth();
	gy=graphics->GetHeight();

	std::cout << "gx=" << gx << " , gy=" << gy << std::endl;

	std::ofstream myfile;
	myfile.open("map.txt");

	for (px=0;px<gx;px++)
		for (py=0;py<gy;py++)
		{
        		Ray cameraRay = camera->GetScreenRay((float)px / graphics->GetWidth(),
                		(float)py / graphics->GetHeight());
        		PODVector<RayQueryResult> results;
        		RayOctreeQuery query(results, cameraRay, RAY_TRIANGLE, maxDistance, DRAWABLE_GEOMETRY);
        		scene_->GetComponent<Octree>()->Raycast(query);
        		if (results.Size())
        		{
                		RayQueryResult& result = results[0];
                        	Node *n=result.drawable_->GetNode();
				myfile << px << "," << py << "," << n->GetName().CString() << "," << result.distance_ << "\n";
				//std::cout << px << "," << px << " " << n->GetName().CString() << " " << result.distance_ << std::endl;
			}
		}
	myfile.close();

}
