#include <SDL2/SDL.h>

#include <logger.h>
#include <modelr.h>
#include <vec.h>

#include <iostream>
#include <string>

#include <vector>
#include <cmath>
#include <format>

int width = 800; 
int height = 800;
char name[] = "teapot";

void RenderModel(SDL_Renderer* renderer, const modelr& model, float modelShrink, float xOff, float yOff) {
    const std::vector<std::vector<int>> faces = model.GetFaces();
    const std::vector<modelr::Vertex>& vertices = model.GetVertices();

    Vec3f light_dir(0,0,1);

    Vec2f screen_coords[3]; 
    Vec3f world_coords[3]; 

    for (const std::vector<int>& faceIndices : faces) {
        const modelr::Vertex& a = vertices[faceIndices[0] - 1 ];
        screen_coords[0] = Vec2f((((a.x + xOff) * width / modelShrink) + width / 2), ((-(a.y + yOff) * height / modelShrink) + height / 2)); 
        world_coords[0] = Vec3f(a.x,a.y,a.z);

        for (size_t i = 0; i < faceIndices.size() - 1; ++i) {

            const modelr::Vertex& v0 = vertices[faceIndices[i] - 1 ];
            screen_coords[1] = Vec2f((((v0.x + xOff) * width / modelShrink) + width / 2), ((-(v0.y + yOff) * height / modelShrink) + height / 2)); 
            
            const modelr::Vertex& v1 = vertices[faceIndices[i + 1] - 1 ];
            screen_coords[2] = Vec2f((((v1.x + xOff) * width / modelShrink) + width / 2), ((-(v1.y + yOff) * height / modelShrink) + height / 2)); 
        
            world_coords[1] = Vec3f(v0.x,v0.y,v0.z);
            world_coords[2] = Vec3f(v1.x,v1.y,v1.z);

            Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]); 
            n.normalize();

            float intensity = n*light_dir;
            if (intensity > 0) {
                SDL_Vertex sv0 = {{screen_coords[0].x,screen_coords[0].y},{intensity*255},{1,1}};
                SDL_Vertex sv1 = {{screen_coords[1].x,screen_coords[1].y},{intensity*255},{1,1}};
                SDL_Vertex sv2 = {{screen_coords[2].x,screen_coords[2].y},{intensity*255},{1,1}};

                SDL_Vertex sv_vertices[] = {
                    sv0,
                    sv1,
                    sv2,
                };
                    
                SDL_RenderGeometry(renderer,nullptr,sv_vertices,3,NULL,0);
            }
            // const modelr::Vertex& v1 = vertices[faceIndices[i] - 1 ]; // thank you unnick + dunkyl <3
            // const modelr::Vertex& v2 = vertices[faceIndices[i+1] - 1 ];
                
            // float x0 = (((v0.x + xOff) * width / modelShrink) + width / 2);
            // float y0 = ((-(v0.y + yOff) * height / modelShrink) + height / 2);
                
            // float x1 = (((v1.x + xOff) * width / modelShrink) + width / 2);
            // float y1 = ((-(v1.y + yOff) * height / modelShrink) + height / 2);

            // float x2 = (((v2.x + xOff) * width / modelShrink) + width / 2);
            // float y2 = ((-(v2.y + yOff) * height / modelShrink) + height / 2);
        }
    }
}

int main(int argc, char ** argv)
{   
    printlog(0, "init", "initializing SDL!");
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printlog(3, "init", "failed initializing SDL!");
        return -1;
    }

    printlog(0, "init", "loading obj file");

    modelr model("../obj/teapot.obj");
    if (model.LoadModel()) {
        printlog(1, "obj", "model loaded!");
    } else {
        printlog(3, "obj", "model did Not load!");
    }

    // Create a window
    printlog(0, "init", "creating a window!");
    SDL_Window *window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          width,
                                          height,
                                          SDL_WINDOW_RESIZABLE);
    if (window == nullptr)
    {
        printlog(3, "init", "could not create window!");
        return -1;  
    }

    printlog(0, "init", "initializing renderer");
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        printlog(3,"init", "cannot render </3");
        return -1;
    }

    printlog(1, "init", "[rendrr/init] completed initalization, starting loop!\n");
    Uint32 lastTime = SDL_GetTicks();
    
    while (true)
    {
        // Get the next event
        Uint32 start_time, frame_time;
        float fps;

        SDL_Event event;
        start_time = SDL_GetTicks();
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 49, 26, 34,255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255,255,255,50);

        RenderModel(renderer, model, 7.0, 0, -1.5);

        frame_time = SDL_GetTicks()-start_time;
        fps = (frame_time > 0) ? 1000.0f / frame_time : 0.0f;

        printlog(2, "loop", std::format("running at {} fps", fps).data());
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }
    // Tidy up
    printf("[rendrr/main] see you next time!");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

