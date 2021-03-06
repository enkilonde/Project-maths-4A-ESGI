
#ifndef GL_ES
  #version 110
#endif


attribute vec2 a_Position;
attribute vec4 a_Color;

varying vec4 v_Color;

uniform float u_Time;

void main(void)
{
  v_Color = a_Color;

	gl_Position = vec4(a_Position.x * cos(u_Time), a_Position.y * sin(u_Time), 0.0, 1.0);

}
