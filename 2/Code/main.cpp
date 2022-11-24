/*
 * @Description: 
 * @Author: Fishermanykx
 * @Date: 2022-01-03 12:39:01
 * @LastEditors: Fishermanykx
 * @LastEditTime: 2022-01-03 16:40:03
 */
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"
#include "Light.hpp"
#include "Renderer.hpp"

// In the main function of the program, we create the scene (create objects and lights)
// as well as set the options for the render (image width and height, maximum recursion
// depth, field-of-view, etc.). We then call the render function().
int main()
{
    // 定义场景
    Scene scene(1280, 960);

    // 球1
    auto sph1 = std::make_unique<Sphere>(Vector3f(-1, 0, -12), 2);
    // 球1材质
    sph1->materialType = DIFFUSE_AND_GLOSSY;
    sph1->diffuseColor = Vector3f(0.2, 0.7, 0.8);

    // 球2
    auto sph2 = std::make_unique<Sphere>(Vector3f(0.5, -0.5, -8), 1.5);
    sph2->ior = 1.5;
    sph2->materialType = REFLECTION_AND_REFRACTION;

    // 球3
    auto sph3 = std::make_unique<Sphere>(Vector3f(1.5, 0, -5), 1);
    sph3->materialType = DIFFUSE_AND_GLOSSY;
    sph3->diffuseColor = Vector3f(0.2, 0.7, 0.6);

    // sph3->ior = 1.54;

    // 添加到场景
    scene.Add(std::move(sph1));
    scene.Add(std::move(sph2));
    scene.Add(std::move(sph3));

    // 四个顶点
    Vector3f verts[4] = {{-5,-3,-6}, {5,-3,-6}, {5,-3,-16}, {-5,-3,-16}};
    uint32_t vertIndex[6] = {0, 1, 3, 1, 2, 3};
    Vector2f st[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    // 建立三角网格
    auto mesh = std::make_unique<MeshTriangle>(verts, vertIndex, 2, st);
    mesh->materialType = DIFFUSE_AND_GLOSSY;
    // 添加到场景
    scene.Add(std::move(mesh));

    // 在场景中添加光源
    scene.Add(std::make_unique<Light>(Vector3f(-20, 70, 20), 0.5));
    scene.Add(std::make_unique<Light>(Vector3f(30, 50, -12), 0.5));    

    // 执行渲染
    Renderer r;
    r.Render(scene);

    return 0;
}