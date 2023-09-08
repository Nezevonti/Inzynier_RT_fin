// RT_inz_copy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "sphere.h"
#include "hittable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include "Quad.h"
#include "Box.h"
#include "Random.h"

#include "VoxelGrid.h"


#include <Windows.h>
#include <vector>
// Window dimensions
const int WindowWidth = 1920/2;
const int WindowHeight = 1080/2;

Vec3 pixels[WindowHeight][WindowWidth];

// Win32 Window Procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Iterate over the pixels and set their colors on the window
        for (int y = 0; y < WindowHeight; ++y)
        {
            for (int x = 0; x < WindowWidth; ++x)
            {
                int index = y * WindowWidth + x;
                Vec3 pixel = pixels[y][x];

                COLORREF color = RGB(pixel.r(), pixel.g(), pixel.b());
                SetPixel(hdc, x, y, color);
            }
        }

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


Vec3 color(const Ray& r, Hittable* world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001, FLT_MAX, rec)) {
        Ray scattered;
        Vec3 attenuation;
        if (depth < 8 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1);
        }
        else {
            return Vec3(0, 0, 0);
        }
    }
    else {
        Vec3 unit_direction = unit_vector(r.direction);
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
    }
}


Hittable* random_scene() {
    int n = 500;
    Hittable** list = new Hittable * [n + 1];
    list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new lambertian(Vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = random_double();
            Vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
            if ((center - Vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) {  // diffuse
                    list[i++] = new Sphere(
                        center, 0.2,
                        new lambertian(Vec3(random_double() * random_double(),
                            random_double() * random_double(),
                            random_double() * random_double()))
                    );
                }
                else if (choose_mat < 0.95) { // metal
                    list[i++] = new Sphere(
                        center, 0.2,
                        new metal(Vec3(0.5 * (1 + random_double()),
                            0.5 * (1 + random_double()),
                            0.5 * (1 + random_double())),
                            0.5 * random_double())
                    );
                }
                else {  // glass
                    list[i++] = new Sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new Sphere(Vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new Sphere(Vec3(-4, 1, 0), 1.0, new lambertian(Vec3(0.4, 0.2, 0.1)));
    list[i++] = new Sphere(Vec3(4, 1, 0), 1.0, new metal(Vec3(0.7, 0.6, 0.5), 0.0));

    return new Hittable_List(list, i);
}

Hittable* simple_scene() {

    int n = 4;
    Hittable** list = new Hittable * [n + 1];
    list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new lambertian(Vec3(0.5, 0.5, 0.5)));


    list[1] = new Sphere(Vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[2] = new Sphere(Vec3(-3, 1, 0), 1.0, new lambertian(Vec3(0.4, 0.2, 0.1)));
    list[3] = new Sphere(Vec3(3, 1, 0), 1.0, new metal(Vec3(0.7, 0.6, 0.5), 0.0));

    return new Hittable_List(list, 4);
}

Hittable* simpleSceneList() {

    int n = 5;
    Hittable** list = new Hittable * [n + 1];
    //list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new lambertian(Vec3(0.5, 0.5, 0.5)));


    //list[1] = new Sphere(Vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[0] = new Sphere(Vec3(-3, -1, 0), 0.25, new lambertian(Vec3(0.4, 0.2, 0.1)));
    list[1] = new Sphere(Vec3(-1.5, 0, 0), 0.25, new lambertian(Vec3(0.4, 0.2, 0.1)));
    list[2] = new Sphere(Vec3(0, 1, 0), 0.25, new lambertian(Vec3(0.4, 0.2, 0.1)));
    list[3] = new Sphere(Vec3(1.5, 0, 0), 0.25, new lambertian(Vec3(0.4, 0.2, 0.1)));
    list[4] = new Sphere(Vec3(3, -1, 0), 0.25, new lambertian(Vec3(0.4, 0.2, 0.1)));
    //list[3] = new Sphere(Vec3(3, 1, 0), 1.0, new metal(Vec3(0.7, 0.6, 0.5), 0.0));

    return new Hittable_List(list, n);
}

std::vector<Hittable*> simpleSceneVector() {
    std::vector<Hittable*> scene;

    
    //scene.push_back(new Sphere(Vec3(0, -250, 0), 250, new lambertian(Vec3(0.5, 0.5, 0.5))));
    //scene.push_back(new Sphere(Vec3(0, 1, 0), 1, new dielectric(1.5)));
    //scene.push_back(new Sphere(Vec3(-4, 1, 0), 1.0, new lambertian(Vec3(0.4, 0.2, 0.1))));
    //scene.push_back(new Sphere(Vec3(4, 1, 0), 1, new metal(Vec3(0.25, 0.25, 0.25), 0.0)));

    //scene.push_back(new Quad(Vec3(-8, 0, -8), Vec3(16, 0, 0), Vec3(0, 0, 16), new lambertian(Vec3(0.2, 0.2, 1.0))));
    
    /*
    scene.push_back(new Sphere(Vec3(-2, 0, 2), 0.75, new dielectric(1.5)));
    scene.push_back(new Sphere(Vec3(-3, -1, 0), 0.25, new lambertian(Vec3(0.4, 0.2, 0.1))));
    scene.push_back(new Sphere(Vec3(-1.5, 0, 0), 0.25, new lambertian(Vec3(0.4, 0.2, 0.1))));
    scene.push_back(new Sphere(Vec3(0, 1, 0), 0.25, new lambertian(Vec3(0.4, 0.2, 0.1))));
    scene.push_back(new Sphere(Vec3(1.5, 0, 0), 0.25, new lambertian(Vec3(0.4, 0.2, 0.1))));
    scene.push_back(new Sphere(Vec3(3, -1, 0), 0.25, new lambertian(Vec3(0.4, 0.2, 0.1))));
    scene.push_back(new Sphere(Vec3(0, 0, 0), 0.25, new lambertian(Vec3(0.4, 0.2, 0.1))));
    */

    scene.push_back(new Sphere(Vec3(11, 9, 8), 3.0, new lambertian(Vec3(0.4, 0.2, 0.1))));
    //scene.push_back(new Sphere(Vec3(8, 8, 8), 2.0, new dielectric(1.5)));
    scene.push_back(new Sphere(Vec3(5, 9, 8), 3.0, new metal(Vec3(0.25, 0.25, 0.25), 0.0)));
    //scene.push_back(new Sphere(Vec3(5, 18, 8), 1.0, new light(Vec3(1.0, 1.0, 1.0))));

    scene.push_back(new Quad(Vec3(2, 10, 2), Vec3(12, 0, 0), Vec3(0, 0, 12), new lambertian(Vec3(0.2, 0.2, 1.0))));
    scene.push_back(new Quad(Vec3(2, 19, 2), Vec3(16, 0, 0), Vec3(0, 0, 16), new light(Vec3(1.0, 1.0, 1.0))));
    scene.push_back(new Quad(Vec3(2, 3, 2), Vec3(16, 0, 0), Vec3(0, 0, 16), new lambertian(Vec3(0.2, 0.2, 1.0))));
    //scene.push_back(new Sphere(Vec3(16, 18, 8), 1.0, new light(Vec3(1, 1, 1))));

    //(-3,-2,5) -> (-3,2,1)
    //scene.push_back(new Quad(Vec3(-3, -2, 5), Vec3(0, 0, -4), Vec3(0, 4, 0), new lambertian(Vec3(1.0, 0.2, 0.2))));
    //(-2, -2, 0)->(2, 2, 0)
    //scene.push_back(new Quad(Vec3(-2, -2, 0), Vec3(4, 0, 0), Vec3(0, 4, 0), new lambertian(Vec3(1.0, 0.2, 0.2))));
    //(3, -2, 1)->(3, 2, 5)
    //scene.push_back(new Quad(Vec3(3, -2, 1), Vec3(0, 0, 4), Vec3(0, 4, 0), new lambertian(Vec3(1.0, 0.2, 0.2))));


    //THREE QUADS 
    /*
    scene.push_back(new Quad(Vec3(-3.5, -1, 0), Vec3(2, 0, 0), Vec3(0, 2, 0), new lambertian(Vec3(1.0, 0.2, 0.2))));

    scene.push_back(new Quad(Vec3(-1, -1, 0), Vec3(2, 0, -2), Vec3(0, 2, 0), new lambertian(Vec3(0.2, 1.0, 0.2))));

    //float diag = 2.5 * std::sqrt(5);
    scene.push_back(new Quad(Vec3(1.5, -1, 0), Vec3(2, 0, 0), Vec3(0, 2, 0), new lambertian(Vec3(0.2, 0.2, 1.0))));
    */

    return scene;
}

std::vector<Hittable*> sceneBallOverCamera() {
    std::vector<Hittable*> scene;

    //scene.push_back(new Quad(Vec3(2, 6, 2), Vec3(12, 0, 0), Vec3(0, 0, 12), new lambertian(Vec3(0.2, 0.2, 0.2))));

    scene.push_back(new Sphere(Vec3(8, 8, 9), 2.0, new lambertian(Vec3(0.4, 0.2, 0.1))));


    return scene;

}


std::vector<Hittable*> BallLightOcclusion() {
    std::vector<Hittable*> scene;

    //scene.push_back(new Quad(Vec3(2, 6, 2), Vec3(12, 0, 0), Vec3(0, 0, 12), new lambertian(Vec3(0.2, 0.2, 0.2))));

    scene.push_back(new Sphere(Vec3(8, 8, 5), 2.0, new light(Vec3(1.0, 1.0, 1.0))));
    scene.push_back(new Sphere(Vec3(8, 8, 9), 1.775, new lambertian(Vec3(0.4, 0.2, 0.1))));


    return scene;

}

std::vector<Hittable*> RotateBox() {
    std::vector<Hittable*> scene;

    scene.push_back(new Box(Vec3(10, 6, 15), Vec3(12, 10, 17), 0, 0, 0, new lambertian(Vec3(0.0, 0.9, 0.9))));

    scene.push_back(new Sphere(Vec3(15, 8, 14), 1.775, new lambertian(Vec3(0.8, 0.4, 0.2))));

    scene.push_back(new Sphere(Vec3(15, 10.5, 13.2), 0.5, new metal(Vec3(0.25, 0.25, 0.25), 0.0)));


    scene.push_back(new Sphere(Vec3(7, 8, 12), 2, new dielectric(1.5)));

    //scene.push_back(new Sphere(Vec3(12, 2, 12), 0.5, new light(Vec3(1.0, 1.0, 1.0))));

    //scene.push_back(new Sphere(Vec3(8, 8, 5), 2.0, new light(Vec3(1.0, 1.0, 1.0))));
    // 
    // 
    // 
    //box
    //bottom
    //scene.push_back(new Quad(Vec3(0.5, 0.5, 0.5), Vec3(19, 0, 0), Vec3(0, 0, 19), new lambertian(Vec3(0.8, 0.8, 1.0))));
    scene.push_back(new Quad(Vec3(0.5, 0.5, 0.5), Vec3(19, 0, 0), Vec3(0, 0, 19), new light(Vec3(1.0, 1.0, 1.0))));
    //top
    scene.push_back(new Quad(Vec3(0.5, 19.5, 0.5), Vec3(19, 0, 0), Vec3(0, 0, 19), new light(Vec3(1.0, 1.0, 1.0))));
    //back

    scene.push_back(new Quad(Vec3(0.5, 19.5, 19.5), Vec3(19, 0, 0), Vec3(0, -19, 0), new lambertian(Vec3(0.8, 0.8, 1.0))));
    scene.push_back(new Quad(Vec3(4, 8.5, 19.0), Vec3(5.75, 0, 0), Vec3(0, -2, 0), new lambertian(Vec3(0.99, 0.75, 0.0))));
    //scene.push_back(new Quad(Vec3(0.5, 0.5, 19.5), Vec3(19, 0, 0), Vec3(0, 19, 0), new lambertian(Vec3(0.0, 0.8, 1.0))));

    //left right
    scene.push_back(new Quad(Vec3(0.5, 19.5, 0.5), Vec3(0, 0, 19), Vec3(0, -19, 0), new lambertian(Vec3(1.0, 0.1, 0.1))));
    scene.push_back(new Quad(Vec3(19.5, 0, 0.5), Vec3(0, 0, 19), Vec3(0, 19, 0), new lambertian(Vec3(0.1, 1.0, 0.1))));

    //scene.push_back(new Quad(Vec3(0.5, 19.5, 0.5), Vec3(0, 0, 19), Vec3(0, -19, 0), new light(Vec3(1.0, 1.0, 1.0))));
    //scene.push_back(new Quad(Vec3(19.5, 0, 0.5), Vec3(0, 0, 19), Vec3(0, 19, 0), new light(Vec3(1.0, 1.0, 1.0))));

    
    return scene;

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"RayTracingWindow";
    // Create a window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"RayTracingWindow";
    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,
        L"RayTracingWindow",
        L"Ray Tracing Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WindowWidth,
        WindowHeight,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL)
    {
        return 0;
    }



    int nx = WindowWidth;
    int ny = WindowHeight;
    int ns = 16;

    int sizeA = sizeof(Voxel);



    /*
    for (int i = 0; i < 10; i++) {
        Vec3 tmp = randomDirectionWithBias(Vec3(0, 1, 0), M_PI_2);
    }
    */


    //---Box test ---
    //hit_record r;
    //AABB testBB(Vec3(-1,-1,-1),Vec3(1,1,1))




    //---Box test end---







    //std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    //Hittable* world = simpleSceneList();

    Vec3 lookfrom(10, 18, 5);
    //Vec3 lookfrom(10, 18, 0);
    //Vec3 lookfrom(10, 18, -5);
    //Vec3 lookfrom(10, 18, -10);
    //Vec3 lookfrom(10, 16, -15);
    //Vec3 lookfrom(10, 12, -20);
    //Vec3 lookfrom(10, 12, -30);
    //Vec3 lookfrom(10, 10, -40);
    //Vec3 lookfrom(10, 8, -40);
    //Vec3 lookfrom(8, 30, 40);

    Vec3 lookat(10, 8, 10);
    //Vec3 lookat(15, 10, 12);


    Vec3 focusDistance(lookfrom.x() - lookat.x(), lookfrom.y() - lookat.y(), lookfrom.z() - lookat.z());

    float dist_to_focus = std::sqrt(focusDistance.x()*focusDistance.x() + focusDistance.y()*focusDistance.y() + focusDistance.z()*focusDistance.z());
    float aperture = 0.2;

    Camera cam(lookfrom, lookat, Vec3(0, 1, 0), 16, float(nx) / float(ny), aperture, dist_to_focus);

    Vec3 gridMin(0, 0, 0);
    Vec3 gridMax(20, 20, 20);
    VoxelGrid Grid(60, 60, 60,gridMin,gridMax);

    //2D test with reflection
    //Grid.getVoxel(4, 4, 4)->material = new opticalMedium(1.5, 0.0);

    
    /*
    Grid.mainVoxel.primitives = RotateBox();
    Grid.setPrimitivesToVoxels();

    int iterator = 0;
    for (Voxel* v : Grid.voxels) {
        if (v->occupied) {
            bool b = true;
        }
        iterator++;
    }
    */
    //Grid.GridWalk_Start(Ray(Vec3(8, 9, 5), Vec3(1, 0, 1))); //R1 - OK
    //Grid.GridWalk_Start(Ray(Vec3(3, 9, 7), Vec3(2.5, 0, 1))); //R2 - OK (both refractions)
    //Grid.GridWalk_Start(Ray(Vec3(15, 9, 19), Vec3(-1, 0, -2))); //R3 - OK Like R1 but -dir 

    Grid.mainVoxel.primitives = RotateBox();//BallLightOcclusion();//simpleSceneVector();
    
    //Grid.setAABB();
    Grid.setPrimitivesToVoxels();


    opticalMedium* mediumII = new opticalMedium(1.33, 0.05, Vec3(0.3,0.3,0.5));
    
    for (Voxel* v : Grid.voxels) {
        if (v->maxPoint.y() > 8.0) continue;

        v->material = mediumII;
    }
    
    /*
    //Ray r(lookfrom,lookat-lookfrom);
    //Vec3 returned = Grid.walkGrid(r, 0);
    int iterator = 0;
    for (Voxel* v : Grid.voxels) {
        if (v->occupied) {
            bool b = true;
        }
        iterator++;
    }
    */
    

    //make new class scene that holds the voxel grid + camera, has functions for this v and can add primitives to the corresponding voxels;
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            Vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(i + random_double()) / float(nx);
                float v = float(j + random_double()) / float(ny);
                Ray r = cam.get_ray(u, v);
                //col += color(r, world, 0);
                //col = col + color(r, world, 0);
                //Vec3 returned = Grid.walkGrid(r, 0);
                Vec3 returned = Grid.GridWalk_Start(r);

                //check for NaN errors
                if (returned.x() != returned.x() || returned.y() != returned.y() || returned.z() != returned.z()) {
                    continue;
                }

                col = col + returned;
            }
            //col /= float(ns);
            col = col / float(ns);
            col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            pixels[ny-j][nx-i] = Vec3(ir, ig, ib);
            //std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }


    // Show the window
    ShowWindow(hwnd, nCmdShow);

    // Enter the message loop
     MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}
