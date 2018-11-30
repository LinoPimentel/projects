
Shader "Custom/Sprites Palette"
{
	Properties
	{
		[PerRendererData] _MainTex("Sprite Texture", 2D) = "white" {}
		[PerRendererData] _Palette("Palette", 2D) = "white" {}
		[PerRendererData] _Dimensions("Dimensions", Vector) = (0,0,0,1)
	}

	SubShader
	{
		Tags
		{
			"Queue" = "Transparent"
			"IgnoreProjector" = "True"
			"RenderType" = "Transparent"
		}

		Cull Off
		Lighting Off
		ZWrite Off
		Blend One OneMinusSrcAlpha

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			struct appdata_t
			{
				float4 vertex   : POSITION;
				float2 texcoord : TEXCOORD0;
				float4 color : COLOR;
			};

			struct v2f
			{
				float4 vertex   : SV_POSITION;
				half2 texcoord  : TEXCOORD0;
				float4 color : COLOR;
			};

			float4 _MainTex_TexelSize;

			v2f vert(appdata_t IN)
			{
				v2f OUT;
				OUT.vertex = UnityObjectToClipPos(IN.vertex);
				OUT.texcoord = IN.texcoord;
				OUT.color = IN.color;
				return OUT;
			}

			sampler2D _MainTex;
			sampler2D _Palette;
			float2 _Dimensions;

			float4 ColorFromPalette(fixed4 color, float alpha)
			{			
				float4 alphaC = float4(1, 1, 1, alpha);
				float4 palColor = tex2D(_Palette, float2(color.r, color.g)) * alphaC;
				palColor.rgb *= alphaC.a;
				return palColor;
			}


			fixed4 frag(v2f IN) : SV_Target
			{
				float2 texelSize = (1 / _Dimensions.xy) * 0.48;
				float2 uv_pixels = IN.texcoord * _Dimensions.xy;
				float4 uv_min_max = float4((floor(uv_pixels) - texelSize) / _Dimensions.xy, (ceil(uv_pixels) - texelSize) / _Dimensions.xy);
				
				float2 uv_frac = frac(uv_pixels);

				float4 texelA = ColorFromPalette(tex2D(_MainTex, uv_min_max.xy), tex2D(_MainTex, uv_min_max.xy).b);
				float4 texelB = ColorFromPalette(tex2D(_MainTex, uv_min_max.xw), tex2D(_MainTex, uv_min_max.xw).b);
				float4 texelC = ColorFromPalette(tex2D(_MainTex, uv_min_max.zy), tex2D(_MainTex, uv_min_max.zy).b);
				float4 texelD = ColorFromPalette(tex2D(_MainTex, uv_min_max.zw), tex2D(_MainTex, uv_min_max.zw).b);

				float4 bilinear = lerp(lerp(texelA, texelB, uv_frac.y), lerp(texelC, texelD, uv_frac.y), uv_frac.x) * IN.color;
				bilinear.rgb *= IN.color.a;
				return bilinear;
				//return ColorFromPalette(tex2D(_MainTex, IN.texcoord), tex2D(_MainTex, IN.texcoord).b);
			}
			ENDCG
		}
	}
}