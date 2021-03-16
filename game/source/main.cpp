#include <Magnum/Platform/GlfwApplication.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/DebugOutput.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Timeline.h>
#include <random>

#include "ScriptContext.hpp"
#include "ImContext.hpp"
#include "hex/all.hpp"

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

		_shader = Shaders::Flat2D{Shaders::Flat2D::Flag::Textured};
		_ctx = ImContext{windowSize()};
		_time.start();

		_scr.file("assets/script/test.lua");
		_grid.load_terrain_types();
		_grid.create_texture();

		std::random_device rd;
		std::mt19937 gen{rd()};
		std::uniform_int_distribution<u64> distrib{0, 3};

		const i32 map_radius = 400;
		for (int q = -map_radius; q <= map_radius; q++)
		{
			int r1 = std::max(-map_radius, -q - map_radius);
			int r2 = std::min(map_radius, -q + map_radius);
			for (int r = r1; r <= r2; r++)
				_grid.insert({q, r}, distrib(gen));
		}

		Debug{} << "Generated" << _grid.size() << "tiles";
	}

	virtual ~CallistoGame() = default;

private:
	ScriptContext _scr{};
	ImContext _ctx{NoCreate};
	Timeline _time{};

	Shaders::Flat2D _shader{NoCreate};
	hex::Grid _grid{};

	f32mat3 _proj{};

	std::unordered_map<KeyEvent::Key, bool> _keyTracker{};
	f32vec3 _viewPos{0.f, 0.f, 1.f};
	f32vec2 _mousePos{}, _mousePick{};
	hex::Axial _mouseHex{0, 0};

	void drawEvent() override
	{
		GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);
		_ctx.newFrame();
		_ctx.updateApplicationCursor(*this);

		const f32 speed = 1000.f / _viewPos.z();
		if (_keyTracker.contains(KeyEvent::Key::W) && _keyTracker[KeyEvent::Key::W])
			_viewPos.y() += -speed * _time.previousFrameDuration();
		if (_keyTracker.contains(KeyEvent::Key::S) && _keyTracker[KeyEvent::Key::S])
			_viewPos.y() += speed * _time.previousFrameDuration();

		if (_keyTracker.contains(KeyEvent::Key::A) && _keyTracker[KeyEvent::Key::A])
			_viewPos.x() += speed * _time.previousFrameDuration();
		if (_keyTracker.contains(KeyEvent::Key::D) && _keyTracker[KeyEvent::Key::D])
			_viewPos.x() += -speed * _time.previousFrameDuration();

		_mousePick = unproject(_mousePos);
		_mouseHex = hex::Axial::from_position(_mousePick);

		_grid.set_pick_coord(_mouseHex);
		_grid.render(_proj * f32mat3::scaling({_viewPos.z(), _viewPos.z()}) * f32mat3::translation(_viewPos.xy()));
		ImGui::ShowMetricsWindow();

		ImGui::InputFloat2("Raw Mouse Position", _mousePos.data());
		ImGui::InputFloat2("Pick Mouse Position", _mousePick.data());
		ImGui::InputInt2("Hex Mouse Position", _mouseHex.data.data());

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
	{
		_ctx.handleKeyReleaseEvent(event);
		_keyTracker[event.key()] = false;
	}

	void keyPressEvent(KeyEvent& event) override
	{
		if (!_ctx.handleKeyPressEvent(event))
		{
			_keyTracker[event.key()] = true;
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
	{
		if (!_ctx.handleMouseMoveEvent(event))
		{
			_mousePos = f32vec2{event.position()};
		}
	}

	void mouseScrollEvent(MouseScrollEvent& event) override
	{
		if (!_ctx.handleMouseScrollEvent(event))
		{
			f32 amount = event.offset().y() < 0.f ? 0.9f : 1.1f;
			_viewPos.z() *= amount;

			if (_viewPos.z() > 1.f) _viewPos.z() = 1.f;
			if (_viewPos.z() < 0.1f) _viewPos.z() = 0.1f;
		}
	}

	void textInputEvent(TextInputEvent& event) override
	{ _ctx.handleTextInputEvent(event); }

	f32vec2 unproject(f32vec2 const& position)
	{
		f32mat4 inverse{(_proj
		                 * f32mat3::scaling({_viewPos.z(), _viewPos.z()})
		                 * f32mat3::translation(-_viewPos.xy())
		                ).inverted()};
		f32vec4 viewport{0.f, 0.f, static_cast<f32>(framebufferSize().x()), static_cast<f32>(framebufferSize().y())};

		f32vec3 tmp{position, 0.f};
		tmp.x() = (tmp.x() - viewport[0]) / viewport[2];
		tmp.y() = (tmp.y() - viewport[1]) / viewport[3];
		tmp = tmp * 2.f - f32vec3{1.f};
		tmp.y() = -tmp.y();
		return inverse.transformPoint(tmp).xy();
	}
};

int main(int argc, char** argv)
{
	CallistoGame app{{argc, argv}};
	return app.exec();
}
