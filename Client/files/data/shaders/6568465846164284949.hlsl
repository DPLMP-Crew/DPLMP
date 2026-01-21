sampler g_samplerDiffuse : register(s0);
sampler g_samplerNormalMap : register(s1);
float4 s_LightAmbientDirection : register(c2);
float4 s_LightDirectionalDirection : register(c5);
float4 g_LightInstanceDirection : register(c7);
float4 s_LightAmbientDirectionIntensity : register(c3);
float4 s_LightDirectionalColour : register(c4);
float4 s_LightInstanceAmbientColour : register(c1);
float4 g_LightInstanceColour : register(c8);
float4 g_fAlpha : register(c6);
float4 s_FogColour : register(c0);
float4 main (
    float2 texcoord0 : TEXCOORD0,
    float3 texcoord4 : TEXCOORD4,
    float3 texcoord6 : TEXCOORD6,
    float3 texcoord7 : TEXCOORD7,
    float1 texcoord2 : TEXCOORD2
) : COLOR
{
    float4 constant9 = {2,-1,1,0};
    float4 register1 = {0,0,0,0};
    float4 register0 = {0,0,0,0};
    float4 register2 = {0,0,0,0};
    float4 register3 = {0,0,0,0};
    float4 register4 = {0,0,0,0};
    float4 returnValue0 = {0,0,0,0};
	
	// Detect broken tangents and fix
	if (texcoord7.y >= 0.6)
	{
		texcoord7.x = -texcoord7.x;
		texcoord7.y = -texcoord7.y;
		texcoord7.z = -texcoord7.z;
		
		texcoord6.x = -texcoord6.x;
		texcoord6.y = -texcoord6.y;
		texcoord6.z = -texcoord6.z;
	}
	
	float handedness = sign(dot(cross(texcoord4, texcoord6), texcoord7));

    register1 = tex2D(g_samplerNormalMap, texcoord0);
    register0 = tex2D(g_samplerDiffuse, texcoord0);
    register2.xy = (constant9.x * register1.wzyx) + constant9.y;
	register2.y *= handedness;
    register1.w = saturate(register2.r * -register2.r + register2.g * -register2.g + constant9.z);
    register1.w = rsqrt(register1.w);
    register2.z = rcp(register1.w);
    register1.xyz = normalize(register2);
    register2.xyz = normalize(texcoord7);
    register3.xyz = register1.y * register2;
    register2.xyz = normalize(texcoord6);
    register2.xyz = (register1.x * register2) + register3;
    register1.xyz = (register1.z * texcoord4) + register2;
    register4.x = dot(register1.xyz,-s_LightAmbientDirection.xyz);
    register3.x = saturate(dot(register1.xyz,-s_LightDirectionalDirection.xyz));
    register2.x = saturate(dot(register1.xyz,-g_LightInstanceDirection.xyz));
    register1.y = constant9.z;
    register1.w = (s_LightAmbientDirectionIntensity.x * register4.x) + register1.y;
    register1.xyz = register3.x * s_LightDirectionalColour;
    register1.xyz = (s_LightInstanceAmbientColour * register1.w) + register1;
    register1.xyz = (register2.x * g_LightInstanceColour) + register1;
    register2.x = g_fAlpha.x;
    register0.xyz = (register0 * register1) + -s_FogColour;
    register0.w = register0.w * register2.x;
    register0.xyz = (texcoord2.x * register0) + s_FogColour;
    returnValue0 = register0;
    return returnValue0;
}
