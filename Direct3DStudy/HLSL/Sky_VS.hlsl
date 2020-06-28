#include "Basic.hlsli"

VertexPosHL VS(VertexPos vIn)
{
    VertexPosHL vOut;
    matrix viewProj = mul(g_View, g_Proj);
    float4 posW = mul(float4(vIn.PosL, 1.0f), g_World);
    vOut.PosH = mul(posW, viewProj).xyww;
    // 设置z = w使得z/w = 1(天空盒保持在远平面)
    vOut.PosL = vIn.PosL;
    return vOut;

    //VertexPosHL vOut;

    //// 设置z = w使得z/w = 1(天空盒保持在远平面)
    //float4 posH = mul(float4(vIn.PosL, 1.0f), g_WorldViewProj);
    //vOut.PosH = posH.xyww;
    //vOut.PosL = vIn.PosL;
    //return vOut;
}