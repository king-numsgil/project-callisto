#include <Magnum/Platform/GlfwApplication.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/DebugOutput.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Timeline.h>

#include <Magnum/GL/TextureArray.h>
#include <Magnum/GL/ImageFormat.h>

#include "ScriptContext.hpp"
#include "ImContext.hpp"
#include "HexGrid.hpp"

using namespace Magnum;

class CallistoGame : public Platform::Application
{
public:
	explicit CallistoGame(Arguments const& arguments)
			: Platform::Application{arguments, NoCreate}
	{
		create(Configuration{}
				       .setTitle("Callisto Game Rendering Window")
				       .setSize({1280, 768}),
		       GLConfiguration{}
				       .setSrgbCapable(true)
				       .setSampleCount(2)
#ifndef NDEBUG
				       .addFlags(GLConfiguration::Flag::Debug)
#endif
		);

#ifndef NDEBUG
		GL::Renderer::enable(GL::Renderer::Feature::DebugOutput);
		GL::Renderer::enable(GL::Renderer::Feature::DebugOutputSynchronous);
		GL::DebugOutput::setDefaultCallback();
		GL::DebugOutput::setEnabled(GL::DebugOutput::Source::Api, GL::DebugOutput::Type::Other, {131185}, false);
#endif

		GL::Renderer::disable(GL::Renderer::Feature::DepthTest);
		GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
		GL::Renderer::disable(GL::Renderer::Feature::ScissorTest);
		GL::Renderer::disable(GL::Renderer::Feature::Blending);

		GL::Renderer::setClearColor(0xa5c9ea_rgbf);
		GL::Renderer::setBlendEquation(GL::Renderer::BlendEquation::Add, GL::Renderer::BlendEquation::Add);
		GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::SourceAlpha,
		                               GL::Renderer::BlendFunction::OneMinusSourceAlpha);

		_proj = f32mat3::projection(f32vec2{windowSize()});
		_view = f32mat3::translation({0.f, 0.f});

		_shader = Shaders::Flat2D{Shaders::Flat2D::Flag::Textured};
		_ctx = ImContext{windowSize()};
		_time.start();

		_scr.file("assets/script/test.lua");
		_grid.load_terrain_types();
		_grid.create_texture();

		_grid.insert({0, 1}, 1);
		_grid.insert({0, 0}, 1);
		_grid.insert({0, -1}, 2);
		_grid.insert({1, 0}, 0);
	}

	virtual ~CallistoGame() = default;

private:
	ScriptContext _scr{};
	ImContext _ctx{NoCreate};
	Timeline _time{};

	Shaders::Flat2D _shader{NoCreate};
	hex::Grid _grid{};

	f32mat3 _proj{};
	f32mat3 _view{};

	void drawEvent() override
	{
		GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);
		_ctx.newFrame();
		_ctx.updateApplicationCursor(*this);

		_grid.render(_proj * _view);

		GL::Renderer::enable(GL::Renderer::Feature::Blending);
		GL::Renderer::enable(GL::Renderer::Feature::ScissorTest);
		GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);

		_ctx.drawFrame();

		GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
		GL::Renderer::disable(GL::Renderer::Feature::ScissorTest);
		GL::Renderer::disable(GL::Renderer::Feature::Blending);

		swapBuffers();
		redraw();
		_time.nextFrame();
	}

	void keyReleaseEvent(KeyEvent& event) override
	{ _ctx.handleKeyReleaseEvent(event); }

	void keyPressEvent(KeyEvent& event) override
	{
		if (!_ctx.handleKeyPressEvent(event))
		{
			if (event.key() == KeyEvent::Key::Esc)
			{
				exit();
				return;
			}
		}
	}

	void mouseReleaseEvent(MouseEvent& event) override
	{ _ctx.handleMouseReleaseEvent(event); }

	void mousePressEvent(MouseEvent& event) override
	{ _ctx.handleMousePressEvent(event); }

	void mouseMoveEvent(MouseMoveEvent& event) override
	{ _ctx.handleMouseMoveEvent(event); }

	void mouseScrollEvent(MouseScrollEvent& event) override
	{ _ctx.handleMouseScrollEvent(event); }

	void textInputEvent(TextInputEvent& event) override
	{ _ctx.handleTextInputEvent(event); }
};

int main(int argc, char** argv)
{
	CallistoGame app{{argc, argv}};
	return app.exec();
}
