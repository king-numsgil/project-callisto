#pragma once

#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/Timeline.h>
#include <Magnum/GL/Mesh.h>
#include <imgui.h>

#include "Types.hpp"

namespace Magnum::Math::Implementation
{
	template<>
	struct VectorConverter<2, f32, ImVec2>
	{
		static f32vec2 from(ImVec2 const& other)
		{
			return {other.x, other.y};
		}

		static ImVec2 to(f32vec2 const& other)
		{
			return {other[0], other[1]};
		}
	};

	template<>
	struct VectorConverter<4, f32, ImVec4>
	{
		static f32vec4 from(ImVec4 const& other)
		{
			return {other.x, other.y, other.z, other.w};
		}

		static ImVec4 to(f32vec4 const& other)
		{
			return {other[0], other[1], other[2], other[3]};
		}
	};

	template<>
	struct VectorConverter<4, f32, ImColor>
	{
		static f32vec4 from(ImColor const& other)
		{
			return f32vec4{other.Value};
		}

		static ImColor to(f32vec4 const& other)
		{
			return ImVec4{other};
		}
	};

	template<>
	struct VectorConverter<3, f32, ImColor>
	{
		static f32vec3 from(ImColor const& other)
		{
			return f32vec3{other.Value.x, other.Value.y, other.Value.z};
		}

		static ImColor to(f32vec3 const& other)
		{
			return ImVec4{f32vec4{other[0], other[1], other[2], 1.0f}};
		}
	};
}

class ImContext
{
public:
	explicit ImContext(f32vec2 const& size, i32vec2 const& windowSize, i32vec2 const& framebufferSize);

	explicit ImContext(i32vec2 const& size);

	explicit ImContext(ImGuiContext& context, f32vec2 const& size, i32vec2 const& windowSize,
	                   i32vec2 const& framebufferSize);

	explicit ImContext(ImGuiContext& context, i32vec2 const& size);

	explicit ImContext(Magnum::NoCreateT) noexcept;

	ImContext(ImContext const&) = delete;

	ImContext(ImContext&& other) noexcept;

	~ImContext();

	ImContext& operator=(ImContext const&) = delete;

	ImContext& operator=(ImContext&& other) noexcept;

	ImGuiContext* context()
	{ return _context; }

	ImGuiContext* release();

	Magnum::GL::Texture2D& atlasTexture()
	{ return _texture; }

	void relayout(f32vec2 const& size, i32vec2 const& windowSize, i32vec2 const& framebufferSize);

	void relayout(i32vec2 const& size);

	void newFrame();

	void drawFrame();

	template<class MouseEvent>
	inline bool handleMousePressEvent(MouseEvent& event)
	{ return handleMouseEvent(event, true); }

	template<class MouseEvent>
	inline bool handleMouseReleaseEvent(MouseEvent& event)
	{ return handleMouseEvent(event, false); }

	template<class MouseScrollEvent>
	inline bool handleMouseScrollEvent(MouseScrollEvent& event)
	{
		ImGui::SetCurrentContext(_context);

		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2{f32vec2{event.position()} * _eventScaling};
		io.MouseWheel += event.offset().y();
		io.MouseWheelH += event.offset().x();
		return io.WantCaptureMouse;
	}

	template<class MouseMoveEvent>
	inline bool handleMouseMoveEvent(MouseMoveEvent& event)
	{
		ImGui::SetCurrentContext(_context);

		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2{f32vec2{event.position()} * _eventScaling};
		return io.WantCaptureMouse;
	}

	template<class KeyEvent>
	inline bool handleKeyPressEvent(KeyEvent& event)
	{ return handleKeyEvent(event, true); }

	template<class KeyEvent>
	inline bool handleKeyReleaseEvent(KeyEvent& event)
	{ return handleKeyEvent(event, false); }

	template<class TextInputEvent>
	inline bool handleTextInputEvent(TextInputEvent& event)
	{
		ImGui::SetCurrentContext(_context);

		ImGui::GetIO().AddInputCharactersUTF8(event.text().data());
		return false;
	}

