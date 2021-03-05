layout(location = 1) uniform float radius = 100.0;
layout(location = 2) uniform int flat_topped = 1;
layout(location = 3) uniform vec2 pick_coord = vec2(0.0, 0.0);

layout(location = 0) in vec2 inCoords;
layout(location = 1) in int inLayer;

out float vsLayer;
out flat int vsPicked;

void main()
{
	float q = inCoords.x, r = inCoords.y;
	vec2 position = flat_topped == 1
		? vec2(
			radius * (3.0 / 2.0 * q),
			radius * (sqrt(3.0) / 2.0 * q + sqrt(3.0) * r)
		)
		: vec2(
			radius * (sqrt(3.0) * q + sqrt(3.0) / 2.0 * r),
			radius * (3.0 / 2.0 * r)
		);
	gl_Position = vec4(vec3(position, 0.0), 0.0);
	vsLayer = float(inLayer);
	vsPicked = inCoords == pick_coord ? 1 : 0;
}
