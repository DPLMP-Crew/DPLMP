// NIGHT TIME NORMAL MAPPED CHARACTER
// Fix handedness and broken tangents - lazy duchess

sampler g_samplerDiffuse : register(s0);
sampler g_samplerNormalMap : register(s1);
sampler s_samplerProjector : register(s3);
float4 s_LightAmbientDirection : register(c2);
float4 s_LightDirectionalDirection : register(c5);
float4 s_LightAmbientDirectionIntensity : register(c3);
float4 s_LightDirectionalColour : register(c4);
float4 s_LightInstanceAmbientColour : register(c1);
float4 g_LightInstanceDirection : register(c7);
float4 g_LightInstanceColour : register(c8);
float4 g_fAlpha : register(c6);
float4 g_ProjectedLightDir : register(c10);
float4 g_ProjectedLightColour : register(c9);
float4 s_FogColour : register(c0);
float4 main (
    float2 texcoord0 : TEXCOORD0,
    float4 texcoord2 : TEXCOORD2,
    float4 texcoord3 : TEXCOORD3,
    float3 texcoord4 : TEXCOORD4,
    float3 texcoord6 : TEXCOORD6,
    float3 texcoord7 : TEXCOORD7,
    float1 color0 : COLOR0
) : COLOR
{
    float4 constant11 = {2,-1,1,0.5};
    float4 constant12 = {0,0,0,0};
    float4 register0 = {0,0,0,0};
    float4 register1 = {0,0,0,0};
    float4 register2 = {0,0,0,0};
    float4 register3 = {0,0,0,0};
    float4 register4 = {0,0,0,0};
    float4 register5 = {0,0,0,0};
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
    
    register0 = tex2D(g_samplerNormalMap, texcoord0);
    register1 = tex2D(g_samplerDiffuse, texcoord0);
    register2 = tex2Dproj(s_samplerProjector, texcoord2);
    register3.xy = (constant11.x * register0.wzyx) + constant11.y;
	register3.y *= handedness;
    register0.w = saturate(register3.r * -register3.r + register3.g * -register3.g + constant11.z);
    register0.w = rsqrt(register0.w);
    register3.z = rcp(register0.w);
    register0.xyz = normalize(register3);
    register3.xyz = normalize(texcoord7);
    register4.xyz = register0.y * register3;
    register3.xyz = normalize(texcoord6);
    register3.xyz = (register0.x * register3) + register4;
    register4.xyz = (register0.z * texcoord4) + register3;
    register3.x = dot(register4.xyz,-s_LightAmbientDirection.xyz);
    register0.x = saturate(dot(register4.xyz,-s_LightDirectionalDirection.xyz));
    register0.y = constant11.z;
    register0.w = (s_LightAmbientDirectionIntensity.x * register3.x) + register0.y;
    register0.xyz = register0.x * s_LightDirectionalColour;
    register0.xyz = (s_LightInstanceAmbientColour * register0.w) + register0;
    register5.x = saturate(dot(register4.xyz,-g_LightInstanceDirection.xyz));
    register3.xyz = normalize(register4);
    register0.xyz = (register5.x * g_LightInstanceColour) + register0;
    register0.w = g_fAlpha.x;
    register0 = register1 * register0;
    register4.x = saturate(dot(g_ProjectedLightDir.xyz,register3.xyz));
    register3.x = saturate(texcoord3.z);
    register3.y = saturate(texcoord3.w);
    register2.xyz = saturate(register2 + -register3.y);
    register1.w = -register3.x + constant11.w;
    if (register1.w >= 0)
    {
        register2.xyz = register2;
    }
    else
    {
        register2.xyz = constant12.x;
    }
    register2.xyz = register2 * g_ProjectedLightColour;
    register2.xyz = register2 + register2;
    register2.xyz = register4.x * register2;
    register1.xyz = (register2 * register1) + register0;
    register0.xyz = lerp(s_FogColour, register1, color0.x);
    returnValue0 = register0;
    return returnValue0;
}
