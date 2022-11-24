#include "Renderer.hpp"

#include <fstream>
#include <optional>

#include "Scene.hpp"
#include "Vector.hpp"

inline float deg2rad(const float &deg) { return deg * M_PI / 180.0; }

// Compute reflection direction
Vector3f reflect(const Vector3f &I, const Vector3f &N) {
  Vector3f L = -1 * I;
  Vector3f T = L - 2 * dotProduct(L, N) * N;
  return -1 * T;
}

// [comment]
// Compute refraction direction using Snell's law
//
// We need to handle with care the two possible situations:
//
//    - When the ray is inside the object
//
//    - When the ray is outside.
//
// If the ray is outside, you need to make cosi positive cosi = -N.I
//
// If the ray is inside, you need to invert the refractive indices and negate
// the normal N
// [/comment]
Vector3f refract(const Vector3f &I, const Vector3f &N, const float &ior) {
  float cosi = clamp(-1, 1, dotProduct(I, N));
  float etai = 1, etat = ior;
  Vector3f n = N;
  if (cosi < 0) {
    cosi = -cosi;
  } else {
    std::swap(etai, etat);
    n = -N;
  }
  float eta = etai / etat;
  float k = 1 - eta * eta * (1 - cosi * cosi);
  return k < 0 ? 0 : eta * I + (eta * cosi - sqrtf(k)) * n;
}

// [comment]
// Compute Fresnel equation
//
// \param I is the incident view direction
//
// \param N is the normal at the intersection point
//
// \param ior is the material refractive index
// [/comment]
float fresnel(const Vector3f &I, const Vector3f &N, const float &ior) {
  float cosi = clamp(-1, 1, dotProduct(I, N));
  float etai = 1, etat = ior;
  if (cosi > 0) {
    std::swap(etai, etat);
  }
  // Compute sini using Snell's law
  float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
  // Total internal reflection
  if (sint >= 1) {
    return 1;
  } else {
    float cost = sqrtf(std::max(0.f, 1 - sint * sint));
    cosi = fabsf(cosi);
    float Rs =
        ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
    float Rp =
        ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
    return (Rs * Rs + Rp * Rp) / 2;
  }
  // As a consequence of the conservation of energy, transmittance is given by:
  // kt = 1 - kr;
}

// [comment]
// Returns true if the ray intersects an object, false otherwise.
//
// \param orig is the ray origin
// \param dir is the ray direction
// \param objects is the list of objects the scene contains
// \param[out] tNear contains the distance to the cloesest intersected object.
// \param[out] index stores the index of the intersect triangle if the
// interesected object is a mesh. \param[out] uv stores the u and v barycentric
// coordinates of the intersected point \param[out] *hitObject stores the
// pointer to the intersected object (used to retrieve material information,
// etc.) \param isShadowRay is it a shadow ray. We can return from the function
// sooner as soon as we have found a hit.
// [/comment]

std::optional<hit_payload> trace(
    const Vector3f &orig, const Vector3f &dir,
    const std::vector<std::unique_ptr<Object> > &objects) {
  float tNear = kInfinity;
  std::optional<hit_payload> payload;
  for (const auto &object : objects) {
    float tNearK = kInfinity;
    uint32_t indexK;
    Vector2f uvK;
    if (object->intersect(orig, dir, tNearK, indexK, uvK) && tNearK < tNear) {
      payload.emplace();
      payload->hit_obj = object.get();
      payload->tNear = tNearK;
      payload->index = indexK;
      payload->uv = uvK;
      tNear = tNearK;
    }
  }

  return payload;
}

