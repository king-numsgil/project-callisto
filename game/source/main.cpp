#include <Magnum/Platform/GlfwApplication.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <umbriel/Types.hpp>

using namespace Magnum;

class UmbrielApp : public Platform::Application
{
public:
	explicit UmbrielApp(const Arguments& arguments)
			: Platform::Application{arguments, Configuration{}
			.setTitle("Umbriel Rendering Window")
			.setSize({1280, 768})}
	{
		GL::Renderer::setClearColor(0xa5c9ea_rgbf);
	}

private:
	void keyPressEvent(KeyEvent& event) override
	{
		if (event.key() == KeyEvent::Key::Esc)
			exit();
	}

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
