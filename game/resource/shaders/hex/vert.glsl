layout(location = 1) uniform float radius = 100.0;

layout(location = 0) in vec2 inCoords;
layout(location = 1) in int inLayer;

out float vsLayer;

void main()
{
	float q = inCoords.x, r = inCoords.y;
	vec2 position = vec2(
		radius * (3.0 / 2.0 * q),
		radius * (sqrt(3.0) / 2.0 * q + sqrt(3.0) * r)
	);
	gl_Position = vec4(vec3(position, 0.0), 0.0);
	vsLayer = float(inLayer);
}
