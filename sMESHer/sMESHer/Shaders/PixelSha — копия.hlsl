
float4 main(float3 pin_normal : Normal, float2 pin_UV : UV, float4 pin_color : Colory, float3 pin_pixel_pos_worldspace : PixelPosWS, float3 pin_pixel_pos_viewspace : PixelPosWS, float3 pin_pixel_pos_screenspace : PixelPosSS) : SV_Target //uint FaceID : SV_PrimitiveID
{
    return float4(2.0f, 2.0f, 0.0f, 1.0f);
}
