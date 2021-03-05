layout(location = 0) out vec4 fragColor;

layout(binding = 0) uniform sampler2DArray texArray;

in vec3 geoTex;
in flat int geoPicked;

void main()
{
	fragColor = texture(texArray, geoTex);
	if (fragColor.a <= 0)
	{
		discard;
	}

	if (geoPicked == 1)
	{
		fragColor = mix(fragColor, vec4(1.0, 1.0, 1.0, 1.0), 0.4);
	}
}