// [comment]
// Implementation of the Whitted-style light transport algorithm (E [S*] (D|G)
// L)
//
// This function is the function that compute the color at the intersection
// point of a ray defined by a position and a direction. Note that thus function
// is recursive (it calls itself).
//
// If the material of the intersected object is either reflective or reflective
// and refractive, then we compute the reflection/refraction direction and cast
// two new rays into the scene by calling the castRay() function recursively.
// When the surface is transparent, we mix the reflection and refraction color
// using the result of the fresnel equations (it computes the amount of
// reflection and refraction depending on the surface normal, incident view
// direction and surface refractive index).
//
// If the surface is diffuse/glossy we use the Phong illumation model to compute
// the color at the intersection point.
// [/comment]
Vector3f castRay(const Vector3f &orig, const Vector3f &dir, const Scene &scene,
                 int depth) {
  // 递归深度
  if (depth > scene.maxDepth) {
    return Vector3f(0.0, 0.0, 0.0);
  }

  Vector3f hitColor = scene.backgroundColor;

  // 光线和场景所有物体的最近交点
  if (auto payload = trace(orig, dir, scene.get_objects()); payload) {
    // 交点
    Vector3f hitPoint = orig + dir * payload->tNear;
    Vector3f N;   // normal
    Vector2f st;  // st coordinates
    payload->hit_obj->getSurfaceProperties(hitPoint, dir, payload->index,
                                           payload->uv, N, st);
    switch (payload->hit_obj->materialType) {
      // 材质表面只存在反射
      case REFLECTION: {
        // 计算菲涅尔方程，获取反射光线的能量占比
        float kr = fresnel(dir, N, payload->hit_obj->ior);
        // 得到反射方向
        Vector3f reflectionDirection = normalize(reflect(dir, N));
        // 得到反射光线起点：注意起点在物体内部还是外部很重要
        Vector3f reflectionRayOrig = (dotProduct(reflectionDirection, N) < 0)
                                         ? hitPoint + N * scene.epsilon
                                         : hitPoint - N * scene.epsilon;
        // 递归投射光线
        hitColor =
            castRay(reflectionRayOrig, reflectionDirection, scene, depth + 1) *
            kr;
        break;
      }
      // 材质表面既有反射,又有折射,比如透明/半透明材质
      case REFLECTION_AND_REFRACTION: {
        // 反射方向
        Vector3f reflectionDirection = normalize(reflect(dir, N));
        // 折射方向
        Vector3f refractionDirection =
            normalize(refract(dir, N, payload->hit_obj->ior));
        // 反射光线起点
        Vector3f reflectionRayOrig = (dotProduct(reflectionDirection, N) < 0)
                                         ? hitPoint - N * scene.epsilon
                                         : hitPoint + N * scene.epsilon;
        // 折射光线起点
        Vector3f refractionRayOrig = (dotProduct(refractionDirection, N) < 0)
                                         ? hitPoint - N * scene.epsilon
                                         : hitPoint + N * scene.epsilon;

        // 反射光线递归调用castRay获取追踪结果  TODO
        Vector3f reflectionColor =
            castRay(reflectionRayOrig, reflectionDirection, scene, depth + 1);

        // 折射光线递归调用castRay获取追踪结果  TODO
        Vector3f refractionColor =
            castRay(refractionRayOrig, refractionDirection, scene, depth + 1);

        // 结合菲涅尔定律: 反射和折射光各自的能量占多少用kr表示
        float kr = fresnel(dir, N, payload->hit_obj->ior);
        hitColor = reflectionColor * kr + refractionColor * (1 - kr);
        break;
      }

      // 默认使用Phong光照模型: diffuse + specular
      default: {
        // [comment]
        // We use the Phong illumation model int the default case. The phong
        // model is composed of a diffuse and a specular reflection component.
        // [/comment]

        Vector3f lightAmt = 0, lightSpecular = 0;

        Vector3f shadowPointOrig = (dotProduct(dir, N) < 0)
                                       ? hitPoint + N * scene.epsilon
                                       : hitPoint - N * scene.epsilon;
        // [comment]
        // Loop over all lights in the scene and sum their contribution up
        // We also apply the lambert cosine law
        // [/comment]

        // 本场景中有不止一个光源
        for (auto &light : scene.get_lights()) {
          Vector3f lightDir = light->position - hitPoint;
          // square of the distance between hitPoint and the light
          float lightDistance2 = dotProduct(lightDir, lightDir);
          lightDir = normalize(lightDir);
          float LdotN = std::max(0.f, dotProduct(lightDir, N));

          // 判断交点是否在阴影里:
          // 从光源出发,向lightDir的反方向发出一条光线,如果最近的交点比hitPoint更近,
          // 说明在阴影中
          auto shadow_res =
              trace(shadowPointOrig, lightDir, scene.get_objects());
          bool inShadow = shadow_res && (shadow_res->tNear * shadow_res->tNear <
                                         lightDistance2);

          // 漫反射
          lightAmt += inShadow ? 0 : light->intensity * LdotN;

          Vector3f reflectionDirection = reflect(-lightDir, N);

          // 高光
          lightSpecular +=
              powf(std::max(0.f, -dotProduct(reflectionDirection, dir)),
                   payload->hit_obj->specularExponent) *
              light->intensity;
        }

        // Phong model:  Kd * diffuseColor * Id + Ks * specularColor * Is

        // 漫反射部分
        float Kd = payload->hit_obj->Kd;
        Vector3f diffuseColor = payload->hit_obj->evalDiffuseColor(st);
        Vector3f Id = lightAmt;

        // 光泽反射部分
        // 由于反射光一般情况下就是光源颜色,颜色不变,所以这里没有一个evalSpecularColor;
        // 请注意:对于金属来说,不同金属对不同波长的光反射率不同,
        // 具有不同的specularColor
        float Ks = payload->hit_obj->Ks;
        Vector3f Is = lightSpecular;
        // Vector3f specularColor(1.0)

        // 安装Phong模型公式，补充完整
        hitColor = Kd * diffuseColor * Id + Ks * Is;
        break;
      }
    }
  }

  return hitColor;
}

