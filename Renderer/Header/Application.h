#include "GameObject.h"
#include "Scene.h"
#include "LineDrawer.h"

namespace Renderer
{
	class Application
	{
		public:
			struct DeltaTime
			{
				float mLastFrame = 0;
				float mDeltaTime = 0;
				void updateDeltaTime();
			};

			Window mWindow;
			VKRenderer mRenderer;
			ResourceManager mResources;
			Camera mCamera;
			Scene<GameObject> mScene;
			Shader** mLightShader = nullptr;
			static LineDrawer* mLineDrawer;
			static LineDrawer* mLineDrawer2;
			DeltaTime mTime;

			Model** mManequin5;

			Application(const char* pTitle, const unsigned int& pWidth, const unsigned int& pHeight);
			void keyCallback(int pKey, int pScancode, int pAction, int pMods);
			void Application::processInput(const float& pDeltaTime);
			void initScene();
			void run();

			void update();

			void draw();

			void cleanUp();

			void updateDemo(float pDeltaTime);
	};
}