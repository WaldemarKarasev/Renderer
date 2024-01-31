#pragma once

#include <stdint.h>
#include <string>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

// Vertices and Indices

float squareVertices[8 * 24] = {
    //    position             normal            UV                  index

    // FRONT
    -1.0f, -1.f, -1.f,    -1.f,  0.f,  0.f,     0.f, 0.f,              // 0
    -1.0f,  1.f, -1.f,    -1.f,  0.f,  0.f,     1.f, 0.f,              // 1
    -1.0f,  1.f,  1.f,    -1.f,  0.f,  0.f,     1.f, 1.f,              // 2
    -1.0f, -1.f,  1.f,    -1.f,  0.f,  0.f,     0.f, 1.f,              // 3

    // BACK                                  
    1.0f, -1.f, -1.f,     1.f,  0.f,  0.f,     1.f, 0.f,              // 4
    1.0f,  1.f, -1.f,     1.f,  0.f,  0.f,     0.f, 0.f,              // 5
    1.0f,  1.f,  1.f,     1.f,  0.f,  0.f,     0.f, 1.f,              // 6
    1.0f, -1.f,  1.f,     1.f,  0.f,  0.f,     1.f, 1.f,              // 7

    // RIGHT
    -1.0f,  1.f, -1.f,    0.f,  1.f,  0.f,     0.f, 0.f,              // 8
    1.0f,  1.f, -1.f,     0.f,  1.f,  0.f,     1.f, 0.f,              // 9
    1.0f,  1.f,  1.f,     0.f,  1.f,  0.f,     1.f, 1.f,              // 10
    -1.0f,  1.f,  1.f,    0.f,  1.f,  0.f,     0.f, 1.f,              // 11

    // LEFT
    -1.0f, -1.f, -1.f,    0.f, -1.f,  0.f,     1.f, 0.f,              // 12
    1.0f, -1.f, -1.f,     0.f, -1.f,  0.f,     0.f, 0.f,              // 13
    1.0f, -1.f,  1.f,     0.f, -1.f,  0.f,     0.f, 1.f,              // 14
    -1.0f, -1.f,  1.f,    0.f, -1.f,  0.f,     1.f, 1.f,              // 15

    // TOP
    -1.0f, -1.f,  1.f,    0.f,  0.f,  1.f,     0.f, 0.f,              // 16
    -1.0f,  1.f,  1.f,    0.f,  0.f,  1.f,     1.f, 0.f,              // 17
    1.0f,  1.f,  1.f,     0.f,  0.f,  1.f,     1.f, 1.f,              // 18
    1.0f, -1.f,  1.f,     0.f,  0.f,  1.f,     0.f, 1.f,              // 19

    // BOTTOM
    -1.0f, -1.f, -1.f,   0.f,  0.f, -1.f,     0.f, 1.f,              // 20
    -1.0f,  1.f, -1.f,   0.f,  0.f, -1.f,     1.f, 1.f,              // 21
    1.0f,  1.f, -1.f,    0.f,  0.f, -1.f,     1.f, 0.f,              // 22
    1.0f, -1.f, -1.f,    0.f,  0.f, -1.f,     0.f, 0.f,              // 23
};	

uint32_t squareIndices[6 * 6] = { 	
    0,   1,  2,  2,  3,  0, // front
    4,   5,  6,  6,  7,  4, // back
    8,   9, 10, 10, 11,  8, // right
    12, 13, 14, 14, 15, 12, // left
    16, 17, 18, 18, 19, 16, // top
    20, 21, 22, 22, 23, 20  // bottom
};


// Shaders 
std::string blueShaderVertexSrc = R"(
    #version 450
    layout(location = 0) in vec3 vertex_position;
    layout(location = 1) in vec3 vertex_normal;
    layout(location = 2) in vec2 texture_coord;

    layout(location = 0) out vec2 tex_coord_smile;
    layout(location = 1) out vec2 tex_coord_quads;
    layout(location = 2) out vec3 frag_position;
    layout(location = 3) out vec3 frag_normal;

    layout(std140, binding = 0) uniform CameraData
    {
        mat4 model_matrix;
        mat4 view_projection_matrix;
        int current_frame;
    } u_data;

    //mat4 model_matrix;
    //uniform mat4 view_projection_matrix;
    //uniform int current_frame;

    void main() {
        tex_coord_smile = texture_coord;
        tex_coord_quads = texture_coord + vec2(u_data.current_frame / 1000.f, u_data.current_frame / 1000.f);
        frag_normal = mat3(transpose(inverse(u_data.model_matrix))) * vertex_normal;
        vec4 vertex_position_world = u_data.model_matrix * vec4(vertex_position, 1.0);
        frag_position = vertex_position_world.xyz;
        gl_Position = u_data.view_projection_matrix * vertex_position_world;
    }
)";

