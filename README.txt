Assignment 6 -- Spinning Lights & Shaders

Summary:

For this assignment, we are to implement more control schemes 
	in order to spin two light sources around the screen, 
	implement Gouraud vertex shading, and alter the Blinn-Phong 
	fragment shader with the function specified in the prompt.
	
	rotateUp, rotateLeft are working just as intended, both
	lights are going up, down, left and right both teapots.
	
	The roll method seems to be doing nothing when pressed.	
		
	For the vertex shader, the diffuse lighting is working
	just "close" to satisfactory, illuminating the teapot B 
	and showing its brass/goldish colors. 
	
	However, there seems to be a circular gray spot on both
	top and bottom parts of the teapot. 	
	
	Specular lighting is not working as supposed to. There 
	is reflection occuring on the teapot, but the shading does
	not look natural, showing rectangular shadow edges. In addition, 
	the specular light shows too much yellow when the lights 
	are shone against it, as well as too much blue/green-ish.
	
	On the brighter side, the altered Blinn-Phong fragment shader
	is working. When the lights are shone against Teapot A, the
	shading looks as natural as it gets, especially compared to
	the mess on Teapot B. The reflections are showing perfectly,
	with no indication (it seems) of any strangeness happening 
	on the surfaces (no gray patches on the top and bottom). 			

	Although, I am not entirely sure if the color of Teapot A
	is correct, since it's entirely gold, and seemed to have 
	lost the blue/green-ish tint it originally had. But once
	again, when both lights are moved, its shades are changing
	propery, as well as the light being reflected back. 

	To run the program, a makefile is already provided so all
	is needed is to type in the make command in the terminal. 
	
	Control Scheme:

	'-' & '+'     : decrease and increase the angle in degrees
	'r'	      : re-initialize the camera to its default position
	'LEFT-ARROW'  : rotate the camera towards the right
	'RIGHT-ARROW' : rotate the camera towards the left
	'UP-ARROW'    : rotate the camera downwards
	'DOWN-ARROW'  : rotate the camera upwards
	'w'	      : rotate light 0 up
	's'	      : rotate light 0 down
	'a'	      : rotate light 0 left
	'd'	      : rotate light 0 right
	'y'	      : rotate light 1 up
	'h'	      : rotate light 1 down
	'g'	      : rotate light 1 left
	'j'	      : rotate light 1 right
	'x'	      : roll light 0 
	'n'	      : roll light 1
	'q' & 'esc'   : terminate the program.	

