
#include "SandboxApp.h"

#include <Simplex.h>
#include <Ref.h>
#include <Event.h>

#include <functional>

Scope<App> App::CreateApp()
{
	return CreateScope<SandboxApp>();
}

SandboxApp::SandboxApp()
	: m_Running(false)
{
}

SandboxApp::~SandboxApp()
{
}

void SandboxApp::Run()
{
	m_Running = true;

	LOG_INFO("Hello world");
	LOG_WARN("Hello world");
	LOG_ERROR("Hello world");
	LOG_CRITICAL("Hello world");

	m_Window = Window::Create();

	gfx = m_Window->GetGraphicsContext();

	std::vector<float> data = {
		-0.5f, -0.5f,	1.0f, 0.0f, 1.0f, 1.0f,
		 0.0f,  0.5f,	0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,	0.0f, 1.0f, 0.0f, 1.0f
	};

	VertexArrayProps va_create;
	va_create.topology = SXG_TOPOLOGY::TRIANGLES;
	va = gfx->CreateArray(va_create);

	VertexBufferLayout layout = {
		{ SXG_TYPES::FLOAT2, "positions" },
		{ SXG_TYPES::FLOAT4, "colors" }
	};

	VertexBufferProps vb_create = { data, layout };
	vb = gfx->CreateBuffer(vb_create);

	va->AddBuffer(vb);

	Ref<Pipeline> shader = gfx->CreatePipelineFromFiles("res/vert.glsl", "res/frag.glsl");

	gfx->SetPipeline(shader);

	gfx->ClearColor(1.0f, 0.2f, 1.0f, 1.0f);

	m_Window->RegisterEventCallback([&](Event &e) { OnEvent(e); });

	m_Window->SetVisible();

	// Update
	while (m_Running)
	{
		OnUpdate();
		m_Window->Update();
	}
	//

	LOG_INFO("Terminating...");
}

void SandboxApp::OnEvent(Event &e)
{
	e.Dispatch<WindowClose>([&](auto)
	{
		m_Running = false;
	});

	e.Dispatch<KeyDown>([](KeyDown e)
	{
		LOG_INFO("Key down {0}", (char) e.keyCode);
	});

	e.Dispatch<MouseButtonDown>([](MouseButtonDown e)
	{
		LOG_INFO("Mouse button down {0}", e.mouseButton);
	});

	e.Dispatch<MouseScroll>([](MouseScroll e)
	{
		LOG_INFO("Mouse scrolled {0}", e.velocityY);
	});
}

void SandboxApp::OnUpdate()
{
	gfx->SetArray(va);

	gfx->ClearRenderTarget(SXG::COLOR_BUFFER_BIT);
	gfx->Draw(va->GetVertexCount());
}
