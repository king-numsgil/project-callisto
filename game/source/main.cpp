#include <Magnum/Platform/GlfwApplication.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/DebugOutput.h>
#include <Magnum/GL/Renderer.h>

#include <umbriel/ImContext.hpp>
#include <umbriel/Types.hpp>

using namespace umbriel;
using namespace Magnum;

class UmbrielApp : public Platform::Application
{
public:
	explicit UmbrielApp(const Arguments& arguments)
			: Platform::Application{arguments, NoCreate}
	{
		create(Configuration{}
				       .setTitle("Umbriel Rendering Window")
				       .setSize({1280, 768}),
		       GLConfiguration{}
				       .setSrgbCapable(true)
				       .setSampleCount(2)
		#ifndef NDEBUG
				       .addFlags(GLConfiguration::Flag::Debug)
		#endif
		);
		_imgui = ImContext{windowSize()};

		#ifndef NDEBUG
		GL::Renderer::enable(GL::Renderer::Feature::DebugOutput);
		GL::Renderer::enable(GL::Renderer::Feature::DebugOutputSynchronous);
		GL::DebugOutput::setDefaultCallback();
		GL::DebugOutput::setEnabled(GL::DebugOutput::Source::Api, GL::DebugOutput::Type::Other, {131185}, false);
		#endif

		GL::Renderer::setClearColor(0xa5c9ea_rgbf);
		GL::Renderer::setBlendEquation(GL::Renderer::BlendEquation::Add, GL::Renderer::BlendEquation::Add);
		GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::SourceAlpha,
		                               GL::Renderer::BlendFunction::OneMinusSourceAlpha);
	}

private:
	ImContext _imgui{NoCreate};

	void drawEvent() override
	{
		GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);
		_imgui.newFrame();
		if (ImGui::GetIO().WantTextInput && !isTextInputActive())
			startTextInput();
		else if (!ImGui::GetIO().WantTextInput && isTextInputActive())
			stopTextInput();

		ImGui::ShowDemoWindow(nullptr);

		GL::Renderer::enable(GL::Renderer::Feature::Blending);
		GL::Renderer::disable(GL::Renderer::Feature::DepthTest);
		GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);
		GL::Renderer::enable(GL::Renderer::Feature::ScissorTest);

		_imgui.drawFrame();

		GL::Renderer::disable(GL::Renderer::Feature::ScissorTest);
		GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
		GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
		GL::Renderer::disable(GL::Renderer::Feature::Blending);

		swapBuffers();
		redraw();
	}

	void keyReleaseEvent(KeyEvent& event) override
	{
		if (_imgui.handleKeyReleaseEvent(event)) return;
	}

	void keyPressEvent(KeyEvent& event) override
	{
		if (_imgui.handleKeyPressEvent(event)) return;

		if (event.key() == KeyEvent::Key::Esc)
			exit();
	}

	void mouseReleaseEvent(MouseEvent& event) override
	{
		if (_imgui.handleMouseReleaseEvent(event)) return;
	}

	void mousePressEvent(MouseEvent& event) override
	{
		if (_imgui.handleMousePressEvent(event)) return;
	}

	void mouseMoveEvent(MouseMoveEvent& event) override
	{
		if (_imgui.handleMouseMoveEvent(event)) return;
	}

	void mouseScrollEvent(MouseScrollEvent& event) override
	{
		if (_imgui.handleMouseScrollEvent(event))
		{
			event.setAccepted();
			return;
		}
	}

	void textInputEvent(TextInputEvent& event) override
	{
		if (_imgui.handleTextInputEvent(event)) return;
	}
};

int main(int argc, char** argv)
{
	UmbrielApp app{{argc, argv}};
	return app.exec();
}