	template<class Application>
	inline void updateApplicationCursor(Application& application)
	{
		ImGui::SetCurrentContext(_context);

		switch (ImGui::GetMouseCursor())
		{
			case ImGuiMouseCursor_TextInput:
				application.setCursor(Application::Cursor::TextInput);
				return;
			case ImGuiMouseCursor_ResizeNS:
				application.setCursor(Application::Cursor::ResizeNS);
				return;
			case ImGuiMouseCursor_ResizeEW:
				application.setCursor(Application::Cursor::ResizeWE);
				return;
			case ImGuiMouseCursor_Hand:
				application.setCursor(Application::Cursor::Hand);
				return;
			case ImGuiMouseCursor_None:
				application.setCursor(Application::Cursor::Hidden);
				return;

				/* For unknown cursors we set Arrow as well */
			case ImGuiMouseCursor_Arrow:
			default:
				application.setCursor(Application::Cursor::Arrow);
				return;
		}

		CORRADE_INTERNAL_ASSERT_UNREACHABLE();
	}

private:
	template<class KeyEvent>
	inline bool handleKeyEvent(KeyEvent& event, bool value)
	{
		ImGui::SetCurrentContext(_context);

		ImGuiIO& io = ImGui::GetIO();

		switch (event.key())
		{
			case KeyEvent::Key::LeftShift:
			case KeyEvent::Key::RightShift:
				io.KeyShift = value;
				break;
			case KeyEvent::Key::LeftCtrl:
			case KeyEvent::Key::RightCtrl:
				io.KeyCtrl = value;
				break;
			case KeyEvent::Key::LeftAlt:
			case KeyEvent::Key::RightAlt:
				io.KeyAlt = value;
				break;
			case KeyEvent::Key::LeftSuper:
			case KeyEvent::Key::RightSuper:
				io.KeySuper = value;
				break;
			case KeyEvent::Key::Tab:
				io.KeysDown[ImGuiKey_Tab] = value;
				break;
			case KeyEvent::Key::Up:
				io.KeysDown[ImGuiKey_UpArrow] = value;
				break;
			case KeyEvent::Key::Down:
				io.KeysDown[ImGuiKey_DownArrow] = value;
				break;
			case KeyEvent::Key::Left:
				io.KeysDown[ImGuiKey_LeftArrow] = value;
				break;
			case KeyEvent::Key::Right:
				io.KeysDown[ImGuiKey_RightArrow] = value;
				break;
			case KeyEvent::Key::Home:
				io.KeysDown[ImGuiKey_Home] = value;
				break;
			case KeyEvent::Key::End:
				io.KeysDown[ImGuiKey_End] = value;
				break;
			case KeyEvent::Key::PageUp:
				io.KeysDown[ImGuiKey_PageUp] = value;
				break;
			case KeyEvent::Key::PageDown:
				io.KeysDown[ImGuiKey_PageDown] = value;
				break;
			case KeyEvent::Key::Enter:
			case KeyEvent::Key::NumEnter:
				io.KeysDown[ImGuiKey_Enter] = value;
				break;
			case KeyEvent::Key::Esc:
				io.KeysDown[ImGuiKey_Escape] = value;
				break;
			case KeyEvent::Key::Space:
				io.KeysDown[ImGuiKey_Space] = value;
				break;
			case KeyEvent::Key::Backspace:
				io.KeysDown[ImGuiKey_Backspace] = value;
				break;
			case KeyEvent::Key::Delete:
				io.KeysDown[ImGuiKey_Delete] = value;
				break;
			case KeyEvent::Key::A:
				io.KeysDown[ImGuiKey_A] = value;
				break;
			case KeyEvent::Key::C:
				io.KeysDown[ImGuiKey_C] = value;
				break;
			case KeyEvent::Key::V:
				io.KeysDown[ImGuiKey_V] = value;
				break;
			case KeyEvent::Key::X:
				io.KeysDown[ImGuiKey_X] = value;
				break;
			case KeyEvent::Key::Y:
				io.KeysDown[ImGuiKey_Y] = value;
				break;
			case KeyEvent::Key::Z:
				io.KeysDown[ImGuiKey_Z] = value;
				break;

				/* Unknown key, do nothing */
			default:
				return false;
		}

		return io.WantCaptureKeyboard;
	}

	template<class MouseEvent>
	inline bool handleMouseEvent(MouseEvent& event, bool value)
	{
		ImGui::SetCurrentContext(_context);

		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2{f32vec2{event.position()} * _eventScaling};

		std::size_t buttonId;
		switch (event.button())
		{
			case MouseEvent::Button::Left:
				buttonId = 0;
				break;
			case MouseEvent::Button::Right:
				buttonId = 1;
				break;
			case MouseEvent::Button::Middle:
				buttonId = 2;
				break;

				/* Unknown button, do nothing */
			default:
				return false;
		}

		_mousePressed.set(buttonId, value);
		if (value) _mousePressedInThisFrame.set(buttonId, true);

		return io.WantCaptureMouse;
	}

	ImGuiContext* _context;
	Magnum::Shaders::Flat2D _shader;
	Magnum::GL::Texture2D _texture{Magnum::NoCreate};
	Magnum::GL::Buffer _vertexBuffer{Magnum::GL::Buffer::TargetHint::Array};
	Magnum::GL::Buffer _indexBuffer{Magnum::GL::Buffer::TargetHint::ElementArray};
	Magnum::Timeline _timeline;
	Magnum::GL::Mesh _mesh;
	f32vec2 _supersamplingRatio, _eventScaling;
	Magnum::BoolVector3 _mousePressed, _mousePressedInThisFrame;
};
