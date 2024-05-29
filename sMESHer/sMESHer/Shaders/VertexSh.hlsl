cbuffer ConstBuf0 : register(b0)
{
    matrix BUF0_view;
    matrix BUF0_projection;
};

cbuffer ConstBuf1 : register(b1)
{
    matrix BUF1_model;
    matrix BUF1_modelTR;
    float4 BUF1_color;
};

struct VSOut
{
    float3 VS_Normal : Normal;
    float2 VS_UV : UV;
    float4 VS_color : Colory;
    float3 VS_pixelPosWS : PixelPosWS;
    float3 VS_pixelPosVS : PixelPosVS;
    float3 VS_pixelPosSS : PixelPosSS;
    float4 VS_Pos : SV_Position;
};

VSOut main(float3 v_pos : Position, float3 v_normal : Normal, float2 v_UV : UV, float4 v_Color : Color)
{
    VSOut ret;
    //ret.VS_Pos = mul(BUF0_projection, mul(BUF0_view, mul(BUF1_model, float4(v_pos.x, v_pos.y, v_pos.z, 1.0f))));
    ret.VS_Pos = mul(BUF0_projection, mul(BUF0_view, mul(BUF1_model, float4(v_pos.x, v_pos.y, v_pos.z, 1.0f))));
    //ret.VS_Pos = float4(v_pos.x, v_pos.y, v_pos.z, 1.0f);
    //ret.VS_Normal = mul(BUF1_modelTR, float4(v_normal, 0.0f));
    ret.VS_Normal = mul((float3x3) BUF1_modelTR, v_normal);
    ret.VS_UV = v_UV;
    ret.VS_color = BUF1_color;
    ret.VS_pixelPosWS = mul(BUF1_model, float4(v_pos.xyz, 1.0f));
    ret.VS_pixelPosVS = mul(BUF0_view, mul(BUF1_model, float4(v_pos.x, v_pos.y, v_pos.z, 1.0f)));
    ret.VS_pixelPosSS = mul(BUF0_projection, mul(BUF0_view, float4(ret.VS_pixelPosWS, 1.0f)));
    return ret;
}