#include <Magnum/Platform/GlfwApplication.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/DebugOutput.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Timeline.h>

#include <umbriel/ImContext.hpp>
#include <umbriel/GameState.hpp>
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

		_manager.push_state(new GameState(_manager.weak_ptr()));
		_time.start();
	}

private:
	StateManager _manager{};
	Timeline _time{};
	ImContext _imgui{NoCreate};

	void drawEvent() override
	{
		GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);
		_imgui.newFrame();
		if (ImGui::GetIO().WantTextInput && !isTextInputActive())
			startTextInput();
		else if (!ImGui::GetIO().WantTextInput && isTextInputActive())
			stopTextInput();

		_manager.draw_event(_time.previousFrameDuration());

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
		_time.nextFrame();
	}

	void keyReleaseEvent(KeyEvent& event) override
	{
		if (_imgui.handleKeyReleaseEvent(event)) return;
		_manager.key_release_event(event);
	}

	void keyPressEvent(KeyEvent& event) override
	{
		if (_imgui.handleKeyPressEvent(event)) return;

		if (event.key() == KeyEvent::Key::Esc)
		{
			exit();
			return;
		}

		_manager.key_press_event(event);
	}

	void mouseReleaseEvent(MouseEvent& event) override
	{
		if (_imgui.handleMouseReleaseEvent(event)) return;
		_manager.mouse_release_event(event);
	}

	void mousePressEvent(MouseEvent& event) override
	{
		if (_imgui.handleMousePressEvent(event)) return;
		_manager.mouse_press_event(event);
	}

	void mouseMoveEvent(MouseMoveEvent& event) override
	{
		if (_imgui.handleMouseMoveEvent(event)) return;
		_manager.mouse_move_event(event);
	}

	void mouseScrollEvent(MouseScrollEvent& event) override
	{
		if (_imgui.handleMouseScrollEvent(event))
		{
			event.setAccepted();
			return;
		}

		_manager.mouse_scroll_event(event);
	}

	void textInputEvent(TextInputEvent& event) override
	{
		if (_imgui.handleTextInputEvent(event)) return;
		_manager.text_input_event(event);
	}
};

int main(int argc, char** argv)
{
	UmbrielApp app{{argc, argv}};
	return app.exec();
}