// [comment]
// The main render function. This where we iterate over all pixels in the image,
// generate primary rays and cast these rays into the scene. The content of the
// framebuffer is saved to a file.
// [/comment]
void Renderer::Render(const Scene &scene) {
  std::vector<Vector3f> framebuffer(scene.width * scene.height);

  float scale = std::tan(deg2rad(scene.fov * 0.5f));
  float imageAspectRatio = scene.width / (float)scene.height;

  std::cout << scale << ", " << imageAspectRatio << std::endl;

  // Use this variable as the eye position to start your rays.
  Vector3f eye_pos(0);
  int m = 0;
  for (int j = 0; j < scene.height; ++j) {
    for (int i = 0; i < scene.width; ++i) {
      // generate primary ray direction
      float x;
      float y;
      // 光线的生成
      x = ((i + 0.5) * 2 - scene.width) / scene.width * imageAspectRatio *
          tan(scene.fov * 0.5);
      y = (scene.height - (j + 0.5) * 2) / scene.height * tan(scene.fov * 0.5);
      Vector3f dir = normalize(
          Vector3f(x, y, -1));  // Don't forget to normalize this direction!
      framebuffer[m++] = castRay(eye_pos, dir, scene, 0);
    }
    UpdateProgress(j / (float)scene.height);
  }

  // 保存结果， ppm文件在ubuntu上可以查看，在window上需要转换格式
  FILE *fp = fopen("binary.ppm", "wb");
  (void)fprintf(fp, "P6\n%d %d\n255\n", scene.width, scene.height);
  for (auto i = 0; i < scene.height * scene.width; ++i) {
    static unsigned char color[3];
    color[0] = (char)(255 * clamp(0, 1, framebuffer[i].x));
    color[1] = (char)(255 * clamp(0, 1, framebuffer[i].y));
    color[2] = (char)(255 * clamp(0, 1, framebuffer[i].z));
    fwrite(color, 1, 3, fp);
  }
  fclose(fp);
}