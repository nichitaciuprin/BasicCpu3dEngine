/*
            IMPORTANT!!!
    read about: #pragma pack_matrix( row_major )
    HLSL will transpose modelViewProj matrix in constant buffer
    Example with translation matrix:
        1,0,0,0,    1,0,0,1,
        0,1,0,0, => 0,1,0,2,
        0,0,1,0,    0,0,1,3,
        1,2,3,1     0,0,0,1
*/

#define IDENTITY_MATRIX float4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)
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
float4 ApplyConstantBuffer(float3 vertexPosition)
{
    return mul(modelViewProj,float4(vertexPosition, 1.0f));
}
PS_Input vs_main(VS_Input input)
{
    PS_Input output;

    output.color = input.vertexPosition;
    output.color += float3(0.5f, 0.5f, 0.5f);
    output.color = abs(output.color);

    // vertexPosition.z = -vertexPosition.z;

    // float4x4 proj = float4x4
    // (
    //     3,0,0,0,
    //     0,3,0,0,
    //     0,0,3,-2,
    //     0,0,2,1
    // );
    // float4x4 view = float4x4
    // (
    //     1,0,0,0,
    //     0,1,0,0,
    //     0,0,1,0,
    //     0,0,0,1
    // );
    // float4x4 world = float4x4
    // (
    //     1,0,0,1,
    //     0,1,0,0,
    //     0,0,1,5,
    //     0,0,0,1
    // );
    float4 vertexPosition = float4(input.vertexPosition, 1.0f);

    // float4x4 modeviewproj =
    //     mul(proj,
    //     mul(view,
    //         world));

    output.screenSpace = mul(modelViewProj,vertexPosition);
    output.screenSpace.z = -output.screenSpace.z;

    // input.vertexPosition.z -= 10;
    // output.screenSpace = float4(input.vertexPosition, 1.0f);
    return output;

    // PS_Input output;
    // output.screenSpace = ApplyConstantBuffer(input.vertexPosition);
    // output.color = input.vertexPosition;
    // output.color += float3(0.5f, 0.5f, 0.5f);
    // output.color = abs(output.color);
    // return output;
}
float4 ps_main(PS_Input input) : SV_Target
{
    return float4(input.color, 1.0);
}