std::string blueShaderFragmentSrc = R"(
    #version 450
    //in vec3 color;
    layout(location = 0) in vec2 tex_coord_smile;
    layout(location = 1) in vec2 tex_coord_quads;
    layout(location = 2) in vec3 frag_position;
    layout(location = 3) in vec3 frag_normal;

    layout(binding = 0) uniform sampler2D InTexture_Smile;
    layout(binding = 1) uniform sampler2D InTexture_Quads;

    layout(std140, binding = 1) uniform LightData
    {
        vec3 camera_position;
        vec3 light_position;
        vec3 light_color;
        float ambient_factor;
        float diffuse_factor;
        float specular_factor;
        float shininess;
    }u_data;

    //uniform vec3 camera_position;
    //uniform vec3 light_position;
    //uniform vec3 light_color;
    //uniform float ambient_factor;
    //uniform float diffuse_factor;
    //uniform float specular_factor;
    //uniform float shininess;

    layout(location = 0) out vec4 frag_color;

    void main() {
                    
        // ambient
        vec3 ambient = u_data.ambient_factor * u_data.light_color;
        // diffuse
        vec3 normal = normalize(frag_normal);
        vec3 light_dir = normalize(u_data.light_position - frag_position);
        vec3 diffuse = u_data.diffuse_factor * u_data.light_color * max(dot(normal, light_dir), 0.0);

        // specular
        vec3 view_dir = normalize(u_data.camera_position - frag_position);
        vec3 reflect_dir = reflect(-light_dir, normal);
        float specular_value = pow(max(dot(view_dir, reflect_dir), 0.0), u_data.shininess);
        vec3 specular = u_data.specular_factor * specular_value * u_data.light_color;

        //frag_color = vec4(color, 1.0);
        //frag_color = texture(InTexture_Smile, tex_coord_smile);// * texture(InTexture_Quads, tex_coord_quads);
        //frag_color = texture(InTexture_Smile, tex_coord_smile) * vec4(ambient + diffuse + specular, 1.f);
        frag_color = vec4(ambient + diffuse + specular, 1.f);

    }
)";



std::string light_source_vertex_shader =
R"(#version 450
    layout(location = 0) in vec3 vertex_position;
    layout(location = 1) in vec3 vertex_normal;
    layout(location = 2) in vec2 texture_coord;
    
    layout(std140, binding = 2) uniform CamData
    {
        mat4 model_matrix;
        mat4 view_projection_matrix;
    }u_data;

    //uniform mat4 model_matrix;
    //uniform mat4 view_projection_matrix;
    void main() {
        gl_Position = u_data.view_projection_matrix * u_data.model_matrix * vec4(vertex_position * 0.1f, 1.0);
    }
)";

std::string light_source_fragment_shader =
R"(#version 450
    layout(location = 0) out vec4 frag_color;
    layout(std140, binding = 3) uniform LightCol
    {
        vec3 light_color;
    } u_data;
    //uniform vec3 light_color;
    void main() {
        frag_color = vec4(u_data.light_color, 1.f);
    }
)";


namespace tmp_detail {
	// for blueShader ubo's
	struct BlueCameraData
	{
		glm::mat4 model_matrix;
		glm::mat4 view_projection_matrix;
		int current_frame;
	};

	struct BlueLightData 
	{
		glm::vec3 camera_position;
		float padding0;
		glm::vec3 light_position;
		float padding1;
		glm::vec3 light_color;
		//float c; 
		float ambient_factor = 0.1f;
		float diffuse_factor = 1.0f;
		float specular_factor = 0.5f;
		float shininess = 32.f;
	};

	// for lightShader ubo's
	struct LightCameraData
	{
		glm::mat4 model_matrix;
		glm::mat4 view_projection_matrix;
	};

	struct LightColor
	{
		glm::vec3 light_color;
	};	
}












std::string VulkanVertexShader = R"(
    #version 450

    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 color;
    layout(location = 2) in vec3 normal;
    layout(location = 3) in vec2 uv;

    layout(location = 0) out vec3 fragColor;
    layout(location = 1) out vec3 fragPosWorld;
    layout(location = 2) out vec3 fragNormalWorld;

    struct PointLight {
    vec4 position; // ignore w
    vec4 color; // w is intensity
    };

    layout(set = 0, binding = 0) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    mat4 invView;
    vec4 ambientLightColor; // w is intensity
    PointLight pointLights[10];
    int numLights;
    } ubo;

    layout(set = 1, binding = 0) uniform GlobalUbo1 {
    mat4 projection;
    mat4 view;
    mat4 invView;
    vec4 ambientLightColor; // w is intensity
    PointLight pointLights[10];
    int numLights;
    } ubo1;

    layout(push_constant) uniform Push {
    mat4 modelMatrix;
    mat4 normalMatrix;
    } push;

    void main() {
    vec4 positionWorld = push.modelMatrix * vec4(position, 1.0);
    gl_Position = ubo.projection * ubo.view * positionWorld;
    fragNormalWorld = normalize(mat3(push.normalMatrix) * normal);
    fragPosWorld = positionWorld.xyz;
    fragColor = color;
    }

)";



