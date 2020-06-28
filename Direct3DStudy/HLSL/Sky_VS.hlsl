#include "Basic.hlsli"

VertexPosHL VS(VertexPos vIn)
{
    VertexPosHL vOut;
    matrix viewProj = mul(g_View, g_Proj);
    float4 posW = mul(float4(vIn.PosL, 1.0f), g_World);
    vOut.PosH = mul(posW, viewProj).xyww;
    // ����z = wʹ��z/w = 1(��պб�����Զƽ��)
    vOut.PosL = vIn.PosL;
    return vOut;

    //VertexPosHL vOut;

    //// ����z = wʹ��z/w = 1(��պб�����Զƽ��)
    //float4 posH = mul(float4(vIn.PosL, 1.0f), g_WorldViewProj);
    //vOut.PosH = posH.xyww;
    //vOut.PosL = vIn.PosL;
    //return vOut;
}