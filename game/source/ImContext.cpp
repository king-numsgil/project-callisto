#include <Corrade/Containers/Reference.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Corrade/Utility/Resource.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/GL/PixelFormat.h>
#include <Magnum/GL/Extensions.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/Shader.h>
#include <Magnum/ImageView.h>
#include <cstring>

#include "ImContext.hpp"

using namespace Magnum;

ImContext::ImContext(f32vec2 const& size, i32vec2 const& windowSize, i32vec2 const& framebufferSize)
		: ImContext{*ImGui::CreateContext(), size, windowSize, framebufferSize}
{}

ImContext::ImContext(i32vec2 const& size)
		: ImContext{f32vec2{size}, size, size}
{}

ImContext::ImContext(ImGuiContext& context, f32vec2 const& size, i32vec2 const& windowSize,
                     i32vec2 const& framebufferSize)
		: _context{&context}, _shader{Shaders::Flat2D::Flag::Textured | Shaders::Flat2D::Flag::VertexColor}
{
	ImGui::SetCurrentContext(&context);

	ImGuiIO& io = ImGui::GetIO();
	io.KeyMap[ImGuiKey_Tab] = ImGuiKey_Tab;
	io.KeyMap[ImGuiKey_LeftArrow] = ImGuiKey_LeftArrow;
	io.KeyMap[ImGuiKey_RightArrow] = ImGuiKey_RightArrow;
	io.KeyMap[ImGuiKey_UpArrow] = ImGuiKey_UpArrow;
	io.KeyMap[ImGuiKey_DownArrow] = ImGuiKey_DownArrow;
	io.KeyMap[ImGuiKey_PageUp] = ImGuiKey_PageUp;
	io.KeyMap[ImGuiKey_PageDown] = ImGuiKey_PageDown;
	io.KeyMap[ImGuiKey_Home] = ImGuiKey_Home;
	io.KeyMap[ImGuiKey_End] = ImGuiKey_End;
	io.KeyMap[ImGuiKey_Delete] = ImGuiKey_Delete;
	io.KeyMap[ImGuiKey_Backspace] = ImGuiKey_Backspace;
	io.KeyMap[ImGuiKey_Space] = ImGuiKey_Space;
	io.KeyMap[ImGuiKey_Enter] = ImGuiKey_Enter;
	io.KeyMap[ImGuiKey_Escape] = ImGuiKey_Escape;
	io.KeyMap[ImGuiKey_A] = ImGuiKey_A;
	io.KeyMap[ImGuiKey_C] = ImGuiKey_C;
	io.KeyMap[ImGuiKey_V] = ImGuiKey_V;
	io.KeyMap[ImGuiKey_X] = ImGuiKey_X;
	io.KeyMap[ImGuiKey_Y] = ImGuiKey_Y;
	io.KeyMap[ImGuiKey_Z] = ImGuiKey_Z;

	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;

	relayout(size, windowSize, framebufferSize);

	_mesh.setPrimitive(GL::MeshPrimitive::Triangles);
	_mesh.addVertexBuffer(_vertexBuffer, 0,
	                      Shaders::Flat2D::Position{},
	                      Shaders::Flat2D::TextureCoordinates{},
	                      Shaders::Flat2D::Color4{
			                      Shaders::Flat2D::Color4::DataType::UnsignedByte,
			                      Shaders::Flat2D::Color4::DataOption::Normalized});

	_timeline.start();
}

ImContext::ImContext(ImGuiContext& context, i32vec2 const& size)
		: ImContext{context, f32vec2{size}, size, size}
{}

ImContext::ImContext(Magnum::NoCreateT) noexcept
		: _context{nullptr}, _shader{NoCreate}, _texture{NoCreate}, _vertexBuffer{NoCreate}, _indexBuffer{NoCreate},
		  _mesh{NoCreate}
{}