std::string VulkanFragmentShader = R"(
    #version 450

    layout (location = 0) in vec3 fragColor;
    layout (location = 1) in vec3 fragPosWorld;
    layout (location = 2) in vec3 fragNormalWorld;

    layout (location = 0) out vec4 outColor;

    struct PointLight {
    vec4 position; // ignore w
    vec4 color; // w is intensity
    };

    layout(set = 0, binding = 0) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    mat4 invView;
    vec4 ambientLightColor; // w is intensity
    PointLight pointLights[10];
    int numLights;
    } ubo;

    //layout(set = 1, binding = 0) uniform GlobalUbo1 {
    //  mat4 projection;
    //  mat4 view;
    //  mat4 invView;
    //  vec4 ambientLightColor; // w is intensity
    //  PointLight pointLights[10];
    //  int numLights;
    //} ubo1;

    layout(push_constant) uniform Push {
    mat4 modelMatrix;
    mat4 normalMatrix;
    } push;

    void main() {
    vec3 diffuseLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
    vec3 specularLight = vec3(0.0);
    vec3 surfaceNormal = normalize(fragNormalWorld);

    vec3 cameraPosWorld = ubo.invView[3].xyz;
    vec3 viewDirection = normalize(cameraPosWorld - fragPosWorld);

    for (int i = 0; i < ubo.numLights; i++) {
        PointLight light = ubo.pointLights[i];
        vec3 directionToLight = light.position.xyz - fragPosWorld;
        float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared
        directionToLight = normalize(directionToLight);

        float cosAngIncidence = max(dot(surfaceNormal, directionToLight), 0);
        vec3 intensity = light.color.xyz * light.color.w * attenuation;

        diffuseLight += intensity * cosAngIncidence;

        // specular lighting
        vec3 halfAngle = normalize(directionToLight + viewDirection);
        float blinnTerm = dot(surfaceNormal, halfAngle);
        blinnTerm = clamp(blinnTerm, 0, 1);
        blinnTerm = pow(blinnTerm, 100.0); // higher values -> sharper highlight
        specularLight += intensity * blinnTerm;
    }
    
    outColor = vec4(diffuseLight * fragColor + specularLight * fragColor, 1.0);
    }

)";


std::string VulkanPointLightVertexShader = R"(
    #version 450

    const vec2 OFFSETS[6] = vec2[](
    vec2(-1.0, -1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, -1.0),
    vec2(1.0, -1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, 1.0)
    );

    layout (location = 0) out vec2 fragOffset;

    struct PointLight {
    vec4 position; // ignore w
    vec4 color; // w is intensity
    };

    layout(set = 0, binding = 0) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    mat4 invView;
    vec4 ambientLightColor; // w is intensity
    PointLight pointLights[10];
    int numLights;
    } ubo;

    layout(push_constant) uniform Push {
    vec4 position;
    vec4 color;
    float radius;
    } push;


    void main() {
    fragOffset = OFFSETS[gl_VertexIndex];
    vec3 cameraRightWorld = {ubo.view[0][0], ubo.view[1][0], ubo.view[2][0]};
    vec3 cameraUpWorld = {ubo.view[0][1], ubo.view[1][1], ubo.view[2][1]};

    vec3 positionWorld = push.position.xyz
        + push.radius * fragOffset.x * cameraRightWorld
        + push.radius * fragOffset.y * cameraUpWorld;

    gl_Position = ubo.projection * ubo.view * vec4(positionWorld, 1.0);
    }
)";

std::string VulkanPointLightFragmentFragmentShader = R"(
    #version 450

    layout (location = 0) in vec2 fragOffset;
    layout (location = 0) out vec4 outColor;

    struct PointLight {
    vec4 position; // ignore w
    vec4 color; // w is intensity
    };

    layout(set = 0, binding = 0) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    mat4 invView;
    vec4 ambientLightColor; // w is intensity
    PointLight pointLights[10];
    int numLights;
    } ubo;

    layout(push_constant) uniform Push {
    vec4 position;
    vec4 color;
    float radius;
    } push;

    const float M_PI = 3.1415926538;

    void main() {
    float dis = sqrt(dot(fragOffset, fragOffset));
    if (dis >= 1.0) {
    discard;
    }

    float cosDis = 0.5 * (cos(dis * M_PI) + 1.0); // ranges from 1 -> 0
    outColor = vec4(push.color.xyz + 0.5 * cosDis, cosDis);
    }

)";
