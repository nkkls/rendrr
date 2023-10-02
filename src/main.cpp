#include <SDL2/SDL.h>
#include <logger.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <vector>
#include <cmath>
#include <format>


int width = 500; 
int height = 500;
char name[] = "meow!";

struct Vertex {
    float x, y, z;
};

int main(int argc, char ** argv)
{   

    printlog(0, "init", "initializing SDL!");
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printlog(3, "init", "failed initializing SDL!");
        return -1;
    }

    printlog(0, "init", "loading obj file");
    std::ifstream objFile("../obj/teapot.obj");
    if (!objFile.is_open()) {
        printlog(3, "init", "failed to load obj file");
        return 1;
    }

    std::vector<Vertex> vertices;
    std::vector<std::vector<int>> faces; 
    std::string line;

    while (std::getline(objFile, line)) {
    if (line.substr(0, 2) == "v ") {
        Vertex vertex;
        sscanf(line.c_str(), "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
        vertices.push_back(vertex);
    } // this works

    else if (line.substr(0, 2) == "f ") {
        std::istringstream iss(line.substr(2));
        std::vector<int> faceIndices;
        int vertexIndex, textureIndex, normalIndex;

            while (iss >> vertexIndex) {
                if (iss.peek() == '/') {
                    iss.ignore();
                    if (iss.peek() != '/') iss >> textureIndex;
                        if (iss.peek() == '/') {
                            iss.ignore();
                            iss >> normalIndex;
                        }
                    }

                faceIndices.push_back(vertexIndex);
            }
        faces.push_back(faceIndices);
        }   
    }

    objFile.close();

    // Create a window
    printf("[rendrr/init] creating window!\n");
    SDL_Window *window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          width,
                                          height,
                                          SDL_WINDOW_OPENGL);
    if (window == nullptr)
    {
        SDL_Log("\n \033[0;31m [rendrr/init] Could not create a window: %s\033[0;37m \n", SDL_GetError());
        return -1;  
    }

    printf("[rendrr/init] initializing renderer!\n");
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("\n[rendrr/init] Could not create a renderer: %s\n\n", SDL_GetError());
        return -1;
    }

    printf("\033[0;32m[rendrr/init] completed initalization, starting loop!\n\n");
    Uint32 lastTime = SDL_GetTicks();
    while (true)
    {
        // Get the next event
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 49, 26, 34,255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255,255,255,100);

        for (const std::vector<int>& faceIndices : faces) {
            for (size_t i = 0; i < faceIndices.size(); ++i) {
                const Vertex& v0 = vertices[faceIndices[i] - 1];
                const Vertex& v1 = vertices[faceIndices[(i + 1) % faceIndices.size()] -1 ];
                const Vertex& v2 = vertices[faceIndices[(i + 2) % faceIndices.size()] -1 ];
                
                // const Vertex& v3 = vertices[faceIndices[(i + 3) % faceIndices.size()] -1 ];
                // printlog(0, "obj", std::format("Processing {} / {}, Vertex; {} {} / {} {} / {} {}", i, faceIndices.size(), v0.x, v0.y, v1.x, v1.y, v2.x,v2.y).data());

                float x0 = ((v0.x * width / 7.0) + width / 2);
                float y0 = (((v0.y - 1.5) * height / 7.0) + height / 2);
                
                float x1 = ((v1.x * width / 7.0) + width / 2);
                float y1 = (((v1.y - 1.5) * height / 7.0) + height / 2);

                float x2 = ((v2.x * width / 7.0) + width / 2);
                float y2 = (((v2.y - 1.5) * height / 7.0) + height / 2);

                // float x3 = ((v3.x * width / 6.0) + width / 2);
                // float y3 = (((v3.y - 1.5) * height / 6.0) + height / 2);

                SDL_Vertex sv0 = {{x0,y0},{rand()%255,rand()%255,rand()%255},{1,1}};
                SDL_Vertex sv1 = {{x1,y1},{rand()%255,rand()%255,rand()%255},{1,1}};
                SDL_Vertex sv2 = {{x2,y2},{rand()%255,rand()%255,rand()%255},{1,1}};
                // SDL_Vertex sv3 = {{x3,y3},{rand()%255,rand()%255,rand()%255},{1,1}};

                SDL_Vertex sv_vertices[] = {
                    sv0,
                    sv1,
                    sv2,
                    // sv3
                };

                // Render the line segment between the transformed vertices
                // nvm fuck dat shit we making geometry

                SDL_RenderGeometry(renderer,nullptr,sv_vertices,3   ,NULL,0);  
            }
        }
        SDL_RenderPresent(renderer);
    }
    // Tidy up
    printf("[rendrr/main] see you next time!");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

