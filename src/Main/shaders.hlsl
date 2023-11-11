#define IDENTITY_MATRIX float4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)
float4x4 Transpose(float4x4 m)
{
    return float4x4
    (
        m[0][0],m[1][0],m[2][0],m[3][0],
        m[0][1],m[1][1],m[2][1],m[3][1],
        m[0][2],m[1][2],m[2][2],m[3][2],
        m[0][3],m[1][3],m[2][3],m[3][3]
    );
}
cbuffer constants : register(b0)
{
    float4x4 modelViewProj;
};
struct VS_Input
{
    float3 vertexPosition : SV_POSITION;
};
struct PS_Input
{
    float4 screenSpace : SV_POSITION;
    float3 color : COLOR;
};
PS_Input vs_main(VS_Input input)
{
    PS_Input output;

    float4 vertexPosition = float4(input.vertexPosition, 1.0f);
    output.screenSpace = mul(vertexPosition, modelViewProj);

    output.color = input.vertexPosition;
    output.color += float3(0.5f, 0.5f, 0.5f);
    output.color = abs(output.color);

    return output;
}
float4 ps_main(PS_Input input) : SV_Target
{
    return float4(input.color, 1.0);
}
