#include <Magnum/Platform/GlfwApplication.h>
#include <Magnum/GL/DefaultFramebuffer.h>

using namespace Magnum;

class UmbrielApp : public Platform::Application
{
public:
	explicit UmbrielApp(const Arguments& arguments)
			: Platform::Application{arguments, Configuration{}
			.setTitle("Umbriel Rendering Window")
			.setSize({1280, 768})}
	{
	}

private:
	void drawEvent() override
	{
		GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

		swapBuffers();
	}
};

int main(int argc, char** argv)
{
	UmbrielApp app{{argc, argv}};
	return app.exec();
}
