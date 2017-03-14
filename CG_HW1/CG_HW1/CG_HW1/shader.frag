varying vec3 vv3color;
uniform int channel ;
void main() {
	gl_FragColor = vec4(vv3color, 1.0);

	// normal
	if(      ( channel%4 )== 0)     gl_FragColor = vec4(vv3color, 1.0 );      
	// R channel only
	else if( ( channel%4 )== 1) 	gl_FragColor = vec4(vv3color.r, 0, 0, 1.0 );
	// G channel only
	else if( ( channel%4 )== 2) 	gl_FragColor = vec4(0, vv3color.g, 0, 1.0 );
	// B channel only
	else	                        gl_FragColor = vec4(0, 0, vv3color.b, 1.0 );
}
