layout(location = 0) out vec4 fragColor;

layout(binding = 0) uniform sampler2DArray texArray;

in vec3 geoTex;

void main()
{
	fragColor = texture(texArray, geoTex);
	if (fragColor.a <= 0)
	{
		discard;
	}
}
