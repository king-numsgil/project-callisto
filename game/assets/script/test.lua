i2 = -i32vec2.new(3, 6)
print(i2:x())

f2 = f32vec2.new(1, 2) + f32vec2.new(3, 4) - f32vec2.new(10, 10) * -1
print(f2:product())

print(f64mat4.rotation(f64rad.new(f64deg.new(30)), f64vec3.new(0, 1, 0)))
print(f32dquat.rotation(f32rad.new(f32deg.new(30)), f32vec3.new(0, 1, 0)) * f32dquat.translation(f32vec3.new(10, 0, 0)))
