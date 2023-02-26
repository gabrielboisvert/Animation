#include "Application.h"

using namespace Renderer;

GameObject* obju5 = nullptr;
LineDrawer* Application::mLineDrawer = nullptr;
LineDrawer* Application::mLineDrawer2 = nullptr;

Application::Application(const char* pTitle, const unsigned int& pWidth, const unsigned int& pHeight) : 
    mWindow(pTitle, pWidth, pHeight),
    mRenderer(mWindow),
    mCamera(pWidth, pHeight, Maths::FMatrix4::createPerspective(-45, float(pWidth) / float(pHeight), 0.01f, 500.f), Maths::FVector3(0, 2, 4)),
    mScene(mRenderer, mCamera)
{
    mWindow.setWindowUserPointer(this);
    mRenderer.init();
    mScene.init();
    mLineDrawer = new LineDrawer(mCamera, mRenderer, "Shader/debugVertex.vert.spv", "Shader/debugFrag.frag.spv");
    mLineDrawer2 = new LineDrawer(mCamera, mRenderer, "Shader/debugVertex.vert.spv", "Shader/debugFrag.frag.spv");
}

void Application::keyCallback(int pKey, int pScancode, int pAction, int pMods)
{
    if (mWindow.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
        mWindow.close();

    if (mWindow.getKey(GLFW_KEY_E) == GLFW_PRESS)
        GameObject::factor += 0.25;

    if (mWindow.getKey(GLFW_KEY_Q) == GLFW_PRESS)
        GameObject::factor -= 0.25;

    if (mWindow.getKey(GLFW_KEY_SPACE) == GLFW_PRESS)
        Bone::animationInPlace = !Bone::animationInPlace;

    if (mWindow.getKey(GLFW_KEY_X) == GLFW_PRESS)
        obju5->transition(1);
}

void Application::processInput(const float& pDeltaTime)
{
    mCamera.mIsRun = mWindow.getKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

    if (mWindow.getKey(GLFW_KEY_A) == GLFW_PRESS)
        mCamera.processKeyboard(Renderer::CameraMovement::LEFT, pDeltaTime);
    else if (mWindow.getKey(GLFW_KEY_D) == GLFW_PRESS)
        mCamera.processKeyboard(Renderer::CameraMovement::RIGHT, pDeltaTime);

    if (mWindow.getKey(GLFW_KEY_W) == GLFW_PRESS)
        mCamera.processKeyboard(Renderer::CameraMovement::FORWARD, pDeltaTime);
    else if (mWindow.getKey(GLFW_KEY_S) == GLFW_PRESS)
        mCamera.processKeyboard(Renderer::CameraMovement::BACKWARD, pDeltaTime);
}

void Application::initScene()
{
    mLightShader = mResources.create<Shader>("shader", mRenderer, "Shader/vertex.vert.spv", "Shader/frag.frag.spv");

    //Floor
    Model** model2 = mResources.create<Model>("plane", mRenderer, "Assets/quad.obj");
    GameObject* obj2 = new GameObject(mRenderer, mCamera, model2, {}, mLightShader, nullptr, Maths::FVector3(0, 0, 0), Maths::FVector3(180, 0, 0), Maths::FVector3(20, 1, 20));
    mScene.addNode(obj2);

    //Animation
    mManequin5 = mResources.create<Model>("bonhome", mRenderer, "Assets/SKM_Manny_Simple.fbx");
    Animation** animRun = mResources.create<Animation>("run", "Assets/MM_Run_Fwd.fbx", mManequin5);
    Animation** animWalk = mResources.create<Animation>("walk", "Assets/MM_Walk_Fwd.fbx", mManequin5);
    Texture** text = mResources.create<Texture>("Texture", mRenderer, "Assets/T_Manny_01_D.BMP");
    obju5 = new GameObject(mRenderer, mCamera, mManequin5, { animWalk, animRun }, mLightShader, text, Maths::FVector3(0, 0, -3), Maths::FVector3(90, 180, 0), Maths::FVector3(1, 1, 1));
    mScene.addNode(obju5);

    mLineDrawer->mUniform.mModel = Maths::FMatrix4::createTransformMatrix(Maths::FVector3(0, 0, -4), Maths::FVector3(90, 180, 0), Maths::FVector3(1, 1, 1));
    mLineDrawer2->mUniform.mModel = Maths::FMatrix4::createTransformMatrix(Maths::FVector3(0, 0, -2), Maths::FVector3(90, 180, 0), Maths::FVector3(1, 1, 1));
    mTime.updateDeltaTime();
}


void Application::run()
{
    initScene();

    while (!mWindow.shouldClose())
    {
        update();
        draw();
    }

    cleanUp();
}

void Application::update()
{
    mRenderer.finishSetup();

    mWindow.pollEvents();
    mTime.updateDeltaTime();

    processInput(mTime.mDeltaTime);
    mCamera.updatePos();
    mScene.update(mTime.mDeltaTime);

    if (!mLineDrawer->init && mManequin5 != nullptr && *mManequin5 != nullptr)
    {
        mLineDrawer->createVertexBuffer();
        mLineDrawer2->createVertexBuffer();
    }
}

void Application::cleanUp()
{
    mRenderer.waitForCleanUp();
    mWindow.shutDown();
    mResources.mPool.stop();
    mRenderer.finishSetup();
    mResources.clear();
    mScene.clear();
    delete mLineDrawer;
    delete mLineDrawer2;
}

void Application::draw()
{
    mRenderer.beginDraw();

    if (mLightShader != nullptr && *mLightShader != nullptr)
        mScene.sendLight(*(*mLightShader));

    mScene.draw();
    mLineDrawer->flushLines();
    mLineDrawer2->flushLines();
    mRenderer.endDraw();
}

void Application::DeltaTime::updateDeltaTime()
{
    float currentFrame = glfwGetTime();
    mDeltaTime = currentFrame - mLastFrame;
    mLastFrame = currentFrame;
}