ImContext::ImContext(ImContext&& other) noexcept
		: _context{other._context}, _shader{std::move(other._shader)}, _texture{std::move(other._texture)},
		  _vertexBuffer{std::move(other._vertexBuffer)}, _indexBuffer{std::move(other._indexBuffer)},
		  _timeline{other._timeline}, _mesh{std::move(other._mesh)}, _supersamplingRatio{other._supersamplingRatio},
		  _eventScaling{other._eventScaling}
{
	other._context = nullptr;

	ImGuiContext* current = ImGui::GetCurrentContext();
	ImGui::SetCurrentContext(_context);
	ImGui::GetIO().Fonts->SetTexID(reinterpret_cast<ImTextureID>(&_texture));
	ImGui::SetCurrentContext(current);
}

ImContext::~ImContext()
{
	if (_context)
	{
		ImGui::SetCurrentContext(_context);
		ImGui::DestroyContext();
	}
}

ImContext& ImContext::operator=(ImContext&& other) noexcept
{
	std::swap(_context, other._context);
	std::swap(_shader, other._shader);
	std::swap(_texture, other._texture);
	std::swap(_vertexBuffer, other._vertexBuffer);
	std::swap(_indexBuffer, other._indexBuffer);
	std::swap(_timeline, other._timeline);
	std::swap(_mesh, other._mesh);
	std::swap(_supersamplingRatio, other._supersamplingRatio);
	std::swap(_eventScaling, other._eventScaling);

	/* Update the pointers to _texture */
	ImGuiContext* current = ImGui::GetCurrentContext();
	if (_context)
	{
		ImGui::SetCurrentContext(_context);
		ImGui::GetIO().Fonts->SetTexID(reinterpret_cast<ImTextureID>(&_texture));
	}
	if (other._context)
	{
		ImGui::SetCurrentContext(other._context);
		ImGui::GetIO().Fonts->SetTexID(reinterpret_cast<ImTextureID>(&other._texture));
	}
	ImGui::SetCurrentContext(current);

	return *this;
}

ImGuiContext* ImContext::release()
{
	ImGuiContext* context = _context;
	_context = nullptr;
	return context;
}

void ImContext::relayout(f32vec2 const& size, i32vec2 const& windowSize, i32vec2 const& framebufferSize)
{
	ImGui::SetCurrentContext(_context);

	const f32vec2 supersamplingRatio = f32vec2{framebufferSize} / size;

	_eventScaling = size / f32vec2{windowSize};

	ImGuiIO& io = ImGui::GetIO();

	bool allFontsLoaded = !io.Fonts->Fonts.empty();
	for (auto& font: io.Fonts->Fonts)
		if (!font->IsLoaded())
		{
			allFontsLoaded = false;
			break;
		}

	if (_supersamplingRatio != supersamplingRatio || !allFontsLoaded)
	{
		const f32 nonZeroSupersamplingRatio = (supersamplingRatio.x() > .0f ? supersamplingRatio.x() : 1.f);

		if (io.Fonts->Fonts.empty()
		    || (io.Fonts->Fonts.size() == 1
		        && std::strcmp(io.Fonts->Fonts[0]->GetDebugName(), "ProggyClean.ttf, 13px [SCALED]") == 0))
		{
			io.Fonts->Clear();

			ImFontConfig cfg;
			std::strcpy(cfg.Name, "ProggyClean.ttf, 13px [SCALED]");
			cfg.SizePixels = 13.f * nonZeroSupersamplingRatio;
			io.Fonts->AddFontDefault(&cfg);
		}

		_supersamplingRatio = supersamplingRatio;

		io.FontGlobalScale = 1.f / nonZeroSupersamplingRatio;

		u8* pixels;
		i32 width, height, pixelSize;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &pixelSize);
		CORRADE_INTERNAL_ASSERT(width > 0 && height > 0 && pixelSize == 4);

		ImageView2D image{GL::PixelFormat::RGBA,
		                  GL::PixelType::UnsignedByte, {width, height},
		                  {pixels, std::size_t(pixelSize * width * height)}};

		_texture = GL::Texture2D{};
		_texture.setMagnificationFilter(GL::SamplerFilter::Linear)
				.setMinificationFilter(GL::SamplerFilter::Linear)
				.setStorage(1, GL::TextureFormat::RGBA8, image.size())
				.setSubImage(0, {}, image);

		io.Fonts->ClearTexData();

		io.Fonts->SetTexID(reinterpret_cast<ImTextureID>(&_texture));
	}

	io.DisplaySize = ImVec2{f32vec2{size}};
}

