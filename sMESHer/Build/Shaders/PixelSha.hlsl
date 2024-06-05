struct DirectionalLight
{
    float4 direction; //xyz - direction
    float4 color; //rgb - color, a - directionalLightSpecular
};
struct PointLight
{
    float4 position; //w - specular strength
    float4 color;
    float4 params; //r - const, g - linear, b - quadratic,
};
    
cbuffer ConstBuf0 : register(b0)
{
    float4 BUF0_view_position; //xyz - view position, w - pow factor 
    float4 BUF0_view_direction; //xyz - view_direction, w - ambient
    PointLight BUF0_pointLight;
    DirectionalLight BUF0_directionLight;
};

float3 CalculateDirectionalLight(const DirectionalLight directLight, const float3 normNormalWS, const float3 pixelPosWS);
float3 CalculatePointLight(const PointLight pointLight, const float3 normNormalWS, const float3 pixelPosWS);

float4 main(float3 pin_normal : Normal, float2 pin_UV : UV, float4 pin_color : Colory, float3 pin_pixel_pos_worldspace : PixelPosWS, float3 pin_pixel_pos_viewspace : PixelPosWS, float3 pin_pixel_pos_screenspace : PixelPosSS) : SV_Target //uint FaceID : SV_PrimitiveID
{
    float3 normalNormalized = normalize(pin_normal);

    float3 light = BUF0_view_direction.w;
    light += CalculateDirectionalLight(BUF0_directionLight, normalNormalized, pin_pixel_pos_worldspace);
    
    light += CalculatePointLight(BUF0_pointLight, normalNormalized, pin_pixel_pos_worldspace);
    
    light = clamp(light, float3(0.0f, 0.0f, 0.0f), float3(1.0f, 1.0f, 1.0f));
    
    return pin_color * float4(light, 1.0f);
}

float3 CalculateDirectionalLight(const DirectionalLight directLight, const float3 normNormalWS, const float3 pixelPosWS)
{
    float3 toLightDir = normalize(-directLight.direction.xyz);
    float3 reflectFromLightDir = reflect(directLight.direction.xyz, normNormalWS);
    float diffuse = max(dot(normNormalWS, toLightDir), 0.0f);
    float specular = pow(max(dot(toLightDir, reflectFromLightDir), 0.0f), BUF0_view_position.w) * directLight.color.a;
    
    return ((diffuse * (1.0f - BUF0_view_direction.w)) + specular) * directLight.color.rgb;

}
float3 CalculatePointLight(const PointLight pointLight, const float3 normNormalWS, const float3 pixelPosWS)
{
    float3 toLightDir = normalize(pointLight.position.xyz - pixelPosWS);
    float3 reflectFromLightDir = reflect(-toLightDir, normNormalWS);
    
    float distance = length(pointLight.position.xyz - pixelPosWS);
    float attenuation = 1.0f / (pointLight.params.r +
    (pointLight.params.g * distance) +
    (pointLight.params.b * distance * distance));

    float diffuse = max(dot(normNormalWS, toLightDir), 0.0f);
    float specular = pow(max(dot(toLightDir, reflectFromLightDir), 0.0f), BUF0_view_position.w) * pointLight.position.w;
    
    return (attenuation * (diffuse)) * pointLight.color.rgb;
}