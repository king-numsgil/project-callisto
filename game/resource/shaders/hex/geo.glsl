layout(location = 0) uniform mat3 transform = mat3(1.0);
layout(location = 1) uniform float radius = 100.0;
layout(location = 2) uniform int flat_topped = 1;

layout(points) in;
layout(triangle_strip, max_vertices = 12) out;

in float vsLayer[];

out vec3 geoTex;

#define PI 3.141592654
#define TAU 6.283185307

void addPoint(int index)
{
	vec2 center = gl_in[0].gl_Position.xy;
	float layer = vsLayer[0];
	float increment = TAU / 6.0;

	float angle = index * increment;
	vec2 sincos = flat_topped == 1
		? vec2(cos(angle), sin(angle))
		: vec2(cos(angle - PI / 6.0), sin(angle - PI / 6.0));

	gl_Position.xywz = vec4(transform * vec3(sincos.x * radius + center.x, sincos.y * radius + center.y, 1.0), 0.0);
	geoTex = vec3(sincos.x * 0.5 + 0.5, sincos.y * 0.5 + 0.5, layer);
	EmitVertex();
}

void main()
{
	addPoint(0);
	addPoint(1);
	addPoint(2);
	EndPrimitive();

	addPoint(0);
	addPoint(2);
	addPoint(3);
	EndPrimitive();

	addPoint(0);
	addPoint(3);
	addPoint(4);
	EndPrimitive();

	addPoint(0);
	addPoint(4);
	addPoint(5);
	EndPrimitive();
}