void ImContext::relayout(i32vec2 const& size)
{ relayout(f32vec2{size}, size, size); }

void ImContext::newFrame()
{
	ImGui::SetCurrentContext(_context);

	_timeline.nextFrame();

	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = _timeline.previousFrameDuration();
	if (ImGui::GetFrameCount() != 0)
		io.DeltaTime = Math::max(io.DeltaTime, std::numeric_limits<float>::epsilon());

	for (const i32 buttonId: {0, 1, 2})
		io.MouseDown[buttonId] = _mousePressed[buttonId] || _mousePressedInThisFrame[buttonId];

	ImGui::NewFrame();

	_mousePressedInThisFrame = {};
}

void ImContext::drawFrame()
{
	ImGui::SetCurrentContext(_context);

	ImGui::Render();

	ImGuiIO& io = ImGui::GetIO();
	const f32vec2 fbSize = Vector2{io.DisplaySize} * Vector2{io.DisplayFramebufferScale};
	if (!fbSize.product()) return;

	ImDrawData* drawData = ImGui::GetDrawData();
	CORRADE_INTERNAL_ASSERT(drawData);
	drawData->ScaleClipRects(io.DisplayFramebufferScale);

	const f32mat3 projection =
			f32mat3::translation({-1.f, 1.f}) *
			f32mat3::scaling({2.f / f32vec2{io.DisplaySize}}) *
			f32mat3::scaling({1.f, -1.f});
	_shader.setTransformationProjectionMatrix(projection);

	for (std::int_fast32_t n = 0; n < drawData->CmdListsCount; ++n)
	{
		const ImDrawList* cmdList = drawData->CmdLists[n];
		ImDrawIdx indexBufferOffset = 0;

		_vertexBuffer.setData({cmdList->VtxBuffer.Data, std::size_t(cmdList->VtxBuffer.Size)},
		                      GL::BufferUsage::StreamDraw);
		_indexBuffer.setData({cmdList->IdxBuffer.Data, std::size_t(cmdList->IdxBuffer.Size)},
		                     GL::BufferUsage::StreamDraw);

		for (std::int_fast32_t c = 0; c < cmdList->CmdBuffer.Size; ++c)
		{
			const ImDrawCmd* pcmd = &cmdList->CmdBuffer[c];

			GL::Renderer::setScissor(i32range2{f32range2{
					{pcmd->ClipRect.x, fbSize.y() - pcmd->ClipRect.w},
					{pcmd->ClipRect.z, fbSize.y() - pcmd->ClipRect.y}}
					                                   .scaled(_supersamplingRatio)});

			_mesh.setCount(pcmd->ElemCount);
			_mesh.setIndexBuffer(_indexBuffer, indexBufferOffset * sizeof(ImDrawIdx),
			                     sizeof(ImDrawIdx) == 2
			                     ? GL::MeshIndexType::UnsignedShort
			                     : GL::MeshIndexType::UnsignedInt);

			indexBufferOffset += pcmd->ElemCount;

			_shader.bindTexture(*static_cast<GL::Texture2D*>(pcmd->TextureId))
					.draw(_mesh);
		}
	}

	GL::Renderer::setScissor(i32range2{f32range2{{}, fbSize}.scaled(_supersamplingRatio)});
}
