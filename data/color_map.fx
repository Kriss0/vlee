float flash = 0;
float fade  = 1;
float blend = 0;
float2 noffs = float2(0, 0);

texture bloom;
sampler bloom_sampler = sampler_state {
	Texture = (bloom);
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
	sRGBTexture = FALSE;
};

texture tex;
sampler tex_sampler = sampler_state {
	Texture = (tex);
	MipFilter = NONE;
	MinFilter = POINT;
	MagFilter = POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
	sRGBTexture = FALSE;
};

texture noise_tex;
sampler noise = sampler_state {
	Texture = (noise_tex);
	MipFilter = NONE;
	MinFilter = POINT;
	MagFilter = POINT;
	AddressU = WRAP;
	AddressV = WRAP;
	sRGBTexture = FALSE;
};

struct VS_OUTPUT {
	float4 pos  : POSITION;
	float2 tex  : TEXCOORD1;
};

VS_OUTPUT vertex(float4 ipos : POSITION, float2 tex  : TEXCOORD0)
{
	VS_OUTPUT Out;
	Out.pos = ipos;
	Out.tex = tex;
	return Out;
}

float luminance(float3 color)
{
	return color.r * 0.299 +
	       color.g * 0.587 +
	       color.b * 0.114;
}

float4 pixel(VS_OUTPUT In) : COLOR
{
	float4 color = tex2D(tex_sampler, In.tex);
	color += pow(tex2D(bloom_sampler, In.tex) * 0.75, 1.5);
	float n = tex2D(noise, In.tex * 15 + noffs).r;
	color.rgb -= (n - 0.5) * 0.01;
	return color * fade + flash;
}

technique color_map {
	pass P0 {
		VertexShader = compile vs_2_0 vertex();
		PixelShader  = compile ps_2_0 pixel();
	}
}

float3 rgbe_to_rgb(float4 rgbe)
{
	return rgbe.rgb * exp2(rgbe.a * 255 - 128);
}

float4 pixel_rgbe(VS_OUTPUT In) : COLOR
{
	float3 color = rgbe_to_rgb(tex2D(tex_sampler, In.tex));
//	color += pow(tex2D(bloom_sampler, In.tex) * 0.75, 1.5);
	float n = tex2D(noise, In.tex * 15 + noffs).r;
	color.rgb -= (n - 0.5) * 0.01;
	return float4(color * fade + flash, 1);
}

technique rgbe {
	pass P0 {
		VertexShader = compile vs_2_0 vertex();
		PixelShader  = compile ps_2_0 pixel_rgbe();
	}
}