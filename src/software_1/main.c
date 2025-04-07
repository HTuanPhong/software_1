#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_shadercross/SDL_shadercross.h>
typedef struct AppState AppState;
struct AppState
{
	SDL_Window *window;
	SDL_GPUDevice *gpu_device;
	const char* base_path;
	SDL_GPUGraphicsPipeline *fill_pipe;
	SDL_GPUGraphicsPipeline *line_pipe;
};

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	static AppState state = {0}; 
	*appstate = &state;

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
	{
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, "60");

	state.window = SDL_CreateWindow("software_1", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (!state.window)
	{
		SDL_Log("CreateWindow failed: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

    state.gpu_device = SDL_CreateGPUDevice(
        SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL,
        false,
        NULL);
    if (!state.gpu_device)
	{
        SDL_Log("CreateGPUDevice failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

	if (!SDL_ClaimWindowForGPUDevice(state.gpu_device, state.window))
	{
		SDL_Log("GPUClaimWindow failed: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	state.base_path = SDL_GetBasePath();

	char fullPath[256];
	SDL_snprintf(fullPath, sizeof(fullPath), "%sb.vert.hlsl", state.base_path);
	void *vert_shader_file = SDL_LoadFile(fullPath, NULL);
	SDL_ShaderCross_HLSL_Info hlsl_info = {
		.source = vert_shader_file,
		.entrypoint = "main",
		.shader_stage = SDL_GPU_SHADERSTAGE_VERTEX
	};
	SDL_ShaderCross_GraphicsShaderMetadata temp;
	SDL_GPUShader* vertex_shader = SDL_ShaderCross_CompileGraphicsShaderFromHLSL(state.gpu_device, &hlsl_info, &temp);
	if (vertex_shader == NULL)
	{
		SDL_Log("Failed to create vertex shader!");
		return SDL_APP_FAILURE;
	}

	SDL_snprintf(fullPath, sizeof(fullPath), "%sa.frag.hlsl", state.base_path);
	void *frag_shader_file = SDL_LoadFile(fullPath, NULL);
	SDL_ShaderCross_HLSL_Info hlsl_info2 = {
		.source = frag_shader_file,
		.entrypoint = "main",
		.shader_stage = SDL_GPU_SHADERSTAGE_FRAGMENT
	};
	SDL_ShaderCross_GraphicsShaderMetadata temp2;
	SDL_GPUShader* fragment_shader = SDL_ShaderCross_CompileGraphicsShaderFromHLSL(state.gpu_device, &hlsl_info2, &temp2);
	if (fragment_shader == NULL)
	{
		SDL_Log("Failed to create fragment shader!");
		return SDL_APP_FAILURE;
	}

	SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = {
		.target_info = {
			.num_color_targets = 1,
			.color_target_descriptions = (SDL_GPUColorTargetDescription[]){{
				.format = SDL_GetGPUSwapchainTextureFormat(state.gpu_device, state.window)
			}},
		},
		.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
		.vertex_shader = vertex_shader,
		.fragment_shader = fragment_shader,
	};

	pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
	state.fill_pipe = SDL_CreateGPUGraphicsPipeline(state.gpu_device, &pipelineCreateInfo);
	if (state.fill_pipe == NULL)
	{
		SDL_Log("Failed to create fill pipeline!");
		return SDL_APP_FAILURE;
	}

	pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_LINE;
	state.line_pipe = SDL_CreateGPUGraphicsPipeline(state.gpu_device, &pipelineCreateInfo);
	if (state.line_pipe == NULL)
	{
		SDL_Log("Failed to create line pipeline!");
		return SDL_APP_FAILURE;
	}

	SDL_ReleaseGPUShader(state.gpu_device, vertex_shader);
	SDL_ReleaseGPUShader(state.gpu_device, fragment_shader);
	SDL_free(vert_shader_file);
	SDL_free(frag_shader_file);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
	AppState* state = appstate;
	SDL_GPUCommandBuffer *cmdbuf = SDL_AcquireGPUCommandBuffer(state->gpu_device);
	if (!cmdbuf)
    {
        SDL_Log("AcquireGPUCommandBuffer failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
	SDL_GPUTexture* swapchainTexture;
	if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmdbuf, state->window, &swapchainTexture, NULL, NULL))
	{
        SDL_Log("WaitAndAcquireGPUSwapchainTexture failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
	if (swapchainTexture != NULL)
	{
		SDL_GPUColorTargetInfo colorTargetInfo = { 0 };
		colorTargetInfo.texture = swapchainTexture;
		colorTargetInfo.clear_color = (SDL_FColor){ 0.0f, 0.0f, 0.0f, 1.0f };
		colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
		colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

		SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(cmdbuf, &colorTargetInfo, 1, NULL);
		SDL_BindGPUGraphicsPipeline(renderPass, state->fill_pipe);
		SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);
		SDL_EndGPURenderPass(renderPass);
	}
	SDL_SubmitGPUCommandBuffer(cmdbuf);
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
	AppState* state = appstate;
	switch (event->type)
	{
	case SDL_EVENT_QUIT:
		return SDL_APP_SUCCESS;
		break;
	}
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
	(void)result;
	AppState* state = appstate;	
	SDL_ReleaseWindowFromGPUDevice(state->gpu_device, state->window);
	SDL_DestroyWindow(state->window);
	SDL_DestroyGPUDevice(state->gpu_device);
	SDL_Quit();
 	return;
}
