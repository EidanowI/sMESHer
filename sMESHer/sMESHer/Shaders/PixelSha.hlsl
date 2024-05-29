#define POINT_LIGHT_NUM 64

struct Material
{
    float ambient;
    float diffuse;
    float specular;
    float shiness;
};
struct DirectionalLight
{
    float4 direction; //xyz - direction, w - ambient
    float4 color; //rgb - color, a - specular
};
struct PointLight
{
    float4 position; //w - specular strength
    float4 color;
    float4 params; //r - const, g - linear, b - quadratic, a - m_ambientStrength ( intensity * 1 / (c + b * distance + a * distance^2))
};
struct SpotLight
{
    float4 position; //xyz - position, w - cutStart
    float4 color; //rgb - color, a - cutEnd
    float4 direction;
};
    
/*cbuffer ConstBuf0 : register(b0)
{
    float4 BUF0_view_position; //xyz - view position, w - pow factor 
    float4 BUF0_view_direction; //xyz - view_direction, w - cutof cosine
    PointLight BUF0_pointLight[POINT_LIGHT_NUM]; //PointLight BUF0_pointLight[POINT_LIGHT_NUM];
    unsigned int BUF0_pointLigthNum;
    float tmp1;
    float tmp2;
    float tmp3;
};*/


float4 main(float3 pin_normal : Normal, float2 pin_UV : UV, float4 pin_color : Colory, float3 pin_pixel_pos_worldspace : PixelPosWS, float3 pin_pixel_pos_viewspace : PixelPosWS, float3 pin_pixel_pos_screenspace : PixelPosSS) : SV_Target //uint FaceID : SV_PrimitiveID
{
    return pin_color;
}
