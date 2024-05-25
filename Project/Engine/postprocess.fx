#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"

struct VS_IN
{
	float3 vLocalPos : POSITION;
	float2 vUV : TEXCOORD;	
};

struct VS_OUT
{
	float4 vPosition : SV_Position;
	float2 vUV : TEXCOORD;
};

// ============================
// GrayShader
// mesh : RectMesh
// Domain : DOMAIN_POSTPROCESS
// g_tex_0 : RederTarget Copy Texture
// ============================
VS_OUT VS_GrayShader(VS_IN _in)
{
	VS_OUT output = (VS_OUT)0.f;

	output.vPosition = float4(_in.vLocalPos * 2.f, 1.f);
	output.vUV = _in.vUV;

	return output;
}

float4 PS_GrayShader(VS_OUT _in) : SV_Target
{
	float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);

	float vAver = (vColor.r + vColor.g + vColor.b) / 3.f;

	vColor = float4(vAver, vAver, vAver, 1.f);

	return vColor;
}


// ============================
// Distortion Shader
// mesh : RectMesh
// Domain : DOMAIN_POSTPROCESS
// g_tex_0 : RederTarget Copy Texture
// ============================
VS_OUT VS_Distortion(VS_IN _in)
{
	VS_OUT output = (VS_OUT)0.f;

	output.vPosition = float4(_in.vLocalPos * 2.f, 1.f);
	output.vUV = _in.vUV;

	return output;
}

float4 PS_Distortion(VS_OUT _in) : SV_Target
{
	float2 vUV = _in.vPosition.xy / g_Resolution;
		
	float fChange = cos(( (vUV.x - g_AccTime * 0.4f) / 0.15f) * 2 * 3.1415926535f) * 0.01f;
	vUV.y += fChange;
	float4 vColor = g_tex_0.Sample(g_sam_0, vUV);
	return vColor;
}

// ============================
// Distortion Shader
// mesh : RectMesh
// Domain : DOMAIN_POSTPROCESS
// g_tex_0 : RederTarget Copy Texture
// ============================
VS_OUT VS_XDistortion(VS_IN _in)
{
	VS_OUT output = (VS_OUT)0.f;

	output.vPosition = float4(_in.vLocalPos * 2.f, 1.f);
	output.vUV = _in.vUV;

	return output;
}

float4 PS_XDistortion(VS_OUT _in) : SV_Target
{
	float2 vUV = _in.vPosition.xy / g_Resolution;


	float fChange = cos(((vUV.x - g_AccTime * 0.4f) / 0.15f) * 2 * 3.1415926535f) * 0.01f;

	vUV.x += fChange;
	vUV.y += fChange;

	float4 vColor = g_tex_0.Sample(g_sam_0, vUV);
	//vColor.r *= 2.f;

	return vColor;
}

// ============================
// Purple Shader
// mesh : RectMesh
// Domain : DOMAIN_POSTPROCESS
// g_tex_0 : RederTarget Copy Texture
// ============================
VS_OUT VS_Purple(VS_IN _in)
{
	VS_OUT output = (VS_OUT)0.f;

	output.vPosition = float4(_in.vLocalPos * 2.f, 1.f);
	output.vUV = _in.vUV;

	return output;
}

float4 PS_Purple(VS_OUT _in) : SV_Target
{
	float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
	float progress = (g_AccTime - g_float_0) / 0.1f;

	vColor.x *= 1 - saturate(progress) * 0.3f;
	vColor.y *= 1 - saturate(progress) * 0.67f;
	vColor.z *= 1 - saturate(progress) * 0.37f;

	return vColor;
}

// ============================
// Dark Shader
// mesh : RectMesh
// Domain : DOMAIN_POSTPROCESS
// g_tex_0 : RederTarget Copy Texture
// ============================
VS_OUT VS_Dark(VS_IN _in)
{
	VS_OUT output = (VS_OUT)0.f;

	output.vPosition = float4(_in.vLocalPos * 2.f, 1.f);
	output.vUV = _in.vUV;

	return output;
}

float4 PS_Dark(VS_OUT _in) : SV_Target
{
	float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
	float progress = (g_AccTime - g_float_0) / 0.2f;

	if (vColor.y < 0.9f || vColor.z < 0.9f)
	{
		vColor.x *= 1.f - saturate(progress) * 0.7f;
		vColor.y *= 1.f - saturate(progress) * 0.7f;
		vColor.z *= 1.f - saturate(progress) * 0.7f;
	}

	return vColor;
}

// ============================
// Grid Shader
// mesh : RectMesh
// Domain : DOMAIN_POSTPROCESS
// g_tex_0 : RederTarget Copy Texture
// ============================
VS_OUT VS_Grid(VS_IN _in)
{
	VS_OUT output = (VS_OUT)0.f;

	output.vPosition = float4(_in.vLocalPos * 2.f, 1.f);
	output.vUV = _in.vUV;

	return output;
}

float4 PS_Grid(VS_OUT _in) : SV_Target
{
	float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
	float progress = (g_AccTime - g_float_0) / 1.f;

	vColor.x *= 1.f - saturate(progress);
	vColor.y *= 1.f - saturate(progress);
	vColor.z *= 1.f - saturate(progress);

	return vColor;
}
//
//// ============================
//// PaperBurn Shader
//// mesh : RectMesh
//// Domain : DOMAIN_POSTPROCESS
//// g_tex_0 : RederTarget Copy Texture
//// ============================
//VS_OUT VS_PaperBurn(VS_IN _in)
//{
//	VS_OUT output = (VS_OUT)0.f;
//
//	output.vPosition = float4(_in.vLocalPos * 2.f, 1.f);
//	output.vUV = _in.vUV;
//
//	return output;
//}
//
//float4 PS_PaperBurn(VS_OUT _in) : SV_Target
//{
//	float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
//	float progress = (g_AccTime - g_float_0) / 1.f;
//
//	vec2 uv = _in.vUV.xy / iResolution.xy;
//	vec4 o = texture(g_tex_1, uv);
//	float noise = 1.0 - (o.x + o.y + o.z) / 1.0;
//
//	float m = progress / 5.f;
//
//	float range = 0.1f;
//	float s = smoothstep(m - range, m + range, sqrt(pow(uv.x - 1.f, 2.f) + pow(uv.y, 2.f)));
//	float v = mix(noise, s, 0.55f);
//	fragColor = vec4(1.f, 1.f, 1.f, 1.f) * step(0.5f, v);
//
//	float cut = (1.f - step(0.5f, v) * step(-0.55f, -v));
//	fragColor.y = fragColor.y * cut;
//	fragColor.z = fragColor.z * cut;
//
//	return vColor;
//}



#endif